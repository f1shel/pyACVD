/*=========================================================================

Program:   ManifoldSimplification
Module:    vtkSurface
Language:  C++
Date:      2011/06
Author:   Sebastien Valette

=========================================================================*/

/* ---------------------------------------------------------------------

* Copyright (c) CREATIS-LRMN (Centre de Recherche en Imagerie Medicale)
* Author : Sebastien Valette
*
*  This software is governed by the CeCILL-B license under French law and 
*  abiding by the rules of distribution of free software. You can  use, 
*  modify and/ or redistribute the software under the terms of the CeCILL-B 
*  license as circulated by CEA, CNRS and INRIA at the following URL 
*  http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html 
*  or in the file LICENSE.txt.
*
*  As a counterpart to the access to the source code and  rights to copy,
*  modify and redistribute granted by the license, users are provided only
*  with a limited warranty  and the software's author,  the holder of the
*  economic rights,  and the successive licensors  have only  limited
*  liability. 
*
*  The fact that you are presently reading this means that you have had
*  knowledge of the CeCILL-B license and that you accept its terms.
* ------------------------------------------------------------------------ */  

// .NAME ManifoldSimplification
// .SECTION Description

#include <vtkPLYWriter.h>
#include "vtkManifoldSimplification.h"

/////////////////////////////////////////////////////////////////////////////////////////
// ManifoldSimplification program:
/////////////////////////////////////////////////////////////////////////////////////////
// 
// Coarsening of triangular meshes
// This program should be run with 2 arguments:
// run: "ManifoldSimplification file nvertices"
// file is the name of the mesh file to read
// nvertices is the desired number of vertices

int main( int argc, char *argv[] )
{

	//******************************************************************************************
	// Inside input parameters:
	int NumberOfVertices=0;	// the number of desired vertices
	//*******************************************************************************************

	vtkSurface *Mesh=vtkSurface::New();

	if(argc>2)
	{
		cout <<"load : "<<argv[1]<<endl;
		Mesh->CreateFromFile(argv[1]);
		NumberOfVertices=atoi(argv[2]);
	}
	else
	{
		cout<<"Usage : ManifoldSimplification file nvertices"<<endl;
		cout<<"nvertices is the desired number of vertices"<<endl;
		exit (1);
	}

	Mesh->DisplayMeshProperties();

	vtkManifoldSimplification *Simplification=vtkManifoldSimplification::New();
	Simplification->SetInput(Mesh);
	Simplification->SetNumberOfOutputVertices(NumberOfVertices);
	Simplification->Simplify();
	vtkSurface *CleanOutput=Mesh->CleanMemory();

	cout<<"Output mesh:"<<endl;
	CleanOutput->DisplayMeshProperties();

	vtkPLYWriter *Writer=vtkPLYWriter::New();
	Writer->SetInputData(CleanOutput);
	Writer->SetFileName("output.ply");
	Writer->Write();
	Writer->Delete();
	Simplification->Delete();
	Mesh->Delete();
	CleanOutput->Delete();
	return (0);
}
