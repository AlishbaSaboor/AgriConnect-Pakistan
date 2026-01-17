// Buyer Orders Management
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    loadOrders();
    loadPaymentOptions();
    loadNotifications();

    // Logout
    document.getElementById('logoutBtn').addEventListener('click', function(e) {
        e.preventDefault();
        sessionStorage.removeItem('currentUser');
        window.location.href = 'login.html';
    });

    // Payment form
    document.getElementById('paymentForm').addEventListener('submit', function(e) {
        e.preventDefault();
        processPayment();
    });

    // Payment amount update
    document.getElementById('orderIdForPayment').addEventListener('change', function() {
        updatePaymentAmount();
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
    if (user.role.toLowerCase() !== 'buyer') {
        alert('This page is for buyers only!');
        window.location.href = 'dashboard.html';
    }
}

function switchTab(tabName) {
    // Hide all tabs
    document.getElementById('ordersTab').style.display = 'none';
    document.getElementById('paymentsTab').style.display = 'none';
    document.getElementById('trackingTab').style.display = 'none';
    document.getElementById('notificationsTab').style.display = 'none';

    // Remove active class
    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));

    // Show selected tab
    if (tabName === 'orders') {
        document.getElementById('ordersTab').style.display = 'block';
        event.target.classList.add('active');
        loadOrders();
    } else if (tabName === 'payments') {
        document.getElementById('paymentsTab').style.display = 'block';
        event.target.classList.add('active');
        loadPaymentOptions();
    } else if (tabName === 'tracking') {
        document.getElementById('trackingTab').style.display = 'block';
        event.target.classList.add('active');
        loadTrackingInfo();
    } else if (tabName === 'notifications') {
        document.getElementById('notificationsTab').style.display = 'block';
        event.target.classList.add('active');
        loadNotifications();
    }
}

function loadOrders() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    
    // Filter orders for current buyer
    const myOrders = orders.filter(o => o.buyer === currentUser.username);
    
    const tbody = document.getElementById('ordersTableBody');
    
    if (myOrders.length === 0) {
        tbody.innerHTML = '<tr><td colspan="8" class="text-center">No orders yet</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    myOrders.forEach((order, index) => {
        const crop = crops.find(c => c.id === order.cropId);
        const totalCost = (order.quantity * order.price) || 0;
        
        // Determine status
        let status = 'Pending';
        let statusClass = 'status-pending';
        if (order.farmerApproved && !order.paid) {
            status = 'Approved';
            statusClass = 'status-approved';
        } else if (order.paid && !order.delivered) {
            status = 'Paid';
            statusClass = 'status-paid';
        } else if (order.delivered) {
            status = 'Completed';
            statusClass = 'status-completed';
        }

        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${crop ? crop.type : 'Unknown'}</td>
            <td>${order.quantity}</td>
            <td>${order.price}</td>
            <td>${totalCost} PKR</td>
            <td>${order.farmer}</td>
            <td><span class="badge ${statusClass}">${status}</span></td>
            <td>
                ${!order.paid && order.farmerApproved ? 
                    `<button class="btn btn-sm btn-primary" onclick="selectForPayment(${order.id})">Pay Now</button>` : 
                    '<button class="btn btn-sm btn-secondary" disabled>-</button>'}
            </td>
        `;
        tbody.appendChild(row);
    });
}

function filterOrders() {
    const filter = document.getElementById('orderFilter').value;
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    
    let myOrders = orders.filter(o => o.buyer === currentUser.username);

    if (filter === 'pending') {
        myOrders = myOrders.filter(o => !o.farmerApproved);
    } else if (filter === 'approved') {
        myOrders = myOrders.filter(o => o.farmerApproved && !o.paid);
    } else if (filter === 'paid') {
        myOrders = myOrders.filter(o => o.paid && !o.delivered);
    } else if (filter === 'completed') {
        myOrders = myOrders.filter(o => o.delivered);
    }

    const tbody = document.getElementById('ordersTableBody');
    if (myOrders.length === 0) {
        tbody.innerHTML = '<tr><td colspan="8" class="text-center">No orders in this category</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    myOrders.forEach((order, index) => {
        const crop = crops.find(c => c.id === order.cropId);
        const totalCost = (order.quantity * order.price) || 0;
        
        let status = 'Pending';
        let statusClass = 'status-pending';
        if (order.farmerApproved && !order.paid) {
            status = 'Approved';
            statusClass = 'status-approved';
        } else if (order.paid && !order.delivered) {
            status = 'Paid';
            statusClass = 'status-paid';
        } else if (order.delivered) {
            status = 'Completed';
            statusClass = 'status-completed';
        }

        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${crop ? crop.type : 'Unknown'}</td>
            <td>${order.quantity}</td>
            <td>${order.price}</td>
            <td>${totalCost} PKR</td>
            <td>${order.farmer}</td>
            <td><span class="badge ${statusClass}">${status}</span></td>
            <td>
                ${!order.paid && order.farmerApproved ? 
                    `<button class="btn btn-sm btn-primary" onclick="selectForPayment(${order.id})">Pay Now</button>` : 
                    '<button class="btn btn-sm btn-secondary" disabled>-</button>'}
            </td>
        `;
        tbody.appendChild(row);
    });
}

function loadPaymentOptions() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    
    // Get approved but unpaid orders
    const approvedOrders = orders.filter(o => 
        o.buyer === currentUser.username && 
        o.farmerApproved && 
        !o.paid
    );

    const select = document.getElementById('orderIdForPayment');
    select.innerHTML = '<option value="">-- Select Approved Order --</option>';
    
    approvedOrders.forEach(order => {
        const crop = crops.find(c => c.id === order.cropId);
        const totalCost = (order.quantity * order.price) || 0;
        const option = document.createElement('option');
        option.value = order.id;
        option.textContent = `Order #${order.id} - ${crop.type} (${order.quantity}kg) - ${totalCost} PKR`;
        select.appendChild(option);
    });

    // Load payment history
    const payments = JSON.parse(localStorage.getItem('payments')) || [];
    const myPayments = payments.filter(p => p.buyer === currentUser.username);
    
    const tbody = document.getElementById('paymentHistoryBody');
    if (myPayments.length === 0) {
        tbody.innerHTML = '<tr><td colspan="6" class="text-center">No payments yet</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    myPayments.forEach(payment => {
        const order = orders.find(o => o.id === payment.orderId);
        const crop = crops.find(c => c.id === order.cropId);
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>#${payment.orderId}</td>
            <td>${crop.type}</td>
            <td>${payment.amount} PKR</td>
            <td>${payment.method}</td>
            <td>${new Date(payment.date).toLocaleDateString()}</td>
            <td><span class="badge status-completed">Completed</span></td>
        `;
        tbody.appendChild(row);
    });
}

function updatePaymentAmount() {
    const orderId = document.getElementById('orderIdForPayment').value;
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const order = orders.find(o => o.id == orderId);
    
    if (order) {
        const totalCost = (order.quantity * order.price) || 0;
        document.getElementById('paymentAmount').textContent = totalCost;
    }
}

function selectForPayment(orderId) {
    document.getElementById('orderIdForPayment').value = orderId;
    updatePaymentAmount();
    switchTab('payments');
}

function processPayment() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const orderId = parseInt(document.getElementById('orderIdForPayment').value);
    const method = document.getElementById('paymentMethod').value;
    
    if (!orderId || !method) {
        alert('Please select an order and payment method!');
        return;
    }

    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const order = orders.find(o => o.id === orderId);
    
    if (!order) {
        alert('Order not found!');
        return;
    }

    // Mark order as paid
    order.paid = true;
    order.paymentDate = new Date().toISOString();

    // Save updated orders
    localStorage.setItem('orders', JSON.stringify(orders));

    // Record payment
    let payments = JSON.parse(localStorage.getItem('payments')) || [];
    const totalCost = (order.quantity * order.price) || 0;
    payments.push({
        orderId: orderId,
        buyer: currentUser.username,
        amount: totalCost,
        method: method,
        date: new Date().toISOString()
    });
    localStorage.setItem('payments', JSON.stringify(payments));

    alert(`Payment of ${totalCost} PKR completed successfully!`);
    document.getElementById('paymentForm').reset();
    loadPaymentOptions();
    loadOrders();
}

function loadTrackingInfo() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    const crops = JSON.parse(localStorage.getItem('crops')) || [];
    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];

    // Get paid orders
    const paidOrders = orders.filter(o => 
        o.buyer === currentUser.username && o.paid
    );

    const tbody = document.getElementById('shipmentTableBody');
    
    if (paidOrders.length === 0) {
        tbody.innerHTML = '<tr><td colspan="6" class="text-center">No active shipments</td></tr>';
        return;
    }

    tbody.innerHTML = '';
    paidOrders.forEach((order, index) => {
        const crop = crops.find(c => c.id === order.cropId);
        const transport = transportRequests.find(t => t.buyerId === order.id);
        
        let status = 'Pending Pickup';
        let progress = 0;
        
        if (transport) {
            if (transport.completed) {
                status = 'Delivered';
                progress = 100;
            } else if (transport.accepted) {
                status = 'In Transit';
                progress = 50;
            }
        }

        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${index + 1}</td>
            <td>${crop ? crop.type : 'Unknown'}</td>
            <td>${order.quantity} kg</td>
            <td>${transport ? transport.provider : 'N/A'}</td>
            <td>${status}</td>
            <td>
                <div style="background-color: #e0e0e0; border-radius: 5px; height: 15px;">
                    <div style="background-color: #2c5f2d; width: ${progress}%; height: 100%; border-radius: 5px;"></div>
                </div>
                ${progress}%
            </td>
        `;
        tbody.appendChild(row);
    });
}

function loadNotifications() {
    const currentUser = JSON.parse(sessionStorage.getItem('currentUser'));
    const transportRequests = JSON.parse(localStorage.getItem('transportRequests')) || [];
    const orders = JSON.parse(localStorage.getItem('orders')) || [];
    
    const notificationsContainer = document.getElementById('notificationsContainer');
    const notifications = [];

    // Check for transport acceptances
    transportRequests.forEach(tr => {
        if (tr.buyerId && tr.accepted) {
            notifications.push({
                type: 'success',
                message: `✓ Transport request #${tr.id} accepted by ${tr.provider}`,
                time: new Date().toLocaleString()
            });
        }
    });

    // Check for order approvals
    orders.forEach(order => {
        if (order.buyer === currentUser.username && order.farmerApproved && !order.paid) {
            notifications.push({
                type: 'info',
                message: `👨‍🌾 Farmer ${order.farmer} approved your order #${order.id}. Proceed to payment.`,
                time: new Date().toLocaleString()
            });
        }
    });

    if (notifications.length === 0) {
        notificationsContainer.innerHTML = '<div class="notification"><p>No new notifications</p></div>';
        return;
    }

    notificationsContainer.innerHTML = '';
    notifications.forEach(notif => {
        const div = document.createElement('div');
        div.className = `notification notification-${notif.type}`;
        div.innerHTML = `
            <p>${notif.message}</p>
            <small>${notif.time}</small>
        `;
        notificationsContainer.appendChild(div);
    });
}
