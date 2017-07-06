
const int c4 = 262;
const int c4S = 277;
const int d4 = 294;
const int e4F = 311;
const int e4 = 330;
const int f4 = 349;
const int f4S = 370;
const int g4 = 392;
const int g4S = 415;
const int a4 = 440;
const int b4F = 466;
const int b4 = 494;

const int c5 = 523;
const int e5 = 659;
const int f5 = 698;
const int e6 = 1319;
const int g6 = 1568;
const int a6 = 1760;
const int as6 = 1865;
const int b6 = 1976;
const int c7 = 2093;
const int d7 = 2349;
const int e7 = 2637;
const int f7 = 2794;
const int g7 = 3136;



const int buzzerPin = 18;
 
void setup()
{
  //Setup pin modes
  pinMode(buzzerPin, OUTPUT);
}
 
void loop()
{
//  scale4();
//  delay(3000);
  MarioTheme();
  delay(3000);
  ImperialMarch();
  delay(3000);
}

void beep(int tone, int duration)
{
  int noteDelay = int(500000.0/tone);
  for (float i = 0.0; i < float(duration/1000.0); i += (1.0/tone))
  {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(noteDelay);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(noteDelay);
  }
  delay(30);
}

void scale4()
{
  beep(c4, 200);
//  beep(c4S, 100);
  beep(d4, 100);
//  beep(e4F, 100);
  beep(e4, 100);
  beep(f4, 100);
//  beep(f4S, 100);
  beep(g4, 100);
//  beep(g4S, 100);
  beep(a4, 100);
//  beep(b4F, 100);
  beep(b4, 100);
  beep(c5, 200);
}

void  ImperialMarch()
{
  beep(a4, 500);
  beep(a4, 500);    
  beep(a4, 500);
  beep(f4, 350);
  beep(c5, 150);  
  beep(a4, 500);
  beep(f4, 350);
  beep(c5, 150);
  beep(a4, 650);
  delay(500);
  beep(e5, 500);
  beep(e5, 500);
  beep(e5, 500);  
  beep(f5, 350);
  beep(c5, 150);
  beep(g4S, 500);
  beep(f4, 350);
  beep(c5, 150);
  beep(a4, 650);
}

void  MarioTheme()
{
  beep(e7,150);
  beep(e7,150);
  delay(150);
  beep(e7,150);  
  delay(150);
  beep(c7,150);
  beep(e7,150);
  delay(150);
  beep(g7,150);
  delay(450);
  beep(g6,150);
  delay(450);
  beep(c7,150);
  delay(300);
  beep(g6,150);
  delay(300);
  beep(e6,150);
  delay(300);
  beep(a6,150);
  delay(150);
  beep(b6,150);
  delay(150);
  beep(as6,150);
  beep(a6,150);
  delay(150);
  beep(g6,112);
  beep(e7,112); 
  beep(g7,112);
  beep(a6,150);
  delay(150);
  beep(f7,150);
  beep(g7,150);
  delay(150);
  beep(e7,150);
  delay(150); 
  beep(c7,150);
  beep(d7,150);
  beep(b6,150);
}
