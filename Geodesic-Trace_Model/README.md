# Geodesic Projection: 4D Conceptual Console Model

### Theoretical Purpose
This project is a conceptual model demonstrating how a **geodesic (a straight line)** in a higher-dimensional space (N-space) can appear as a curved or oscillating path when viewed from a lower-dimensional subspace (N-1 space).

This is a core concept used in General Relativity and Cosmology to explain why "straight" paths in spacetime (geodesics) are observed as "curved" paths in 3D space (e.g., orbits).

### Simulation Mechanics
* **4D Geodesic:** The `Particle` class models linear, unaccelerated motion in a 4D Euclidean space (x, y, z, w).
* **The Projection:** The `getProjectedX()` function uses a periodic function (`std::cos`) on the 4th dimension variable (`position.w`). This simulates the geometric effect of slicing a higher-dimensional path (like a helical line on a 4D torus) down to a 3D axis.
* **Visualization:** The console output uses amplitude mapping to display the observed "wave," proving that the apparent curvature is purely an artifact of the projection and frame of reference.

### Project Value
This model shows theoretical depth by emphasizing **coordinate systems** and **geometric projection** over complex physics simulation, proving the ability to conceptualize and model highly abstract physical principles using minimal code.

### Tech Stack
* **C++:** Standard library implementation.
* **Console Visualization:** Use of `<iomanip>` for simple amplitude plotting, making the concept easily digestible without reliance on graphics libraries.
