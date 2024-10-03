#ifndef CT_IO_GAME_CONTEXT_HPP
#define CT_IO_GAME_CONTEXT_HPP

namespace ct {
  enum camera_state {
    FREE,
    TARGET 
  };

  enum entity_type {
    OBJECT,
    PLAYER,
    ENEMY
  };

  enum pickup_type {
    NONE      = 2 << 1,
    DRAG      = 2 << 2,
    SLINGSHOT = 2 << 3
  };

  enum collided_state {
    FRACTURED = 2 << 1
  };
}

#endif