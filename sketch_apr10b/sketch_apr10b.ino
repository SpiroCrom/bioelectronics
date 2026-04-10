#include <Wire.h>
#include <SoftwareWire.h>

// Hardware I2C gyros
const byte MPU1_ADDR = 0x68;
const byte MPU2_ADDR = 0x69;

// Software I2C gyro
const byte MPU3_ADDR = 0x68;

// Software I2C on spare pins
SoftwareWire myWire3(10, 11);

int16_t gx1, gy1, gz1;
int16_t gx2, gy2, gz2;
int16_t gx3, gy3, gz3;

void setup() {
  Wire.begin();
  myWire3.begin();

  Serial.begin(115200);
  delay(1000);

  initMPU_HW(MPU1_ADDR);
  initMPU_HW(MPU2_ADDR);
  initMPU_SW(myWire3, MPU3_ADDR);

  // CSV header (sent once)
  Serial.println("time_ms,g1x,g1y,g1z,g2x,g2y,g2z,g3x,g3y,g3z");
}

void loop() {
  bool ok1 = readGyro_HW(MPU1_ADDR, gx1, gy1, gz1);
  bool ok2 = readGyro_HW(MPU2_ADDR, gx2, gy2, gz2);
  bool ok3 = readGyro_SW(myWire3, MPU3_ADDR, gx3, gy3, gz3);

  // Only send data if all sensors read successfully
  if (ok1 && ok2 && ok3) {

    Serial.print(millis());
    Serial.print(",");

    Serial.print(gx1); Serial.print(",");
    Serial.print(gy1); Serial.print(",");
    Serial.print(gz1); Serial.print(",");

    Serial.print(gx2); Serial.print(",");
    Serial.print(gy2); Serial.print(",");
    Serial.print(gz2); Serial.print(",");

    Serial.print(gx3); Serial.print(",");
    Serial.print(gy3); Serial.print(",");
    Serial.println(gz3);
  }

  delay(20);   // ~50 Hz sampling
}

void initMPU_HW(byte addr) {
  Wire.beginTransmission(addr);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Wire.beginTransmission(addr);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void initMPU_SW(SoftwareWire &wire, byte addr) {
  wire.beginTransmission(addr);
  wire.write(0x6B);
  wire.write(0x00);
  wire.endTransmission();

  wire.beginTransmission(addr);
  wire.write(0x1B);
  wire.write(0x00);
  wire.endTransmission();
}

bool readGyro_HW(byte addr, int16_t &gx, int16_t &gy, int16_t &gz) {
  Wire.beginTransmission(addr);
  Wire.write(0x43);
  if (Wire.endTransmission(false) != 0) return false;

  if (Wire.requestFrom((int)addr, 6, (int)true) != 6) return false;

  gx = (Wire.read() << 8) | Wire.read();
  gy = (Wire.read() << 8) | Wire.read();
  gz = (Wire.read() << 8) | Wire.read();

  return true;
}

bool readGyro_SW(SoftwareWire &wire, byte addr, int16_t &gx, int16_t &gy, int16_t &gz) {
  wire.beginTransmission(addr);
  wire.write(0x43);
  if (wire.endTransmission(false) != 0) return false;

  if (wire.requestFrom((uint8_t)addr, (uint8_t)6) != 6) return false;

  gx = (wire.read() << 8) | wire.read();
  gy = (wire.read() << 8) | wire.read();
  gz = (wire.read() << 8) | wire.read();

  return true;
}