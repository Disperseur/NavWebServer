#ifndef PAGE1_H
#define PAGE1_H


const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>Arduino Giga R1 WiFi - Data</title>
  <script>
    function updateData() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          for (const key in data) {
            const el = document.getElementById(key);
            if (el) el.textContent = data[key];
          }
        })
        .catch(err => console.error(err));
    }
    setInterval(updateData, 1000);
    window.onload = updateData;
  </script>
</head>
<body>
  <h1>Arduino Giga R1 WiFi - Data</h1>
  <p>Running Time: <span id="running_time"></span></p>
  <p>Ground Time: <span id="ground_time"></span></p>
  <p>Ground Date: <span id="ground_date"></span></p>
  <p>Latitude: <span id="ground_latitude"></span> <span id="ground_latDir"></span></p>
  <p>Longitude: <span id="ground_longitude"></span> <span id="ground_longDir"></span></p>
  <p>Ground Speed (kts): <span id="ground_speedKts"></span></p>
  <p>Ground Course: <span id="ground_course"></span></p>
  <p>Water Depth (m): <span id="water_depthMeters"></span></p>
  <p>Water Speed (kts): <span id="water_speedKnots"></span></p>
  <p>Water Temperature (°C): <span id="water_temperatureCelsius"></span></p>
  <p>Wind Angle: <span id="wind_angle"></span></p>
  <p>Wind Angle Reference: <span id="wind_angleReference"></span></p>
  <p>Wind Speed (kts): <span id="wind_speedKts"></span></p>
</body>
</html>
)rawliteral";

#endif
