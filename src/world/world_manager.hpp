#ifndef CT_WORLD_MANAGER_HPP
#define CT_WORLD_MANAGER_HPP

#include "entity_base.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <queue>

#define CT_WORLD_BEDROCK 20000
#define CT_WORLD_LIMIT 500000
#define CT_FREE_ENTITY(p_entity, index) bicudo::erase(&ct::p_app->bicudo_runtime, p_entity->placement.id); delete p_entity; p_entity = nullptr; this->loaded_entity_list.erase(this->loaded_entity_list.begin() + index)

namespace ct {
  struct event_t {
  public:
    ct::event_type type {};
    void *p_data {};
  };

  class world_manager {
  public:
    std::vector<ct::entity_base*> loaded_entity_list {};
    std::queue<ct::event_t> in_world_event_poll {};
    int32_t fract_amount {4};
  protected:
    void process_world_events(
      ct::event_t *p_event
    );
  public:
    ct::entity_base *find_entity_by_id(bicudo::id id, uint64_t *p_index = nullptr);
    ct::entity_base *push_back_entity(ct::entity_base *p_entity_base);
    bool erase_entity(bicudo::id id);

    void on_load();
    void on_poll_event();
    void on_update();
    void on_render();
  };
}

namespace ct {
  void on_event_collision_pre_apply_forces(
    bicudo::physics::placement *&p_a,
    bicudo::physics::placement *&p_b
  );
}

#endif