async function fetchData(endpoint) {
    const response = await fetch(`api/data/${endpoint}`);
    const data = await response.json();
    return data;
}

function updateValue(elementId, endpoint) {
    setInterval(async () => {
        const newData = await fetchData(endpoint);
        document.getElementById(elementId).innerText = newData.toFixed(2);
    }, 5000);
}

updateValue('avg-value', 'averageTemperature');
updateValue('max-value', 'maxTemperature');
updateValue('min-value', 'minTemperature');
