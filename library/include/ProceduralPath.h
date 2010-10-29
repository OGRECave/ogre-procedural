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
#ifndef PROCEDURAL_PATH_INCLUDED
#define PROCEDURAL_PATH_INCLUDED
#include "OgreVector3.h"
#include "ProceduralUtils.h"
#include "ProceduralPlatform.h"

namespace Procedural
{
class _ProceduralExport Path
{
	std::vector<Ogre::Vector3> points;
	bool isClosed;
public:
	Path& addPoint(const Ogre::Vector3& pt)
	{
		points.push_back(pt);
		return *this;
	}

	Path& addPoint(float x, float y, float z)
	{
		points.push_back(Ogre::Vector3(x,y,z));
		return *this;
	}

	Path& reset()
	{
		points.clear();
		return *this;
	}

	Path& close()
	{
		assert(points.size()>0 && "Cannot close an empty path");
		isClosed = true;
		return *this;
	}

	std::vector<Ogre::Vector3> getPoints()
	{
		return points;
	}

	const Ogre::Vector3& getPoint(int i)
	{
		return points[i];
	}

	const Ogre::Vector3& safeGetPoint(int i)
	{
		if (isClosed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	int getSegCount()
	{
		return (points.size()-1) + isClosed?1:0;
	}

	/**
	 * Returns local direction, being point[i+1]-point[i]
	 */
	Ogre::Vector3 getDirection(int i)
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!isClosed && i == points.size()-1 && i>0)
			return points[i] - points[i-1];
		else
			return safeGetPoint(i+1) - safeGetPoint(i);
	}
};
}

#endif
