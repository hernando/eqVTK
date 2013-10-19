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

#include "paths.h"

#include <vtkActor.h>
#include <vtkContourFilter.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetMapper.h>
#include <vtkOutlineFilter.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
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

private:
    /* Private declarations */
    class Pipeline : public eqVTK::Pipeline
    {
    public:
        Pipeline();

        virtual void drawRange(const eq::Range &)
        {
        }

    private:
        vtkSmartPointer<vtkActor> _outline;
        vtkSmartPointer<vtkActor> _isosurface;
    };
};

Hello::Pipeline::Pipeline()
{
    vtkSmartPointer<vtkDataSetReader> reader = vtkDataSetReader::New();
    reader->SetFileName((paths::getDataPath() + "/noise.vtk").c_str());

    vtkSmartPointer<vtkOutlineFilter> outliner = vtkOutlineFilter::New();
    outliner->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(outliner->GetOutputPort());

    _outline = vtkActor::New();
    _outline->SetMapper(outlineMapper);
    addActor(_outline);

    vtkSmartPointer<vtkContourFilter> contour = vtkContourFilter::New();
    contour->SetInputConnection(reader->GetOutputPort());
    contour->SetNumberOfContours(3);
    contour->SetValue(0, 1.5);
    contour->SetValue(1, 3.0);
    contour->SetValue(2, 4.5);

    vtkSmartPointer<vtkDataSetMapper> mapper = vtkDataSetMapper::New();
    mapper->Print(std::cout);
    mapper->SetInputConnection(contour->GetOutputPort());

    reader->Update();
    double range[2] = {reader->GetOutput()->GetScalarRange()[0],
                       reader->GetOutput()->GetScalarRange()[1]};
    mapper->SetScalarRange(range);

    vtkSmartPointer<vtkColorTransferFunction> transferFunction =
        vtkColorTransferFunction::New();
    transferFunction->AddRGBPoint(range[0], 1, 0, 0);
    transferFunction->AddRGBPoint(range[1], 0, 0, 1);
    mapper->SetLookupTable(transferFunction);

    _isosurface = vtkActor::New();
    _isosurface->SetMapper(mapper);
    addActor(_isosurface);
}

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
