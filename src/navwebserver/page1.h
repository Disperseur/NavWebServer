#ifndef PAGE1_H
#define PAGE1_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>Saint Lou - Données de navigation</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      font-size: 35px;
      background: #000;
      color: #fff;
      margin: 0;
      padding: 10px;
    }
    h1 {
      text-align: center;
      color: #fff;
    }
    .grid {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      gap: 15px;
    }
    .card {
      background: #111;
      margin: 10px;
      padding: 10px;
      border-radius: 8px;
      // box-shadow: 0 2px 5px rgba(0,0,0,0.7);
      border: 1px solid #fff;
    }
    .card h2 {
      text-align: center;
      margin-top: 0;
      font-size: 1.1em;
      border-bottom: 1px solid #fff;
      padding-bottom: 5px;
      color: #fff;
    }
    table {
      width: auto;
      border-collapse: collapse;
    }
    td {
      padding: 4px;
      vertical-align: top;
      white-space: nowrap;
      min-width: 200px;
      color: #fff;
    }
    td.label {
      font-weight: bold;
      color: #fff;
      width: 60%;
    }
    footer {
      text-align: center;
      font-size: 0.9em;
      color: #fff;
      margin-top: 10px;
    }
    #speedGraph {
      display: block;
      margin: 10px auto;
      background: #181818;
      border: 1px solid #fff;
      border-radius: 4px;
    }
    #depthGraph {
      display: block;
      margin: 10px auto;
      background: #181818;
      border: 1px solid #fff;
      border-radius: 4px;
    }
    #windCanvas {
      display: block;
      margin: 10px auto;
      border-radius: 4px;
    }
  </style>



  <script>


    function disablePressureAlarm() {
      fetch('/disable-pressure-alarm') // tu devras gérer cette route côté Arduino
        // .then(response => {
        //   if (response.ok) {
        //     document.getElementById('alarmButton_pressure').style.backgroundColor = 'grey';
        //   }
        // })
        // .catch(err => console.error('Erreur désactivation:', err));
    }

    function disableDepthAlarm() {
      fetch('/disable-depth-alarm') // tu devras gérer cette route côté Arduino
        // .then(response => {
        //   if (response.ok) {
        //     document.getElementById('alarmButton_depth').style.backgroundColor = 'grey';
        //   }
        // })
        // .catch(err => console.error('Erreur désactivation:', err));
    }

    function updatePressureAlarm(isActive) {
      const button_pressureAlarm = document.getElementById('alarmButton_pressure');
      if (isActive) {
        button_pressureAlarm.style.backgroundColor = 'red';
      } else {
        button_pressureAlarm.style.backgroundColor = 'grey';
      }
    }

    function updateDepthAlarm(isActive) {
      const button_depthAlarm = document.getElementById('alarmButton_depth');
      if (isActive) {
        button_depthAlarm.style.backgroundColor = 'red';
      } else {
        button_depthAlarm.style.backgroundColor = 'grey';
      }
    }


    let speedHistory = [];
    let depthHistory = [];


    // Charger l'historique depuis localStorage au démarrage
    if (localStorage.getItem('speedHistory')) {
      try {
        speedHistory = JSON.parse(localStorage.getItem('speedHistory'));
      } catch (e) {
        speedHistory = [];
      }
    }
    if (localStorage.getItem('depthHistory')) {
      try {
        depthHistory = JSON.parse(localStorage.getItem('depthHistory'));
      } catch (e) {
        depthHistory = [];
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

          // mise a jour alarmes
          updatePressureAlarm(data.pressure_alarm);
          updateDepthAlarm(data.depth_alarm);

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
          ctx.strokeStyle = '#fff';
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
          gctx.strokeStyle = '#fff';
          gctx.lineWidth = 1;
          gctx.stroke();

          // Cadre autour du graphe
          gctx.beginPath();
          gctx.rect(0, 0, graph.width, graph.height);
          gctx.strokeStyle = '#fff';
          gctx.lineWidth = 1;
          gctx.stroke();

          // Axe vertical et graduations
          const maxSpeed = 20; // ajustable
          const gradStep = 5;  // graduation tous les 5 kt
          gctx.font = "10px Arial";
          gctx.fillStyle = "#fff";
          gctx.strokeStyle = "#fff";
          gctx.lineWidth = 1;
          for (let v = 0; v <= maxSpeed; v += gradStep) {
            const y = graph.height - (v / maxSpeed) * graph.height;
            // Graduation
            gctx.beginPath();
            gctx.moveTo(0, y);
            gctx.lineTo(8, y);
            gctx.stroke();
            // Texte
            gctx.fillText(v + " kt", 12, y + 3);
            // Ligne horizontale légère (optionnel)
            if (v > 0 && v < maxSpeed) {
              gctx.beginPath();
              gctx.moveTo(0, y);
              gctx.lineTo(graph.width, y);
              gctx.strokeStyle = "#fff";
              gctx.stroke();
              gctx.strokeStyle = "#fff";
            }
          }

          // Courbe
          gctx.beginPath();
          for (let i = 0; i < speedHistory.length; i++) {
            const x = (i / speedHistory.length) * graph.width;
            const y = graph.height - (speedHistory[i] / maxSpeed) * graph.height;
            if (i === 0) gctx.moveTo(x, y);
            else gctx.lineTo(x, y);
          }
          gctx.strokeStyle = 'white';
          gctx.lineWidth = 2;
          gctx.stroke();


          // --- Graphe de profondeur sous quille ---
          const depthGraph = document.getElementById('depthGraph');
          const dctx = depthGraph.getContext('2d');
          const newDepth = parseFloat(data.water_depthMeters) || 0;

          depthHistory.push(newDepth);
          if (depthHistory.length > 60) depthHistory.shift();
          localStorage.setItem('depthHistory', JSON.stringify(depthHistory));

          dctx.clearRect(0, 0, depthGraph.width, depthGraph.height);

          // Axe horizontal
          dctx.beginPath();
          dctx.moveTo(0, depthGraph.height - 1);
          dctx.lineTo(depthGraph.width, depthGraph.height - 1);
          dctx.strokeStyle = '#fff';
          dctx.lineWidth = 1;
          dctx.stroke();

          // Cadre autour du graphe
          dctx.beginPath();
          dctx.rect(0, 0, depthGraph.width, depthGraph.height);
          dctx.strokeStyle = '#fff';
          dctx.lineWidth = 1;
          dctx.stroke();

          // Axe vertical et graduations
          const maxDepth = 30; // ajustable selon ton plan d'eau
          const gradStepDepth = 5; // graduation tous les 2 m
          dctx.font = "10px Arial";
          dctx.fillStyle = "#fff";
          dctx.strokeStyle = "#fff";
          dctx.lineWidth = 1;
          for (let v = 0; v <= maxDepth; v += gradStepDepth) {
            const y = (v / maxDepth) * depthGraph.height;
            // Graduation
            dctx.beginPath();
            dctx.moveTo(0, y);
            dctx.lineTo(8, y);
            dctx.stroke();
            // Texte
            dctx.fillText(v + " m", 12, y + 3);
            // Ligne horizontale légère (optionnel)
            if (v > 0 && v < maxDepth) {
              dctx.beginPath();
              dctx.moveTo(0, y);
              dctx.lineTo(depthGraph.width, y);
              dctx.strokeStyle = "#fff";
              dctx.stroke();
              dctx.strokeStyle = "#fff";
            }
          }

          // Courbe profondeur
          dctx.beginPath();
          for (let i = 0; i < depthHistory.length; i++) {
            const x = (i / depthHistory.length) * depthGraph.width;
            const y = (depthHistory[i] / maxDepth) * depthGraph.height;
            if (i === 0) dctx.moveTo(x, y);
            else dctx.lineTo(x, y);
          }
          dctx.strokeStyle = 'white';
          dctx.lineWidth = 2;
          dctx.stroke();

        })
        .catch(err => console.error(err));
    }

    

    setInterval(updateData, 1000);
    window.onload = updateData;
  </script>
</head>
<body>

  <div class="grid">
    <div class="card">
      <h2>Alarmes</h2>
      <div style="display: flex; flex-direction: column; align-items: center; gap: 10px;">
        <button id="alarmButton_pressure" onclick="disablePressureAlarm()" style="font-size: 20px; padding: 15px 30px; border: none; border-radius: 10px; background-color: grey; color: white;">
          GRAIN IMMINENT
        </button>

        <button id="alarmButton_depth" onclick="disableDepthAlarm()" style="font-size: 20px; padding: 15px 30px; border: none; border-radius: 10px; background-color: grey; color: white; margin: 10px;">
          HAUTS-FONDS
        </button>
      </div>
    </div>

    <div class="card">
      <h2>Horloge</h2>
      <table>
        <tr><td class="label">Durée :</td><td><span id="running_time"></span></td></tr>
        <tr><td class="label">Date :</td><td><span id="ground_date"></span></td></tr>
        <tr><td class="label">Heure :</td><td><span id="ground_time"></span></td></tr>
      </table>
    </div>

    <div class="card">
      <h2>Route Fond</h2>
      <table>
        <tr><td class="label">Lat :</td><td><span id="ground_latitude"></span> <span id="ground_latDir"></span></td></tr>
        <tr><td class="label">Long :</td><td><span id="ground_longitude"></span> <span id="ground_longDir"></span></td></tr>
        <tr><td class="label">Cap :</td><td><span id="ground_course"></span>°</td></tr>
        <tr><td class="label">VF :</td><td><span id="ground_speedKts"></span> kt</td></tr>
        <tr><td class="label">VFM :</td><td><span id="ground_speedKts_avg"></span> kt</td></tr>
      </table>
    </div>

    <div class="card">
      <h2>Route Surface</h2>
      <table>
        <tr><td class="label">Profondeur :</td><td><span id="water_depthMeters"></span> m</td></tr>
        <tr><td class="label">VS :</td><td><span id="water_speedKts"></span> kt</td></tr>
        <tr><td class="label">VSM :</td><td><span id="water_speedKts_avg"></span> kt</td></tr>
        <tr><td class="label">Temp eau :</td><td><span id="water_temperatureCelsius"></span>°C</td></tr>
      </table>
    </div>

    <div class="card">
      <h2>Vent</h2>
      <table>
        <tr><td class="label">Origine :</td><td><span id="wind_angle"></span>°</td></tr>
        <tr><td class="label">Vitesse :</td><td><span id="wind_speedKts"></span> kt</td></tr>
      </table>
      <canvas id="windCanvas" width="200" height="200"></canvas>
    </div>

     <div class="card">
      <h2>Historiques</h2>

      <canvas id="speedGraph" width="800" height="200"></canvas>
      <div style="text-align:center; font-size:0.95em; color:#fff; margin-bottom:2px;">
        Vitesse Sol
      </div>

      <canvas id="depthGraph" width="800" height="200"></canvas>
      <div style="text-align:center; font-size:0.95em; color:#fff; margin-bottom:2px;">
        Profondeur Sous Quille
      </div>

    </div>
  </div>

  <footer>
    <p>Antoine Chassaigne - Juillet 2025 - Version 1.0</p>
  </footer>
</body>
</html>
)rawliteral";

#endif
