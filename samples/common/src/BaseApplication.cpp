/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
	  |___/
	  Tutorial Framework
	  http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "ProceduralStableHeaders.h"
#include "BaseApplication.h"
#include "ProceduralPlatform.h"

#include <OgreTextureManager.h>
#include <OgreInput.h>

using namespace Ogre;
//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
	: OgreBites::ApplicationContext()
    , mCam(0)
	, mCamera(0)
    , movingLight(0)
	, mSceneMgr(0)
	, mTrayMgr(0)
	, mCameraMan(0)
	, mAdvancedControls(0)
{
}

//-------------------------------------------------------------------------------------
void BaseApplication::shutdown(void)
{
	if (mTrayMgr) delete mTrayMgr;
	if (mCameraMan) delete mCameraMan;
}

//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
	// Get the SceneManager, in this case a generic one
#if OGRE_VERSION < ((2 << 16) | (0 << 8) | 0)
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");
#	ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
	RTShader::ShaderGenerator::getSingleton().addSceneManager(mSceneMgr);
#	endif
#else
	const size_t numThreads = std::max<size_t>(1, Ogre::PlatformInformation::getNumLogicalCores() / 2);
	Ogre::InstancingTheadedCullingMethod threadedCullingMethod = Ogre::INSTANCING_CULLING_SINGLETHREAD;
	if(numThreads > 1) Ogre::InstancingTheadedCullingMethod threadedCullingMethod = Ogre::INSTANCING_CULLING_THREADED;
	mSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, numThreads, threadedCullingMethod);
#endif
}
//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
	// Create the camera
	mCam = mSceneMgr->createCamera("PlayerCam");
    mCam->setNearClipDistance(.5);

    mCamera = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mCamera->attachObject(mCam);

	// Position it at 500 in Z direction
	mCamera->setPosition(Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Vector3(0,0,-300), Node::TS_WORLD);


	mCameraMan = new OgreBites::CameraMan(mCamera);   // create a default camera controller
	mCameraMan->setStyle(OgreBites::CS_ORBIT);
	addInputListener(mCameraMan);

	mSceneMgr->setShadowFarDistance(100.0);
	mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setAmbientLight(ColourValue::Black);
	// Setup camera and light
	mCamera->setPosition(0,50,-50);
	mCamera->lookAt(Vector3(0,0,0), Node::TS_WORLD);
	// Slow down speed, as the scene is small
	mCameraMan->setTopSpeed(20);

	Light* l = mSceneMgr->createLight();
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(l);

	l->setType(Light::LT_DIRECTIONAL);
	l->setDirection(Vector3(0,-1,1).normalisedCopy());
	l->setDiffuseColour(ColourValue(.7f,.5f,.5f));
	l->setSpecularColour(ColourValue::White);

	movingLight = mSceneMgr->createLight();
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(movingLight);
	movingLight->getParentSceneNode()->setPosition(mCamera->getPosition());

	movingLight->setType(Light::LT_POINT);
	movingLight->setDiffuseColour(ColourValue(.5f,.5f,.7f));
	movingLight->setSpecularColour(ColourValue::White);
	movingLight->setCastShadows(false);
}
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
	mTrayMgr = new OgreBites::TrayManager("InterfaceName", getRenderWindow(), this);
	addInputListener(mTrayMgr);

    mAdvancedControls= new OgreBites::AdvancedRenderControls(mTrayMgr, mCam);
    addInputListener(mAdvancedControls);

	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	mTrayMgr->hideCursor();

	mSceneMgr->addRenderQueueListener(getOverlaySystem());
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
	mSceneMgr->removeRenderQueueListener(getOverlaySystem());
}
//-------------------------------------------------------------------------------------
#if OGRE_VERSION < ((2 << 16) | (0 << 8) | 0)
void BaseApplication::createViewports(void)
{
	// Create one viewport, entire window
	Viewport* vp = getRenderWindow()->addViewport(mCam);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCam->setAspectRatio(
	    Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}
#else
void BaseApplication::createCompositor(void)
{
	Ogre::CompositorManager2* pCompositorManager = mRoot->getCompositorManager2();
	const Ogre::IdString workspaceName = "scene workspace";
	pCompositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue::White);
	pCompositorManager->addWorkspace(mSceneMgr, mWindow, mCamera, workspaceName, true);
}
#endif

//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
    initApp();

	mRoot->startRendering();

	// clean up
	destroyScene();
	closeApp();
}
//-------------------------------------------------------------------------------------
void BaseApplication::setup(void)
{
    OgreBites::ApplicationContext::setup();

    addInputListener(this);

	createLogManager();
	chooseSceneManager();
	createCamera();
#if OGRE_VERSION < ((2 << 16) | (0 << 8) | 0)
	createViewports();
#else
	createCompositor();
#endif

	// Create the scene
	createScene();

	createFrameListener();
};

//-------------------------------------------------------------------------------------
void BaseApplication::putMesh3(const std::string& meshName, const Vector3& position)
{
	putMeshMat(meshName, "Examples/Road", position, false);
}
//-------------------------------------------------------------------------------------
void BaseApplication::putMesh2(const std::string& meshName, const Vector3& position)
{
	putMeshMat(meshName, "Examples/Rockwall", position, false);
}
//-------------------------------------------------------------------------------------
void BaseApplication::putMesh(const std::string& meshName, const Vector3& position)
{
	putMeshMat(meshName, "Examples/BeachStones", position, true);
}
//-------------------------------------------------------------------------------------
void BaseApplication::putMeshMat(const std::string& meshName, const std::string& matName, const Vector3& position, bool castShadows)
{
	Entity* ent2 = mSceneMgr->createEntity(meshName);
	SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent2);
	sn->setPosition(position);
	ent2->setMaterialName(matName);
	ent2->setCastShadows(castShadows);
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OgreBites::KeyboardEvent& arg )
{
    using namespace OgreBites;
	if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

	if (arg.keysym.sym == SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	else if (arg.keysym.sym == 'o')   // switch between standard and orthographic projection
	{
		if (mCam->getProjectionType() == PT_PERSPECTIVE)
		{
		    mCam->setProjectionType(PT_ORTHOGRAPHIC);
		    mCam->setOrthoWindow(100,100);
		}
		else
		    mCam->setProjectionType(PT_PERSPECTIVE);
	}

	return true;
}

void BaseApplication::createLogManager(void)
{
}
