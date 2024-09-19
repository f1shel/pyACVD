project(ACVD)

cmake_minimum_required(VERSION 3.0.0)

if( POLICY CMP0042 )
  cmake_policy( SET CMP0042 NEW )
endif()

add_subdirectory(ACVD)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
find_package(Eigen3 REQUIRED)
add_subdirectory(pybind11)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_library(acvdtool STATIC 
    pyacvd/interface.h
    pyacvd/acvdqp.cpp
)
target_include_directories(acvdtool PRIVATE ACVD/DiscreteRemeshing ACVD/Common)
target_compile_features(acvdtool PRIVATE cxx_std_17)
target_link_libraries(acvdtool PRIVATE
    Eigen3::Eigen
    vtkSurface
    ${VTK_LIBRARIES}
)

pybind11_add_module(pyacvd module.cpp)
target_link_libraries(pyacvd PRIVATE pybind11::module
  acvdtool
  Eigen3::Eigen
  vtkSurface
  ${VTK_LIBRARIES}
)