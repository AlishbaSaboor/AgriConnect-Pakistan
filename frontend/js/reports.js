// Reports JavaScript
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    displayRoleSpecificReports();
    loadReports();
    
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
    }
}

function displayRoleSpecificReports() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const userRole = currentUser.role.toLowerCase();
    
    // Hide all report sections
    const systemReports = document.getElementById('systemReports');
    const farmerReports = document.getElementById('farmerReports');
    const buyerReports = document.getElementById('buyerReports');
    const storageReports = document.getElementById('storageReports');
    const transportReports = document.getElementById('transportReports');
    
    if (systemReports) systemReports.style.display = 'none';
    if (farmerReports) farmerReports.style.display = 'none';
    if (buyerReports) buyerReports.style.display = 'none';
    if (storageReports) storageReports.style.display = 'none';
    if (transportReports) transportReports.style.display = 'none';
    
    // Show role-specific reports
    if (userRole === 'farmer' && farmerReports) {
        farmerReports.style.display = 'block';
        loadFarmerReports();
    } else if (userRole === 'buyer' && buyerReports) {
        buyerReports.style.display = 'block';
        loadBuyerReports();
    } else if (userRole === 'storage_owner' && storageReports) {
        storageReports.style.display = 'block';
        loadStorageReports();
    } else if (userRole === 'transport_provider' && transportReports) {
        transportReports.style.display = 'block';
        loadTransportReports();
    }
}

function loadReports() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const userRole = currentUser.role.toLowerCase();
    
    // Get all data from localStorage
    const users = JSON.parse(localStorage.getItem('users')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    const storage = JSON.parse(localStorage.getItem('storage')) || [];
    const transport = JSON.parse(localStorage.getItem('transport')) || [];
    
    if (userRole === 'farmer') {
        loadFarmerReports(currentUser, crops);
    } else if (userRole === 'buyer') {
        loadBuyerReports(currentUser, crops);
    } else if (userRole === 'storage_owner') {
        loadStorageReports(currentUser, storage);
    } else if (userRole === 'transport_provider') {
        loadTransportReports(currentUser, transport);
    }
}

function loadFarmerReports(currentUser, crops) {
    const myCrops = crops.filter(c => c.farmer === currentUser.username);
    
    let totalQuantity = 0;
    let totalValue = 0;
    const cropTypes = {};
    
    myCrops.forEach(crop => {
        totalQuantity += crop.quantity;
        totalValue += crop.quantity * crop.pricePerKg;
        cropTypes[crop.type] = (cropTypes[crop.type] || 0) + 1;
    });
    
    if (document.getElementById('totalMyCrops')) document.getElementById('totalMyCrops').textContent = myCrops.length;
    if (document.getElementById('totalMyQuantity')) document.getElementById('totalMyQuantity').textContent = totalQuantity;
    if (document.getElementById('totalMyValue')) document.getElementById('totalMyValue').textContent = totalValue.toLocaleString();
    
    let mostListed = 'N/A';
    let maxCount = 0;
    for (let type in cropTypes) {
        if (cropTypes[type] > maxCount) {
            maxCount = cropTypes[type];
            mostListed = type;
        }
    }
    if (document.getElementById('myMostListed')) document.getElementById('myMostListed').textContent = mostListed;
}

function loadBuyerReports(currentUser, crops) {
    // Buyer would see order history, spending, etc.
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const myOrders = orders.filter(o => o.buyer === currentUser.username);
    
    let totalSpent = 0;
    let completedOrders = 0;
    
    myOrders.forEach(order => {
        totalSpent += (order.quantity * order.price) || 0;
        if (order.delivered) completedOrders++;
    });
    
    if (document.getElementById('myTotalOrders')) document.getElementById('myTotalOrders').textContent = myOrders.length;
    if (document.getElementById('myCompletedOrders')) document.getElementById('myCompletedOrders').textContent = completedOrders;
    if (document.getElementById('myTotalSpent')) document.getElementById('myTotalSpent').textContent = totalSpent.toLocaleString();
}

function loadStorageReports(currentUser, storage) {
    const myStorage = storage.filter(s => s.owner === currentUser.username);
    
    let totalCapacity = 0;
    let usedCapacity = 0;
    
    myStorage.forEach(s => {
        totalCapacity += s.capacity;
        usedCapacity += (s.capacity - s.available);
    });
    
    const utilization = totalCapacity > 0 ? (((usedCapacity) / totalCapacity) * 100).toFixed(1) : 0;
    
    if (document.getElementById('myStorageCenters')) document.getElementById('myStorageCenters').textContent = myStorage.length;
    if (document.getElementById('myTotalCapacity')) document.getElementById('myTotalCapacity').textContent = totalCapacity;
    if (document.getElementById('myUsedCapacity')) document.getElementById('myUsedCapacity').textContent = usedCapacity;
    if (document.getElementById('myUtilization')) document.getElementById('myUtilization').textContent = utilization;
}

function loadTransportReports(currentUser, transport) {
    const myTransport = transport.filter(t => t.provider === currentUser.username);
    
    let totalCapacity = 0;
    const routes = {};
    
    myTransport.forEach(t => {
        totalCapacity += t.capacity;
        routes[t.route] = (routes[t.route] || 0) + 1;
    });
    
    let mostUsedRoute = 'N/A';
    let maxCount = 0;
    for (let route in routes) {
        if (routes[route] > maxCount) {
            maxCount = routes[route];
            mostUsedRoute = route;
        }
    }
    
    if (document.getElementById('myTransportServices')) document.getElementById('myTransportServices').textContent = myTransport.length;
    if (document.getElementById('myTotalCapacity')) document.getElementById('myTotalCapacity').textContent = totalCapacity;
    if (document.getElementById('myMostUsedRoute')) document.getElementById('myMostUsedRoute').textContent = mostUsedRoute;
}

function exportReport(format) {
    if (format === 'pdf') {
        alert('PDF export functionality would be implemented with a library like jsPDF.\n\nReport data has been prepared for export.');
    } else if (format === 'csv') {
        // Simple CSV export
        const users = JSON.parse(localStorage.getItem('users')) || [];
        const crops = JSON.parse(localStorage.getItem('crops')) || [];
        
        let csv = 'AgriConnect Pakistan - System Report\n\n';
        csv += 'Users Report\n';
        csv += 'Username,Email,Role,Location\n';
        
        users.forEach(user => {
            csv += `${user.username},${user.email},${user.role},${user.location}\n`;
        });
        
        csv += '\nCrops Report\n';
        csv += 'Type,Quantity,Quality,Price,Farmer\n';
        
        crops.forEach(crop => {
            csv += `${crop.type},${crop.quantity},${crop.quality},${crop.pricePerKg},${crop.farmer}\n`;
        });
        
        // Create download link
        const blob = new Blob([csv], { type: 'text/csv' });
        const url = window.URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = 'agriconnect-report.csv';
        a.click();
        
        alert('CSV report has been downloaded!');
    }
}
