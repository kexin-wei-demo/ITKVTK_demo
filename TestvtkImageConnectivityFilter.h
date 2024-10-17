#pragma once

#include <vtkIdTypeArray.h>
#include <vtkImageConnectivityFilter.h>
#include <vtkIntArray.h>
#include <vtkNIFTIImageReader.h>
#include <vtkSmartPointer.h>

#include "DEFINE_PATH.h"
#include <spdlog/spdlog.h>
namespace Log = spdlog;

void testvtkImageConnectivityFilter()
{
    Log::debug("start testvtkImageConnectivityFilter");
    auto labelImagePath = THIS_PROJECT_PATH + "data/segmented.nii";
    auto reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
    reader->SetFileName(labelImagePath.c_str());
    reader->Update();
    auto size = reader->GetOutput()->GetDimensions();
    Log::debug("Image size: ({}, {}, {}), number of voxels: {}", size[0], size[1], size[2], size[0] * size[1] * size[2]);

    Log::debug("Image read successfully");
    auto connectivityFilter = vtkSmartPointer<vtkImageConnectivityFilter>::New();
    connectivityFilter->SetInputData(reader->GetOutput());
    connectivityFilter->SetExtractionModeToAllRegions();
    connectivityFilter->SetScalarRange(2, 3);
    auto scalarRange = connectivityFilter->GetScalarRange();
    Log::debug("Scalar range: ({}, {})", scalarRange[0], scalarRange[1]);
    connectivityFilter->Update();

    auto numRegions = connectivityFilter->GetNumberOfExtractedRegions();
    auto labelArray = connectivityFilter->GetExtractedRegionLabels();
    auto seedId = connectivityFilter->GetExtractedRegionSeedIds();
    auto extentArray = connectivityFilter->GetExtractedRegionExtents();
    auto sizeArray = connectivityFilter->GetExtractedRegionSizes();

    Log::info("Number of regions: {}", numRegions);
    for (int i = 0; i < numRegions; i++)
    {
        Log::info("Region {} - Label: {}, Seed: {}, Size:{} ",
            i,
            labelArray->GetValue(i),
            seedId->GetValue(i),
            sizeArray->GetValue(i));

        if (connectivityFilter->GetGenerateRegionExtents())
        {
            for (int i = 0; i < numRegions; i++)
            {
                Log::info("Region {} - Label: {}, Extent: ({}, {}, {}, {}, {}, {})", i, labelArray->GetValue(i),
                    extentArray->GetValue(6 * i), extentArray->GetValue(6 * i + 1), extentArray->GetValue(6 * i + 2),
                    extentArray->GetValue(6 * i + 3), extentArray->GetValue(6 * i + 4), extentArray->GetValue(6 * i + 5));
            }
        }
    }
}