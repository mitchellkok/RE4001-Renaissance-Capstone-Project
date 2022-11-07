#include <RH_RF95.h>

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
    double db[4]; // double size is 8 Bytes
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

union lora_union {
    struct data_struct {
      uint8_t start_byte; // = STARTBYTE;
      datetime_union datetime;
      gravity_so2_union gravity_so2;
      atm_union atm_master;
      atm_union atm_slave;
      // TODO: add GPS
      imu_union imu;
      thermo_union thermocouple;
      // TODO: add Batt voltage
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
    Serial.print("   "); Serial.println(rx.data_struct.gravity_so2.fl[0]);
    Serial.print("   "); Serial.println(rx.data_struct.gravity_so2.fl[1]);
    Serial.println("atm master:");
    Serial.print("   "); Serial.println(rx.data_struct.atm_master.fl[0]);
    Serial.print("   "); Serial.println(rx.data_struct.atm_master.fl[1]);
    Serial.print("   "); Serial.println(rx.data_struct.atm_master.fl[2]);
    Serial.println("atm slave:");
    Serial.print("   "); Serial.println(rx.data_struct.atm_slave.fl[0]);
    Serial.print("   "); Serial.println(rx.data_struct.atm_slave.fl[1]);
    Serial.print("   "); Serial.println(rx.data_struct.atm_slave.fl[2]);
    Serial.println("imu:");
    Serial.print("   "); Serial.println(rx.data_struct.imu.fl[0]);
    Serial.print("   "); Serial.println(rx.data_struct.imu.fl[1]);
    Serial.print("   "); Serial.println(rx.data_struct.imu.fl[2]);
    Serial.print("   "); Serial.println(rx.data_struct.imu.fl[3]);
    Serial.print("   "); Serial.println(rx.data_struct.imu.fl[4]);
    Serial.print("   "); Serial.println(rx.data_struct.imu.fl[5]);
    Serial.print("   "); Serial.println(rx.data_struct.imu.fl[6]);
    Serial.println("thermocouple:");
    Serial.print("   "); Serial.println(rx.data_struct.thermocouple.fl[0]);
    Serial.print("   "); Serial.println(rx.data_struct.thermocouple.fl[1]);
    Serial.print("   "); Serial.println((int32_t)rx.data_struct.thermocouple.fl[2]);
}