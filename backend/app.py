from __future__ import annotations

from collections import deque
from datetime import date
import heapq
from typing import Dict, List, Tuple

from flask import Flask, jsonify, request

app = Flask(__name__)

# ---------------------------
# In-memory data structures
# ---------------------------
users: Dict[str, Dict] = {}
farmers = set()
buyers = set()
storage_owners = set()
transport_providers = set()

crops: Dict[int, Dict] = {}
storage_map: Dict[int, Dict] = {}
transport_map: Dict[int, Dict] = {}
orders = deque()
order_id_counter = 1

# Graph as adjacency list: city -> list[(neighbor, distance_km)]
city_graph: Dict[str, List[Tuple[str, int]]] = {}


# ---------------------------
# Helpers
# ---------------------------
def current_date() -> str:
    return date.today().isoformat()


def init_graph() -> None:
    city_graph.clear()
    edges = [
        ("Lahore", "Islamabad", 375),
        ("Lahore", "Multan", 342),
        ("Lahore", "Sialkot", 110),
        ("Lahore", "Faisalabad", 180),
        ("Islamabad", "Peshawar", 450),
        ("Islamabad", "Multan", 680),
        ("Multan", "Faisalabad", 220),
        ("Multan", "Karachi", 980),
        ("Faisalabad", "Sialkot", 280),
        ("Faisalabad", "Karachi", 1050),
        ("Peshawar", "Quetta", 1200),
        ("Karachi", "Quetta", 1000),
    ]
    for a, b, d in edges:
        city_graph.setdefault(a, []).append((b, d))
        city_graph.setdefault(b, []).append((a, d))


def dijkstra(start: str, end: str) -> Dict:
    if start not in city_graph or end not in city_graph:
        return {"path": [], "distance": 0, "estimatedTime": "0h 0m"}

    dist = {city: float("inf") for city in city_graph}
    prev: Dict[str, str | None] = {city: None for city in city_graph}
    dist[start] = 0
    pq: List[Tuple[int, str]] = [(0, start)]

    while pq:
        cur_dist, city = heapq.heappop(pq)
        if cur_dist > dist[city]:
            continue
        if city == end:
            break
        for neighbor, weight in city_graph.get(city, []):
            nd = cur_dist + weight
            if nd < dist[neighbor]:
                dist[neighbor] = nd
                prev[neighbor] = city
                heapq.heappush(pq, (nd, neighbor))

    if dist[end] == float("inf"):
        return {"path": [], "distance": 0, "estimatedTime": "0h 0m"}

    path = []
    cur = end
    while cur is not None:
        path.append(cur)
        cur = prev[cur]
    path.reverse()

    total = dist[end]
    hours = total // 80
    minutes = (total % 80) * 60 // 80
    return {
        "path": path,
        "distance": total,
        "estimatedTime": f"{hours}h {minutes}m",
    }


def allocate_storage(quantity: int) -> Dict:
    heap: List[Tuple[float, int]] = []  # max-heap via negative score
    for sid, s in storage_map.items():
        if s["availableCapacity"] < quantity:
            continue
        temp_penalty = abs(s["temperature"] - 4.0) * 10
        score = s["availableCapacity"] - temp_penalty
        heapq.heappush(heap, (-score, sid))
    if not heap:
        return {"id": -1}
    _, sid = heapq.heappop(heap)
    storage_map[sid]["availableCapacity"] -= quantity
    s = storage_map[sid]
    return {
        "id": sid,
        "name": s["name"],
        "temperature": s["temperature"],
        "availableCapacity": s["availableCapacity"],
    }


def init_sample_data() -> None:
    users.clear()
    users.update({
        "admin": {"username": "admin", "password": "admin123", "role": "admin", "email": "admin@agriconnect.pk"},
        "farmer1": {"username": "farmer1", "password": "pass123", "role": "farmer", "email": "farmer1@gmail.com"},
        "farmer2": {"username": "farmer2", "password": "pass123", "role": "farmer", "email": "farmer2@gmail.com"},
        "buyer1": {"username": "buyer1", "password": "pass123", "role": "buyer", "email": "buyer1@gmail.com"},
        "storage_owner1": {"username": "storage_owner1", "password": "pass123", "role": "storage_owner", "email": "storage@gmail.com"},
        "transport_provider1": {"username": "transport_provider1", "password": "pass123", "role": "transport_provider", "email": "transport@gmail.com"},
    })

    farmers.clear(); farmers.update(["farmer1", "farmer2"])
    buyers.clear(); buyers.update(["buyer1"])
    storage_owners.clear(); storage_owners.update(["storage_owner1"])
    transport_providers.clear(); transport_providers.update(["transport_provider1"])

    crops.clear()
    crops.update({
        1: {"id": 1, "type": "Wheat", "quantity": 5000, "quality": "A", "price": 85.0, "farmer": "farmer1", "harvestDate": current_date()},
        2: {"id": 2, "type": "Rice", "quantity": 3000, "quality": "B", "price": 120.0, "farmer": "farmer1", "harvestDate": current_date()},
        3: {"id": 3, "type": "Potato", "quantity": 2000, "quality": "A", "price": 60.0, "farmer": "farmer2", "harvestDate": current_date()},
        4: {"id": 4, "type": "Tomato", "quantity": 1500, "quality": "A", "price": 90.0, "farmer": "farmer2", "harvestDate": current_date()},
    })

    storage_map.clear()
    storage_map.update({
        1: {"id": 1, "name": "Lahore Cold Storage", "location": "Lahore", "totalCapacity": 10000, "availableCapacity": 8000, "temperature": 4.0},
        2: {"id": 2, "name": "Karachi Agri Hub", "location": "Karachi", "totalCapacity": 15000, "availableCapacity": 12000, "temperature": 5.0},
        3: {"id": 3, "name": "Multan Storage Facility", "location": "Multan", "totalCapacity": 8000, "availableCapacity": 7000, "temperature": 3.5},
        4: {"id": 4, "name": "Islamabad Cold Chain", "location": "Islamabad", "totalCapacity": 12000, "availableCapacity": 10000, "temperature": 4.5},
    })

    transport_map.clear()
    transport_map.update({
        1: {"id": 1, "vehicleType": "Refrigerated Truck", "capacity": 5000, "route": "Lahore-Karachi", "pricePerKm": 15.0, "status": "Available"},
        2: {"id": 2, "vehicleType": "Cargo Van", "capacity": 2000, "route": "Lahore-Islamabad", "pricePerKm": 12.0, "status": "Available"},
        3: {"id": 3, "vehicleType": "Large Truck", "capacity": 10000, "route": "Multan-Karachi", "pricePerKm": 18.0, "status": "Available"},
    })

    orders.clear()
    global order_id_counter
    order_id_counter = 1

    init_graph()


# ---------------------------
# CORS
# ---------------------------
@app.after_request
def add_cors(resp):
    resp.headers["Access-Control-Allow-Origin"] = "*"
    resp.headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, OPTIONS"
    resp.headers["Access-Control-Allow-Headers"] = "Content-Type"
    return resp


@app.route("/api/<path:_any>", methods=["OPTIONS"])
def cors_preflight(_any):
    return ("", 204)


# ---------------------------
# User endpoints
# ---------------------------
@app.post("/api/register")
def register():
    data = request.get_json(force=True)
    username = (data.get("username") or "").strip()
    password = (data.get("password") or "").strip()
    role = (data.get("role") or "").strip()
    email = (data.get("email") or "").strip()

    if not username or not password or not role:
        return jsonify({"success": False, "message": "Missing fields"}), 400
    if username in users:
        return jsonify({"success": False, "message": "Username already exists"}), 400

    users[username] = {"username": username, "password": password, "role": role, "email": email}
    if role == "farmer":
        farmers.add(username)
    elif role == "buyer":
        buyers.add(username)
    elif role == "storage_owner":
        storage_owners.add(username)
    elif role == "transport_provider":
        transport_providers.add(username)

    return jsonify({"success": True, "message": "Registration successful"})


@app.post("/api/login")
def login():
    data = request.get_json(force=True)
    username = data.get("username")
    password = data.get("password")
    user = users.get(username)
    if user and user["password"] == password:
        return jsonify({"success": True, "user": {"username": username, "role": user["role"], "email": user.get("email", "")}})
    return jsonify({"success": False, "message": "Invalid credentials"}), 401


@app.get("/api/users/by-role")
def users_by_role():
    role = request.args.get("role", "")
    role_set = {
        "farmer": farmers,
        "buyer": buyers,
        "storage_owner": storage_owners,
        "transport_provider": transport_providers,
    }.get(role)
    if role_set is None:
        return jsonify({"success": True, "users": []})
    return jsonify({"success": True, "users": [{"username": u} for u in sorted(role_set)]})


# ---------------------------
# Crop endpoints
# ---------------------------
@app.get("/api/crops")
def get_crops():
    return jsonify({"success": True, "crops": list(crops.values())})


@app.post("/api/crops")
def add_crop():
    data = request.get_json(force=True)
    new_id = max(crops.keys(), default=0) + 1
    crop = {
        "id": new_id,
        "type": data.get("type", ""),
        "quantity": int(data.get("quantity", 0)),
        "quality": data.get("quality", ""),
        "price": float(data.get("price", 0)),
        "farmer": data.get("farmer", ""),
        "harvestDate": current_date(),
    }
    crops[new_id] = crop
    return jsonify({"success": True, "message": "Crop added", "cropId": new_id})


# ---------------------------
# Storage endpoints
# ---------------------------
@app.get("/api/storage")
def get_storage():
    return jsonify({"success": True, "storage": list(storage_map.values())})


@app.post("/api/storage/allocate")
def storage_allocate():
    data = request.get_json(force=True)
    qty = int(data.get("quantity", 0))
    best = allocate_storage(qty)
    if best.get("id", -1) == -1:
        return jsonify({"success": False, "message": "No suitable storage"}), 400
    return jsonify({"success": True, "storage": best})


# ---------------------------
# Transport endpoints
# ---------------------------
@app.get("/api/transport")
def get_transport():
    return jsonify({"success": True, "transport": list(transport_map.values())})


@app.post("/api/transport")
def add_transport():
    data = request.get_json(force=True)
    new_id = max(transport_map.keys(), default=0) + 1
    t = {
        "id": new_id,
        "vehicleType": data.get("vehicleType", ""),
        "capacity": int(data.get("capacity", 0)),
        "route": data.get("route", ""),
        "pricePerKm": float(data.get("pricePerKm", 0)),
        "status": "Available",
    }
    transport_map[new_id] = t
    return jsonify({"success": True, "message": "Transport added", "transportId": new_id})


# ---------------------------
# Routing endpoint (Dijkstra)
# ---------------------------
@app.get("/api/route/shortest")
def shortest_route():
    start = request.args.get("start", "")
    end = request.args.get("end", "")
    if not start or not end:
        return jsonify({"success": False, "message": "Missing start or end city"}), 400
    result = dijkstra(start, end)
    if not result["path"]:
        return jsonify({"success": False, "message": "No route found"}), 404
    path_str = " -> ".join(result["path"])
    return jsonify({
        "success": True,
        "route": {
            "startCity": start,
            "endCity": end,
            "distance": result["distance"],
            "estimatedTime": result["estimatedTime"],
            "path": path_str,
            "algorithm": "Dijkstra O((V+E)logV)",
        },
    })


# ---------------------------
# Orders (Queue)
# ---------------------------
@app.post("/api/orders")
def add_order():
    global order_id_counter
    data = request.get_json(force=True)
    order = {
        "orderId": order_id_counter,
        "username": data.get("username", ""),
        "cropId": int(data.get("cropId", 0)),
        "quantity": int(data.get("quantity", 0)),
        "status": "pending",
        "timestamp": current_date(),
    }
    order_id_counter += 1
    orders.append(order)
    return jsonify({"success": True, "message": "Order queued", "orderId": order["orderId"]})


@app.get("/api/orders/queue")
def queue_status():
    return jsonify({"success": True, "queueSize": len(orders), "pendingOrders": len(orders)})


# ---------------------------
# Stats & info
# ---------------------------
@app.get("/api/stats")
def stats():
    total_storage = sum(s["totalCapacity"] for s in storage_map.values())
    total_available = sum(s["availableCapacity"] for s in storage_map.values())
    total_crops_qty = sum(c["quantity"] for c in crops.values())
    return jsonify({
        "success": True,
        "stats": {
            "totalUsers": len(users),
            "farmers": len(farmers),
            "buyers": len(buyers),
            "totalCrops": len(crops),
            "totalCropQuantity": total_crops_qty,
            "storagecenters": len(storage_map),
            "totalStorageCapacity": total_storage,
            "availableStorage": total_available,
            "transportServices": len(transport_map),
            "pendingOrders": len(orders),
            "dataStructures": "Dict,Set,Heap (Priority Queue),Graph,Deque",
            "algorithms": "Dijkstra,GreedyAllocation,FIFO",
        },
    })


@app.get("/api/info/data-structures")
def info_ds():
    return jsonify({
        "success": True,
        "dataStructures": [
            {"name": "Dict", "usage": "Users, Crops, Storage - O(1) average lookup"},
            {"name": "Set", "usage": "Role filtering - O(1) membership"},
            {"name": "Heap (Priority Queue)", "usage": "Storage allocation - O(log n)"},
            {"name": "Graph", "usage": "City network - Dijkstra"},
            {"name": "Deque (Queue)", "usage": "Order processing FIFO - O(1)"},
        ],
    })


@app.get("/api/info/algorithms")
def info_algo():
    return jsonify({
        "success": True,
        "algorithms": [
            {"name": "Dijkstra", "complexity": "O((V+E)logV)", "usage": "Shortest path"},
            {"name": "Priority Queue Greedy", "complexity": "O(nlogn)", "usage": "Optimal storage"},
            {"name": "FIFO Queue", "complexity": "O(1)", "usage": "Order processing"},
        ],
    })


# Initialize data immediately on import so Flask debug/reload has data ready.
init_sample_data()


if __name__ == "__main__":
    print("Flask API server starting on http://localhost:5000")
    app.run(host="0.0.0.0", port=5000)
