#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string_view>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

#include "libredsteps.hxx"

#define X_DEFAULT_SCREEN(display) DefaultScreen(display)

using namespace std::literals;

namespace redsteps {


constexpr static int x_ext_min_major = 2;
constexpr static int x_ext_min_minor = 0;

struct GammaRGBf {
  float r{};
  float g{};
  float b{};
  
  [[nodiscard]] constexpr operator XF86VidModeGamma() { return {r, g, b}; }
};

// r = k >= 6500 ? 0.49379567 + 3058.16052692 / k : 1
// g = k >= 6500 ? 0.66392691 + 2061.71543054 / k : -2.55827344 + 0.40722327 * ln(k)
// b = k >= 6500 ? 1 : (k >= 2000 ? -5.72087058 + 0.76701598 * ln(k) : 0)

[[nodiscard]] constexpr float k2r(Kelvin k) noexcept {
  if (k <= 6500)
    return 1.0f;
  return 0.49379567f + 3058.16052692f / k;
}

[[nodiscard]] constexpr float k2g(Kelvin k) noexcept {
  if (k <= 6500)
    return -2.55827344f + 0.40722327f * std::log(static_cast<float>(k));
  return 0.66392691f + 2061.71543054f / k;
}

[[nodiscard]] constexpr float k2b(Kelvin k) noexcept {
  if (k < 2000)
    return 0.1f; // 0.1 is the minimum value allowed by XF86VidMode
  if (k < 6500)
    return -5.72087058f + 0.76701598f * std::log(static_cast<float>(k));
  return 1.0f;
}

[[nodiscard]] constexpr GammaRGBf kelvin_to_gamma(Kelvin k) noexcept {
  return {k2r(k), k2g(k), k2b(k)};
}

int set_xf86vm_gamma(::Display* display, int screen, GammaRGBf gamma) noexcept {
  ::XF86VidModeGamma xf86vm_gamma = gamma;
  if (!::XF86VidModeSetGamma(display, screen, &xf86vm_gamma)) {
      std::cerr << "Unable to set gamma correction\n";
      return 2;
  }
  
  return 0;
}

int set_xf86vm_temperature(::Display* display, int screen, Kelvin temperature) noexcept {
  return set_xf86vm_gamma(display, screen, kelvin_to_gamma(temperature));
}

Display::Display() noexcept {
  char* display_name = nullptr;
  auto display = std::unique_ptr<::Display, decltype(&XCloseDisplay)>{::XOpenDisplay(display_name), XCloseDisplay};
  if (!display) {
    std::cerr << "Unable to open display '" << ::XDisplayName(display_name) << "'\n";
    return;
  }
  const int screen = X_DEFAULT_SCREEN(display.get());
  
  int major_version;
  int minor_version;
  if (!::XF86VidModeQueryVersion(display.get(), &major_version, &minor_version)) {
    std::cerr << "Unable to query video extension version\n";
    ::XCloseDisplay(display.get());
    return;
  }

  int event_base;
  int error_base;
  if (!::XF86VidModeQueryExtension(display.get(), &event_base, &error_base)) {
    std::cerr << "Unable to query video extension information\n";
    ::XCloseDisplay(display.get());
    return;
  }

  if (major_version < x_ext_min_major || (major_version == x_ext_min_major && minor_version < x_ext_min_minor)) {
    std::cerr << "Xserver is running an old XFree86-VidModeExtension version (" << major_version << '.' << minor_version << ")\n";
    std::cerr << "Minimum required version is " << x_ext_min_major << '.' << x_ext_min_minor << '\n';
    ::XCloseDisplay(display.get());
    return;
  }
  
  m_xdisplay = display.release();
  m_screen = screen;
}

Display::~Display() {
  if (m_xdisplay)
    ::XCloseDisplay(static_cast<::Display*>(m_xdisplay));
}
  
bool Display::set_crude_temperature(Kelvin temperature) noexcept {
  if (!m_xdisplay)
    return false;
  assert(1'000 <= temperature && temperature <= 25'000);
  return !set_xf86vm_temperature(static_cast<::Display*>(m_xdisplay), m_screen, temperature);
}

}
