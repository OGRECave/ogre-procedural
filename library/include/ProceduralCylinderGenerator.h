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
#ifndef PROCEDURAL_CYLINDER_GENERATOR_INCLUDED
#define PROCEDURAL_CYLINDER_GENERATOR_INCLUDED

#include "Ogre.h"
#include "ProceduralMeshGenerator.h"
#include "ProceduralPlatform.h"

namespace Procedural
{

class _ProceduralExport CylinderGenerator : public MeshGenerator<CylinderGenerator>
{
    int numSegBase;
    int numSegHeight;
    bool capped;
    float radius;
    float height;

public:
    CylinderGenerator() : numSegBase(16),
        numSegHeight(1),
        capped(true),
        radius(1.f),
        height(1.f)
    {}

    void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

    inline CylinderGenerator & setNumSegBase(int numSegBase)
    {
        this->numSegBase = numSegBase;
        return *this;
    }

    inline CylinderGenerator & setNumSegHeight(int numSegHeight)
    {
        this->numSegHeight = numSegHeight;
        return *this;
    }

    inline CylinderGenerator & setCapped(bool capped)
    {
        this->capped = capped;
        return *this;
    }

    inline CylinderGenerator & setRadius(float radius)
    {
        this->radius = radius;
        return *this;
    }

    inline CylinderGenerator & setHeight(float height)
    {
        this->height = height;
        return *this;
    }

};
}

#endif
