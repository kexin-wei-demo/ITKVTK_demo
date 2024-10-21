#pragma once

#include <string>

#include <vtkCamera.h>
#include <vtkCellType.h>
#include <vtkFlyingEdges3D.h>
#include <vtkImageData.h>
#include <vtkNIFTIImageReader.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkTransform.h>

#include <QDir>
#include <QFileInfo>
#include <QString>

#include "DEFINE_PATH.h"
#include <spdlog/spdlog.h>
namespace Log = spdlog;

std::map<int, std::string> VTK_CELL_TYPE = {
    // Linear cells
    { VTK_EMPTY_CELL, "vtk_empty_cell" },
    { VTK_VERTEX, "vtk_vertex" },
    { VTK_POLY_VERTEX, "vtk_poly_vertex" },
    { VTK_LINE, "vtk_line" },
    { VTK_POLY_LINE, "vtk_poly_line" },
    { VTK_TRIANGLE, "vtk_triangle" },
    { VTK_TRIANGLE_STRIP, "vtk_triangle_strip" },
    { VTK_POLYGON, "vtk_polygon" },
    { VTK_PIXEL, "vtk_pixel" },
    { VTK_QUAD, "vtk_quad" },
    { VTK_TETRA, "vtk_tetra" },
    { VTK_VOXEL, "vtk_voxel" },
    { VTK_HEXAHEDRON, "vtk_hexahedron" },
    { VTK_WEDGE, "vtk_wedge" },
    { VTK_PYRAMID, "vtk_pyramid" },
    { VTK_PENTAGONAL_PRISM, "vtk_pentagonal_prism" },
    { VTK_HEXAGONAL_PRISM, "vtk_hexagonal_prism" },
    // Quadratic, isoparametric cells
    { VTK_QUADRATIC_EDGE, "vtk_quadratic_edge" },
    { VTK_QUADRATIC_TRIANGLE, "vtk_quadratic_triangle" },
    { VTK_QUADRATIC_QUAD, "vtk_quadratic_quad" },
    { VTK_QUADRATIC_TETRA, "vtk_quadratic_tetra" },
    { VTK_QUADRATIC_HEXAHEDRON, "vtk_quadratic_hexahedron" },
    { VTK_QUADRATIC_WEDGE, "vtk_quadratic_wedge" },
    { VTK_QUADRATIC_PYRAMID, "vtk_quadratic_pyramid" },
    { VTK_BIQUADRATIC_QUAD, "vtk_biquadratic_quad" },
    { VTK_TRIQUADRATIC_HEXAHEDRON, "vtk_triquadratic_hexahedron" },
    { VTK_QUADRATIC_LINEAR_QUAD, "vtk_quadratic_linear_quad" },
    { VTK_QUADRATIC_LINEAR_WEDGE, "vtk_quadratic_linear_wedge" },
    { VTK_BIQUADRATIC_QUADRATIC_WEDGE, "vtk_biquadratic_quadratic_wedge" },
    { VTK_BIQUADRATIC_QUADRATIC_HEXAHEDRON, "vtk_biquadratic_quadratic_hexahedron" },
    { VTK_BIQUADRATIC_TRIANGLE, "vtk_biquadratic_triangle" },
    // Cubic, isoparametric cell
    { VTK_CUBIC_LINE, "vtk_cubic_line" },
    // Special class of cells
    { VTK_CONVEX_POINT_SET, "vtk_convex_point_set" },
    // Polyhedron cell, composed of polygonal faces
    { VTK_POLYHEDRON, "vtk_polyhedron" },

    // Higher order cells in parametric form
    { VTK_PARAMETRIC_CURVE, "vtk_parametric_curve" },
    { VTK_PARAMETRIC_SURFACE, "vtk_parametric_surface" },
    { VTK_PARAMETRIC_TRI_SURFACE, "vtk_parametric_tri_surface" },
    { VTK_PARAMETRIC_QUAD_SURFACE, "vtk_parametric_quad_surface" },
    { VTK_PARAMETRIC_TETRA_REGION, "vtk_parametric_tetra_region" },
    { VTK_PARAMETRIC_HEX_REGION, "vtk_parametric_hex_region" },

    // Higher order cells
    { VTK_HIGHER_ORDER_EDGE, "vtk_higher_order_edge" },
    { VTK_HIGHER_ORDER_TRIANGLE, "vtk_higher_order_triangle" },
    { VTK_HIGHER_ORDER_QUAD, "vtk_higher_order_quad" },
    { VTK_HIGHER_ORDER_POLYGON, "vtk_higher_order_polygon" },
    { VTK_HIGHER_ORDER_TETRAHEDRON, "vtk_higher_order_tetrahedron" },
    { VTK_HIGHER_ORDER_WEDGE, "vtk_higher_order_wedge" },
    { VTK_HIGHER_ORDER_PYRAMID, "vtk_higher_order_pyramid" },
    { VTK_HIGHER_ORDER_HEXAHEDRON, "vtk_higher_order_hexahedron" },

    // Arbitrary order Lagrange elements (formulated separated from
    { VTK_LAGRANGE_CURVE, "vtk_lagrange_curve" },
    { VTK_LAGRANGE_TRIANGLE, "vtk_lagrange_triangle" },
    { VTK_LAGRANGE_QUADRILATERAL, "vtk_lagrange_quadrilateral" },
    { VTK_LAGRANGE_TETRAHEDRON, "vtk_lagrange_tetrahedron" },
    { VTK_LAGRANGE_HEXAHEDRON, "vtk_lagrange_hexahedron" },
    { VTK_LAGRANGE_WEDGE, "vtk_lagrange_wedge" },
    { VTK_LAGRANGE_PYRAMID, "vtk_lagrange_pyramid" },

    { VTK_NUMBER_OF_CELL_TYPES, "vtk_number_of_cell_types" }

};

std::vector<std::string> BACKGROUND_COLORS = { "AliceBlue", "GhostWhite", "WhiteSmoke" };
std::vector<std::string> POLYDATA_COLORS = { "DeepPink", "DarkMagenta", "ForestGreen" };

void getImagePath(std::string& originImagePath, std::string& newImagePath)
{
    std::string projectPath = THIS_PROJECT_PATH;
    std::string dataPath = QString::fromStdString(projectPath).append("/data/").toStdString();
    std::string originImageName = "segmented.nii";
    std::string newImageName = "segmented_new.nii.gz";

    originImagePath = dataPath + originImageName;
    newImagePath = dataPath + newImageName;
}

void extractSurface(const QString& imagePath, vtkSmartPointer<vtkPolyData>& surfacePolyData)
{
    auto reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
    reader->SetFileName(imagePath.toStdString().c_str());
    try
    {
        reader->Update();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    auto flyingEdges = vtkSmartPointer<vtkFlyingEdges3D>::New();
    flyingEdges->SetInputConnection(reader->GetOutputPort());
    flyingEdges->SetValue(0, 0.5);
    try
    {
        flyingEdges->Update();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    surfacePolyData = flyingEdges->GetOutput();
}

void printPolyData(vtkSmartPointer<vtkPolyData>& polyData)
{
    auto numberOfPoints = polyData->GetNumberOfPoints();
    auto numberOfCells = polyData->GetNumberOfCells();
    auto numberOfPolys = polyData->GetNumberOfPolys();
    auto numberOfStrips = polyData->GetNumberOfStrips();
    auto numberOfLines = polyData->GetNumberOfLines();
    auto numberOfVerts = polyData->GetNumberOfVerts();

    Log::debug("Number of points: {}", numberOfPoints);
    Log::debug("Number of cells: {}", numberOfCells);
    Log::debug("Number of polys: {}", numberOfPolys);
    Log::debug("Number of strips: {}", numberOfStrips);
    Log::debug("Number of lines: {}", numberOfLines);
    Log::debug("Number of verts: {}", numberOfVerts);
}

void printCellsType(vtkSmartPointer<vtkPolyData>& polyData)
{
    auto numberOfCells = polyData->GetNumberOfCells();
    std::vector<int> recordedCellType;

    for (vtkIdType i = 0; i < numberOfCells; i++)
    {
        auto cellType = polyData->GetCellType(i);
        if (std::find(recordedCellType.begin(), recordedCellType.end(), cellType) == recordedCellType.end())
        {
            recordedCellType.push_back(cellType);
            Log::debug("Cell type: {}", VTK_CELL_TYPE.at(cellType));
        }
    }
}

void copyPolyData(vtkSmartPointer<vtkPolyData>& sourcePolyData, vtkSmartPointer<vtkPolyData>& targetPolyData)
{
    // // Copy the points and point data
    // auto sourcePoints = sourcePolyData->GetPoints();
    // auto targetPoints = vtkSmartPointer<vtkPoints>::New();
    // targetPoints->DeepCopy(sourcePoints);
    // targetPolyData->SetPoints(targetPoints);

    targetPolyData->CopyStructure(sourcePolyData);
}

void testWriteVtkImageData()
{
    std::string originImagePath, newImagePath;
    getImagePath(originImagePath, newImagePath);

    std::string referenceString = "Reference polydata";
    Log::info("============= {}", referenceString);
    auto referencePolyData = vtkSmartPointer<vtkPolyData>::New();
    extractSurface(QString::fromStdString(originImagePath), referencePolyData);
    printPolyData(referencePolyData);

    std::string originalString = "Original polydata";
    Log::info("============= {}", originalString);
    auto originPolyData = vtkSmartPointer<vtkPolyData>::New();
    extractSurface(QString::fromStdString(originImagePath), originPolyData);
    printPolyData(originPolyData);
    printCellsType(originPolyData);

    std::string newString = "New polydata";
    Log::info("============= {}", newString);
    auto newPolyData = vtkSmartPointer<vtkPolyData>::New();
    extractSurface(QString::fromStdString(newImagePath), newPolyData);
    printPolyData(newPolyData);
    printCellsType(newPolyData);

    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkRenderWindow> renderWindow;
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    std::vector<vtkSmartPointer<vtkPolyData>> polyDataList = { referencePolyData, originPolyData, newPolyData };

    copyPolyData(newPolyData, originPolyData);
    Log::info("============= {}", "after editing");
    printPolyData(originPolyData);
    printCellsType(originPolyData);

    for (int i = 0; i < 3; i++)
    {
        vtkNew<vtkRenderer> renderer;
        renderWindow->AddRenderer(renderer);

        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputData(polyDataList[i]);

        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(colors->GetColor3d(POLYDATA_COLORS[i]).GetData());
        actor->GetProperty()->SetPointSize(20);

        renderer->SetViewport(i * 0.33, 0, (i + 1) * 0.33, 1);
        renderer->AddActor(actor);
        renderer->SetBackground(colors->GetColor3d(BACKGROUND_COLORS[i]).GetData());

        renderer->ResetCamera();
    }

    renderWindow->Render();
    renderWindow->SetSize(900, 300);
    renderWindowInteractor->Start();
}
