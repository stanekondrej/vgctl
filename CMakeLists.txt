cmake_minimum_required (VERSION 3.8)

project ("vgctl")

add_executable (vgctl "vgctl.cpp" "Vanguard.h" "Vanguard.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET vgctl PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
