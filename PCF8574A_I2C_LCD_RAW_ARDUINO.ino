#include <Wire.h>
#define DEV_ADDR 0x3F // I2C address of PCF8574A extender

byte backLight;

void setup() 
{
  Wire.begin();
  setBackLight(true);
  initDisplay();
}

void loop() 
{
  showData("Hello World", 1, 3);  // String Data, Row Index, Cursor Index
  showData('.', 1, 14);  // Char Data, Row Index, Cursor Index
  showData("Brillo Solutions", 2, 1);  // Data, Row Index, Cursor Index
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
  sendToDisplay(nByte | (backLight | 0x04));
  delay(1);
  sendToDisplay(nByte | (backLight | 0x00));
  nByte = ((mByte << 4) & 0xF0);
  sendToDisplay(nByte | (backLight | 0x04));
  delay(1);
  sendToDisplay(nByte | (backLight | 0x00));
}

void brkDataByte(byte mByte)
{
  byte nByte;
  nByte = mByte & 0xF0;
  sendToDisplay(nByte | (backLight | 0x05));
  delay(1);
  sendToDisplay(nByte | (backLight | 0x01));
  nByte = ((mByte << 4) & 0xF0);
  sendToDisplay(nByte | (backLight | 0x05));
  delay(1);
  sendToDisplay(nByte | (backLight | 0x01));
}

void initDisplay()
{
  byte mArr[4] = {0x02, 0x28, 0x01, 0x0C};  // LCD 4-bit mode initialization commands
  for (int k = 0; k < 4; k++)
    brkInstByte(mArr[k]);  // Send 8-bit LCD commands in 4-bit mode
}

void showData(const char *p, byte rn, byte cp)
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
}

void showData(byte mByte, byte rn, byte cp)
{
  byte rowAddr;
  
  if (rn == 1)
    rowAddr = 0x80; // First row of 16x2 LCD
  else
    rowAddr = 0xC0; // Second row of 16x2 LCD

  brkInstByte(rowAddr + (cp - 1));  // Send 8-bit LCD commands in 4-bit mode 
  brkDataByte(mByte);  // Send 8-bit LCD data in 4-bit mode
}

void setBackLight(boolean mBool)
{
  if (mBool)
    backLight = 0x08;  // Turn ON backlight of LCD
  else
    backLight = 0x00;  // Turn OFF backlight of LCD
}
