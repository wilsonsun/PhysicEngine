#pragma once

class OctTree;

class PhysicSystem
{
public:
    PhysicSystem();
    ~PhysicSystem();

    void Init();
    void Update();


private:
    OctTree* octTree;
};
