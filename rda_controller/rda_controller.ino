/*
  RDA Controller
*/

#include <IRremote.h>

#define RDA 2

IRrecv irrecv(RDA);
decode_results responseRDA;

unsigned int valueRDA = 0;

typedef struct {
    uint8_t id;
    bool state;
} idState;

idState pins[] = {
    {3, true},
    {4, true},
    {5, true}
};

void setup()
{
    for (int i = 0; i <= sizeof(pins) / sizeof(pins[0]); i++) {
        pinMode(pins[i].id, OUTPUT);
    }

    Serial.begin(9600);
    Serial.println("Enabling IRin");
    irrecv.enableIRIn(); // Start the receiver
    Serial.println("Enabled IRin");
}

void updateState(int indx) {
    pins[indx].state = !pins[indx].state;
    digitalWrite(pins[indx].id, pins[indx].state ? HIGH : LOW);
}

void loop() {
    if (irrecv.decode(&responseRDA)) {
        valueRDA = responseRDA.value;
        //    Serial.println(valueRDA);

        switch (valueRDA) {
        case 41565:
            Serial.println("1");
            updateState(0);
            break;
        case 25245:
            Serial.println("2");
            updateState(1);
            break;
        case 57885:
            Serial.println("3");
            updateState(2);
            break;
        }

        irrecv.resume(); // Receive the next value
    }

    delay(100);
}
