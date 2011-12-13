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
#include "Extrusion.h"
#include "Procedural.h"
#include "ProceduralUtils.h"

//-------------------------------------------------------------------------------------
void Sample_Extrusion::createScene(void)
{
		// -- Ground plane
		Procedural::PlaneGenerator().setNumSegX(20).setNumSegY(20).setSizeX(150).setSizeY(150).setUTile(5.0).setVTile(5.0).realizeMesh("planeMesh");
		putMesh2("planeMesh");

		// -- Road
		// The path of the road, generated from a simple spline
		Procedural::Path p = Procedural::CatmullRomSpline3().setNumSeg(8).addPoint(0,0,0).addPoint(0,0,10).addPoint(10,0,10).addPoint(20,0,0).close().realizePath().scale(2);
		// The shape that will be extruded along the path
		Procedural::Shape s = Procedural::Shape().addPoint(-1.2f,.2f).addPoint(-1.f,.2f).addPoint(-.9f,.1f).addPoint(.9f,.1f).addPoint(1.f,.2f).addPoint(1.2f,.2f).scale(2).setOutSide(Procedural::SIDE_LEFT);
		// This is an example use of a shape texture track, 
		// which specifies how texture coordinates should be mapped relative to the shape points
		Procedural::Track textureTrack = Procedural::Track(Procedural::Track::AM_POINT).addKeyFrame(0,0).addKeyFrame(2,.2).addKeyFrame(3,.8).addKeyFrame(5,1);
		// The extruder actually creates the road mesh from all parameters
		Procedural::Extruder().setExtrusionPath(&p).setShapeToExtrude(&s).setShapeTextureTrack(&textureTrack).setUTile(20.).realizeMesh("extrudedMesh");
		putMesh3("extrudedMesh");

		// -- Pillar
		// The path of the pillar, just a straight line
		Procedural::Path p2 = Procedural::LinePath().betweenPoints(Vector3(0,0,0), Vector3(0,5,0)).realizePath();
		// We're doing something custom for the shape to extrude
		Procedural::Shape s2;
		for (int i=0;i<=64;i++)						
			s2.addPoint(.5*(1-.1*Math::Abs(Math::Sin(i/8.*Math::TWO_PI))) * Vector2(Math::Cos(i/64.*Math::TWO_PI), Math::Sin(i/64.*Math::TWO_PI)));
		Procedural::Extruder().setExtrusionPath(&p2).setShapeToExtrude(&s2).setCapped(false).realizeMesh("pillar");
		// We put the pillars on the side of the road
		for (int i=0;i<p.getSegCount();i++)
			if (i%2==0)
				putMeshMat("pillar", "Examples/Marble", p.getPoint(i)+4*p.getAvgDirection(i).crossProduct(Ogre::Vector3::UNIT_Y).normalisedCopy());

		// -- Jarre
		// 
		//Procedural::Shape s3 = Procedural::CubicHermiteSpline2().addPoint(Ogre::Vector2(0,0), Ogre::Vector2::UNIT_X, Ogre::Vector2::UNIT_X)
			//.addPoint(;
}
//-------------------------------------------------------------------------------------
void Sample_Extrusion::createCamera(void)
{
	BaseApplication::createCamera();
}
//-------------------------------------------------------------------------------------
bool Sample_Extrusion::frameStarted(const FrameEvent& evt)
{
	movingLight->setPosition(mCamera->getPosition());
	return true;
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
