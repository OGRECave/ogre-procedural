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
		ent->setMaterialName("Examples/Rockwall");	
		mEntities.push_back(ent);
		mSceneNodes.push_back(sn);
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
			Procedural::SphereGenerator().realizeMesh("sphereMesh");
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
