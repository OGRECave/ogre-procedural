#pragma once

#define MIN(a,b) ((a<b)?(a):(b))
#define MAX(a,b) ((a>b)?(a):(b))
#define BORNE(x,a,b) MIN(MAX(x,a),b)
#define MODULO(a,b) (a)-int(floor(float(a)/float(b)))*b
#define DANS(x,a,b) ((x>=a)&&(x<=b))

// DEBUG pour un CHAR*
#define LOG(x) Ogre::LogManager::getSingletonPtr()->logMessage(x);OutputDebugStringA(x);OutputDebugStringA("\n");
// DEBUG pour une std::string
#define LOGS(x) Ogre::LogManager::getSingletonPtr()->logMessage(x);OutputDebugStringA((x).c_str());OutputDebugStringA("\n");
#define LOGN(x) LOGS(Ogre::StringConverter::toString(x));

#define TOSTRING(x) Ogre::StringConverter::toString(x)

#define VISUALDEBUG(x) debugText->setCaption(debugText->getCaption() + Ogre::StringConverter::toString(x) + "\n");