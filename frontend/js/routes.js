// Routes JavaScript - Dijkstra's Algorithm Implementation
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();
    
    // Logout functionality
    document.getElementById('logoutBtn').addEventListener('click', function(e) {
        e.preventDefault();
        sessionStorage.removeItem('currentUser');
        window.location.href = 'login.html';
    });
    
    // Route form submission
    document.getElementById('routeForm').addEventListener('submit', function(e) {
        e.preventDefault();
        calculateShortestRoute();
    });
});

function checkAuth() {
    const currentUser = sessionStorage.getItem('currentUser');
    if (!currentUser) {
        alert('Please login first!');
        window.location.href = 'login.html';
    }
}

// Graph representation of Pakistan cities
const cityGraph = {
    'Lahore': { 'Islamabad': 375, 'Multan': 342, 'Faisalabad': 130, 'Sialkot': 120, 'Karachi': 1200 },
    'Karachi': { 'Lahore': 1200, 'Islamabad': 1400, 'Multan': 900, 'Quetta': 680, 'Faisalabad': 1000 },
    'Islamabad': { 'Lahore': 375, 'Karachi': 1400, 'Multan': 550, 'Faisalabad': 350, 'Peshawar': 180, 'Sialkot': 250 },
    'Multan': { 'Lahore': 342, 'Karachi': 900, 'Islamabad': 550, 'Faisalabad': 340, 'Quetta': 800 },
    'Faisalabad': { 'Lahore': 130, 'Karachi': 1000, 'Islamabad': 350, 'Multan': 340, 'Sialkot': 200 },
    'Peshawar': { 'Islamabad': 180, 'Quetta': 850 },
    'Quetta': { 'Karachi': 680, 'Multan': 800, 'Peshawar': 850 },
    'Sialkot': { 'Lahore': 120, 'Islamabad': 250, 'Faisalabad': 200 }
};

// Dijkstra's Algorithm Implementation
function dijkstra(graph, start, end) {
    // Initialize distances and visited set
    const distances = {};
    const previous = {};
    const unvisited = new Set();
    
    // Initialize all distances to infinity
    for (let city in graph) {
        distances[city] = Infinity;
        previous[city] = null;
        unvisited.add(city);
    }
    
    // Distance to start is 0
    distances[start] = 0;
    
    while (unvisited.size > 0) {
        // Find unvisited city with minimum distance
        let current = null;
        let minDistance = Infinity;
        
        for (let city of unvisited) {
            if (distances[city] < minDistance) {
                minDistance = distances[city];
                current = city;
            }
        }
        
        // If we reached the destination
        if (current === end) {
            break;
        }
        
        // If no path exists
        if (current === null || minDistance === Infinity) {
            break;
        }
        
        // Remove current from unvisited
        unvisited.delete(current);
        
        // Update distances to neighbors
        const neighbors = graph[current] || {};
        for (let neighbor in neighbors) {
            const distance = distances[current] + neighbors[neighbor];
            
            if (distance < distances[neighbor]) {
                distances[neighbor] = distance;
                previous[neighbor] = current;
            }
        }
    }
    
    // Reconstruct path
    const path = [];
    let current = end;
    
    while (current !== null) {
        path.unshift(current);
        current = previous[current];
    }
    
    return {
        distance: distances[end],
        path: path
    };
}

function calculateShortestRoute() {
    const startCity = document.getElementById('startCity').value;
    const endCity = document.getElementById('endCity').value;
    
    if (!startCity || !endCity) {
        alert('Please select both start and destination cities!');
        return;
    }
    
    if (startCity === endCity) {
        alert('Start and destination cities cannot be the same!');
        return;
    }
    
    // Run Dijkstra's algorithm
    const result = dijkstra(cityGraph, startCity, endCity);
    
    if (result.distance === Infinity) {
        document.getElementById('routeResult').innerHTML = `
            <div class="alert alert-danger">
                <strong>❌ No route found!</strong><br>
                There is no direct or indirect connection between ${startCity} and ${endCity}.
            </div>
        `;
        document.getElementById('routeResult').style.display = 'block';
        return;
    }
    
    // Calculate estimated time (assuming average speed of 80 km/h)
    const hours = Math.floor(result.distance / 80);
    const minutes = Math.floor((result.distance % 80) / 80 * 60);
    const estimatedTime = `${hours} hours ${minutes} minutes`;
    
    // Display results
    document.getElementById('resultStart').textContent = startCity;
    document.getElementById('resultEnd').textContent = endCity;
    document.getElementById('resultDistance').textContent = result.distance;
    document.getElementById('resultTime').textContent = estimatedTime;
    document.getElementById('resultPath').textContent = result.path.join(' → ');
    
    document.getElementById('routeResult').style.display = 'block';
    
    // Scroll to results
    document.getElementById('routeResult').scrollIntoView({ behavior: 'smooth' });
}
