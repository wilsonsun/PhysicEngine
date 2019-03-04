#pragma once
#include <vector>
#include <list>

class GameObject;
class BoundingBox;
class Contact;

class OctTree
{
public:
    OctTree();
    OctTree(BoundingBox *_boundingBox, std::list<GameObject*> _objectList, OctTree *_parent);
    ~OctTree();

    BoundingBox *boundingBox;
    OctTree *parent;
    std::list<GameObject*> objectList;
    OctTree* subTrees[8];

    void BuildTree();
    void DrawBox();
    void Update();
    void PruneTree(OctTree* cOctTree);
    std::list<Contact*> GeneratePotentialContactList();
private:

    bool aux_PruneTree(OctTree* cOctTree);
    void TopDownRelocate(GameObject* gameObject);
    bool isDeadBranch(OctTree* cOctTree);
    OctTree* aux_TopDownRelocate(GameObject* gameObject, OctTree* cOctTree);
    void aux_TopDownRelocateCleanUp(OctTree* cOctTree, BoundingBox** cBoxes);
    void recursivePruneTree(OctTree* cOctTree);
    int getSubTreeIndex(OctTree* cParent, OctTree* cChild);
    void aux_GenerateContact(OctTree* cOctTree, std::list<GameObject*> pObjectList, std::list<Contact*> &contactList);
    void GenerateSelfContact(const std::list<GameObject*>& objectList, std::list<Contact*> &contactList);
    void GenerateSelfParentContact(const std::list<GameObject*>& selfObjectList, const std::list<GameObject*>& parentObjectList, std::list<Contact*> &contactList);
};
