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

#include "Channel.h"
#include "vtkCamera.h"
#include "vtkTransform.h"

#include <GL/gl.h>

#include <cassert>

namespace eqVTK
{

vtkCamera *vtkCamera::New()
{
    return new vtkCamera();
}

void vtkCamera::setChannel(Channel *channel)
{
    _channel = channel;
}

void vtkCamera::setModelview(const eq::Matrix4f &matrix)
{
    _modelview = matrix;
}

void vtkCamera::Render(vtkRenderer *)
{
    assert(_channel);
    _channel->applyBuffer();
    glEnable(GL_SCISSOR_TEST);
    _channel->applyViewport();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    _channel->applyFrustum();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); /* It seems that VTK is doing a pop somewher. If removed
                       a stack underflow will occur. */

    eq::Matrix4f m;
    if (_channel->useOrtho())
        m = _channel->getOrthoTransform() * _modelview;
    else
        m = _channel->getPerspectiveTransform() * _modelview;

    glLoadMatrixf(m.array);
}

void vtkCamera::UpdateViewport(vtkRenderer *)
{
    assert(_channel);
    glEnable(GL_SCISSOR_TEST);
    _channel->applyViewport();
}

}
