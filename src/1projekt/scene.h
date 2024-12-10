#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>
#include <vector>

#include "object.h"
#include "camera.h"


class Scenegl {
  public:
    void update(float time);
    void render();

    std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);
    std::unique_ptr<Camera> camera;
    std::list< std::unique_ptr<Object> > objects;
    std::map< int, int > keyboard;
    glm::vec3 lightDirection{-1.0f, -1.0f, -1.0f};

    struct {
      double x, y;
      bool left, right;
    } cursor;
};

#endif // _PPGSO_SCENE_H
