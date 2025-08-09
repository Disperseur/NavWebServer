#include "NMEA.h"

// fonctions utilisateur


void parserThreadEntryPoint(void* arg) {
  Nmea* bateau = (Nmea*)arg;

  USBHostSerialDevice hser(true); // a mettre en variable globale peut etre. actuellement pour que ca fontionne : alimenter la giga puis brancher le cable usb

  // Enable the USBHost
  pinMode(PA_15, OUTPUT);
  digitalWrite(PA_15, HIGH);

  while (!hser.connect()) {
    Serial1.println("No USB host Serial1 device connected");
    Serial.println("No USB host Serial1 device connected");
    delay(1000);
  }
  hser.begin(460800);

  Serial1.println("[USB PARSER] Service started.");
  Serial.println("[USB PARSER] Service started.");


  String trame;

  while(true) {
    trame = get_nmea_from_usbhost(hser);
    hser.flush();
    bateau->parse(trame);

    ThisThread::sleep_for(100);
  }
}



// String get_nmea_from_usbhost(USBHostSerialDevice &dev) {
//   String incomingLine = "";  // Buffer pour construire une ligne complète

//   while (dev.available()) {
//     char c = dev.read();

//     if (c == '\n') {  // Fin de trame NMEA
//       incomingLine.trim(); // enlève '\r' ou espaces

//       if (incomingLine.length() > 0) {
//         // On a une trame complète, on la revoie
// #ifdef DEBUG_NMEA
//         Serial1.println(incomingLine); // pour le log avec le pc
// #endif
//         return incomingLine;
//       }

//       incomingLine = ""; // Reset pour la prochaine trame
//     } else {
//       incomingLine += c; // Ajoute le caractère au buffer
//     }
//   }
// }

String get_nmea_from_usbhost(USBHostSerialDevice &dev) {
  static String incomingLine = "";  // Conserve les caractères entre les appels

  while (dev.available()) {
    char c = dev.read();

    if (c == '\n') {
      incomingLine.trim();  // supprime '\r' et espaces

      if (incomingLine.length() > 0) {
#ifdef DEBUG_NMEA
        Serial1.println(incomingLine);
        Serial.println(incomingLine);

#endif
        String complete = incomingLine;
        incomingLine = "";  // reset pour prochaine trame
        return complete;
      }

      incomingLine = "";  // réinitialise si la ligne était vide
    } else {
      incomingLine += c;
    }
  }

  return "";  // rien à renvoyer pour le moment
}





String millisToTimeString(unsigned long ms) {
  unsigned long totalSeconds = ms / 1000;
  unsigned int hours = totalSeconds / 3600;
  unsigned int minutes = (totalSeconds % 3600) / 60;
  unsigned int seconds = totalSeconds % 60;

  // Formater avec des zéros devant si nécessaire
  char buffer[9]; // "HH:MM:SS"
  sprintf(buffer, "%02u:%02u:%02u", hours, minutes, seconds);
  return String(buffer);
}



Nmea::Nmea()
: ground_speedKts_avg(1800), water_speedKts_avg(1800) {

}



int Nmea::parse(String nmea) {
  running_time = millisToTimeString(millis()); // fix hardfault ?

  if(nmea.lastIndexOf("$") != 0 || nmea.indexOf("!") != -1) return -1; // gere les chevauchement de trame nmea par une autre ou par une trame AIS

  if(nmea.indexOf("GPRMC") == 1) {
    getGPRMCData(nmea);
  }
  else if(nmea.indexOf("SDDBT") == 1) {
    getSDDBTData(nmea);
  }
  else if(nmea.indexOf("VWVHW") == 1) {
    getVWVHWData(nmea);
  }
  else if(nmea.indexOf("WIMTW") == 1) {
    getWIMTWData(nmea);
  }
  else if(nmea.indexOf("WIMWV") == 1) {
    getWIMWVData(nmea);
  }
}






void Nmea::getGPRMCData(String nmea) {
  GPRMC_Data result = parseGPRMC(nmea);

  if(result.data_valid) {
    set_ground_time(result.ground_time);
    set_ground_sensorStatus(result.ground_sensorStatus);
    set_ground_latitude(result.ground_latitude);
    set_ground_latDir(result.ground_latDir);
    set_ground_longitude(result.ground_longitude);
    set_ground_longDir(result.ground_lonDir);
    set_ground_speedKts(result.ground_speedKts);
    set_ground_course(result.ground_course);
    set_ground_date(result.ground_date);

    ground_speedKts_avg.addSample(result.ground_time, result.ground_speedKts);
    ground_speedKts_avg.computeAverage();
  } else {
    //
  }
}

void Nmea::getSDDBTData(String nmea) {
  SDDBT_Data result = parseSDDBT(nmea);

  if(result.data_valid) {
    set_water_depthMeters(result.water_depthMeters);
  } else {
    //
  }
}

void Nmea::getVWVHWData(String nmea) {
  VWVHW_Data result = parseVWVHW(nmea);

  if(result.data_valid) {
    set_water_speedKts(result.water_speedKts);

    water_speedKts_avg.addSample(ground_time, result.water_speedKts);
    water_speedKts_avg.computeAverage();
  } else {
    //
  }
}

void Nmea::getWIMTWData(String nmea) {
  WIMTW_Data result = parseWIMTW(nmea);

  if(result.data_valid) {
    set_water_temperatureCelsius(result.water_temperatureCelsius);
  } else {
    //
  }
}

void Nmea::getWIMWVData(String nmea) {
  WIMWV_Data result = parseWIMWV(nmea);

  if(result.data_valid) {
    set_wind_angle(result.wind_angle);
    set_wind_angleReference(result.wind_angleReference);
    set_wind_speedKts(result.wind_speedKts);
    set_wind_sensorStatus(result.wind_sensorStatus);
  } else {
    //
  }
}



// void Nmea::printData(void) {
//   Serial1.println("");
//   // Serial1.println("Statut anemometre: " + String(bateau.get_wind_sensorStatus()));
//   Serial1.println("Statut GPS: " + String(get_ground_sensorStatus()));

//   Serial1.println("");
//   Serial1.println("Date: " + String(get_ground_date()));
//   Serial1.println("Heure UTC: " + get_ground_time());
//   Serial1.println("Temps de navigation: " + String(get_running_time()) + " s");

//   Serial1.println("");
//   Serial1.println("Latitude: " + get_ground_latitude() + " " + String(get_ground_latDir()));
//   Serial1.println("Longitude: " + get_ground_longitude() + " " + String(get_ground_longDir()));
//   Serial1.println("Vitesse sol: " + String(get_ground_speedKts()) + " kt");
//   Serial1.println("Cap: " + String(get_ground_course()) + "°");
  
//   Serial1.println("");
//   Serial1.println("Profondeur sous quille : " + String(get_water_depthMeters()) + " m");
//   Serial1.println("Vitesse dans l'eau: " + String(get_water_speedKts()) + " kt");
//   Serial1.println("Temperature de l'eau: " + String(get_water_temperatureCelsius()) + " °C");

//   Serial1.println("");
//   Serial1.println("Direction du vent: " + String(get_wind_angle()) + "°");
//   Serial1.println("Vitesse du vent: " + String(get_wind_speedKts()) + " kt");
// }

// Mutateurs et accesseurs

int Nmea::set_ground_time(String t) {
  ground_time = t;
}

int Nmea::set_ground_date(String d) {
  ground_date = d;
}

int Nmea::set_wind_sensorStatus(char s) {
  wind_sensorStatus = s;
}

int Nmea::set_ground_sensorStatus(char s) {
  ground_sensorStatus = s;
}

int Nmea::set_ground_latitude(String lat) {
  ground_latitude = lat;
}

int Nmea::set_ground_latDir(char latD) {
  ground_latDir = latD;
}

int Nmea::set_ground_longitude(String lon) {
  ground_longitude = lon;
}

int Nmea::set_ground_longDir(char lonD) {
  ground_longDir = lonD;
}

int Nmea::set_ground_speedKts(float s) {
  ground_speedKts = s;
}

int Nmea::set_ground_course(float c) {
  ground_course = c;
}

int Nmea::set_water_depthMeters(float d) {
  water_depthMeters = d;
}

int Nmea::set_water_speedKts(float s) {
  water_speedKts = s;
}

int Nmea::set_water_temperatureCelsius(float t) {
  water_temperatureCelsius = t;
}

int Nmea::set_wind_angle(float a) {
  wind_angle = a;
}

int Nmea::set_wind_angleReference(char ar) {
  wind_angleReference = ar;
}

int Nmea::set_wind_speedKts(float s) {
  wind_speedKts = s;
}

void Nmea::set_pressure_alarm(bool s) {
  pressure_alarm = s;
}

 void Nmea::set_depth_alarm(bool s) {
  depth_alarm = s;
 }



String Nmea::get_running_time(void) {
  return running_time;
}

String Nmea::get_ground_time(void) {
  return ground_time;
}

String Nmea::get_ground_date(void) {
  return ground_date;
}

char Nmea::get_wind_sensorStatus(void) {
  return wind_sensorStatus;
}

char Nmea::get_ground_sensorStatus(void) {
  return ground_sensorStatus;
}

String Nmea::get_ground_latitude(void) {
  return ground_latitude;
}

char Nmea::get_ground_latDir(void) {
  return ground_latDir;
}

String Nmea::get_ground_longitude(void) {
  return ground_longitude;
}

char Nmea::get_ground_longDir(void) {
  return ground_longDir;
}

float Nmea::get_ground_speedKts(void) {
  return ground_speedKts;
}

float Nmea::get_ground_course(void) {
  return ground_course;
}

float Nmea::get_water_depthMeters(void) {
  return water_depthMeters;
}

float Nmea::get_water_speedKts(void) {
  return water_speedKts;
}

float Nmea::get_water_temperatureCelsius(void) {
  return water_temperatureCelsius;
}

float Nmea::get_wind_angle(void) {
  return wind_angle;
}

char Nmea::get_wind_angleReference(void) {
  return wind_angleReference;
}

float Nmea::get_wind_speedKts(void) {
  return wind_speedKts;
}


float Nmea::get_ground_speedKts_avg(void) {
  return ground_speedKts_avg.getAverage();
}

float Nmea::get_water_speedKts_avg(void) {
  return water_speedKts_avg.getAverage();
}

bool Nmea::get_pressure_alarm(void) {
  return pressure_alarm;
}

bool Nmea::get_depth_alarm(void) {
  return depth_alarm;
}