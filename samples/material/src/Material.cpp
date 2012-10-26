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
#include "ProceduralStableHeaders.h"
#include "Material.h"
#include "Procedural.h"
#include "ProceduralUtils.h"

//-------------------------------------------------------------------------------------
void Sample_Material::createScene(void)
{
	// -- Ground material
	int brickLines = 4;
	int pxPerBrick = 32;

	Procedural::Texture::TextureBuffer bricks(brickLines * pxPerBrick);
	
	// Basic structure
	Procedural::Texture::Cell(&bricks).setRegularity(233).setDensity(brickLines).process();
	Procedural::Texture::Colours(&bricks).setBrithness(174).setContrast(198).process();
	Procedural::Texture::TextureBuffer distort(brickLines * pxPerBrick);
	Procedural::Texture::Solid(&distort).setColour((Ogre::uchar)125, (Ogre::uchar)133, (Ogre::uchar)0, (Ogre::uchar)255).process();
	Procedural::Texture::Rectangle rectDraw(&distort);
	for(size_t i = 1; i < brickLines; i++)
	{
		Ogre::ColourValue rc = Ogre::ColourValue((i % 2 == 0) ? Ogre::Math::RangeRandom(0.4f, 0.6f) : Ogre::Math::RangeRandom(0.0f, 0.2f), 0.52f, 1.0f);
		rc.a = 1.0f;
		rectDraw.setRectangle(0, i * pxPerBrick, brickLines * pxPerBrick, i * pxPerBrick + pxPerBrick).setColour(rc).process();
	}
	Procedural::Texture::Distort(&bricks).setParameterImage(&distort).setPower(50).process();
	Procedural::Texture::Cloud(&distort).process();
	Procedural::Texture::Normals(&distort).process();
	Procedural::Texture::Distort(&bricks).setParameterImage(&distort).setPower(8).process();

	Procedural::Texture::TextureBuffer normal(&bricks);

	// Normal map & lightning
	Procedural::Texture::TextureBuffer light(&bricks);
	Procedural::Texture::Colours(&light).setColourBase(0.325f, 0.0f, 0.0f, 0.0f).setColourPercent(0.78f, 0.443f, 0.333f, 1.0f).process();
	Procedural::Texture::Normals(&normal).process();
	Procedural::Texture::Light(&light).setNormalMap(&normal).setColourAmbient(0.164f, 0.0f, 0.0f, 0.0f).setPosition(255.0f, 255.0f, 200.0f).setBumpPower(48).setSpecularPower(8).process();

	// Joint
	Procedural::Texture::TextureBuffer joint(&bricks);
	Procedural::Texture::Invert(&joint).process();
	Procedural::Texture::Threshold(&joint).setThreshold(200).setRatio(255).process();
	Procedural::Texture::Colours(&joint).setColourBase(0.215f, 0.207f, 0.137f, 0.0f).setColourPercent(0.294f, 0.266f, 0.345f, 1.0f).setBrithness(110).setContrast(153).process();

	// Additional structure
	Procedural::Texture::TextureBuffer colourcloud(&bricks);
	Procedural::Texture::Threshold(&colourcloud).process();
	Procedural::Texture::TextureBuffer cloud(&bricks);
	Procedural::Texture::Cloud(&cloud).process();
	Procedural::Texture::Combine(&colourcloud).addImage(&cloud, Procedural::Texture::Combine::METHOD_MULTIPLY).process();
	Procedural::Texture::Colours(&colourcloud).setColourBase(0.329f, 0.141f, 0.0f, 0.0f).setColourPercent(0.95f, 0.949f, 0.862f, 1.0f).setBrithness(30).process();

	// Finish texture
	Procedural::Texture::Combine(&light)
		.addImage(&joint, Procedural::Texture::Combine::METHOD_ADD_CLAMP)
		.addImage(&colourcloud, Procedural::Texture::Combine::METHOD_ADD_CLAMP)
		.process();

	Ogre::TexturePtr demoTexture = light.createTexture("proceduralTexture");
	//Ogre::TexturePtr demoTextureNormal = normal.createTexture("proceduralTextureNormal");

	Ogre::MaterialPtr demoMaterial = Ogre::MaterialManager::getSingletonPtr()->create("proceduralMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	demoMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("proceduralTexture");
	demoMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);

	// -- Test plane
	Procedural::PlaneGenerator().setNumSegX(20).setNumSegY(20).setSizeX(150).setSizeY(150).setUTile(5.0).setVTile(5.0).realizeMesh("planeMesh");
	Entity* ent2 = mSceneMgr->createEntity("planeMesh");
	SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	sn->attachObject(ent2);
	ent2->setCastShadows(false);
	ent2->setMaterial(demoMaterial);
}
//-------------------------------------------------------------------------------------
void Sample_Material::createCamera(void)
{
	BaseApplication::createCamera();
}
//-------------------------------------------------------------------------------------
bool Sample_Material::frameStarted(const FrameEvent& evt)
{
	movingLight->setPosition(mCamera->getPosition());
	return true;
}
//-------------------------------------------------------------------------------------
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
		Sample_Material app;

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
