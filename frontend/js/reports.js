// Reports JavaScript
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
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

function loadReports() {
    // Get all data from localStorage
    const users = JSON.parse(localStorage.getItem('users')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    const storage = JSON.parse(localStorage.getItem('storage')) || [];
    const transport = JSON.parse(localStorage.getItem('transport')) || [];
    
    // User Statistics
    document.getElementById('totalUsers').textContent = users.length;
    
    const farmers = users.filter(u => u.role === 'farmer').length;
    const buyers = users.filter(u => u.role === 'buyer').length;
    const storageOwners = users.filter(u => u.role === 'storage_owner').length;
    const transportProviders = users.filter(u => u.role === 'transport_provider').length;
    
    document.getElementById('totalFarmers').textContent = farmers;
    document.getElementById('totalBuyers').textContent = buyers;
    document.getElementById('totalStorageOwners').textContent = storageOwners;
    document.getElementById('totalTransportProviders').textContent = transportProviders;
    
    // Crop Statistics
    document.getElementById('totalCropsListed').textContent = crops.length;
    
    let totalCropQuantity = 0;
    let totalCropValue = 0;
    const cropTypeCounts = {};
    
    crops.forEach(crop => {
        totalCropQuantity += crop.quantity;
        totalCropValue += crop.quantity * crop.pricePerKg;
        
        cropTypeCounts[crop.type] = (cropTypeCounts[crop.type] || 0) + 1;
    });
    
    document.getElementById('totalCropQuantity').textContent = totalCropQuantity;
    
    const avgPrice = crops.length > 0 ? (totalCropValue / totalCropQuantity).toFixed(2) : 0;
    document.getElementById('avgCropPrice').textContent = avgPrice;
    
    // Find most listed crop
    let mostListedCrop = 'N/A';
    let maxCount = 0;
    for (let cropType in cropTypeCounts) {
        if (cropTypeCounts[cropType] > maxCount) {
            maxCount = cropTypeCounts[cropType];
            mostListedCrop = cropType;
        }
    }
    document.getElementById('mostListedCrop').textContent = mostListedCrop;
    
    // Storage Statistics
    document.getElementById('totalStorageCenters').textContent = storage.length;
    
    let totalCapacity = 0;
    let availableCapacity = 0;
    
    storage.forEach(s => {
        totalCapacity += s.capacity;
        availableCapacity += s.available;
    });
    
    document.getElementById('totalStorageCapacity').textContent = totalCapacity;
    document.getElementById('availableStorageCapacity').textContent = availableCapacity;
    
    const utilization = totalCapacity > 0 ? 
        (((totalCapacity - availableCapacity) / totalCapacity) * 100).toFixed(1) : 0;
    document.getElementById('storageUtilization').textContent = utilization;
    
    // Transport Statistics
    document.getElementById('totalTransportServices').textContent = transport.length;
    
    let totalTransportCapacity = 0;
    const routeCounts = {};
    
    transport.forEach(t => {
        totalTransportCapacity += t.capacity;
        routeCounts[t.route] = (routeCounts[t.route] || 0) + 1;
    });
    
    document.getElementById('totalTransportCapacity').textContent = totalTransportCapacity;
    document.getElementById('activeDeliveries').textContent = Math.floor(transport.length * 0.3); // Simulated
    
    // Find most used route
    let mostUsedRoute = 'N/A';
    maxCount = 0;
    for (let route in routeCounts) {
        if (routeCounts[route] > maxCount) {
            maxCount = routeCounts[route];
            mostUsedRoute = route;
        }
    }
    document.getElementById('mostUsedRoute').textContent = mostUsedRoute;
    
    // Financial Statistics (simulated)
    const totalTransactions = crops.length + transport.length;
    const totalRevenue = totalCropValue;
    
    document.getElementById('totalTransactions').textContent = totalTransactions;
    document.getElementById('totalRevenue').textContent = totalRevenue.toLocaleString();
    document.getElementById('cashPayments').textContent = Math.floor(totalTransactions * 0.6);
    document.getElementById('digitalPayments').textContent = Math.floor(totalTransactions * 0.4);
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
