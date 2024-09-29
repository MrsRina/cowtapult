#include "pickup.hpp"

#include <bicudo/bicudo.hpp>
#include <ekg/ekg.hpp>
#include "client.hpp"

void ct::tools_to_local_camera(bicudo::vec2 *p_vec) {
  p_vec->x /= ct::p_app->camera.zoom;
  p_vec->y /= ct::p_app->camera.zoom;
}

void ct::tools_to_world(bicudo::vec2 *p_vec) {
  p_vec->x *= ct::p_app->camera.zoom;
  p_vec->y *= ct::p_app->camera.zoom;
}

bicudo::collided ct::tools_pick_entity(
  ct::entity_base *&p_entity_base,
  bicudo::vec2 pos
) {
  bicudo::vec2 &cam {ct::p_app->camera.placement.pos};
  float &zoom {ct::p_app->camera.zoom};
  pos = (pos / zoom) + cam;

  for (ct::entity_base *p_entities : ct::p_app->world_manager.loaded_entity_list) {
    if (bicudo::aabb_collide_with_vec2(p_entities->placement.min, p_entities->placement.max, pos)) {
      p_entity_base = p_entities;
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
      0.005f
    );
  }

  if (camera.state == ct::camera_state::FREE && !p_pickup_info->p_entity_base && ekg::hovered.id == 0 && ekg::input::action("click-on-camera")) {
    p_pickup_info->p_entity_base = &camera;

    p_pickup_info->delta.x = interact.x - p_pickup_info->p_entity_base->placement.min.x;
    p_pickup_info->delta.y = interact.y - p_pickup_info->p_entity_base->placement.min.y;

    p_pickup_info->pick_pos.x = p_pickup_info->p_entity_base->placement.pos.x;
    p_pickup_info->pick_pos.y = p_pickup_info->p_entity_base->placement.pos.y;

    p_pickup_info->prev_pos.x = interact.x;
    p_pickup_info->prev_pos.y = interact.y;
  } else if (ekg::input::action("drop-camera")) {
    p_pickup_info->p_entity_base = nullptr;
  }
}

void ct::tools_update_picked_camera(
  ct::pickup_info_t *p_pickup_info
) {
  if (!p_pickup_info->p_entity_base) {
    return;
  }

  ekg::vec4 &interact {ekg::input::interact()};

  p_pickup_info->p_entity_base->placement.velocity = {
    -((interact.x - p_pickup_info->delta.x) - (p_pickup_info->prev_pos.x - p_pickup_info->delta.x)),
    -((interact.y - p_pickup_info->delta.y) - (p_pickup_info->prev_pos.y - p_pickup_info->delta.y))
  };

  p_pickup_info->p_entity_base->placement.velocity *= (5.0f * bicudo_clamp_min(1.0f - ct::p_app->camera.zoom, 0.1f));

  p_pickup_info->prev_pos.x = interact.x;
  p_pickup_info->prev_pos.y = interact.y;
}

bicudo::collided ct::tools_pick_object_from_world(
  ct::pickup_info_t *p_pickup_info
) {
  ekg::vec4 &interact {ekg::input::interact()};
  bool failed_to_pickup {};
  bool is_drag {};
  bool is_slightshot {};

  if (
      !p_pickup_info->p_entity_base &&
      ekg::hovered.id == 0 &&
      ct::tools_pick_entity(p_pickup_info->p_entity_base, bicudo::vec2(interact.x, interact.y)) &&
      !(
        failed_to_pickup = !(
          (ekg_bitwise_contains(p_pickup_info->p_entity_base->pickup, ct::pickup_type::SLINGSHOT) && (is_slightshot = ekg::input::action("drag-slingshot-object")))
          ||
          (ekg_bitwise_contains(p_pickup_info->p_entity_base->pickup, ct::pickup_type::DRAG) && (is_drag = ekg::input::action("drag-object")))
        )
      )
    ) {

    if (is_slightshot) {
      p_pickup_info->pickup_type = ct::pickup_type::SLINGSHOT;
    } else if (is_drag) {
      p_pickup_info->pickup_type = ct::pickup_type::DRAG;
    }

    p_pickup_info->p_entity_base->placement.turn_off_gravity = true;

    p_pickup_info->delta.x = interact.x - p_pickup_info->p_entity_base->placement.min.x;
    p_pickup_info->delta.y = interact.y - p_pickup_info->p_entity_base->placement.min.y;

    p_pickup_info->pick_pos.x = p_pickup_info->p_entity_base->placement.pos.x;
    p_pickup_info->pick_pos.y = p_pickup_info->p_entity_base->placement.pos.y;

    p_pickup_info->prev_pos.x = interact.x;
    p_pickup_info->prev_pos.y = interact.y;

    return true;
  } else if (ekg::input::action("drop-object") && p_pickup_info->p_entity_base) {
    switch (p_pickup_info->pickup_type) {
    case ct::pickup_type::SLINGSHOT:
      if (bicudo::assert_float(p_pickup_info->p_entity_base->placement.mass, 0.0f)) {
        break; // may it is possible to infinity mass objects be slingshot idk how
      }

      bicudo::vec2 pick_pos_center {
        (p_pickup_info->pick_pos + (p_pickup_info->p_entity_base->placement.size / 2))
      };

      bicudo::vec2 center {
        (p_pickup_info->p_entity_base->placement.pos + (p_pickup_info->p_entity_base->placement.size / 2))
      };

      bicudo::vec2 diff {center - pick_pos_center};

      float dist {
        diff.magnitude()
      };

      p_pickup_info->p_entity_base->placement.velocity = -diff.normalize() * (dist * 0.2f);
      break;
    }

    p_pickup_info->p_entity_base->placement.turn_off_gravity = false;
    p_pickup_info->p_entity_base = nullptr;
    return false;
  }

  if (failed_to_pickup && p_pickup_info->p_entity_base) {
    p_pickup_info->p_entity_base = nullptr;
  }

  return false;
}

void ct::tools_update_picked_object(
  ct::pickup_info_t *p_pickup_info
) {
  if (!p_pickup_info->p_entity_base) {
    return;
  }

  ekg::vec4 interact {ekg::input::interact()};

  interact.x /= ct::p_app->camera.zoom;
  interact.y /= ct::p_app->camera.zoom;

  p_pickup_info->p_entity_base->placement.acc = {};

  ct::tools_to_local_camera(&p_pickup_info->prev_pos);
  ct::tools_to_local_camera(&p_pickup_info->delta);

  switch (p_pickup_info->pickup_type) {
  case ct::pickup_type::DRAG:
    p_pickup_info->p_entity_base->placement.velocity = {
      ((interact.x - p_pickup_info->delta.x) - (p_pickup_info->prev_pos.x - p_pickup_info->delta.x)),
      ((interact.y - p_pickup_info->delta.y) - (p_pickup_info->prev_pos.y - p_pickup_info->delta.y))
    };

    if (bicudo::assert_float(p_pickup_info->p_entity_base->placement.mass, 0.0f)) {
      bicudo::physics_placement_move(
        &p_pickup_info->p_entity_base->placement,
        p_pickup_info->p_entity_base->placement.velocity
      );
  
      p_pickup_info->p_entity_base->placement.velocity = {};
    }
    break;
  case ct::pickup_type::SLINGSHOT:
    p_pickup_info->p_entity_base->placement.velocity = {
      ((interact.x - p_pickup_info->delta.x) - (p_pickup_info->prev_pos.x - p_pickup_info->delta.x)),
      ((interact.y - p_pickup_info->delta.y) - (p_pickup_info->prev_pos.y - p_pickup_info->delta.y))
    };

    break;
  }

  p_pickup_info->prev_pos.x = interact.x;
  p_pickup_info->prev_pos.y = interact.y;

  ct::tools_to_world(&p_pickup_info->prev_pos);
  ct::tools_to_world(&p_pickup_info->delta);
}