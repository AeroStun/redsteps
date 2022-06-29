cmake_minimum_required (VERSION 3.17)

project (redsteps VERSION 0.1)

find_package (X11 REQUIRED)


add_library (libredsteps)
target_sources (libredsteps PRIVATE libredsteps.cxx)
target_compile_features (libredsteps PRIVATE cxx_std_17)
target_link_libraries (libredsteps PRIVATE X11::Xxf86vm)

add_executable (redsteps)
target_sources (redsteps PRIVATE redsteps.cxx)
target_compile_features (redsteps PRIVATE cxx_std_17)
target_link_libraries (redsteps PRIVATE libredsteps)

if (BUILD_GUI)
  find_package (PkgConfig REQUIRED)
  pkg_check_modules (GTKMM REQUIRED gtkmm-3.0)
  add_library (gtkmm INTERFACE)
  target_include_directories (gtkmm INTERFACE ${GTKMM_INCLUDE_DIRS})
  target_link_directories (gtkmm INTERFACE ${GTKMM_LIBRARY_DIRS})
  target_link_libraries (gtkmm INTERFACE ${GTKMM_LIBRARIES})

  add_executable (redsteps-gtk)
  target_sources (redsteps-gtk PRIVATE redsteps-gtk.cxx)
  target_compile_features (redsteps-gtk PRIVATE cxx_std_17)
  target_link_libraries (redsteps-gtk PRIVATE libredsteps gtkmm)
endif ()
