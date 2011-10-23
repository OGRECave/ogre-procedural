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
#include "Ogre.h"
#include "Illustrations.h"
#include "Procedural.h"

using namespace Procedural;

//-------------------------------------------------------------------------------------
	void Illustrations::init()
	{	
	
	String resourcesCfg, pluginsCfg;
	#ifdef _DEBUG
	resourcesCfg = "resources_d.cfg";
	pluginsCfg = "plugins_d.cfg";
	#else
	resourcesCfg = "resources.cfg";
	pluginsCfg = "plugins.cfg";
	#endif

	mRoot = new Ogre::Root(pluginsCfg); 
  
	ConfigFile cf;
	cf.load(resourcesCfg);
 
	ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}

	if (!mRoot->restoreConfig())
		mRoot->showConfigDialog();
		 
	mWindow = mRoot->initialise(true, ""); 
	mWindow->resize(256,256);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();	
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC);  
	mCamera = mSceneMgr->createCamera("SimpleCamera");  
	mViewPort = mWindow->addViewport(mCamera);
	mViewPort->setBackgroundColour(ColourValue::White);
	mCamera->setAspectRatio(1.);
	mCamera->setPosition(3,5,5);
	mCamera->lookAt(0,0,0);
	mCamera->setNearClipDistance(1.);
	Light* light = mSceneMgr->createLight();
	light->setType(Light::LT_DIRECTIONAL);
	light->setDiffuseColour(ColourValue::White);
	light->setDirection(Vector3(-1,-1,-1).normalisedCopy());
	//camera->projectSphere
}

void Illustrations::next(std::string name)
{
	Entity* e = *mEntities.begin();
	SceneNode* sn = *mSceneNodes.begin();
	Vector3 position = sn->_getDerivedPosition();
	Real radius = e->getBoundingRadius();
	Sphere bSphere(position, radius);
	Real distance = 3*radius/Math::Tan(mCamera->getFOVy());
	mCamera->setPosition(distance * mCamera->getPosition().normalisedCopy());

	mRoot->renderOneFrame();
	mWindow->writeContentsToFile(name + ".png");

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

void Illustrations::putMesh(MeshPtr mesh, int materialIndex)
{
	Entity* ent = mSceneMgr->createEntity(mesh->getName());
	SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent);
	if (materialIndex==0)
		ent->setMaterialName("HiddenLine");
	else if (materialIndex==1)
		ent->setMaterialName("RedLine");
	mEntities.push_back(ent);
	mSceneNodes.push_back(sn);
}


void Illustrations::go()
{	
	MeshPtr mp;
	mp = BoxGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_box");

	mp = RoundedBoxGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_roundedbox");

	mp = SphereGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_sphere");

	mp = IcoSphereGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_icosphere");

	mp = TorusGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_torus");

	mp = TorusKnotGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_torusknot");

	mp = CylinderGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_cylinder");

	mp = ConeGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_cone");

	mp = TubeGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_tube");

	mp = CapsuleGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_capsule");

	mp = CapsuleGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_capsule");

	mp = CatmullRomSpline2().addPoint(0,0).addPoint(1,0).addPoint(1,1).addPoint(2,1).realizeShape().realizeMesh();
	putMesh(mp,1);
	next("spline_catmull");

	mp = CubicHermiteSpline2().addPoint(Vector2(0,0), AT_CATMULL).addPoint(Vector2(1,0), AT_CATMULL).addPoint(Vector2(1,1), Vector2(0,2), Vector2(0,-2)).addPoint(Vector2(2,1), AT_CATMULL).setNumSeg(16).realizeShape().realizeMesh();
	putMesh(mp,1);
	next("spline_cubichermite");

	Shape s1 = RectangleShape().realizeShape();
	Shape s2 = s1;
	s2.translate(.5,.5);
	mp = s1.booleanUnion(s2).realizeMesh();
	putMesh(mp,1);
	next("shape_union");

	mp = s1.booleanIntersect(s2).realizeMesh();
	putMesh(mp,1);
	next("shape_intersection");

	mp = s1.booleanDifference(s2).realizeMesh();
	putMesh(mp,1);
	next("shape_difference");

}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		Illustrations app;

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

#ifdef __cplusplus
}
#endif
