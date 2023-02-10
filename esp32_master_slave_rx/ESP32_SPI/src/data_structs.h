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
            float lat;
            float lng;
            float meters;
            float deg;
            float mps;
            float value;
        } readings;
        uint8_t buf[32];
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
            int co2;
            float battery_voltage;
            float battery_percent;
        } readings;
        char s[64]; // 64 bytes is maximum buffer length for SPI library
        uint8_t buf[64];
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
            float ecsense_so2;
            int co2;
            int m_battery_adc;
            float m_battery_voltage;
            float m_battry_percent;
            int s_battery_adc;
            float s_battery_voltage;
            float s_battry_percent;
            int16_t tx_rssi;
            int16_t rx_rssi;
        } data_struct;
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; // 251 bytes
    };

#endif