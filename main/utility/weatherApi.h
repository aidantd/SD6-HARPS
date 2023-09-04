// Includes
#include <stdio.h>

// External Dependencies

// Declarations

// The below structs are used to store the data from weatherAPI.com

struct condition {
    char *text;
    char *icon;
    char *code;
};

struct location {
    char *name;
    char *region;
    char *country;
    char *lat;
    char *lon;
    char *tz_id;
    char *localtime_epoch;
    char *localtime;
};

struct currentWeather {
    char *last_updated_epoch;
    char *last_updated;
    char *temp_c;
    char *temp_f;
    char *is_day;
    struct condition conditionData;
    char *wind_mph;
    char *wind_kph;
    char *wind_degree;
    char *wind_dir;
    char *pressure_mb;
    char *pressure_in;
    char *precip_mm;
    char *precip_in;
    char *humidity;
    char *cloud;
    char *feelslike_c;
    char *feelslike_f;
    char *vis_km;
    char *vis_miles;
    char *uv;
    char *gust_mph;
    char *gust_kph;
};

struct weatherResponseData {
    struct location locationData;
    struct currentWeather currentWeatherData;
};