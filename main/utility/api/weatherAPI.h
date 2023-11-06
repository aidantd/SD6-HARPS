// Includes
#include <stdio.h>

// External Dependencies

// Declarations
#define MAX_FIELD_LENGTH 256

// The below structs are used to store the data from weatherAPI.com
// We do not request all the data the weatherAPI.com can send us, because of this the below structs
// only hold the date we want

struct condition {
    char text[MAX_FIELD_LENGTH];
    char icon[MAX_FIELD_LENGTH];
    int code;
};

struct location {
    char name[MAX_FIELD_LENGTH];
    char region[MAX_FIELD_LENGTH];
    char country[MAX_FIELD_LENGTH];
    int lat;
    int lon;
    char tz_id[MAX_FIELD_LENGTH];
    int localtime_epoch;
    char localtime[MAX_FIELD_LENGTH];
};

struct currentWeather {
    int last_updated_epoch;
    char last_updated[MAX_FIELD_LENGTH];
    int temp_c;
    int temp_f;
    int is_day;
    struct condition conditionData;
    int wind_mph;
    int wind_kph;
    int wind_degree;
    char wind_dir[MAX_FIELD_LENGTH];
    int pressure_mb;
    int pressure_in;
    int precip_mm;
    int precip_in;
    int humidity;
    int cloud;
    int feelslike_c;
    int feelslike_f;
    int vis_km;
    int vis_miles;
    int uv;
    int gust_mph;
    int gust_kph;
};

struct dayForecast {
    int maxtemp_c;
    int maxtemp_f;
    int mintemp_c;
    int mintemp_f;
    int avgtemp_c;
    int avgtemp_f;
    int maxwind_mph;
    int maxwind_kph;
    int totalprecip_mm;
    int totalprecip_in;
    int totalsnow_cm;
    int avgvis_km;
    int avgvis_miles;
    int avghumidity;
    int daily_will_it_rain;
    int daily_chance_of_rain;
    int daily_will_it_snow;
    int daily_chance_of_snow;
    struct condition forecastCondition;
    int uv;
};

struct forecastDaily {
    char date[MAX_FIELD_LENGTH];
    int date_epoch;
    struct dayForecast currentDayForecast;
};

struct forecastWeather {
    struct forecastDaily currentForecastDaily;
};

struct alert {
    char headline[MAX_FIELD_LENGTH];
    char msgtype[MAX_FIELD_LENGTH];
    char severity[MAX_FIELD_LENGTH];
    char urgency[MAX_FIELD_LENGTH];
    char areas[MAX_FIELD_LENGTH];
    char category[MAX_FIELD_LENGTH];
    char certainty[MAX_FIELD_LENGTH];
    char event[MAX_FIELD_LENGTH];
    char note[MAX_FIELD_LENGTH];
    char effective[MAX_FIELD_LENGTH];
    char expires[MAX_FIELD_LENGTH];
    char desc[MAX_FIELD_LENGTH];
    char instruction[MAX_FIELD_LENGTH];
};

struct weatherAlerts {
    struct alert currentAlerts;
};
struct weatherResponseData {
    struct location locationData;
    struct currentWeather currentWeatherData;
    struct forecastWeather currentForecastData;
    struct weatherAlerts currentAlertData;
};