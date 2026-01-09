// Dashboard JavaScript
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    loadDashboardData();
    
    // Logout functionality
    document.getElementById('logoutBtn').addEventListener('click', function(e) {
        e.preventDefault();
        sessionStorage.removeItem('currentUser');
        window.location.href = 'login.html';
    });
});

function checkAuth() {
    const currentUser = sessionStorage.getItem('currentUser');
    if (!currentUser) {
        alert('Please login first!');
        window.location.href = 'login.html';
        return;
    }
    
    const user = JSON.parse(currentUser);
    document.getElementById('userName').textContent = user.username;
    document.getElementById('userRole').textContent = user.role.replace('_', ' ').toUpperCase();
}

function loadDashboardData() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    // Load crops
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    const myCrops = crops.filter(crop => crop.farmer === currentUser.username);
    document.getElementById('myCrops').textContent = myCrops.length;
    
    // Load orders (if any)
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const myOrders = orders.filter(order => 
        order.buyer === currentUser.username || 
        crops.some(crop => crop.farmer === currentUser.username && crop.type === order.cropType)
    );
    document.getElementById('activeOrders').textContent = myOrders.length;
    
    // Calculate storage used
    let storageUsed = 0;
    myCrops.forEach(crop => {
        storageUsed += parseInt(crop.quantity);
    });
    document.getElementById('storageUsed').textContent = storageUsed;
}
