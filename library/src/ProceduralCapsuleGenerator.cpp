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
#include "ProceduralCapsuleGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void CapsuleGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	assert(numRings>0 && numSegments>0 && numSegHeight>0 && "Num seg must be positive integers");
	assert(height>0. && radius>0. && "Height and radius must be positive");

	Ogre::Real fDeltaRingAngle = (Ogre::Math::HALF_PI / numRings);
	Ogre::Real fDeltaSegAngle = (Ogre::Math::TWO_PI / numSegments);

	float sphereRatio = radius / (2 * radius + height);
	float cylinderRatio = height / (2 * radius + height);
	// Top half sphere

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= numRings; ring++ ) {
		Ogre::Real r0 = radius * sinf ( ring * fDeltaRingAngle);
		Ogre::Real y0 = radius * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= numSegments; seg++) {
			Ogre::Real x0 = r0 * cosf(seg * fDeltaSegAngle);
			Ogre::Real z0 = r0 * sinf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, 0.5*height + y0, z0);
			if (enableNormals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for (unsigned int tc=0;tc<numTexCoordSet;tc++)
				manual->textureCoord((Ogre::Real) seg / (Ogre::Real) numSegments * uTile, (Ogre::Real) ring / (Ogre::Real) numRings * vTile * sphereRatio);

			//if (ring != numRings) {
				// each vertex (except the last) has six indices pointing to it
				manual->index(offset + numSegments + 1);
				manual->index(offset + numSegments);
				manual->index(offset);
				manual->index(offset + numSegments + 1);
				manual->index(offset);
				manual->index(offset + 1);
				//}
				offset ++;
		}; // end for seg
	} // end for ring

	// Cylinder part
	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegments);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	for (int i = 1; i <=numSegHeight-1; i++)
		for (int j = 0; j<=numSegments; j++)
		{
			Ogre::Real x0 = radius * cosf(j*deltaAngle);
			Ogre::Real z0 = radius * sinf(j*deltaAngle);
			manual->position(x0, 0.5*height-i*deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegments*uTile, i/(Ogre::Real)numSegHeight*vTile * cylinderRatio + sphereRatio);

			//if (i != numSegHeight) {
				manual->index(offset + numSegments + 1);
				manual->index(offset + numSegments);
				manual->index(offset);
				manual->index(offset + numSegments + 1);
				manual->index(offset);
				manual->index(offset + 1);
				//}
				offset ++;
		}

	// Bottom half sphere

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= numRings; ring++ ) {
		Ogre::Real r0 = radius * sinf (Ogre::Math::HALF_PI + ring * fDeltaRingAngle);
		Ogre::Real y0 =  radius * cosf (Ogre::Math::HALF_PI + ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= numSegments; seg++) {
			Ogre::Real x0 = r0 * cosf(seg * fDeltaSegAngle);
			Ogre::Real z0 = r0 * sinf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, -0.5*height + y0, z0);
			if (enableNormals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for (unsigned int tc=0;tc<numTexCoordSet;tc++)
				manual->textureCoord((Ogre::Real) seg / (Ogre::Real) numSegments * uTile, (Ogre::Real) ring / (Ogre::Real) numRings * vTile*sphereRatio + cylinderRatio + sphereRatio);

			if (ring != numRings) {
				// each vertex (except the last) has six indices pointing to it
				manual->index(offset + numSegments + 1);
				manual->index(offset + numSegments);
				manual->index(offset);
				manual->index(offset + numSegments + 1);
				manual->index(offset);
				manual->index(offset + 1);
				}
				offset ++;
		}; // end for seg
	} // end for ring

	boundingRadius = height + radius;
	Utils::updateAABB(aabb, Ogre::AxisAlignedBox(-radius, -radius-height, -radius, radius, radius+height, radius));
}
}
