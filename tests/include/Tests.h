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
#ifndef __Test_h_
#define __Test_h_

#include "BaseApplication.h"
#include "Procedural.h"
using namespace Ogre;
using namespace Procedural;

class Unit_Test
{	
protected:
	SceneManager* mSceneMgr;

	std::vector<Entity*> mEntities;
	std::vector<SceneNode*> mSceneNodes;
	Ogre::Timer mTimer;

	void putMesh(const String& meshName, int materialIndex=0)
	{
		Entity* ent = mSceneMgr->createEntity(meshName);
		SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		sn->attachObject(ent);
		switch (materialIndex)
		{
			case 1:ent->setMaterialName("Examples/Rockwall");break;
		}
		mEntities.push_back(ent);
		mSceneNodes.push_back(sn);
		int currentIndex = mEntities.size();
		int numSN = mSceneNodes.size();
		int side = sqrt((float)numSN);
		for (int i=0; i<numSN; i++)
		{
			mSceneNodes[i]->setPosition(((i%side) - side * .5)*10., 0., -(i/side - side*.5)*10.);
		}
	}

	void putMesh(MeshPtr mesh, int materialIndex=0)
	{
		String s = mesh->getName();
		putMesh(s, materialIndex);
	}

public:
	Unit_Test(SceneManager* sn) : mSceneMgr(sn) {}
	
	virtual String getDescription()=0;
	virtual void initImpl()=0;

	void init()
	{
		Utils::log("Loading test : " + getDescription());
		mTimer.reset();
		initImpl();
		Utils::log("Test loaded in : " + Ogre::StringConverter::toString(mTimer.getMilliseconds()) + " ms");
		Ogre::OverlayManager::getSingleton().getOverlayElement("myText")->setCaption(getDescription());
	}

	void destroy()
	{
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
};

class Unit_Tests : public BaseApplication
{
	Light* movingLight;

	/* --------------------------------------------------------------------------- */
	class Test_Primitives : public Unit_Test
	{
	public:		
		Test_Primitives(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Primitive generation";
		}

		void initImpl()
		{			
			putMesh(BoxGenerator().realizeMesh(), 1);
			putMesh(CapsuleGenerator().realizeMesh(), 1);
			putMesh(ConeGenerator().realizeMesh(), 1);
			putMesh(CylinderGenerator().realizeMesh(), 1);
			putMesh(IcoSphereGenerator().realizeMesh(), 1);
			putMesh(PlaneGenerator().realizeMesh(), 1);
			putMesh(RoundedBoxGenerator().realizeMesh(), 1);			
			putMesh(SphereGenerator().realizeMesh(), 1);
			putMesh(TorusGenerator().realizeMesh(), 1);
			putMesh(TorusKnotGenerator().realizeMesh(), 1);
			putMesh(TubeGenerator().realizeMesh(), 1);
		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_SharpAngles : public Unit_Test
	{
	public:		
		Test_SharpAngles(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Sharp Angles";
		}

		void initImpl()
		{
			CatmullRomSpline3 pp;
			pp.addPoint(0,0,0).addPoint(0,0,4).addPoint(1,0,5).addPoint(5,0,5);
			Path p = pp.realizePath();
			Shape s = CircleShape().setRadius(3.).realizeShape();
			Extruder e;
			putMesh(e.setShapeToExtrude(&s).setExtrusionPath(&p).realizeMesh(),1);
			putMesh(p.realizeMesh());
			putMesh(e.setFixSharpAngles(true).setShapeToExtrude(&s).setExtrusionPath(&p).realizeMesh(),1);
			putMesh(p.realizeMesh());
		}
	};
	/* --------------------------------------------------------------------------- */
	/*class Test_RollerCoaster : public Unit_Test
	{
	public:		
		Test_RollerCoaster(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Roller Coaster! Fuck Yeah!";
		}

		void initImpl()
		{
			PlaneGenerator pg;
			putMesh(pg.setSizeX(1000).setSizeY(1000).realizeMesh(),1);
			Shape s = CircleShape().realizeShape();
			Shape s2 = CircleShape().setRadius(0.5).realizeShape();
			s2.translate(Ogre::Vector2(2,2));
			Shape s3 = CircleShape().setRadius(0.5).realizeShape();
			s3.translate(Ogre::Vector2(-2,2));
			MultiShape ms;
			ms.addShape(s).addShape(s2).addShape(s3);
			CatmullRomSpline3 crs;
			
			Ogre::Vector3 v(0,10,0);
			for (int i=0;i<100;i++)
			{
				v+=Ogre::Vector3(Math::RangeRandom(1,5), Math::RangeRandom(-1,1), Math::RangeRandom(-5,5));
				if (v.y<10) v.y=10;
				crs.addPoint(v);
			}
			Path p = crs.realizePath();
			Extruder e;
			e.setExtrusionPath(&p).setMultiShapeToExtrude(&ms);
			putMesh(e.realizeMesh(),1);
		}
	};*/

	/* --------------------------------------------------------------------------- */
	class Test_Triangulation : public Unit_Test
	{
	public:		
		Test_Triangulation(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Delaunay Triangulation";
		}

		void initImpl()
		{
			Shape s1 = CircleShape().setNumSeg(16).realizeShape().scale(4,4);
			Shape s2 = CircleShape().setNumSeg(16).realizeShape().switchSide().scale(1,.3).translate(1.5*Vector2::UNIT_X);
			MultiShape ms = MultiShape().addShape(s1);
			for (int i=0;i<8;i++)
			{
				Shape s = s2;
				s.rotate((Radian)i/8.*Math::TWO_PI);
				ms.addShape(s);
			}
			putMesh(ms.realizeMesh());
			Triangulator::triangulateToMesh(ms, "contourMesh");
			putMesh("contourMesh");

			Path p = LinePath().realizePath();
			Extruder().setMultiShapeToExtrude(&ms).setExtrusionPath(&p).realizeMesh("extrudedMesh");
			putMesh("extrudedMesh",1);

			Shape s = Shape().addPoint(0.2,.9).addPoint(1,0).addPoint(1,1).addPoint(0,1).addPoint(0,2).addPoint(2,2).addPoint(2,-1).addPoint(0.,-.2).setOutSide(SIDE_LEFT).close();			
			putMesh(s.realizeMesh());
			Triangulator::triangulateToMesh(s, "contourMesh2");
			putMesh("contourMesh2");
		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_ShapeBoolean : public Unit_Test
	{
	public:		
		Test_ShapeBoolean(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Tests different types of 2D CSG : union, difference and intersection";
		}

		void initImpl()
		{
			//CSG
			Shape s1 = CircleShape().realizeShape();
			Shape s2 = RectangleShape().setWidth(0.5).setHeight(2).realizeShape();
			
			s1.realizeMesh("contour1");
			putMesh("contour1");
			s2.realizeMesh("contour2");
			putMesh("contour2");
			MultiShape s;
			s = s1.booleanIntersect(s2);			
			s.realizeMesh("contourinter");
			putMesh("contourinter");
			s = s1.booleanUnion(s2);
			s.realizeMesh("contourunion");
			putMesh("contourunion");
			s = s1.booleanDifference(s2);
			s.realizeMesh("contourdifference");
			putMesh("contourdifference");
		}
	};

		/* --------------------------------------------------------------------------- */
	class Test_Splines : public Unit_Test
	{
	public:		
		Test_Splines(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Shape and path splines";
		}

		void initImpl()
		{
			// CatmullRomSpline
			CatmullRomSpline2 cs;
			cs.addPoint(Vector2(0,-1))
				.addPoint(Vector2(2,2))
				.addPoint(Vector2(1,2.5))
				.addPoint(Vector2(0,1.5))
				.addPoint(Vector2(-1,2.5))
				.addPoint(Vector2(-2,2))
				.setNumSeg(8)
				.close();
			putMesh(cs.realizeShape().realizeMesh());

			// Kochanek Bartels
			KochanekBartelsSpline2 kbs2;
			kbs2.addPoint(Vector2(0,-1),0,0,-1)
				.addPoint(Vector2(2,2))
				.addPoint(Vector2(1,3))
				.addPoint(Vector2(0,1.5),0,0,-1)
				.addPoint(Vector2(-1,3))
				.addPoint(Vector2(-2,2))
				.setNumSeg(8)
				.close();

			putMesh(kbs2.realizeShape().realizeMesh());

		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_Extruder : public Unit_Test
	{
	public:		
		Test_Extruder(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Extruder";
		}

		void initImpl()
		{
			Shape shape = Shape().addPoint(0,0).addPoint(0,1).addPoint(1,1).addPoint(1,0).setOutSide(SIDE_RIGHT).close();
			Shape shape2 = Shape().addPoint(1,0).addPoint(1,1).addPoint(0,1).addPoint(0,0).setOutSide(SIDE_LEFT).close();
			Path line = LinePath().betweenPoints(Vector3::ZERO, Vector3(1,10,0)).setNumSeg(2).realizePath();
			Path line2 = LinePath().betweenPoints(Vector3(1,10,0), Vector3::ZERO).setNumSeg(2).realizePath();
			Extruder e;
			e.setCapped(false);
			
			// linear extrusion
			putMesh(e.setShapeToExtrude(&shape).setExtrusionPath(&line).realizeMesh(),1);			
			putMesh(e.setShapeToExtrude(&shape2).setExtrusionPath(&line).realizeMesh(),1);			
			putMesh(e.setShapeToExtrude(&shape).setExtrusionPath(&line2).realizeMesh(),1);			
			putMesh(e.setShapeToExtrude(&shape2).setExtrusionPath(&line2).realizeMesh(),1);

			// extrusion with rotation and scale track
			{
				Path l = LinePath().betweenPoints(Vector3(0,10,0),Vector3::ZERO).setNumSeg(20).realizePath();
				Track t = Track().addKeyFrame(2,0).addKeyFrame(5,1.5).addKeyFrame(8,0);
				Track t2 = Track().addKeyFrame(0,.8).addKeyFrame(10,1.2);
				Shape s = RectangleShape().realizeShape();
				Extruder ex;
				putMesh(ex.setShapeToExtrude(&s).setExtrusionPath(&l).setRotationTrack(&t).setScaleTrack(&t2).realizeMesh(),1);
			}

			// tests different addressing modes for the (scale) track
			{
				Path l = LinePath().betweenPoints(Vector3(0,10,0),Vector3::ZERO).setNumSeg(20).realizePath();
				Track t = Track(Track::AM_ABSOLUTE_LINEIC).addKeyFrame(0,1).addKeyFrame(5,1.5).addKeyFrame(8,1);
				Shape s = RectangleShape().realizeShape();
				Extruder ex;
				// Absolute lineic
				putMesh(ex.setShapeToExtrude(&s).setExtrusionPath(&l).setScaleTrack(&t).realizeMesh(),1);
				// Relative Lineic
				Track t2 = Track(Track::AM_RELATIVE_LINEIC).addKeyFrame(0,1).addKeyFrame(0.5,1.5).addKeyFrame(1,1);
				putMesh(ex.setScaleTrack(&t2).realizeMesh(),1);
				// On path point
				Track t3 = Track(Track::AM_POINT).addKeyFrame(0,1).addKeyFrame(3,1.5).addKeyFrame(4,1);
				putMesh(ex.setScaleTrack(&t3).realizeMesh(),1);
			}


		}
	};

	/* --------------------------------------------------------------------------- */
	class Test_Lathe : public Unit_Test
	{
	public:		
		Test_Lathe(SceneManager* sn) : Unit_Test(sn) {}

		String getDescription()
		{
			return "Lathe";
		}

		void initImpl()
		{
			CatmullRomSpline2 cs;
			cs.addPoint(0,0).addPoint(1,0).addPoint(3,5).addPoint(1,10).addPoint(0,10);
			Shape s = cs.realizeShape();
			Lathe l = Lathe().setShapeToExtrude(&s);
			putMesh(l.realizeMesh(),1);
		}
	};



	/* --------------------------------------------------------------------------- */
	std::vector<Unit_Test*> mUnitTests;

	int mCurrentTestIndex;

	void switchToTest(int index)
	{
		if (index == mCurrentTestIndex)
			return;
		mUnitTests[mCurrentTestIndex]->destroy();
		mUnitTests[index]->init();
		mCurrentTestIndex = index;
	}

	void nextTest()
	{
		switchToTest(Utils::modulo(mCurrentTestIndex+1,mUnitTests.size()));
	}
	void previousTest()
	{
		switchToTest(Utils::modulo(mCurrentTestIndex-1,mUnitTests.size()));
	}

protected:
	bool keyReleased( const OIS::KeyEvent &arg )
	{		
		if (arg.key == OIS::KC_ADD || arg.key == OIS::KC_PGDOWN)
		{
			nextTest();
			return true;
		}
		if (arg.key == OIS::KC_SUBTRACT || arg.key == OIS::KC_PGUP)		
		{
			previousTest();
			return true;
		}
		return BaseApplication::keyReleased(arg);
	}

	virtual void createScene(void);
	
	virtual void createCamera(void);

	virtual void createViewports(void);
	
	virtual bool frameStarted(const FrameEvent& evt);
public:
	Unit_Tests() : mCurrentTestIndex(0)
	{}

};

#endif // #ifndef __TutorialApplication_h_
