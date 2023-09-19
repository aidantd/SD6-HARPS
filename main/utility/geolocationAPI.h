// Includes
#include <stdio.h>

// External Dependencies

// Declarations

// The below structs are used to store the data from getgeoapi.com/

struct city {
    int geoname_id;
    char name[64];
    int population;
};

struct area {
    char code[16];
    int geoname_id;
    char name[64];
};

struct flag {
    char emoji[8];
    char emoji_unicode[8];
    char svg[256];
};

struct languages {
    char languages[256];
};

struct country {
    char code[8];
    char name[64];
    int phone_code;
    char area_size[64];
    char capital[32];
    int population;
    int geoname_id;
    char is_in_eu[8];
    struct flag flagData;
    struct languages languageData;
};

struct time {
    char timezone[64];
    char code[16];
    int gmt_offset;
    char is_daylight_saving[8];
};

struct asn {
    char organisation[256];
    int number;
};

struct currency {
    char code[8];
    char name[64];
};

struct locationIpAPI {
    int latitude;
    int longitude;
};

struct security {
    char is_tor[8];
    char is_proxy[8];
    char is_threat[8];
    char is_crawler[8];
};

struct continent {
    int geoname_id;
    char code[8];
    char name[64];
};

struct geoLocationResponseData {
    char status[64];
    char ip[64];
    char type[32];
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