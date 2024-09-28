#ifndef CT_WORLD_ENTITY_BASE_HPP
#define CT_WORLD_ENTITY_BASE_HPP

#include <bicudo/physics/placement.hpp>

namespace ct {
  class entity_base {
  public:
    bicudo::physics::placement placement {};
  public:
    entity_base() = default;
    entity_base(bicudo::physics::placement _placement) : placement(_placement) {
      // meow
    };

    virtual void on_spawn() {};
    virtual void on_dead() {};
    virtual void on_update();
    virtual void on_render();
  };
}

#endif