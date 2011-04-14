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
	const Ogre::Vector3& getPoint(int i)
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

	/**
	 * Outputs a mesh representing the path.
	 * Mostly for debugging purposes
	 */
	Ogre::MeshPtr realizeMesh(const std::string& name)
	{
		Ogre::ManualObject * manual = Root::getInstance()->sceneManager->createManualObject(name);
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
			   
		
		for (std::vector<Ogre::Vector3>::iterator itPos = mPoints.begin(); itPos != mPoints.end();itPos++)		
			manual->position(*itPos);		
		if (mClosed)
			manual->position(*(mPoints.begin()));
		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name);

		return mesh;
	}

};
//-----------------------------------------------------------------------
/// Base class for Path generators
template<class T>
class BaseSpline3
{
protected:
	/// The number of segments between 2 control points
	int mNumSeg;
	/// Tells if the spline is closed or not
	bool mClosed;
public:
	BaseSpline3() : mNumSeg(4), mClosed(false) {}	

	/// Sets the number of segments between 2 control points
	T& setNumSeg(int numSeg)
	{
		assert(numSeg>=1);
		mNumSeg = numSeg;
		return (T&)*this;
	}
	
	/// Closes the spline
	T& close()
	{
		mClosed = true;
		return (T&)*this;
	}
};
//-----------------------------------------------------------------------
/**
 * Builds a path from a Catmull-Rom Spline.
 * Catmull-Rom Spline is the exact equivalent of Ogre's simple spline, ie
 * a spline for which position is smoothly interpolated between control points
 */
class _ProceduralExport CatmullRomSpline3 : public BaseSpline3<CatmullRomSpline3>
{	
	std::vector<Ogre::Vector3> mPoints;
	public:	
	/// Default constructor
	CatmullRomSpline3() {}
	
	/// Copy constructor from an Ogre simplespline
	CatmullRomSpline3(const Ogre::SimpleSpline& input) 
	{
		mPoints.resize(input.getNumPoints());
		for (unsigned short i=0; i<input.getNumPoints(); i++)
			mPoints.push_back(input.getPoint(i));
	}
	
	/// Outputs current spline to an Ogre spline
	Ogre::SimpleSpline toSimpleSpline() const 
	{
		Ogre::SimpleSpline spline;
		for (unsigned short i=0;i<mPoints.size();i++)
			spline.addPoint(mPoints[i]);
		return spline;
	}
	
	/// Adds a control point
	CatmullRomSpline3& addPoint(const Ogre::Vector3& pt)
	{
		mPoints.push_back(pt);
		return *this;
	}

	/// Adds a control point
	CatmullRomSpline3& addPoint(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		mPoints.push_back(Ogre::Vector3(x,y,z));
		return *this;
	}
	
	/// Safely gets a control point
	const Ogre::Vector3& safeGetPoint(int i) const
	{
		if (mClosed)
			return mPoints[Utils::modulo(i,mPoints.size())];
		return mPoints[Utils::cap(i,0,mPoints.size()-1)];
	}
	
	/**
	 * Build a path from Catmull-Rom control points
	 */
	Path realizePath();	
};
//-----------------------------------------------------------------------
/// Builds a line Path between 2 points
class _ProceduralExport LinePath
{
	Ogre::Vector3 mPoint1, mPoint2;
	int mNumSeg;

public:
	/// Default constructor
	LinePath() : mPoint1(Ogre::Vector3::ZERO), mPoint2(Ogre::Vector3::UNIT_Y), mNumSeg(1) {}

	/// Sets first point
	LinePath& setPoint1(Ogre::Vector3 point1)
	{
		mPoint1 = point1;
		return *this;
	}

	/// Sets second point
	LinePath& setPoint2(Ogre::Vector3 point2)
	{
		mPoint2 = point2;
		return *this;
	}
	
	/// Sets the number of segments for this line
	LinePath& setNumSeg(int numSeg)
	{
		mNumSeg = numSeg;
		return *this;
	}

	/// Builds a linepath between 2 points
	LinePath& betweenPoints(Ogre::Vector3 point1, Ogre::Vector3 point2)
	{
		mPoint1 = point1;
		mPoint2 = point2;
		return *this;
	}

	/// Outputs a path
	Path realizePath()
	{
		assert(mNumSeg>0);
		Path p;
		for (int i=0;i<=mNumSeg;i++)
		{
			p.addPoint(i/(Ogre::Real)mNumSeg * mPoint1 + i/(Ogre::Real)mNumSeg * mPoint2);
		}
		return p;
	}

};
}

#endif
