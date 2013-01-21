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

#ifndef SCRIPTINTERPRETER_H
#define SCRIPTINTERPRETER_H

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

class ScriptInterpreter : public BaseApplication
{
	Ogre::OverlayElement* mTextMessage;
	
	void reloadScript();
	void checkScriptModified();

	enum ScriptSourceMode
	{
		SSM_RESOURCES, SSM_SCRIPTDIR, SSM_SCRIPTFILE
	};

	std::map<std::string, long> mExecutionTimes;

	ScriptSourceMode mScriptSourceMode;

	std::string mScriptFileName;
	std::string mScriptDir;
	bool mBatchMode;
	bool mPerformanceMode;
	bool mWriteToDisk;
	int mCurrentPerformanceIndex;

	void writeEveythingToDisk();

	void writePerformanceFile();

	void reloadScriptNameFromIndex()
	{
		StringVectorPtr scripts = ResourceGroupManager::getSingleton().findResourceNames("Scripts", "*.lua");
		mCurrentScriptName= (*scripts)[mCurrentScriptIndex];
	}

protected:
	bool keyReleased( const OIS::KeyEvent &arg );

	bool keyPressed(const OIS::KeyEvent &arg)
	{
		return BaseApplication::keyPressed(arg);
	}

	virtual void createScene(void);

	virtual void createCamera(void);

	virtual void createViewports(void);

	virtual bool frameStarted(const FrameEvent& evt);

	virtual void destroyScene(void);

	virtual void createLogManager(void);

	static ScriptInterpreter* mInstance;

	std::vector<Entity*> mEntities;
	std::vector<MaterialPtr> mMaterials;
	std::vector<SceneNode*> mSceneNodes;
	std::vector<TexturePtr> mTextures;

	TriangleBuffer mDefaultTriangleBuffer;

	Ogre::String mCurrentScriptName;
	time_t mCurrentScriptReloadTime;
	size_t mCurrentScriptIndex;

public:
	bool processInput(int argc, char *argv[]);

	void addTextureBuffer(const TextureBuffer* tb)
	{
		addTriangleTextureBuffer(&mDefaultTriangleBuffer, tb);
	}

	void addTriangleBuffer(const TriangleBuffer* tb)
	{
		std::string s = Utils::getName();
		tb->transformToMesh(s);
		addMesh(s.c_str());
	}

	void addTriangleTextureBuffer(const TriangleBuffer* tb, const TextureBuffer* texb)
	{
		std::string meshId = Utils::getName();
		std::string texId = Utils::getName();
		std::string matId = Utils::getName();
		tb->transformToMesh(meshId);
		texb->createTexture(texId);
		addMaterial(texId.c_str(), matId.c_str());
		addMesh(meshId.c_str(), matId.c_str());
	}

	void addShape(const Shape* shape)
	{
		std::string meshId = Utils::getName();
		shape->realizeMesh(meshId);
		addMesh(meshId.c_str());
	}

	void addPath(const Path* path)
	{
		std::string meshId = Utils::getName();
		path->realizeMesh(meshId);
		addMesh(meshId.c_str());
	}

	void addMesh(const char* meshName, const char* materialName="Examples/Rockwall")
	{
		Entity* entity = mSceneMgr->createEntity(meshName);
		entity->setMaterialName(materialName);
		SceneNode* sceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sceneNode->attachObject(entity);
		mSceneNodes.push_back(sceneNode);
		mEntities.push_back(entity);
	}

	void addMaterial(const char* textureName, const char* materialName)
	{
		Ogre::MaterialPtr demoMaterial = Ogre::MaterialManager::getSingletonPtr()->create(std::string(materialName), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);			
		demoMaterial->getTechnique(0)->getPass(0)->setDiffuse(Ogre::ColourValue::White);
		demoMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(std::string(textureName));
		mMaterials.push_back(demoMaterial);
		TexturePtr tex = TextureManager::getSingleton().getByName(textureName);
		mTextures.push_back(tex);
	}
		

	static ScriptInterpreter* getInstance()
	{
		return mInstance;
	}

	ScriptInterpreter()
	{
		mInstance = this;
		mCurrentScriptIndex = 0;
		mCurrentScriptReloadTime = 0;
		mPerformanceMode = false;
		mCurrentPerformanceIndex = 0;
		mNonExclusiveMouse = true;
	}

};

#endif
