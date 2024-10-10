#ifndef CT_SCENE_MANAGER_HPP
#define CT_SCENE_MANAGER_HPP

#include "scene.hpp"

namespace ct {
  class scene_manager {
  public:
    ct::scene *p_current_scene {nullptr};
  public:
    void open(ct::scene *p_scene);
    void close();
    void on_update();
  };
}

#endif