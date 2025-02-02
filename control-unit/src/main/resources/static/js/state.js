async function fetchState() {
    const response = await fetch('api/data/state');
    const data = await response.json();
    return data;
}
setInterval(async () => {
    const newData = await fetchState();
    document.getElementById('state').innerText = newData;
}, 1000);

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
