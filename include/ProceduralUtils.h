#include "Ogre.h"

namespace Procedural
{
class Utils
{

template <typename T> static const T& min(const T& t1, const T& t2)
{
	return (t1<t2)?t1:t2;
}

template <typename T> static const T& max(const T& t1, const T& t2)
{
	 return (t1<t2)?t2:t1;
}


static Ogre::Vector3 min(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
{
	return Ogre::Vector3(Utils::min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
}

static Ogre::Vector3 max(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
{
	return Ogre::Vector3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
}


public:
static void updateAABB(Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax, const Ogre::Vector3& AABBnewMin, const Ogre::Vector3& AABBnewMax)
{
	AABBmin = min(AABBmin, AABBnewMin);
	AABBmax = max(AABBmax, AABBnewMax);
}
};
}
