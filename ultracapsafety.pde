const int voltpin = A0;  // pin number of resistors to measure voltage
const int brightlight = 8;  // pin number of transistor controlling bright light
const int uparrow = 9;   // pin number controlling up arrow
const int downarrow = 10;  // pin number controlling down arrow
const int soundpin = 13; // pin goes to soundsystem to make noises
const int lowtone = 200;  // frequency of low voltage tone
const int hightone = 900;  // frequency of high voltage tone
const int waitrate = 50;  // milliseconds to wait between voltage measurements
const int howmany = 10;  // how many voltage measurements to take each time
// voltage and lights are updated after waitrate times howmany milliseconds

const float lowvoltagetone = 10.75;  // voltage below which low tone happens
const float lowvoltage = 11.25;  //voltage below which up arrow lights
const float highvoltage = 13.2;  //voltage above which down arrow lights
const float highvoltagetone = 13.4;  //voltage above which high tone happens
const float brightlightvoltage = 13.5;  //voltage above which bright light goes on

const double voltcoeff = 29.0;  // coefficient of voltage measurement value

int digitalvalue[10];   // THE [50] IS THE ARRAY SIZE, ALSO CHECK THE LINE BELOW
const int digitalvaluesize = 10;   // THIS VALUE MUST MATCH THE SIZE OF THE ARRAY, ABOVE
int digitalvalueindex = 0;   //   THIS STUFF IS FOR VOLTAGE MEASUREMENT SMOOTHING
double digitalvalueadder;  //this is used to sum the array before division

double voltage;

void setup(){
  pinMode(voltpin,INPUT);
  digitalWrite(voltpin,LOW);
  
  digitalWrite(brightlight,LOW);
  pinMode(brightlight,OUTPUT);
  digitalWrite(uparrow,LOW);
  pinMode(uparrow,OUTPUT);
  digitalWrite(downarrow,LOW);
  pinMode(downarrow,OUTPUT);
  checkvoltage();
  Serial.begin(9600);
}

void loop(){
  getvoltage();
  if (voltage < lowvoltagetone) tone(soundpin,lowtone);
  if (voltage > highvoltagetone) tone(soundpin,hightone);
  if ((voltage >= lowvoltagetone) & (voltage <= highvoltagetone)) noTone(soundpin);
  if (voltage < lowvoltage) digitalWrite(uparrow,HIGH); else digitalWrite(uparrow,LOW);
  if (voltage > highvoltage) digitalWrite(downarrow,HIGH); else digitalWrite(downarrow,LOW);
  if (voltage > brightlightvoltage) digitalWrite(brightlight,HIGH); else digitalWrite(brightlight,LOW);
  Serial.print(analogRead(voltpin));
  Serial.print("   ");
  Serial.println(voltage);
  delay(waitrate);
//  tone(soundpin,lowtone);
//  delay(500);
//  tone(soundpin,hightone);
//  delay(500);
//  noTone(soundpin);
//  delay(500);  
}

void checkvoltage(){
    voltage = 0;
for (int pos = 0; pos < howmany; pos++) {
  voltage += analogRead(voltpin) / voltcoeff;
  delay(waitrate);
  }
}

void getvoltage()
{
  digitalvalue[digitalvalueindex] = analogRead(voltpin);
  digitalvalueindex +=1;
  if (digitalvalueindex >= digitalvaluesize) digitalvalueindex = 0;
  digitalvalueadder = 0;
  for(int adder = 0 ; adder < (digitalvaluesize-1); adder +=1) {
    digitalvalueadder += digitalvalue[adder];
  };
  voltage = digitalvalueadder / (digitalvaluesize-1) / voltcoeff; // 10Kohm and 1Kohm voltage divider guess
}

