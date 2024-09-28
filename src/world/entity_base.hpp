#ifndef CT_WORLD_ENTITY_BASE_HPP
#define CT_WORLD_ENTITY_BASE_HPP

#include <bicudo/physics/placement.hpp>
#include "io/game_context.hpp"

namespace ct {
  class entity_base {
  public:
    bicudo::physics::placement placement {};
    ct::pickup_type pickup_type {ct::pickup_type::NONE};
    bicudo::id texture_id {};
  public:
    entity_base() = default;
    entity_base(bicudo::physics::placement _placement) : placement(_placement) {
      // meow
    };

    void set_texuture(bicudo::id id);

    virtual void on_spawn() {};
    virtual void on_dead() {};
    virtual void on_update();
    virtual void on_render();
  };
}

#endif