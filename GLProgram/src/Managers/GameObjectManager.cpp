#include "GameObjectManager.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "GraphicComponent.h"
#include "GeometryComponent.h"
#include "GeometryCube.h"

std::list<GameObject*> GameObjectManager::GOJList;

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::LoadLevel(const char * pFileName)
{
    FILE *fp;
    fopen_s(&fp, pFileName, "r");

    if (fp) {
        while (!feof(fp)) {
            char objectFileName[256];
            memset(objectFileName, 0, 256 * sizeof(char));
            fscanf_s(fp, "%255s", objectFileName, sizeof(objectFileName));

            std::string fullPath = "prefabs\\";
            fullPath += objectFileName;

            GameObject *newObject = LoadObject(fullPath.c_str());
            if (newObject == NULL)
                continue;

            GameObjectManager::GetGOJList().push_back(newObject);

            char positionData[256];
            memset(positionData, 0, 256 * sizeof(char));

            fscanf_s(fp, "%255s", positionData, sizeof(positionData));
            if (strcmp(positionData, "Position") == 0) {
                fscanf_s(fp, "%f %f %f\n", &newObject->position.x, &newObject->position.y, &newObject->position.z);
            }
        }
        fclose(fp);
    }
}

GameObject * GameObjectManager::LoadObject(const char * pFileName)
{
    GameObject *newObject = NULL;

    FILE *fp;
    fopen_s(&fp, pFileName, "r");

    if (fp) {
        newObject = new GameObject();
        
        char objectType[256];
        fscanf_s(fp, "%255s", objectType, sizeof(objectType));

        if (strcmp(objectType, "Cube") == 0)
        {
            newObject->shape = SHAPE_CUBE;

            /* Geometry Component */
            GeometryCube *geometryCube = new GeometryCube();
            newObject->geometryComponent = geometryCube;
            geometryCube->owner = newObject;

            /* Graphic Component */
            GraphicComponent *graphicComponent = new GraphicComponent();
            newObject->graphicComponent = graphicComponent;
            graphicComponent->owner = newObject;

            /* RigidBody Component */
            RigidBodyComponent *rigidbodyComponent = new RigidBodyComponent();
            newObject->rigidbodyComponent = rigidbodyComponent;
            rigidbodyComponent->owner = newObject;

        }
        else if (strcmp(objectType, "OtherShapes") == 0)
        {
            /* Add other shapes components here. */
        }

        while (!feof(fp)) {
            char infoBuffer[256];
            memset(infoBuffer, 0, 256 * sizeof(char));
            fscanf_s(fp, "%255s", infoBuffer, sizeof(infoBuffer));

            /* Load Physic parameters here */
            if (newObject->shape == SHAPE_CUBE)
            {
                GeometryCube *geometryCube = (GeometryCube*)newObject->geometryComponent;
                GraphicComponent *graphicComponent = newObject->graphicComponent;
                RigidBodyComponent *rigidbodyComponent = newObject->rigidbodyComponent;

                if (strcmp(infoBuffer, "Dimensions") == 0)
                {
                    fscanf_s(fp, "%f %f %f\n", &geometryCube->width, &geometryCube->height, &geometryCube->depth);
                }
                else if (strcmp(infoBuffer, "Texture") == 0)
                {
                    char textureFilePath[256];
                    memset(textureFilePath, 0, 256 * sizeof(char));
                    fscanf_s(fp, "%255s", textureFilePath, sizeof(textureFilePath));
                    std::string path(textureFilePath);
                    graphicComponent->TextureFilePath = path;
                }
                else if (strcmp(infoBuffer, "Mass") == 0)
                {
                    fscanf_s(fp, "%f\n", &rigidbodyComponent->mass);
                    if (rigidbodyComponent->mass != 0.0f)
                        rigidbodyComponent->inverseMass = 1.0f / rigidbodyComponent->mass;
                    else
                        rigidbodyComponent->inverseMass = 0.0f;
                }

                // Serialization Here
                geometryCube->Init();
                graphicComponent->Init();
            }
        }

        fclose(fp);
    }

    return newObject;
}

void GameObjectManager::CleanUp()
{
    std::list<GameObject*> GOJList = GameObjectManager::GetGOJList();
    for (auto it = GOJList.begin(); it != GOJList.end(); ++it)
    {
        delete *it;
    }
}


