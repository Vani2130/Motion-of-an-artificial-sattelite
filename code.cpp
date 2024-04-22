#include <iostream>
#include <fstream>
#include <cmath>

const double G = 6.67430e-11; // Gravitational constant (m^3 kg^-1 s^-2)
const double M_EARTH = 5.972e24; // Mass of the Earth (kg)
const double R_EARTH = 6.371e6; // Radius of the Earth (m)

class Vector {
    double x, y, z;

    Vector operator+(const Vector& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector operator-(const Vector& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector operator*(double scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    double magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

Vector gravitationalForce(const Vector& position) {
    double distance = position.magnitude();
    double forceMagnitude = (G * M_EARTH) / (distance * distance);
    return position * (-forceMagnitude / distance);
}

void integrate(Vector& position, Vector& velocity, double dt) {
    // Calculate acceleration
    Vector acceleration = gravitationalForce(position);

    // Update position and velocity using Verlet integration
    Vector new_position = position + velocity * dt + acceleration * (dt * dt / 2);
    Vector new_acceleration = gravitationalForce(new_position);
    Vector new_velocity = velocity + (acceleration + new_acceleration) * (dt / 2);

   // Set new position and velocity
    position = new_position;
    velocity = new_velocity;
}

int main() {
    // Initial conditions for an elliptical orbit
    Vector position = {R_EARTH + 700e3, 0, 0}; // Starting position (altitude of 700 km above Earth)
    Vector velocity = {0, 7500, 0}; // Initial velocity in m/s (typical speed for an elliptical orbit)
    
    // Time step and number of simulation steps
    double dt = 10; // Time step in seconds
    int steps = 10000; // Number of time steps to simulate
    
    // Open a file to save trajectory data
    std::ofstream trajectoryFile("trajectory_data.txt");
    
    // Simulate satellite motion
    for (int i = 0; i < steps; i++) {
        // Save current position to file
        trajectoryFile << position.x << " " << position.y << " " << position.z << "\n";

        // Update satellite position and velocity
        integrate(position, velocity, dt);
    }
    
    // Close the file after simulation
    trajectoryFile.close();

    return 0;
}


