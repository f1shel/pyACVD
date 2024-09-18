#include "interface.h"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <ctime>
#include <memory>
#include <system_error>
#include <tuple>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

using namespace std;
using namespace Eigen;

#define __ACVDQP__
#define DOmultithread

#define RETURN_EMPTY2 { \
    Matrix<float, -1, -1, RowMajor> a; \
    Matrix<int, -1, -1, RowMajor> b; \
    return std::tie(a, b); }

typedef struct {
    float x, y, z;
} float3;

std::tuple<
    Eigen::Matrix<float, -1, -1, Eigen::RowMajor>,
    Eigen::Matrix<int, -1, -1, Eigen::RowMajor>
>
acvdqp(
    const Eigen::Matrix<float, -1, -1, Eigen::RowMajor>& vertices,
    const Eigen::Matrix<int, -1, -1, Eigen::RowMajor>& faces,
    int numberOfSamples,      // number of desired vertices
    float gradation,          // gamma parameter for simplification (if gamma=0: uniform)
    int subsamplingThreshold, // subsampling threshold
    bool forceManifold,
    int NumberOfThreads
) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (uint32_t i_vertex = 0; i_vertex < vertices.rows(); i_vertex++) {
        auto& vertex = vertices.row(i_vertex);
        points->InsertNextPoint(vertex[0], vertex[1], vertex[2]);
    }

    vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
    for (uint32_t i_face = 0; i_face < faces.rows(); i_face++) {
        vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
        auto& face = faces.row(i_face);
        triangle->GetPointIds()->SetId(0, face[0]);
        triangle->GetPointIds()->SetId(1, face[1]);
        triangle->GetPointIds()->SetId(2, face[2]);
        triangles->InsertNextCell(triangle);
    }

    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetPolys(triangles);

    vtkSurface *mesh = vtkSurface::New();
    mesh->CreateFromPolyData(polyData);
    vtkQIsotropicDiscreteRemeshing *remesh = vtkQIsotropicDiscreteRemeshing::New();

	mesh->GetCellData()->Initialize();
	mesh->GetPointData()->Initialize();
	// mesh->DisplayMeshProperties();

    remesh->SetForceManifold(forceManifold);

    remesh->SetInput( mesh );
    // remesh->SetFileLoadSaveOption( 0 );
    // remesh->SetConsoleOutput( 2 );
    remesh->SetSubsamplingThreshold( subsamplingThreshold );
    remesh->GetMetric()->SetGradation( gradation );
    // remesh->SetDisplay( display );
    remesh->SetUnconstrainedInitialization( 1 );
    remesh->SetNumberOfClusters( numberOfSamples );

    remesh->Remesh();

    vtkSurface *mesh2 = remesh->GetOutput();
    // vtkSmartPointer<vtkPolyData> polydata = mesh2->GetOutput();
    // vtkSmartPointer<vtkPoints> points = polydata->GetPoints();
    std::vector<float3> vertices2{};

    for (vtkIdType i = 0; i < mesh2->GetNumberOfPoints(); ++i)
    {
        double p[3];
        mesh2->GetPoint(i, p);
        vertices2.push_back({p[0], p[1], p[2]});
    }

    vtkSmartPointer<vtkCellArray> polys = mesh2->GetPolys();

    std::vector<uint32_t> faces2{};

    vtkIdType npts;
    const vtkIdType* pts;

    polys->InitTraversal();
    while (polys->GetNextCell(npts, pts)) 
    {
        std::vector<vtkIdType> face;
        for (vtkIdType i = 0; i < npts; ++i)
        {
            faces2.push_back(pts[i]);
        }
    }

    remesh->Delete();
    mesh->Delete();

    Map<Matrix<float, -1, -1, RowMajor>> remesh_vertices(reinterpret_cast<float*>(vertices2.data()), vertices2.size(), 3);
    Map<Matrix<int, -1, -1, RowMajor>> remesh_faces(reinterpret_cast<int*>(faces2.data()), int(faces2.size()/3), 3);
    return std::tie(remesh_vertices, remesh_faces);
}