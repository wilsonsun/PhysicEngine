#pragma once
class GameObject;

class Contact
{
public:
    Contact(GameObject* _objectA, GameObject* _objectB);
    ~Contact();

    GameObject* objectA;
    GameObject* objectB;
};
