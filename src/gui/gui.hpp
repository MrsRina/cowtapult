#ifndef CT_GUI_HPP
#define CT_GUI_HPP

#include <ekg/util/gui.hpp>

namespace ct {
  class gui {
  public:
    bool was_open {};
    ekg::stack stack {};
  public:
    virtual void on_open() {};
    virtual void on_screen_change_size() {};
    virtual void on_close() {};
  };

  class gui_loading : public ct::gui {
  public:
    static float progress;
  public:
    void on_open() override;
    void on_screen_change_size() override;
    void on_close() override;
  };

  class gui_main_menu : public ct::gui {
  public:
    void on_open() override;
    void on_screen_change_size() override;
    void on_close() override;
  };
}

#endif