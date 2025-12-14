Hierarchical N-Body Gravity Simulation

Overview
This project simulates a multi-body gravitational system with a hierarchical mass structure 
(Central Black Hole -> Stars -> Planets -> Moons). Unlike standard N-body simulations, 
this model introduces a non-inertial reference frame by applying a global centrifugal force term to simulate a rotating universe.

Physics Implementation
Newtonian Gravity: Calculates the inverse-square force between all particle pairs (F = G * m1 * m2 / r^2).
Hierarchical Initialization: Procedurally generates stable orbital velocities using tangent vectors derived 
from the cross product of the radial vector and the orbital plane normal.
Non-Inertial Frame: Applies a centrifugal force vector Fc = m * (Omega x (Omega x r)) to test stability in rotating coordinate systems.

Technologies
C++: Core simulation logic.
OpenGL (GLFW): Real-time rendering of particle trajectories.
GLM: Vector mathematics and matrix transformations.
