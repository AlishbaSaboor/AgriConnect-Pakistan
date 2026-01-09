-- AgriConnect Pakistan Database Schema
-- MySQL Server 2019

-- Create Database
CREATE DATABASE IF NOT EXISTS agriconnect_pakistan;
USE agriconnect_pakistan;

-- Users Table
CREATE TABLE IF NOT EXISTS users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role ENUM('farmer', 'buyer', 'storage_owner', 'transport_provider', 'admin') NOT NULL,
    phone VARCHAR(20),
    location VARCHAR(100),
    registered_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_login TIMESTAMP NULL,
    is_verified BOOLEAN DEFAULT FALSE
);

-- Crops Table
CREATE TABLE IF NOT EXISTS crops (
    crop_id INT PRIMARY KEY AUTO_INCREMENT,
    farmer_id INT NOT NULL,
    crop_type VARCHAR(50) NOT NULL,
    quantity INT NOT NULL,
    quality_grade ENUM('A', 'B', 'C') NOT NULL,
    price_per_kg DECIMAL(10, 2) NOT NULL,
    harvest_date DATE NOT NULL,
    listed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status ENUM('available', 'sold', 'in_storage') DEFAULT 'available',
    FOREIGN KEY (farmer_id) REFERENCES users(user_id) ON DELETE CASCADE
);

-- Cold Storage Centers Table
CREATE TABLE IF NOT EXISTS storage_centers (
    storage_id INT PRIMARY KEY AUTO_INCREMENT,
    owner_id INT NOT NULL,
    storage_name VARCHAR(100) NOT NULL,
    location VARCHAR(100) NOT NULL,
    total_capacity INT NOT NULL,
    available_capacity INT NOT NULL,
    temperature INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status ENUM('active', 'inactive', 'maintenance') DEFAULT 'active',
    FOREIGN KEY (owner_id) REFERENCES users(user_id) ON DELETE CASCADE
);

-- Storage Allocations Table
CREATE TABLE IF NOT EXISTS storage_allocations (
    allocation_id INT PRIMARY KEY AUTO_INCREMENT,
    storage_id INT NOT NULL,
    crop_id INT NOT NULL,
    allocated_quantity INT NOT NULL,
    allocated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    release_date DATE NULL,
    status ENUM('active', 'released') DEFAULT 'active',
    FOREIGN KEY (storage_id) REFERENCES storage_centers(storage_id) ON DELETE CASCADE,
    FOREIGN KEY (crop_id) REFERENCES crops(crop_id) ON DELETE CASCADE
);

-- Transport Services Table
CREATE TABLE IF NOT EXISTS transport_services (
    transport_id INT PRIMARY KEY AUTO_INCREMENT,
    provider_id INT NOT NULL,
    vehicle_type VARCHAR(50) NOT NULL,
    capacity INT NOT NULL,
    route VARCHAR(100) NOT NULL,
    price_per_km DECIMAL(10, 2) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    status ENUM('available', 'in_transit', 'unavailable') DEFAULT 'available',
    FOREIGN KEY (provider_id) REFERENCES users(user_id) ON DELETE CASCADE
);

-- Orders Table
CREATE TABLE IF NOT EXISTS orders (
    order_id INT PRIMARY KEY AUTO_INCREMENT,
    buyer_id INT NOT NULL,
    crop_id INT NOT NULL,
    quantity INT NOT NULL,
    total_amount DECIMAL(10, 2) NOT NULL,
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    delivery_address VARCHAR(255),
    status ENUM('pending', 'confirmed', 'in_transit', 'delivered', 'cancelled') DEFAULT 'pending',
    FOREIGN KEY (buyer_id) REFERENCES users(user_id) ON DELETE CASCADE,
    FOREIGN KEY (crop_id) REFERENCES crops(crop_id) ON DELETE CASCADE
);

-- Transport Bookings Table
CREATE TABLE IF NOT EXISTS transport_bookings (
    booking_id INT PRIMARY KEY AUTO_INCREMENT,
    order_id INT NOT NULL,
    transport_id INT NOT NULL,
    pickup_location VARCHAR(100) NOT NULL,
    delivery_location VARCHAR(100) NOT NULL,
    distance_km INT NOT NULL,
    transport_cost DECIMAL(10, 2) NOT NULL,
    booking_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    pickup_time TIMESTAMP NULL,
    delivery_time TIMESTAMP NULL,
    status ENUM('booked', 'in_transit', 'delivered', 'cancelled') DEFAULT 'booked',
    FOREIGN KEY (order_id) REFERENCES orders(order_id) ON DELETE CASCADE,
    FOREIGN KEY (transport_id) REFERENCES transport_services(transport_id) ON DELETE CASCADE
);

-- Payments Table
CREATE TABLE IF NOT EXISTS payments (
    payment_id INT PRIMARY KEY AUTO_INCREMENT,
    order_id INT NOT NULL,
    payer_id INT NOT NULL,
    amount DECIMAL(10, 2) NOT NULL,
    payment_method ENUM('cash', 'easypaisa', 'jazzcash', 'bank_transfer') NOT NULL,
    payment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    transaction_id VARCHAR(100) NULL,
    status ENUM('pending', 'completed', 'failed', 'refunded') DEFAULT 'pending',
    FOREIGN KEY (order_id) REFERENCES orders(order_id) ON DELETE CASCADE,
    FOREIGN KEY (payer_id) REFERENCES users(user_id) ON DELETE CASCADE
);

-- Routes Network Table (For Dijkstra's Algorithm)
CREATE TABLE IF NOT EXISTS city_routes (
    route_id INT PRIMARY KEY AUTO_INCREMENT,
    city_from VARCHAR(50) NOT NULL,
    city_to VARCHAR(50) NOT NULL,
    distance_km INT NOT NULL,
    estimated_time_hours DECIMAL(5, 2) NOT NULL
);

-- System Logs Table
CREATE TABLE IF NOT EXISTS system_logs (
    log_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NULL,
    action VARCHAR(100) NOT NULL,
    description TEXT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE SET NULL
);

-- Insert Default Admin User
INSERT INTO users (username, email, password, role, phone, location, is_verified)
VALUES ('admin', 'admin@agriconnect.pk', 'admin123', 'admin', '03001234567', 'Islamabad', TRUE);

-- Insert Sample City Routes for Dijkstra's Algorithm
INSERT INTO city_routes (city_from, city_to, distance_km, estimated_time_hours) VALUES
('Lahore', 'Islamabad', 375, 4.7),
('Lahore', 'Multan', 342, 4.3),
('Lahore', 'Faisalabad', 130, 1.6),
('Lahore', 'Sialkot', 120, 1.5),
('Lahore', 'Karachi', 1200, 15.0),
('Karachi', 'Multan', 900, 11.3),
('Karachi', 'Quetta', 680, 8.5),
('Karachi', 'Islamabad', 1400, 17.5),
('Islamabad', 'Peshawar', 180, 2.3),
('Islamabad', 'Multan', 550, 6.9),
('Islamabad', 'Faisalabad', 350, 4.4),
('Multan', 'Faisalabad', 340, 4.3),
('Multan', 'Quetta', 800, 10.0),
('Faisalabad', 'Sialkot', 200, 2.5),
('Peshawar', 'Quetta', 850, 10.6);

-- Create Indexes for Better Performance
CREATE INDEX idx_users_username ON users(username);
CREATE INDEX idx_users_role ON users(role);
CREATE INDEX idx_crops_farmer ON crops(farmer_id);
CREATE INDEX idx_crops_status ON crops(status);
CREATE INDEX idx_storage_location ON storage_centers(location);
CREATE INDEX idx_orders_buyer ON orders(buyer_id);
CREATE INDEX idx_orders_status ON orders(status);
CREATE INDEX idx_payments_status ON payments(status);

-- Create Views for Reports

-- View: User Statistics
CREATE VIEW vw_user_statistics AS
SELECT 
    role,
    COUNT(*) as user_count,
    COUNT(CASE WHEN is_verified = TRUE THEN 1 END) as verified_count
FROM users
GROUP BY role;

-- View: Crop Summary
CREATE VIEW vw_crop_summary AS
SELECT 
    c.crop_type,
    COUNT(*) as total_listings,
    SUM(c.quantity) as total_quantity,
    AVG(c.price_per_kg) as avg_price,
    u.username as farmer_name
FROM crops c
JOIN users u ON c.farmer_id = u.user_id
GROUP BY c.crop_type, u.username;

-- View: Storage Utilization
CREATE VIEW vw_storage_utilization AS
SELECT 
    s.storage_name,
    s.location,
    s.total_capacity,
    s.available_capacity,
    (s.total_capacity - s.available_capacity) as used_capacity,
    ROUND(((s.total_capacity - s.available_capacity) / s.total_capacity * 100), 2) as utilization_percentage
FROM storage_centers s
WHERE s.status = 'active';

-- View: Active Orders with Details
CREATE VIEW vw_active_orders AS
SELECT 
    o.order_id,
    u.username as buyer_name,
    c.crop_type,
    o.quantity,
    o.total_amount,
    o.status,
    o.order_date
FROM orders o
JOIN users u ON o.buyer_id = u.user_id
JOIN crops c ON o.crop_id = c.crop_id
WHERE o.status IN ('pending', 'confirmed', 'in_transit');

-- View: Transport Statistics
CREATE VIEW vw_transport_statistics AS
SELECT 
    t.vehicle_type,
    COUNT(*) as total_vehicles,
    SUM(t.capacity) as total_capacity,
    COUNT(CASE WHEN t.status = 'available' THEN 1 END) as available_vehicles
FROM transport_services t
GROUP BY t.vehicle_type;

-- Stored Procedure: Allocate Storage (Priority Queue Logic)
DELIMITER //

CREATE PROCEDURE sp_allocate_storage(
    IN p_crop_id INT,
    IN p_quantity INT,
    OUT p_storage_id INT,
    OUT p_message VARCHAR(255)
)
BEGIN
    DECLARE v_best_storage_id INT;
    DECLARE v_best_score INT DEFAULT -999999;
    DECLARE v_score INT;
    DECLARE v_available INT;
    DECLARE v_temp INT;
    DECLARE done INT DEFAULT FALSE;
    
    DECLARE storage_cursor CURSOR FOR
        SELECT storage_id, available_capacity, temperature
        FROM storage_centers
        WHERE status = 'active' AND available_capacity >= p_quantity;
    
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
    
    OPEN storage_cursor;
    
    read_loop: LOOP
        FETCH storage_cursor INTO v_best_storage_id, v_available, v_temp;
        IF done THEN
            LEAVE read_loop;
        END IF;
        
        -- Calculate score: available space - temperature penalty
        SET v_score = v_available - (ABS(v_temp - 4) * 10);
        
        IF v_score > v_best_score THEN
            SET v_best_score = v_score;
            SET p_storage_id = v_best_storage_id;
        END IF;
    END LOOP;
    
    CLOSE storage_cursor;
    
    IF p_storage_id IS NOT NULL THEN
        -- Update storage capacity
        UPDATE storage_centers
        SET available_capacity = available_capacity - p_quantity
        WHERE storage_id = p_storage_id;
        
        -- Create allocation record
        INSERT INTO storage_allocations (storage_id, crop_id, allocated_quantity)
        VALUES (p_storage_id, p_crop_id, p_quantity);
        
        SET p_message = 'Storage allocated successfully';
    ELSE
        SET p_message = 'No suitable storage found';
    END IF;
END //

DELIMITER ;

-- Function: Calculate Route Distance (For Dijkstra's Algorithm)
DELIMITER //

CREATE FUNCTION fn_get_route_distance(
    p_city_from VARCHAR(50),
    p_city_to VARCHAR(50)
)
RETURNS INT
DETERMINISTIC
BEGIN
    DECLARE v_distance INT;
    
    SELECT distance_km INTO v_distance
    FROM city_routes
    WHERE (city_from = p_city_from AND city_to = p_city_to)
       OR (city_from = p_city_to AND city_to = p_city_from)
    LIMIT 1;
    
    IF v_distance IS NULL THEN
        SET v_distance = 9999; -- Infinity
    END IF;
    
    RETURN v_distance;
END //

DELIMITER ;

-- Trigger: Update Crop Status on Storage Allocation
DELIMITER //

CREATE TRIGGER trg_update_crop_status
AFTER INSERT ON storage_allocations
FOR EACH ROW
BEGIN
    UPDATE crops
    SET status = 'in_storage'
    WHERE crop_id = NEW.crop_id;
END //

DELIMITER ;

-- Trigger: Log User Actions
DELIMITER //

CREATE TRIGGER trg_log_user_login
AFTER UPDATE ON users
FOR EACH ROW
BEGIN
    IF NEW.last_login != OLD.last_login THEN
        INSERT INTO system_logs (user_id, action, description)
        VALUES (NEW.user_id, 'LOGIN', CONCAT('User ', NEW.username, ' logged in'));
    END IF;
END //

DELIMITER ;

-- Sample Data for Testing

-- Insert Sample Farmers
INSERT INTO users (username, email, password, role, phone, location, is_verified) VALUES
('farmer1', 'farmer1@test.com', 'pass123', 'farmer', '03001111111', 'Lahore', TRUE),
('farmer2', 'farmer2@test.com', 'pass123', 'farmer', '03002222222', 'Multan', TRUE);

-- Insert Sample Buyers
INSERT INTO users (username, email, password, role, phone, location, is_verified) VALUES
('buyer1', 'buyer1@test.com', 'pass123', 'buyer', '03003333333', 'Karachi', TRUE),
('buyer2', 'buyer2@test.com', 'pass123', 'buyer', '03004444444', 'Islamabad', TRUE);

-- Insert Sample Storage Owners
INSERT INTO users (username, email, password, role, phone, location, is_verified) VALUES
('storage1', 'storage1@test.com', 'pass123', 'storage_owner', '03005555555', 'Lahore', TRUE);

-- Insert Sample Transport Providers
INSERT INTO users (username, email, password, role, phone, location, is_verified) VALUES
('transport1', 'transport1@test.com', 'pass123', 'transport_provider', '03006666666', 'Karachi', TRUE);

-- Display Success Message
SELECT 'Database schema created successfully!' AS Message;
SELECT 'Tables, views, procedures, functions, and triggers have been set up.' AS Info;
