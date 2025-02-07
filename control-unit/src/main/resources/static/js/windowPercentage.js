async function fetchWindowPercentage() {
    const response = await fetch('/api/data/windowPercentage');
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
        console.error("Error updating windowPercentage:", error);
    }
}

setInterval(async () => {
    const newData = await fetchWindowPercentage();
    if (newData && typeof newData.percentage === "number") {
        document.getElementById('windowPercentage').innerText = newData.percentage;
        document.getElementById('windowSlider').value = newData.percentage;
    } else {
        console.error("Invalid data received:", newData);
    }
}, 1000);
