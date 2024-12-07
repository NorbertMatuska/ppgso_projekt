#include "scene.h"
#include "car.h"

void Scenegl::update(float time) {
  camera->update();

  // Use iterator to update all objects so we can remove while iterating
  auto i = std::begin(objects);

  while (i != std::end(objects)) {
    // Update and remove from list if needed
    auto obj = i->get();
    if (!obj->update(*this, time))
      i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;
  }
  for (auto it1 = objects.begin(); it1 != objects.end(); ++it1) {
    auto car1 = dynamic_cast<Car*>(it1->get());
    if (!car1) continue;

    for (auto it2 = std::next(it1); it2 != objects.end(); ++it2) {
      auto car2 = dynamic_cast<Car*>(it2->get());
      if (!car2 || car1 == car2) continue;

      //if (car1->checkCollision(*car2)) {
        //car1->simulateCollision(*car2);
        //car2->simulateCollision(*car1);
        // Print the position of the camera to the console
        //std::cout << "CARS CRASHED" << std::endl;
      //}
    }
  }
}

void Scenegl::render() {
  // Simply render all objects
  for ( auto& obj : objects )
    obj->render(*this);
}

std::vector<Object*> Scenegl::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
  std::vector<Object*> intersected = {};
  for(auto& object : objects) {
    // Collision with sphere of size object->scale.x
    auto oc = position - object->position;
    auto radius = object->scale.x;
    auto a = glm::dot(direction, direction);
    auto b = glm::dot(oc, direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto dis = b * b - a * c;

    if (dis > 0) {
      auto e = sqrt(dis);
      auto t = (-b - e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }

      t = (-b + e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }
    }
  }

  return intersected;
}
