// Registration JavaScript - Connected to C++ Backend
document.getElementById('registerForm').addEventListener('submit', async function(e) {
    e.preventDefault();
    
    const username = document.getElementById('username').value;
    const email = document.getElementById('email').value;
    const password = document.getElementById('password').value;
    const confirmPassword = document.getElementById('confirmPassword').value;
    const role = document.getElementById('role').value;
    const phone = document.getElementById('phone').value;
    const location = document.getElementById('location').value;
    
    // Validate passwords match
    if (password !== confirmPassword) {
        alert('Passwords do not match!');
        return;
    }
    
    // Call C++ backend API
    const result = await apiCall('/register', 'POST', {
        username: username,
        email: email,
        password: password,
        role: role,
        phone: phone,
        location: location
    });
    
    if (result.success) {
        alert('Registration successful! Please login.');
        window.location.href = 'login.html';
    } else {
        alert(result.message || 'Registration failed!');
    }
});
