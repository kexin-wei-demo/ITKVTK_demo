#include <iostream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageReslice.h>
#include <vtkMatrix4x4.h>
#include <vtkNrrdReader.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

#include "DEFINE_PATH.h"
#include <ImageCoordinate.h>
#include <Point.h>

#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

namespace Log = spdlog;
void setUpSpdlog()
{
    try
    {
        Log::set_level(Log::level::debug);
        Log::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
        Log::info("Logging system initialized.");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

int main()
{
    setUpSpdlog();

    auto usFile = THIS_PROJECT_PATH + "data/robot_scan_15-11-02_29-03-2024(2).nrrd";
    auto mriFile = THIS_PROJECT_PATH + "data/SUPAWAT20240211MR_AXT2THIN.nrrd";

    auto reader = vtkSmartPointer<vtkNrrdReader>::New();
    reader->SetFileName(usFile.c_str());
    reader->Update();

    auto usOrigin = reader->GetDataOrigin();
    auto usSpacing = reader->GetDataSpacing();
    auto usBounds = reader->GetOutput()->GetBounds();

    Log::debug("US Origin: ({}, {}, {})", usOrigin[0], usOrigin[1], usOrigin[2]);
    Log::debug("US Spacing: ({}, {}, {})", usSpacing[0], usSpacing[1], usSpacing[2]);
    Log::debug("US Bounds: ({}, {}, {}, {}, {}, {})", usBounds[0], usBounds[1], usBounds[2], usBounds[3], usBounds[4], usBounds[5]);

    std::vector<Point<double>> corners;
    for (int x = 0; x < 2; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            Point<double> corner;
            for (int z = 0; z < 2; z++)
            {
                corner.x = usBounds[x];
                corner.y = usBounds[2 + y];
                corner.z = usBounds[4 + z];
            }
            Log::debug("Corner: {}", corner.toStdString());
        }
    }

    // // Point<int> voxel = { 0, 0, 0 };
    // Point<double> usTestOrigin = { usOrigin[0], usOrigin[1], usOrigin[2] };
    // Point<double> usTestSpacing = { usSpacing[0], usSpacing[1], usSpacing[2] };
    // double usDirection[9] = { 0, 1, 0, 0, 0, -1, -1, 0, 0 };
    // // Point<double> usWorld;
    // // fromVoxelIJKToWorldXYZ(voxel);
    // Point<int> voxel;
    // Point<double> world = { usOrigin[0], usOrigin[1], usOrigin[2] };
    // fromWorldXYZToVoxelIJK(world, usTestOrigin, usTestSpacing, usDirection, voxel);

    // Log::debug("Voxel: ({}, {}, {})", voxel.x, voxel.y, voxel.z);

    // ...

    // Create a vtkImageData object from the reader's output
    vtkSmartPointer<vtkImageData> imageData = reader->GetOutput();

    // Create a vtkImageActor to display the image
    vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
    imageActor->SetInputData(imageData);

    // Create a renderer and add the image actor to it
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(imageActor);

    // Create a render window and set the renderer
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // Create an interactor and set the render window
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    // Initialize the interactor and start the event loop
    interactor->Initialize();
    interactor->Start();

    return 0;
}