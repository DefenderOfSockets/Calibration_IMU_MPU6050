#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// Объявление объекта класса MPU6050, который имеет все необходимые методы для работы с датчиком
MPU6050 accelgyro;

void setup() {
    delay(3000);
    // Подключение по шине I2C к MPU6500, инициализация последовательныго порта и датчика
    Wire.begin(22,21);
    Serial.begin(38400);
    Serial.println(F("Initializing I2C devices..."));
    accelgyro.initialize();
    accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_1000);
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);

    accelgyro.CalibrateAccel(6);
    accelgyro.CalibrateGyro(6);

    // Проверка соединения микроконтроллера и датчика
    Serial.println(F("Testing device connections..."));
    Serial.println(accelgyro.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));              

    AccelSelfTest();
    
    delay(1000);
    
    GyroSelfTest();
}

void loop() {}

void AccelSelfTest()
{
    
  int16_t ax1, ay1, az1, ax2, ay2, az2;
  int16_t gx, gy, gz;
  float ftxa, ftya, ftza, xp, yp, zp;
  float LSB_accel;

    Serial.println(" ");
    Serial.println("Acceleroemeter self-test");
    int range_accel = accelgyro.getFullScaleAccelRange();
    Serial.print("Range_accel: "); Serial.println(range_accel);

    switch (range_accel)
    {
      case 0:
        LSB_accel = 16384.0; break;
      case 1:
        LSB_accel = 8192.0; break;
      case 2:
        LSB_accel = 4096.0; break;
      case 3:
        LSB_accel = 2048.0; break;
      default:
        Serial.print("Error with range of accelerometer in IMU. Can't choose LSB.");
    }
    
    accelgyro.setAccelXSelfTest(false);
    accelgyro.setAccelYSelfTest(false);
    accelgyro.setAccelZSelfTest(false);
    
    accelgyro.getMotion6(&ax1, &ay1, &az1, &gx, &gy, &gz);
    Serial.print("Values with Self-Test Off - ");
    Serial.print(ax1); Serial.print("\t");
    Serial.print(ay1); Serial.print("\t");
    Serial.println(az1);
    
    delay(100);
    
    accelgyro.setAccelXSelfTest(true);
    accelgyro.setAccelYSelfTest(true);
    accelgyro.setAccelZSelfTest(true);
    
    delay(200);
    
    accelgyro.getMotion6(&ax2, &ay2, &az2, &gx, &gy, &gz);
    Serial.print("Values with Self-Test On - ");
    Serial.print(ax2); Serial.print("\t");
    Serial.print(ay2); Serial.print("\t");
    Serial.println(az2);
    
    delay(200);
    
    int xa_test = accelgyro.getAccelXSelfTestFactoryTrim();
    Serial.print("getAccelXSelfTestFactoryTrim xa_test: ");
    Serial.println(xa_test);
    
    int ya_test = accelgyro.getAccelYSelfTestFactoryTrim();
    Serial.print("getAccelYSelfTestFactoryTrim ya_test: ");
    Serial.println(ya_test);
    
    int za_test = accelgyro.getAccelZSelfTestFactoryTrim();
    Serial.print("getAccelZSelfTestFactoryTrim za_test: ");
    Serial.println(za_test);
    
    ftxa = LSB_accel * 0.34 * pow((0.92/0.34), (xa_test - 1)/30.0);
    ftya = LSB_accel * 0.34 * pow((0.92/0.34), (ya_test - 1)/30.0);
    ftza = LSB_accel * 0.34 * pow((0.92/0.34), (za_test - 1)/30.0);
    
    Serial.print("Deduced value for FT[Xa] = "); Serial.println(ftxa, DEC);
    Serial.print("Deduced value for FT[Ya] = "); Serial.println(ftya, DEC);
    Serial.print("Deduced value for FT[Za] = "); Serial.println(ftza, DEC);
    
    xp = 100 * ((ax2 - ax1) - ftxa)/ftxa;
    yp = 100 * ((ay2 - ay1) - ftya)/ftya;
    zp = 100 * ((az2 - az1) - ftza)/ftza;
    
    Serial.print("X change from Fact Trim Self Test Response: "); Serial.println(xp);
    Serial.print("Y change from Fact Trim Self Test Response: "); Serial.println(yp);
    Serial.print("Z change from Fact Trim Self Test Response: "); Serial.println(zp);  
}

void GyroSelfTest()
{
  int16_t gx1, gy1, gz1, gx2, gy2, gz2;
  int16_t ax, ay, az;
  float ftxg, ftyg, ftzg, xpg, ypg, zpg;
  float LSB_gyro;
  
    Serial.println(" ");
    Serial.println("Gyroscope self-test");
    int range_gyro = accelgyro.getFullScaleGyroRange();
    Serial.print("Range_gyro: "); Serial.println(range_gyro);

    switch (range_gyro)
    {
      case 0:
        LSB_gyro = 131; break;
      case 1:
        LSB_gyro = 65.5; break;
      case 2:
        LSB_gyro = 32.8; break;
      case 3:
        LSB_gyro = 16.4; break;
      default:
        Serial.print("Error with range of gyroscope in IMU. Can't choose LSB.");
    }
    
    accelgyro.setGyroXSelfTest(false);
    accelgyro.setGyroYSelfTest(false);
    accelgyro.setGyroZSelfTest(false);
    
    accelgyro.getMotion6(&ax, &ay, &az, &gx1, &gy1, &gz1);
    Serial.print("Values with Self-Test Off - ");
    Serial.print(gx1); Serial.print("\t");
    Serial.print(gy1); Serial.print("\t");
    Serial.println(gz1);
    
    delay(100);
    
    accelgyro.setGyroXSelfTest(true);
    accelgyro.setGyroYSelfTest(true);
    accelgyro.setGyroZSelfTest(true);
    
    delay(200);
    
    accelgyro.getMotion6(&ax, &ay, &az, &gx2, &gy2, &gz2);
    Serial.print("Values with Self-Test On - ");
    Serial.print(gx2); Serial.print("\t");
    Serial.print(gy2); Serial.print("\t");
    Serial.println(gz2);
    
    delay(200);
    
    int xg_test = accelgyro.getGyroXSelfTestFactoryTrim();
    Serial.print("getGyroXSelfTestFactoryTrim xa_test: ");
    Serial.println(xg_test);
    
    int yg_test = accelgyro.getGyroYSelfTestFactoryTrim();
    Serial.print("getGyroYSelfTestFactoryTrim ya_test: ");
    Serial.println(yg_test);
    
    int zg_test = accelgyro.getGyroZSelfTestFactoryTrim();
    Serial.print("getGyroZSelfTestFactoryTrim za_test: ");
    Serial.println(zg_test);

    ftxg = LSB_gyro * 25.0 * pow(1.046, (xg_test - 1.0));
    ftyg = LSB_gyro * (-25.0) * pow(1.046, (yg_test - 1.0));
    ftzg = LSB_gyro * 25.0 * pow(1.046, (zg_test - 1.0));
    
    Serial.print("Deduced value for FT[Xg] = "); Serial.println(ftxg, DEC);
    Serial.print("Deduced value for FT[Yg] = "); Serial.println(ftyg, DEC);
    Serial.print("Deduced value for FT[Zg] = "); Serial.println(ftzg, DEC);
    
    xpg = 100 * ((gx2 - gx1) - ftxg)/ftxg;
    ypg = 100 * ((gy2 - gy1) - ftyg)/ftyg;
    zpg = 100 * ((gz2 - gz1) - ftzg)/ftzg;
    
    Serial.print("X change from Fact Trim Self Test Response: "); Serial.println(xpg);
    Serial.print("Y change from Fact Trim Self Test Response: "); Serial.println(ypg);
    Serial.print("Z change from Fact Trim Self Test Response: "); Serial.println(zpg);  
}
