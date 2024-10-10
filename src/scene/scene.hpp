#ifndef CT_SCENE_HPP
#define CT_SCENE_HPP

#include <string_view>

namespace ct {
  struct scene_context_output_t {
  public:
    const char *p_tag {};
    bool was_complete {};
  };

  class scene {
  public:
    std::string_view tag {};
    ct::scene_context_output_t context_output {};
  public:
    virtual void on_open() {};
    virtual void on_close() {};
    virtual void on_update() {};
  };
}

#endif