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
#include "ProceduralPath.h"

namespace Procedural
{
//-----------------------------------------------------------------------
/// Base class for Path generators
template<class T>
class BaseSpline3
{
protected:
	/// The number of segments between 2 control points
	unsigned int mNumSeg;
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
	unsigned int mNumSeg;

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
		assert(mNumSeg > 0);
		Path p;
		for (unsigned int i = 0; i <= mNumSeg; ++i)
		{
			p.addPoint(i/(Ogre::Real)mNumSeg * mPoint1 + i/(Ogre::Real)mNumSeg * mPoint2);
		}
		return p;
	}

};
}
