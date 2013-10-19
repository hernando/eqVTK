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

#ifndef EQVTK_CHANNEL_H
#define EQVTK_CHANNEL_H

#include <eq/client/channel.h>

namespace eqVTK
{

class FrameData;

class Channel : public eq::Channel
{
public:
    /* Public constructor and destructor */

    Channel(eq::Window* parent);

protected:
    /* Protected constructors and destructor */

    virtual ~Channel();

    /* Protected member functions */

    virtual bool configInit(const eq::uint128_t &frameID);
    virtual void frameClear(const eq::uint128_t &frameID);
    virtual void frameDraw(const eq::uint128_t &frameID);
    virtual void frameAssemble(const eq::uint128_t &frameID);
    virtual void frameReadback(const eq::uint128_t &frameID);

private:
    /* Private member fuctions */
    const FrameData &_getFrameData() const;
    class Impl;
    Impl *_impl;
};

}
#endif // EQ_PLY_CHANNEL_H

