Brane Cosmology & Bulk Fluctuation Visualizer

Note: This simulation was developed on macOS (Apple Silicon).

Theoretical Purpose
This program is a conceptual visualization of **Brane Cosmology** (specifically, Randall-Sundrum-like models). In these theories, our 3-dimensional universe is a "Brane" embedded within a higher-dimensional space called the "Bulk."

Simulation Dynamics & Physics Demonstrated
The core purpose is to visualize the topological stability and slow fluctuations of the brane within the bulk.

The Brane:Rendered as a 2D lattice, representing a simplified slice of our 3-brane universe.
The Bulk (Extra Dimension):The vertical (Z-axis) displacement of the lattice points visualizes the projection of the extra dimension.
Radion Field Dynamics: The slow, controlled undulation of the grid simulates hypothetical **scalar field fluctuations (Radions). These fields are theorized to stabilize the size of the extra dimension, with the wave patterns representing low-energy gravitational ripples propagating through the bulk.
Controlled Visualization: The low frequency ensures a clear, academic visualization of geometric deformation, emphasizing topology preservation rather than rapid motion.

### Tech Stack
* **C++:** Core logic for wave calculation and vertex displacement.
* **OpenGL (GLFW):** Real-time, wireframe rendering for clean visualization.
* **Wave Dynamics:** Implements interference patterns using coupled sine-wave functions: $z = A \cdot \sin(r - \omega t) \cdot \cos(kx)$ (where $\omega$ is angular frequency and $r$ is radial distance).
