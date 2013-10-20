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

#include "Pipeline.h"

#include "Channel.h"
#include "FrameData.h"
#include "vtkCamera.h"
#include "vtkRenderWindow.h"

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

namespace eqVTK
{

class Pipeline::Impl
{
public:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkCamera> camera;
    vtkSmartPointer<vtkRenderWindow> window;
};

Pipeline::Pipeline()
    : _impl(new Impl)
{
    _impl->renderer = vtkRenderer::New();
    _impl->window = vtkRenderWindow::New();
    _impl->window->AddRenderer(_impl->renderer);
    _impl->camera = vtkCamera::New();
}

Pipeline::~Pipeline()
{
    delete _impl;
}

void Pipeline::drawFrame(Channel &channel, const FrameData &frameData)
{
    const eq::Vector3f &pivotPoint = frameData.getRotationPivot();
    eq::Matrix4f pivot;
    identity(pivot);
    pivot.set_translation(-pivotPoint);
    eq::Matrix4f pivotInverse;
    identity(pivotInverse);
    pivotInverse.set_translation(pivotPoint);

    eq::Matrix4f translation;
    identity(translation);
    translation.set_translation(-frameData.getCameraPosition());

    eq::Matrix4f modelview =
        (frameData.getCameraRotation() * translation *
         pivotInverse * frameData.getModelRotation()) * pivot;

    _impl->camera->setChannel(&channel);
    _impl->camera->setModelview(modelview);
    /* This is neccesary to get lighting correct as we get closer to the
       model */
    _impl->camera->SetFocalPoint(pivotPoint[0], pivotPoint[1], pivotPoint[2]);
    _impl->renderer->SetActiveCamera(_impl->camera);

    eq::Viewport vp = channel.getViewport();
    _impl->renderer->SetViewport(vp.x, vp.y, vp.w, vp.h);

    drawRange(channel.getRange());

    _impl->window->setWindow(channel.getWindow());
    _impl->window->Render();
}

void Pipeline::addActor(vtkActor *actor)
{
    _impl->renderer->AddActor(actor);
}

}
