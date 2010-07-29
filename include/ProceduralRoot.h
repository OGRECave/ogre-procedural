#ifndef PROCEDURALROOT_H_INCLUDED
#define PROCEDURALROOT_H_INCLUDED

#include "Ogre.h"

namespace Procedural
{
class Root
{
    static Root* instance;
    Root(): sceneManager(0)
    {}
    public:

    Ogre::SceneManager* sceneManager;
    static Root* getInstance()
    {
        if (!instance)
            instance = new Root();
        return instance;
    }

};
}

#endif // PROCEDURALROOT_H_INCLUDED
