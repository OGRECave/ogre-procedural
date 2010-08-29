#include "ProceduralIcoSphereGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void IcoSphereGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
    std::vector<Ogre::Vector3> vertices;

    /// Step 1 : Generate icosahedron
    float phi = .5*(1.+sqrt(5));
    float invnorm = 1/sqrt(phi*phi+1);

    vertices.push_back(invnorm*Ogre::Vector3(-1,  phi, 0));//0
    vertices.push_back(invnorm*Ogre::Vector3( 1,  phi, 0));//1
    vertices.push_back(invnorm*Ogre::Vector3(0,   1,  -phi));//2
    vertices.push_back(invnorm*Ogre::Vector3(0,   1,   phi));//3
    vertices.push_back(invnorm*Ogre::Vector3(-phi,0,  -1));//4
    vertices.push_back(invnorm*Ogre::Vector3(-phi,0,   1));//5
    vertices.push_back(invnorm*Ogre::Vector3( phi,0,  -1));//6
    vertices.push_back(invnorm*Ogre::Vector3( phi,0,   1));//7
    vertices.push_back(invnorm*Ogre::Vector3(0,   -1, -phi));//8
    vertices.push_back(invnorm*Ogre::Vector3(0,   -1,  phi));//9
    vertices.push_back(invnorm*Ogre::Vector3(-1,  -phi,0));//10
    vertices.push_back(invnorm*Ogre::Vector3( 1,  -phi,0));//11

    int firstFaces[] = {0,1,2,
                        0,3,1,
                        0,4,5,
                        1,7,6,
                        1,6,2,
                        1,3,7,
                        0,2,4,
                        0,5,3,
                        2,6,8,
                        2,8,4,
                        3,5,9,
                        3,9,7,
                        11,6,7,
                        10,5,4,
                        10,4,8,
                        10,9,5,
                        11,8,6,
                        11,7,9,
                        10,8,11,
                        10,11,9
                       };

    std::vector<int> faces(firstFaces, firstFaces + sizeof(firstFaces)/sizeof(*firstFaces));
    int size = 60;

    /// Step 2 : tessellate
    for (int iteration = 0; iteration<numIterations; iteration++)
    {
        size*=4;
        std::vector<int> newFaces;
        newFaces.clear();
        //newFaces.resize(size);
        for (int i=0; i<size/12; i++)
        {
            int i1 = faces[i*3];
            int i2 = faces[i*3+1];
            int i3 = faces[i*3+2];
            int i12 = vertices.size();
            int i23 = i12+1;
            int i13 = i12+2;
            Ogre::Vector3 v1 = vertices[i1];
            Ogre::Vector3 v2 = vertices[i2];
            Ogre::Vector3 v3 = vertices[i3];
            //make 1 vertice at the center of each edge and project it onto the sphere
            vertices.push_back((v1+v2).normalisedCopy());
            vertices.push_back((v2+v3).normalisedCopy());
            vertices.push_back((v1+v3).normalisedCopy());
            //now recreate indices
            newFaces.push_back(i1);
            newFaces.push_back(i12);
            newFaces.push_back(i13);
            newFaces.push_back(i2);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i3);
            newFaces.push_back(i13);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i23);
            newFaces.push_back(i13);
        }
        faces.swap(newFaces);
    }

    /// Step 3 : realize
    for (int i=0; i<vertices.size(); i++)
    {
        manual->position(radius*vertices[i]);
        if (enableNormals)
            manual->normal(vertices[i]);//note : vertices are already normalised
        const Ogre::Vector3& vec = vertices[i];
        float u, v;
        float r0 = sqrtf(vec.x*vec.x+vec.z*vec.z);
        float alpha;
        alpha = atan2f(vec.z,vec.x);
        u = alpha/Ogre::Math::TWO_PI+.5;
        v = atan2f(vec.y, r0)/Ogre::Math::PI + .5;
        for (unsigned int tc=0; tc<numTexCoordSet; tc++)
            manual->textureCoord(u,v);
    }
    for (int i=0; i<size; i++)
    {
        manual->index(faces[i]);
    }
    boundingRadius = radius;
    Utils::updateAABB(aabb, Ogre::AxisAlignedBox(-radius, -radius, -radius, radius, radius, radius));
}
}
