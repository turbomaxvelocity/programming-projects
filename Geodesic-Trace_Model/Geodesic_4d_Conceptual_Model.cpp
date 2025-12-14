#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

// A simple structure to represent a point in 4D space (Euclidean)
struct Vector4D {
    // w is the 4th spatial dimension (conceptual time/angle)
    double x, y, z, w;
};

class Particle {
private:
    Vector4D position;
    Vector4D velocity; // Constant velocity in 4D (representing the Geodesic)

public:
    Particle(double x, double y, double z, double w,
             double vx, double vy, double vz, double vw) {
        position = {x, y, z, w};
        velocity = {vx, vy, vz, vw};
    }

    // Update position: simple linear movement in 4D (the Geodesic)
    void update() {
        position.x += velocity.x;
        position.y += velocity.y;
        position.z += velocity.z;
        position.w += velocity.w;
    }

    // THE PROJECTION FUNCTION:
    // Simulates observing the 4D path from a 3D reference frame.
    // The straight path in 4D (position.w) is perceived as an oscillation (wave)
    // when projected onto the 3D X-axis.
    double getProjectedX() {
        // We use a cosine function to simulate the circular component
        // that appears when a straight helix/torus is viewed from a 3D slice.
        return std::cos(position.w * 0.5);
    }

    double getW() {
        return position.w; // The parameter describing distance along the 4D line
    }
};

int main() {
    // Particle is moving in a straight line through the 4th spatial dimension (w)
    Particle p(0, 0, 0, 0,   0, 0, 0, 0.5); // (vx,vy,vz are zero)

    std::cout << "--- 4D GEODESIC CONCEPTUAL MODEL (C++) ---" << std::endl;
    std::cout << "The particle moves in a 4D straight line (Geodesic)." << std::endl;

    // *** FIX IS HERE: The extra '<< std::cout' was removed. ***
    std::cout << "We observe the path's projection onto the X-axis (3D View)." << std::endl;
    // *********************************************************

    std::cout << std::endl;

    std::cout << "Path Parameter (W)\t| 3D Projection (X-Amplitude) | Visual Waveform" << std::endl;
    std::cout << "-------------------------------------------------------------------" << std::endl;

    // Run the simulation
    for (int i = 0; i < 40; i++) {
        p.update();

        double amp = p.getProjectedX();
        double time = p.getW();

        // Visualization logic
        std::cout << std::fixed << std::setprecision(1) << std::setw(16) << time << "\t| "
                  << std::setw(19) << amp << "     | ";

        // Draw the wave based on the projection
        int pos = (amp + 1.0) * 15; // Map -1..1 to 0..30 for display
        for (int k = 0; k < pos; k++) std::cout << " ";
        std::cout << "*" << std::endl;
    }

    std::cout << "\nCONCLUSION: The observed oscillation is a projection artifact." << std::endl;
    std::cout << "The particle's motion in the 4D bulk remained linear." << std::endl;

    return 0;
}
