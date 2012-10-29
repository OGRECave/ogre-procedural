/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2010-2012 Michael Broutin

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

#ifndef LUATESTS_H
#define LUATESTS_H

#include "BaseApplication.h"
#include "Procedural.h"
#include "Ogre.h"
using namespace Ogre;
using namespace Procedural;

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}



class LuaTests : public BaseApplication
{

	Ogre::OverlayElement* mTextMessage;
	/* --------------------------------------------------------------------------- */
	/*void switchToTest(int index)
	{
		if (index == mCurrentTestIndex)
			return;
	
		String test_description = mUnitTests[index]->getDescription();

		// update text here:
		String txt = "[OgreProcedural Unit Tests] (Use key N/M to switch between tests)\n";
		txt += "[" + StringConverter::toString(index+1) + "/" + StringConverter::toString(mUnitTests.size()) + "] ";

		// and add the description
		txt += test_description;

		// properly print the time ...
		char time_str[255] = {0};
		sprintf(time_str, " (loaded in %6.6f ms)", time);
		txt += String(time_str);

		// and finally set it
		OverlayManager::getSingleton().getOverlayElement("myText")->setCaption(txt);


		mCurrentTestIndex = index;
	}*/

	void reloadScript();

protected:

	bool keyReleased( const OIS::KeyEvent &arg )
	{
		if (arg.key == OIS::KC_F5)
		{
			reloadScript();
		}
		return BaseApplication::keyReleased(arg);
	}

	bool keyPressed(const OIS::KeyEvent &arg)
	{
		return BaseApplication::keyPressed(arg);
	}

	virtual void createScene(void);

	virtual void createCamera(void);

	virtual void createViewports(void);

	virtual bool frameStarted(const FrameEvent& evt);

	virtual void destroyScene(void);

	static LuaTests* mInstance;

	std::vector<Entity*> mEntities;

	std::vector<SceneNode*> mSceneNodes;

public:

	void addMesh(const char* meshName, const char* materialName="Examples/Rockwall")
	{
		Entity* entity = mSceneMgr->createEntity(meshName);
		entity->setMaterialName(materialName);
		SceneNode* sceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sceneNode->attachObject(entity);
		mSceneNodes.push_back(sceneNode);
		mEntities.push_back(entity);
	}
		

	static LuaTests* getInstance()
	{
		return mInstance;
	}

	LuaTests()
	{
		mInstance = this;
	}

};

#endif