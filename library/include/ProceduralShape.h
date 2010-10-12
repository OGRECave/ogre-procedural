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
#ifndef PROCEDURAL_SHAPE_INCLUDED
#define PROCEDURAL_SHAPE_INCLUDED
#include "OgreVector2.h"
#include "ProceduralPlatform.h"

namespace Procedural
{
class _ProceduralExport Shape
{
	std::vector<Ogre::Vector2> points;

public:
	Shape& addPoint(const Ogre::Vector2& pt)
	{
		points.push_back(pt);
		return *this;
	}

	Shape& addPoint(float x, float y)
	{
		points.push_back(Ogre::Vector2(x, y));
		return *this;
	}
	
	Shape& reset()
	{
		points.clear();
		return *this;
	}

	std::vector<Ogre::Vector2> getPoints()
	{
		return points;
	}

	Shape& close()
	{
		assert(points.size()>0 && "Cannot close an empty shape");
		points.push_back(points[0]);
		return *this;
	}
};

class _ProceduralExport BezierShape
{
	std::vector<Ogre::Vector2> bezierPoints;
public:
	BezierShape& addPoint(const Ogre::Vector2& pt)
	{
		bezierPoints.push_back(pt);
		return *this;
	}

	BezierShape& reset()
	{
		//bezierPoints.erase();
		return *this;
	}

	Shape* realizeShape()
	{
		//TODO
		return 0;
	}
};
}

#endif