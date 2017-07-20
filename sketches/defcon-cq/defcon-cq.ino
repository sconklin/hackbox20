#include "WiFi.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Adafruit_NeoPixel.h>

// TFT Display Pins
#define TFT_CS    19
#define TFT_DC    22
#define TFT_MOSI  23
#define TFT_CLK   26
#define TFT_RST   21
#define TFT_MISO  25

// NeoPixel Values
#define PIXELPIN   5
#define NUMPIXELS  5
#define pixlux    20  //saturation level for NeoPixels colors

// Morse Code
#define CWSTARTMASK (0x1f)
#define CWSIZEMASK (0x07)
#define CWSIZESHIFT (0x05)
char *crypt=".....-----....-.-.--..--..-...--.- ";
char *plain=".,:?-/=@!0123456789abcdefghijklmnopqrstuvwxyz";
byte code[]={0xcd,0xd2,0xc7,0xd4,0xc9,0xb7,0xad,0xcb,0xcb,0xa5,0xa4,0xa3, \
           0xa2,0xa1,0xa0,0xa9,0xa8,0xa7,0xa6,0x44,0x89,0x8e,0x69,0x20, \
           0x8c,0x68,0x80,0x40,0x84,0x6e,0x99,0x45,0x49,0x65,0x91,0x9e, \
           0x6d,0x60,0x25,0x63,0x82,0x64,0x93,0x90,0x88};

const int tonefreq = 800;

// Audio Buzzer Values
const int buzzerPin = 18;

int muted = false;
int mute_touched = false;
char ssid[]="HackerBoxer_AI4QR";  //put your handle after the underscore
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);
char hackers_found[13][70];
int next_hacker_found = 0;

void setup() 
{
  tft.begin();
  tft.setRotation(3); // rotate 3*(pi/2)
  pixels.begin();
  pinMode(buzzerPin, OUTPUT);
  for (int i; i<13; i++)
    hackers_found[i][0] = 0; //empty array of strings
   touchAttachInterrupt(15, mutebutton, 40);  //threshold 40
}

void loop() 
{
  // cycle some NeoPixel Rainbows
  RainbowLEDcycle(18);
  // scan other SSIDs
  wifiScan2LCD();
  // start broadcating SSID (AP on)
  WiFi.softAP(ssid, NULL, 1, 0, 1);
  // Play Mario Theme on Buzzer
  mute_handler();
  if (!muted)
      sendcw(tonefreq, 15, "CQ CQ CQ DE AI4QR AI4QR AI4QR k");
  // chill here for a while
  delay(10000);
  //diplay list of found hackers tagged
  found2LCD();
  // cycle some NeoPixel Blues
  BlueLEDcycle(18);
  // Play Imperial March on Buzzer
  mute_handler();
  if (!muted)
      sendcw(tonefreq, 15, "CQ CQ CQ DE AI4QR AI4QR AI4QR k");
  // chill here for a while
  delay(10000);
  // stop broadcating SSID (AP off)  
  WiFi.softAPdisconnect(1);
}

void wifiScan2LCD() 
{
  int netsfound;
  int displaylines=13;
  
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(4);
  tft.println(" CQ DE AI4QR");
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);

  while (displaylines > 0)
  {
    netsfound = WiFi.scanNetworks();
    if (netsfound==0)
    {
      tft.println(". . .");
      displaylines--;
    }
    for (int i = 0; i < netsfound; ++i) 
    {
      if (WiFi.SSID(i).startsWith("HackerBoxer"))
      {
        WiFi.SSID(i).toCharArray(hackers_found[next_hacker_found],70);
        hackers_found[next_hacker_found][25] = 0;  //truncate string
        next_hacker_found++; 
        if (next_hacker_found == 13)
          next_hacker_found = 0;
      }
      else
      {
        // Print SSID and RSSI for each network found
        tft.print(" [");
        tft.print(WiFi.RSSI(i));
        tft.print("] ");
        tft.print(WiFi.SSID(i).substring(0,17));
        tft.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        delay(50);
        displaylines--;
      }
    }
    delay(3000); // Wait before scanning again
  }
}

void found2LCD() 
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(4);
  tft.println(" IDs TAGGED");
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(2);
  for (int i=0; i<13; i++)
  {
    tft.print(" ");
    tft.println(hackers_found[i]+12);
  }
}

void RainbowLEDcycle(int cycles)
{
  int i=0;
  while(cycles) 
  { 
    pixels.setPixelColor(i, pixels.Color(pixlux,0,0));
    i = (i==4) ? 0 : (i+1);
    pixels.setPixelColor(i, pixels.Color(pixlux,pixlux,0));
    i = (i==4) ? 0 : (i+1);
    pixels.setPixelColor(i, pixels.Color(0,pixlux,0));
    i = (i==4) ? 0 : (i+1);
    pixels.setPixelColor(i, pixels.Color(0,0,pixlux));
    i = (i==4) ? 0 : (i+1);
    pixels.setPixelColor(i, pixels.Color(pixlux,0,pixlux));
    i = (i==4) ? 0 : (i+1);
    i = (i==4) ? 0 : (i+1);
    pixels.show();
    delay(150);
    cycles--;
  }
}

void BlueLEDcycle(int cycles)
{
  int i=0;
  while(cycles) 
  { 
    pixels.setPixelColor(i, pixels.Color(0,0,pixlux*2));
    i = (i==4) ? 0 : (i+1);
    pixels.setPixelColor(i, pixels.Color(0,0,pixlux/2));
    i = (i==4) ? 0 : (i+1);
    pixels.setPixelColor(i, pixels.Color(0,0,pixlux/2));
    i = (i==4) ? 0 : (i+1);
    pixels.setPixelColor(i, pixels.Color(0,0,pixlux/2));
    i = (i==4) ? 0 : (i+1);
    pixels.setPixelColor(i, pixels.Color(0,0,pixlux/2));
    i = (i==4) ? 0 : (i+1);
    i = (i==4) ? 0 : (i+1);
    pixels.show();
    delay(150);
    cycles--;
  }
}

void mutebutton()
{
  mute_touched = true;
}

void mute_handler()
{
  if (mute_touched)
  {
    if (muted)
    {
      muted = false;
    }
    else
    {
      muted = true;
    }
    mute_touched = false;
  }
}

/* returns the index of char c in string s */
int strpos(const char *s, char c) {
 int x = 0;
 while (s[x] != 0) {
  if (s[x] == c) return x;
  x++;
 }
 return -1;
}

/* get the code for char c. -1 if not found. */
byte getCode(char c) {
 byte place = strpos(plain, tolower(c));
 if (place >= 0) {
   return code[place];
 }
 else return -1;
}

/* generate morse code equivalent for each character, period is in uS */
void genMorse(char c, int tone, int period) {
    char position, x;
    byte start, size;
    position = getCode(c);
    if (position != -1) {
        start = (position & CWSTARTMASK);
        size = ((position >> CWSIZESHIFT) & CWSIZEMASK);

        for (x = 0; x < size; x++) {
            char c = crypt[start+x];
            if (c == '.') {
              maketone(tone, period);
            } else if (c == '-') {
              maketone(tone, 3 * period);
            }
            delayMicroseconds(period); // intraletter spacing is 3 periods
        }
    }
}

void sendcw(int tone, int rate, char *string)
{
    /* 
     *  rate is in wpm
     * Timings derived from: http://www.arrl.org/files/file/Technology/x9004008.pdf
     * TODO support Farnsworth timing
     */
     int period = 1200000/float(rate); // period of a single period in microseconds
     int noteDelay = int(500000.0/tone);
     char *strptr = string;
     while (*strptr != 0)
     {
         if (*strptr == ' ') {
             delayMicroseconds(7*period); // space timing is 7 periods
         } else {
             genMorse(*strptr, tone, period);
             delayMicroseconds(3*period); // interletter spacing is 3 periods
         }
         strptr++;
     }
}

void maketone(int tone, int duration)
{
    // like beep() but with no delay, duration is in microSeconds
    int noteDelay = int(500000.0/tone);
    for (float i = 0.0; i < float(duration/1000000.0); i += (1.0/tone))
    {
        digitalWrite(buzzerPin, HIGH);
        delayMicroseconds(noteDelay);
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(noteDelay);
    }
}
