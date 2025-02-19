Real-Time Sensor Dashboard in C
This project is a real-time sensor dashboard implemented in C using pthread for multithreading and ncurses for a terminal-based graphical interface. It simulates real-time updates for temperature, humidity, and pressure sensors, displaying the data in both gauges and graphs.

Features
Real-Time Updates: Simulated sensor data is updated every second.

Graphical Display: Uses ncurses to display gauges and graphs for temperature, humidity, and pressure.

Multithreading: Each sensor (temperature, humidity, pressure) runs in its own thread for concurrent updates.

Interactive: Press q to exit the dashboard.

Code Overview
The program consists of:

Sensor Simulation: Threads simulate sensor data for temperature, humidity, and pressure.

Data Visualization: Gauges and graphs are drawn using ncurses to display the data in real-time.

Thread Synchronization: A mutex lock ensures thread-safe access to shared data.

How to Run
