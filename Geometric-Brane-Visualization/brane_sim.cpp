#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

// Simulation Parameters
const int GRID_SIZE = 40;
const float SPACING = 0.5f;
const float AMPLITUDE = 1.2f;
const float FREQUENCY = 0.5f;    // <--- REDUCED WAVE SPEED (from 2.0f)
float currentTime = 0.0f;

// Camera
float cameraAngleX = 30.0f;
float cameraAngleY = -45.0f;
float zoom = -35.0f;

struct Vertex {
    float x, y, z;
};

std::vector<Vertex> brane;

void initBrane() {
    brane.clear();
    float offset = (GRID_SIZE * SPACING) / 2.0f;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            Vertex v;
            v.x = (i * SPACING) - offset;
            v.y = (j * SPACING) - offset;
            v.z = 0.0f;
            brane.push_back(v);
        }
    }
}

// Physics: Calculate the shape of the brane at current time
void updateBranePhysics() {
    currentTime += 0.005f; // <--- REDUCED TIME STEP (from 0.02f)

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            int index = i * GRID_SIZE + j;
            float x = brane[index].x;
            float y = brane[index].y;

            float dist = sqrt(x*x + y*y);
            // The wave function remains complex but moves slower
            brane[index].z = sin(dist * 0.4f - currentTime * FREQUENCY) * cos(x * 0.2f) * AMPLITUDE;
        }
    }
}

void drawGrid() {
    glColor3f(0.0f, 1.0f, 0.8f);

    // Draw horizontal lines
    for (int i = 0; i < GRID_SIZE; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < GRID_SIZE; j++) {
            int index = i * GRID_SIZE + j;
            glVertex3f(brane[index].x, brane[index].z, brane[index].y);
        }
        glEnd();
    }

    // Draw vertical lines
    for (int j = 0; j < GRID_SIZE; j++) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < GRID_SIZE; i++) {
            int index = i * GRID_SIZE + j;
            glVertex3f(brane[index].x, brane[index].z, brane[index].y);
        }
        glEnd();
    }
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1200, 800, "Brane Cosmology Visualizer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);

    // Camera Lens Setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = 1200.0f / 800.0f;
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float top = nearPlane * tan(fov * 0.5f * 3.14159f / 180.0f);
    float right = top * aspect;
    glFrustum(-right, right, -top, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);

    initBrane();

    while (!glfwWindowShouldClose(window)) {
        updateBranePhysics();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glTranslatef(0.0f, 0.0f, zoom);
        glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
        glRotatef(currentTime * 1.5f, 0.0f, 1.0f, 0.0f); // <--- REDUCED AUTO-ROTATION (from 10.0f)

        drawGrid();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
