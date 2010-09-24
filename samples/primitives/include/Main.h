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
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include "Ogre.h"
#include "OIS.h"

#include "SdkSample.h"
#include "SampleContext.h"
#include "Procedural.h"

using namespace Ogre;
using namespace OgreBites;

class Main : public SdkSample
{
public:
	Main()
	{
		mInfo["Title"] = "Ogre procedural sample";
		mInfo["Description"] = "Shows ogre-procedural primitives";
	}
	void setupContent()
	{
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
		mSceneMgr->setShadowFarDistance(100.0);
		mSceneMgr->setShadowTextureSize(1024);
		// Setup camera and light
		mCamera->setPosition(0,50,-50);
		mCamera->lookAt(0,0,0);
		Light* l = mSceneMgr->createLight("myLight");
		l->setType(Light::LT_DIRECTIONAL);
		l->setDirection(Vector3(0,-1,1).normalisedCopy());
		l->setDiffuseColour(ColourValue(1.0,0.7,0.7));

		l = mSceneMgr->createLight("myLight2");
		l->setType(Light::LT_DIRECTIONAL);
		l->setDirection(Vector3(1,-1,-1).normalisedCopy());
		l->setDiffuseColour(ColourValue(0.7,1.0,1.0));

		// Setup Procedural root (crappy init method, have to find another one)
		Procedural::Root::getInstance()->sceneManager = mSceneMgr;
		// Test primitive generation
		Procedural::PlaneGenerator().setNumSegX(20).setNumSegY(20).setSizeX(150).setSizeY(150).setUTile(5.0).setVTile(5.0).realizeMesh("planeMesh");
		putMesh2("plane", "planeMesh");
	   /* Procedural::SphereGenerator().setRadius(2.f).realizeMesh("sphereMesh");
		putMesh("sphere", "sphereMesh", Vector3(0,10,0));
		Procedural::CylinderGenerator().setHeight(3.f).setRadius(1.f).realizeMesh("cylinderMesh");
		putMesh("cylinder", "cylinderMesh", Vector3(10,10,0));
		Procedural::TorusGenerator().setRadius(3.f).setSectionRadius(1.f).realizeMesh("torusMesh");
		putMesh("torus", "torusMesh", Vector3(-10,10,0));
		Procedural::ConeGenerator().setRadius(2.f).setHeight(3.f).setNumSegBase(36).setNumSegHeight(2).realizeMesh("coneMesh");
		putMesh("cone", "coneMesh", Vector3(0,10,-10));
		Procedural::TubeGenerator().setHeight(3.f).realizeMesh("tubeMesh");
		putMesh("tube", "tubeMesh", Vector3(-10,10,-10));
		Procedural::BoxGenerator().setSizeX(2.0).setSizeY(4.f).setSizeZ(6.f).realizeMesh("boxMesh");
		putMesh("box", "boxMesh", Vector3(10,10,-10));
		Procedural::CapsuleGenerator().setHeight(2.f).realizeMesh("capsuleMesh");
		putMesh("capsule", "capsuleMesh", Vector3(0,10,10));
		Procedural::TorusKnotGenerator().realizeMesh("torusKnotMesh");
		putMesh("torusKnot", "torusKnotMesh", Vector3(-10,10,10));
		Procedural::IcoSphereGenerator().setRadius(5.).setNumIterations(3).realizeMesh("icoSphereMesh");
		putMesh("icosphere", "icoSphereMesh", Vector3(10,10,10));*/
		Procedural::RoundedBoxGenerator().setSizeX(5.f).setSizeY(5.f).setSizeZ(5.f).setChamferSize(1.f).realizeMesh("roundedBoxMesh");
		putMesh("roundedBox", "roundedBoxMesh", Vector3(20,10,10));
		//putMesh("icosphere", "geosphere8000.mesh", Vector3(10,10,10));
	}

	void putMesh2(const std::string& entityName, const std::string& meshName, const Vector3& position = Vector3::ZERO)
	{
		Entity* ent2 = mSceneMgr->createEntity(entityName, meshName);
		SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sn->attachObject(ent2);
		sn->setPosition(position);
		ent2->setMaterialName("Examples/Rockwall");
		ent2->setCastShadows(false);
	}

	void putMesh(const std::string& entityName, const std::string& meshName, const Vector3& position = Vector3::ZERO)
	{
		Entity* ent2 = mSceneMgr->createEntity(entityName, meshName);
		SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sn->attachObject(ent2);
		sn->setPosition(position);
		ent2->setMaterialName("Examples/BeachStones");
	}
};
#endif
