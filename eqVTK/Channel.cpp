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

#include "Pipe.h"

#include <eq/client/gl.h>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

namespace
{

void identity(eq::Matrix4f &m)
{
    for (int i = 0; i < 4; ++i)
        m.set_row(i, eq::Vector4f(i == 0, i == 1, i == 2, i == 3));
}

}

class Channel::Impl
{
public:
    void createPipeline()
    {
        //std::cout << "createPipeline" << std::endl;
        //sphere = vtkSphereSource::New();
        //sphere->SetCenter(0, 0, 0);
        //sphere->SetRadius(1);
        //
        //mapper = vtkPolyDataMapper::New();
        //mapper->SetInputConnection(sphere->GetOutputPort());
        //
        //actor = vtkActor::New();
        //actor->SetMapper(mapper);
        //actor->GetProperty()->SetColor(1, 0, 0);
        //
        //renderer = vtkRenderer::New();
        //renderer->AddActor(actor);
        //renderer->SetBackground(0.2, 0.3, 0.4);
    }

    void drawFrame(const Channel &, const eq::Matrix4f &)
    {
        //const eq::Vector3ub color = channel.getUniqueColor();
    }

private:
    vtkSmartPointer<vtkSphereSource> sphere;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderer> renderer;
};

Channel::Channel(eq::Window* parent)
    : eq::Channel( parent )
    , _impl(new Impl())
{
}

Channel::~Channel()
{
    delete _impl;
}


bool Channel::configInit(const eq::uint128_t &initID)
{
    if (!eq::Channel::configInit(initID))
        return false;

    _impl->createPipeline();

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

void Channel::frameDraw(const eq::uint128_t &frameID)
{
    eq::Channel::frameDraw(frameID);

    const FrameData &frameData = _getFrameData();
    const eq::Vector3f &position = frameData.getCameraPosition();

    eq::Matrix4f modelview = frameData.getCameraRotation();
    eq::Matrix4f translation;
    identity(translation);
    translation.set_translation(position);
    modelview = frameData.getModelRotation() * translation * modelview;

    std::cout << modelview << std::endl;

    _impl->drawFrame(*this, modelview);
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
