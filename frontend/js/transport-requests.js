// Transport Provider Request Management
let selectedRequestId = null;

document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    loadPendingRequests();
    loadActiveDeliveries();
    loadCompletedDeliveries();
    loadFleet();

    // Logout
    document.getElementById('logoutBtn').addEventListener('click', function(e) {
        e.preventDefault();
        sessionStorage.removeItem('currentUser');
        window.location.href = 'login.html';
    });

    // Vehicle form
    document.getElementById('vehicleForm').addEventListener('submit', function(e) {
        e.preventDefault();
        addVehicle();
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
    if (user.role.toLowerCase() !== 'transport_provider') {
        alert('This page is for transport providers only!');
        window.location.href = 'dashboard.html';
    }
}

function switchTab(tabName) {
    // Hide all tabs
    document.getElementById('pendingTab').style.display = 'none';
    document.getElementById('activeTab').style.display = 'none';
    document.getElementById('completedTab').style.display = 'none';
    document.getElementById('fleetTab').style.display = 'none';

    // Remove active class
    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));

    // Show selected tab
    if (tabName === 'pending') {
        document.getElementById('pendingTab').style.display = 'block';
        event.target.classList.add('active');
        loadPendingRequests();
    } else if (tabName === 'active') {
        document.getElementById('activeTab').style.display = 'block';
        event.target.classList.add('active');
        loadActiveDeliveries();
    } else if (tabName === 'completed') {
        document.getElementById('completedTab').style.display = 'block';
        event.target.classList.add('active');
        loadCompletedDeliveries();
    } else if (tabName === 'fleet') {
        document.getElementById('fleetTab').style.display = 'block';
        event.target.classList.add('active');
        loadFleet();
    }
}

function loadPendingRequests() {
    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];

    // Get pending requests (not accepted, not rejected)
    const pending = transportRequests.filter(tr => !tr.accepted && !tr.rejected);

    const tbody = document.getElementById('pendingTableBody');
    
    if (pending.length === 0) {
        tbody.innerHTML = '<tr><td colspan="7" class="text-center">No pending requests</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    pending.forEach((tr, index) => {
        const row = document.createElement('tr');
        const cropList = tr.crops ? tr.crops.join(', ') : 'Multiple crops';
        const posted = new Date(tr.requestDate).toLocaleDateString();
        
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${tr.requestedBy || 'Farmer'}</td>
            <td>${cropList}</td>
            <td>${tr.totalWeight || 0} kg</td>
            <td>${tr.budget || 0} PKR</td>
            <td>${posted}</td>
            <td>
                <button class="btn btn-sm btn-primary" onclick="viewRequestDetails(${tr.id})">View & Respond</button>
            </td>
        `;
        tbody.appendChild(row);
    });
}

function loadActiveDeliveries() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];

    // Get active requests (accepted, not completed)
    const active = transportRequests.filter(tr => tr.accepted && !tr.completed && tr.acceptedBy === currentUser.username);

    const tbody = document.getElementById('activeTableBody');
    
    if (active.length === 0) {
        tbody.innerHTML = '<tr><td colspan="7" class="text-center">No active deliveries</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    active.forEach((tr, index) => {
        const row = document.createElement('tr');
        const progress = 50; // Simulate progress
        
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${tr.requestedBy || 'Farmer'}</td>
            <td>Multiple destinations</td>
            <td>${tr.totalWeight || 0} kg</td>
            <td>In Transit</td>
            <td>
                <div style="background-color: #e0e0e0; border-radius: 5px; height: 15px;">
                    <div style="background-color: #2c5f2d; width: ${progress}%; height: 100%; border-radius: 5px;"></div>
                </div>
                ${progress}%
            </td>
            <td>
                <button class="btn btn-sm btn-primary" onclick="completeDelivery(${tr.id})">Mark Complete</button>
            </td>
        `;
        tbody.appendChild(row);
    });
}

function loadCompletedDeliveries() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];

    // Get completed requests
    const completed = transportRequests.filter(tr => tr.completed && tr.acceptedBy === currentUser.username);

    document.getElementById('totalCompleted').textContent = completed.length;
    
    let totalEarnings = 0;
    completed.forEach(tr => {
        totalEarnings += tr.earnings || 0;
    });
    document.getElementById('totalEarnings').textContent = totalEarnings + ' PKR';

    const tbody = document.getElementById('completedTableBody');
    
    if (completed.length === 0) {
        tbody.innerHTML = '<tr><td colspan="6" class="text-center">No completed deliveries</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    completed.forEach((tr, index) => {
        const row = document.createElement('tr');
        const completedDate = new Date(tr.completionDate).toLocaleDateString();
        
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${tr.requestedBy || 'Farmer'}</td>
            <td>${tr.totalWeight || 0} kg</td>
            <td>${tr.earnings || 0} PKR</td>
            <td>${completedDate}</td>
            <td>⭐⭐⭐⭐⭐</td>
        `;
        tbody.appendChild(row);
    });
}

function loadFleet() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const vehicles = JSON.parse(localStorage.getItem('vehicles')) || [];

    // Get user's vehicles
    const myVehicles = vehicles.filter(v => v.owner === currentUser.username);

    const tbody = document.getElementById('fleetTableBody');
    
    if (myVehicles.length === 0) {
        tbody.innerHTML = '<tr><td colspan="7" class="text-center">No vehicles registered. Add one to get started!</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    myVehicles.forEach((vehicle, index) => {
        const status = vehicle.available ? 'Available' : 'In Use';
        const row = document.createElement('tr');
        
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${vehicle.type}</td>
            <td>${vehicle.capacity}</td>
            <td>${vehicle.number}</td>
            <td><span class="badge ${vehicle.available ? 'status-approved' : 'status-paid'}">${status}</span></td>
            <td>${vehicle.trips || 0}</td>
            <td>
                <button class="btn btn-sm btn-secondary" onclick="editVehicle(${vehicle.id})">Edit</button>
            </td>
        `;
        tbody.appendChild(row);
    });
}

function viewRequestDetails(requestId) {
    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];
    const request = transportRequests.find(tr => tr.id === requestId);

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
            <p><strong>From:</strong> ${request.requestedBy || 'Farmer'}</p>
            <p><strong>Total Weight:</strong> ${request.totalWeight || 0} kg</p>
            <p><strong>Budget:</strong> ${request.budget || 0} PKR</p>
            <p><strong>Crops:</strong> ${request.crops ? request.crops.join(', ') : 'Unknown'}</p>
            <p><strong>Date Posted:</strong> ${new Date(request.requestDate).toLocaleString()}</p>
        </div>
    `;

    // Load available vehicles for assignment
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const vehicles = JSON.parse(localStorage.getItem('vehicles')) || [];
    const myVehicles = vehicles.filter(v => v.owner === currentUser.username && v.available);

    const vehicleSelector = document.getElementById('vehicleSelector');
    if (myVehicles.length === 0) {
        vehicleSelector.innerHTML = '<p class="text-danger">⚠️ No available vehicles for this delivery!</p>';
    } else {
        let suitableVehicles = myVehicles.filter(v => v.capacity >= (request.totalWeight || 0));
        if (suitableVehicles.length === 0) {
            vehicleSelector.innerHTML = '<p class="text-warning">⚠️ No vehicle with sufficient capacity!</p>';
        } else {
            vehicleSelector.innerHTML = '<h5>Available Vehicles:</h5>';
            suitableVehicles.forEach(v => {
                const div = document.createElement('div');
                div.className = 'vehicle-option';
                div.innerHTML = `
                    <label>
                        <input type="radio" name="selectedVehicle" value="${v.id}">
                        ${v.type} (${v.number}) - Capacity: ${v.capacity}kg
                    </label>
                </div>
                `;
                vehicleSelector.appendChild(div);
            });
        }
    }

    // Show modal
    document.getElementById('requestModal').style.display = 'block';
}

function acceptRequest() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const selectedVehicle = document.querySelector('input[name="selectedVehicle"]:checked');

    if (!selectedVehicle) {
        alert('Please select a vehicle for this delivery!');
        return;
    }

    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];
    const request = transportRequests.find(tr => tr.id === selectedRequestId);

    if (!request) {
        alert('Request not found!');
        return;
    }

    // Update request
    request.accepted = true;
    request.acceptedBy = currentUser.username;
    request.assignedVehicleId = parseInt(selectedVehicle.value);
    request.acceptanceDate = new Date().toISOString();

    // Update vehicle status
    const vehicles = JSON.parse(localStorage.getItem('vehicles')) || [];
    const vehicle = vehicles.find(v => v.id === request.assignedVehicleId);
    if (vehicle) {
        vehicle.available = false;
        vehicle.trips = (vehicle.trips || 0) + 1;
        localStorage.setItem('vehicles', JSON.stringify(vehicles));
    }

    // Save updated requests
    localStorage.setItem('transportRequests', JSON.stringify(transportRequests));

    alert(`Request #${request.id} accepted! Vehicle assigned.`);
    closeRequestModal();
    loadPendingRequests();
    loadActiveDeliveries();
    loadFleet();
}

function rejectRequest() {
    if (!confirm('Are you sure you want to reject this request?')) {
        return;
    }

    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];
    const request = transportRequests.find(tr => tr.id === selectedRequestId);

    if (!request) {
        alert('Request not found!');
        return;
    }

    request.rejected = true;
    localStorage.setItem('transportRequests', JSON.stringify(transportRequests));

    alert(`Request #${request.id} rejected.`);
    closeRequestModal();
    loadPendingRequests();
}

function completeDelivery(requestId) {
    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];
    const request = transportRequests.find(tr => tr.id === requestId);

    if (!request) {
        alert('Request not found!');
        return;
    }

    request.completed = true;
    request.completionDate = new Date().toISOString();

    // Mark vehicle as available again
    const vehicles = JSON.parse(localStorage.getItem('vehicles')) || [];
    const vehicle = vehicles.find(v => v.id === request.assignedVehicleId);
    if (vehicle) {
        vehicle.available = true;
        localStorage.setItem('vehicles', JSON.stringify(vehicles));
    }

    // Calculate earnings
    request.earnings = request.budget || 0;

    localStorage.setItem('transportRequests', JSON.stringify(transportRequests));

    alert(`Delivery #${requestId} completed! Earnings: ${request.earnings} PKR`);
    loadActiveDeliveries();
    loadCompletedDeliveries();
    loadFleet();
}

function showAddVehicleForm() {
    document.getElementById('addVehicleForm').style.display = 'block';
}

function hideAddVehicleForm() {
    document.getElementById('addVehicleForm').style.display = 'none';
    document.getElementById('vehicleForm').reset();
}

function addVehicle() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    const vehicle = {
        id: Date.now(),
        type: document.getElementById('vehicleType').value,
        capacity: parseInt(document.getElementById('vehicleCapacity').value),
        number: document.getElementById('vehicleNumber').value,
        owner: currentUser.username,
        available: true,
        trips: 0,
        addedAt: new Date().toISOString()
    };

    let vehicles = JSON.parse(localStorage.getItem('vehicles')) || [];
    vehicles.push(vehicle);
    localStorage.setItem('vehicles', JSON.stringify(vehicles));

    alert('Vehicle registered successfully!');
    hideAddVehicleForm();
    loadFleet();
}

function editVehicle(vehicleId) {
    alert('Edit vehicle feature coming soon!');
}

function closeRequestModal() {
    document.getElementById('requestModal').style.display = 'none';
    selectedRequestId = null;
}
