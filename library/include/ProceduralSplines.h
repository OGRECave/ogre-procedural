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
#include "Ogre.h"

namespace Procedural
{
	template <class T> 
	struct CubicHermiteSplineControlPoint
	{
		T position;
		T tangentBefore;
		T tangentAfter;	
		
		CubicHermiteSplineControlPoint(const T& p, const T& before, const T& after) : position(p), tangentBefore(before), tangentAfter(after) {}
	};
	
	template<class T>
	void computePoint(CubicHermiteSplineControlPoint<T> pointBefore, CubicHermiteSplineControlPoint<T> pointAfter, int numSeg, std::vector<T>& pointList)
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
				Ogre::Vector2 P = (2*t3-3*t2+1)*p0+(t3-2*t2+t)*m0+(-2*t3+3*t2)*p1+(t3-t2)*m1;
				pointList.push_back(P);
			}
	}

}