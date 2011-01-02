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
#include "Extrusion.h"
#include "Procedural.h"

//-------------------------------------------------------------------------------------
void Sample_Extrusion::createScene(void)
{
		// Setup Procedural root (crappy init method, have to find another one)
		Procedural::Root::getInstance()->sceneManager = mSceneMgr;
		// Test primitive generation
		Procedural::PlaneGenerator().setNumSegX(20).setNumSegY(20).setSizeX(150).setSizeY(150).setUTile(5.0).setVTile(5.0).realizeMesh("planeMesh");
		putMesh2("planeMesh");
		//Procedural::Path p = Procedural::Path().addPoint(0,5,0).addPoint(0,4,10).addPoint(10,5,10).addPoint(20,3,0).close();
		Procedural::Path p = Procedural::BezierPath().setNumSeg(8).addPoint(0,5,0).addPoint(0,4,10).addPoint(10,5,10).addPoint(20,3,0).close().realizePath();
		//Procedural::Shape s = Procedural::Shape().addPoint(-1,0).addPoint(0,1).addPoint(1,0).close();
		Procedural::Shape s = Procedural::BezierShape().addPoint(-1,0).addPoint(0,1).addPoint(1,0).close().realizeShape();
		Procedural::Extruder().setExtrusionPath(&p).setShapeToExtrude(&s).realizeMesh("extrudedMesh");
		//putMesh("extrudedMesh");

		//Procedural::Shape s2 = Procedural::Shape().addPoint(0,0).addPoint(5,5).addPoint(1,10);
		Procedural::Shape s2 = Procedural::BezierShape().addPoint(0,0).addPoint(5,5).addPoint(0,10).setNumSeg(4).setOutSide(Procedural::SIDE_LEFT).realizeShape();
		Procedural::Lathe().setShapeToExtrude(&s2).realizeMesh("lathedMesh");
		//putMesh("lathedMesh");

		Procedural::Triangulator::triangulate(s2).transformToMesh(mSceneMgr,"toto");
		putMesh("toto");

}

void Sample_Extrusion::createCamera(void)
{
	BaseApplication::createCamera();
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowFarDistance(100.0);
	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setAmbientLight(ColourValue::Black);
	// Setup camera and light
	mCamera->setPosition(0,50,-50);
	mCamera->lookAt(0,0,0);
	// Slow down speed, as the scene is small
	mCameraMan->setTopSpeed(20);

	Light* l = mSceneMgr->createLight("myLight");
	l->setType(Light::LT_DIRECTIONAL);
	l->setDirection(Vector3(0,-1,1).normalisedCopy());
	l->setDiffuseColour(ColourValue(.7,.5,.5));
	l->setSpecularColour(ColourValue::White);

	movingLight = mSceneMgr->createLight("movingLight");
	movingLight->setType(Light::LT_POINT);
	movingLight->setDiffuseColour(ColourValue(.5,.5,.7));
	movingLight->setSpecularColour(ColourValue::White);
	movingLight->setPosition(mCamera->getPosition());
	movingLight->setCastShadows(false);
}

bool Sample_Extrusion::frameStarted(const FrameEvent& evt)
{
	movingLight->setPosition(mCamera->getPosition());
	return true;
}


void Sample_Extrusion::putMesh2(const std::string& meshName, const Vector3& position)
{
	Entity* ent2 = mSceneMgr->createEntity(meshName);
	SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent2);
	sn->setPosition(position);
	ent2->setMaterialName("Examples/Rockwall");
	ent2->setCastShadows(false);
}

void Sample_Extrusion::putMesh(const std::string& meshName, const Vector3& position)
{
	Entity* ent2 = mSceneMgr->createEntity(meshName);
	SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent2);
	sn->setPosition(position);
	//ent2->setMaterialName("Examples/BeachStones");
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
		Sample_Extrusion app;

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
