#include "ProceduralPlaneGenerator.h"

namespace Procedural
{
Ogre::MeshPtr PlaneGenerator::realizeMesh()
{
for (int i1 = 0; i1<=numSeg1;i1++)
		for (int i2 = 0; i2<=numSeg2;i2++)
			{
				manual->position(orig+i1*delta1+i2*delta2);
				manual->textureCoord(i1/(Ogre::Real)numSeg1*uTile, i2/(Ogre::Real)numSeg2*vTile);
				manual->normal(normal);
			}

	bool reverse = false;
	if (delta1.crossProduct(delta2).dotProduct(normal)>0)
		reverse= true;
	for (int n1 = 0; n1<numSeg1;n1++)
	{
		for (int n2 = 0; n2<numSeg2;n2++)
		{	
			if (reverse)
			{	
			manual->index(offset+0);
			manual->index(offset+(numSeg2+1));		
			manual->index(offset+1);
			manual->index(offset+1);		
			manual->index(offset+(numSeg2+1));					
			manual->index(offset+(numSeg2+1)+1);
			} else {				
			manual->index(offset+0);
			manual->index(offset+1);
			manual->index(offset+(numSeg2+1));		
			manual->index(offset+1);		
			manual->index(offset+(numSeg2+1)+1);
			manual->index(offset+(numSeg2+1));	
			}
			offset++;
		}
		offset++;
	}
	offset+=numSeg2+1;
}

/*void ProceduralPrimitiveFactory::_internalPlane(Ogre::ManualObject* manual, int& offset, const Ogre::Vector3& normal, int numSeg1, int numSeg2, const Ogre::Vector3& orig, const Ogre::Vector3& delta1, const Ogre::Vector3& delta2, Ogre::Real uTile, Ogre::Real vTile)
{

}*/
}