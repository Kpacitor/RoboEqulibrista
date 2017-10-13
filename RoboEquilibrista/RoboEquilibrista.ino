
/* Include para comunicação I2C */
#include <Wire.h>

uint32_t timer;

/* Buffer de dados do I2C */
uint8_t i2cData[14];

/* Variáveis que receberão informações das acelerações nos eixos xyz da MPU6050 via i2C */
double accX, accY, accZ;
/* Variáveis que receberão informações das giro nos eixos xyz da MPU6050 via i2C */
double gyroX, gyroY, gyroZ;

int16_t tempRaw;

/* Iniciando as configurações do Arduino */
void setup() {

  /* Serial para plotar as informações no monitor Serial */
  Serial.begin(115200);
  
  /* Inicinado o I2C */
  Wire.begin();

  
#if ARDUINO >= 157
  /*Set I2C frequency to 400kHz*/
  Wire.setClock(400000UL); 
#else
  /* Set I2C frequency to 400kHz */
  TWBR = ((F_CPU / 400000UL) - 16) / 2;
#endif

  /* Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz*/
  i2cData[0] = 7;
  /* Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling */
  i2cData[1] = 0x00;  
  /* Set Gyro Full Scale Range to ±250deg/s */
  i2cData[2] = 0x00;  
  /* Set Accelerometer Full Scale Range to ±2g */
  i2cData[3] = 0x00;  

  /* Write to all four registers at once */
  while (i2cWrite(0x19, i2cData, 4, false));

  /* PLL with X axis gyroscope reference and disable sleep mode */
  while (i2cWrite(0x6B, 0x01, true));

  while (i2cRead(0x75, i2cData, 1));
  
  /*Read "WHO_AM_I" register*/
  if (i2cData[0] != 0x68) { 
    Serial.print(F("Error reading sensor"));
    while (1);
  }

  /* Wait for sensor to stabilize */
  delay(100);

  timer = micros();
}

void loop() {
  
  /* Update all the values */
  while (i2cRead(0x3B, i2cData, 14));

  /* Leitura e organização de dois bytes de dados do Buffer I2C */
  accX = (int16_t)((i2cData[0] << 8) | i2cData[1]); // [ 0 ] MSB e [ 1 ] LSB

  /* Leitura e organização de dois bytes de dados do Buffer I2C */
  accY = (int16_t)((i2cData[2] << 8) | i2cData[3]); // [ 2 ] MSB e [ 3 ] LSB

  /* Leitura e organização de dois bytes de dados do Buffer I2C */
  accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]); // [ 4 ] MSB e [ 5 ] LSB

  /* Leitura e organização de dois bytes de dados do Buffer I2C */
  tempRaw = (int16_t)((i2cData[6] << 8) | i2cData[7]); // [ 6 ] MSB e [ 7 ] LSB

  /* Leitura e organização de dois bytes de dados do Buffer I2C */
  gyroX = (int16_t)((i2cData[8] << 8) | i2cData[9]); // [ 8 ] MSB e [ 9 ] LSB

  /* Leitura e organização de dois bytes de dados do Buffer I2C */
  gyroY = (int16_t)((i2cData[10] << 8) | i2cData[11]); // [ 10 ] MSB e [ 11 ] LSB

  /* Leitura e organização de dois bytes de dados do Buffer I2C */
  gyroZ = (int16_t)((i2cData[12] << 8) | i2cData[13]); // [ 12 ] MSB e [ 13 ] LSB
  

  /* Exibe no Monitor serial o valor especificado como argumento da função*/
  //Serial.print(accX);
  //Serial.print(accY);
  //Serial.print(accZ);
  //Serial.print(gyroX);
  //Serial.print(gyroY);
  Serial.print(gyroZ);
  Serial.print("\n");

}
