#pragma once
struct GLFWwindow;

class UISystem
{
public:
    UISystem();
    ~UISystem();
    void Init(GLFWwindow *window);
    void Update();
    void Terminate();

private:

};
