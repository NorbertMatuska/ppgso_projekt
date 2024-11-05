#include <iostream>
#include <vector>
#include <map>
#include <list>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/random.hpp>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

class Camera {
public:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    // Added yaw and pitch angles
    float yaw;
    float pitch;

    /// Representation of a camera
    /// \param fov - Field of view (in degrees)
    /// \param ratio - Viewport ratio (width/height)
    /// \param near - Distance of the near clipping plane
    /// \param far - Distance of the far clipping plane
    Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 100.0f) {
        projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
        position = glm::vec3(0.0f, 5.0f, 20.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);

        // Initialize yaw and pitch
        yaw = -90.0f; // Yaw is initialized to -90 degrees to face towards negative Z
        pitch = 0.0f;

        update();
    }

    /// Recalculate viewMatrix from position, yaw, and pitch
    void update() {
        // Calculate the new front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        target = position + front;

        viewMatrix = glm::lookAt(position, target, up);
    }
};

/// Abstract renderable object interface
class Renderable; // Forward declaration for Scene
using Scene = std::list<std::unique_ptr<Renderable>>; // Type alias

class Renderable {
public:
    // Virtual destructor is needed for abstract interfaces
    virtual ~Renderable() = default;

    /// Render the object
    /// \param camera - Camera to use for rendering
    virtual void render(const Camera& camera) = 0;

    /// Update the object. Useful for specifying animation and behaviour.
    /// \param dTime - Time delta
    /// \param scene - Scene reference
    /// \return - Return true to keep object in scene
    virtual bool update(float dTime, Scene &scene) = 0;
};

class Plane final : public Renderable {
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

public:
    Plane() {
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
    }

    bool update(float dTime, Scene &scene) override {
        // Plane does not need to update
        return true;
    }

    void render(const Camera& camera) override {
        shader->use();
        shader->setUniform("OverallColor", glm::vec3(0.5f, 0.5f, 0.5f));
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(30.0f));
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        mesh->render();
    }
};

std::unique_ptr<ppgso::Mesh> Plane::mesh;
std::unique_ptr<ppgso::Shader> Plane::shader;

/// Splash particle class for splash animation
class SplashParticle final : public Renderable {
    // Static resources shared between all splash particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 color;
    float age;
    float lifetime;
    glm::mat4 modelMatrix;

public:
    SplashParticle(glm::vec3 p, glm::vec3 s, glm::vec3 c, float lt) {
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
        position = p;
        speed = s;
        color = c;
        age = 0.0f;
        lifetime = lt; // Splash particles live for 1 second
    }

    bool update(float dTime, Scene &scene) override {
        position += speed * dTime;
        age += dTime;

        // Simple gravity effect
        speed.y -= 9.8f * dTime; // Gravity acceleration

        if (age >= lifetime)
            return false;

        // Update modelMatrix
        float scale = 0.1f * (1.0f - age / lifetime);
        modelMatrix = glm::translate(glm::mat4(1.0f), position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
        return true;
    }

    void render(const Camera& camera) override {
        shader->use();
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("OverallColor", color);
        mesh->render();
    }
};
// Static resources need to be instantiated outside of the class as they are globals
std::unique_ptr<ppgso::Mesh> SplashParticle::mesh;
std::unique_ptr<ppgso::Shader> SplashParticle::shader;

/// Basic particle that will render a sphere
class Particle final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 color;
    float age;
    float lifetime;
    glm::mat4 modelMatrix;

public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    Particle(glm::vec3 p, glm::vec3 s, glm::vec3 c) {
        // First particle will initialize resources
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
        position = p;
        speed = s;
        color = c;
        age = 0.0f;
        lifetime = 5.0f;
    }

    bool update(float dTime, Scene &scene) override {
        // Animate position using speed and dTime.
        position += speed * dTime;
        age += dTime;

        // Check if particle has reached the ground
        if (position.y <= 0.5f) {
            // Create splash particles
            const int NUM_SPLASH_PARTICLES = 10;
            for (int i = 0; i < NUM_SPLASH_PARTICLES; ++i) {
                float angle = glm::linearRand(0.0f, glm::two_pi<float>());
                float speedValue = glm::linearRand(0.1f, 0.5f);
                glm::vec3 splashSpeed = glm::vec3(cos(angle), 0.5f, sin(angle)) * speedValue;
                glm::vec3 splashColor = glm::vec3(0.0f, 0.0f, 1.0f); // Blue color
                auto splashParticle = std::make_unique<SplashParticle>(position, splashSpeed, splashColor, 1.0f);
                scene.push_back(std::move(splashParticle));
            }
            return false; // Remove the rain drop particle
        }

        // Update the modelMatrix
        modelMatrix = glm::translate(glm::mat4(1.0f), position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f)); // Small sphere

        return true;
    }

    void render(const Camera& camera) override {
        // Render the object
        shader->use();
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("OverallColor", color);
        mesh->render();
    }
};
// Static resources need to be instantiated outside of the class as they are globals
std::unique_ptr<ppgso::Mesh> Particle::mesh;
std::unique_ptr<ppgso::Shader> Particle::shader;

class ParticleWindow : public ppgso::Window {
private:
    // Scene of objects
    Scene scene;

    // Create camera
    Camera camera = {60.0f, (float)width / (float)height, 0.1f, 100.0f};

    // Store keyboard state
    std::map<int, bool> keys;

    // Mouse state
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
    bool firstMouse = true;

    // Mouse sensitivity
    float sensitivity = 0.1f;

public:
    ParticleWindow() : Window{"task7_particles", SIZE, SIZE} {
        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        auto plane = std::make_unique<Plane>();
        scene.push_back(std::move(plane));

        // Hide the cursor and capture it within the window
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        // Update key state
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }

        // Handle spacebar press to create particles
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            // Add renderable object to the scene
            // Create an explosion of particles at a random position
            float x = glm::linearRand(-7.0f, 7.0f);
            float y = glm::linearRand(0.5f, 5.0f);
            float z = glm::linearRand(-7.0f, 7.0f);
            glm::vec3 position = glm::vec3(x, y, z);

            float r = glm::linearRand(0.2f, 1.0f);
            float g = glm::linearRand(0.2f, 1.0f);
            float b = glm::linearRand(0.2f, 1.0f);

            float spd = glm::linearRand(5.0f, 10.0f);
            for (int i = 0; i < 100; ++i) {
                // Random speed in all directions
                glm::vec3 color = glm::vec3(r, g, b); // Random color
                glm::vec3 speed = glm::sphericalRand(spd);
                float lt = glm::linearRand(0.1f, 1.5f);

                auto particle = std::make_unique<SplashParticle>(position, speed, color, lt);

                scene.push_back(std::move(particle));
            }
        }

        // Close the window if Escape is pressed
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void onCursorPos(double xpos, double ypos) override {
        if (firstMouse) {
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos) - lastX;
        float yoffset = lastY - static_cast<float>(ypos); // Reversed since y-coordinates go from bottom to top
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.yaw += xoffset;
        camera.pitch += yoffset;

        // Constrain the pitch angle
        if (camera.pitch > 89.0f)
            camera.pitch = 89.0f;
        if (camera.pitch < -89.0f)
            camera.pitch = -89.0f;

        // Update the camera's direction
        camera.update();
    }

    void onIdle() override {
        // Track time
        static auto time = (float)glfwGetTime();
        // Compute time delta
        float dTime = (float)glfwGetTime() - time;
        time = (float)glfwGetTime();

        // Set gray background
        glClearColor(.1f, .1f, .1f, 1.0f);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static float particleTime = 0.0f;
        particleTime += dTime;
        if (particleTime > 0.0f) { // Generate a particle every 0.01 seconds
            particleTime = 0.0f;
            // Create rain particle at random x,z position, y at some height
            float x = glm::linearRand(-10.0f, 10.0f);
            float y = 10.0f; // Starting height
            float z = glm::linearRand(-10.0f, 10.0f);
            glm::vec3 position = glm::vec3(x, y, z);
            glm::vec3 speed = glm::vec3(0.0f, -5.0f, 0.0f); // Falling down
            glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue color
            auto particle = std::make_unique<Particle>(position, speed, color);
            scene.push_back(std::move(particle));
        }

        // Handle camera movement
        float cameraSpeed = 5.0f; // Adjust speed as necessary
        glm::vec3 forward = glm::normalize(camera.target - camera.position);
        glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));

        if (keys[GLFW_KEY_W]) {
            camera.position += forward * cameraSpeed * dTime;
        }
        if (keys[GLFW_KEY_S]) {
            camera.position -= forward * cameraSpeed * dTime;
        }
        if (keys[GLFW_KEY_A]) {
            camera.position -= right * cameraSpeed * dTime;
        }
        if (keys[GLFW_KEY_D]) {
            camera.position += right * cameraSpeed * dTime;
        }

        // Update camera (already called in onCursorPos, but position may have changed)
        camera.update();

        // Update all objects in scene
        auto i = std::begin(scene);
        while (i != std::end(scene)) {
            // Update object and remove from list if needed
            auto obj = i->get();
            if (!obj->update(dTime, scene))
                i = scene.erase(i);
            else
                ++i;
        }

        // Render every object in scene
        for (auto& object : scene) {
            object->render(camera);
        }
    }
};

int main() {
    // Create new window
    auto window = ParticleWindow{};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
