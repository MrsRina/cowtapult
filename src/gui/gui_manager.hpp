#ifndef CT_GUI_MANAGER_HPP
#define CT_GUI_MANAGER_HPP

#include <ekg/ekg.hpp>
#include "tools/pickup.hpp"
#include "gui/gui.hpp"
#include <unordered_map>

namespace ct {
  class gui_manager {
  public:
    ekg::runtime ekg_runtime {};
    std::unordered_map<std::string_view, ct::gui*> gui_map {};
    std::string_view current_open_gui {};
  public:
    bicudo::id entity_target_sequence {};
    ct::pickup_info_t camera_pickup_info {};
    ct::pickup_info_t object_pickup_info {};
  public:
    template<typename t>
    void open(std::string_view tag) {
      if (this->current_open_gui != "null") {
        this->close(this->current_open_gui);
      }

      this->current_open_gui = tag;

      ct::gui *&p_gui {
        this->gui_map[tag]
      };

      if (p_gui == nullptr) {
        p_gui = new t();
      }

      if (p_gui->was_open) {
        return;
      }

      p_gui->on_open();
      p_gui->was_open = true;
    }

    void close(std::string_view tag);

    void init();
    void quit();
    void on_poll_event();
    void on_update();
  };
}

#endif