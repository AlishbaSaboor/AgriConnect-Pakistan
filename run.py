#!/usr/bin/env python
"""
AgriConnect Pakistan - Full Stack Runner
Runs both Backend (Flask API) and Frontend (HTTP Server)
Usage: python run.py
"""

import subprocess
import sys
import time
import webbrowser
from pathlib import Path
import threading

def run_backend(python_exe, app_path):
    """Run Flask backend server"""
    try:
        subprocess.run([python_exe, str(app_path)])
    except KeyboardInterrupt:
        pass

def run_frontend(python_exe, frontend_dir):
    """Run HTTP server for frontend"""
    try:
        subprocess.run([python_exe, "-m", "http.server", "8000"], cwd=str(frontend_dir))
    except KeyboardInterrupt:
        pass

def main():
    project_root = Path(__file__).parent
    app_path = project_root / "backend" / "app.py"
    frontend_dir = project_root / "frontend"
    
    # Use virtual environment Python if available
    venv_python = project_root / ".venv" / "Scripts" / "python.exe"
    python_exe = str(venv_python) if venv_python.exists() else sys.executable
    
    print("=" * 70)
    print("🌾 AgriConnect Pakistan - Full Stack Application")
    print("=" * 70)
    print(f"Using Python: {python_exe}")
    print()
    print("Starting servers:")
    print("  📡 Backend API:  http://localhost:5000")
    print("  🌐 Frontend UI:  http://localhost:8000")
    print()
    print("Press CTRL+C to stop both servers")
    print("=" * 70)
    print()
    
    try:
        # Start backend in a separate thread
        backend_thread = threading.Thread(target=run_backend, args=(python_exe, app_path), daemon=True)
        backend_thread.start()
        
        # Give backend time to start
        time.sleep(2)
        
        print("✅ Backend started on http://localhost:5000")
        print("✅ Starting frontend on http://localhost:8000...")
        print()
        
        # Open browser after a short delay
        def open_browser():
            time.sleep(1)
            webbrowser.open("http://localhost:8000")
        
        browser_thread = threading.Thread(target=open_browser, daemon=True)
        browser_thread.start()
        
        # Run frontend in main thread (so Ctrl+C works)
        run_frontend(python_exe, frontend_dir)
        
    except KeyboardInterrupt:
        print("\n\n🛑 Stopping servers...")
        print("✅ Both servers stopped")
    except Exception as e:
        print(f"\n❌ Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
