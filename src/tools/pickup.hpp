#ifndef CT_TOOLS_PICKUP_HPP
#define CT_TOOLS_PICKUP_HPP

#include "io/game_context.hpp"
#include "world/entity_base.hpp"

namespace ct {
  struct pickup_info_t {
  public:
    bicudo::vec2 delta {};
    bicudo::vec2 pick_pos {};
    bicudo::vec2 prev_pos {};
    ct::entity_base *p_entity_base {};
    ct::pickup_type pickup_type {};
  };
}

namespace ct {
  void tools_to_local_camera(
    bicudo::vec2 *p_vec
  );

  void tools_to_world(
    bicudo::vec2 *p_vec
  );

  bicudo::collided tools_pick_entity(
    ct::entity_base *&p_entity_base,
    bicudo::vec2 pos
  );

  void tools_pick_camera(
    ct::pickup_info_t *p_pickup_info
  );

  void tools_update_picked_camera(
    ct::pickup_info_t *p_pickup_info
  );

  bicudo::collided tools_pick_object_from_world(
    ct::pickup_info_t *p_pickup_info
  );

  void tools_update_picked_object(
    ct::pickup_info_t *p_pickup_info
  );
}

#endif