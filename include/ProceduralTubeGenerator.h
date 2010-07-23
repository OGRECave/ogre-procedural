#ifndef PROCEDURAL_TUBE_GENERATOR_INCLUDED
#define PROCEDURAL_TUBE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class TubeGenerator
{
public:
	Ogre::MeshPtr realizeMesh();
};
}
#endif