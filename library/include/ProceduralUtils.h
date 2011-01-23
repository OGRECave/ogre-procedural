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
#ifndef PROCEDURAL_UTILS_INCLUDED
#define PROCEDURAL_UTILS_INCLUDED
#include "OgreVector3.h"
#include "OgreAxisAlignedBox.h"

namespace Procedural
{
/**
 * Holds a bunch of static utility functions
 */
class Utils
{

	static int counter;
public:

	static Ogre::Vector3 min(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
	{
		return Ogre::Vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}

	static Ogre::Vector3 max(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
	{
		return Ogre::Vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}

	static Ogre::AxisAlignedBox AABBfromPoints(std::vector<Ogre::Vector3> points)
	{
	    Ogre::AxisAlignedBox aabb;
	    if (points.size() == 0)
            return aabb;
		aabb.setMinimum(points[0]);
		aabb.setMaximum(points[0]);
	    for (std::vector<Ogre::Vector3>::iterator it = points.begin(); it!=points.end();it++)
	    {
	        aabb.setMinimum(min(aabb.getMinimum(), *it));
	        aabb.setMaximum(max(aabb.getMaximum(), *it));
	    }

	    return aabb;
	}

    static void updateBoundingRadius(Ogre::Real& radius, Ogre::Vector3 point)
    {
        radius = std::max(radius, point.length());
    }

	static Ogre::Real boundingRadiusFromPoints(std::vector<Ogre::Vector3> points)
	{
	    Ogre::Real radius = 0.f;
	    for (std::vector<Ogre::Vector3>::iterator it = points.begin(); it!=points.end();it++)
	    {
	        if (it->length()>radius)
                radius = it->length();
	    }
		return radius;
	}

	static std::string getName(const std::string& prefix= "default")
	{
		counter++;
		return prefix;
	}

	static Ogre::Vector3 vectorPermute(const Ogre::Vector3& in)
	{
		return Ogre::Vector3(in.z, in.x, in.y);
	}

	static Ogre::Vector3 vectorAntiPermute(const Ogre::Vector3& in)
	{
		return Ogre::Vector3(in.y, in.z, in.x);
	}

	static int cap(int n, int min, int max)
	{
		return std::max(std::min(n,max),min);
	}

	/**
	 * An extend version of the standard modulo, in that int values are "wrapped"
	 * in both directions, whereas with standard modulo, (-1)%2 == -1
	 * Always return an int between 0 and cap-1
	 */
	static int modulo(int n, int cap)
	{
		if (n>=0)
			return n%cap;
		return (cap-1)-((1+n)%cap);
	}

	/**
	 * Equivalent of Ogre::Vector3::angleBetween, applied to Ogre::Vector2
	 */
	static Ogre::Radian angleBetween(const Ogre::Vector2& v1, const Ogre::Vector2& v2)
	{
		// TODO : also compute angle greater than PI
		Ogre::Real lenProduct = v1.length() * v2.length();
		// Divide by zero check
		if(lenProduct < 1e-6f)
			lenProduct = 1e-6f;
		
		Ogre::Real f = v1.dotProduct(v2) / lenProduct;
	
		f = Ogre::Math::Clamp(f, (Ogre::Real)-1.0, (Ogre::Real)1.0);
		return Ogre::Math::ACos(f);
	}
};
}
#endif
