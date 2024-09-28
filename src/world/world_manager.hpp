#ifndef CT_WORLD_MANAGER_HPP
#define CT_WORLD_MANAGER_HPP

#include "entity_base.hpp"
#include <SDL2/SDL.h>
#include <vector>

namespace ct {
  class world_manager {
  public:
    std::vector<ct::entity_base*> loaded_entity_list {};
  public:
    ct::entity_base *push_back_entity(ct::entity_base *p_entity_base);
    void on_load();
    void on_poll_event();
    void on_update();
    void on_render();
  };
}

#endif