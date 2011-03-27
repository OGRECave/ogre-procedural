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

#include "BaseApplication.h"
#include "Procedural.h"
using namespace Ogre;
using namespace Procedural;

class Unit_Test
{	
protected:
	SceneManager* mSceneMgr;

	std::vector<Entity*> mEntities;
	std::vector<SceneNode*> mSceneNodes;
	Ogre::Timer mTimer;

	void putMesh(const String& meshName)
	{
		Entity* ent = mSceneMgr->createEntity(meshName);
		SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sn->attachObject(ent);
		//ent->setMaterialName("Examples/Rockwall");	
		mEntities.push_back(ent);
		mSceneNodes.push_back(sn);
		int currentIndex = mEntities.size();
		sn->setPosition(currentIndex*10.0, 0., (currentIndex%10)*10.0);		
	}

public:
	Unit_Test(SceneManager* sn) : mSceneMgr(sn) {}
	
	virtual String getDescription()=0;
	virtual void initImpl()=0;

	void init()
	{
		Procedural::Utils::log("Loading test : " + getDescription());
		mTimer.reset();
		initImpl();
		Procedural::Utils::log("Test loaded in : " + Ogre::StringConverter::toString(mTimer.getMilliseconds()) + " ms");
	}

	void destroy()
	{
		for (std::vector<SceneNode*>::iterator it = mSceneNodes.begin(); it != mSceneNodes.end(); it++) 
		{
			(*it)->detachAllObjects();
			mSceneMgr->destroySceneNode(*it);
		}
		for (std::vector<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); it++)
		{
			MeshManager::getSingletonPtr()->remove((*it)->getMesh()->getName());			
			mSceneMgr->destroyEntity(*it);
		}		
		mEntities.clear();
		mSceneNodes.clear();
	}
};

class Unit_Tests : public BaseApplication
{
	Light* movingLight;

	/* --------------------------------------------------------------------------- */
	class Test_Primitives : public Unit_Test
	{
	public:		
		Test_Primitives(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Tests all primitive generation";
		}

		void initImpl()
		{
			Procedural::SphereGenerator sg = Procedural::SphereGenerator();
			sg.realizeMesh("sphereMesh");			
			putMesh("sphereMesh");
		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_Empty : public Unit_Test
	{
	public:		
		Test_Empty(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Tests nothing";
		}

		void initImpl()
		{
		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_Triangulation : public Unit_Test
	{
	public:		
		Test_Triangulation(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Tests Delaunay Triangulation";
		}

		void initImpl()
		{
			Procedural::Shape s = Procedural::Shape().addPoint(0,0).addPoint(5,0).addPoint(0,5).close();
			Procedural::Triangulator::triangulateToMesh(s, "contourMesh");
			putMesh("contourMesh");
		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_ShapeGenerators : public Unit_Test
	{
	public:		
		Test_ShapeGenerators(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Tests different types of shape generation";
		}

		void initImpl()
		{
			//CSG
			Procedural::Shape s1 = Procedural::CircleShape().realizeShape();
			Procedural::Shape s2 = Procedural::RectangleShape().setWidth(0.5).setHeight(2).realizeShape();
			
			s1.realizeMesh("contour1");
			putMesh("contour1");
			s2.realizeMesh("contour2");
			putMesh("contour2");
			Procedural::MultiShape s;
			s = s1.booleanIntersect(s2);			
			s.realizeMesh("contourinter");
			putMesh("contourinter");
			s = s1.booleanUnion(s2);
			s.realizeMesh("contourunion");
			putMesh("contourunion");
			s = s1.booleanDifference(s2);
			s.realizeMesh("contourdifference");
			putMesh("contourdifference");
		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_Extruder : public Unit_Test
	{
	public:		
		Test_Extruder(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Tests the extruder";
		}

		void initImpl()
		{
		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_Lathe : public Unit_Test
	{
	public:		
		Test_Lathe(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Tests the lathe";
		}

		void initImpl()
		{
		}
	};



	/* --------------------------------------------------------------------------- */
	std::vector<Unit_Test*> mUnitTests;

	int mCurrentTestIndex;

	void switchToTest(int index)
	{
		if (index == mCurrentTestIndex)
			return;
		mUnitTests[mCurrentTestIndex]->destroy();
		mUnitTests[index]->init();
		mCurrentTestIndex = index;
	}

	void nextTest()
	{
		switchToTest(Procedural::Utils::modulo(mCurrentTestIndex+1,mUnitTests.size()));
	}
	void previousTest()
	{
		switchToTest(Procedural::Utils::modulo(mCurrentTestIndex-1,mUnitTests.size()));
	}

protected:
	bool keyReleased( const OIS::KeyEvent &arg )
	{		
		if (arg.key == OIS::KeyCode::KC_ADD)
		{
			nextTest();
			return true;
		}
		if (arg.key == OIS::KeyCode::KC_SUBTRACT)		
		{
			previousTest();
			return true;
		}
		return BaseApplication::keyReleased(arg);
	}

	virtual void createScene(void);
	
	virtual void createCamera(void);
	
	virtual bool frameStarted(const FrameEvent& evt);
public:
	Unit_Tests() : mCurrentTestIndex(0)
	{}

};

#endif // #ifndef __TutorialApplication_h_
