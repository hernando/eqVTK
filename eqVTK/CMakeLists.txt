#
# EqVTK
# Copyright (C) 2013 Juan Hernando jhernando@fi.upm.es
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2.1 as published
# by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

add_library(eqVTK SHARED
  Channel.cpp
  Client.cpp
  Config.cpp
  FrameData.cpp
  InitData.cpp
  Node.cpp
  Pipe.cpp
  Pipeline.cpp
  Window.cpp
  vtkRenderWindow.cpp
  vtkCamera.cpp)

add_executable(hello hello.cpp)
target_link_libraries(hello eqVTK ${VTK_LIBRARIES} ${EQUALIZER_LIBRARIES})

#add_executable(isosurface isosurface.cpp)
#target_link_libraries(isosurface eqVTK ${VTK_LIBRARIES} ${EQUALIZER_LIBRARIES})
