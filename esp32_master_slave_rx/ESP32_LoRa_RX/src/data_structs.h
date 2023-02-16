#include <RH_RF95.h>

#ifndef DATA_STRUCTS
    #define DATA_STRUCTS
    # define STARTBYTE 0xAA

    union datetime_union {
        struct datetime {
        uint16_t year;
        uint8_t b[5];
        } datetime;
        uint8_t buf[14];
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

    union gravity_so2_union {
        float fl[2]; // float size is 4 Bytes
        uint8_t buf[8];
    };

    union ecsense_so2_union {
        struct readings {
            uint8_t full_reading[13];
            float ecsense_so2;
            float ecsense_temp;
            float ecsense_hum;
        } readings;
        uint8_t buf[25];
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
            ecsense_so2_union ecsense_so2;
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


    void print_lora_union(lora_union rx) {
        Serial.println("datetime:");
        Serial.print("   "); Serial.print(rx.data_struct.datetime.datetime.year);
        Serial.print("-"); Serial.print(rx.data_struct.datetime.datetime.b[0]);
        Serial.print("-"); Serial.print(rx.data_struct.datetime.datetime.b[1]);
        Serial.print(" "); Serial.print(rx.data_struct.datetime.datetime.b[2]);
        Serial.print(":"); Serial.print(rx.data_struct.datetime.datetime.b[3]);
        Serial.print(":"); Serial.println(rx.data_struct.datetime.datetime.b[4]);
        Serial.println("gravity so2:");
        Serial.print("   GravitySO2: "); Serial.println(rx.data_struct.gravity_so2.fl[0]);
        Serial.print("   GravityTemp: "); Serial.println(rx.data_struct.gravity_so2.fl[1]);
        Serial.println("ecsense so2:");
        Serial.print("   ECSenseSO2: "); 
        for (int n=0; n<9; n++) {
            Serial.print(rx.data_struct.ecsense_so2.readings.full_reading[n], HEX); Serial.print(" "); 
        }
        Serial.println("");
        Serial.print("   ECSenseSO2: "); Serial.println(rx.data_struct.ecsense_so2.readings.ecsense_so2);
        Serial.println("co2:");
        Serial.print("   CO2: "); Serial.println(rx.data_struct.co2);
        Serial.println("atm master:");
        Serial.print("   Temp: "); Serial.println(rx.data_struct.atm_master.fl[0]);
        Serial.print("   Pres: "); Serial.println(rx.data_struct.atm_master.fl[1]);
        Serial.print("   Hum: "); Serial.println(rx.data_struct.atm_master.fl[2]);
        Serial.println("atm slave:");
        Serial.print("   Temp: "); Serial.println(rx.data_struct.atm_slave.fl[0]);
        Serial.print("   Pres: "); Serial.println(rx.data_struct.atm_slave.fl[1]);
        Serial.print("   Hum: "); Serial.println(rx.data_struct.atm_slave.fl[2]);
        Serial.println("gps slave:");
        Serial.print("   Sat: "); Serial.println(rx.data_struct.gps_slave.readings.satellites);
        Serial.print("   Lat: "); Serial.println(rx.data_struct.gps_slave.readings.lat);
        Serial.print("   Lng: "); Serial.println(rx.data_struct.gps_slave.readings.lng);
        Serial.print("   Met: "); Serial.println(rx.data_struct.gps_slave.readings.meters);
        Serial.print("   Deg: "); Serial.println(rx.data_struct.gps_slave.readings.deg);
        Serial.print("   MPS: "); Serial.println(rx.data_struct.gps_slave.readings.mps);
        Serial.print("   HDOP: "); Serial.println(rx.data_struct.gps_slave.readings.hdop);
        Serial.println("imu:");
        Serial.print("   Temp: "); Serial.println(rx.data_struct.imu.fl[0]);
        Serial.print("   AccX: "); Serial.println(rx.data_struct.imu.fl[1]);
        Serial.print("   AccY: "); Serial.println(rx.data_struct.imu.fl[2]);
        Serial.print("   AccZ: "); Serial.println(rx.data_struct.imu.fl[3]);
        Serial.print("   GyroX: "); Serial.println(rx.data_struct.imu.fl[4]);
        Serial.print("   GyroY: "); Serial.println(rx.data_struct.imu.fl[5]);
        Serial.print("   GyroZ: "); Serial.println(rx.data_struct.imu.fl[6]);
        Serial.println("thermocouple:");
        Serial.print("   Thermo: "); Serial.println(rx.data_struct.thermocouple.fl[0]);
        Serial.print("   Ambient: "); Serial.println(rx.data_struct.thermocouple.fl[1]);
        Serial.print("   ADC: "); Serial.println((int32_t)rx.data_struct.thermocouple.fl[2]);
        Serial.println("battery:");
        Serial.print("   M_A: "); Serial.println(rx.data_struct.m_battery_adc);
        Serial.print("   M_V: "); Serial.println(rx.data_struct.m_battery_voltage);
        Serial.print("   M_%: "); Serial.println(rx.data_struct.m_battry_percent);

        Serial.print("   S_A: "); Serial.println(rx.data_struct.s_battery_adc);
        Serial.print("   S_V: "); Serial.println(rx.data_struct.s_battery_voltage);
        Serial.print("   S_%: "); Serial.println(rx.data_struct.s_battry_percent);
        Serial.println("rssi:");
        Serial.print("   TX: "); Serial.println(rx.data_struct.tx_rssi);
        Serial.print("   RX: "); Serial.println(rx.data_struct.rx_rssi);
    }
#endif