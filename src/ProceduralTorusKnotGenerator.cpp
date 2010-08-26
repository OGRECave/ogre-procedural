#include "ProceduralTorusKnotGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void TorusKnotGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	for (int i = 0; i <= numSegCircle * p;i++)
	{
        float phi = Ogre::Math::TWO_PI * i/(float)numSegCircle;
        float x0 = radius*(2 + cos(q*phi/(float)p)) * cos(phi);
        float y0 = radius*sin(q*phi/(float)p);
        float z0 = radius*(2 + cos(q*phi/(float)p)) * sin(phi);

        float phi1 = Ogre::Math::TWO_PI * (i+1)/(float)numSegCircle;
        float x1 = radius*(2 + cos(q*phi1/p)) * cos(phi1);
        float y1 = radius*sin(q*phi1/p);
        float z1 = radius*(2 + cos(q*phi1/p)) * sin(phi1);

        Ogre::Vector3 v0(x0,y0,z0);
        Ogre::Vector3 v1(x1,y1,z1);

        Ogre::Quaternion quat = Ogre::Vector3::UNIT_Z.getRotationTo(v1-v0);

        for (int j =0;j<=numSegSection;j++)
        {
            float alpha = Ogre::Math::TWO_PI *j/numSegSection;
            Ogre::Vector3 vp = sectionRadius*(quat * Ogre::Vector3(cos(alpha), sin(alpha),0));

			manual->position(v0+vp);
			manual->normal(vp.normalisedCopy());
			manual->textureCoord(i/(Ogre::Real)numSegCircle*uTile, j/(Ogre::Real)numSegSection*vTile);

			if (i != numSegCircle * p)
			{
				manual->index(offset + numSegSection + 1);
				manual->index(offset + numSegSection);
				manual->index(offset);
				manual->index(offset + numSegSection + 1);
				manual->index(offset);
				manual->index(offset + 1);
			}
			offset ++;
        }
	}

	boundingRadius = radius + sectionRadius;
    aabb = Ogre::AxisAlignedBox(-radius-sectionRadius,-sectionRadius,-radius-sectionRadius, radius+sectionRadius, sectionRadius, radius+sectionRadius);
}
}
