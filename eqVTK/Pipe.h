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

#ifndef EQVTK_PIPE_H
#define EQVTK_PIPE_H

#include <eq/client/pipe.h>

#include "FrameData.h"

namespace eqVTK
{

/**
   The representation of GPU device

   Contains the thread local copy of the frame data to use in the thread
   associated with this object
 */
class Pipe : public eq::Pipe
{
public:
    Pipe(eq::Node *parent)
        : eq::Pipe(parent)
    {}

    const FrameData& getFrameData() const
    {
        return _frameData;
    }

protected:
    virtual ~Pipe() {}

    virtual bool configInit(const eq::uint128_t &initID);
    virtual bool configExit();
    virtual void frameStart(const eq::uint128_t &frameID,
                            const uint32_t frameNumber);

private:
    FrameData _frameData;
};

}
#endif
