#ifndef CT_WORLD_ENTITY_ENEMY_HPP
#define CT_WORLD_ENTITY_ENEMY_HPP

#include "entity_base.hpp"

namespace ct {
  class entity_enemy : public ct::entity_base {
  public:
    entity_enemy() = default;
    entity_enemy(bicudo::physics::placement _placement) : entity_base(_placement) {
      this->type = ct::entity_type::ENEMY;
    };

    void on_spawn() override;
    void on_dead() override;
    void on_update() override;
    void on_render() override;
  };
}

#endif