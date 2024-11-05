// Task 6 - Generate a Bezier surface of variable density with UV coordinates.
//        - Confine the Bezier data and associated methods into a reusable class.
//        - Define a modelMatrix that uses position, rotation and scale.
//        - Render the generated mesh with texturing applied.
//        - Animate rotation.

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include <ppgso/ppgso.h>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

const float WAVE_FREQUENCY = 2.0f;
const float WAVE_AMPLITUDE = 0.2f;
const unsigned int SIZE = 512;

// Object to represent Bezier patch
class BezierPatch {
private:
  // 3D vectors define points/vertices of the shape
  std::vector<glm::vec3> vertices;

  // Texture coordinates
  std::vector<glm::vec2> texCoords;

  // Store original positions for animation
  std::vector<glm::vec3> originalVertices;

    // Define our face using indexes to 3 vertices
  struct face {
    GLuint v0, v1, v2;
  };

  // Define our mesh as collection of faces
  std::vector<face> mesh;

  // These will hold the data and object buffers
  GLuint vao, vbo, tbo, ibo;
  glm::mat4 modelMatrix{1.0f};

  glm::vec3 bezierPoint(const glm::vec3 controlPoints[4], float t) {
      // TODO: Compute 3D point on bezier curve
      glm::vec3 a = glm::mix(controlPoints[0], controlPoints[1], t);
      glm::vec3 b = glm::mix(controlPoints[1], controlPoints[2], t);
      glm::vec3 c = glm::mix(controlPoints[2], controlPoints[3], t);
      glm::vec3 d = glm::mix(a, b, t);
      glm::vec3 e = glm::mix(b, c, t);

      return glm::mix(d, e, t);
  }

  ppgso::Shader program{texture_vert_glsl, texture_frag_glsl};
  ppgso::Texture texture{ppgso::image::loadBMP("lena.bmp")};
public:
  // Public attributes that define position, color ..
  glm::vec3 position{0,0,0};
  glm::vec3 rotation{0,0,0};
  glm::vec3 scale{1,1,1};

  // Initialize object data buffers
  BezierPatch(const glm::vec3 controlPoints[4][4]) {
    // Generate Bezier patch points and incidences
    unsigned int PATCH_SIZE = 10;
    for(unsigned int i = 0; i < PATCH_SIZE ; i++) {
      for (unsigned int j = 0; j < PATCH_SIZE; j++) {
        // TODO: Compute points on the bezier patch
          float x = (float)i / (PATCH_SIZE - 1) - 0.5f;
          float y = (float)j / (PATCH_SIZE - 1) - 0.5f;
          vertices.push_back({x, y, 0.0f});
          originalVertices.push_back({x, y, 0.0f});
          texCoords.push_back({(float)i / (PATCH_SIZE - 1), (float)j / (PATCH_SIZE - 1)});
      }
    }
    // Generate indices
    for(unsigned int i = 1; i < PATCH_SIZE; i++) {
      for (unsigned int j = 1; j < PATCH_SIZE; j++) {
          int row1 = (i - 1) * PATCH_SIZE;
          int row2 = i * PATCH_SIZE;

          mesh.push_back({row1 + j - 1, row1 + j, row2 + j});
          mesh.push_back({row1 + j - 1, row2 + j, row2 + j - 1});
      }
    }

    // Copy data to OpenGL
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Copy positions to gpu
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto position_attrib = program.getAttribLocation("Position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy texture positions to gpu
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto texCoord_attrib = program.getAttribLocation("TexCoord");
    glEnableVertexAttribArray(texCoord_attrib);
    glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy indices to gpu
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(face), mesh.data(), GL_STATIC_DRAW);

  };
  // Clean up
  ~BezierPatch() {
    // Delete data from OpenGL
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }
  // Set the object transformation matrix
  void update() {
      // TODO: Compute transformation by scaling, rotating and then translating the shape
      for (size_t i = 0; i < vertices.size(); i++) {
          auto t = (float) glfwGetTime();

          int row = i / 10;

          if (row == 0) {
              vertices[i] = originalVertices[i];
          } else {
              vertices[i].z = originalVertices[i].z + WAVE_AMPLITUDE * sin(WAVE_FREQUENCY * t - (float)row / 2.0f);
          }
      }
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());

  }

  // Draw polygons
  void render(){
    // Update transformation and color uniforms in the shader
    program.use();

    // Initialize projection
    // Create projection matrix (field of view (radians), aspect ratio, near plane distance, far plane distance)
    // You can think of this as the camera objective settings
    auto projection = glm::perspective( (ppgso::PI/180.f) * 60.0f, 1.0f, 0.1f, 10.0f);
    program.setUniform("ProjectionMatrix", projection);

    // Create view matrix (translate camera a bit backwards, so we can see the geometry)
    // This can be seen as the camera position/rotation in space
    auto view = glm::lookAt(
            glm::vec3(-2.0f, 2.0f, -2.0f),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 1)
    );
      //glm::translate(glm::mat4{}, {0.0f, 0.0f, -3.0f});
    program.setUniform("ViewMatrix", view);

    // Set model position
    program.setUniform("ModelMatrix", modelMatrix);

    // Bind texture
    program.setUniform("Texture", texture);

    glBindVertexArray(vao);
    // TODO: Use correct rendering mode to draw the result
    glDrawElements(GL_TRIANGLES, mesh.size() * 3, GL_UNSIGNED_INT, 0);
  };
};

class BezierSurfaceWindow : public ppgso::Window {
private:
  // Define 16 control points
  glm::vec3 controlPoints[4][4]{
      { {-1,1,0}, {-0.5,1,0}, {.5,1,0}, {1,1,3}, },
      { {-1,.5,0}, {-0.5,.5,0}, {.5,.5,0}, {1,.5,0}, },
      { {-1,-.5,0}, {-0.5,-.5,0}, {.5,-.5,0}, {1,-.5,-1}, },
      { {-1,-1,3}, {-0.5,-1,0}, {.5,-1,0}, {1,-1,0}, },
  };

  BezierPatch bezier{controlPoints};
public:
  BezierSurfaceWindow() : Window{"task6_bezier_surface", SIZE, SIZE} {
    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  }

  void onIdle() final {
    // Set gray background
    glClearColor(.1f,.1f,.1f,1.0f);

    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Move and Render shape
    auto t = (float) glfwGetTime();
    bezier.rotation = {0,0,t};
    bezier.update();
    bezier.render();
  }
};

int main() {
  // Create new window
  auto window = BezierSurfaceWindow{};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
