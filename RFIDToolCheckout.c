/*

    Program Name:  T-Space RFID Tool Checkout
    Developer:     Jeremy Maxey-Vesperman
    Date:          10-11-2014
    Purpose:       This program allows students to checkout T-Space tools by scanning their
                   student id and the tools RFID tag. The program then creates an entry logging its checkout

*/

// Import necessary libraries
#include <SD.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_PN532.h>

// Define colors for lcd
#define WHITE 0x7
#define TEAL 0x6
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define VIOLET 0x5

// Define arduino pins for card reader
#define SCK (2)
#define MOSI (3)
#define SS (4)
#define MISO (5)

// Init variables, constants, and objects
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
Adafruit_PN532 nfc(SCK, MISO, MOSI, SS);
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;
String initError;
char mode = 's';
boolean initOK = true;

// Declare functions / procedures
void messageDelay(int iDelay = 1000)
{
  delay(iDelay);
}

void initSerial()
{
  // Open serial communication
  Serial.begin(115200);
}

void initLCD()
{
  // Setup lcd's number of columns and rows and background
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
}

uint32_t initRFID()
{
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  return versiondata;
  nfc.SAMConfig();
}

void printInitRFIDMessage()
{
  // Output status
  Serial.print("Initializing RFID reader...");
  lcd.clear();
  lcd.setBacklight(WHITE);
  lcd.setCursor(2, 0);
  lcd.print("Initializing");
  lcd.setCursor(1, 1);
  lcd.print("RFID reader...");
  messageDelay();
}

boolean checkRFIDState(uint32_t RFIDVersion)
{
  if (RFIDVersion == 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void printRFIDInitSuccess(uint32_t RFIDVersion)
{
  Serial.print("Found chip PN5");
  Serial.println((RFIDVersion>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. ");
  Serial.print((RFIDVersion>>16) & 0xFF, DEC); 
  Serial.print('.');
  Serial.println((RFIDVersion>>8) & 0xFF, DEC);
  lcd.clear();
  lcd.setBacklight(GREEN);
  lcd.setCursor(3, 0);
  lcd.print("RFID Init");
  lcd.setCursor(2, 1);
  lcd.print("Successful.");
  messageDelay();
}

void printRFIDInitFailed()
{
  Serial.println("RFID Initialization failed!");
  lcd.clear();
  lcd.setBacklight(RED);
  lcd.setCursor(3, 0);
  lcd.print("RFID Init");
  lcd.setCursor(4, 1);
  lcd.print("Failed!");
  messageDelay(3000);
}

void printInitSDMessage()
{
  // Output status
  Serial.print("Initializing SD card...");
  lcd.clear();
  lcd.setBacklight(WHITE);
  lcd.setCursor(2, 0);
  lcd.print("Initializing");
  lcd.setCursor(3, 1);
  lcd.print("SD card...");
  messageDelay();
}

void initSDCard()
{
  // Change pinMode for writing to SD card
  pinMode(10, OUTPUT);
}

boolean checkSDState()
{
  return SD.begin(10);
}

void printSDInitFailed()
{
  Serial.println("Initialization failed!");
  lcd.clear();
  lcd.setBacklight(RED);
  lcd.setCursor(4, 0);
  lcd.print("SD Init");
  lcd.setCursor(4, 1);
  lcd.print("Failed!");
  messageDelay(3000);
}

void printSDInitSuccess()
{
  Serial.println("Initialization done.");
  lcd.clear();
  lcd.setBacklight(GREEN);
  lcd.setCursor(4, 0);
  lcd.print("SD Init");
  lcd.setCursor(2, 1);
  lcd.print("Successful.");
  messageDelay();
}

void setup()
{
  initSerial(); 

  initLCD();
 
  printInitSDMessage();
  
  initSDCard();
  
  if (checkSDState())
  {
    printSDInitSuccess();
  }
  else
  {
    printSDInitFailed();
    initOK = false;
    initError = "SD ";
  }
  
  printInitRFIDMessage();
  
  uint32_t versiondata = initRFID();
  
  if (checkRFIDState(versiondata))
  {
    printRFIDInitSuccess(versiondata);
  }
  else
  {
    printRFIDInitFailed();
    initOK = false;
    initError += "RFID ";
  }
}

void printWelcomeMessage()
{
  lcd.clear();
  lcd.setBacklight(TEAL);
  lcd.setCursor(3, 0);
  lcd.print("Weclome to");
  lcd.setCursor(4, 1);
  lcd.print("T-Space!");
  messageDelay(2000);
}

void printScanIDMessage()
{  
  lcd.clear();
  lcd.setBacklight(YELLOW);
  lcd.setCursor(3, 0);
  lcd.print("Please Scan");
  lcd.setCursor(4, 1);
  lcd.print("ID Badge");
  messageDelay(2000);
}

void checkMode()
{ 
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength))
  {
    Serial.println("Card found! Switching to active mode...");
    mode = 'a';
  }
}

void standbyMode()
{
  Serial.println("Standby Mode...");
  checkMode();
  if (mode = 'a')
  {
    return;
  }
  printWelcomeMessage();
  checkMode();
  if (mode = 'a')
  {
    return;
  }
  printScanIDMessage();
}

void activeMode()
{
  Serial.println("Active Mode...");
}

void printInitErrorMessage()
{
  Serial.print("Initialization error occurred. ");
  Serial.print(initError);
  Serial.println("failed to initialize.");
  lcd.clear();
  lcd.setBacklight(VIOLET);
  lcd.setCursor(1, 0);
  lcd.print("Initialization");
  lcd.setCursor(1, 1);
  lcd.print("Error Occured.");
  messageDelay(3000);
}

void loop()
{
  if (initOK)
  {
    checkMode;
  
    if (mode == 's')
    {
      standbyMode();
    }
    else
    {
      activeMode();
    }
  }
  else
  {
    printInitErrorMessage();
  }
}
