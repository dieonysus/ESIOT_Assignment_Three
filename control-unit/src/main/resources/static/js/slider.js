async function fetchMode() {
    try {
        const response = await fetch('/api/data/mode');
        if (!response.ok) {
            throw new Error('Failed to fetch mode');
        }
        const mode = await response.json();
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

    clearTimeout(sliderTimeout);
    sliderTimeout = setTimeout(() => {
        updateWindowPercentage(value);
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
    if (!isDragging) {
        const newData = await fetchWindowPercentage();
        document.getElementById("windowPercentage").innerText = newData;
        document.getElementById("windowSlider").value = newData;
    }
}, 1000);
