uint8_t buf[8] = {0};  // Keyboard Report Buffer: 8 bytes

#define PIN_DIT A0
#define PIN_DA A1

//#define SERIAL_DEBUG  // for serial debug: remove //
// for actual HID: put //

struct keyType {
  char Key;
  byte Pin;
  byte HiD;
  byte currState = HIGH;
  byte prevState = HIGH;
  unsigned long prevTime = 0;
  unsigned long waitTime = 50;
};

const int NoOfKeys = 2;
keyType k[NoOfKeys];

void setup()
{
  Serial.begin(9600);
  k[0].Key = 'DIT';
  k[0].Pin = A0;
  k[0].HiD = 228;
  k[1].Key = 'DAH';
  k[1].Pin = A1;
  k[1].HiD =  224;
  for (int i = 0; i < NoOfKeys; i++)
         pinMode(k[i].Pin, INPUT_PULLUP);
    delay(200);
}

void loop()
{
  checkButton();
}


void checkButton() {
  for (int i = 0; i < NoOfKeys;i++) {
    byte currRead =   digitalRead(k[i].Pin);
    if (currRead != k[i].prevState) k[i].prevTime = millis();
    if ((millis() - k[i].prevTime) > k[i].waitTime) {
      if (currRead != k[i].currState) {
        k[i].currState = currRead;
        if (k[i].currState == LOW) {
          // Send the code
          buf[2+i] = k[i].HiD;    // HID: key
#ifdef SERIAL_DEBUG
          buf[2+i] = k[i].Key;     // Serial: key
#endif
          Serial.write(buf, 8); // Send keypress
        } else {
          // Release the keyboard
          buf[0] = 0;
          buf[2+i] = 0;
          Serial.write(buf, 8); // Release key
        }
      }
    }
    k[i].prevState = currRead;
  }
}