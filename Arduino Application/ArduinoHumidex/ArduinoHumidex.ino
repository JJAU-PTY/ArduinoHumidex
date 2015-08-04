/*
 * Created: August 4, 2015 by Joaquin Arosemena (joaquin.arosemena.507@gmail.com)
 * LIVE Example: https://thingspeak.com/channels/49870
 * References
 * https://learn.adafruit.com/memories-of-an-arduino/optimizing-sram
 * https://learn.adafruit.com/dht
 * https://learn.sparkfun.com/tutorials/bmp180-barometric-pressure-sensor-hookup-?_ga=1.102655620.480421976.1434405733
 * https://thingspeak.com/
 * http://www.engineeringtoolbox.com/absolute-humidity-air-d_681.html
 * http://www.best-microcontroller-projects.com/dht22.html#Dew_Point
 * https://www.safaribooksonline.com/library/view/arduino-cookbook/9781449399368/ch04.html
 * http://www.instructables.com/id/Send-sensor-data-DHT11-BMP180-to-ThingSpeak-with-a/step2/The-Program/
 * http://www.ccohs.ca/oshanswers/phys_agents/humidex.html
 * https://www.electronicsblog.net/arduino-based-temperature-and-humidityhumidex-meter-with-dht22-sensor-and-color-lcd/
 */

//Ethernet libraries
#include <SPI.h>
#include <Ethernet.h>
EthernetClient client;

//Sensors libraries
#include "DHT.h"
#include <SFE_BMP180.h>
#include <Wire.h>
#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11 Selector
SFE_BMP180 pressure;
#define ALTITUDE 12.0 // Set your current altitude in meters here
DHT dht(DHTPIN, DHTTYPE);

//Custom Made Struct Lib
#include "MyTypes.h"
STHumidex THumidex;

//Ethernet Variables
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network
String writeAPIKey = "PUT-API-KEY-HERE"; //Put your API Key between the quotes
char thingSpeakAddress[] = "api.thingspeak.com";
const int updateThingSpeakInterval = 16 * 1000;
long lastConnectionTime = 0;
boolean lastConnected = false;
int failedCounter = 0;
char textserver;
unsigned int WaitTime = 10000; //Aditional wait time to the obligatory 20 seconds on the code

//Characters Buffers To Convert from Double
char T_buffer[15]; // Temperature Buffer
char P_buffer[15]; // Absolute Pressure Buffer
//char p0_buffer[15]; // Sea Level Pressure Buffer (Not Using)
char h_buffer[15]; // Humidity Buffer
char Pt_buffer[15]; // Dew Point Buffer
char humidex_buffer[15]; // Humidex Buffer
//Strings to send to the Thingspeak server
String T_send; // Temperature String
String P_send; // Absolute Pressure String
//String p0_send; // Sea Level Pressure String (Not Using)
String h_send; // Humidity String
String Pt_send; // Dew Point String
String humidex_send; // Humidex String

//LEDs Declaration
byte ledR = 3;
byte ledG = 6;
byte ledB = 5;

//Sensors Variables
double h, t, humidex, T, P, p0, Pt;
char status;



void setup() {
  // ----------------------------------
  //Initial Boot, it turns the LEDS ON on the RED, GREEN and BLUE order
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  digitalWrite(ledR, HIGH);   // set the RED LED on
  delay(250);              // wait for a second
  digitalWrite(ledR, LOW);    // set the RED LED off
  delay(250);              // wait for a second
  digitalWrite(ledG, HIGH);   // set the GREEN LED on
  delay(250);              // wait for a second
  digitalWrite(ledG, LOW);    // set the GREEN LED off
  delay(250);              // wait for a second
  digitalWrite(ledB, HIGH);   // set the BLUE LED on
  delay(250);              // wait for a second
  digitalWrite(ledB, LOW);    // set the BLUE LED off
  delay(250);              // wait for a second

  // ----------------------------------
  //Serial BOOT
  Serial.begin(9600);
  Serial.println(F("Sensor TEST!"));

  // ----------------------------------
  //BMP180 and DHT sensors Test Start
  dht.begin();
  if (pressure.begin())
    Serial.println(F("BMP180 init success"));
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println(F("BMP180 init fail\n\n"));
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, HIGH);
    delay(2000);
    while (1); // Pause forever.
  }
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.print(F("Failed to read from DHT sensor!"));
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, HIGH);
    delay(2000);
    return;
  }
  //Serial Print of Sensors
  Serial.print(F("DHT Humidity: "));
  Serial.print(h);
  Serial.println(F(" %"));
  Serial.print(F("DHT Temperature: "));
  Serial.print(t);
  Serial.println(F(" *C"));
  Serial.print(F("BMP180 provided altitude: "));
  Serial.print(ALTITUDE, 0);
  Serial.println(F(" meters"));
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.
    delay(1000);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print(F("BMP180 temperature: "));
      Serial.print(T, 2);
      Serial.println(F(" deg C"));

      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.
        delay(1000);
        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print(F("Absolute pressure: "));
          Serial.print(P, 2);
          Serial.println(F(" mb"));


          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P, ALTITUDE); // Esta a 0 m [we're at 1655 meters (Boulder, CO)]
          Serial.print(F("Relative (sea-level) pressure: "));
          Serial.print(p0, 2);
          Serial.println(F(" mb"));

        }
        else Serial.println(F("error retrieving pressure measurement\n"));
      }
      else Serial.println(F("error starting pressure measurement\n"));
    }
    else Serial.println(F("error retrieving temperature measurement\n"));
  }
  else Serial.println(F("error starting temperature measurement\n"));

  // ----------------------------------
  //Here ends the code from the sensors examples
  //Now we compare the temperature sensors for a big variation
  Serial.print(F("Diference in temperature between BMP180-DHT sensors: "));
  Serial.println((T - t));
  if ((T - t) > 2) {
    Serial.print(F("Diference exceeds 2 degrees, consider checking the sensors"));
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, HIGH);
  }
  //Print of Dewpoint and Humidex values
  Pt = dewPoint(T, h);
  Serial.print(F("Dew Point: "));
  Serial.println(Pt);
  humidex = calculate_humidex(T, h);
  Serial.print(F("Humidex: "));
  Serial.println(humidex);
  TCHumidex(humidex);
  Serial.print(F("Comfort: "));
  Serial.println(THumidex.Sens);
  Serial.println(F("\t"));
  delay(1000); //1 Second Pause
  startEthernet();
}

void loop() {
  // ----------------------------------
  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println(F("...desconectado.\n"));
    client.stop();
  }
  delay(15000); //15 seconds delay (Obligatory delay for the Thingspeak server)
  delay(WaitTime); //Aditional delay

  // ----------------------------------
  //Boot of BMP180
  if (pressure.begin()) {
  }
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println(F("BMP180 init fail\n\n"));
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, HIGH);
    delay(2000);
    while (1); // Pause forever.
  }

  //BMP180 Reading
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
  }
  status = pressure.getTemperature(T);
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
  }
  status = pressure.startPressure(3);
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
  }
  status = pressure.getPressure(P, T);
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
  }
  p0 = pressure.sealevel(P, ALTITUDE);

  //DHT Reading
  h = dht.readHumidity();
  t = dht.readTemperature();
  //DHT Status
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledB, HIGH);
    delay(2000);
    return;
  }

  // ----------------------------------
  //Values Serial Print
  Serial.print(F("Absolute pressure: "));
  Serial.print(P, 2);
  Serial.println(F(" mb"));
  Serial.print(F("Relative (sea-level) pressure: "));
  Serial.print(p0, 2);
  Serial.println(F(" mb"));
  Serial.print(F("BMP180 temperature: "));
  Serial.print(T);
  Serial.println(F(" deg C"));
  Serial.print(F("DHT Temperature: "));
  Serial.println(t);
  Serial.print(F("DHT Humidity: "));
  Serial.println(h);
  Serial.print(F("Diference in temperature between BMP180-DHT sensors: "));
  Serial.println((T - t));
  Pt = dewPoint(T, h);
  Serial.print(F("Calculated Dew Point: "));
  Serial.println(Pt);
  humidex = calculate_humidex(T, h);
  Serial.print(F("Calculated Humidex: "));
  Serial.println(humidex);
  TCHumidex(humidex);
  Serial.print(F("Comfort: "));
  Serial.println(THumidex.Sens);
  //LED Ilumination
  analogWrite(ledR, THumidex.R);
  analogWrite(ledG, THumidex.G);
  analogWrite(ledB, THumidex.B);
  delay(500); //Half Second delay

  // ----------------------------------
  //Conversion of Double values to Char and then to String
  T_send = dtostrf(T, 0, 5, T_buffer);
  P_send = dtostrf(P, 0, 5, P_buffer);
  //  p0_send = dtostrf(P, 0, 5, p0_buffer); (Not Using)
  h_send = dtostrf(h, 0, 5, h_buffer);
  Pt_send = dtostrf(Pt, 0, 5, Pt_buffer);
  humidex_send = dtostrf(humidex, 0, 5, humidex_buffer);
  delay(500); //Pausa medio segundo para permitir conversion correcta

  // ----------------------------------
  //Update ThingSpeak
  if (!client.connected())
  {
    updateThingSpeak("field1=" + T_send + "&field2=" + P_send + "&field3=" + h_send + "&field4=" + Pt_send + "&field5=" + humidex_send);
  }
  // Check if Arduino Ethernet needs to be restarted
  if (failedCounter > 3 ) {
    startEthernet();
  }
  lastConnected = client.connected();

  delay(5000); //Pausa obligatoria para dejar que el Ethernet Shield se comunique bien con el arduino
  if (client.available()) {
    while (client.available()) {
      textserver = client.read();
      Serial.print(textserver); //Print de datos enviados del servidor al arduino
    }

  }
  else {
    Serial.println(F("No ethernet data"));
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, HIGH);
  }
}

