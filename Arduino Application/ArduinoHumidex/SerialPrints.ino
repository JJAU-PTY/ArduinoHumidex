//Optional Void to print the serial values of the sensors
void SerialPrints() {
  //Values Serial Print
  Serial.print(F("Absolute pressure: "));
  Serial.print(P, 2);
  Serial.println(F(" mb"));
  Serial.print(F("Relative (sea-level) pressure: "));
  Serial.print(p0, 2);
  Serial.println(F(" mb"));
  
  Serial.print(F("BMP180 temperature: "));
  Serial.print(T, 2);
  Serial.print(F(" deg C, "));
  Serial.print((9.0 / 5.0)*T + 32.0, 2);
  Serial.println(F(" deg F"));
  
  Serial.print(F("DHT Temperature: "));
  Serial.print(t);
  Serial.print(F(" deg C, "));
  Serial.print((9.0 / 5.0)*t + 32.0, 2);
  Serial.println(F(" deg F"));
  
  Serial.print(F("DHT Humidity: "));
  Serial.println(h);
  Serial.print(F("Diference in temperature between BMP180-DHT sensors: "));
  Serial.println((T - t));

  Serial.print(F("Calculated Dew Point: "));
  Serial.print(Pt);
  Serial.print(F(" deg C, "));
  Serial.print((9.0 / 5.0)*Pt + 32.0, 2);
  Serial.println(F(" deg F"));

  Serial.print(F("Calculated Humidex: "));
  Serial.println(humidex);

  Serial.print(F("Comfort: "));
  Serial.println(THumidex.Sens);
}

