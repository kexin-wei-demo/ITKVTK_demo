#pragma once
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

#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkNrrdImageIO.h>
#include <itkNrrdImageIOFactory.h>

namespace Log = spdlog;
void testReadImageDirection()
{
    auto usFile = THIS_PROJECT_PATH + "data/robot_scan_15-11-02_29-03-2024(2).nrrd";
    // auto mriFile = THIS_PROJECT_PATH + "data/SUPAWAT20240211MR_AXT2THIN.nrrd";

    Log::warn("Using VTK reader");
    auto vtkReader = vtkSmartPointer<vtkNrrdReader>::New();
    vtkReader->SetFileName(usFile.c_str());
    vtkReader->Update();
    {
        auto usOrigin = vtkReader->GetDataOrigin();
        auto usSpacing = vtkReader->GetDataSpacing();
        auto usBounds = vtkReader->GetOutput()->GetBounds();
        // auto usDirection = vtkReader->GetOutput()->GetDirection();

        Log::debug("US Origin: ({}, {}, {})", usOrigin[0], usOrigin[1], usOrigin[2]);
        Log::debug("US Spacing: ({}, {}, {})", usSpacing[0], usSpacing[1], usSpacing[2]);
        Log::debug("US Bounds: ({}, {}, {}, {}, {}, {})", usBounds[0], usBounds[1], usBounds[2], usBounds[3], usBounds[4], usBounds[5]);
        // std::string directionStr;
        // for (int i = 0; i < 9; i++)
        // {
        //     directionStr += std::to_string(usDirection[i]) + " ";
        // }
        // Log::debug("US Direction: {}", directionStr);

        // std::vector<Point<double>> corners;
        // for (int x = 0; x < 2; x++)
        // {
        //     for (int y = 0; y < 2; y++)
        //     {
        //         Point<double> corner;
        //         for (int z = 0; z < 2; z++)
        //         {
        //             corner.x = usBounds[x];
        //             corner.y = usBounds[2 + y];
        //             corner.z = usBounds[4 + z];
        //         }
        //         Log::debug("Corner: {}", corner.toStdString());
        //     }
        // }
    }

    itk::NrrdImageIOFactory::RegisterOneFactory();

    auto itkReader = itk::ImageFileReader<itk::Image<float, 3>>::New();
    itkReader->SetFileName(usFile);
    itkReader->Update();
    // auto metadata = itkReader->GetMetaDataDictionary();
    // for (const auto& key : metadata.GetKeys())
    // {
    //     std::string value;
    //     itk::ExposeMetaData<std::string>(metadata, key, value);
    //     Log::debug("Key: {}, Value: {}", key, value);
    // }

    Log::warn("Using ITK to VTK filter");

    auto itkToVtkFilter = itk::ImageToVTKImageFilter<itk::Image<float, 3>>::New();
    itkReader->Update();
    auto usDirection = itkReader->GetOutput()->GetDirection();
    double usDirectionArray[9];
    for (int i = 0; i < 9; i++)
    {
        usDirectionArray[i] = usDirection[i / 3][i % 3];
    }

    Log::debug("US Direction: {}, {}, {}, {}, {}, {}, {}, {}, {}",
        usDirectionArray[0], usDirectionArray[1], usDirectionArray[2],
        usDirectionArray[3], usDirectionArray[4], usDirectionArray[5],
        usDirectionArray[6], usDirectionArray[7], usDirectionArray[8]);

    itkToVtkFilter->SetInput(itkReader->GetOutput());
    itkToVtkFilter->Update();

    {
        auto usOrigin = itkToVtkFilter->GetOutput()->GetOrigin();
        auto usSpacing = itkToVtkFilter->GetOutput()->GetSpacing();
        auto usBounds = itkToVtkFilter->GetOutput()->GetBounds();
        Log::debug("US Origin: ({}, {}, {})", usOrigin[0], usOrigin[1], usOrigin[2]);
        Log::debug("US Spacing: ({}, {}, {})", usSpacing[0], usSpacing[1], usSpacing[2]);
        Log::debug("US Bounds: ({}, {}, {}, {}, {}, {})", usBounds[0], usBounds[1], usBounds[2], usBounds[3], usBounds[4], usBounds[5]);
        std::string directionStr;
    }
}