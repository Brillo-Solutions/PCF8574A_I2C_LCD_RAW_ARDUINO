#include <Wire.h>
#define DEV_ADDR 0x3F // I2C address of PCF8574A extender

void setup() 
{
  Wire.begin();
  initDisplay();
}

void loop() 
{
  showData("Hello World", 1, 3, true);  // Data, Row Index, Cursor Index, Backlight Condition
  showData("Brillo Solutions", 2, 1, true);  // Data, Row Index, Cursor Index, Backlight Condition
  while (1);
}

void sendToDisplay(byte mByte)  // Methods sending 8-bit data to PCF8574A I2C extender
{
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(mByte);
  Wire.endTransmission();
}

void brkInstByte(byte mByte)
{
  byte nByte;
  nByte = mByte & 0xF0;
  sendToDisplay(nByte | 0x04);
  delay(1);
  sendToDisplay(nByte | 0x00);
  nByte = ((mByte << 4) & 0xF0);
  sendToDisplay(nByte | 0x04);
  delay(1);
  sendToDisplay(nByte | 0x00);
}

void brkDataByte(byte mByte)
{
  byte nByte;
  nByte = mByte & 0xF0;
  sendToDisplay(nByte | 0x05);
  delay(1);
  sendToDisplay(nByte | 0x01);
  nByte = ((mByte << 4) & 0xF0);
  sendToDisplay(nByte | 0x05);
  delay(1);
  sendToDisplay(nByte | 0x01);
}

void initDisplay()
{
  byte mArr[4] = {0x02, 0x28, 0x01, 0x0C};  // LCD 4-bit mode initialization commands
  for (int k = 0; k < 4; k++)
    brkInstByte(mArr[k]);  // Send 8-bit LCD commands in 4-bit mode
}

void showData(const char *p, byte rn, byte cp, boolean bl)
{
  byte rowAddr;
  
  if (rn == 1)
    rowAddr = 0x80; // First row of 16x2 LCD
  else
    rowAddr = 0xC0; // Second row of 16x2 LCD

  brkInstByte(rowAddr + (cp - 1));  // Send 8-bit LCD commands in 4-bit mode
    
  while (*p != '\0')
  {
    brkDataByte(*p);  // Send 8-bit LCD data in 4-bit mode
    p++;
  }

  setBackLight(bl);
}

void setBackLight(boolean mBool)
{
  if (mBool)
    sendToDisplay(0xFF);  // Turn ON backlight of LCD
  else
    sendToDisplay(0xF7);  // Turn OFF backlight of LCD
}
