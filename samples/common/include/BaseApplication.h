/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
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
#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>

#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include <OgreVector3.h>

#include <OgreApplicationContext.h>
#include <OgreAdvancedRenderControls.h>

class BaseApplication : public OgreBites::ApplicationContext, public OgreBites::InputListener, OgreBites::TrayListener
{
public:
	BaseApplication(void);

	virtual void shutdown(void);

	virtual void go(void);

protected:
	virtual void setup();
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createScene(void) = 0; // Override me!
	virtual void destroyScene(void);
#if OGRE_VERSION < ((2 << 16) | (0 << 8) | 0)
	virtual void createViewports(void);
#else
	virtual void createCompositor(void);
#endif
	virtual void createLogManager(void);

	// OIS::KeyListener
	virtual bool keyPressed( const OgreBites::KeyboardEvent& arg );

	void putMeshMat(const std::string& meshName, const std::string& matName, const Ogre::Vector3& position = Ogre::Vector3::ZERO, bool castShadows=true);
	void putMesh(const std::string& meshName, const Ogre::Vector3& position = Ogre::Vector3::ZERO);
	void putMesh2(const std::string& meshName, const Ogre::Vector3& position = Ogre::Vector3::ZERO);
	void putMesh3(const std::string& meshName, const Ogre::Vector3& position = Ogre::Vector3::ZERO);


	Ogre::Camera* mCam;
	Ogre::SceneNode* mCamera;
	Ogre::Light* movingLight;
	Ogre::SceneManager* mSceneMgr;

	// OgreBites
	OgreBites::TrayManager* mTrayMgr;
	OgreBites::CameraMan* mCameraMan;       // basic camera controller
	OgreBites::AdvancedRenderControls* mAdvancedControls;     // sample details panel
};

#endif // #ifndef __BaseApplication_h_
