async function toggleMode() {
    try {
        const response = await fetch('/api/data/toggleMode', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        });
        const newMode = await response.json();
        document.getElementById('mode-toggle').textContent = newMode;
    } catch (error) {
        console.error('Failed to toggle mode:', error);
    }
}
