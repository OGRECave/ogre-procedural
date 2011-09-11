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
#include "ProceduralShapeGenerators.h"

namespace Procedural
{	
//-----------------------------------------------------------------------
Shape CubicHermiteSpline2::realizeShape()
	{
	Shape shape;

		unsigned int numPoints = mClosed ? mPoints.size() : (mPoints.size() - 1);
		for (unsigned int i = 0; i < numPoints; ++i)
		{
			const ControlPoint& pointBefore = mPoints[i];
			const ControlPoint& pointAfter = safeGetPoint(i+1);

			computePoint(pointBefore, pointAfter, mNumSeg, shape.getPointsReference());

			if (i == mPoints.size() - 2 && !mClosed)
			{
				shape.addPoint(pointAfter.position);
			}
		}
		if (mClosed)
			shape.close();
		shape.setOutSide(mOutSide);

		return shape;
	}
//-----------------------------------------------------------------------
Shape CatmullRomSpline2::realizeShape()
	{
		Shape shape;

		unsigned int numPoints = mClosed ? mPoints.size() : (mPoints.size() - 1);
		for (unsigned int i = 0; i < numPoints; ++i)
		{			
			const Ogre::Vector2& P1 = safeGetPoint(i-1);
			const Ogre::Vector2& P2 = safeGetPoint(i);
			const Ogre::Vector2& P3 = safeGetPoint(i+1);
			const Ogre::Vector2& P4 = safeGetPoint(i+2);

			for (unsigned int j = 0; j < mNumSeg; ++j)
			{				
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)mNumSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t*t2;
				Ogre::Vector2 P = 0.5f*((-t3+2.f*t2-t)*P1 + (3.f*t3-5.f*t2+2.f)*P2 + (-3.f*t3+4.f*t2+t)*P3 + (t3-t2)*P4);
				shape.addPoint(P);
			}
			if (i == mPoints.size() - 2 && !mClosed)
			{
				shape.addPoint(P3);
			}

		}
		if (mClosed)
			shape.close();
		shape.setOutSide(mOutSide);

		return shape;
	}
//-----------------------------------------------------------------------
	Shape KochanekBartelsSpline2::realizeShape()	
	{
		Shape shape;
		
		unsigned int numPoints = mClosed ? mPoints.size() : (mPoints.size() - 1);
		for (unsigned int i = 0; i < numPoints; ++i)
		{
			const ControlPoint& pm1 = safeGetPoint(i-1);
			const ControlPoint& p0 = safeGetPoint(i);
			const ControlPoint& p1 = safeGetPoint(i+1);
			const ControlPoint& p2 = safeGetPoint(i+2);
			
			Ogre::Vector2 m0 = (1-p0.tension)*(1+p0.bias)*(1+p0.continuity)/2.f*(p0.position-pm1.position)+(1-p0.tension)*(1-p0.bias)*(1-p0.continuity)/2.f*(p1.position-p0.position);
			Ogre::Vector2 m1 = (1-p1.tension)*(1+p1.bias)*(1-p1.continuity)/2.f*(p1.position-p0.position)+(1-p1.tension)*(1-p1.bias)*(1+p1.continuity)/2.f*(p2.position-p1.position);
			
			for (unsigned int j = 0;j < mNumSeg; ++j)
			{
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)mNumSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t2*t;
				Ogre::Vector2 P = (2*t3-3*t2+1)*p0.position+(t3-2*t2+t)*m0+(-2*t3+3*t2)*p1.position+(t3-t2)*m1;
				shape.addPoint(P);
			}
			if (i == mPoints.size() - 2 && !mClosed)
			{
				shape.addPoint(p1.position);
			}
		}
		if (mClosed)
			shape.close();
		shape.setOutSide(mOutSide);
		return shape;
	}
}
