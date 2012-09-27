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
#ifndef __Test_h_
#define __Test_h_

#include "Ogre.h"
#include "Procedural.h"
using namespace Ogre;
using namespace Procedural;

class Illustrations
{
	Ogre::Root* mRoot;
	RenderWindow* mWindow;
	Viewport* mViewPort;
	SceneManager* mSceneMgr;
	Camera* mCamera;

	std::vector<Entity*> mEntities;
	std::vector<SceneNode*> mSceneNodes;

	RaySceneQuery* mRaySceneQuery;
	PixelBox* mRenderWindowPixelBox;

	void GetMeshInformation(Entity *entity, size_t &vertex_count, Ogre::Vector3* &vertices, size_t &index_count, Ogre::uint32* &indices, const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale);

	struct SVGPATH
	{
		std::vector<Vector2> points;
		bool closed;
		Real lineWidth;
		Real distance;
		ColourValue color;

		bool operator<(SVGPATH rhs) { return distance > rhs.distance; }
		bool operator>(SVGPATH rhs) { return distance < rhs.distance; }
	};


public:
	Illustrations()
	{
		init();
	}

	void init();
	void go();
	void next(std::string name, Real size, Shape* pShape1 = NULL, Shape* pShape2 = NULL, Path* pPath = NULL);
	void putMesh(MeshPtr mesh, int materialIndex=0);

	void cameraPerspective()
	{
		mCamera->setPosition(3,5,-5);
		mCamera->lookAt(0,0,0);
	}

	void cameraBack()
	{
		mCamera->setPosition(0,0,5);
		mCamera->lookAt(0,0,0);
	}

	void cameraFront()
	{
		mCamera->setPosition(0,0,-5);
		mCamera->lookAt(0,0,0);
	}
	
};

#endif // #ifndef __TutorialApplication_h_
