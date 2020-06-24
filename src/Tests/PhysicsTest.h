
#ifndef ENGINE_PHYSICS_TEST_H
#define ENGINE_PHYSICS_TEST_H
#include "Engine/Physics/PhysicsEngine.h"

class PhysicsTester 
{
public:
    void EmptyGameLoop(float time);

    #pragma region Tests
    void Test0(float time);
    void Test1(float time);
    void Test2(float time);
    void Test3(float time);
    void Test4(float time);
    void Test5(float time);
    void Test6(float time);
    void Test7(float time);
    void Test8(float time);
    void Test9(float time);
    void Test10(float time);
    void Test11();
    #pragma endregion

    void RunTests();
};

#endif
