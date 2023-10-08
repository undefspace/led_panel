# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS esp-idf-lib/components)
project(undef_led_panel)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=format"
                                       "-fsanitize=undefined" "-fno-sanitize=shift-base" APPEND)
