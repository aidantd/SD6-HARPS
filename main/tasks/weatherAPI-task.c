// Includes
#include <stdio.h>
#include <string.h>

#include "/Users/aidan/esp/esp-idf/components/lwip/include/apps/ping/ping_sock.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_tls.h"
#include "esp_tls_errors.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "utility/weatherAPI.h"

// External Dependencies

// Declarations
#define ESP_WIFI_SSID "ShadowHouse5"
#define ESP_WIFI_PASS "BigBrainers11!!"
#define ESP_MAXIMUM_RETRY 4

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define MAX_HTTP_OUTPUT_BUFFER 2048

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

static int s_retry_num = 0;

char jsonResponse[MAX_HTTP_OUTPUT_BUFFER];
static int jsonOffset = 0;
struct weatherResponseData weatherData;

void printJsonFormatted(const char* json) {
    if (json == NULL || json[0] == '\0') {
        printf("Invalid JSON input\n");
        return;
    }

    int indentLevel = 0;
    int len = strlen(json);
    char lastChar = '\0';

    for (int i = 0; i < len; i++) {
        char currentChar = json[i];

        if (currentChar == '{' || currentChar == '[') {
            putchar(currentChar);
            putchar('\n');
            indentLevel++;
            for (int j = 0; j < indentLevel; j++) {
                putchar('\t');
            }
        } else if (currentChar == '}' || currentChar == ']') {
            putchar('\n');
            indentLevel--;
            for (int j = 0; j < indentLevel; j++) {
                putchar('\t');
            }
            putchar(currentChar);
        } else if (currentChar == ',') {
            putchar(currentChar);
            putchar('\n');
            for (int j = 0; j < indentLevel; j++) {
                putchar('\t');
            }
        } else {
            putchar(currentChar);
        }

        lastChar = currentChar;
    }

    putchar('\n');
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            printf("retry to connect to the AP\n");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        printf("connect to the AP fail\n");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        printf("got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        printf("\n");
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t _http_event_handler(esp_http_client_event_t* evt) {
    switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
        break;
    case HTTP_EVENT_ON_CONNECTED:
        break;
    case HTTP_EVENT_HEADER_SENT:
#ifdef DEBUG
        printf("HTTP_EVENT_HEADER_SENT\n");
#endif
        memset(jsonResponse, 0, MAX_HTTP_OUTPUT_BUFFER);
        jsonOffset = 0;
        break;
    case HTTP_EVENT_ON_HEADER:
#ifdef DEBUG
        printf("HTTP_EVENT_ON_HEADER, key=%s, value=%s\n", evt->header_key, evt->header_value);
#endif
        break;
    case HTTP_EVENT_ON_DATA:
        memcpy(jsonResponse + jsonOffset, evt->data, evt->data_len);
        jsonOffset += evt->data_len;

#ifdef DEBUG
        printf("HTTP_EVENT_ON_DATA, len=%d\n", evt->data_len);
        printf("%.*s", evt->data_len, (char*)evt->data);
        printf("jsonOffset: %d\n", jsonOffset);
        printf("jsonResponse: %s\n", jsonResponse);
        printJsonFormatted(jsonResponse);
#endif

        break;
    case HTTP_EVENT_ON_FINISH:
#ifdef DEBUG
        printf("HTTP_EVENT_ON_FINISH\n");
#endif
        break;
    case HTTP_EVENT_DISCONNECTED:
#ifdef DEBUG
        printf("HTTP_EVENT_DISCONNECTED\n");
#endif
        int mbedtls_err = 0;
        esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
        if (err != 0) {
#ifdef DEBUG
            printf("Last esp error code: 0x%x\n", err);
            printf("Last mbedtls failure: 0x%x\n", mbedtls_err);
#endif
        }
        break;
    case HTTP_EVENT_REDIRECT:
#ifdef DEBUG
        printf("HTTP_EVENT_REDIRECT\n");
#endif
        esp_http_client_set_header(evt->client, "From", "user@example.com");
        esp_http_client_set_header(evt->client, "Accept", "text/html");
        esp_http_client_set_redirection(evt->client);
        break;
    }
    return ESP_OK;
}

esp_err_t wifi_init(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

#ifdef DEBUG
    printf("ESP_WIFI_MODE_STA\n");
#endif

    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = ESP_WIFI_SSID,
            .password = ESP_WIFI_PASS,
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            .threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_UNSPECIFIED,
            .sae_h2e_identifier = "\0",
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        printf("connected to ap SSID:%s password:%s\n", ESP_WIFI_SSID, ESP_WIFI_PASS);
        return ESP_OK;
    } else if (bits & WIFI_FAIL_BIT) {
        printf("Failed to connect to SSID:%s, password:%s\n", ESP_WIFI_SSID, ESP_WIFI_PASS);
        return ESP_FAIL;
    } else {
        printf("UNEXPECTED EVENT\n");
        return ESP_FAIL;
    }
}

static void http_rest_with_url(void) {
    esp_http_client_config_t config = {
        .url = "http://api.weatherapi.com/v1/current.json?key=99014095ccf64eca81e155920230409&q=Orlando&aqi=no",
        .event_handler = _http_event_handler,
        .user_data = NULL,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET Request
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        printf("HTTP GET Status = %d, content_length = %lld\n", esp_http_client_get_status_code(client), esp_http_client_get_content_length(client));
    } else {
        printf("HTTP GET request failed: %s\n", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

void weatherApiTask(void* pvParameter) {
    while (1) {
        http_rest_with_url();

        // #ifdef DEBUG
        printf("\n\n*******************************************\n");
        printJsonFormatted(jsonResponse);
        printf("*******************************************\n\n");
        // #endif

        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}