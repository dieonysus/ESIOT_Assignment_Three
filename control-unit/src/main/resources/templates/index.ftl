<!DOCTYPE html>
<html lang="en">
<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">

    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

        <style>
            body {
                background-color: #EBEBEB;
            }
        </style>

</head>


<body>
    <div class="container mt-5 mb-5">
        <h1>Dashboard</h1>
        <div class="card-deck">
          <div class="card mt-3">
            <div class="card-body">
                <h5 class="card-title">Temperature</h5>
                <canvas id="myChart"></canvas>
            </div>
          </div>
          <div class="card mt-3">
            <div class="card-body">
              <h5 class="card-title">Parameters</h5>
              <p class="card-text">Avg: <span id="avg-value">0.00</span></p>
              <p class="card-text">Max: <span id="max-value">0.00</span></p>
              <p class="card-text">Min: <span id="min-value">0.00</span></p></p>
            </div>
          </div>
        </div>
        <div class="card-deck">
          <div class="card mt-3">
            <div class="card-body">
                <h5 class="card-title">State</h5>
                <p><span id="state">NORMAL</span></p>
                <p>
                    <button id="solve-button" type="button" class="btn btn-danger" onclick="resetState()" disabled>Problem Solved</button>
                </p>
            </div>
          </div>
          <div class="card mt-3">
            <div class="card-body">
              <h5 class="card-title">Window</h5>
              <p>
                <button id="mode-toggle" type="button" class="btn btn-primary" onclick="toggleMode()">AUTOMATIC</button>
              </p>
              <div class="slidecontainer">
                <input type="range" id="windowSlider" min="1" max="100" oninput="handleSliderChange(this.value)">
              </div>
              <p class="card-text"><span id="windowPercentage">0</span>%</p></p>
            </div>
          </div>
        </div>
    </div>


    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/popper.js@1.12.9/dist/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.0.0/dist/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>


    <script>
        const ctx = document.getElementById('myChart').getContext('2d');
        const myChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: ['', '', '', '', '', '', '', '', '', 'time'],
                datasets: [{
                    label: 'Dynamic Data',
                    data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0], // Изначально пустой массив данных
                    borderColor: 'rgba(255, 99, 132, 1)', // Новый цвет линии (розовый)
                    borderWidth: 2,
                    fill: false,
                    pointBackgroundColor: 'rgba(255, 99, 132, 1)', // Цвет точек
                    pointRadius: 5, // Размер точек
                    pointHoverRadius: 7, // Размер точек при наведении
                }]
            },
            options: {
                responsive: true,
                plugins: {
                    legend: {
                        display: false // Отключаем легенду
                    }
                },
                scales: {
                    x: {
                        grid: {
                            display: false, // Отключаем сетку по оси X
                        },
                        beginAtZero: true
                    },
                    y: {
                        grid: {
                            color: 'rgba(200, 200, 200, 0.3)', // Лёгкий серый цвет сетки
                            lineWidth: 0.5, // Тонкие линии сетки
                        },
                        beginAtZero: true
                    }
                }
            }
        });


        async function fetchTemperatureQueue() {
            const response = await fetch('/api/data/temperatureQueue');
            const data = await response.json();
            return data;
        }

        setInterval(async () => {
            const newData = await fetchTemperatureQueue();
            myChart.data.datasets[0].data = newData;
            myChart.update();
        }, 100);


        async function fetchAverageTemperature() {
            const response = await fetch('api/data/averageTemperature');
            const data = await response.json();
            return data;
        }

        setInterval(async () => {
            const newData = await fetchAverageTemperature();
            document.getElementById('avg-value').innerText = newData.toFixed(2);
        }, 5000);


        async function fetchMaxTemperature() {
            const response = await fetch('api/data/maxTemperature');
            const data = await response.json();
            return data;
        }

        setInterval(async () => {
            const newData = await fetchMaxTemperature();
            document.getElementById('max-value').innerText = newData.toFixed(2);
        }, 5000);


        async function fetchMinTemperature() {
            const response = await fetch('api/data/minTemperature');
            const data = await response.json();
            return data;
        }

        setInterval(async () => {
            const newData = await fetchMinTemperature();
            document.getElementById('min-value').innerText = newData.toFixed(2);
        }, 5000);


        async function fetchState() {
            const response = await fetch('api/data/state');
            const data = await response.json();
            return data;
        }

        setInterval(async () => {
            const newData = await fetchState();
            document.getElementById('state').innerText = newData;
        }, 1000);


        async function fetchWindowPercentage() {
            const response = await fetch('api/data/windowPercentage');
            const data = await response.json();
            return data;
        }

        async function updateWindowPercentage(value) {
            try {
                await fetch('/api/data/setWindowPercentage', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ percentage: value })
                });
            } catch (error) {
                console.error("Ошибка при обновлении windowPercentage:", error);
            }
        }

        setInterval(async () => {
            const newData = await fetchWindowPercentage();
            document.getElementById('windowPercentage').innerText = newData;
            document.getElementById('windowSlider').value = newData;
        }, 1000);
    </script>


    <script>
        async function toggleMode() {
            try {
                const response = await fetch('/api/data/toggleMode', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    }
                });
                const newMode = await response.json();
                document.getElementById('mode-toggle').textContent = newMode;
            } catch (error) {
                console.error('Failed to toggle mode:', error);
            }
        }
    </script>


    <script>
        function updateButtonState() {
            const state = document.getElementById('state').textContent.trim();
            const solveButton = document.getElementById('solve-button');
            if (state === 'ALARM') {
                solveButton.disabled = false;
            } else {
                solveButton.disabled = true;
            }
        }
        setInterval(updateButtonState, 1000);
    </script>


    <script>
        async function resetState() {
            try {
                const response = await fetch('/api/data/resetState', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    }
                });
            } catch (error) {
                console.error('Failed to toggle mode:', error);
            }
        }
    </script>


    <script>
        async function fetchMode() {
            try {
                const response = await fetch('/api/data/mode');
                if (!response.ok) {
                    throw new Error('Failed to fetch mode');
                }
                const mode = await response.text();
                document.getElementById('mode-toggle').textContent = mode.toUpperCase();

                const slider = document.getElementById('windowSlider');
                if (mode.toUpperCase() === "AUTOMATIC") {
                    slider.disabled = true;
                } else {
                    slider.disabled = false;
                }
            } catch (error) {
                console.error('Error fetching mode:', error);
            }
        }
        setInterval(fetchMode, 1000);


        let sliderTimeout;

        function handleSliderChange(value) {
            document.getElementById("windowPercentage").innerText = value;

            clearTimeout(sliderTimeout); // Сбрасываем предыдущий таймер
            sliderTimeout = setTimeout(() => {
                updateWindowPercentage(value); // Отправляем только после задержки
            }, 500);
        }


        let isDragging = false;

        document.getElementById("windowSlider").addEventListener("mousedown", () => {
            isDragging = true;
        });
        document.getElementById("windowSlider").addEventListener("mouseup", () => {
            isDragging = false;
        });

        setInterval(async () => {
            if (!isDragging) { // Только если ползунок НЕ тянут
                const newData = await fetchWindowPercentage();
                document.getElementById("windowPercentage").innerText = newData;
                document.getElementById("windowSlider").value = newData;
            }
        }, 1000);


    </script>


</body>

</html>
