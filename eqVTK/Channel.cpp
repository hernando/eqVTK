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

#include "eq/GL/glew.h"

#include "Channel.h"
#include "Config.h"
#include "Client.h"
#include "Pipe.h"

#include <eq/client/gl.h>


namespace eqVTK
{

namespace
{

void identity(eq::Matrix4f &m)
{
    for (int i = 0; i < 4; ++i)
        m.set_row(i, eq::Vector4f(i == 0, i == 1, i == 2, i == 3));
}

}

Channel::Channel(eq::Window* parent)
    : eq::Channel( parent )
{
}

Channel::~Channel()
{
}


bool Channel::configInit(const eq::uint128_t &initID)
{
    if (!eq::Channel::configInit(initID))
        return false;

    Config *config = static_cast<Config*>(getConfig());
    Client *client = static_cast<Client*>(config->getClient().get());
    _pipeline = client->createPipeline(config->getInitData());

    return true;
}

void Channel::frameClear(const eq::uint128_t &)
{
    applyBuffer();
    applyViewport();

#ifndef NDEBUG
    if( getenv( "EQ_TAINT_CHANNELS" ))
    {
        const eq::Vector3ub color = getUniqueColor();
        glClearColor(color.r() / 255.0, color.g() / 255.0,
                     color.b() / 255.0, 0.0);
    }
#endif // NDEBUG
    else
        glClearColor(0, 0, 0, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Channel::frameDraw(const eq::uint128_t &)
{
    const FrameData &frameData = _getFrameData();

    eq::Matrix4f pivot;
    identity(pivot);
    pivot.set_translation(-frameData.getRotationPivot());
    eq::Matrix4f pivotInverse;
    identity(pivotInverse);
    pivotInverse.set_translation(frameData.getRotationPivot());

    eq::Matrix4f translation;
    identity(translation);
    translation.set_translation(frameData.getCameraPosition());

    eq::Matrix4f modelview =
        (frameData.getCameraRotation() * translation *
         pivotInverse * frameData.getModelRotation()) * pivot;

    _pipeline->drawFrame(*this, modelview);
}

void Channel::frameAssemble(const eq::uint128_t &frameID)
{
    eq::Channel::frameAssemble(frameID);
}

void Channel::frameReadback(const eq::uint128_t &frameID)
{
    eq::Channel::frameReadback(frameID);
}

const FrameData& Channel::_getFrameData() const
{
    const Pipe* pipe = static_cast<const Pipe*>(getPipe());
    return pipe->getFrameData();
}

}
