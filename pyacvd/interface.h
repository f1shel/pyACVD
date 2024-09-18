#include <Eigen/Dense>

#include <iostream>
#include <fstream>
#include <cstdint>
#include <ctime>
#include <memory>
#include <system_error>

#include <sstream>
#include <vtkPLYWriter.h>
#include <vtkSTLWriter.h>
#include <vtkCellData.h>

#include "vtkIsotropicDiscreteRemeshing.h"

std::tuple<
    Eigen::Matrix<float, -1, -1, Eigen::RowMajor>,
    Eigen::Matrix<int, -1, -1, Eigen::RowMajor>
>
acvdqp(
    const Eigen::Matrix<float, -1, -1, Eigen::RowMajor>& vertices,
    const Eigen::Matrix<int, -1, -1, Eigen::RowMajor>& faces,
    int numberOfSamples = 500,    // number of desired vertices
    float gradation = 0,          // gamma parameter for simplification (if gamma=0: uniform)
    int subsamplingThreshold = 10, // subsampling threshold
    bool forceManifold = false,
    int NumberOfThreads = 0
);