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
#include "ProceduralSphereGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void SphereGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	assert(numRings>0 && numSegments>0 && "Num seg must be positive");
	assert(radius>0 && "Radius must be positive");

	Ogre::Real fDeltaRingAngle = (Ogre::Math::PI / numRings);
	Ogre::Real fDeltaSegAngle = (Ogre::Math::TWO_PI / numSegments);

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= numRings; ring++ ) {
		Ogre::Real r0 = radius * sinf (ring * fDeltaRingAngle);
		Ogre::Real y0 = radius * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= numSegments; seg++) {
			Ogre::Real x0 = r0 * sinf(seg * fDeltaSegAngle);
			Ogre::Real z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, y0, z0);
			if (enableNormals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for (unsigned int tc=0;tc<numTexCoordSet;tc++)
				manual->textureCoord((Ogre::Real) seg / (Ogre::Real) numSegments * uTile, (Ogre::Real) ring / (Ogre::Real) numRings * vTile);

			if (ring != numRings) {
				// each vertex (except the last) has six indices pointing to it
				manual->index(offset + numSegments + 1);
				manual->index(offset);
				manual->index(offset + numSegments);
				manual->index(offset + numSegments + 1);
				manual->index(offset + 1);
				manual->index(offset);
				offset ++;
				}
		}; // end for seg
	} // end for ring

	boundingRadius = radius;
	Utils::updateAABB(aabb, Ogre::AxisAlignedBox(-radius, -radius, -radius, radius, radius, radius));
}
}
