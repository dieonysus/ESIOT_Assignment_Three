const ctx = document.getElementById('myChart').getContext('2d');

const myChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: ['', '', '', '', '', '', '', '', '', 'time'],
        datasets: [{
            label: 'Dynamic Data',
            data: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            borderColor: 'rgba(255, 99, 132, 1)',
            borderWidth: 2,
            fill: false,
            pointBackgroundColor: 'rgba(255, 99, 132, 1)',
            pointRadius: 5,
            pointHoverRadius: 7,
        }]
    },
    options: {
        responsive: true,
        plugins: {
            legend: {
                display: false
            }
        },
        scales: {
            x: {
                grid: {
                    display: false,
                },
                beginAtZero: true
            },
            y: {
                grid: {
                    color: 'rgba(200, 200, 200, 0.3)',
                    lineWidth: 0.5,
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
}, 1000);