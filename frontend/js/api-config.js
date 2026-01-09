// API Configuration
const API_BASE_URL = 'http://localhost:5000/api';

// API Helper function
async function apiCall(endpoint, method = 'GET', data = null) {
    const options = {
        method: method,
        headers: {
            'Content-Type': 'application/json',
        }
    };
    
    if (data && method !== 'GET') {
        options.body = JSON.stringify(data);
    }
    
    try {
        const response = await fetch(`${API_BASE_URL}${endpoint}`, options);
        const result = await response.json();
        return result;
    } catch (error) {
        console.error('API call failed:', error);
        return { success: false, message: 'Connection error. Make sure C++ server is running on port 8080.' };
    }
}

// Export for use in other files
window.apiCall = apiCall;
window.API_BASE_URL = API_BASE_URL;
