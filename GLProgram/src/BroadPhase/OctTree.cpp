#include "OctTree.h"

#include <algorithm>
#include <iterator>

#include "debug_draw.hpp"

#include "Global.h"

#include "GameObjectManager.h"

#include "GameObject.h"
#include "BoundingBox.h"

#include "Contact.h"

OctTree::OctTree()
{
    memset(subTrees, NULL, sizeof(subTrees));
}

OctTree::OctTree(BoundingBox * _boundingBox, std::list<GameObject*> _objectList, OctTree * _parent)
    : boundingBox(_boundingBox), objectList(_objectList), parent(_parent)
{
    for (auto it = _objectList.begin(); it != _objectList.end(); ++it)
    {
        (*it)->octTree = this;
    }
}

OctTree::~OctTree()
{
    delete boundingBox;

    for (unsigned i = 0; i < 8; ++i)
    {
        if (subTrees[i] != NULL)
            delete subTrees[i];
    }
    
}

void OctTree::BuildTree()
{

    //DrawBox();
    // Base case : no more subdivide if there is only one object
    if (objectList.size() <= 1)
    {
        return;
    }

    // Base case : no more subdivide if bounding box is small enough
    if (boundingBox->width <= OCTTREE_MINSIZE &&
        boundingBox->height <= OCTTREE_MINSIZE &&
        boundingBox->depth <= OCTTREE_MINSIZE)
    {
        return;
    }

    // Subdivide into 8 regions
    float subBoxSize = boundingBox->width / 2.0f;
    glm::vec3 parentCenter = boundingBox->position;
    float offset = subBoxSize / 2.0f;

    BoundingBox* subBoxes[8];
    subBoxes[0] = new BoundingBox(glm::vec3(parentCenter.x + offset, parentCenter.y + offset, parentCenter.z + offset), subBoxSize, subBoxSize, subBoxSize);
    subBoxes[1] = new BoundingBox(glm::vec3(parentCenter.x - offset, parentCenter.y + offset, parentCenter.z + offset), subBoxSize, subBoxSize, subBoxSize);
    subBoxes[2] = new BoundingBox(glm::vec3(parentCenter.x + offset, parentCenter.y + offset, parentCenter.z - offset), subBoxSize, subBoxSize, subBoxSize);
    subBoxes[3] = new BoundingBox(glm::vec3(parentCenter.x - offset, parentCenter.y + offset, parentCenter.z - offset), subBoxSize, subBoxSize, subBoxSize);
    subBoxes[4] = new BoundingBox(glm::vec3(parentCenter.x + offset, parentCenter.y - offset, parentCenter.z + offset), subBoxSize, subBoxSize, subBoxSize);
    subBoxes[5] = new BoundingBox(glm::vec3(parentCenter.x - offset, parentCenter.y - offset, parentCenter.z + offset), subBoxSize, subBoxSize, subBoxSize);
    subBoxes[6] = new BoundingBox(glm::vec3(parentCenter.x + offset, parentCenter.y - offset, parentCenter.z - offset), subBoxSize, subBoxSize, subBoxSize);
    subBoxes[7] = new BoundingBox(glm::vec3(parentCenter.x - offset, parentCenter.y - offset, parentCenter.z - offset), subBoxSize, subBoxSize, subBoxSize);

    // Object List for child trees
    std::vector<std::list<GameObject*>> subBoxObjectList(8);
    std::vector<GameObject*> removeList;

    for (unsigned i = 0; i < 8; ++i)
    {
        for (auto it = objectList.begin(); it != objectList.end(); ++it)
        {
            if (subBoxes[i]->isContain(*it))
            {
                subBoxObjectList[i].push_back(*it);
                removeList.push_back(*it);
            }
        }

        // Remove object from the parent
        for (auto it = removeList.begin(); it != removeList.end(); ++it)
        {
            objectList.remove(*it);
        }
    }

    // Create sub tree
    for (unsigned i = 0; i < 8; ++i)
    {
        if (subBoxObjectList[i].size() != 0)
        {
            subTrees[i] = new OctTree(subBoxes[i], subBoxObjectList[i], this);
            // Recursively Build Trees
            subTrees[i]->BuildTree();
        }
        else
        {
            subTrees[i] = NULL;
            // Delete the corresponding boundingbox
            delete subBoxes[i];
        }
    }
}

void OctTree::DrawBox()
{
#define GLM_TO_FLOAT(v, glmV) \
            v[0] = glmV.x; \
            v[1] = glmV.y; \
            v[2] = glmV.z

    float mins[3];
    float maxs[3];
    GLM_TO_FLOAT(mins, boundingBox->mins);
    GLM_TO_FLOAT(maxs, boundingBox->maxs);
    dd::aabb(mins, maxs, dd::colors::Green, 100000);
}

void OctTree::Update()
{
    std::list<GameObject*> GOJList = GameObjectManager::GetGOJList();
    for (auto it = GOJList.begin(); it != GOJList.end(); ++it)
    {
        OctTree* t_octTree = (*it)->octTree;
        BoundingBox* t_boundingBox = (*it)->octTree->boundingBox;

        if (!t_boundingBox->isContain(*it))
        {
            (*it)->octTree->objectList.remove(*it);
            
            // Cache oldTree to prune later
            OctTree* oldTree = (*it)->octTree;

            do 
            {
                if (t_octTree->parent == NULL)
                {
                    printf("OBJECT IS OUTSIDE OF THE OCTTREE MAX REGION! \n ");
                    break;
                }

                t_octTree = t_octTree->parent;
                t_boundingBox = t_octTree->boundingBox;

            } while (!t_boundingBox->isContain(*it));


            (*it)->octTree = t_octTree;
            t_octTree->objectList.push_back(*it);
            recursivePruneTree(oldTree);
        }
    }

    for (auto it = GOJList.begin(); it != GOJList.end(); ++it)
    {
        TopDownRelocate(*it);
    }

}

void OctTree::PruneTree(OctTree* cOctTree)
{    
    for (unsigned i = 0; i < 8; ++i)
    {
        if (aux_PruneTree(cOctTree->subTrees[i]))
            cOctTree->subTrees[i] = NULL;
    }
}

void OctTree::aux_GenerateContact(OctTree* cOctTree, std::list<GameObject*> pObjectList, std::list<Contact*> &contactList)
{
    // Generate
    GenerateSelfContact(cOctTree->objectList, contactList);

    GenerateSelfParentContact(cOctTree->objectList, pObjectList, contactList);
    
    // Union self object list and parent object list
    std::list<GameObject*> unionList = cOctTree->objectList;
    unionList.insert(unionList.end(), pObjectList.begin(), pObjectList.end());
    //std::merge(cOctTree->objectList.begin(), cOctTree->objectList.end(), pObjectList.begin(), pObjectList.end(), std::back_inserter(unionList));
    
    for (int i = 0; i < 8; ++i)
    {
        if (cOctTree->subTrees[i] != NULL)
        {
            aux_GenerateContact(cOctTree->subTrees[i], unionList, contactList);
        }
    }
 
}

void OctTree::GenerateSelfContact(const std::list<GameObject*>& objectList, std::list<Contact*>& contactList)
{
    for (auto it = objectList.begin(); it != objectList.end(); ++it)
    {
        GameObject* curObject = *it;
        auto next_it = std::next(it, 1);
        //auto next_it = it+1;
        for (auto n_it = next_it; n_it != objectList.end(); ++n_it)
        {
            Contact* contact = new Contact(curObject, *n_it);
            contactList.push_back(contact);
        }
    }
}

void OctTree::GenerateSelfParentContact(const std::list<GameObject*>& selfObjectList, const std::list<GameObject*>& parentObjectList, std::list<Contact*>& contactList)
{
    for (auto self_it = selfObjectList.begin(); self_it != selfObjectList.end(); ++self_it)
    {
        GameObject* curObject = *self_it;
        for (auto parent_it = parentObjectList.begin(); parent_it != parentObjectList.end(); ++parent_it)
        {
            Contact* contact = new Contact(curObject, *parent_it);
            contactList.push_back(contact);
        }
    }
}

std::list<Contact*> OctTree::GeneratePotentialContactList()
{
    std::list<Contact*> contactList;

    aux_GenerateContact(this, std::list<GameObject*>(), contactList);

     return contactList;
}

bool OctTree::aux_PruneTree(OctTree* cOctTree)
{
    if (cOctTree == NULL || cOctTree->objectList.size() > 0 )
    {
        return false;
    }

    for (unsigned i = 0; i < 8; ++i)
    {
        if (cOctTree->subTrees[i] != NULL)
        {
            if (aux_PruneTree(cOctTree->subTrees[i]))
            {
                cOctTree->subTrees[i] = NULL;
            }
        }
    }

    bool deadBranch = true;
    for (unsigned i = 0; i < 8; ++i)
    {
        if (cOctTree->subTrees[i] != NULL)
            deadBranch = false;
    }

    if (deadBranch)
    { 
        delete cOctTree;
        return true;
    }

    return false;
}

void OctTree::TopDownRelocate(GameObject* gameObject)
{
    OctTree* tOctTree = gameObject->octTree;
    OctTree* dOctTree = aux_TopDownRelocate(gameObject, tOctTree);

    if (dOctTree != NULL && tOctTree != dOctTree)
    {
        tOctTree->objectList.remove(gameObject);
        dOctTree->objectList.push_back(gameObject);
        gameObject->octTree = dOctTree;
    }
}

bool OctTree::isDeadBranch(OctTree * cOctTree)
{
    for (unsigned i = 0; i < 8; ++i)
    {
        if (cOctTree->subTrees[i] != NULL)
        {
            return false;
        }
    }

    return true;
}

OctTree* OctTree::aux_TopDownRelocate(GameObject* gameObject, OctTree* cOctTree)
{
    // Base case
    if (cOctTree == NULL)
    {
        return NULL;
    }
        
    // Base case : no more subdivide if there is only one object
    /*
    if (cOctTree->objectList.size() < 1)
    {
        return cOctTree;
    }
    */
    
    if (isDeadBranch(cOctTree) && cOctTree->objectList.size() <= 1)
    {
        return cOctTree;
    }
    
    // Base case : no more subdivide if bounding box is small enough
    if (cOctTree->boundingBox->width <= OCTTREE_MINSIZE &&
        cOctTree->boundingBox->height <= OCTTREE_MINSIZE &&
        cOctTree->boundingBox->depth <= OCTTREE_MINSIZE)
    {
        return cOctTree;
    }

    glm::vec3 cTreeCenter = cOctTree->boundingBox->position;
    float cSubBoxSize = cOctTree->boundingBox->width / 2.0f;
    float offset = cSubBoxSize / 2.0f;

    BoundingBox* subBoxes[8];
    subBoxes[0] = cOctTree->subTrees[0] != NULL ? cOctTree->subTrees[0]->boundingBox : new BoundingBox(glm::vec3(cTreeCenter.x + offset, cTreeCenter.y + offset, cTreeCenter.z + offset), cSubBoxSize, cSubBoxSize, cSubBoxSize);
    subBoxes[1] = cOctTree->subTrees[1] != NULL ? cOctTree->subTrees[1]->boundingBox : new BoundingBox(glm::vec3(cTreeCenter.x - offset, cTreeCenter.y + offset, cTreeCenter.z + offset), cSubBoxSize, cSubBoxSize, cSubBoxSize);
    subBoxes[2] = cOctTree->subTrees[2] != NULL ? cOctTree->subTrees[2]->boundingBox : new BoundingBox(glm::vec3(cTreeCenter.x + offset, cTreeCenter.y + offset, cTreeCenter.z - offset), cSubBoxSize, cSubBoxSize, cSubBoxSize);
    subBoxes[3] = cOctTree->subTrees[3] != NULL ? cOctTree->subTrees[3]->boundingBox : new BoundingBox(glm::vec3(cTreeCenter.x - offset, cTreeCenter.y + offset, cTreeCenter.z - offset), cSubBoxSize, cSubBoxSize, cSubBoxSize);
    subBoxes[4] = cOctTree->subTrees[4] != NULL ? cOctTree->subTrees[4]->boundingBox : new BoundingBox(glm::vec3(cTreeCenter.x + offset, cTreeCenter.y - offset, cTreeCenter.z + offset), cSubBoxSize, cSubBoxSize, cSubBoxSize);
    subBoxes[5] = cOctTree->subTrees[5] != NULL ? cOctTree->subTrees[5]->boundingBox : new BoundingBox(glm::vec3(cTreeCenter.x - offset, cTreeCenter.y - offset, cTreeCenter.z + offset), cSubBoxSize, cSubBoxSize, cSubBoxSize);
    subBoxes[6] = cOctTree->subTrees[6] != NULL ? cOctTree->subTrees[6]->boundingBox : new BoundingBox(glm::vec3(cTreeCenter.x + offset, cTreeCenter.y - offset, cTreeCenter.z - offset), cSubBoxSize, cSubBoxSize, cSubBoxSize);
    subBoxes[7] = cOctTree->subTrees[7] != NULL ? cOctTree->subTrees[7]->boundingBox : new BoundingBox(glm::vec3(cTreeCenter.x - offset, cTreeCenter.y - offset, cTreeCenter.z - offset), cSubBoxSize, cSubBoxSize, cSubBoxSize);

    OctTree* targetTree;
    for (unsigned i = 0; i < 8; ++i)
    {
        if (subBoxes[i]->isContain(gameObject))
        {
            if (cOctTree->subTrees[i] == NULL)
            {
                cOctTree->subTrees[i] = new OctTree(subBoxes[i], std::list<GameObject*>(), cOctTree);
            }

            targetTree = aux_TopDownRelocate(gameObject, cOctTree->subTrees[i]);
            aux_TopDownRelocateCleanUp(cOctTree, subBoxes);
            return targetTree;
        }
    }

    aux_TopDownRelocateCleanUp(cOctTree, subBoxes);
    return NULL;
}

void OctTree::aux_TopDownRelocateCleanUp(OctTree * cOctTree, BoundingBox ** cBoxes)
{
    for (unsigned i = 0; i < 8; ++i)
    {
        if (cOctTree->subTrees[i] == NULL && cBoxes[i] != NULL)
        {
            delete cBoxes[i];
        }
    }
}

void OctTree::recursivePruneTree(OctTree *cOctTree)
{
    if (cOctTree == NULL || cOctTree->parent == NULL)
        return;

    if (cOctTree->objectList.size() == 0 && isDeadBranch(cOctTree))
    {
        OctTree* parent = cOctTree->parent;
        int index = getSubTreeIndex(parent, cOctTree);
        delete parent->subTrees[index];
        parent->subTrees[index] = NULL;
        recursivePruneTree(parent);
    }
}

int OctTree::getSubTreeIndex(OctTree * cParent, OctTree * cChild)
{
    for (int i = 0; i < 8; ++i)
    {
        if (cParent->subTrees[i] == cChild)
        {
            return i;
        }
    }

    return -1;
}


