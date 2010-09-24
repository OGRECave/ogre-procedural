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
#include "Ogre.h"

namespace Procedural
{
class Utils
{

	static int counter;
public:
	template <typename T> static const T& min(const T& t1, const T& t2)
	{
		return (t1<t2)?t1:t2;
	}

	template <typename T> static const T& max(const T& t1, const T& t2)
	{
		return (t1<t2)?t2:t1;
	}


	static Ogre::Vector3 min(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
	{
		return Ogre::Vector3(Utils::min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
	}

	static Ogre::Vector3 max(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
	{
		return Ogre::Vector3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
	}



	static void updateAABB(Ogre::AxisAlignedBox& aabb, const Ogre::AxisAlignedBox& newAABB)
	{
		aabb.setMinimum(min(aabb.getMinimum(), newAABB.getMinimum()));
		aabb.setMaximum(max(aabb.getMaximum(), newAABB.getMaximum()));
	}

	static std::string getName(const std::string& prefix= "default")
	{
		counter++;
		return prefix;
	}

	static Ogre::Vector3 vectorPermute(const Ogre::Vector3 in)
	{
		return Ogre::Vector3(in.z, in.x, in.y);
	}

	static Ogre::Vector3 vectorAntiPermute(const Ogre::Vector3 in)
	{
		return Ogre::Vector3(in.y, in.z, in.x);
	}
};
}
#endif
