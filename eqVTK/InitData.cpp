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

#include "InitData.h"

#include <co/dataOStream.h>
#include <co/dataIStream.h>

InitData::InitData()
    : frameDataID()
{}

InitData::~InitData()
{
}

void InitData::getInstanceData(co::DataOStream &os)
{
    os << frameDataID;
}

void InitData::applyInstanceData(co::DataIStream &is)
{
    is >> frameDataID;
    LBASSERT(frameDataID != 0);
}

