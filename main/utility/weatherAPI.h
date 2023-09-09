// Includes
#include <stdio.h>

// External Dependencies

// Declarations
#define MAX_FIELD_LENGTH 256

// The below structs are used to store the data from weatherAPI.com

struct condition {
    char text[256];
    char icon[256];
    int code;
};

struct location {
    char name[256];
    char region[256];
    char country[256];
    int lat;
    int lon;
    char tz_id[256];
    int localtime_epoch;
    char localtime[256];
};

struct currentWeather {
    int last_updated_epoch;
    char last_updated[256];
    int temp_c;
    int temp_f;
    int is_day;
    struct condition conditionData;
    int wind_mph;
    int wind_kph;
    int wind_degree;
    char wind_dir[256];
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