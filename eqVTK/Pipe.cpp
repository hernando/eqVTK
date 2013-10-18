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

#include "Pipe.h"

#include "Config.h"

bool Pipe::configInit(const eq::uint128_t &initID)
{
    if (!eq::Pipe::configInit(initID))
        return false;

    /* Mapping the frame data distributed object */
    Config *config = static_cast<Config*>(getConfig());
    const InitData& initData = config->getInitData();

    return config->mapObject(&_frameData, initData.frameDataID);
}

bool Pipe::configExit()
{
    eq::Config *config = getConfig();
    config->unmapObject(&_frameData);

    return eq::Pipe::configExit();
}

void Pipe::frameStart(const eq::uint128_t &frameID, const uint32_t frameNumber)
{
    eq::Pipe::frameStart(frameID, frameNumber);
    _frameData.sync(frameID);
}

