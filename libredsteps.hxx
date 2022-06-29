#ifndef LIBREDSTEPS_HPP
#define LIBREDSTEPS_HPP

namespace redsteps {

using Kelvin = unsigned;

class Display {
  void* m_xdisplay = nullptr;
  int m_screen = 0;

public:
  Display() noexcept;
  Display(const Display&) = delete;
  ~Display();
  Display& operator=(const Display&) = delete;
  
  bool set_crude_temperature(Kelvin temperature) noexcept;
  
  [[nodiscard]] explicit constexpr operator bool() noexcept { return m_xdisplay; }
};

}

#endif
