// Storage Owner Request Management
let selectedRequestId = null;

document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    loadPendingRequests();
    loadApprovedRequests();
    loadReleasedCrops();
    loadCapacityInfo();

    // Logout
    document.getElementById('logoutBtn').addEventListener('click', function(e) {
        e.preventDefault();
        sessionStorage.removeItem('currentUser');
        window.location.href = 'login.html';
    });

    // Capacity form
    document.getElementById('capacityForm').addEventListener('submit', function(e) {
        e.preventDefault();
        updateCapacity();
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
    if (user.role.toLowerCase() !== 'storage_owner') {
        alert('This page is for storage owners only!');
        window.location.href = 'dashboard.html';
    }
}

function switchTab(tabName) {
    // Hide all tabs
    document.getElementById('pendingTab').style.display = 'none';
    document.getElementById('approvedTab').style.display = 'none';
    document.getElementById('releasedTab').style.display = 'none';
    document.getElementById('capacityTab').style.display = 'none';

    // Remove active class
    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));

    // Show selected tab
    if (tabName === 'pending') {
        document.getElementById('pendingTab').style.display = 'block';
        event.target.classList.add('active');
        loadPendingRequests();
    } else if (tabName === 'approved') {
        document.getElementById('approvedTab').style.display = 'block';
        event.target.classList.add('active');
        loadApprovedRequests();
    } else if (tabName === 'released') {
        document.getElementById('releasedTab').style.display = 'block';
        event.target.classList.add('active');
        loadReleasedCrops();
    } else if (tabName === 'capacity') {
        document.getElementById('capacityTab').style.display = 'block';
        event.target.classList.add('active');
        loadCapacityInfo();
    }
}

function loadPendingRequests() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const storageRequests = JSON.parse(localStorage.getItem('storageRequests')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];

    // Get pending requests for this owner (not accepted, not rejected)
    const pending = storageRequests.filter(sr => 
        sr.organization === currentUser.username && 
        !sr.accepted && 
        !sr.rejected
    );

    const tbody = document.getElementById('pendingTableBody');
    
    if (pending.length === 0) {
        tbody.innerHTML = '<tr><td colspan="8" class="text-center">No pending requests</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    pending.forEach((sr, index) => {
        const row = document.createElement('tr');
        const posted = new Date(sr.registrationDate).toLocaleDateString();
        
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${sr.requesterName || 'Farmer'}</td>
            <td>${sr.cropName || 'Unknown'}</td>
            <td>${sr.quantity} kg</td>
            <td>${sr.budget || 0} PKR</td>
            <td>30 days</td>
            <td><span class="badge status-pending">Pending</span></td>
            <td>
                <button class="btn btn-sm btn-primary" onclick="viewStorageRequest(${sr.id})">Review</button>
            </td>
        `;
        tbody.appendChild(row);
    });
}

function loadApprovedRequests() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const storageRequests = JSON.parse(localStorage.getItem('storageRequests')) || [];

    // Get approved requests for this owner (accepted, not released)
    const approved = storageRequests.filter(sr => 
        sr.organization === currentUser.username && 
        sr.accepted && 
        !sr.released
    );

    const tbody = document.getElementById('approvedTableBody');
    
    if (approved.length === 0) {
        tbody.innerHTML = '<tr><td colspan="8" class="text-center">No accepted requests</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    approved.forEach((sr, index) => {
        const row = document.createElement('tr');
        const storedDate = new Date(sr.approvalDate).toLocaleDateString();
        const expectedRelease = new Date(sr.approvalDate);
        expectedRelease.setDate(expectedRelease.getDate() + 30);
        
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${sr.requesterName || 'Farmer'}</td>
            <td>${sr.cropName || 'Unknown'}</td>
            <td>${sr.quantity} kg</td>
            <td>${storedDate}</td>
            <td>${expectedRelease.toLocaleDateString()}</td>
            <td><span class="badge status-approved">Stored</span></td>
            <td>
                <button class="btn btn-sm btn-primary" onclick="releaseRequest(${sr.id})">Release</button>
            </td>
        `;
        tbody.appendChild(row);
    });
}

function loadReleasedCrops() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const storageRequests = JSON.parse(localStorage.getItem('storageRequests')) || [];

    // Get released requests
    const released = storageRequests.filter(sr => 
        sr.organization === currentUser.username && 
        sr.released
    );

    let totalReleased = 0;
    let totalRevenue = 0;
    
    released.forEach(sr => {
        totalReleased += sr.quantity;
        totalRevenue += sr.pricePerKg * sr.quantity;
    });

    document.getElementById('totalReleased').textContent = totalReleased + ' kg';
    document.getElementById('totalRevenue').textContent = totalRevenue + ' PKR';

    const tbody = document.getElementById('releasedTableBody');
    
    if (released.length === 0) {
        tbody.innerHTML = '<tr><td colspan="7" class="text-center">No released crops</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    released.forEach((sr, index) => {
        const row = document.createElement('tr');
        const storedDate = new Date(sr.approvalDate);
        const releasedDate = new Date(sr.releaseDate);
        const storageDays = Math.floor((releasedDate - storedDate) / (1000 * 60 * 60 * 24));
        const charges = sr.pricePerKg * sr.quantity;
        
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${sr.requesterName || 'Farmer'}</td>
            <td>${sr.cropName || 'Unknown'}</td>
            <td>${sr.quantity} kg</td>
            <td>${storageDays} days</td>
            <td>${charges} PKR</td>
            <td>${releasedDate.toLocaleDateString()}</td>
        `;
        tbody.appendChild(row);
    });
}

function loadCapacityInfo() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const storageCenters = JSON.parse(localStorage.getItem('storageCenters')) || [];

    // Get user's storage center
    const center = storageCenters.find(sc => sc.organization === currentUser.username);

    if (!center) {
        document.getElementById('utilizationStats').innerHTML = 
            '<p class="text-danger">No storage center registered. Please add one first.</p>';
        document.getElementById('currentCapacity').textContent = '0';
        return;
    }

    const totalCapacity = center.totalCapacity;
    const usedCapacity = totalCapacity - (center.availableCapacity || 0);
    const utilizationPercent = ((usedCapacity / totalCapacity) * 100).toFixed(1);

    // Utilization stats
    const utilizationDiv = document.getElementById('utilizationStats');
    utilizationDiv.innerHTML = `
        <div class="stat-box">
            <p><strong>Total Capacity:</strong> ${totalCapacity} kg</p>
            <p><strong>Currently Used:</strong> ${usedCapacity} kg</p>
            <p><strong>Utilization Rate:</strong> ${utilizationPercent}%</p>
            <div style="background-color: #e0e0e0; border-radius: 5px; height: 20px; margin: 10px 0;">
                <div style="background-color: #2c5f2d; width: ${utilizationPercent}%; height: 100%; border-radius: 5px;"></div>
            </div>
        </div>
    `;

    // Available capacity
    const availableDiv = document.getElementById('availableCapacityStats');
    availableDiv.innerHTML = `
        <div class="stat-box">
            <p><strong>Available:</strong> ${center.availableCapacity} kg</p>
            <p><strong>Temperature:</strong> ${center.temperature}°C</p>
            <p><strong>Location:</strong> ${center.location}</p>
        </div>
    `;

    document.getElementById('currentCapacity').textContent = totalCapacity;
}

function viewStorageRequest(requestId) {
    const storageRequests = JSON.parse(localStorage.getItem('storageRequests')) || [];
    const request = storageRequests.find(sr => sr.id === requestId);

    if (!request) {
        alert('Request not found!');
        return;
    }

    selectedRequestId = requestId;

    // Display request details
    const detailsDiv = document.getElementById('requestDetails');
    detailsDiv.innerHTML = `
        <div class="result-box">
            <p><strong>Request ID:</strong> #${request.id}</p>
            <p><strong>Farmer:</strong> ${request.requesterName || 'Unknown'}</p>
            <p><strong>Crop:</strong> ${request.cropName || 'Unknown'}</p>
            <p><strong>Quantity Requested:</strong> ${request.quantity} kg</p>
            <p><strong>Budget:</strong> ${request.budget || 0} PKR</p>
            <p><strong>Price per kg:</strong> ${request.pricePerKg || 0} PKR</p>
            <p><strong>Date Posted:</strong> ${new Date(request.registrationDate).toLocaleString()}</p>
        </div>
    `;

    document.getElementById('requestQuantity').textContent = request.quantity;

    // Show modal
    document.getElementById('requestModal').style.display = 'block';
}

function approveRequest() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const storageRequests = JSON.parse(localStorage.getItem('storageRequests')) || [];
    const storageCenters = JSON.parse(localStorage.getItem('storageCenters')) || [];
    
    const request = storageRequests.find(sr => sr.id === selectedRequestId);
    const center = storageCenters.find(sc => sc.organization === currentUser.username);

    if (!request || !center) {
        alert('Request or storage center not found!');
        return;
    }

    // Check if capacity is available
    if (center.availableCapacity < request.quantity) {
        alert(`Not enough capacity! Available: ${center.availableCapacity}kg, Requested: ${request.quantity}kg`);
        return;
    }

    // Update request
    request.accepted = true;
    request.approvalDate = new Date().toISOString();

    // Update center capacity
    center.availableCapacity -= request.quantity;

    // Save updates
    localStorage.setItem('storageRequests', JSON.stringify(storageRequests));
    localStorage.setItem('storageCenters', JSON.stringify(storageCenters));

    alert(`Request #${request.id} approved! Capacity updated.`);
    closeRequestModal();
    loadPendingRequests();
    loadApprovedRequests();
    loadCapacityInfo();
}

function rejectRequest() {
    if (!confirm('Are you sure you want to reject this request?')) {
        return;
    }

    const storageRequests = JSON.parse(localStorage.getItem('storageRequests')) || [];
    const request = storageRequests.find(sr => sr.id === selectedRequestId);

    if (!request) {
        alert('Request not found!');
        return;
    }

    request.rejected = true;
    localStorage.setItem('storageRequests', JSON.stringify(storageRequests));

    alert(`Request #${request.id} rejected.`);
    closeRequestModal();
    loadPendingRequests();
}

function releaseRequest(requestId) {
    if (!confirm('Release this crop from storage?')) {
        return;
    }

    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const storageRequests = JSON.parse(localStorage.getItem('storageRequests')) || [];
    const storageCenters = JSON.parse(localStorage.getItem('storageCenters')) || [];

    const request = storageRequests.find(sr => sr.id === requestId);
    const center = storageCenters.find(sc => sc.organization === currentUser.username);

    if (!request || !center) {
        alert('Request or storage center not found!');
        return;
    }

    // Update request
    request.released = true;
    request.releaseDate = new Date().toISOString();

    // Restore capacity
    center.availableCapacity += request.quantity;

    // Save updates
    localStorage.setItem('storageRequests', JSON.stringify(storageRequests));
    localStorage.setItem('storageCenters', JSON.stringify(storageCenters));

    alert(`Crop #${request.id} released! Capacity restored.`);
    loadApprovedRequests();
    loadReleasedCrops();
    loadCapacityInfo();
}

function updateCapacity() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const newCapacity = parseInt(document.getElementById('newCapacity').value);
    const storageCenters = JSON.parse(localStorage.getItem('storageCenters')) || [];

    const center = storageCenters.find(sc => sc.organization === currentUser.username);

    if (!center) {
        alert('Storage center not found!');
        return;
    }

    const currentUsed = center.totalCapacity - (center.availableCapacity || 0);
    
    if (newCapacity < currentUsed) {
        alert(`Cannot reduce capacity below current usage (${currentUsed}kg)!`);
        return;
    }

    const difference = newCapacity - center.totalCapacity;
    center.totalCapacity = newCapacity;
    center.availableCapacity = (center.availableCapacity || 0) + difference;

    localStorage.setItem('storageCenters', JSON.stringify(storageCenters));

    alert(`Storage capacity updated to ${newCapacity}kg!`);
    document.getElementById('capacityForm').reset();
    loadCapacityInfo();
}

function closeRequestModal() {
    document.getElementById('requestModal').style.display = 'none';
    selectedRequestId = null;
}
