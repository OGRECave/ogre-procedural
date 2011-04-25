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
#ifndef PROCEDURAL_TRACK_INCLUDED
#define PROCEDURAL_TRACK_INCLUDED

#include "OgreMath.h"

namespace Procedural
{
class _ProceduralExport Track
{
	std::map<Ogre::Real, Ogre::Real> mKeyFrames;
public:
	inline Track& addKeyFrame(Ogre::Real pos, Ogre::Real value)
	{
		mKeyFrames[pos] = value;
		return *this;
	}

	Ogre::Real getValue(Ogre::Real pos)
	{
		std::map<Ogre::Real, Ogre::Real>::iterator it = mKeyFrames.lower_bound(pos);
		if (it==mKeyFrames.begin())
			return it->second;
		if (it==mKeyFrames.end())
		{
			std::map<Ogre::Real, Ogre::Real>::iterator itlast = mKeyFrames.end();
			itlast--;
			return itlast->second;
		}
		std::map<Ogre::Real, Ogre::Real>::iterator itBefore=it;
		itBefore--;
		Ogre::Real x1 = itBefore->first;
		Ogre::Real y1 = itBefore->second;
		Ogre::Real x2 = it->first;
		Ogre::Real y2 = it->second;
		return (pos-x1)/(x2-x1)*(y2-y1)+y1;
	}
};
}
#endif