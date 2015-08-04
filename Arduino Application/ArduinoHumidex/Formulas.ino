// Dew Point
// See more at: http://www.best-microcontroller-projects.com/dht22.html#Dew_Point

double dewPoint(double celsius, double humidity) {
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

  // (2) DEWPOINT = F(Vapor Pressure)
  double TV = log(VP / 0.61078); // temp var
  return ((241.88 * TV) / (17.558 - TV));
}

//Humidex Calculation
//https://www.electronicsblog.net/arduino-based-temperature-and-humidityhumidex-meter-with-dht22-sensor-and-color-lcd/

double calculate_humidex(double temperature, double humidity) {
  double e;
  e = (6.112 * pow(10, (7.5 * temperature / (237.7 + temperature))) * humidity / 100); //vapor pressure
  double humix = temperature + 0.55555555 * (e - 10.0); //humidex
  return humix;
}


