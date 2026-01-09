// Storage Management JavaScript
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    loadStorage();
    
    // Check if user is storage owner to show add storage button
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    if (currentUser.role === 'storage_owner') {
        document.getElementById('storageOwnerActions').style.display = 'block';
    }
    
    // Logout functionality
    document.getElementById('logoutBtn').addEventListener('click', function(e) {
        e.preventDefault();
        sessionStorage.removeItem('currentUser');
        window.location.href = 'login.html';
    });
    
    // Add storage form submission
    document.getElementById('storageForm').addEventListener('submit', function(e) {
        e.preventDefault();
        addStorage();
    });
    
    // Allocate storage form submission
    document.getElementById('allocateStorageForm').addEventListener('submit', function(e) {
        e.preventDefault();
        allocateStorage();
    });
});

function checkAuth() {
    const currentUser = sessionStorage.getItem('currentUser');
    if (!currentUser) {
        alert('Please login first!');
        window.location.href = 'login.html';
    }
}

function showAddStorageForm() {
    document.getElementById('addStorageForm').style.display = 'block';
}

function hideAddStorageForm() {
    document.getElementById('addStorageForm').style.display = 'none';
    document.getElementById('storageForm').reset();
}

function addStorage() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    const capacity = parseInt(document.getElementById('capacity').value);
    
    const storage = {
        id: Date.now(),
        name: document.getElementById('storageName').value,
        location: document.getElementById('storageLocation').value,
        capacity: capacity,
        available: capacity,
        temperature: parseInt(document.getElementById('temperature').value),
        owner: currentUser.username,
        addedAt: new Date().toISOString()
    };
    
    // Get existing storage
    let storageList = JSON.parse(localStorage.getItem('storage')) || [];
    storageList.push(storage);
    
    // Save to localStorage
    localStorage.setItem('storage', JSON.stringify(storageList));
    
    alert('Storage center added successfully!');
    hideAddStorageForm();
    loadStorage();
}

function loadStorage() {
    const storageList = JSON.parse(localStorage.getItem('storage')) || [];
    const tbody = document.getElementById('storageTableBody');
    
    if (storageList.length === 0) {
        tbody.innerHTML = '<tr><td colspan="6">No storage centers available</td></tr>';
        return;
    }
    
    tbody.innerHTML = '';
    storageList.forEach((storage, index) => {
        const utilizationPercent = ((storage.capacity - storage.available) / storage.capacity * 100).toFixed(1);
        const status = storage.available > 0 ? 'Available' : 'Full';
        const statusClass = storage.available > 0 ? 'text-success' : 'text-danger';
        
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${storage.name}</td>
            <td>${storage.location}</td>
            <td>${storage.available} / ${storage.capacity}</td>
            <td>${storage.temperature}°C</td>
            <td class="${statusClass}">${status} (${utilizationPercent}% used)</td>
        `;
        tbody.appendChild(row);
    });
}

// Priority Queue Algorithm for Storage Allocation
function allocateStorage() {
    const quantity = parseInt(document.getElementById('allocateQuantity').value);
    const storageList = JSON.parse(localStorage.getItem('storage')) || [];
    
    if (storageList.length === 0) {
        document.getElementById('allocationResult').innerHTML = 
            '<div class="alert alert-danger">No storage centers available!</div>';
        return;
    }
    
    // Priority Queue Implementation using Min Heap concept
    // Calculate score for each storage: higher score = better option
    // Score = available space - penalty for temperature deviation from optimal (4°C)
    
    let bestStorage = null;
    let bestScore = -Infinity;
    
    for (let storage of storageList) {
        if (storage.available >= quantity) {
            // Optimal temperature for most crops is 4°C
            const tempPenalty = Math.abs(storage.temperature - 4) * 10;
            const score = storage.available - tempPenalty;
            
            if (score > bestScore) {
                bestScore = score;
                bestStorage = storage;
            }
        }
    }
    
    if (bestStorage) {
        // Update storage
        bestStorage.available -= quantity;
        localStorage.setItem('storage', JSON.stringify(storageList));
        
        const utilizationPercent = ((bestStorage.capacity - bestStorage.available) / bestStorage.capacity * 100).toFixed(1);
        
        document.getElementById('allocationResult').innerHTML = `
            <div class="result-box">
                <h4>✅ Storage Allocated Successfully!</h4>
                <p><strong>Storage Center:</strong> ${bestStorage.name}</p>
                <p><strong>Location:</strong> ${bestStorage.location}</p>
                <p><strong>Temperature:</strong> ${bestStorage.temperature}°C</p>
                <p><strong>Allocated Quantity:</strong> ${quantity} kg</p>
                <p><strong>Remaining Capacity:</strong> ${bestStorage.available} kg</p>
                <p><strong>Utilization:</strong> ${utilizationPercent}%</p>
                <p><strong>Algorithm Score:</strong> ${bestScore.toFixed(2)}</p>
                <hr>
                <p><em>Algorithm Used: Priority Queue (Greedy Approach)</em></p>
                <p><em>Time Complexity: O(n) where n = number of storage centers</em></p>
            </div>
        `;
        
        loadStorage();
    } else {
        document.getElementById('allocationResult').innerHTML = `
            <div class="alert alert-danger">
                <strong>❌ No suitable storage found!</strong><br>
                Required capacity: ${quantity} kg<br>
                All available storage centers are either full or have insufficient capacity.
            </div>
        `;
    }
}
