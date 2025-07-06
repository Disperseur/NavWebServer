#ifndef PAGE1_H
#define PAGE1_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>Arduino Giga R1 WiFi - Server</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f2f2f2;
      margin: 0;
      padding: 10px;
    }
    h1 {
      text-align: center;
      color: #333;
    }
    .card {
      background: #fff;
      margin-bottom: 10px;
      padding: 10px;
      border-radius: 8px;
      box-shadow: 0 2px 5px rgba(0,0,0,0.1);
    }
    .card h2 {
      margin-top: 0;
      font-size: 1.1em;
      border-bottom: 1px solid #ccc;
      padding-bottom: 5px;
      color: #555;
    }
    table {
      width: 100%;
      border-collapse: collapse;
    }
    td {
      padding: 4px;
      vertical-align: top;
    }
    td.label {
      font-weight: bold;
      color: #333;
      width: 60%;
    }
    footer {
      text-align: center;
      font-size: 0.9em;
      color: #666;
      margin-top: 10px;
    }
  </style>
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
  <h1>Saint-Lou - Données de navigation</h1>

  <div class="card">
    <h2>Horloge</h2>
    <table>
      <tr><td class="label">Durée de navigation:</td><td><span id="running_time"></span> s</td></tr>
      <tr><td class="label">Date:</td><td><span id="ground_date"></span></td></tr>
      <tr><td class="label">Heure:</td><td><span id="ground_time"></span></td></tr>
    </table>
  </div>

  <div class="card">
    <h2>Terre</h2>
    <table>
      <tr><td class="label">Latitude:</td><td><span id="ground_latitude"></span> <span id="ground_latDir"></span></td></tr>
      <tr><td class="label">Longitude:</td><td><span id="ground_longitude"></span> <span id="ground_longDir"></span></td></tr>
      <tr><td class="label">Vitesse sol:</td><td><span id="ground_speedKts"></span> kt</td></tr>
      <tr><td class="label">Cap:</td><td><span id="ground_course"></span>°</td></tr>
    </table>
  </div>

  <div class="card">
    <h2>Mer</h2>
    <table>
      <tr><td class="label">Profondeur sous quille:</td><td><span id="water_depthMeters"></span> m</td></tr>
      <tr><td class="label">Vitesse dans l'eau:</td><td><span id="water_speedKnots"></span> kt</td></tr>
      <tr><td class="label">Température de l'eau:</td><td><span id="water_temperatureCelsius"></span>°C</td></tr>
    </table>
  </div>

  <div class="card">
    <h2>Air</h2>
    <table>
      <tr><td class="label">Origine du vent:</td><td><span id="wind_angle"></span>°</td></tr>
      <tr><td class="label">Vitesse du vent:</td><td><span id="wind_speedKts"></span> kt</td></tr>
    </table>
  </div>

  <footer>
    <p>Antoine Chassaigne</p>
  </footer>
</body>
</html>
)rawliteral";

#endif
