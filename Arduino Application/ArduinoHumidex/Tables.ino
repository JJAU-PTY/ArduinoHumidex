/*
 * HUMIDEX Comparative Tables
 * This includes the LEDs Colors values
 * http://www.ccohs.ca/oshanswers/phys_agents/humidex.html
 * http://www.ccacac.com/wp-content/uploads/2010/06/Humidex-Graph.pdf
 */

void TCHumidex (double Hdex) {
  if (Hdex <= 23.00) {
    THumidex.R = 0;
    THumidex.G = 0;
    THumidex.B = 255;
    THumidex.Sens = (F("Unconfortable"));
  }
  if ((Hdex > 23.00) && (Hdex <= 26.00)) {
    THumidex.R = 0;
    THumidex.G = 255;
    THumidex.B = 255;
    THumidex.Sens = (F("Very comfortable"));
  }
  if ((Hdex > 26.00) && (Hdex <= 30.00)) {
    THumidex.R = 0;
    THumidex.G = 255;
    THumidex.B = 128;
    THumidex.Sens = (F("Comfortable"));
  }
  if ((Hdex > 30.00) && (Hdex <= 39.00)) {
    THumidex.R = 64;
    THumidex.G = 192;
    THumidex.B = 0;
    THumidex.Sens = (F("Disconfort / Hot Feeling"));
  }
  if ((Hdex > 39.00) && (Hdex <= 45.00)) {
    THumidex.R = 192;
    THumidex.G = 128;
    THumidex.B = 0;
    THumidex.Sens = (F("Great disconfort / Avoid exertion"));
  }
  if ((Hdex > 45.00) && (Hdex <= 54.00)) {
    THumidex.R = 255;
    THumidex.G = 64;
    THumidex.B = 0;
    THumidex.Sens = (F("Exesive heat / Dangerous"));
  }
  if (Hdex > 54.00) {
    THumidex.R = 255;
    THumidex.G = 0;
    THumidex.B = 0;
    THumidex.Sens = (F("Heat Stroke imminent"));
  }
  /*
   * Code to force a color or text
   * else {
   * R=255;
   * G=255;
   * B=255;
   * Sens=("Forced");}}
   */
}

