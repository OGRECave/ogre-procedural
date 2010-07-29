#include "Main.h"
#include "Common.h"
#include "OIS.h"
#include "Procedural.h"
using namespace Ogre;

bool Main::init()
{	// 1 Initialiser la racine de Ogre
	root = new Root;

    LOG("*** Initializing Ogre ***");

    // 2 Configurer les répertoires de ressources
    ConfigFile cf;
    cf.load("resources.cfg");
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
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }

    // 3 Configurer l'application et créer la fenetre
    if (!root->restoreConfig())
        if (!root->showConfigDialog())
            return false;
    mWindow = root->initialise(true, "Animals");
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // 4 Créer le SceneManager
    sceneMgr = root->createSceneManager(ST_GENERIC);
	//context->sceneMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
    sceneMgr->setShadowDirectionalLightExtrusionDistance(10000.0);
    sceneMgr->setShadowColour(ColourValue(.1,.1,.1));
	sceneMgr->setAmbientLight(ColourValue(.5,.5,.5,.5));

    // 5 Créer la caméra et ajouter le viewport
	camera = sceneMgr->createCamera("MainCamera");
	camera->setPosition(Vector3(50,10,0));
	camera->setFOVy(Radian(Math::PI * 0.3));
	camera->lookAt(Vector3::ZERO);
	camera->setNearClipDistance(5);

    mViewPort = mWindow->addViewport(camera);

	mViewPort->setBackgroundColour(Ogre::ColourValue(0.5,0.5,0.8));

	Light* l = sceneMgr->createLight("myLight");
	l->setType(Light::LT_DIRECTIONAL);
	l->setDirection(Vector3(0,-1,0.2).normalisedCopy());
	l->setCastShadows(true);

	//TODO init keys


	timer = root->getTimer();

	// overlay pour le debug
	Overlay* o = OverlayManager::getSingleton().create("myOverlay");
	OverlayContainer* cont = (OverlayContainer*)OverlayManager::getSingleton().createOverlayElement("Panel","myCont");
	o->add2D(cont);
	OverlayElement* el = OverlayManager::getSingleton().createOverlayElement("TextArea","myText");
	cont->addChild(el);
	el->setCaption("Ogre program");
	el->setParameter("font_name","StarWars");
	o->show();
	debugText = el;


	// setup OIS
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputSystem = OIS::InputManager::createInputSystem(pl);
	mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject( OIS::OISMouse, true ));
	mMouse->getMouseState().width=1000;
	mMouse->getMouseState().height=1000;

	// setup scene
	Plane plane(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);

	Entity* ent = sceneMgr->createEntity("GroundEntity", "ground");
	sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	ent->setMaterialName("Examples/BeachStones");
	ent->setCastShadows(false);

    Procedural::Root::getInstance()->sceneManager = sceneMgr;
	// Test primitive generation
	Procedural::SphereGenerator(sceneMgr).setRadius(10.0).setNumRings(16).setNumSegments(16).setUTile(2.0).realizeMesh("sphereMesh");

	Entity* ent2 = sceneMgr->createEntity("sphere", "sphereMesh");
	SceneNode* sn = sceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent2);
	sn->setPosition(0,10,0);
	ent2->setMaterialName("Examples/BeachStones");

    return true;
}

void Main::cleanUp(void)
{
	delete root;
}

void Main::run()
{
	lastFrameTime=timer->getMilliseconds();
	frameDuration=0.01;
	bool mustquit = false;
	while (!mWindow->isClosed()&& !mustquit)
	{
		debugText->setCaption("");
		unsigned long currentTime = timer->getMilliseconds();
		frameDuration= 0.001*(currentTime-lastFrameTime);
		lastFrameTime = currentTime;

		if (frameDuration>0)
		{
			float f= 1.0/(frameDuration);
			VISUALDEBUG(f);
		} else {
			VISUALDEBUG(0.0f);
		}


	mKeyboard->capture();
	mMouse->capture();

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		mustquit=true;

		Vector3 v(100,0,0);
	Quaternion q1;
	q1.FromAngleAxis(Radian(0.01*mMouse->getMouseState().X.abs),Vector3::UNIT_Y);
	Quaternion q2;
	q2.FromAngleAxis(Radian(0.01*mMouse->getMouseState().Y.abs),Vector3::UNIT_Z);
	camera->setPosition(q1*q2*v);
	camera->lookAt(Vector3::ZERO);

		root->renderOneFrame();
        WindowEventUtilities::messagePump();
	}
}


void Main::go(void)
{
	if (!init())
        return;
    run();
    cleanUp();
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    //AllocConsole();
    // Create application object
    Main app;


    try {
        app.go();
    } catch( Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }
 //FreeConsole();


    return 0;
}
