
#pragma once
#include "Engine/Physics/PhysicsEngine.h"

class PhysicsTester {
public:
    void EmptyGameLoop(PScene_ptr scene, float time);

    #pragma region Tests
    // Pointers, Destructors, & Shit
    void Test0(PScene_ptr scene, float time);
    // Capsule
    void Test1(PScene_ptr scene, float time);
     //Plane
    void Test2(PScene_ptr scene, float time);
    // Actor with multiple colliders
    void Test3(PScene_ptr scene, float time);
    // Convex Mesh
    void Test4(PScene_ptr scene, float time);
    // Triangle Mesh
    void Test5(PScene_ptr scene, float time);
    // Height Field
    void Test6(PScene_ptr scene, float time);
    // Falling sphere on boxes
    void Test7(PScene_ptr scene, float time);
    // Applying forces & torques
    void Test8(PScene_ptr scene, float time);
    // Collision filteration
    void Test9(PScene_ptr scene, float time);
    // Joints
    void Test10(PScene_ptr scene, float time);
    // Scene queries
    void Test11(PScene_ptr scene);
    #pragma endregion

    void RunTests();
};