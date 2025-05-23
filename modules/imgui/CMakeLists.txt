##########################################################################################
#                                                                                        #
# OpenSpace                                                                              #
#                                                                                        #
# Copyright (c) 2014-2025                                                                #
#                                                                                        #
# Permission is hereby granted, free of charge, to any person obtaining a copy of this   #
# software and associated documentation files (the "Software"), to deal in the Software  #
# without restriction, including without limitation the rights to use, copy, modify,     #
# merge, publish, distribute, sublicense, and/or sell copies of the Software, and to     #
# permit persons to whom the Software is furnished to do so, subject to the following    #
# conditions:                                                                            #
#                                                                                        #
# The above copyright notice and this permission notice shall be included in all copies  #
# or substantial portions of the Software.                                               #
#                                                                                        #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,    #
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A          #
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT     #
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF   #
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE   #
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                          #
##########################################################################################

include(${PROJECT_SOURCE_DIR}/support/cmake/module_definition.cmake)

set(HEADER_FILES
  include/guiactioncomponent.h
  include/guicomponent.h
  include/guifilepathcomponent.h
  include/guigibscomponent.h
  include/guiglobebrowsingcomponent.h
  include/guihelpcomponent.h
  include/guijoystickcomponent.h
  include/guimemorycomponent.h
  include/guimissioncomponent.h
  include/guiparallelcomponent.h
  include/guipropertycomponent.h
  include/guiscenecomponent.h
  include/guispacetimecomponent.h
  include/imgui_include.h
  include/renderproperties.h
)
source_group("Header Files" FILES ${HEADER_FILES})

set(SOURCE_FILES
  src/guiactioncomponent.cpp
  src/guicomponent.cpp
  src/guifilepathcomponent.cpp
  src/guigibscomponent.cpp
  src/guiglobebrowsingcomponent.cpp
  src/guihelpcomponent.cpp
  src/guijoystickcomponent.cpp
  src/guimemorycomponent.cpp
  src/guimissioncomponent.cpp
  src/guiparallelcomponent.cpp
  src/guipropertycomponent.cpp
  src/guiscenecomponent.cpp
  src/guispacetimecomponent.cpp
  src/renderproperties.cpp
)
source_group("Source Files" FILES ${SOURCE_FILES})

set(SHADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/gui_fs.glsl
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/gui_vs.glsl
)
source_group("Shader Files" FILES ${SHADER_FILES})

create_new_module(
  "ImGUI"
  imgui_module
  STATIC
  ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES}
)

add_subdirectory(ext/imgui SYSTEM)
target_link_libraries(${imgui_module} PUBLIC Imgui)
set_target_properties(Imgui PROPERTIES FOLDER "External")
