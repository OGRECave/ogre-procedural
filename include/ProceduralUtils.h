#ifndef PROCEDURAL_UTILS_INCLUDED
#define PROCEDURAL_UTILS_INCLUDED
#include "Ogre.h"

namespace Procedural
{
class Utils
{

    static int counter;
public:
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



    static void updateAABB(Ogre::AxisAlignedBox& aabb, const Ogre::AxisAlignedBox& newAABB)
    {
        aabb.setMinimum(min(aabb.getMinimum(), newAABB.getMinimum()));
        aabb.setMaximum(max(aabb.getMaximum(), newAABB.getMaximum()));
    }

    static std::string getName(const std::string& prefix= "default")
    {
        counter++;
        return prefix;
    }
};
}
#endif
