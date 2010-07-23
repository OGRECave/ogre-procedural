#ifndef PROCEDURAL_CYLINDER_GENERATOR_INCLUDED
#define PROCEDURAL_CYLINDER_GENERATOR_INCLUDED

#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class CylinderGenerator
{
public:
	Ogre::MeshPtr realizeMesh();
};
}

#endif