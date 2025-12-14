#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <GLUT/glut.h>

// Simulation Parameters
const float RADIUS = 4.0f; // Radius of the helix (Amplitude of the sine wave)
const float PITCH = 2.0f;  // Spacing along the Z-axis (Rate of propagation)
const int SEGMENTS = 300;
float currentTime = 0.0f;

// --- Helper function to draw coordinate axes for context ---
void drawAxes() {
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    // X-axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-10.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    // Y-axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    // Z-axis (Blue - Direction of Propagation)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -10.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();
}

// --- Left Panel: 3D Sine Wave Projection (The Helix/EM Wave) ---
void drawSineWaveView() {
    // Draw the helix (the combined wave path)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow path
    glLineWidth(4.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < SEGMENTS; ++i) {
        float t = (float)i / SEGMENTS * 8.0f * M_PI;

        float x = RADIUS * cos(t + currentTime * 0.5f); // X-component (Sine Wave 1)
        float y = RADIUS * sin(t + currentTime * 0.5f); // Y-component (Sine Wave 2, 90 deg phase shift)
        float z = t * PITCH - 15.0f;                    // Z-component (Linear propagation)

        glVertex3f(x, y, z);
    }
    glEnd();

    // Draw X-axis projection (The first sine wave component)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < SEGMENTS; ++i) {
        float t = (float)i / SEGMENTS * 8.0f * M_PI;
        float x = RADIUS * cos(t + currentTime * 0.5f);
        float z = t * PITCH - 15.0f;
        glVertex3f(x, 0.0f, z); // Projected onto the XZ plane
    }
    glEnd();

    // Draw Y-axis projection (The second sine wave component)
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < SEGMENTS; ++i) {
        float t = (float)i / SEGMENTS * 8.0f * M_PI;
        float y = RADIUS * sin(t + currentTime * 0.5f);
        float z = t * PITCH - 15.0f;
        glVertex3f(0.0f, y, z); // Projected onto the YZ plane
    }
    glEnd();
}

// --- Right Panel: 4D Geodesic View (The Straight Line) ---
void drawGeodesicView() {
    // This view conceptually represents the unrolled 4D space.

    // Draw the straight line (Geodesic)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow path
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex3f(-15.0f, 0.0f, 0.0f); // Start point
    glVertex3f(15.0f, 0.0f, 0.0f);  // End point
    glEnd();
    glLineWidth(1.0f);

    // Add a single point moving along the line, synchronized with the wave.
    float t_sync = fmod(currentTime * 0.5f, 2.0f * M_PI);
    float line_pos = 15.0f * (t_sync / (2.0f * M_PI)) * 2.0f - 15.0f;

    glColor3f(0.8f, 0.8f, 0.8f); // White point
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex3f(line_pos, 0.0f, 0.0f);
    glEnd();
}

// Camera and Viewport setup functions (remain the same)
void setupCamera(float width, float height, float left_view) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = width / height;
    // NOTE: This uses GLU. We handle the linking in CMake.
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -30.0f);
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

    // Specific rotation for the view
    if (left_view > 0.5f) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    } else {
        glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    }
}

void renderScene(GLFWwindow* window) {
    currentTime += 0.03f;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- LEFT VIEWPORT: The 3D Sine Wave ---
    glViewport(0, 0, width / 2, height);
    setupCamera((float)width / 2, (float)height, 1.0f);
    drawAxes();
    drawSineWaveView();

    // --- RIGHT VIEWPORT: The 4D Geodesic (Straight Line) ---
    glViewport(width / 2, 0, width / 2, height);
    setupCamera((float)width / 2, (float)height, 0.0f);
    drawAxes();
    drawGeodesicView();

    // Draw the separator line
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f((float)width / 2, 0);
    glVertex2f((float)width / 2, (float)height);
    glEnd();
    glEnable(GL_DEPTH_TEST);
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1200, 600, "4D Geodesic Geometry Contrast", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);

    // NOTE: This is the function that handles window resizing/camera setup
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    setupCamera((float)width, (float)height, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        renderScene(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
