// Includes
#include <stdio.h>
#include <string.h>

#ifdef AIDAN
#include "/Users/aidan/esp/esp-idf/components/lwip/include/apps/ping/ping_sock.h"
#endif

#ifdef SPENCER
#include "/Users/spencer/esp/esp-idf/components/lwip/include/apps/ping/ping_sock.h"
#endif

#include "cJSON.h"
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
#include "utility/api/weatherAPI.h"
#include "utility/timers/timers.h"

// External Dependencies

// Declarations
// #define ESP_WIFI_SSID "ShadowHouse5"
// #define ESP_WIFI_PASS "BigBrainers11!!"
#define ESP_WIFI_SSID "Aidan iPhone"
#define ESP_WIFI_PASS "SpreadSeal"

#define ESP_MAXIMUM_RETRY 4

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define MAX_HTTP_OUTPUT_BUFFER 2048

#define WEATHER_API_URL "http://api.weatherapi.com/v1/current.json?key=99014095ccf64eca81e155920230409&q=Orlando&days=1&aqi=no&alerts=yes"

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

static int s_retry_num = 0;

char jsonResponse[MAX_HTTP_OUTPUT_BUFFER];
static int jsonOffset = 0;

static struct weatherResponseData weatherData;

static char systemIP[16];

static bool wifiInitializationCompleted = false;

// ********************************************************************************
// Parses the JSON response received from the weather API
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
static esp_err_t parseJsonResponse(void) {
    if (jsonResponse[0] == '\0') {
        printf("Invalid JSON input\n");
        return ESP_FAIL;
    }

    cJSON* parsedResponse = cJSON_Parse(jsonResponse);
    if (parsedResponse == NULL) {
        cJSON_Delete(parsedResponse);
        return ESP_FAIL;
    }

    cJSON* location = cJSON_GetObjectItemCaseSensitive(parsedResponse, "location");
    cJSON* name = cJSON_GetObjectItemCaseSensitive(location, "name");
    cJSON* region = cJSON_GetObjectItemCaseSensitive(location, "region");
    cJSON* country = cJSON_GetObjectItemCaseSensitive(location, "country");
    cJSON* lat = cJSON_GetObjectItemCaseSensitive(location, "lat");
    cJSON* lon = cJSON_GetObjectItemCaseSensitive(location, "lon");
    cJSON* tz_id = cJSON_GetObjectItemCaseSensitive(location, "tz_id");
    cJSON* localtime_epoch = cJSON_GetObjectItemCaseSensitive(location, "localtime_epoch");
    cJSON* localtime = cJSON_GetObjectItemCaseSensitive(location, "localtime");

    cJSON* current = cJSON_GetObjectItemCaseSensitive(parsedResponse, "current");
    cJSON* last_updated_epoch = cJSON_GetObjectItemCaseSensitive(current, "last_updated_epoch");
    cJSON* last_updated = cJSON_GetObjectItemCaseSensitive(current, "last_updated");
    cJSON* temp_c = cJSON_GetObjectItemCaseSensitive(current, "temp_c");
    cJSON* temp_f = cJSON_GetObjectItemCaseSensitive(current, "temp_f");
    cJSON* is_day = cJSON_GetObjectItemCaseSensitive(current, "is_day");

    cJSON* condition = cJSON_GetObjectItemCaseSensitive(current, "condition");
    cJSON* text = cJSON_GetObjectItemCaseSensitive(condition, "text");
    cJSON* icon = cJSON_GetObjectItemCaseSensitive(condition, "icon");
    cJSON* code = cJSON_GetObjectItemCaseSensitive(condition, "code");

    cJSON* wind_mph = cJSON_GetObjectItemCaseSensitive(current, "wind_mph");
    cJSON* wind_kph = cJSON_GetObjectItemCaseSensitive(current, "wind_kph");
    cJSON* wind_degree = cJSON_GetObjectItemCaseSensitive(current, "wind_degree");
    cJSON* wind_dir = cJSON_GetObjectItemCaseSensitive(current, "wind_dir");
    cJSON* pressure_mb = cJSON_GetObjectItemCaseSensitive(current, "pressure_mb");
    cJSON* pressure_in = cJSON_GetObjectItemCaseSensitive(current, "pressure_in");
    cJSON* precip_mm = cJSON_GetObjectItemCaseSensitive(current, "precip_mm");
    cJSON* precip_in = cJSON_GetObjectItemCaseSensitive(current, "precip_in");
    cJSON* humidity = cJSON_GetObjectItemCaseSensitive(current, "humidity");
    cJSON* cloud = cJSON_GetObjectItemCaseSensitive(current, "cloud");
    cJSON* feelslike_c = cJSON_GetObjectItemCaseSensitive(current, "feelslike_c");
    cJSON* feelslike_f = cJSON_GetObjectItemCaseSensitive(current, "feelslike_f");
    cJSON* vis_km = cJSON_GetObjectItemCaseSensitive(current, "vis_km");
    cJSON* vis_miles = cJSON_GetObjectItemCaseSensitive(current, "vis_miles");
    cJSON* uv = cJSON_GetObjectItemCaseSensitive(current, "uv");
    cJSON* gust_mph = cJSON_GetObjectItemCaseSensitive(current, "gust_mph");
    cJSON* gust_kph = cJSON_GetObjectItemCaseSensitive(current, "gust_kph");

    // cJSON* forecast = cJSON_GetObjectItemCaseSensitive(parsedResponse, "forecast");
    // cJSON* forecastday = cJSON_GetObjectItemCaseSensitive(forecast, "forecastday");
    // cJSON* date = cJSON_GetObjectItemCaseSensitive(forecastday, "date");
    // cJSON* date_epoch = cJSON_GetObjectItemCaseSensitive(forecastday, "date_epoch");
    // cJSON* day = cJSON_GetObjectItemCaseSensitive(forecastday, "day");
    // cJSON* maxtemp_c = cJSON_GetObjectItemCaseSensitive(day, "maxtemp_c");
    // cJSON* maxtemp_f = cJSON_GetObjectItemCaseSensitive(day, "maxtemp_f");
    // cJSON* mintemp_c = cJSON_GetObjectItemCaseSensitive(day, "mintemp_c");
    // cJSON* mintemp_f = cJSON_GetObjectItemCaseSensitive(day, "mintemp_f");
    // cJSON* avgtemp_c = cJSON_GetObjectItemCaseSensitive(day, "avgtemp_c");
    // cJSON* avgtemp_f = cJSON_GetObjectItemCaseSensitive(day, "avgtemp_f");
    // cJSON* maxwind_mph = cJSON_GetObjectItemCaseSensitive(day, "maxwind_mph");
    // cJSON* maxwind_kph = cJSON_GetObjectItemCaseSensitive(day, "maxwind_kph");
    // cJSON* totalprecip_mm = cJSON_GetObjectItemCaseSensitive(day, "totalprecip_mm");
    // cJSON* totalprecip_in = cJSON_GetObjectItemCaseSensitive(day, "totalprecip_in");
    // cJSON* totalsnow_cm = cJSON_GetObjectItemCaseSensitive(day, "totalsnow_cm");
    // cJSON* avgvis_km = cJSON_GetObjectItemCaseSensitive(day, "avgvis_km");
    // cJSON* avgvis_miles = cJSON_GetObjectItemCaseSensitive(day, "avgvis_miles");
    // cJSON* avghumidity = cJSON_GetObjectItemCaseSensitive(day, "avghumidity");
    // cJSON* daily_will_it_rain = cJSON_GetObjectItemCaseSensitive(day, "daily_will_it_rain");
    // cJSON* daily_chance_of_rain = cJSON_GetObjectItemCaseSensitive(day, "daily_chance_of_rain");
    // cJSON* daily_will_it_snow = cJSON_GetObjectItemCaseSensitive(day, "daily_will_it_snow");
    // cJSON* daily_chance_of_snow = cJSON_GetObjectItemCaseSensitive(day, "daily_chance_of_snow");
    // cJSON* conditionForecast = cJSON_GetObjectItemCaseSensitive(day, "condition");
    // cJSON* textForecast = cJSON_GetObjectItemCaseSensitive(conditionForecast, "text");
    // cJSON* iconForecast = cJSON_GetObjectItemCaseSensitive(conditionForecast, "icon");
    // cJSON* codeForecast = cJSON_GetObjectItemCaseSensitive(conditionForecast, "code");
    // cJSON* uvForecast = cJSON_GetObjectItemCaseSensitive(day, "uv");

    // cJSON* alerts = cJSON_GetObjectItemCaseSensitive(parsedResponse, "alerts");
    // cJSON* alert = cJSON_GetObjectItemCaseSensitive(alerts, "alert");
    // cJSON* headline = cJSON_GetObjectItemCaseSensitive(alert, "headline");
    // cJSON* msgtype = cJSON_GetObjectItemCaseSensitive(alert, "msgtype");
    // cJSON* severity = cJSON_GetObjectItemCaseSensitive(alert, "severity");
    // cJSON* urgency = cJSON_GetObjectItemCaseSensitive(alert, "urgency");
    // cJSON* areas = cJSON_GetObjectItemCaseSensitive(alert, "areas");
    // cJSON* category = cJSON_GetObjectItemCaseSensitive(alert, "category");
    // cJSON* certainty = cJSON_GetObjectItemCaseSensitive(alert, "certainty");
    // cJSON* event = cJSON_GetObjectItemCaseSensitive(alert, "event");
    // cJSON* note = cJSON_GetObjectItemCaseSensitive(alert, "note");
    // cJSON* effective = cJSON_GetObjectItemCaseSensitive(alert, "effective");
    // cJSON* expires = cJSON_GetObjectItemCaseSensitive(alert, "expires");
    // cJSON* desc = cJSON_GetObjectItemCaseSensitive(alert, "desc");
    // cJSON* instruction = cJSON_GetObjectItemCaseSensitive(alert, "instruction");

    memcpy(&weatherData.locationData.name, name->valuestring, strlen(name->valuestring));
    memcpy(&weatherData.locationData.region, region->valuestring, strlen(region->valuestring));
    memcpy(&weatherData.locationData.country, country->valuestring, strlen(country->valuestring));
    weatherData.locationData.lat = lat->valueint;
    weatherData.locationData.lon = lon->valueint;
    memcpy(&weatherData.locationData.tz_id, tz_id->valuestring, strlen(tz_id->valuestring));
    memcpy(&weatherData.locationData.localtime, localtime->valuestring, strlen(localtime->valuestring));
    weatherData.locationData.localtime_epoch = localtime_epoch->valueint;

    memcpy(&weatherData.currentWeatherData.last_updated, last_updated->valuestring, strlen(last_updated->valuestring));
    weatherData.currentWeatherData.last_updated_epoch = last_updated_epoch->valueint;
    weatherData.currentWeatherData.temp_c = temp_c->valueint;
    weatherData.currentWeatherData.temp_f = temp_f->valueint;
    weatherData.currentWeatherData.is_day = is_day->valueint;
    memcpy(&weatherData.currentWeatherData.conditionData.text, text->valuestring, strlen(text->valuestring));
    memcpy(&weatherData.currentWeatherData.conditionData.icon, icon->valuestring, strlen(icon->valuestring));
    weatherData.currentWeatherData.conditionData.code = code->valueint;
    weatherData.currentWeatherData.wind_mph = wind_mph->valueint;
    weatherData.currentWeatherData.wind_kph = wind_kph->valueint;
    weatherData.currentWeatherData.wind_degree = wind_degree->valueint;
    memcpy(&weatherData.currentWeatherData.wind_dir, wind_dir->valuestring, strlen(wind_dir->valuestring));
    weatherData.currentWeatherData.pressure_mb = pressure_mb->valueint;
    weatherData.currentWeatherData.pressure_in = pressure_in->valueint;
    weatherData.currentWeatherData.precip_mm = precip_mm->valueint;
    weatherData.currentWeatherData.precip_in = precip_in->valueint;
    weatherData.currentWeatherData.humidity = humidity->valueint;
    weatherData.currentWeatherData.cloud = cloud->valueint;
    weatherData.currentWeatherData.feelslike_c = feelslike_c->valueint;
    weatherData.currentWeatherData.feelslike_f = feelslike_f->valueint;
    weatherData.currentWeatherData.vis_km = vis_km->valueint;
    weatherData.currentWeatherData.vis_miles = vis_miles->valueint;
    weatherData.currentWeatherData.uv = uv->valueint;
    weatherData.currentWeatherData.gust_mph = gust_mph->valueint;
    weatherData.currentWeatherData.gust_kph = gust_kph->valueint;

    // memcpy(&weatherData.currentForecastData.currentForecastDaily.date, date->valuestring, strlen(date->valuestring));
    // weatherData.currentForecastData.currentForecastDaily.date_epoch = date_epoch->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.maxtemp_c = maxtemp_c->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.maxtemp_f = maxtemp_f->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.mintemp_c = mintemp_c->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.mintemp_f = mintemp_f->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avgtemp_c = avgtemp_c->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avgtemp_f = avgtemp_f->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.maxwind_mph = maxwind_mph->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.maxwind_kph = maxwind_kph->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.totalprecip_mm = totalprecip_mm->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.totalprecip_in = totalprecip_in->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.totalsnow_cm = totalsnow_cm->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avgvis_km = avgvis_km->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avgvis_miles = avgvis_miles->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avghumidity = avghumidity->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.daily_will_it_rain = daily_will_it_rain->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.daily_chance_of_rain = daily_chance_of_rain->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.daily_will_it_snow = daily_will_it_snow->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.daily_chance_of_snow = daily_chance_of_snow->valueint;
    // memcpy(&weatherData.currentForecastData.currentForecastDaily.currentDayForecast.forecastCondition.text, textForecast->valuestring, strlen(textForecast->valuestring));
    // memcpy(&weatherData.currentForecastData.currentForecastDaily.currentDayForecast.forecastCondition.icon, iconForecast->valuestring, strlen(icon->valuestring));
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.forecastCondition.code = codeForecast->valueint;
    // weatherData.currentForecastData.currentForecastDaily.currentDayForecast.uv = uvForecast->valueint;

    // memcpy(&weatherData.currentAlertData.currentAlerts.headline, headline->valuestring, strlen(headline->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.msgtype, msgtype->valuestring, strlen(msgtype->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.severity, severity->valuestring, strlen(severity->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.urgency, urgency->valuestring, strlen(urgency->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.areas, areas->valuestring, strlen(areas->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.category, category->valuestring, strlen(category->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.certainty, certainty->valuestring, strlen(certainty->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.event, event->valuestring, strlen(event->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.note, note->valuestring, strlen(note->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.effective, effective->valuestring, strlen(effective->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.expires, expires->valuestring, strlen(expires->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.desc, desc->valuestring, strlen(desc->valuestring));
    // memcpy(&weatherData.currentAlertData.currentAlerts.instruction, instruction->valuestring, strlen(instruction->valuestring));

    cJSON_Delete(parsedResponse);

    return ESP_OK;
}

// ********************************************************************************
// Clears the weather data
// ********************************************************************************
static void clearWeatherData(void) {
    memset(&weatherData, 0, sizeof(weatherData));
}

#ifdef DEBUG
// ********************************************************************************
// Prints the JSON response in a formatted output
// @param json: The JSON response to print
// ********************************************************************************
void printJsonFormatted(const char* json) {
    if (json == NULL || json[0] == '\0') {
        printf("Invalid JSON input\n");
        return;
    }

    int indentLevel = 0;
    int jsonLength = strlen(json);

    for (int i = 0; i < jsonLength; i++) {
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
    }

    putchar('\n');
}
#endif

// ********************************************************************************
// Sets up the WiFi connection to the access point and handles the WiFi events for
// the status of the connection
// @param arg: The argument passed to the event handler
// @param event_base: The event base passed to the event handler
// @param event_id: The event ID passed to the event handler
// @param event_data: The event data passed to the event handler
// ********************************************************************************
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            printf("retry to connect to the AP\n");
        } else {
            xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            memset(jsonResponse, 0, sizeof(jsonResponse));
            clearWeatherData();
        }
        printf("connect to the AP fail\n");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
#ifdef DEBUF
        printf("got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        printf("\n");
#endif
        sprintf(systemIP, IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

// ********************************************************************************
// Handles the HTTP events for the status of the connection
// @param evt: The event passed to the event handler
// @return: ESP_OK if successful, ESP_FAIL if unsuccessful
// ********************************************************************************
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

// ********************************************************************************
// Initializes the WiFi event group and network interface
// ********************************************************************************
static void wifiGroupInit(void) {
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
}

// ********************************************************************************
// Initializes the WiFi connection to the access point
// ********************************************************************************
esp_err_t wifi_init(void) {
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
        memset(jsonResponse, 0, sizeof(jsonResponse));
        clearWeatherData();
        return ESP_FAIL;
    } else {
        printf("UNEXPECTED EVENT\n");
        return ESP_FAIL;
    }
}

// ********************************************************************************
// Sets up the HTTP client configuration and performs the HTTP GET request to an
// external API
// ********************************************************************************
static void http_rest_with_url(void) {
    // Configures the HTTP client and sets the URL and event handler
    esp_http_client_config_t config = {
        .url = WEATHER_API_URL,
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
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        printf("Bits in http fail: %ld\n", xEventGroupGetBits(s_wifi_event_group));
    }

    esp_http_client_cleanup(client);
}

// ********************************************************************************
// Gets the current status for the WiFi connection
// ********************************************************************************
EventBits_t getWifiStatus(void) {
    return xEventGroupGetBits(s_wifi_event_group);
}

// ********************************************************************************
// Gets the current condition in the weatherAPI response
// ********************************************************************************
void getWeatherCondition(char* pCondition) {
    memcpy(pCondition, &weatherData.currentWeatherData.conditionData.text, sizeof(weatherData.currentWeatherData.conditionData.text));
}

// ********************************************************************************
// Gets the current wind speed (MPH) in the weatherAPI response
// ********************************************************************************
int getWeatherWindSpeedMPH(void) {
    return weatherData.currentWeatherData.wind_mph;
}

// ********************************************************************************
// ********************************************************************************
void weatherApiTask(void* pvParameter) {
    while (1) {
        if (wifiInitializationCompleted == false) {
            wifiGroupInit();
            wifi_init();
            wifiInitializationCompleted = true;
#ifdef DEBUG
            printf("WiFi initialization completed\n");
#endif
        } else {
            EventBits_t bits = getWifiStatus();

            if (bits & WIFI_CONNECTED_BIT) {
                http_rest_with_url();

                parseJsonResponse();

                if (weatherData.locationData.localtime_epoch != 0 && getCurrentKnownEpochTime() == 0) {
                    setCurrentKnownEpochTime(weatherData.locationData.localtime_epoch);
                }

#ifdef DEMO
                printf("\n\n*******************************************\n");
                printf("Location: %s, %s, %s\n", weatherData.locationData.name, weatherData.locationData.region, weatherData.locationData.country);
                printf("Latitude: %d\n", weatherData.locationData.lat);
                printf("Longitude: %d\n", weatherData.locationData.lon);
                printf("Timezone ID: %s\n", weatherData.locationData.tz_id);
                printf("Local Time: %s\n", weatherData.locationData.localtime);
                printf("Local Time Epoch: %d\n", weatherData.locationData.localtime_epoch);
                printf("Last Updated: %s\n", weatherData.currentWeatherData.last_updated);
                printf("Last Updated Epoch: %d\n", weatherData.currentWeatherData.last_updated_epoch);
                printf("Temperature (C): %d\n", weatherData.currentWeatherData.temp_c);
                printf("Temperature (F): %d\n", weatherData.currentWeatherData.temp_f);
                printf("Is Day: %d\n", weatherData.currentWeatherData.is_day);
                printf("Condition: %s\n", weatherData.currentWeatherData.conditionData.text);
                printf("Condition Icon: %s\n", weatherData.currentWeatherData.conditionData.icon);
                printf("Condition Code: %d\n", weatherData.currentWeatherData.conditionData.code);
                printf("Wind Speed (mph): %d\n", weatherData.currentWeatherData.wind_mph);
                printf("Wind Speed (kph): %d\n", weatherData.currentWeatherData.wind_kph);
                printf("Wind Degree: %d\n", weatherData.currentWeatherData.wind_degree);
                printf("Wind Direction: %s\n", weatherData.currentWeatherData.wind_dir);
                printf("Pressure (mb): %d\n", weatherData.currentWeatherData.pressure_mb);
                printf("Pressure (in): %d\n", weatherData.currentWeatherData.pressure_in);
                printf("Precipitation (mm): %d\n", weatherData.currentWeatherData.precip_mm);
                printf("Precipitation (in): %d\n", weatherData.currentWeatherData.precip_in);
                printf("Humidity: %d\n", weatherData.currentWeatherData.humidity);
                printf("Cloud: %d\n", weatherData.currentWeatherData.cloud);
                printf("Feels Like (C): %d\n", weatherData.currentWeatherData.feelslike_c);
                printf("Feels Like (F): %d\n", weatherData.currentWeatherData.feelslike_f);
                printf("Visibility (km): %d\n", weatherData.currentWeatherData.vis_km);
                printf("Visibility (miles): %d\n", weatherData.currentWeatherData.vis_miles);
                printf("UV: %d\n", weatherData.currentWeatherData.uv);
                printf("Gust Speed (mph): %d\n", weatherData.currentWeatherData.gust_mph);
                printf("Gust Speed (kph): %d\n", weatherData.currentWeatherData.gust_kph);
                // printf("Max Temperature (C): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.maxtemp_c);
                // printf("Max Temperature (F): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.maxtemp_f);
                // printf("Min Temperature (C): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.mintemp_c);
                // printf("Min Temperature (F): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.mintemp_f);
                // printf("Average Temperature (C): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avgtemp_c);
                // printf("Average Temperature (F): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avgtemp_f);
                // printf("Max Wind Speed (mph): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.maxwind_mph);
                // printf("Max Wind Speed (kph): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.maxwind_kph);
                // printf("Total Precipitation (mm): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.totalprecip_mm);
                // printf("Total Precipitation (in): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.totalprecip_in);
                // printf("Total Snow (cm): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.totalsnow_cm);
                // printf("Average Visibility (km): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avgvis_km);
                // printf("Average Visibility (miles): %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avgvis_miles);
                // printf("Average Humidity: %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.avghumidity);
                // printf("Daily Will It Rain: %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.daily_will_it_rain);
                // printf("Daily Chance of Rain: %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.daily_chance_of_rain);
                // printf("Daily Will It Snow: %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.daily_will_it_snow);
                // printf("Daily Chance of Snow: %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.daily_chance_of_snow);
                // printf("Forecast Condition: %s\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.forecastCondition.text);
                // printf("Forecast Condition Icon: %s\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.forecastCondition.icon);
                // printf("Forecast Condition Code: %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.forecastCondition.code);
                // printf("Forecast UV: %d\n", weatherData.currentForecastData.currentForecastDaily.currentDayForecast.uv);
                // printf("Alert Headline: %s\n", weatherData.currentAlertData.currentAlerts.headline);
                // printf("Alert Message Type: %s\n", weatherData.currentAlertData.currentAlerts.msgtype);
                // printf("Alert Severity: %s\n", weatherData.currentAlertData.currentAlerts.severity);
                // printf("Alert Urgency: %s\n", weatherData.currentAlertData.currentAlerts.urgency);
                // printf("Alert Areas: %s\n", weatherData.currentAlertData.currentAlerts.areas);
                // printf("Alert Category: %s\n", weatherData.currentAlertData.currentAlerts.category);
                // printf("Alert Certainty: %s\n", weatherData.currentAlertData.currentAlerts.certainty);
                // printf("Alert Event: %s\n", weatherData.currentAlertData.currentAlerts.event);
                // printf("Alert Note: %s\n", weatherData.currentAlertData.currentAlerts.note);
                // printf("Alert Effective: %s\n", weatherData.currentAlertData.currentAlerts.effective);
                // printf("Alert Expires: %s\n", weatherData.currentAlertData.currentAlerts.expires);
                // printf("Alert Description: %s\n", weatherData.currentAlertData.currentAlerts.desc);
                // printf("Alert Instruction: %s\n", weatherData.currentAlertData.currentAlerts.instruction);
                printf("*******************************************\n\n");
#endif
            } else if (bits & WIFI_FAIL_BIT) {
                esp_wifi_connect();
            } else {
            }

#ifdef DEBUG
            printf("WiFi initialization From Failure\n");
#endif
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}