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
#include <vtkCutter.h>
#include <vtkClipPolyData.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetMapper.h>
#include <vtkExtractVOI.h>
#include <vtkInformation.h>
#include <vtkOutlineFilter.h>
#include <vtkPlane.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkSmartPointer.h>
#include <vtkStreamingDemandDrivenPipeline.h>

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

class Isosurface : public Client
{
public:
    Isosurface()
        : _dataFilename(paths::getDataPath() + "/ironProt.vtk")
    {
    }

    virtual bool initLocal(const int argc, char** argv )
    {
        if (argc > 1 && argv[1][0] != '-')
            _dataFilename = argv[1];

        return Client::initLocal(argc, argv);
    }

    PipelinePtr createPipeline(const InitData &) const
    {
        return new Pipeline(_dataFilename);
    }

    virtual void initConfig(Config &config);

private:
    /* Private declarations */
    class Pipeline : public eqVTK::Pipeline
    {
    public:
        Pipeline(const std::string &dataset);

        virtual void drawRange(const eq::Range &range);

    private:
        eq::Range _range;
        vtkSmartPointer<vtkDataSetReader> _reader;;
        vtkSmartPointer<vtkActor> _outline;
        vtkSmartPointer<vtkExtractVOI> _subsetter;
        vtkSmartPointer<vtkDataSetMapper> _isosurfaceMapper;
        vtkSmartPointer<vtkActor> _isosurface;
    };

    std::string _dataFilename;
};

Isosurface::Pipeline::Pipeline(const std::string &dataset)
    : _range(0, 0)
{
    _reader = vtkDataSetReader::New();
    _reader->SetFileName(dataset.c_str());

    vtkSmartPointer<vtkOutlineFilter> outliner = vtkOutlineFilter::New();
    outliner->SetInputConnection(_reader->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(outliner->GetOutputPort());

    _outline = vtkActor::New();
    _outline->SetMapper(outlineMapper);
    addActor(_outline);

    _subsetter = vtkExtractVOI::New();
    _subsetter->SetInputConnection(_reader->GetOutputPort());

    //vtkSmartPointer<vtkCutter> cutter = vtkCutter::New();
    //vtkSmartPointer<vtkPlane> plane = vtkPlane::New();
    //plane->SetNormal(1, 1, 1);
    //plane->SetOrigin(32, 32, 32);
    //cutter->SetCutFunction(plane);
    //cutter->SetInputConnection(_subsetter->GetOutputPort());

    vtkSmartPointer<vtkContourFilter> contour = vtkContourFilter::New();
    //contour->SetInputConnection(_reader->GetOutputPort());
    contour->SetInputConnection(_subsetter->GetOutputPort());
    //contour->SetInputConnection(cutter->GetOutputPort());

    contour->SetNumberOfContours(1);
    contour->SetValue(0, 3.0);
    contour->SetValue(1, 64.0);
    contour->SetValue(2, 127.0);

    //contour->SetNumberOfContours(3);
    //contour->SetValue(0, 1.5);
    //contour->SetValue(1, 3.0);
    //contour->SetValue(2, 4.5);

    vtkSmartPointer<vtkClipPolyData> clipper = vtkClipPolyData::New();
    vtkSmartPointer<vtkPlane> plane = vtkPlane::New();
    plane->SetNormal(1, 1, 1);
    plane->SetOrigin(32, 32, 32);
    clipper->SetClipFunction(plane);
    clipper->SetInputConnection(contour->GetOutputPort());

    _isosurfaceMapper = vtkDataSetMapper::New();
    _isosurfaceMapper->SetInputConnection(clipper->GetOutputPort());

    _isosurface = vtkActor::New();
    _isosurface->SetMapper(_isosurfaceMapper);
    addActor(_isosurface);
}

void Isosurface::Pipeline::drawRange(const eq::Range &range)
{
    if (range == _range)
        return;
    if (_range != eq::Range(0, 0))
        std::cerr << "Dynamic load balancing not supported" << std::endl;
    _range = range;

    /* Reading the meta data of the data set to know the whole extent */
    _reader->UpdateInformation();

    vtkInformation *info = _reader->GetOutputInformation(0);
    int extent[6];
    info->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
    eq::Vector3f min(extent[0], extent[2], extent[4]);
    eq::Vector3f max(extent[1], extent[3], extent[5]);
    eq::Vector3f counts = max - min + 1;
    /* This calculation hasn't be verified in depth. */
    _subsetter->SetVOI(
        min[0] + int(counts[0] * range.start) - 1,
        min[0] + int(counts[0] * range.end) - 1,
        min[1], max[1], min[2], max[2]);

    /* Applying color map taking into account the full scalar range of
       the dataset */
    _reader->Update();
    double scalarRange[2] = {_reader->GetOutput()->GetScalarRange()[0],
                             _reader->GetOutput()->GetScalarRange()[1]};
    _isosurfaceMapper->SetScalarRange(scalarRange);

    vtkSmartPointer<vtkColorTransferFunction> transferFunction =
        vtkColorTransferFunction::New();
    transferFunction->AddRGBPoint(scalarRange[0], 1, 0, 0);
    transferFunction->AddRGBPoint(scalarRange[1], 0, 0, 1);
    _isosurfaceMapper->SetLookupTable(transferFunction);
}

void Isosurface::initConfig(Config &config)
{
    FrameData &frameData = config.getFrameData();

    /* Reading just the meta data of the data set to know the data bouds */
    vtkSmartPointer<vtkDataSetReader> reader = vtkDataSetReader::New();
    reader->SetFileName(_dataFilename.c_str());

    // This only works only with Update not UpdateInformation. Isn't Update
    // readering the whole dataset?
    //reader->Update();
    //vtkInformation *info = reader->GetOutputInformation(0);
    //info->Print(std::cout);
    //vtkDataSet *dataset = vtkDataSet::SafeDownCast(
    //    info->Get(vtkDataObject::DATA_OBJECT()));
    //double *box = dataset->GetBounds();

    // And this is not generic enough. Works for ironProt.vtk but not for
    // noise.vtk
    reader->UpdateInformation();
    vtkInformation *info = reader->GetOutputInformation(0);
    int box[6];
    info->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), box);
    eq::Vector3f min(box[0], box[2], box[4]);
    eq::Vector3f max(box[1], box[3], box[5]);

    eq::Vector3f pivot = (min + max) * 0.5;
    frameData.setRotationPivot(pivot);

    /* Assuming a vertical field of view of 22.5º */
    float distance =
        (max[1] - min[1]) * 0.8 / atan(M_PI / 4) + (max[2] - min[2]) * 0.5;
    frameData.setCameraPosition(pivot + eq::Vector3f(0, 0, distance));
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
    lunchbox::RefPtr<Isosurface> app(new Isosurface());
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
