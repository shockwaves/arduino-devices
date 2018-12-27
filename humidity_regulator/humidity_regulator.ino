// Depends on the following Arduino libraries:
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include "TM1637.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Module connection pins (Digital Pins)
#define CLK 3
#define DIO 2

#define DHTPIN 4 // Pin which is connected to the DHT sensor.

#define RELAY_MODULE 5

// Uncomment the type of sensor in use:
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// The amount of time (in milliseconds) between tests
#define TEST_DELAY 2000

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

//  Custom Parameters
boolean is_high_edge_state = false;

//float temperature = 100;
//float humidity = 100;
float temperature;
float humidity;

bool isIncrease;

int humidity_high_edge = 80;
int humidity_low_edge = 65;

int relay_state = LOW;

TM1637 disp(CLK, DIO);

void setup() {
    disp.set(BRIGHT_TYPICAL); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
                              //  disp.init(D4056A);//D4056A is the type of the module
    disp.init(D4036B);
    disp.point(1);

    Serial.begin(9600);
    // Initialize device.
    dht.begin();
    Serial.println("DHTxx Unified Sensor Example");
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Temperature");
    Serial.print("Sensor:       ");
    Serial.println(sensor.name);
    Serial.print("Driver Ver:   ");
    Serial.println(sensor.version);
    Serial.print("Unique ID:    ");
    Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    ");
    Serial.print(sensor.max_value);
    Serial.println(" *C");
    Serial.print("Min Value:    ");
    Serial.print(sensor.min_value);
    Serial.println(" *C");
    Serial.print("Resolution:   ");
    Serial.print(sensor.resolution);
    Serial.println(" *C");
    Serial.println("------------------------------------");
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Humidity");
    Serial.print("Sensor:       ");
    Serial.println(sensor.name);
    Serial.print("Driver Ver:   ");
    Serial.println(sensor.version);
    Serial.print("Unique ID:    ");
    Serial.println(sensor.sensor_id);
    Serial.print("Max Value:    ");
    Serial.print(sensor.max_value);
    Serial.println("%");
    Serial.print("Min Value:    ");
    Serial.print(sensor.min_value);
    Serial.println("%");
    Serial.print("Resolution:   ");
    Serial.print(sensor.resolution);
    Serial.println("%");
    Serial.println("------------------------------------");
    // Set delay between sensor readings based on sensor details.
    delayMS = sensor.min_delay / 1000;

    pinMode(RELAY_MODULE, OUTPUT);
    digitalWrite(RELAY_MODULE, relay_state);
    displayData(88, 88);
}

void displayData(int temperature, int humidity) {
    temperature = temperature > 99 ? 99 : temperature;
    humidity = humidity > 99 ? 99 : humidity;

    String result = String(temperature) + String(humidity);
    disp.display(result.toFloat());
}

void processTemperature() {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
}

void processHumidity() {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    if (humidity >= humidity_high_edge) {
        is_high_edge_state = true;
        relay_state = HIGH;
        Serial.println("Humidity: >= humidity_high_edge");
    } else if (is_high_edge_state && humidity <= humidity_low_edge) {
        is_high_edge_state = false;
        relay_state = LOW;
        Serial.println("Humidity: <= humidity_low_edge");
    }

    digitalWrite(RELAY_MODULE, relay_state);
    displayData(temperature, humidity);
}

void demoMode() {
    if (humidity <= 50) {
        isIncrease = true;
    }

    if (humidity >= 100) {
        isIncrease = false;
    }

    if (isIncrease) {
        humidity++;
        temperature++;
    } else {
        humidity--;
        temperature--;
    }
}

void loop() {
    delay(delayMS);
    sensors_event_t event;

    dht.temperature().getEvent(&event);
    temperature = event.temperature;

    dht.humidity().getEvent(&event);
    humidity = event.relative_humidity;

    //demoMode();

    if (isnan(temperature)) {
        Serial.println("Error reading temperature!");
        displayData(0, 0);
    } else {
        processTemperature();
    }

    if (isnan(humidity)) {
        Serial.println("Error reading humidity!");
        displayData(0, 0);
    } else {
        processHumidity();
    }
}
