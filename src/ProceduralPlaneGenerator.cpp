#include "ProceduralPlaneGenerator.h"

namespace Procedural
{
void PlaneGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax)
{
    Ogre::Vector3 vX = normal.perpendicular();
    Ogre::Vector3 vY = normal.crossProduct(vX);
    Ogre::Vector3 delta1 = sizeX / numSegX * vX;
    Ogre::Vector3 delta2 = sizeY / numSegY * vY;
    // build one corner of the square
    Ogre::Vector3 orig = -0.5*sizeX*vX - 0.5*sizeY*vY;

for (int i1 = 0; i1<=numSegX;i1++)
		for (int i2 = 0; i2<=numSegY;i2++)
			{
				manual->position(orig+i1*delta1+i2*delta2+position);
				manual->textureCoord(i1/(Ogre::Real)numSegX*uTile, i2/(Ogre::Real)numSegY*vTile);
				manual->normal(normal);
			}

	bool reverse = false;
	if (delta1.crossProduct(delta2).dotProduct(normal)>0)
		reverse= true;
	for (int n1 = 0; n1<numSegX;n1++)
	{
		for (int n2 = 0; n2<numSegY;n2++)
		{
			if (reverse)
			{
			manual->index(offset+0);
			manual->index(offset+(numSegY+1));
			manual->index(offset+1);
			manual->index(offset+1);
			manual->index(offset+(numSegY+1));
			manual->index(offset+(numSegY+1)+1);
			} else {
			manual->index(offset+0);
			manual->index(offset+1);
			manual->index(offset+(numSegY+1));
			manual->index(offset+1);
			manual->index(offset+(numSegY+1)+1);
			manual->index(offset+(numSegY+1));
			}
			offset++;
		}
		offset++;
	}
	offset+=numSegY+1;
}
}
