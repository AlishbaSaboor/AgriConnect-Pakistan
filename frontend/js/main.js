// Main JavaScript for index page
// Simulate loading stats
document.addEventListener('DOMContentLoaded', function() {
    loadStats();
});

function loadStats() {
    // Get data from localStorage
    const users = JSON.parse(localStorage.getItem('users')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    const storage = JSON.parse(localStorage.getItem('storage')) || [];
    const transport = JSON.parse(localStorage.getItem('transport')) || [];
    
    // Update stats
    document.getElementById('totalUsers').textContent = users.length;
    document.getElementById('totalCrops').textContent = crops.length;
    document.getElementById('totalStorage').textContent = storage.length;
    document.getElementById('totalTransport').textContent = transport.length;
}
