// Morse Code - adapted from the cwtext tool, GPLV2 or later
#define CWSTARTMASK (0x1f)
#define CWSIZEMASK (0x07)
#define CWSIZESHIFT (0x05)
char *crypt=".....-----....-.-.--..--..-...--.- ";
char *plain=".,:?-/=@!0123456789abcdefghijklmnopqrstuvwxyz";
byte code[]={0xcd,0xd2,0xc7,0xd4,0xc9,0xb7,0xad,0xcb,0xcb,0xa5,0xa4,0xa3, \
           0xa2,0xa1,0xa0,0xa9,0xa8,0xa7,0xa6,0x44,0x89,0x8e,0x69,0x20, \
           0x8c,0x68,0x80,0x40,0x84,0x6e,0x99,0x45,0x49,0x65,0x91,0x9e, \
           0x6d,0x60,0x25,0x63,0x82,0x64,0x93,0x90,0x88};

const int tonefreq = 500;

const int buzzerPin = 18;

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
             Serial.print("space\n");
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

void setup()
{
  //Setup pin modes
  pinMode(buzzerPin, OUTPUT);
}
 
void loop()
{
  sendcw(800, 13, "CQ CQ CQ DE AI4QR AI4QR AI4QR K");
  //sendcw(600, 10, "PARISPARISPARISPARISPARISPARISPARISPARISPARISPARIS");
  delay(3000);
}


