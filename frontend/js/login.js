// Login JavaScript - Connected to C++ Backend
document.getElementById('loginForm').addEventListener('submit', async function(e) {
    e.preventDefault();
    
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    
    // Call C++ backend API
    const result = await apiCall('/login', 'POST', {
        username: username,
        password: password
    });
    
    if (result.success) {
        // Store current user in sessionStorage
        sessionStorage.setItem('currentUser', JSON.stringify(result.user));
        alert('Login successful!');
        window.location.href = 'dashboard.html';
    } else {
        alert(result.message || 'Invalid username or password!');
    }
});
