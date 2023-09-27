// Includes
#include <stdio.h>

// External Dependencies

// Declarations
#define MAX_FIELD_LENGTH 256
#define MAX_LARGE_FIELD_LENGTH 64
#define MAX_MEDIUM_FIELD_LENGTH 16
#define MAX_SHORT_FIELD_LENGTH 8

// The below structs are used to store the data from getgeoapi.com/

struct city {
    int geoname_id;
    char name[MAX_LARGE_FIELD_LENGTH];
    int population;
};

struct area {
    char code[MAX_MEDIUM_FIELD_LENGTH];
    int geoname_id;
    char name[MAX_LARGE_FIELD_LENGTH];
};

struct flag {
    char emoji[MAX_SHORT_FIELD_LENGTH];
    char emoji_unicode[MAX_SHORT_FIELD_LENGTH];
    char svg[MAX_FIELD_LENGTH];
};

struct languages {
    char languages[MAX_FIELD_LENGTH];
};

struct country {
    char code[MAX_MEDIUM_FIELD_LENGTH];
    char name[MAX_LARGE_FIELD_LENGTH];
    int phone_code;
    char area_size[MAX_LARGE_FIELD_LENGTH];
    char capital[MAX_LARGE_FIELD_LENGTH];
    int population;
    int geoname_id;
    char is_in_eu[MAX_SHORT_FIELD_LENGTH];
    struct flag flagData;
    struct languages languageData;
};

struct time {
    char timezone[MAX_LARGE_FIELD_LENGTH];
    char code[MAX_MEDIUM_FIELD_LENGTH];
    int gmt_offset;
    char is_daylight_saving[MAX_SHORT_FIELD_LENGTH];
};

struct asn {
    char organisation[MAX_FIELD_LENGTH];
    int number;
};

struct currency {
    char code[MAX_MEDIUM_FIELD_LENGTH];
    char name[MAX_LARGE_FIELD_LENGTH];
};

struct locationIpAPI {
    int latitude;
    int longitude;
};

struct security {
    char is_tor[MAX_SHORT_FIELD_LENGTH];
    char is_proxy[MAX_SHORT_FIELD_LENGTH];
    char is_threat[MAX_SHORT_FIELD_LENGTH];
    char is_crawler[MAX_SHORT_FIELD_LENGTH];
};

struct continent {
    int geoname_id;
    char code[MAX_MEDIUM_FIELD_LENGTH];
    char name[MAX_LARGE_FIELD_LENGTH];
};

struct geoLocationResponseData {
    char status[MAX_LARGE_FIELD_LENGTH];
    char ip[MAX_LARGE_FIELD_LENGTH];
    char type[MAX_LARGE_FIELD_LENGTH];
    struct city cityData;
    struct area areaData;
    struct country countryData;
    struct time timeData;
    struct asn asnData;
    struct currency currencyData;
    struct locationIpAPI locationData;
    struct security securityData;
    struct continent continentData;
    int postCode;
};