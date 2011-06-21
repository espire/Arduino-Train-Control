int statusPin = 12;
int ledPin = 11;

volatile byte rpmCount;
volatile int rpmStatus;

unsigned int rpm;

unsigned long timeOld;

void rpm_fun() {
  rpmCount++;
  Serial.println("interrupted");
  if(rpmStatus == LOW) {
    rpmStatus = HIGH;
  }
  else {
    rpmStatus = LOW;
  }
  digitalWrite(statusPin,rpmStatus);
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(0,rpm_fun,FALLING);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(statusPin,OUTPUT);
  
  rpmCount = 0;
  rpm = 0;
  timeOld = 0;
  rpmStatus = LOW;
}

void loop() {
  delay(1000);
  detachInterrupt(0);
  rpm = 30*1000/(millis() - timeOld)*rpmCount;
  timeOld = millis();
  rpmCount = 0;
  
  Serial.println(rpm,DEC);
  attachInterrupt(0, rpm_fun, FALLING);
}
