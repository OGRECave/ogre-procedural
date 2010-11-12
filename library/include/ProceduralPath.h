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
		if (isClosed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	int getSegCount()
	{
		return (points.size()-1) + (isClosed?1:0);
	}

	/**
	 * Returns local direction after the current point
	 */
	Ogre::Vector3 getDirectionAfter(int i)
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!isClosed && i == points.size()-1 && i>0)
			return (points[i] - points[i-1]).normalisedCopy();
		else
			return (getPoint(i+1) - getPoint(i)).normalisedCopy();
	}

	/**
	 * Returns local direction after the current point
	 */
	Ogre::Vector3 getDirectionBefore(int i)
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!isClosed && i == 1)
			return (points[1] - points[0]).normalisedCopy();
		else
			return (getPoint(i) - getPoint(i-1)).normalisedCopy();
	}

	Ogre::Vector3 getAvgDirection(int i)
	{
	    return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();

	}

};

/**
 * Enables to build a Path from Bezier control points.
 * Tangents are automatically calculated from control points, so the curve will "touch" every point you define
 */
class _ProceduralExport BezierPath
{
	std::vector<Ogre::Vector3> points;

	int numSeg;
	bool isClosed;
public:
	BezierPath() : numSeg(4), isClosed(false) {}

	BezierPath& addPoint(const Ogre::Vector3& pt)
	{
		points.push_back(pt);
		return *this;
	}

	BezierPath& addPoint(float x, float y, float z)
	{
	    points.push_back(Ogre::Vector3(x,y,z));
	    return *this;
	}

	BezierPath& reset()
	{
		points.clear();
		return *this;
	}

	BezierPath& close()
	{
		isClosed = true;
		return *this;
	}

	BezierPath& setNumSeg(int numSeg)
	{
		assert(numSeg>=1);
		this->numSeg = numSeg;
		return *this;
	}

	const Ogre::Vector3& getPoint(int i)
	{
		if (isClosed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	/**
	 * Build a shape from bezier shape
	 */
	Path realizePath()
	{
		Path path;

		for (int i=0;i<points.size();i++)
		{
			const Ogre::Vector3& P0 = points[i];
			const Ogre::Vector3& P3 = getPoint(i+1);

			Ogre::Vector3 P1 = P0 + 0.5 * (getPoint(i+1)-getPoint(i-1));
			Ogre::Vector3 P2 = P3 - 0.5 * (getPoint(i+2)-P0);

			for (int j=0;j<numSeg;j++)
			{
				float t = (float)j/(float)numSeg;
				Ogre::Vector3 P = pow(1-t,3)*P0 + 3*pow(1-t,2)*t*P1 + 3*(1-t)*pow(t,2)*P2 + pow(t,3)*P3;
				path.addPoint(P);
			}
			if (i==points.size()-1 && !isClosed)
			{
				path.addPoint(P3);
			}
		}
		if (isClosed)
			path.close();

		return path;
	}
};

class _ProceduralExport LinePath
{
	Ogre::Vector3 point1, point2;

	int numSeg;

public:
	LinePath() : point1(Ogre::Vector3::ZERO), point2(Ogre::Vector3::UNIT_Y), numSeg(1) {}

	LinePath& setPoint1(Ogre::Vector3 point1)
	{
		this->point1 = point1;
		return *this;
	}

	LinePath& setPoint2(Ogre::Vector3 point2)
	{
		this->point2 = point2;
		return *this;
	}
	
	LinePath& setNumSeg(int numSeg)
	{
		this->numSeg = numSeg;
		return *this;
	}

	LinePath& betweenPoints(Ogre::Vector3 point1, Ogre::Vector3 point2)
	{
		this->point1 = point1;
		this->point2 = point2;
		return *this;
	}

	Path realizePath()
	{
		assert(numSeg>1);
		Path p;
		for (int i=0;i<=numSeg;i++)
		{
			p.addPoint(i/(float)numSeg * point1 + i/(float)numSeg * point2);
		}
		return p;
	}

};
}

#endif
