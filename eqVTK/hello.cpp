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
#include "Client.h"
#include "Config.h"
#include "Node.h"
#include "Pipe.h"
#include "Pipeline.h"
#include "Window.h"

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>

using namespace eqVTK;

class NodeFactory : public eq::NodeFactory
{
public:
    virtual eq::Config *createConfig(eq::ServerPtr parent)
    {
        return new Config(parent);
    }
    virtual eq::Node *createNode(eq::Config *parent)
    {
        return new Node(parent);
    }
    virtual eq::Pipe *createPipe(eq::Node *parent)
    {
        return new Pipe(parent);
    }
    virtual eq::Window *createWindow(eq::Pipe *parent)
    {
        return new Window(parent);
    }
    virtual eq::Channel *createChannel(eq::Window *parent)
    {
        return new Channel(parent);
    }
};

class Hello : public Client
{
public:
    PipelinePtr createPipeline(const InitData &) const
    {
        return new Pipeline();
    }

    virtual void initConfig(Config &) {}

private:
    /* Private declarations */
    class Pipeline : public eqVTK::Pipeline
    {
    public:
        Pipeline()
        {
            _sphere = vtkSphereSource::New();
            _sphere->SetCenter(0, 0, 0);
            _sphere->SetRadius(1);

            _mapper = vtkPolyDataMapper::New();
            _mapper->SetInputConnection(_sphere->GetOutputPort());

            _actor = vtkActor::New();
            _actor->SetMapper(_mapper);
            _actor->GetProperty()->SetColor(1, 0, 0);

            addActor(_actor);
        }

        virtual void drawRange(const eq::Range &) {}

    private:
        vtkSmartPointer<vtkSphereSource> _sphere;
        vtkSmartPointer<vtkPolyDataMapper> _mapper;
        vtkSmartPointer<vtkActor> _actor;
    };
};

int main(int argc, char *argv[])
{
    NodeFactory factory;

    if (!eq::init(argc, argv, &factory))
    {
        LBERROR << "Equalizer init failed" << std::endl;
        return EXIT_FAILURE;
    }

    InitData initData;

    /* Initializing local client node */
    lunchbox::RefPtr<Hello> app(new Hello());
    if (!app->initLocal(argc, argv))
    {
        LBERROR << "Can't init app" << std::endl;
        eq::exit();
        return EXIT_FAILURE;
    }

    app->run();

    app->exitLocal();
    app = 0;

    eq::exit();

}
