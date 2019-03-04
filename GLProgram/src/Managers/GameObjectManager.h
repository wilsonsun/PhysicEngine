#pragma once
#include <list>

class GameObject;

class GameObjectManager
{
public:
    GameObjectManager();
    ~GameObjectManager();

    static void LoadLevel(const char * pFileName);
    static GameObject* LoadObject(const char * pFileName);
    static void CleanUp();

    inline static std::list<GameObject*> & GetGOJList() { return GameObjectManager::GOJList; }

private:
    // Game Object List
    static std::list<GameObject*> GOJList;
};
