#ifndef CT_GUI_MANAGER_HPP
#define CT_GUI_MANAGER_HPP

#include <ekg/ekg.hpp>
#include "tools/pickup.hpp"

namespace ct {
  class gui_manager {
  public:
    ekg::runtime ekg_runtime {};
  public:
    ct::pickup_info_t camera_pickup_info {};
    ct::pickup_info_t object_pickup_info {};
  public:
    void init();
    void quit();
    void on_poll_event();
    void on_update();
  };
}

#endif