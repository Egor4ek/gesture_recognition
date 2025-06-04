#include "accel_gyro.h"

int Gscale = GFS_250DPS;
int Ascale = AFS_2G;

float MPU6050lib::getGres()
{
  switch (Gscale)
  {
  // Possible gyro scales (and their register bit settings) are:
  // 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11).
  // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
  case GFS_250DPS:
    return 250.0 / 32768.0;
    break;
  case GFS_500DPS:
    return 500.0 / 32768.0;
    break;
  case GFS_1000DPS:
    return 1000.0 / 32768.0;
    break;
  case GFS_2000DPS:
    return 2000.0 / 32768.0;
    break;
  }
}

float MPU6050lib::getAres()
{
  switch (Ascale)
  {
  // Possible accelerometer scales (and their register bit settings) are:
  // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
  // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
  case AFS_2G:
    return 2.0 / 32768.0;
    break;
  case AFS_4G:
    return 4.0 / 32768.0;
    break;
  case AFS_8G:
    return 8.0 / 32768.0;
    break;
  case AFS_16G:
    return 16.0 / 32768.0;
    break;
  }
}

void MPU6050lib::readAccelData(int16_t *destination)
{
  uint8_t rawData[6];                                         // x/y/z accel register data stored here
  readBytes(MPU6050_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);   // Read the six raw data registers into data array
  destination[0] = (int16_t)((rawData[0] << 8) | rawData[1]); // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)((rawData[2] << 8) | rawData[3]);
  destination[2] = (int16_t)((rawData[4] << 8) | rawData[5]);
}

void MPU6050lib::readGyroData(int16_t *destination)
{
  uint8_t rawData[6];                                         // x/y/z gyro register data stored here
  readBytes(MPU6050_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);    // Read the six raw data registers sequentially into data array
  destination[0] = (int16_t)((rawData[0] << 8) | rawData[1]); // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)((rawData[2] << 8) | rawData[3]);
  destination[2] = (int16_t)((rawData[4] << 8) | rawData[5]);
}

int16_t MPU6050lib::readTempData()
{
  uint8_t rawData[2];                                     // x/y/z gyro register data stored here
  readBytes(MPU6050_ADDRESS, TEMP_OUT_H, 2, &rawData[0]); // Read the two raw data registers sequentially into data array
  return ((int16_t)rawData[0]) << 8 | rawData[1];         // Turn the MSB and LSB into a 16-bit value
}

// Configure the motion detection control for low power accelerometer mode
void MPU6050lib::LowPowerAccelOnlyMPU6050()
{

  // The sensor has a high-pass filter necessary to invoke to allow the sensor motion detection algorithms work properly
  // Motion detection occurs on free-fall (acceleration below a threshold for some time for all axes), motion (acceleration
  // above a threshold for some time on at least one axis), and zero-motion toggle (acceleration on each axis less than a
  // threshold for some time sets this flag, motion above the threshold turns it off). The high-pass filter takes gravity out
  // consideration for these threshold evaluations; otherwise, the flags would be set all the time!

  uint8_t c = readByte(MPU6050_ADDRESS, PWR_MGMT_1);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c & ~0x30); // Clear sleep and cycle bits [5:6]
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c | 0x30);  // Set sleep and cycle bits [5:6] to zero to make sure accelerometer is running

  c = readByte(MPU6050_ADDRESS, PWR_MGMT_2);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c & ~0x38); // Clear standby XA, YA, and ZA bits [3:5]
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c | 0x00);  // Set XA, YA, and ZA bits [3:5] to zero to make sure accelerometer is running

  c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x07); // Clear high-pass filter bits [2:0]
  // Set high-pass filter to 0) reset (disable), 1) 5 Hz, 2) 2.5 Hz, 3) 1.25 Hz, 4) 0.63 Hz, or 7) Hold
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c | 0x00); // Set ACCEL_HPF to 0; reset mode disbaling high-pass filter

  c = readByte(MPU6050_ADDRESS, CONFIG);
  writeByte(MPU6050_ADDRESS, CONFIG, c & ~0x07); // Clear low-pass filter bits [2:0]
  writeByte(MPU6050_ADDRESS, CONFIG, c | 0x00);  // Set DLPD_CFG to 0; 260 Hz bandwidth, 1 kHz rate

  c = readByte(MPU6050_ADDRESS, INT_ENABLE);
  writeByte(MPU6050_ADDRESS, INT_ENABLE, c & ~0xFF); // Clear all interrupts
  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x40);      // Enable motion threshold (bits 5) interrupt only

  // Motion detection interrupt requires the absolute value of any axis to lie above the detection threshold
  // for at least the counter duration
  writeByte(MPU6050_ADDRESS, MOT_THR, 0x80); // Set motion detection to 0.256 g; LSB = 2 mg
  writeByte(MPU6050_ADDRESS, MOT_DUR, 0x01); // Set motion detect duration to 1  ms; LSB is 1 ms @ 1 kHz rate

  delay(100); // Add delay for accumulation of samples

  c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x07); // Clear high-pass filter bits [2:0]
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c | 0x07);  // Set ACCEL_HPF to 7; hold the initial accleration value as a referance

  c = readByte(MPU6050_ADDRESS, PWR_MGMT_2);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c & ~0xC7); // Clear standby XA, YA, and ZA bits [3:5] and LP_WAKE_CTRL bits [6:7]
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, c | 0x47);  // Set wakeup frequency to 5 Hz, and disable XG, YG, and ZG gyros (bits [0:2])

  c = readByte(MPU6050_ADDRESS, PWR_MGMT_1);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c & ~0x20); // Clear sleep and cycle bit 5
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, c | 0x20);  // Set cycle bit 5 to begin low power accelerometer motion interrupts
}

void MPU6050lib::initMPU6050()
{
  // wake up device-don't need this here if using calibration function below
  //  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
  //  delay(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt

  // get stable time source
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01); // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001

  // Configure Gyro and Accelerometer
  // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
  // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
  // Maximum delay time is 4.9 ms corresponding to just over 200 Hz sample rate
  writeByte(MPU6050_ADDRESS, CONFIG, 0x03);

  // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x04); // Use a 200 Hz rate; the same rate set in CONFIG above

  // Set gyroscope full scale range
  // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
  uint8_t c = readByte(MPU6050_ADDRESS, GYRO_CONFIG);
  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c & ~0xE0);       // Clear self-test bits [7:5]
  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c & ~0x18);       // Clear AFS bits [4:3]
  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, c | Gscale << 3); // Set full scale range for the gyro

  // Set accelerometer configuration
  c = readByte(MPU6050_ADDRESS, ACCEL_CONFIG);
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0xE0);       // Clear self-test bits [7:5]
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c & ~0x18);       // Clear AFS bits [4:3]
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, c | Ascale << 3); // Set full scale range for the accelerometer

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips
  // can join the I2C bus and all can be controlled by the Arduino as master
  writeByte(MPU6050_ADDRESS, INT_PIN_CFG, 0x22);
  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x01); // Enable data ready (bit 0) interrupt
}

// Function which accumulates gyro and accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
void MPU6050lib::calibrateMPU6050(float *dest1, float *dest2)
{
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

  // reset device, reset all registers, clear gyro and accelerometer bias registers
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
  delay(100);

  // get stable time source
  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);
  writeByte(MPU6050_ADDRESS, PWR_MGMT_2, 0x00);
  delay(200);

  // Configure device for bias calculation
  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
  writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
  writeByte(MPU6050_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
  delay(15);

  // Configure MPU6050 gyro and accelerometer for bias calculation
  writeByte(MPU6050_ADDRESS, CONFIG, 0x01);       // Set low-pass filter to 188 Hz
  writeByte(MPU6050_ADDRESS, SMPLRT_DIV, 0x00);   // Set sample rate to 1 kHz
  writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

  uint16_t gyrosensitivity = 131;    // = 131 LSB/degrees/sec
  uint16_t accelsensitivity = 16384; // = 16384 LSB/g

  // Configure FIFO to capture accelerometer and gyro data for bias calculation
  writeByte(MPU6050_ADDRESS, USER_CTRL, 0x40); // Enable FIFO
  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x78);   // Enable gyro and accelerometer sensors for FIFO  (max size 1024 bytes in MPU-6050)
  delay(80);                                   // accumulate 80 samples in 80 milliseconds = 960 bytes

  // At end of sample accumulation, turn off FIFO sensor read
  writeByte(MPU6050_ADDRESS, FIFO_EN, 0x00);            // Disable gyro and accelerometer sensors for FIFO
  readBytes(MPU6050_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count / 12; // How many sets of full gyro and accelerometer data for averaging

  for (ii = 0; ii < packet_count; ii++)
  {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    readBytes(MPU6050_ADDRESS, FIFO_R_W, 12, &data[0]);           // read data for averaging
    accel_temp[0] = (int16_t)(((int16_t)data[0] << 8) | data[1]); // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t)(((int16_t)data[2] << 8) | data[3]);
    accel_temp[2] = (int16_t)(((int16_t)data[4] << 8) | data[5]);
    gyro_temp[0] = (int16_t)(((int16_t)data[6] << 8) | data[7]);
    gyro_temp[1] = (int16_t)(((int16_t)data[8] << 8) | data[9]);
    gyro_temp[2] = (int16_t)(((int16_t)data[10] << 8) | data[11]);

    accel_bias[0] += (int32_t)accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t)accel_temp[1];
    accel_bias[2] += (int32_t)accel_temp[2];
    gyro_bias[0] += (int32_t)gyro_temp[0];
    gyro_bias[1] += (int32_t)gyro_temp[1];
    gyro_bias[2] += (int32_t)gyro_temp[2];
  }
  accel_bias[0] /= (int32_t)packet_count; // Normalize sums to get average count biases
  accel_bias[1] /= (int32_t)packet_count;
  accel_bias[2] /= (int32_t)packet_count;
  gyro_bias[0] /= (int32_t)packet_count;
  gyro_bias[1] /= (int32_t)packet_count;
  gyro_bias[2] /= (int32_t)packet_count;

  if (accel_bias[2] > 0L)
  {
    accel_bias[2] -= (int32_t)accelsensitivity; // Remove gravity from the z-axis accelerometer bias calculation
  }
  else
  {
    accel_bias[2] += (int32_t)accelsensitivity;
  }

  // Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
  data[0] = (-gyro_bias[0] / 4 >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0] / 4) & 0xFF;      // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1] / 4 >> 8) & 0xFF;
  data[3] = (-gyro_bias[1] / 4) & 0xFF;
  data[4] = (-gyro_bias[2] / 4 >> 8) & 0xFF;
  data[5] = (-gyro_bias[2] / 4) & 0xFF;

  // Push gyro biases to hardware registers
  writeByte(MPU6050_ADDRESS, XG_OFFS_USRH, data[0]); // might not be supported in MPU6050
  writeByte(MPU6050_ADDRESS, XG_OFFS_USRL, data[1]);
  writeByte(MPU6050_ADDRESS, YG_OFFS_USRH, data[2]);
  writeByte(MPU6050_ADDRESS, YG_OFFS_USRL, data[3]);
  writeByte(MPU6050_ADDRESS, ZG_OFFS_USRH, data[4]);
  writeByte(MPU6050_ADDRESS, ZG_OFFS_USRL, data[5]);

  dest1[0] = (float)gyro_bias[0] / (float)gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
  dest1[1] = (float)gyro_bias[1] / (float)gyrosensitivity;
  dest1[2] = (float)gyro_bias[2] / (float)gyrosensitivity;

  // Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
  // factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
  // non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
  // compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
  // the accelerometer biases calculated above must be divided by 8.

  int32_t accel_bias_reg[3] = {0, 0, 0};                // A place to hold the factory accelerometer trim biases
  readBytes(MPU6050_ADDRESS, XA_OFFSET_H, 2, &data[0]); // Read factory accelerometer trim values
  accel_bias_reg[0] = (int16_t)((int16_t)data[0] << 8) | data[1];
  readBytes(MPU6050_ADDRESS, YA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[1] = (int16_t)((int16_t)data[0] << 8) | data[1];
  readBytes(MPU6050_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[2] = (int16_t)((int16_t)data[0] << 8) | data[1];

  uint32_t mask = 1uL;             // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

  for (ii = 0; ii < 3; ii++)
  {
    if (accel_bias_reg[ii] & mask)
      mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }

  // Construct total accelerometer bias, including calculated average accelerometer bias from above
  accel_bias_reg[0] -= (accel_bias[0] / 8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
  accel_bias_reg[1] -= (accel_bias[1] / 8);
  accel_bias_reg[2] -= (accel_bias[2] / 8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0]) & 0xFF;
  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1]) & 0xFF;
  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2]) & 0xFF;
  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

  // Push accelerometer biases to hardware registers
  writeByte(MPU6050_ADDRESS, XA_OFFSET_H, data[0]); // might not be supported in MPU6050
  writeByte(MPU6050_ADDRESS, XA_OFFSET_L_TC, data[1]);
  writeByte(MPU6050_ADDRESS, YA_OFFSET_H, data[2]);
  writeByte(MPU6050_ADDRESS, YA_OFFSET_L_TC, data[3]);
  writeByte(MPU6050_ADDRESS, ZA_OFFSET_H, data[4]);
  writeByte(MPU6050_ADDRESS, ZA_OFFSET_L_TC, data[5]);

  // Output scaled accelerometer biases for manual subtraction in the main program
  dest2[0] = (float)accel_bias[0] / (float)accelsensitivity;
  dest2[1] = (float)accel_bias[1] / (float)accelsensitivity;
  dest2[2] = (float)accel_bias[2] / (float)accelsensitivity;

  // Stores the 16-bit signed accelerometer sensor output
  int16_t accelCount[3] = {0, 0, 0};
  // Stores the real accel value in g's
  float ax = 0.0, ay = 0.0, az = 0.0;
  // Stores the 16-bit signed gyro sensor output
  int16_t gyroCount[3] = {0, 0, 0};
  // Stores the real gyro value in degrees per seconds
  float gx = 0.0, gy = 0.0, gz = 0.0;
  // scale resolutions per LSB for the sensors
  float aRes, gRes;

  const int READING_COUNT = 10;
  for (size_t i = 0; i < READING_COUNT; ++i)
  {
    // Read the x/y/z adc values
    gyroacc.readAccelData(accelCount);
    aRes = gyroacc.getAres();

    ax += (float)accelCount[0] * aRes;
    ay += (float)accelCount[1] * aRes;
    az += (float)accelCount[2] * aRes;

    gyroacc.readGyroData(gyroCount);
    gRes = gyroacc.getGres();

    // Calculate the gyro value into actual degrees per second
    // get actual gyro value, this depends on scale being set
    gx += (float)gyroCount[1] * gRes;
    gy += (float)gyroCount[0] * gRes;
    gz += (float)gyroCount[2] * gRes;

    delay(100);
  }

  dest1[0] = gx / READING_COUNT;
  dest1[1] = gy / READING_COUNT;
  dest1[2] = gz;

  dest2[0] = ax / READING_COUNT;
  dest2[1] = ay / READING_COUNT;
  dest2[2] = az / READING_COUNT;
}

void MPU6050lib::writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address); // Initialize the Tx buffer
  Wire.write(subAddress);          // Put slave register address in Tx buffer
  Wire.write(data);                // Put data in Tx buffer
  Wire.endTransmission();          // Send the Tx buffer
}

uint8_t MPU6050lib::readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data;                          // `data` will store the register data
  Wire.beginTransmission(address);       // Initialize the Tx buffer
  Wire.write(subAddress);                // Put slave register address in Tx buffer
  Wire.endTransmission(false);           // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, (uint8_t)1); // Read one byte from slave register address
  data = Wire.read();                    // Fill Rx buffer with result
  return data;                           // Return data read from slave register
}

void MPU6050lib::readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest)
{
  Wire.beginTransmission(address); // Initialize the Tx buffer
  Wire.write(subAddress);          // Put slave register address in Tx buffer
  Wire.endTransmission(false);     // Send the Tx buffer, but send a restart to keep connection alive
  uint8_t i = 0;
  Wire.requestFrom(address, count); // Read bytes from slave register address
  while (Wire.available())
  {
    dest[i++] = Wire.read();
  } // Put read results in the Rx buffer
}

void setupAccel()
{
  Wire.begin();
  uint8_t c = gyroacc.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);
  // WHO_AM_I should always be 0x68
  if (c == 0x68)
  {
    Serial.println("Calibrating of gyroscope and accelerometer has started");
    // Calibrate gyro and accelerometers, load biases in bias registers
    gyroacc.calibrateMPU6050(gyroBias, accelBias);
    gyroacc.initMPU6050();
    Serial.println("Calibrating of gyroscope and accelerometer has finished");
  }
}

char getSignOf(float number) {
  if (number >= 0) {
    return 1;
  } else {
    return -1;
  }
}

float remapAngleX(float angle) {
  return getSignOf(angle) * map(abs(angle), 0, 125, 0, 90);
}

float remapAngleY(float angle) {
  return getSignOf(angle) * map(abs(angle), 0, 125, 0, 90);
}

float readAccelAngle(char axis, int mode)
{
  if (gyroacc.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01)
  {
    int16_t accelCount[3], gyroCount[3];
    float aRes, gRes;
    const float ACCEL_MULTIPLIER = 100.0;

    gyroacc.readAccelData(accelCount);
    gyroacc.readGyroData(gyroCount);
    aRes = gyroacc.getAres();
    gRes = gyroacc.getGres();

    switch (mode)
    {
    case RAW_ACCEL:
    {
      switch (axis)
      {
      case 'x':
      {
        gyroacc.lastAx = ((float)accelCount[0] * aRes - accelBias[0]) * ACCEL_MULTIPLIER;
        return gyroacc.lastAx;
      }
      case 'y':
      {
        gyroacc.lastAy = ((float)accelCount[1] * aRes - accelBias[1]) * ACCEL_MULTIPLIER;
        return gyroacc.lastAy;
      }
      case 'z':
      {
        gyroacc.lastAz = -((float)accelCount[2] * aRes - accelBias[2]) * ACCEL_MULTIPLIER;
        return gyroacc.lastAz;
      }
      break;
      }
      break;
    }
    case RAW_GYRO:
    {
      switch (axis)
      {
      case 'x':
      {
        gyroacc.lastGx = (float)gyroCount[1] * gRes - gyroBias[1];
        return remapAngleX(gyroacc.lastGx);
      }
      case 'y':
      {
        gyroacc.lastGy = -((float)gyroCount[0] * gRes - gyroBias[0]);
        return remapAngleY(gyroacc.lastGy);
      }
      case 'z':
      {
       gyroacc.lastGz = -((float)accelCount[2] * aRes - accelBias[2]) * ACCEL_MULTIPLIER;
        return gyroacc.lastGz;
      }
      break;
      }
    }
    }
  }
  else
  {
    switch (mode)
    {
    case RAW_ACCEL:
    {
      switch (axis)
      {
      case 'x':
      {
        return gyroacc.lastAx;
      }
      case 'y':
      {
        return gyroacc.lastAy;
      }
      case 'z':
      {
        return gyroacc.lastAz;
      }
      break;
      }
      break;
    }
    case RAW_GYRO:
    {
      switch (axis)
      {
      case 'x':
      {
        return remapAngleX(gyroacc.lastGx);
      }
      case 'y':
      {
        return remapAngleY(gyroacc.lastGy);
      }
      case 'z':
      {
       return gyroacc.lastGz;
      }
      break;
      }
    }
    }
  }
}

/*
float readAccelerometer(char axis)
{
  int16_t accelCount[3];
  float ax, ay, az;
  float aRes;
  const float ACCEL_MULTIPLIER = 100.0;

  if (gyroacc.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01)
  {
    gyroacc.readAccelData(accelCount);
    aRes = gyroacc.getAres();

    switch (axis)
    {
    case 'x':
    {
      return (static_cast<float>(accelCount[0]) * aRes - accelBias[0]) * ACCEL_MULTIPLIER;
    }
    case 'y':
    {
      return (static_cast<float>(accelCount[1]) * aRes - accelBias[1]) * ACCEL_MULTIPLIER;
    }
    case 'z':
    {
      return -(static_cast<float>(accelCount[2]) * aRes - accelBias[2]) * ACCEL_MULTIPLIER;
    }
    }
  }
}

float readGyroscope(char axis)
{
  int16_t gyroCount[2];
  float gx, gy;
  float gRes;

  if (gyroacc.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01)
  {
    gyroacc.readGyroData(gyroCount);
    gRes = gyroacc.getGres();

    switch (axis)
    {
    case 'x':
    {
      return gx = static_cast<float>(gyroCount[1]) * gRes - gyroBias[1];
    }
    case 'y':
    {
      return gy = -static_cast<float>(gyroCount[0]) * gRes - gyroBias[0];
    }
    }
  }
}

// Temperature in degrees Centigrade
float readChipTemperature()
{
  return static_cast<float>(gyroacc.readTempData()) / 340.0 + 36.53;
}
*/