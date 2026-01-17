// Dashboard JavaScript
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    displayRoleDashboard();
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

function displayRoleDashboard() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const userRole = currentUser.role.toLowerCase();
    
    // Hide all dashboards
    document.getElementById('farmerDashboard').style.display = 'none';
    document.getElementById('buyerDashboard').style.display = 'none';
    document.getElementById('storageDashboard').style.display = 'none';
    document.getElementById('transportDashboard').style.display = 'none';
    
    // Show the appropriate dashboard based on role
    if (userRole === 'farmer') {
        document.getElementById('farmerDashboard').style.display = 'block';
        loadFarmerDashboard();
    } else if (userRole === 'buyer') {
        document.getElementById('buyerDashboard').style.display = 'block';
        loadBuyerDashboard();
    } else if (userRole === 'storage_owner') {
        document.getElementById('storageDashboard').style.display = 'block';
        loadStorageDashboard();
    } else if (userRole === 'transport_provider') {
        document.getElementById('transportDashboard').style.display = 'block';
        loadTransportDashboard();
    }
}

function loadDashboardData() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const userRole = currentUser.role.toLowerCase();
    
    if (userRole === 'farmer') {
        loadFarmerStats();
    } else if (userRole === 'buyer') {
        loadBuyerStats();
    } else if (userRole === 'storage_owner') {
        loadStorageStats();
    } else if (userRole === 'transport_provider') {
        loadTransportStats();
    }
}

function loadFarmerDashboard() {
    loadFarmerStats();
}

function loadFarmerStats() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    // Load crops
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    const myCrops = crops.filter(crop => crop.farmer === currentUser.username);
    if (document.getElementById('myCrops')) {
        document.getElementById('myCrops').textContent = myCrops.length;
    }
    
    // Load orders pending approval
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const pendingOrders = orders.filter(order => 
        crops.some(crop => crop.farmer === currentUser.username && crop.id === order.cropId && !order.farmerApproved)
    );
    if (document.getElementById('pendingOrders')) {
        document.getElementById('pendingOrders').textContent = pendingOrders.length;
    }
    
    // Calculate storage used
    let storageUsed = 0;
    myCrops.forEach(crop => {
        storageUsed += parseInt(crop.quantity);
    });
    if (document.getElementById('storageUsed')) {
        document.getElementById('storageUsed').textContent = storageUsed + ' kg';
    }
}

function loadBuyerDashboard() {
    loadBuyerStats();
}

function loadBuyerStats() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    // Load orders
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const myOrders = orders.filter(order => order.buyer === currentUser.username);
    const activeOrders = myOrders.filter(order => !order.delivered);
    const completedOrders = myOrders.filter(order => order.delivered);
    
    if (document.getElementById('activeOrders')) {
        document.getElementById('activeOrders').textContent = activeOrders.length;
    }
    
    if (document.getElementById('completedOrders')) {
        document.getElementById('completedOrders').textContent = completedOrders.length;
    }
    
    // Calculate total spent
    let totalSpent = 0;
    myOrders.forEach(order => {
        totalSpent += (order.quantity * order.price) || 0;
    });
    if (document.getElementById('totalSpent')) {
        document.getElementById('totalSpent').textContent = totalSpent + ' PKR';
    }
}

function loadStorageDashboard() {
    loadStorageStats();
}

function loadStorageStats() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    // Load storage centers
    const storageCenters = JSON.parse(localStorage.getItem('storageCenters')) || [];
    const myStorageCenters = storageCenters.filter(sc => sc.organization === currentUser.username);
    
    let totalCapacity = 0;
    let usedCapacity = 0;
    myStorageCenters.forEach(sc => {
        totalCapacity += (sc.totalCapacity || 0);
        usedCapacity += (sc.totalCapacity - (sc.availableCapacity || 0));
    });
    
    if (document.getElementById('totalCapacity')) {
        document.getElementById('totalCapacity').textContent = totalCapacity + ' kg';
    }
    
    if (document.getElementById('usedCapacity')) {
        document.getElementById('usedCapacity').textContent = usedCapacity + ' kg';
    }
    
    // Load storage requests
    const storageRequests = JSON.parse(localStorage.getItem('storageRequests')) || [];
    const pendingRequests = storageRequests.filter(sr => 
        sr.organization === currentUser.username && !sr.accepted && !sr.rejected
    );
    
    if (document.getElementById('pendingRequests')) {
        document.getElementById('pendingRequests').textContent = pendingRequests.length;
    }
}

function loadTransportDashboard() {
    loadTransportStats();
}

function loadTransportStats() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    // Load vehicles
    const vehicles = JSON.parse(localStorage.getItem('vehicles')) || [];
    const myVehicles = vehicles.filter(v => v.organization === currentUser.username);
    const activeVehicles = myVehicles.filter(v => v.available);
    
    if (document.getElementById('activeVehicles')) {
        document.getElementById('activeVehicles').textContent = activeVehicles.length;
    }
    
    // Load transport requests
    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];
    const pendingRequests = transportRequests.filter(tr => 
        tr.organization === currentUser.username && !tr.accepted && !tr.rejected
    );
    const completedDeliveries = transportRequests.filter(tr => 
        tr.organization === currentUser.username && tr.completed
    );
    
    if (document.getElementById('pendingRequests')) {
        document.getElementById('pendingRequests').textContent = pendingRequests.length;
    }
    
    if (document.getElementById('completedDeliveries')) {
        document.getElementById('completedDeliveries').textContent = completedDeliveries.length;
    }
}
