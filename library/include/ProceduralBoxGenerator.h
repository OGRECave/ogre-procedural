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
#ifndef PROCEDURAL_BOX_GENERATOR_INCLUDED
#define PROCEDURAL_BOX_GENERATOR_INCLUDED

#include "ProceduralMeshGenerator.h"
#include "ProceduralPlatform.h"

namespace Procedural
{
class _ProceduralExport BoxGenerator : public MeshGenerator<BoxGenerator>
{
    Ogre::Real sizeX,sizeY,sizeZ;
    int numSegX,numSegY,numSegZ;
public:
    BoxGenerator() : sizeX(1.f), sizeY(1.f), sizeZ(1.f),
        numSegX(1), numSegY(1), numSegZ(1) {}


    BoxGenerator& setSizeX(Ogre::Real sizeX)
    {
        this->sizeX = sizeX;
        return *this;
    }

    BoxGenerator& setSizeY(Ogre::Real sizeY)
    {
        this->sizeY = sizeY;
        return *this;
    }

    BoxGenerator& setSizeZ(Ogre::Real sizeZ)
    {
        this->sizeZ = sizeZ;
        return *this;
    }

    BoxGenerator& setNumSegX(int numSegX)
    {
        this->numSegX = numSegX;
        return *this;
    }

    BoxGenerator& setNumSegY(int numSegY)
    {
        this->numSegY = numSegY;
        return *this;
    }

    BoxGenerator& setNumSegZ(int numSegZ)
    {
        this->numSegZ = numSegZ;
        return *this;
    }

    void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

};


}
#endif
