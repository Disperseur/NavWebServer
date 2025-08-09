#include "Alarm.h"


void buzzer_bip(int n, int p) {
    /*Runs a bip n times a sequence of p*/
    for(int i=0; i<n; i++) {
        for(int j=0; j<p; j++) {
            digitalWrite(BUZZER_PIN, HIGH);
            ThisThread::sleep_for(200);
            digitalWrite(BUZZER_PIN, LOW);
            ThisThread::sleep_for(200);
        }
        ThisThread::sleep_for(400);
    }
    ThisThread::sleep_for(1000);
}


void buzzerAlarmWatcherEntryPoint(void *arg)
{
    Nmea *bateau = (Nmea *)arg;

    pinMode(BUZZER_PIN, OUTPUT); // buzzer pin
    digitalWrite(BUZZER_PIN, LOW);

    bool depth_alarm_has_ringed = false;
    bool pressure_alarm_has_ringed = false;

    while (true) {
        if (bateau->get_depth_alarm() && !depth_alarm_has_ringed) {
            buzzer_bip(3, 2);
            depth_alarm_has_ringed = true;
        } else if (bateau->get_pressure_alarm() && !pressure_alarm_has_ringed) {
            buzzer_bip(3, 3);
            pressure_alarm_has_ringed = true;
        } else if (!bateau->get_pressure_alarm() && pressure_alarm_has_ringed) {
            pressure_alarm_has_ringed = false;
        } else if (!bateau->get_depth_alarm() && depth_alarm_has_ringed) {
            depth_alarm_has_ringed = false;
        }
        ThisThread::sleep_for(1000);
    }
}

void depthAlarmThreadEntryPoint(void *arg)
{
    Nmea *bateau = (Nmea *)arg;

    float old_depth = bateau->get_water_depthMeters();

    while (true)
    {
        if ((bateau->get_water_depthMeters() < 2.0) && (old_depth > 2.0))
        {
            bateau->set_depth_alarm(true);
        }

        old_depth = bateau->get_water_depthMeters();

        ThisThread::sleep_for(2000);
    }
}

void pressureAlarmThreadEntryPoint(void *arg)
{
    Nmea *bateau = (Nmea *)arg;

    Adafruit_BME280 bme; // I2C

    int status = bme.begin(0x76, &Wire1);
    if (!status)
    {
        while (1)
        {
            Serial1.println("Erreur d'initialisation du bme");
            delay(10);
        }
    }

    float oldPress = bme.readPressure(); // en Pa
    bateau->set_pressure_alarm(false);

    Serial1.println("[ALARM] Service started.");

    while (true)
    {
#ifdef DEBUG_ALARM
        Serial1.print("[ALARME] Temperature mesuree : ");
        Serial1.print(bme.readPressure());
        Serial1.println(" Pa");
#endif

        if ((oldPress - bme.readPressure()) >= 200.0)
        { // 2hPa
            bateau->set_pressure_alarm(true);
        }
        oldPress = bme.readPressure();

        ThisThread::sleep_for(600000);
    }
}