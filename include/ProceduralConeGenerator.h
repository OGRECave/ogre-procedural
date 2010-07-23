#ifndef PROCEDURAL_CONE_GENERATOR_INCLUDED
#define PROCEDURAL_CONE_GENERATOR_INCLUDED

#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class ConeGenerator
{
public:
	Ogre::MeshPtr realizeMesh();
};
}

#endif