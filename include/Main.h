#pragma once
#include "Ogre.h"
#include "OIS.h"

class Main
{
private:
    Ogre::Camera* camera;
    Ogre::SceneManager* sceneMgr;
    Ogre::Viewport* mViewPort;
    Ogre::RenderWindow* mWindow;
    Ogre::Root* root;
    Ogre::Timer* timer;
    double frameDuration;
    unsigned long lastFrameTime;
    OIS::InputManager* mInputSystem;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;

    Ogre::OverlayElement* debugText;

    void putMesh(const std::string& entityName, const std::string& meshName, const Ogre::Vector3& position);

public:
    void go(void);
    bool init(void);
    void cleanUp(void);
    void run(void);
};
