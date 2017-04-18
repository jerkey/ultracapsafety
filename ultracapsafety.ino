const int voltpin = A0;  // pin number of resistors to measure voltage
const int relay = 8;  // pin number of transistor controlling disconnect relay
const int redpin = 10;   // pin number controlling red lights
const int bluepin = 9;  // pin number controlling blue lights
const int soundpin = 13; // pin goes to soundsystem to make noises
const int lowtone = 200;  // frequency of low voltage tone
const int hightone = 900;  // frequency of high voltage tone
const int waitrate = 50;  // milliseconds to wait between voltage measurements
const int howmany = 10;  // how many voltage measurements to take each time
// voltage and lights are updated after waitrate times howmany milliseconds

const float notonevoltage = 14.0;  // voltage below which NO tone happens
const float lowvoltagetone = 16.0;  // voltage below which low tone happens
const float lowvoltage = 17.0;  //voltage below which up arrow lights
const float highvoltage = 18.0;  //voltage above which down arrow lights
const float highvoltagetone = 19.0;  //voltage above which high tone happens
const float relayonvoltage = 20.0;  //voltage above which disconnect relay goes on
const float relayoffvoltage = 19.0;  //voltage above which disconnect relay goes on

const double voltcoeff = 29.0;  // coefficient of voltage measurement value

int digitalvalue[10];   // THE [50] IS THE ARRAY SIZE, ALSO CHECK THE LINE BELOW
const int digitalvaluesize = 10;   // THIS VALUE MUST MATCH THE SIZE OF THE ARRAY, ABOVE
int digitalvalueindex = 0;   //   THIS STUFF IS FOR VOLTAGE MEASUREMENT SMOOTHING
double digitalvalueadder;  //this is used to sum the array before division

double voltage;

void setup(){
  pinMode(voltpin,INPUT);
  digitalWrite(voltpin,LOW);
  
  digitalWrite(relay,LOW);
  pinMode(relay,OUTPUT);
  digitalWrite(redpin,LOW);
  pinMode(redpin,OUTPUT);
  digitalWrite(bluepin,LOW);
  pinMode(bluepin,OUTPUT);

  Serial.begin(9600);

  checkvoltage();

  Serial.print(voltage);
  Serial.print(" volts.  ADC value = ");
  Serial.println(analogRead(voltpin));
}

void loop(){
  getvoltage();
  if ((voltage < lowvoltagetone) && (voltage > notonevoltage)) tone(soundpin,lowtone);
  if (voltage > highvoltagetone) tone(soundpin,hightone);
  if (((voltage >= lowvoltagetone) && (voltage <= highvoltagetone)) || (voltage <= notonevoltage)) noTone(soundpin);
 
  if (voltage < lowvoltage) digitalWrite(redpin,HIGH); else digitalWrite(redpin,LOW);
  if (voltage > highvoltage) digitalWrite(bluepin,HIGH); else digitalWrite(bluepin,LOW);
  if (digitalRead(relay) && (voltage < relayoffvoltage)) digitalWrite(relay,LOW);
  if (!digitalRead(relay) && (voltage > relayonvoltage)) digitalWrite(relay,HIGH);
  Serial.println(voltage);
//  Serial.print(" volts.  ADC value = ");
//  Serial.println(analogRead(voltpin));
  delay(waitrate);
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

