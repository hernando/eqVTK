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

cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)

project(EqVTK)

###############################################################################
## BUILDYARD BOOTSTRAP

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(Buildyard)
if(BUILDYARD_STOP)
  return()
endif()

###############################################################################
## COMMON OPTIONS

include(Common)

###############################################################################
## EXTERNAL DEPENDENCIES

include(FindPackages)




