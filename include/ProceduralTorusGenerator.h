#ifndef PROCEDURAL_TORUS_GENERATOR_INCLUDED
#define PROCEDURAL_TORUS_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class TorusGenerator
{
public:
	Ogre::MeshPtr realizeMesh();
};
}
#endif