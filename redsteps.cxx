#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

#include "libredsteps.hxx"

#define X_DEFAULT_SCREEN(display) DefaultScreen(display)

using namespace std::literals;

int main(int argc, char** argv) {
  std::ios::sync_with_stdio(false);
  
  if (argc <= 1 || argv[1] == "--help"sv) {
    std::cout << "Usage: " << argv[0] << " <temperature (in Kelvin, min 1000, neutral 6500, max 25000)>\n";
    return 0;
  }
  
  const redsteps::Kelvin temperature = std::strtoul(argv[1], nullptr, 10);
  if (temperature < 1'000 || temperature > 25'000) {
    std::cout << argv[0] << ": temperature out of range (min 1000, neutral 6500, max 25000)\n";
    return 3;
  }
  
  redsteps::Display display;
  if (!display)
    return 1;
  
  return display.set_crude_temperature(temperature) ? 0 : 1;
}
