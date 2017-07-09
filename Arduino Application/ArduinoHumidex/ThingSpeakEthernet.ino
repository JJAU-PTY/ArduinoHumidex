/*
 * http://community.thingspeak.com/arduino/ThingSpeakClient.pde
 * http://www.instructables.com/id/Send-sensor-data-DHT11-BMP180-to-ThingSpeak-with-a/step2/The-Program/
 */

void startEthernet()
{
  client.stop();
  Serial.print(F("Connecting Arduino to network..."));
  Serial.println();
  delay(1000);
  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println(F("DHCP Failed, reset Arduino to try again"));
    Serial.println();
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, LOW);
    digitalWrite(ledB, HIGH);
  }
  else
  {
    Serial.print(F("Arduino connected to network using DHCP. "));
  }
  Serial.print(F("My IP address: "));
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println("\n");
}

void updateThingSpeak(String tsData) {
  if (client.connect(thingSpeakAddress, 80))
  {
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
    lastConnectionTime = millis();
    if (client.connected())
    {
      Serial.print(F("Sending to ThingSpeak...\n\n"));
      failedCounter = 0;
    }
    else
    {
      failedCounter++;
      Serial.println("Connection to ThingSpeak failed (" + String(failedCounter, DEC) + ")");
      Serial.println();
    }
  }
  else
  {
    failedCounter++;
    Serial.println("Connection to ThingSpeak Failed (" + String(failedCounter, DEC) + ")");
    Serial.println();
    lastConnectionTime = millis();
  }
}

