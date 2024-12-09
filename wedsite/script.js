const temperatureChartCtx = document.getElementById('temperatureChart').getContext('2d');

const temperatureChart = new Chart(temperatureChartCtx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Temperature (°C)',
            data: [],
            borderColor: 'rgba(105, 114, 216, 1)',
            backgroundColor: 'rgba(105, 114, 216, 0.2)',
            borderWidth: 2
        }]
    },
    options: {
        responsive: true,
        scales: {
            x: {
                type: 'linear',
                position: 'bottom'
            },
            y: {
                min: 0,
                max: 100
            }
        }
    }
});

let temperatureInterval = null;
let shutdownTimer = null;
let simulatedTemperature = 25.0;

document.getElementById('start-temp-btn').addEventListener('click', () => {
    const setTemp = parseFloat(document.getElementById('set-temperature').value);
    const tolerance = parseFloat(document.getElementById('set-tolerance').value);

    // Simulate real-time temperature data
    if (temperatureInterval) clearInterval(temperatureInterval); // Stop any previous simulation
    temperatureInterval = setInterval(() => {
        simulatedTemperature = setTemp + Math.random() * tolerance * 2 - tolerance;
        updateChart(simulatedTemperature);
        document.getElementById('current-temp').textContent = simulatedTemperature.toFixed(1); // Hiển thị nhiệt độ hiện tại

        // Kiểm tra nếu nhiệt độ nằm trong phạm vi cho phép
        const indicator = document.getElementById('temp-indicator');
        const indicatorText = document.getElementById('indicator-text');
        if (Math.abs(simulatedTemperature - setTemp) <= tolerance) {
            indicator.classList.remove('red');
            indicator.classList.add('green');
            indicatorText.classList.remove('red');
            indicatorText.classList.add('green');
            indicatorText.textContent = 'Temperature is within range';
        } else {
            indicator.classList.remove('green');
            indicator.classList.add('red');
            indicatorText.classList.remove('green');
            indicatorText.classList.add('red');
            indicatorText.textContent = 'Temperature is out of range';
        }
    }, 1000);
});

document.getElementById('stop-temp-btn').addEventListener('click', () => {
    if (temperatureInterval) clearInterval(temperatureInterval);
    document.getElementById('temp-indicator').classList.remove('green');
    document.getElementById('temp-indicator').classList.add('red');
    document.getElementById('indicator-text').classList.remove('green');
    document.getElementById('indicator-text').classList.add('red');
    document.getElementById('indicator-text').textContent = 'Temperature is out of range';
});

// Cập nhật đồ thị
function updateChart(temperature) {
    const labels = temperatureChart.data.labels;
    const data = temperatureChart.data.datasets[0].data;
    if (labels.length >= 20) {
        labels.shift();
        data.shift();
    }
    labels.push(Date.now());
    data.push(temperature);
    temperatureChart.update();
}
