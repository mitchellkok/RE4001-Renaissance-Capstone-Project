#include <RH_RF95.h>

#ifndef DATA_STRUCTS
    #define DATA_STRUCTS
    union datetime_union {
        struct datetime {
        uint16_t year;
        uint8_t b[5];
        } datetime;
        uint8_t buf[14];
    };

    union gravity_so2_union {
        float fl[2]; // float size is 4 Bytes
        uint8_t buf[8];
    };

    union atm_union {
        float fl[3]; // float size is 4 Bytes
        uint8_t buf[12];
    };

    union gps_union {
        struct readings {
        uint32_t satellites;
        uint32_t hdop;
        double lat;
        double lng;
        double meters;
        double deg;
        double mps;
        double value;
        } readings;
        uint8_t buf[56];
    };

    union thermo_union {
        float fl[3]; // float size is 4 Bytes, or uin32_t
        uint8_t buf[12];
    };

    union imu_union {
        float fl[7]; // float size is 4 Bytes
        uint8_t buf[28];
    };

    union data_union {  // SPI
        struct readings {
        atm_union atm;
        gps_union gps;
        } readings;
        char s[68]; // update this!
        uint8_t buf[68];
    };

    union lora_union {
        struct data_struct {
        uint8_t start_byte;
        datetime_union datetime;
        gravity_so2_union gravity_so2;
        atm_union atm_master;
        atm_union atm_slave;
        gps_union gps_slave;
        imu_union imu;
        thermo_union thermocouple;
        // TODO: add Batt voltage
        } data_struct;
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; // 251 bytes
    };
#endif