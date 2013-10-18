/*
 * eqVTK
 * Copyright (C) 2013 Juan Hernando jhernando@fi.upm.es
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef EQVTK_NODE_H
#define EQVTK_NODE_H

#include "InitData.h"

#include <eq/eq.h>

/**
   Representation of a node in the cluster

   Manages node-specific data, namely requesting the mapping of the
   initialization data by the local Config instance.
 */
class Node : public eq::Node
{
public:
    Node(eq::Config* parent) : eq::Node( parent ) {}

protected:
    virtual ~Node() {}

    virtual bool configInit(const eq::uint128_t &initID);
};

#endif
