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
#ifndef PROCEDURAL_SPLINES_INCLUDED
#define PROCEDURAL_SPLINES_INCLUDED

#include "Ogre.h"

namespace Procedural
{
	enum CubicHermiteSplineAutoTangentMode
	{
		AUTOTANGENT_NONE, AUTOTANGENT_STRAIGHT, AUTOTANGENT_CATMULL
	};
		
	template <class T> 
	struct CubicHermiteSplineControlPoint
	{
		T position;
		T tangentBefore;
		T tangentAfter;	
		CubicHermiteSplineAutoTangentMode autoTangentBefore;
		CubicHermiteSplineAutoTangentMode autoTangentAfter;
		
		CubicHermiteSplineControlPoint() {}
		CubicHermiteSplineControlPoint(const T& p, const T& before, const T& after) : position(p), tangentBefore(before), tangentAfter(after), autoTangentBefore(AUTOTANGENT_NONE), autoTangentAfter(AUTOTANGENT_NONE) {}
	};
	
	template <class T>
	void computeTangents(CubicHermiteSplineControlPoint<T>& point, const T& pointBefore, const T& pointAfter)
	{
		switch (point.autoTangentBefore)
		{
		case AUTOTANGENT_STRAIGHT:
			point.tangentBefore = point.position - pointBefore;
			break;
		case AUTOTANGENT_CATMULL:
			point.tangentBefore = pointAfter - pointBefore;
			break;
		}

		switch (point.autoTangentAfter)
		{
		case AUTOTANGENT_STRAIGHT:
			point.tangentAfter = pointAfter - point.position;
			break;
		case AUTOTANGENT_CATMULL:
			point.tangentAfter = pointAfter - pointBefore;
			break;
		}
	}
	
	// Computes the Cubic Hermite interpolation between 2 control points
	// Warning : does not compute auto-tangents, as AUTOTANGENT_CATMULL depend on context
	// Auto-Tangents should be computed before that call
	template<class T>
	void computeCubicHermitePoints(const CubicHermiteSplineControlPoint<T>& pointBefore, const CubicHermiteSplineControlPoint<T>& pointAfter, unsigned int numSeg, std::vector<T>& pointList)
	{	
		const T& p0 = pointBefore.position;
		const T& m0 = pointBefore.tangentAfter;
		const T& p1 = pointAfter.position;
		const T& m1 = pointAfter.tangentBefore;
				
		for (unsigned int j = 0; j < numSeg; ++j)
			{
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t2*t;
				T P = (2*t3-3*t2+1)*p0+(t3-2*t2+t)*m0+(-2*t3+3*t2)*p1+(t3-t2)*m1;
				pointList.push_back(P);
			}
	}

	template<class T>
	void computeCatmullRomPoints(const T& P1, const T& P2, const T& P3, const T& P4, unsigned int numSeg, std::vector<T>& pointList)
	{
		for (unsigned int j = 0; j < numSeg; ++j)
			{				
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
				Ogre::Real t2 = t*t;
				Ogre::Real t3 = t*t2;
				T P = 0.5f*((-t3+2.f*t2-t)*P1 + (3.f*t3-5.f*t2+2.f)*P2 + (-3.f*t3+4.f*t2+t)*P3 + (t3-t2)*P4);
				pointList.push_back(P);
			}
	}

	template <class T>
	struct KochanekBartelsSplineControlPoint
	{
		T position;
		Ogre::Real tension;
		Ogre::Real bias;
		Ogre::Real continuity;
		
		KochanekBartelsSplineControlPoint(const T& p, Ogre::Real t, Ogre::Real b, Ogre::Real c) : position(p), tension(t), bias(b), continuity(c) {}
		KochanekBartelsSplineControlPoint(const T& p) : position(p), tension(0.), bias(0.), continuity(0.) {}
	};

	template<class T>
	void computeKochanekBartelsPoints(const KochanekBartelsSplineControlPoint<T>& P1, const KochanekBartelsSplineControlPoint<T>& P2, 
									const KochanekBartelsSplineControlPoint<T>& P3, const KochanekBartelsSplineControlPoint<T>& P4, unsigned int numSeg, std::vector<T>& pointList)
	{
		Ogre::Vector2 m0 = (1-P2.tension)*(1+P2.bias)*(1+P2.continuity)/2.f*(P2.position-P1.position)+(1-P2.tension)*(1-P2.bias)*(1-P2.continuity)/2.f*(P3.position-P2.position);
		Ogre::Vector2 m1 = (1-P3.tension)*(1+P3.bias)*(1-P3.continuity)/2.f*(P3.position-P2.position)+(1-P3.tension)*(1-P3.bias)*(1+P3.continuity)/2.f*(P4.position-P3.position);
			
		for (unsigned int j = 0;j < numSeg; ++j)
		{
			Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
			Ogre::Real t2 = t*t;
			Ogre::Real t3 = t2*t;
			T P = (2*t3-3*t2+1)*P2.position+(t3-2*t2+t)*m0+(-2*t3+3*t2)*P3.position+(t3-t2)*m1;
			pointList.push_back(P);
		}
	}
}

#endif