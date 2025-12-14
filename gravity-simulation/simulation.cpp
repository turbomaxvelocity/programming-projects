#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <iostream>

enum ParticleType {
    TYPE_CENTRAL_MASS,
    TYPE_STAR,
    TYPE_PLANET,
    TYPE_MOON
};

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    ParticleType type;
};

// Universe parameters
const float UNIVERSE_ROTATION_SPEED = 0.10f;  // Universe is SPINNING
const float G = 0.2f;
const float DT = 0.008f;

// Mass scales
const float CENTRAL_MASS = 5000000.0f;  // MASSIVE BLACK HOLE
const float STAR_MASS = 150.0f;
const float PLANET_MASS = 15.0f;
const float MOON_MASS = 1.5f;

std::vector<Particle> particles;

// Camera parameters
float cameraAngleX = 0.0f;
float cameraAngleY = 20.0f;
float cameraDistance = 50.0f;

// Mouse state
double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool mousePressed = false;

// Helper to create orbital velocity
glm::vec3 getOrbitalVelocity(glm::vec3 position, glm::vec3 centerPos, float centerMass, float factor = 1.0f) {
    glm::vec3 toCenter = centerPos - position;
    float radius = glm::length(toCenter);

    if (radius < 0.1f) return glm::vec3(0.0f);

    glm::vec3 radial = glm::normalize(toCenter);
    glm::vec3 tangent = glm::cross(radial, glm::vec3(0.0f, 1.0f, 0.0f));

    if (glm::length(tangent) < 0.1f) {
        tangent = glm::cross(radial, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    tangent = glm::normalize(tangent);

    float orbitalSpeed = sqrt(G * centerMass / radius) * factor;
    return tangent * orbitalSpeed;
}

void initParticles() {
    particles.clear();

    // 1. CENTRAL SUPERMASSIVE OBJECT at origin
    Particle central;
    central.position = glm::vec3(0.0f, 0.0f, 0.0f);
    central.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    central.mass = CENTRAL_MASS;
    central.type = TYPE_CENTRAL_MASS;
    particles.push_back(central);

    // 2. Create STARS orbiting the central mass
    int numStars = 5;
    for (int i = 0; i < numStars; i++) {
        Particle star;

        // Place stars at various distances from center
        float radius = 15.0f + (rand() / (float)RAND_MAX) * 20.0f;
        float theta = (i / (float)numStars) * 2.0f * M_PI;

        star.position.x = radius * cos(theta);
        star.position.y = (rand() / (float)RAND_MAX - 0.5f) * 5.0f;
        star.position.z = radius * sin(theta);

        star.velocity = getOrbitalVelocity(star.position, central.position, CENTRAL_MASS, 0.9f);
        star.mass = STAR_MASS;
        star.type = TYPE_STAR;

        size_t starIndex = particles.size();
        particles.push_back(star);

        // 3. Create PLANETS orbiting each star
        int numPlanets = 3 + rand() % 3;
        for (int j = 0; j < numPlanets; j++) {
            Particle planet;

            float planetOrbitRadius = 2.0f + j * 1.5f;
            float planetTheta = (rand() / (float)RAND_MAX) * 2.0f * M_PI;

            glm::vec3 offsetFromStar;
            offsetFromStar.x = planetOrbitRadius * cos(planetTheta);
            offsetFromStar.y = (rand() / (float)RAND_MAX - 0.5f) * 0.5f;
            offsetFromStar.z = planetOrbitRadius * sin(planetTheta);

            planet.position = star.position + offsetFromStar;
            planet.velocity = star.velocity + getOrbitalVelocity(planet.position, star.position, STAR_MASS, 1.0f);
            planet.mass = PLANET_MASS;
            planet.type = TYPE_PLANET;

            size_t planetIndex = particles.size();
            particles.push_back(planet);

            // 4. Create MOONS orbiting each planet
            int numMoons = 1 + rand() % 2;
            for (int k = 0; k < numMoons; k++) {
                Particle moon;

                float moonOrbitRadius = 0.5f + k * 0.3f;
                float moonTheta = (rand() / (float)RAND_MAX) * 2.0f * M_PI;

                glm::vec3 offsetFromPlanet;
                offsetFromPlanet.x = moonOrbitRadius * cos(moonTheta);
                offsetFromPlanet.y = (rand() / (float)RAND_MAX - 0.5f) * 0.2f;
                offsetFromPlanet.z = moonOrbitRadius * sin(moonTheta);

                moon.position = planet.position + offsetFromPlanet;
                moon.velocity = planet.velocity + getOrbitalVelocity(moon.position, planet.position, PLANET_MASS, 1.0f);
                moon.mass = MOON_MASS;
                moon.type = TYPE_MOON;

                particles.push_back(moon);
            }
        }
    }

    std::cout << "Created " << particles.size() << " objects:" << std::endl;
    std::cout << "- 1 central mass" << std::endl;
    std::cout << "- " << numStars << " stars" << std::endl;
    std::cout << "- Multiple planets and moons" << std::endl;
}

glm::vec3 calculateGravity(const Particle& p1, const Particle& p2) {
    glm::vec3 dir = p2.position - p1.position;
    float dist = glm::length(dir);

    if (dist < 0.1f) return glm::vec3(0.0f);

    float force = G * p1.mass * p2.mass / (dist * dist);
    return glm::normalize(dir) * force / p1.mass;
}

glm::vec3 calculateCentrifugal(const Particle& p) {
    // UNIVERSE ROTATION - centrifugal force pushes outward
    float distFromCenter = glm::length(p.position);

    if (distFromCenter < 0.1f) return glm::vec3(0.0f);

    glm::vec3 radialDir = glm::normalize(p.position);
    float centrifugalMag = UNIVERSE_ROTATION_SPEED * UNIVERSE_ROTATION_SPEED * distFromCenter;

    return radialDir * centrifugalMag;
}

void updatePhysics() {
    for (size_t i = 0; i < particles.size(); i++) {
        // Don't move central mass
        if (particles[i].type == TYPE_CENTRAL_MASS) continue;

        glm::vec3 totalForce(0.0f);

        // Gravity from ALL other particles
        for (size_t j = 0; j < particles.size(); j++) {
            if (i != j) {
                totalForce += calculateGravity(particles[i], particles[j]);
            }
        }

        // UNIVERSE ROTATION - centrifugal force
        totalForce += calculateCentrifugal(particles[i]);

        particles[i].velocity += totalForce * DT;
        particles[i].position += particles[i].velocity * DT;
    }
}

void drawSphere(float x, float y, float z, float radius, glm::vec3 color) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(color.r, color.g, color.b);

    int stacks = 10;
    int slices = 10;

    for (int i = 0; i < stacks; i++) {
        float lat0 = M_PI * (-0.5f + (float)i / stacks);
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / stacks);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float lng = 2 * M_PI * (float)j / slices;
            float x = cos(lng);
            float y = sin(lng);

            glVertex3f(x * zr0 * radius, y * zr0 * radius, z0 * radius);
            glVertex3f(x * zr1 * radius, y * zr1 * radius, z1 * radius);
        }
        glEnd();
    }

    glPopMatrix();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -cameraDistance);
    glRotatef(cameraAngleY, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleX, 0.0f, 1.0f, 0.0f);

    for (const auto& p : particles) {
        switch(p.type) {
            case TYPE_CENTRAL_MASS:
                drawSphere(p.position.x, p.position.y, p.position.z, 3.5f, glm::vec3(0.1f, 0.0f, 0.1f));  // BLACK HOLE - dark purple/black, BIGGER
                break;
            case TYPE_STAR:
                drawSphere(p.position.x, p.position.y, p.position.z, 0.8f, glm::vec3(1.0f, 0.8f, 0.2f));
                break;
            case TYPE_PLANET:
                drawSphere(p.position.x, p.position.y, p.position.z, 0.3f, glm::vec3(0.3f, 0.6f, 1.0f));
                break;
            case TYPE_MOON:
                drawSphere(p.position.x, p.position.y, p.position.z, 0.15f, glm::vec3(0.7f, 0.7f, 0.7f));
                break;
        }
    }

    // Axes
    glBegin(GL_LINES);
    glColor3f(0.3f, 0.0f, 0.0f);
    glVertex3f(-50.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.3f, 0.0f);
    glVertex3f(0.0f, -50.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.3f);
    glVertex3f(0.0f, 0.0f, -50.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);
    glEnd();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mousePressed = (action == GLFW_PRESS);
        if (mousePressed) {
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        cameraAngleX += (xpos - lastMouseX) * 0.5f;
        cameraAngleY += (ypos - lastMouseY) * 0.5f;
        lastMouseX = xpos;
        lastMouseY = ypos;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraDistance -= yoffset * 3.0f;
    if (cameraDistance < 5.0f) cameraDistance = 5.0f;
    if (cameraDistance > 150.0f) cameraDistance = 150.0f;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1400, 1000, "Spinning Universe - Hierarchical Masses", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = 1400.0f / 1000.0f;
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 300.0f;
    float top = nearPlane * tan(fov * 0.5f * M_PI / 180.0f);
    glFrustum(-top * aspect, top * aspect, -top, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);

    initParticles();

    std::cout << "\n=== SPINNING UNIVERSE SIMULATION ===" << std::endl;
    std::cout << "Universe rotation: " << UNIVERSE_ROTATION_SPEED << std::endl;
    std::cout << "Central mass: " << CENTRAL_MASS << std::endl;
    std::cout << "\nHierarchy:" << std::endl;
    std::cout << "- Yellow: Central mass" << std::endl;
    std::cout << "- Orange: Stars" << std::endl;
    std::cout << "- Blue: Planets" << std::endl;
    std::cout << "- Gray: Moons" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        updatePhysics();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    glfwTerminate();
    return 0;
}
