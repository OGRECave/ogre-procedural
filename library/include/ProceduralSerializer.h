/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2010 Michael Broutin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef PROCEDURALSERIALIZER_H__
#define PROCEDURALSERIALIZER_H__

#include "ProceduralPathGenerators.h"
#include "ProceduralMeshGenerator.h"
#include "ProceduralPlatform.h"

#include "Procedural.h"

namespace Procedural
{

/** Generates a box mesh centered on the origin.
 * Default size is 1.0 with 1 quad per face.
 */
class _ProceduralExport Serializer
{
public:
	/// Contructor with arguments
	Serializer();

	/// Destructor
	~Serializer(void);

	int loadXMLFile(Ogre::String filename, Ogre::String group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);


	/**
	 * Builds a mesh.
	 * @param name of the mesh for the MeshManager
	 * @param group ressource group in which the mesh will be created
	 */
	Ogre::MeshPtr realizeMesh(const std::string& XMLMeshName = "default", const std::string& meshName = "", const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	std::vector<Ogre::MeshPtr> realizeAllMeshes(const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

protected:
	int loadXMLFromString(Ogre::String &str);

	/// overloaded method
	void addToTriangleBuffer(TriangleBuffer& buffer) const;

	/// some internal helper struct
	template<class T>
	struct serializationStorage {
		T obj;
		int realize;

		serializationStorage()
		{
		}

		serializationStorage(T o, int realize) : obj(o), realize(realize)
		{
		}
	};

	// the hashmaps storing the generated objects
	std::map < Ogre::String, serializationStorage<CatmullRomSpline3> > splines;
	std::map < Ogre::String, serializationStorage<Path> >              paths;
	std::map < Ogre::String, serializationStorage<Shape> >             shapes;
	std::map < Ogre::String, serializationStorage<Extruder> >          extruders;

	std::map < Ogre::String, MeshGeneratorInterface*>                  realizeables;
	
};

}
#endif //PROCEDURALSERIALIZER_H__
