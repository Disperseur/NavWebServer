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
    #windCanvas, #speedGraph {
      display: block;
      margin: 10px auto;
    }
  </style>
  <script>
    let speedHistory = [];

    // Charger l'historique depuis localStorage au démarrage
    if (localStorage.getItem('speedHistory')) {
      try {
        speedHistory = JSON.parse(localStorage.getItem('speedHistory'));
      } catch (e) {
        speedHistory = [];
      }
    }

    function updateData() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          for (const key in data) {
            const el = document.getElementById(key);
            if (el) el.textContent = data[key];
          }

          // --- Dessin du vent ---
          const canvas = document.getElementById('windCanvas');
          const ctx = canvas.getContext('2d');
          const cx = canvas.width / 2;
          const cy = canvas.height / 2;
          const radius = 80;
          ctx.clearRect(0, 0, canvas.width, canvas.height);

          // Cercle
          ctx.beginPath();
          ctx.arc(cx, cy, radius, 0, 2 * Math.PI);
          ctx.strokeStyle = '#333';
          ctx.lineWidth = 2;
          ctx.stroke();

          // Aiguille du vent
          const angleDeg = parseFloat(data.wind_angle) || 0;
          const angleRad = (angleDeg - 90) * Math.PI / 180;
          const speed = parseFloat(data.wind_speedKts) || 0;
          const length = Math.min(speed * 5, radius);
          const xEnd = cx + length * Math.cos(angleRad);
          const yEnd = cy + length * Math.sin(angleRad);

          ctx.beginPath();
          ctx.moveTo(cx, cy);
          ctx.lineTo(xEnd, yEnd);
          ctx.strokeStyle = 'red';
          ctx.lineWidth = 3;
          ctx.stroke();

          // --- Graphe de vitesse sol ---
          const graph = document.getElementById('speedGraph');
          const gctx = graph.getContext('2d');
          const newSpeed = parseFloat(data.ground_speedKts) || 0;

          speedHistory.push(newSpeed);
          if (speedHistory.length > 60) speedHistory.shift();

          // Sauvegarder l'historique dans localStorage
          localStorage.setItem('speedHistory', JSON.stringify(speedHistory));

          gctx.clearRect(0, 0, graph.width, graph.height);

          // Axe horizontal
          gctx.beginPath();
          gctx.moveTo(0, graph.height - 1);
          gctx.lineTo(graph.width, graph.height - 1);
          gctx.strokeStyle = '#ccc';
          gctx.stroke();

          // Courbe
          const maxSpeed = 20; // ajustable
          gctx.beginPath();
          for (let i = 0; i < speedHistory.length; i++) {
            const x = (i / speedHistory.length) * graph.width;
            const y = graph.height - (speedHistory[i] / maxSpeed) * graph.height;
            if (i === 0) gctx.moveTo(x, y);
            else gctx.lineTo(x, y);
          }
          gctx.strokeStyle = 'blue';
          gctx.lineWidth = 2;
          gctx.stroke();
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
      <tr><td class="label">Durée de navigation:</td><td><span id="running_time"></span></td></tr>
      <tr><td class="label">Date:</td><td><span id="ground_date"></span></td></tr>
      <tr><td class="label">Heure:</td><td><span id="ground_time"></span></td></tr>
    </table>
  </div>

  <div class="card">
    <h2>Route fond</h2>
    <table>
      <tr><td class="label">Latitude:</td><td><span id="ground_latitude"></span> <span id="ground_latDir"></span></td></tr>
      <tr><td class="label">Longitude:</td><td><span id="ground_longitude"></span> <span id="ground_longDir"></span></td></tr>
      <tr><td class="label">Cap:</td><td><span id="ground_course"></span>°</td></tr>
      <tr><td class="label">Vitesse sol:</td><td><span id="ground_speedKts"></span> kt</td></tr>
      <tr><td class="label">Vitesse moyenne sol sur 30min:</td><td><span id="ground_speedKts_avg"></span> kt</td></tr>
    </table>
    <canvas id="speedGraph" width="600" height="100"></canvas>
  </div>

  <div class="card">
    <h2>Route surface</h2>
    <table>
      <tr><td class="label">Profondeur sous quille:</td><td><span id="water_depthMeters"></span> m</td></tr>
      <tr><td class="label">Vitesse dans l'eau:</td><td><span id="water_speedKts"></span> kt</td></tr>
      <tr><td class="label">Vitesse moyenne dans l'eau sur 30min:</td><td><span id="water_speedKts_avg"></span> kt</td></tr>
      <tr><td class="label">Température de l'eau:</td><td><span id="water_temperatureCelsius"></span>°C</td></tr>
    </table>
  </div>

  <div class="card">
    <h2>Vent</h2>
    <table>
      <tr><td class="label">Origine du vent:</td><td><span id="wind_angle"></span>°</td></tr>
      <tr><td class="label">Vitesse du vent (kts):</td><td><span id="wind_speedKts"></span> kt</td></tr>
    </table>
    <canvas id="windCanvas" width="200" height="200"></canvas>
  </div>

  <footer>
    <p>Antoine Chassaigne</p>
  </footer>
</body>
</html>
)rawliteral";

#endif
