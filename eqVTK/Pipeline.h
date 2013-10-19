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

#ifndef EQVTK_PIPELINE_H
#define EQVTK_PIPELINE_H

#include <lunchbox/referenced.h>
#include <lunchbox/refPtr.h>
#include <eq/client/types.h>

class vtkActor;

namespace eqVTK
{

class Channel;

class Pipeline : public lunchbox::Referenced
{
public:
    /* Public constructors and destructor */
    Pipeline();

    virtual ~Pipeline();

    /* Public member functions */
    void drawFrame(Channel &channel, const eq::Matrix4f &modelview);

protected:
    virtual void drawRange(const eq::Range &range) = 0;

    void addActor(vtkActor *actor);

private:
    class Impl;
    Impl *_impl;
};

typedef lunchbox::RefPtr<Pipeline> PipelinePtr;

}
#endif
