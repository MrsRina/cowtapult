#ifndef CT_WORLD_ENTITY_PLAYER_HPP
#define CT_WORLD_ENTITY_PLAYER_HPP

#include "entity_base.hpp"

namespace ct {
  class entity_player : public entity_base {
  public:
    entity_player() = default;
    entity_player(bicudo::physics::placement _placement) : entity_base(_placement) {};

    void on_update() override;
    void on_render() override;
  };
}

#endif