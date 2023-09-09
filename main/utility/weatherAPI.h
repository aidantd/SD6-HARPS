// Includes
#include <stdio.h>

// External Dependencies

// Declarations

// The below structs are used to store the data from weatherAPI.com

struct condition {
    char *text;
    char *icon;
    int code;
};

struct location {
    char *name;
    char *region;
    char *country;
    int lat;
    int lon;
    char *tz_id;
    int localtime_epoch;
    char *localtime;
};

struct currentWeather {
    int last_updated_epoch;
    char *last_updated;
    int temp_c;
    int temp_f;
    int is_day;
    struct condition conditionData;
    int wind_mph;
    int wind_kph;
    int wind_degree;
    char *wind_dir;
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

struct weatherResponseData {
    struct location locationData;
    struct currentWeather currentWeatherData;
};