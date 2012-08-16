/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2011 Michael Broutin

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
#include "ProceduralStableHeaders.h"
#include "OGRE/Ogre.h"
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
	/*
	if (!mRoot->restoreConfig())
		mRoot->showConfigDialog();*/
	const RenderSystemList& rsList = mRoot->getAvailableRenderers();
	RenderSystem* rs = *rsList.begin();
	for (RenderSystemList::const_iterator it=rsList.begin();it!=rsList.end();it++)
	{
		if ((*it)->getName().find("GL") != String::npos)
			rs = *it;
	}

	ConfigOptionMap optionMap = rs->getConfigOptions();
	String fsaaSetting = optionMap["FSAA"].possibleValues.back();
	rs->setConfigOption("FSAA", fsaaSetting);

	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");	
	
	mRoot->setRenderSystem(rs);	
		 
	mWindow = mRoot->initialise(true); 
	mWindow->setDeactivateOnFocusChange(false);
	mWindow->resize(256,256);
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();	
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC);  
	mCamera = mSceneMgr->createCamera("SimpleCamera");  
	mViewPort = mWindow->addViewport(mCamera);
	mViewPort->setBackgroundColour(ColourValue::White);
	mCamera->setAspectRatio(1.);
	cameraPerspective();
	mCamera->setNearClipDistance(1.);
	mSceneMgr->setAmbientLight(ColourValue(0.5f,0.3f,0.1f));
	Light* light = mSceneMgr->createLight();
	light->setType(Light::LT_DIRECTIONAL);
	light->setDiffuseColour(ColourValue::White);
	light->setDirection(Vector3(-1,-1,-1).normalisedCopy());
}

void Illustrations::next(std::string name, Real size)
{
	// Optimise camera placing
	Real distance = 2*size/Math::Tan(mCamera->getFOVy());
	mCamera->setPosition(distance * mCamera->getPosition().normalisedCopy());

	// Write scene to png image
	mRoot->renderOneFrame();
	mRoot->renderOneFrame();
	mRoot->renderOneFrame();
	mWindow->writeContentsToFile(name + ".png");

	// Clear the scene
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
	else if (materialIndex==2)
		ent->setMaterialName("Examples/Road");
	mEntities.push_back(ent);
	mSceneNodes.push_back(sn);
}

void Illustrations::go()
{	
	//
	// Primitives
	//

	MeshPtr mp;
	mp = BoxGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_box", 1.1f);

	mp = RoundedBoxGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_roundedbox", 1.3f);

	mp = SphereGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_sphere", 1.4f);

	mp = IcoSphereGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_icosphere", 1.4f);

	mp = TorusGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_torus", 1.6f);

	mp = TorusKnotGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_torusknot", 1.6f);

	mp = CylinderGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_cylinder", 1.8f);

	mp = ConeGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_cone", 1.4f);

	mp = TubeGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_tube", 3);

	mp = CapsuleGenerator().realizeMesh();
	putMesh(mp);
	next("primitive_capsule", 2);
	//
	// Operations on shapes and splines
	//

	cameraBack();

	mp = CatmullRomSpline2().addPoint(0,0).addPoint(1,0).addPoint(1,1).addPoint(2,1).addPoint(2,0).addPoint(3,0).addPoint(3,1).addPoint(4,1).realizeShape().translate(-2, 0).realizeMesh();
	putMesh(mp,1);
	next("spline_catmull", 3);

	mp = CubicHermiteSpline2().addPoint(Vector2(0,0), AT_CATMULL).addPoint(Vector2(1,0), AT_CATMULL).addPoint(Vector2(1,1), Vector2(0,2), Vector2(0,-2)).addPoint(Vector2(2,1), AT_CATMULL).addPoint(2,0).addPoint(3,0).addPoint(3,1).addPoint(4,1).setNumSeg(16).realizeShape().translate(-2,0).realizeMesh();
	putMesh(mp,1);
	next("spline_cubichermite", 3);

	mp = KochanekBartelsSpline2().addPoint(Vector2(0,0)).addPoint(Vector2(1,0),1,0,0).addPoint(Vector2(1,1),-1,0,0).addPoint(Vector2(2,1),0,1,0).addPoint(Vector2(2,0),0,-1,0).addPoint(Vector2(3,0),0,0,1).addPoint(Vector2(3,1),0,0,-1).addPoint(Vector2(4,1)).addPoint(Vector2(4,0)).realizeShape().translate(-2,0).realizeMesh();
	putMesh(mp,1);
	next("spline_kochanekbartels", 3);

	mp = RoundedCornerSpline2().addPoint(Vector2(0,0)).addPoint(Vector2(1,0)).addPoint(Vector2(1,1)).addPoint(Vector2(2,1)).addPoint(Vector2(2,0)).addPoint(Vector2(3,0)).addPoint(Vector2(3,1)).addPoint(Vector2(4,1)).addPoint(Vector2(4,0)).setRadius(0.3f).realizeShape().translate(-2,0).realizeMesh();
	putMesh(mp,1);
	next("spline_roundedcorner", 3);

	//
	// Boolean operations
	//
	{
	Shape s1 = RectangleShape().realizeShape();
	Shape s2 = s1;
	s2.translate(.5f,.5f);

	putMesh(s1.realizeMesh(), 1);
	putMesh(s2.realizeMesh(), 1);
	next("shape_booleansetup", 1.5);

	mp = s1.booleanUnion(s2).realizeMesh();
	putMesh(mp,1);
	next("shape_booleanunion", 1.5f);

	mp = s1.booleanIntersect(s2).realizeMesh();
	putMesh(mp,1);
	next("shape_booleanintersection", 1.5f);

	mp = s1.booleanDifference(s2).realizeMesh();
	putMesh(mp,1);
	next("shape_booleandifference", 1.5f);
	}

	//
	// Thicken
	//
	{
	Shape s;
	mp = s.addPoint(-1,-1).addPoint(0.5,0).addPoint(-0.5,0).addPoint(1,1).realizeMesh();
	putMesh(mp,1);
	next("shape_thick1", 1.5f);

	mp = s.thicken(.2f).realizeMesh();
	putMesh(mp,1);
	next("shape_thick2", 1.5f);
	}

	//
	// Delaunay
	//
	{
		cameraFront();
		MultiShape ms;
		CircleShape cs;
		ms.addShape(cs.setRadius(2).realizeShape());
		ms.addShape(cs.setRadius(.3f).realizeShape().translate(-1,.3f).switchSide());
		ms.addShape(cs.realizeShape().translate(1,.3f).switchSide());
		ms.addShape(cs.realizeShape().switchSide());
		ms.addShape(cs.realizeShape().scale(2,1).translate(0,-1).switchSide());
		mp = Triangulator().setMultiShapeToTriangulate(&ms).realizeMesh();
		putMesh(mp);
		next("shape_triangulation", 3);
	}

	//
	// Extrusion
	//
	{
		cameraPerspective();
		//Shape s = RoundedCornerSpline2().addPoint(-1,0).addPoint(0,1).addPoint(1,0).addPoint(0,2).close().realizeShape();
		//Shape s = CircleShape().realizeShape();
		Shape s = Shape().addPoint(-1,-1).addPoint(1,-1).addPoint(1,1).addPoint(0,0).addPoint(-1,1).close();
		Path p = RoundedCornerSpline3().addPoint(-6,2.5,-2.5).addPoint(-5,0,-2.5).addPoint(0,0,2.5).addPoint(5,0,-2.5).setRadius(1.).realizePath();
		mp = Extruder().setShapeToExtrude(&s).setExtrusionPath(&p).realizeMesh();
		putMesh(mp);
		next("extruder_generic", 10);

		Shape s2 = RectangleShape().setHeight(.5).realizeShape();
		Track t = Track(Track::AM_RELATIVE_LINEIC).addKeyFrame(0,0).addKeyFrame(1.0,-1.0);
		Path p2 = LinePath().betweenPoints(Vector3(-5,0,0),Vector3(5,0,0)).setNumSeg(10).realizePath();
		mp = Extruder().setShapeToExtrude(&s2).setExtrusionPath(&p2).setRotationTrack(&t).realizeMesh();
		putMesh(mp);
		next("extruder_rotationtrack", 7);

		t = Track(Track::AM_RELATIVE_LINEIC).addKeyFrame(0,.5f).addKeyFrame(.4f,.5f).addKeyFrame(.5f,1.2f).addKeyFrame(.8f,1).addKeyFrame(1.0f,1);
		mp = Extruder().setShapeToExtrude(&s2).setExtrusionPath(&p2).setScaleTrack(&t).realizeMesh();
		putMesh(mp);
		next("extruder_scaletrack", 7);

		Procedural::Shape s4 = Procedural::Shape().addPoint(-1.2f,.2f).addPoint(-1.f,.2f).addPoint(-.9f,.1f).addPoint(.9f,.1f).addPoint(1.f,.2f).addPoint(1.2f,.2f).scale(2).setOutSide(Procedural::SIDE_LEFT);
		Procedural::Track textureTrack = Procedural::Track(Procedural::Track::AM_POINT).addKeyFrame(0,0).addKeyFrame(2,.2f).addKeyFrame(3,.8f).addKeyFrame(5,1);
		mp = Extruder().setShapeTextureTrack(&textureTrack).setShapeToExtrude(&s4).setExtrusionPath(&p2).setCapped(false).realizeMesh();
		putMesh(mp, 2);
		next("extruder_texturetrack", 7);

		cameraFront();
		Shape s3 = CircleShape().setNumSeg(16).realizeShape();
		MultiShape ms = MultiShape(2, &s3.switchSide(), &Shape(s3).scale(1.1f));
		Path p3 = CatmullRomSpline3().addPoint(0,0,-5).addPoint(0,0,0).addPoint(1,-1,5).realizePath();		
		mp = Extruder().setMultiShapeToExtrude(&ms).setExtrusionPath(&p3).realizeMesh();
		putMesh(mp);
		next("extruder_multishape", 4);	
	}

	//
	// Lathe
	//
	{
		Shape s = Shape().addPoint(0,-3).addPoint(1,-3).addPoint(1,0).addPoint(.8f,1).addPoint(.8f,2).addPoint(1.5f,3).addPoint(0,4);
		mp = Lathe().setShapeToExtrude(&s).realizeMesh();
		putMesh(mp);
		next("lathe_generic",5);

		mp = Lathe().setShapeToExtrude(&s).setAngleBegin((Radian)2).setAngleEnd((Radian)0).realizeMesh();
		putMesh(mp);
		next("lathe_anglerange",5);
	}
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
