// Transport Management JavaScript
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    loadTransport();
    
    // Check if user is transport provider to show add transport button
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    if (currentUser.role === 'transport_provider') {
        document.getElementById('transportProviderActions').style.display = 'block';
    }
    
    // Logout functionality
    document.getElementById('logoutBtn').addEventListener('click', function(e) {
        e.preventDefault();
        sessionStorage.removeItem('currentUser');
        window.location.href = 'login.html';
    });
    
    // Add transport form submission
    document.getElementById('transportForm').addEventListener('submit', function(e) {
        e.preventDefault();
        addTransport();
    });
});

function checkAuth() {
    const currentUser = sessionStorage.getItem('currentUser');
    if (!currentUser) {
        alert('Please login first!');
        window.location.href = 'login.html';
    }
}

function showAddTransportForm() {
    document.getElementById('addTransportForm').style.display = 'block';
}

function hideAddTransportForm() {
    document.getElementById('addTransportForm').style.display = 'none';
    document.getElementById('transportForm').reset();
}

function addTransport() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    
    const transport = {
        id: Date.now(),
        provider: currentUser.username,
        type: document.getElementById('transportType').value,
        capacity: parseInt(document.getElementById('transportCapacity').value),
        route: document.getElementById('transportRoute').value,
        pricePerKm: parseInt(document.getElementById('pricePerKm').value),
        status: 'Available',
        addedAt: new Date().toISOString()
    };
    
    // Get existing transport
    let transportList = JSON.parse(localStorage.getItem('transport')) || [];
    transportList.push(transport);
    
    // Save to localStorage
    localStorage.setItem('transport', JSON.stringify(transportList));
    
    alert('Transport service added successfully!');
    hideAddTransportForm();
    loadTransport();
}

function loadTransport() {
    const transportList = JSON.parse(localStorage.getItem('transport')) || [];
    const tbody = document.getElementById('transportTableBody');
    
    if (transportList.length === 0) {
        tbody.innerHTML = '<tr><td colspan="7">No transport services available</td></tr>';
        return;
    }
    
    tbody.innerHTML = '';
    transportList.forEach((transport, index) => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${transport.provider}</td>
            <td>${transport.type}</td>
            <td>${transport.capacity}</td>
            <td>${transport.route}</td>
            <td>${transport.pricePerKm}</td>
            <td>
                <button class="btn btn-secondary" onclick="trackTransport(${transport.id})">Track</button>
            </td>
        `;
        tbody.appendChild(row);
    });
}

function trackTransport(transportId) {
    const transportList = JSON.parse(localStorage.getItem('transport')) || [];
    const transport = transportList.find(t => t.id === transportId);
    
    if (transport) {
        // Simulate tracking with progress
        const progress = Math.floor(Math.random() * 100);
        const statuses = ['Loading', 'In Transit', 'Near Destination', 'Delivered'];
        const currentStatus = statuses[Math.floor(progress / 25)];
        
        document.getElementById('trackingInfo').innerHTML = `
            <div class="result-box">
                <h4>🚚 Transport Tracking</h4>
                <p><strong>Provider:</strong> ${transport.provider}</p>
                <p><strong>Type:</strong> ${transport.type}</p>
                <p><strong>Route:</strong> ${transport.route}</p>
                <p><strong>Status:</strong> ${currentStatus}</p>
                <p><strong>Progress:</strong> ${progress}%</p>
                <div style="background-color: #e0e0e0; border-radius: 10px; height: 20px; margin-top: 10px;">
                    <div style="background-color: #2c5f2d; width: ${progress}%; height: 100%; border-radius: 10px;"></div>
                </div>
                <p style="margin-top: 10px;"><em>Real-time tracking simulation</em></p>
            </div>
        `;
    }
}
