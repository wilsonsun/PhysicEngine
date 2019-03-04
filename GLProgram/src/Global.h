#pragma once

class Camera;
class GameObjectManager;

extern Camera CAMERA;
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;
extern float OCTTREE_MAXSIZE;
extern float OCTTREE_MINSIZE;
extern bool DEBUG_DRAW_ENABLE;
extern bool TEXTURE_DRAW_ENABLE;
extern float DELTATIME;
extern float MOUSEPICK_FORCE;

// TODO
// Ray to OBB Test
// Oct Tree
// Serialization 