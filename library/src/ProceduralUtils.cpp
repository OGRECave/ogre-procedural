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
#include "ProceduralUtils.h"

int Procedural::Utils::counter = 0;

namespace Procedural
{
	using namespace Ogre;
	
	std::string Utils::getName(const std::string& prefix)
	{
		counter++;
		return prefix + Ogre::StringConverter::toString(counter);		
	}
	
	//-----------------------------------------------------------------------
	Quaternion Utils::_computeQuaternion(Ogre::Vector3 direction)
	{
		// First, compute an approximate quaternion (everything is ok except Roll angle)
		Quaternion quat = Vector3::UNIT_Z.getRotationTo(direction);
		// Then, compute a correction quaternion : we want the "up" direction to be always the same
		Vector3 projectedY = Vector3::UNIT_Y - Vector3::UNIT_Y.dotProduct(direction) * direction;
		if (projectedY.length()<1e-6)
			return quat;
		Vector3 tY = quat * Vector3::UNIT_Y;
		Quaternion quat2 = tY.getRotationTo(projectedY);
		Quaternion q = quat2 * quat;
		return q;
	}
}
