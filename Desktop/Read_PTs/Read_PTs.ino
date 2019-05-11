#include <NAU7802.h>
NAU7802 adc1 = NAU7802();
NAU7802 adc2 = NAU7802();
NAU7802 adc3 = NAU7802();

#include <Wire.h>

#define TCAADDR 0x70
#define PT_READ_INERVAL 100
#define SYNC_HEADER 0b11001111
#define LAST_BYTE 0xFF
#define BITS_PER_BYTE 8

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup(void)
{
  Serial1.begin(9600);
  Serial1.println("Starting"); Serial1.println("");

  /* Initialise the 1st sensor */
  tcaselect(2);
  if (!adc1.begin())
  {
    /* There was a problem detecting the NAU7802 ... check your connections */
    Serial1.println("Ooops, no adc1 detected ... Check your wiring!");
    while (1);
  }
  adc1.selectCh1();

  /* Initialise the 2nd sensor */
  tcaselect(4);
  if (!adc2.begin())
  {
    /* There was a problem detecting the NAU7802 ... check your connections */
    Serial1.println("Ooops, no adc2 detected ... Check your wiring!");
    while (1);
  }
  adc2.selectCh1();

  /* Initialise the 3rd sensor */
  tcaselect(6);
  if (!adc3.begin())
  {
    /* There was a problem detecting the NAU7802 ... check your connections */
    Serial1.println("Ooops, no adc3 detected ... Check your wiring!");
    while (1);
  }
  adc3.selectCh2();

  Serial1.println("ADCs Initialized!");
}

void loop(void)
{
  static unsigned long lastTime = millis();

  // read the adc1 and write the bytes to Serial1
  if (millis() - lastTime > PT_READ_INERVAL) {
    lastTime = millis();

    tcaselect(2);
    long adcValue = adc1.readADC();
    Serial1.write(SYNC_HEADER);
    Serial1.write(sizeof(long));
    for (int i = 0; i < sizeof(long); i++) {
      Serial1.write(adcValue & LAST_BYTE);
      adcValue >>= BITS_PER_BYTE;
    }

    tcaselect(4);
    long adcValue = adc2.readADC();
    Serial1.write(SYNC_HEADER);
    Serial1.write(sizeof(long));
    for (int i = 0; i < sizeof(long); i++) {
      Serial1.write(adcValue & LAST_BYTE);
      adcValue >>= BITS_PER_BYTE;
    }

    tcaselect(6);
    adcValue = adc3.readADC();
    Serial1.write(SYNC_HEADER);
    Serial1.write(sizeof(long));
    for (int i = 0; i < sizeof(long); i++) {
      Serial1.write(adcValue & LAST_BYTE);
      adcValue >>= BITS_PER_BYTE;
    }
  }
}
