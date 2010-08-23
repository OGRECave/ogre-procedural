#include "ProceduralTorusKnotGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void TorusKnotGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	/*Ogre::Real deltaSection = (Ogre::Math::TWO_PI / numSegSection);
	Ogre::Real deltaCircle = (Ogre::Math::TWO_PI / numSegCircle);

	int verticeIndex=0;
	for (int i = 0; i <=numSegCircle; i++)
		for (int j = 0; j<=numSegSection; j++)
		{
			Ogre::Vector3 c0(radius, 0.0, 0.0);
			Ogre::Vector3 v0(radius+sectionRadius * cosf(j*deltaSection),sectionRadius * sinf(j*deltaSection),0.0);
			Ogre::Quaternion q;
			q.FromAngleAxis(Ogre::Radian(i*deltaCircle),Ogre::Vector3::UNIT_Y);
			Ogre::Vector3 v = q * v0;
			Ogre::Vector3 c = q * c0;
			manual->position(v);
			manual->normal((v-c).normalisedCopy());
			manual->textureCoord(i/(Ogre::Real)numSegCircle*uTile, j/(Ogre::Real)numSegSection*vTile);

			if (i != numSegCircle)
			{
				manual->index(verticeIndex + numSegSection + 1);
				manual->index(verticeIndex);
				manual->index(verticeIndex + numSegSection);
				manual->index(verticeIndex + numSegSection + 1);
				manual->index(verticeIndex + 1);
				manual->index(verticeIndex);
			}
			verticeIndex ++;
		}
    boundingRadius = radius + sectionRadius;
    aabb = Ogre::AxisAlignedBox(-radius-sectionRadius,-sectionRadius,-radius-sectionRadius, radius+sectionRadius, sectionRadius, radius+sectionRadius);*/
}
}
