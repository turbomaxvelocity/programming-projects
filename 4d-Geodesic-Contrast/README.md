# 4D Geodesic Geometry Contrast Model

### Project Goal
This model provides a definitive geometric visualization that resolves the paradox of geodesics: how a **straight line** in a higher-dimensional space is perceived as a **curved path (a helix/sine wave)** when projected onto our lower-dimensional space.

### Visualization Mechanics (Dual Panel)

| Panel | The View | Geometric Interpretation |
| :--- | :--- | :--- |
| **Left Panel** | **The Perceived 3D Wave** | Shows the helix (yellow) resulting from two perpendicular sine waves (Red X-axis, Green Y-axis) propagating along Z. This path, familiar from **Electromagnetic Theory**, demonstrates the curvature we observe. |
| **Right Panel** | **The True 4D Geodesic** | Shows a **perfectly straight line** synchronized with the wave. This represents the same path as viewed in a coordinate system where the fourth dimension is "unrolled." The straight line is the shortest path (the true geodesic) in the higher dimension. |

### Conclusion: Curvature as Projection
The model argues that the observed curvature (the sine wave) is purely a **geometric projection artifact**. The helix is fundamentally a straight line that has been parametrically coiled by the projection into the 3D subspace. This is a core concept in General Relativity and theories of Extra Dimensions.

### Tech Stack
* **C++:** Implements advanced dual-viewport rendering logic for synchronized geometric comparison.
* **OpenGL (GLFW/GLU):** Uses `glViewport` to simultaneously render two distinct camera views of the same parameterized data, creating a powerful, single-screen contrast visualization.
