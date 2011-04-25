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
#include "ProceduralPath.h"
#include "ProceduralGeometryHelpers.h"

using namespace Ogre;

namespace Procedural
{/*
	void Path::fixSharpAngles(Ogre::Real radius)
	{
		Ogre::Vector3 lastV0 = mPoints[0];
		Quaternion q;
		Quaternion lastQ = Utils::_computeQuaternion(getAvgDirection(0));
		for (unsigned int i=1;i<mPoints.size();i++)
		{	
			Quaternion q = Utils::_computeQuaternion(getAvgDirection(i));
			Ogre::Vector3& v0 = mPoints[i];
			Plane plane1(lastQ * Vector3::UNIT_Z, lastV0);
			Plane plane2(q * Vector3::UNIT_Z, v0);
			Line inter;
			if (plane1.intersect(plane2, inter))
			{
				Vector3 v = inter.shortestPathToPoint(v0);
				if (v.length() < radius)
				{
					v0 = v0 + (radius-v.length()) * v.normalisedCopy();
					v = inter.shortestPathToPoint(lastV0);
					lastV0+= (radius-v.length()) * v.normalisedCopy();
					mPoints[i-1] = lastV0;
				}
			}

			lastQ = q;
			lastV0 = v0;
		}
	}*/

}