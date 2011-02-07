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
/**
 * Succession of points in 3D space.
 * Can be closed or not.
 */
class _ProceduralExport Path
{
	std::vector<Ogre::Vector3> points;
	bool closed;
public:
	/** Adds a point to the path, as a Vector3 */
	Path& addPoint(const Ogre::Vector3& pt)
	{
		points.push_back(pt);
		return *this;
	}

	/** Adds a point to the path, using its 3 coordinates */
	Path& addPoint(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		points.push_back(Ogre::Vector3(x,y,z));
		return *this;
	}

	/** Clears the content of the Path */
	Path& reset()
	{
		points.clear();
		return *this;
	}

	/** Define the path as being closed. Almost the same as adding a last point on the first point position */
	Path& close()
	{
		assert(points.size()>0 && "Cannot close an empty path");
		closed = true;
		return *this;
	}
	
	/** Tells if the path is closed or not */
	bool isClosed()
	{
		return closed;
	}

	/** Gets the list of points as a vector of Vector3 */
	std::vector<Ogre::Vector3> getPoints()
	{
		return points;
	}

	/** Safely gets a given point.
	 * Takes into account whether the path is closed or not.
	 * @param i the index of the point.
	 *          if it is <0 or >maxPoint, cycle through the list of points
	 */
	const Ogre::Vector3& getPoint(int i)
	{
		if (closed)
			return points[Utils::modulo(i,points.size())];
		return points[Utils::cap(i,0,points.size()-1)];
	}

	/** Gets the number of segments in the path
	 * Takes into accound whether path is closed or not
	 */
	int getSegCount()
	{
		return (points.size()-1) + (closed?1:0);
	}

	/**
	 * Returns local direction after the current point
	 */
	Ogre::Vector3 getDirectionAfter(int i)
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!closed && i == points.size()-1 && i>0)
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
		if (!closed && i == 1)
			return (points[1] - points[0]).normalisedCopy();
		else
			return (getPoint(i) - getPoint(i-1)).normalisedCopy();
	}

	/**
	 * Returns the local direction at the current point.
	 * @param i index of the point
	 */
	Ogre::Vector3 getAvgDirection(int i)
	{
	    return (getDirectionAfter(i) + getDirectionBefore(i)).normalisedCopy();

	}

	/**
	 * Outputs a mesh representing the path.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name)
	{
		Ogre::ManualObject * manual = Root::getInstance()->sceneManager->createManualObject(name);
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
			   
		
		for (std::vector<Ogre::Vector3>::iterator itPos = points.begin(); itPos != points.end();itPos++)		
			manual->position(*itPos);		
		if (closed)
			manual->position(*(points.begin()));
		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name);

		return mesh;
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

	BezierPath& addPoint(Ogre::Real x, Ogre::Real y, Ogre::Real z)
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
				Ogre::Real t = (Ogre::Real)j/(Ogre::Real)numSeg;
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
			p.addPoint(i/(Ogre::Real)numSeg * point1 + i/(Ogre::Real)numSeg * point2);
		}
		return p;
	}

};
}

#endif
