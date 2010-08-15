#include "ProceduralConeGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void ConeGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{

    Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
    Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

    Ogre::Vector3 refNormal = Ogre::Vector3(radius, height, 0.f).normalisedCopy();
    Ogre::Quaternion q;

    int verticeIndex=0;
    for (int i = 0; i <=numSegHeight; i++)
    {
        Ogre::Real r0 = radius * (1 - i / (Ogre::Real)numSegHeight);
        for (int j = 0; j<=numSegBase; j++)
        {
            Ogre::Real x0 = r0* cosf(j*deltaAngle);
            Ogre::Real z0 = r0 * sinf(j*deltaAngle);
            manual->position(x0, i*deltaHeight, z0);
            q.FromAngleAxis(Ogre::Radian(deltaAngle), Ogre::Vector3::UNIT_Y);
            manual->normal(q*refNormal);
            if (i != numSegHeight)
            manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);
            else
            manual->textureCoord(0.f, i/(Ogre::Real)numSegHeight*vTile);

            if (i != numSegHeight&& j != numSegBase)
            {
                manual->index(verticeIndex + numSegBase + 2);
                manual->index(verticeIndex);
                manual->index(verticeIndex + numSegBase+1);
                manual->index(verticeIndex + numSegBase + +2);
                manual->index(verticeIndex + 1);
                manual->index(verticeIndex);
            }

            verticeIndex ++;
        }
    }

    //low cap
    int centerIndex = verticeIndex;
    manual->position(0,0,0);
    manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
    manual->textureCoord(0.0,vTile);
    verticeIndex++;
    for (int j=0; j<=numSegBase; j++)
    {
        Ogre::Real x0 = radius * cosf(j*deltaAngle);
        Ogre::Real z0 = radius * sinf(j*deltaAngle);

        manual->position(x0, 0.0f, z0);
        manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
        manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);
        if (j!=numSegBase)
        {
            manual->index(centerIndex);
            manual->index(verticeIndex);
            manual->index(verticeIndex+1);
        }
        verticeIndex++;
    }

/*    boundingRadius = Utils::max(radius, height);
    aabb = Ogre::AxisAlignedBox(-radius,0,-radius,radius, height, radius);*/
}
}
