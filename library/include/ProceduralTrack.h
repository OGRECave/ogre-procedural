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

#include "ProceduralPlatform.h"
#include "OgreMath.h"

namespace Procedural
{
//---------------------------------------------------
class _ProceduralExport Track
{
public:
	enum AddressingMode
	{
		AM_ABSOLUTE_LINEIC, AM_RELATIVE_LINEIC, AM_POINT, AM_ABSOLUTE_LINEIC_INSERT_POINT, AM_RELATIVE_LINEIC_INSERT_POINT
	};
protected:
	std::map<Ogre::Real, Ogre::Real> mKeyFrames;
	
	AddressingMode mAddressingMode;
public:
	Track(AddressingMode addressingMode=AM_ABSOLUTE_LINEIC) : mAddressingMode(addressingMode) {}

	AddressingMode getAddressingMode() const
	{
		return mAddressingMode;
	}

	inline Track& addKeyFrame(Ogre::Real pos, Ogre::Real value)
	{
		mKeyFrames[pos] = value;
		return *this;
	}

	Ogre::Real getValue(Ogre::Real pos);

	Ogre::Real getFirstValue()
	{
		return mKeyFrames.begin()->second;
	}

	Ogre::Real getLastValue()
	{
		return (--mKeyFrames.end())->second;
	}
};
//---------------------------------------------------


}
#endif
