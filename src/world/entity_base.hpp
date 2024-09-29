#ifndef CT_WORLD_ENTITY_BASE_HPP
#define CT_WORLD_ENTITY_BASE_HPP

#include <bicudo/physics/placement.hpp>
#include "io/game_context.hpp"
#include "io/memory.hpp"

namespace ct {
  class entity_base {
  public:
    bicudo::physics::placement placement {};
    ct::flags pickup {ct::pickup_type::NONE};
    bicudo::id texture_id {};
    ct::entity_type type {ct::entity_type::OBJECT};
  public:
    entity_base() = default;
    entity_base(bicudo::physics::placement _placement) : placement(_placement) {
      // meow
    };

    void set_texture(bicudo::id id);

    virtual void on_spawn() {};
    virtual void on_dead() {};
    virtual void on_update();
    virtual void on_render();
  };
}

#endif