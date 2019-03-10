#define LED_PIN 2
#define RELAY_PIN 3

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(9600);
}
// https://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


bool led_on = false;
int led_pwm = 0;
bool relay_on = false;

void processSerial() {  
  if (Serial.available() == 0) {
    return;
  }
  
  String data = Serial.readString();

  if (data.startsWith("LED ")) {
    String value = getValue(data, ' ', 1);
    if (value.startsWith("OFF")) {
      Serial.write("led off");
      led_on = false;
      return;
    }
    int pwm = value.toInt();
    led_on = true;
    led_pwm = pwm;
  }

  
  if (data.startsWith("RELAY ")) {
    String value = getValue(data, ' ', 1);
    if (value.startsWith("OFF")) {
      relay_on = false;
      Serial.write("relay off");
      digitalWrite(RELAY_PIN, LOW);
      return;
    }
    if (value.startsWith("ON")) {
      Serial.write("relay on");    
      digitalWrite(RELAY_PIN, HIGH);
      relay_on = true;
      return;
    }
  }
  
  Serial.println("UNHANDLED DATA");
  Serial.println(data);
}

void handleLed() {
  if (!led_on) {
    digitalWrite(2, LOW);
    return;
  }
  digitalWrite(2, LOW);
  delayMicroseconds(led_pwm);
  digitalWrite(2, HIGH);
} 

void handleRelay() {
  if (!relay_on) {
    digitalWrite(RELAY_PIN, LOW);
    return;
  }
  digitalWrite(2, LOW);
  delayMicroseconds(led_pwm);
  digitalWrite(2, HIGH);
} 

void loop() {
  processSerial();
  handleLed();
}
