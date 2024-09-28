#include "pickup.hpp"

#include <bicudo/bicudo.hpp>
#include <ekg/ekg.hpp>
#include "client.hpp"

void ct::tools_to_local_camera(bicudo::vec2 *p_vec) {
  p_vec->x /= ct::p_app->camera.zoom;
  p_vec->y /= ct::p_app->camera.zoom;
}

bicudo::collided ct::tools_pick_physics_placement(
  bicudo::physics::placement *&p_placement,
  bicudo::vec2 pos
) {
  bicudo::vec2 &cam {ct::p_app->camera.placement.pos};
  float &zoom {ct::p_app->camera.zoom};
  pos = (pos / zoom) + cam;

  for (bicudo::physics::placement *&p_placements : ct::p_app->bicudo_runtime.placement_list) {
    if (bicudo::aabb_collide_with_vec2(p_placements->min, p_placements->max, pos)) {
      p_placement = p_placements;
      return true;
    }
  }

  return false;
}

void ct::tools_pick_camera(
  ct::pickup_info_t *p_pickup_info
) {
  ct::camera &camera {ct::p_app->camera};
  ekg::vec4 &interact {ekg::input::interact()};

  if (ekg::hovered.id == 0 && ekg::input::action("zoom-camera")) {
    ct::p_app->camera.interpolated_zoom = bicudo_clamp_min(
      ct::p_app->camera.interpolated_zoom + interact.w * 0.09f,
      0.000001f
    );
  }

  if (!p_pickup_info->p_placement && ekg::hovered.id == 0 && ekg::input::action("click-on-camera")) {
    p_pickup_info->p_placement = &camera.placement;

    p_pickup_info->delta.x = interact.x - p_pickup_info->p_placement->min.x;
    p_pickup_info->delta.y = interact.y - p_pickup_info->p_placement->min.y;

    p_pickup_info->pick_pos.x = p_pickup_info->p_placement->pos.x;
    p_pickup_info->pick_pos.y = p_pickup_info->p_placement->pos.y;

    p_pickup_info->prev_pos.x = interact.x;
    p_pickup_info->prev_pos.y = interact.y;
  } else if (ekg::input::action("drop-camera")) {
    p_pickup_info->p_placement = nullptr;
  }
}

void ct::tools_update_picked_camera(
  ct::pickup_info_t *p_pickup_info
) {
  if (!p_pickup_info->p_placement) {
    return;
  }

  ekg::vec4 &interact {ekg::input::interact()};

  p_pickup_info->p_placement->velocity = {
    -((interact.x - p_pickup_info->delta.x) - (p_pickup_info->prev_pos.x - p_pickup_info->delta.x)),
    -((interact.y - p_pickup_info->delta.y) - (p_pickup_info->prev_pos.y - p_pickup_info->delta.y))
  };

  p_pickup_info->p_placement->velocity *= (5.0f * bicudo_clamp_min(1.0f - ct::p_app->camera.zoom, 0.1f));

  p_pickup_info->prev_pos.x = interact.x;
  p_pickup_info->prev_pos.y = interact.y;
}

bicudo::collided ct::tools_pick_object_from_world(
  ct::pickup_info_t *p_pickup_info
) {
  ekg::vec4 &interact {ekg::input::interact()};

  if (
      !p_pickup_info->p_placement &&
      ekg::hovered.id == 0 &&
      ekg::input::action("click-on-object") &&
      ct::tools_pick_physics_placement(p_pickup_info->p_placement, bicudo::vec2(interact.x, interact.y))
    ) {

    p_pickup_info->delta.x = interact.x - p_pickup_info->p_placement->min.x;
    p_pickup_info->delta.y = interact.y - p_pickup_info->p_placement->min.y;

    p_pickup_info->pick_pos.x = p_pickup_info->p_placement->pos.x;
    p_pickup_info->pick_pos.y = p_pickup_info->p_placement->pos.y;

    p_pickup_info->prev_pos.x = interact.x;
    p_pickup_info->prev_pos.y = interact.y;

    ct::tools_to_local_camera(&p_pickup_info->pick_pos);
    ct::tools_to_local_camera(&p_pickup_info->prev_pos);
    ct::tools_to_local_camera(&p_pickup_info->delta);

    return true;
  } else if (ekg::input::action("drop-object")) {
    p_pickup_info->p_placement = nullptr;
    return false;
  }

  return false;
}

void ct::tools_update_picked_object(
  ct::pickup_info_t *p_pickup_info
) {
  if (!p_pickup_info->p_placement) {
    return;
  }

  ekg::vec4 interact {ekg::input::interact()};

  interact.x /= ct::p_app->camera.zoom;
  interact.y /= ct::p_app->camera.zoom;

  p_pickup_info->p_placement->velocity = {
    ((interact.x - p_pickup_info->delta.x) - (p_pickup_info->prev_pos.x - p_pickup_info->delta.x)),
    ((interact.y - p_pickup_info->delta.y) - (p_pickup_info->prev_pos.y - p_pickup_info->delta.y))
  };

  if (bicudo::assert_float(p_pickup_info->p_placement->mass, 0.0f)) {
    bicudo::physics_placement_move(
      p_pickup_info->p_placement,
      p_pickup_info->p_placement->velocity
    );

    p_pickup_info->p_placement->velocity = {};
  }
  p_pickup_info->p_placement->acc = {};

  p_pickup_info->prev_pos.x = interact.x;
  p_pickup_info->prev_pos.y = interact.y;
}