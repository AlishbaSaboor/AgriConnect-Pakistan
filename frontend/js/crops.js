// Crops Management JavaScript
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    displayRoleSpecificUI();
    loadCrops();
    
    // Logout functionality
    document.getElementById('logoutBtn').addEventListener('click', function(e) {
        e.preventDefault();
        sessionStorage.removeItem('currentUser');
        window.location.href = 'login.html';
    });
    
    // Add crop form submission (for farmers)
    const cropForm = document.getElementById('cropForm');
    if (cropForm) {
        cropForm.addEventListener('submit', function(e) {
            e.preventDefault();
            addCrop();
        });
    }
});

function displayRoleSpecificUI() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const userRole = currentUser.role.toLowerCase();
    
    // Show role-specific actions
    if (userRole === 'farmer') {
        const farmerActions = document.getElementById('farmerActions');
        if (farmerActions) farmerActions.style.display = 'block';
    }
}

function checkAuth() {
    const currentUser = sessionStorage.getItem('currentUser');
    if (!currentUser) {
        alert('Please login first!');
        window.location.href = 'login.html';
    }
}

function showAddCropForm() {
    document.getElementById('addCropForm').style.display = 'block';
}

function hideAddCropForm() {
    document.getElementById('addCropForm').style.display = 'none';
    document.getElementById('cropForm').reset();
}

function addCrop() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    const crop = {
        id: Date.now(),
        type: document.getElementById('cropType').value,
        quantity: parseInt(document.getElementById('quantity').value),
        quality: document.getElementById('quality').value,
        pricePerKg: parseInt(document.getElementById('pricePerKg').value),
        harvestDate: document.getElementById('harvestDate').value,
        farmer: currentUser.username,
        addedAt: new Date().toISOString()
    };
    
    // Get existing crops
    let crops = JSON.parse(localStorage.getItem('crops')) || [];
    crops.push(crop);
    
    // Save to localStorage
    localStorage.setItem('crops', JSON.stringify(crops));
    
    alert('Crop added successfully!');
    hideAddCropForm();
    loadCrops();
}

function loadCrops() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const userRole = currentUser.role.toLowerCase();
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    const tbody = document.getElementById('cropsTableBody');
    
    if (crops.length === 0) {
        tbody.innerHTML = '<tr><td colspan="7">No crops available</td></tr>';
        return;
    }
    
    tbody.innerHTML = '';
    
    let filteredCrops = crops;
    
    // Filter based on user role
    if (userRole === 'farmer') {
        filteredCrops = crops.filter(crop => crop.farmer === currentUser.username);
    }
    // Buyers and others see all crops
    
    if (filteredCrops.length === 0) {
        tbody.innerHTML = '<tr><td colspan="7">No crops available</td></tr>';
        return;
    }
    
    filteredCrops.forEach((crop, index) => {
        const row = document.createElement('tr');
        let actionHTML = '';
        
        if (userRole === 'buyer') {
            actionHTML = `<button class="btn btn-secondary" onclick="viewCropDetails(${crop.id})">Order</button>`;
        } else if (userRole === 'farmer') {
            actionHTML = `<button class="btn btn-secondary" onclick="viewCropDetails(${crop.id})">View</button>`;
        } else {
            actionHTML = `<button class="btn btn-secondary" onclick="viewCropDetails(${crop.id})">View</button>`;
        }
        
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${crop.type}</td>
            <td>${crop.quantity}</td>
            <td>${crop.quality}</td>
            <td>${crop.pricePerKg}</td>
            <td>${crop.farmer}</td>
            <td>${actionHTML}</td>
        `;
        tbody.appendChild(row);
    });
}

function searchCrops() {
    const searchTerm = document.getElementById('searchCrop').value.toLowerCase();
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    
    const filteredCrops = crops.filter(crop => 
        crop.type.toLowerCase().includes(searchTerm)
    );
    
    const tbody = document.getElementById('cropsTableBody');
    
    if (filteredCrops.length === 0) {
        tbody.innerHTML = '<tr><td colspan="7">No crops found</td></tr>';
        return;
    }
    
    tbody.innerHTML = '';
    filteredCrops.forEach((crop, index) => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${crop.type}</td>
            <td>${crop.quantity}</td>
            <td>${crop.quality}</td>
            <td>${crop.pricePerKg}</td>
            <td>${crop.farmer}</td>
            <td>
                <button class="btn btn-secondary" onclick="viewCropDetails(${crop.id})">View</button>
            </td>
        `;
        tbody.appendChild(row);
    });
}

function viewCropDetails(cropId) {
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    const crop = crops.find(c => c.id === cropId);
    
    if (crop) {
        alert(`Crop Details:\n\nType: ${crop.type}\nQuantity: ${crop.quantity} kg\nQuality: ${crop.quality}\nPrice: PKR ${crop.pricePerKg}/kg\nFarmer: ${crop.farmer}\nHarvest Date: ${crop.harvestDate}`);
    }
}
