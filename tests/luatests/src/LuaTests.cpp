/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://www.ogreprocedural.org

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
#include "LuaTests.h"
#include "Procedural.h"

extern int luaopen_Procedural(lua_State* L); // declare the wrapped module

LuaTests* LuaTests::mInstance = 0;

//-------------------------------------------------------------------------------------
void LuaTests::createScene(void)
{
	
	Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().create("myOverlay");	
	Ogre::OverlayContainer* cont = (Ogre::OverlayContainer*)OverlayManager::getSingleton().createOverlayElement("Panel","myCont");
	o->add2D(cont);
	mTextMessage = OverlayManager::getSingleton().createOverlayElement("TextArea","myText");
	cont->addChild(mTextMessage);
	mTextMessage->setCaption("Ogre program");
	mTextMessage->setParameter("font_name","SdkTrays/Caption");
	o->show();

	// Init first test
	reloadScript();
}
//-------------------------------------------------------------------------------------
void LuaTests::destroyScene(void)
{	
		for (std::vector<Entity*>::iterator entity = mEntities.begin(); entity != mEntities.end(); entity++)
		{
			MeshManager::getSingletonPtr()->remove((*entity)->getMesh()->getName());
			mSceneMgr->destroyEntity(*entity);
		}
		for (std::vector<SceneNode*>::iterator sceneNode = mSceneNodes.begin(); sceneNode != mSceneNodes.end(); sceneNode++)
		{
			mSceneMgr->destroySceneNode(*sceneNode);
		}
		for (std::vector<MaterialPtr>::iterator it = mMaterials.begin(); it!=mMaterials.end(); it++)
		{
			TextureManager::getSingletonPtr()->remove((*it)->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName());
			MaterialManager::getSingletonPtr()->remove((*it)->getName());
		}
		mSceneNodes.clear();
		mEntities.clear();
		mMaterials.clear();
}
//-------------------------------------------------------------------------------------
void LuaTests::reloadScript()
	{
		ConfigFile cf;
		cf.load(mResourcesCfg);
		StringVector sv = cf.getMultiSetting("FileSystem", "Scripts");
		if (sv.size()<1)
			return;
		String path = *sv.begin();

		lua_State *L; 
		L=luaL_newstate();
		luaopen_base(L);	// load basic libs (eg. print)
		luaopen_Procedural(L);	// load the wrappered module

		destroyScene();

		Timer timer;		
		if (luaL_loadfile(L,(path + "/luaTest.lua").c_str())==0)
		{
			timer.reset();
			if (lua_pcall(L,0,0,0) ==0)
			{				
				mTextMessage->setCaption("OK (loaded in " + StringConverter::toString(timer.getMilliseconds()) + " milliseconds)");
				mCamera->getViewport()->setBackgroundColour(ColourValue(0.2f,0.4f,0.2f));
			}
			else
			{
				mTextMessage->setCaption(lua_tostring(L,-1));
				mCamera->getViewport()->setBackgroundColour(ColourValue(0.4f,0.2f,0.2f));
			}
		} else
		{
			mTextMessage->setCaption(lua_tostring(L,-1));
			mCamera->getViewport()->setBackgroundColour(ColourValue(.4f,.2f,.2f));
		}
		lua_close(L);
	}
//-------------------------------------------------------------------------------------
void LuaTests::createCamera(void)
{
	BaseApplication::createCamera();
	
	// Setup camera and light
	mCamera->setNearClipDistance(.5);
	mCamera->setPosition(0,10,-50);
	mCamera->lookAt(0,0,0);
}
//-------------------------------------------------------------------------------------
void LuaTests::createViewports(void)
{
	BaseApplication::createViewports();
	mCamera->getViewport()->setBackgroundColour(ColourValue(0.2f,0.4f,0.2f));
}
//-------------------------------------------------------------------------------------
bool LuaTests::frameStarted(const FrameEvent& evt)
{
	movingLight->setPosition(mCamera->getPosition());
	return true;
}
//-------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		LuaTests app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}
