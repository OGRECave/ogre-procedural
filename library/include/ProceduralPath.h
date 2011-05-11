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
#include "OgreSimpleSpline.h"
#include "ProceduralUtils.h"
#include "ProceduralPlatform.h"
#include "ProceduralRoot.h"
#include "ProceduralTrack.h"

namespace Procedural
{
/** Succession of points in 3D space.
 * Can be closed or not.
 */
class _ProceduralExport Path
{
	std::vector<Ogre::Vector3> mPoints;
	bool mClosed;
public:
	/// Default constructor
	Path() : mClosed(false)	{}

	/** Adds a point to the path, as a Vector3 */
	Path& addPoint(const Ogre::Vector3& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}

	/** Adds a point to the path, using its 3 coordinates */
	Path& addPoint(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		mPoints.push_back(Ogre::Vector3(x,y,z));
		return *this;
	}

	/** Clears the content of the Path */
	Path& reset()
	{
		mPoints.clear();
		return *this;
	}

	/** Define the path as being closed. Almost the same as adding a last point on the first point position */
	Path& close()
	{
		assert(mPoints.size()>0 && "Cannot close an empty path");
		mClosed = true;
		return *this;
	}
	
	/** Tells if the path is closed or not */
	bool isClosed()
	{
		return mClosed;
	}

	/** Gets the list of points as a vector of Vector3 */
	std::vector<Ogre::Vector3> getPoints()
	{
		return mPoints;
	}

	/** Safely gets a given point.
	 * Takes into account whether the path is closed or not.
	 * @param i the index of the point.
	 *          if it is <0 or >maxPoint, cycle through the list of points
	 */
	const Ogre::Vector3& getPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}

	/** Gets the number of segments in the path
	 * Takes into accound whether path is closed or not
	 */
	int getSegCount()
	{
		return (mPoints.size()-1) + (mClosed?1:0);
	}

	/**
	 * Returns local direction after the current point
	 */
	Ogre::Vector3 getDirectionAfter(int i)
	{
		// If the path isn't closed, we get a different calculation at the end, because
		// the tangent shall not be null
		if (!mClosed && i == mPoints.size()-1 && i>0)
			return (mPoints[i] - mPoints[i-1]).normalisedCopy();
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
		if (!mClosed && i == 1)
			return (mPoints[1] - mPoints[0]).normalisedCopy();
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

	/// Returns the total lineic length of that shape
	Ogre::Real getTotalLength() const;

	/// Gets a position on the shape with index of the point and a percentage of position on the segment
	/// @arg i index of the segment
	/// @arg coord a number between 0 and 1 meaning the percentage of position on the segment
	inline Ogre::Vector3 getPosition(int i, Ogre::Real coord) const
	{
		assert(mClosed||i<mPoints.size()-1 && "Out of Bounds");
		assert(coord>=0. && coord<=1. && "Coord must be comprised between 0 and 1");
		Ogre::Vector3 A = getPoint(i);
		Ogre::Vector3 B = getPoint(i+1);
		return A + coord*(B-A);
	}
	
	/// Gets a position on the shape from lineic coordinate
	/// @arg coord lineic coordinate
	 Ogre::Vector3 getPosition(Ogre::Real coord) const;

	/**
	 * Outputs a mesh representing the path.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name = "");

	/**
	 * WIP
	 * Shifts all points that form sharp angles
	 */
	//void fixSharpAngles(Ogre::Real radius);

	Path mergeKeysWithTrack(const Track& track);

};

}

#endif
