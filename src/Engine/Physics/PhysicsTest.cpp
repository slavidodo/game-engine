
#include "Pch.h"
#include "Engine/Physics/PhysicsTest.h"


void PhysicsTester::EmptyGameLoop(PScene_ptr scene, float time) {
	auto startTime = std::chrono::high_resolution_clock::now();
	while (true) {
		scene->Update(0.01f);
		scene->ApplyUpdateResults(true);

		auto crntTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = crntTime - startTime;
		if (elapsedTime.count() > time) break;
	}
}

#pragma region Tests
// Pointers, Destructors, & Shit
void PhysicsTester::Test0(PScene_ptr scene, float time) {
	PBoxGeometry_ptr geometry = PBoxGeometry::CreateGeometry(glm::vec3(1.0f, 1.0f, 1.0f));
	// A
	{
		PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
		PCollider_ptr sharedCollider = PCollider::CreateCollider(geometry);
		actor->AddCollider(sharedCollider);
		actor->RemoveCollider(sharedCollider);
	}
	// B
	{
		PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
		PCollider_ptr sharedCollider = PCollider::CreateCollider(geometry);
		actor->AddCollider(sharedCollider);
	}
	// C
	{
		PCollider_ptr sharedCollider = PCollider::CreateCollider(geometry);
		{
			PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
			actor->AddCollider(sharedCollider);
		}
	}
	// D
	{
		PCollider_ptr sharedCollider = PCollider::CreateCollider(geometry);
		{
			PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
			actor->AddCollider(sharedCollider);
			actor->RemoveCollider(sharedCollider);
		}
	}
	// E
	{
		PStaticActor_ptr actor1 = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
		{
			PStaticActor_ptr actor2 = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
			PCollider_ptr sharedCollider = PCollider::CreateCollider(geometry);
			actor1->AddCollider(sharedCollider);
			actor2->AddCollider(sharedCollider);
			actor1->RemoveCollider(sharedCollider);
		}
	}
	// F 
	{
		PStaticActor_ptr actor1 = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
		PStaticActor_ptr actor2 = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
		{
			PCollider_ptr sharedCollider = PCollider::CreateCollider(geometry);
			actor1->AddCollider(sharedCollider);
			actor2->AddCollider(sharedCollider);
			scene->AddActor(actor1);
			scene->AddActor(actor2);
			EmptyGameLoop(scene, time);
		}
		EmptyGameLoop(scene, time);
		scene->RemoveActor(actor1);
		scene->RemoveActor(actor2);
	}
}
// Capsule
void PhysicsTester::Test1(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));

	PCapsuleGeometry_ptr geometry = PCapsuleGeometry::CreateGeometry(1.0f, 2.0f);
	PCollider_ptr collider = PCollider::CreateCollider(geometry);
	collider->Rotate(90.0f * 3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	actor->AddCollider(collider);
	scene->AddActor(actor);

	EmptyGameLoop(scene, time);

	scene->RemoveActor(actor);
}
 //Plane
void PhysicsTester::Test2(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f)); // must be static

	PPlaneGeometry_ptr geometry = PPlaneGeometry::CreateGeometry();

	PCollider_ptr collider = PCollider::CreateCollider(geometry); // collider ref cnt = 1 (exclusive collider)
	collider->Rotate(90.0f * 3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	actor->AddCollider(collider);
	
	scene->AddActor(actor);

	EmptyGameLoop(scene, time);
	
	scene->RemoveActor(actor);
	actor->RemoveCollider(std::move(collider)); // DON'T FORGET
}// collider ref cnt = 0
// Actor with multiple colliders
void PhysicsTester::Test3(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));

	PSphereGeometry_ptr geometry1 = PSphereGeometry::CreateGeometry(1.0f);
	PCollider_ptr collider1 = PCollider::CreateCollider(geometry1);
	collider1->SetLocalPosition(glm::vec3(0.0f, 12.0f, 0.0f));
	actor->AddCollider(collider1);

	PSphereGeometry_ptr geometry2 = PSphereGeometry::CreateGeometry(3.0f);
	PCollider_ptr collider2 = PCollider::CreateCollider(geometry2);
	collider2->SetLocalPosition(glm::vec3(0.0f, 8.0f, 0.0f));
	actor->AddCollider(collider2);

	PSphereGeometry_ptr geometry3 = PSphereGeometry::CreateGeometry(5.0f);
	PCollider_ptr collider3 = PCollider::CreateCollider(geometry3);
	actor->AddCollider(collider3);

	scene->AddActor(actor);

	EmptyGameLoop(scene, time);

	scene->RemoveActor(actor);
}
// Convex Mesh
void PhysicsTester::Test4(PScene_ptr scene, float time) {
	PDynamicActor_ptr actor = PDynamicActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
	actor->SetGravity(false);

	PConvexMeshDescriptor desc;
	{
		desc.SetPointsCount(5);
		desc.SetPointsStride(3 * sizeof(float));
		float vertices[] = {
			+0.0f, +1.0f, +0.0f,
			+1.0f, +0.0f, +0.0f,
			-1.0f, +0.0f, +0.0f,
			+0.0f, +0.0f, +1.0f,
			+0.0f, +0.0f, -1.0f,
		};
		desc.SetPointsData(vertices);
		desc.EnableAutomaticHullComputation();
	}
	PConvexMeshGeometry_ptr geometry = PConvexMeshGeometry::CreateGeometry(desc);
	PCollider_ptr collider = PCollider::CreateCollider(geometry);
	
	actor->AddCollider(collider);
	scene->AddActor(actor);

	EmptyGameLoop(scene, time);

	actor->RemoveCollider(std::move(collider));
	scene->RemoveActor(actor);
}
// Triangle Mesh
void PhysicsTester::Test5(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f)); // must be static

	PTriangleMeshDescriptor desc;
	{
		desc.SetPointsCount(5);
		desc.SetPointsStride(3 * sizeof(float));
		float vertices[] {
			+0.0f, +1.0f, +0.0f,
			+1.0f, +0.0f, +0.0f,
			-1.0f, +0.0f, +0.0f,
			+0.0f, +0.0f, +1.0f,
			+0.0f, +0.0f, -1.0f,
		};
		desc.SetPointsData(vertices);

		desc.SetTrianglesCount(6);
		desc.SetTrianglesStride(3 * sizeof(uint32_t));
		uint32_t triangles[] {
			0, 1, 4,
			0, 4, 2,
			0, 2, 3,
			0, 3, 1,
			1, 2, 4,
			1, 3, 2
		};
		desc.SetTrianglesData(triangles);
	}
	PTriangleMeshGeometry_ptr geometry = PTriangleMeshGeometry::CreateGeometry(desc); // the actor must be static or (dynamic & kinematic)
	PCollider_ptr collider = PCollider::CreateCollider(geometry);

	actor->AddCollider(collider);
	scene->AddActor(actor);

	EmptyGameLoop(scene, time);

	actor->RemoveCollider(std::move(collider));
	scene->RemoveActor(actor);
}
// Height Field
void PhysicsTester::Test6(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f)); // the actor must be static

	PHeightFieldStructure structure = PHeightFieldStructure(64, 64, 0.05f);
	{
		for (int r = 0; r < structure.GetNumOfRows(); r++) {
			for (int c = 0; c < structure.GetNumOfCols(); c++) {
				structure.SetVertexHeight(r, c, rand() % 15);
				structure.SetVertexMaterials(r, c, rand() % 3, rand() % 3);
			}
		}
	}
	PHeightFieldDescriptor desc = PHeightFieldDescriptor(structure);
	PHeightFieldGeometry_ptr geometry = PHeightFieldGeometry::CreateGeometry(desc);
	PMaterialSet_ptr materials = PMaterialSet::CreateMaterialSet(std::vector<PMaterialProperties>{{0.5f, 0.5f, 0.5}, {1.0f, 0.2f, 0.4f}, {0.6f, 0.8f, 0.8f}});
	PCollider_ptr collider = PCollider::CreateCollider(geometry, materials);

	actor->AddCollider(collider);
	scene->AddActor(actor);

	EmptyGameLoop(scene, time);

	actor->RemoveCollider(std::move(collider));
	scene->RemoveActor(actor);
}
// Falling sphere on boxes
void PhysicsTester::Test7(PScene_ptr scene, float time) {
	PDynamicActor_ptr ballActor = PDynamicActor::CreateActor(glm::vec3(0.0f, 10.0f, 0.0f));
	ballActor->SetGravity(true);
	PSphereGeometry_ptr ballGeometry = PSphereGeometry::CreateGeometry(1.0f);
	PMaterial_ptr ballMaterial = PMaterial::CreateMaterial(0.5f, 0.5f, 1.5f); // play with parameters
	PCollider_ptr ballCollider = PCollider::CreateCollider(ballGeometry, ballMaterial);
	ballActor->AddCollider(ballCollider);
	scene->AddActor((ballActor));


	PStaticActor_ptr cubeActor1 = PStaticActor::CreateActor(glm::vec3(-1.1f, 0.0f, 0.0f));
	PStaticActor_ptr cubeActor2 = PStaticActor::CreateActor(glm::vec3(4.0f, 0.0f, 0.0f));
	
	PBoxGeometry_ptr cubeGeometry = PBoxGeometry::CreateGeometry(glm::vec3(1.0f, 1.0f, 1.0f));
	PMaterial_ptr cubeMaterial = PMaterial::CreateMaterial(0.5f, 0.5f, 0.0f);
	PCollider_ptr cubeCollider = PCollider::CreateCollider(cubeGeometry);
	
	cubeActor1->AddCollider(cubeCollider);
	cubeActor2->AddCollider(cubeCollider);
	scene->AddActor(cubeActor1);
	scene->AddActor(cubeActor2);

	EmptyGameLoop(scene, time);

	cubeActor1->RemoveCollider(cubeCollider);
	cubeActor2->RemoveCollider(cubeCollider);
	ballActor->RemoveCollider(std::move(ballCollider));

	scene->RemoveActor((ballActor));
	scene->RemoveActor((cubeActor1));
	scene->RemoveActor((cubeActor2));
}
// Applying forces & torques
void PhysicsTester::Test8(PScene_ptr scene, float time) {
	PDynamicActor_ptr actor = PDynamicActor::CreateActor(glm::vec3(0.0f, 10.0f, 0.0f));
	actor->SetGravity(false);

	PSphereGeometry_ptr geometry = PSphereGeometry::CreateGeometry(1.0f);
	PCollider_ptr collider = PCollider::CreateCollider(geometry);
	actor->AddCollider(collider);
	scene->AddActor(actor);

	auto startTime = std::chrono::high_resolution_clock::now();
	while (true) {
		scene->Update(0.01f);
		scene->ApplyUpdateResults(true);

		auto crntTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = crntTime - startTime;
		float elapsedTime = (float)duration.count();

		actor->LockMotion(PDynamicActor::MotionAxis::ANGULAR_X);
		if (elapsedTime < 1.0f)	{
			actor->ApplyTorque(glm::vec3(1.0f, 0.0f, 0.0f));
		} 
		else if (elapsedTime < 2.0f) {	
			actor->ApplyTorque(glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (elapsedTime > time) break;
	}

	scene->RemoveActor(actor);
}
// Collision filteration
void PhysicsTester::Test9(PScene_ptr scene, float time) {
	PDynamicActor_ptr ballActor = PDynamicActor::CreateActor(glm::vec3(0.0f, 8.0f, 0.0f));
	PSphereGeometry_ptr ballGeometry = PSphereGeometry::CreateGeometry(1.0f);
	PCollider_ptr ballCollider = PCollider::CreateCollider(ballGeometry);
	ballActor->AddCollider(ballCollider);

	PStaticActor_ptr cubeActor1 = PStaticActor::CreateActor(glm::vec3(0.0f, 5.0f, 0.0f));
	PStaticActor_ptr cubeActor2 = PStaticActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
	PStaticActor_ptr cubeActor3 = PStaticActor::CreateActor(glm::vec3(0.0f, -5.0f, 0.0f));
	PBoxGeometry_ptr cubeGeometry = PBoxGeometry::CreateGeometry(glm::vec3(0.5f, 0.5f, 0.5f));
	
	/// Use seperate colliders because we'll need different filter data
	PCollider_ptr cubeCollider1 = PCollider::CreateCollider(cubeGeometry);
	cubeCollider1->SetSimulation(false);
	cubeCollider1->SetTrigger(true);
	cubeActor1->AddCollider(cubeCollider1);

	PCollider_ptr cubeCollider2 = PCollider::CreateCollider(cubeGeometry);
	cubeCollider2->SetCollisionCategory(1);
	cubeActor2->AddCollider(cubeCollider2);
	PCollider_ptr cubeCollider3 = PCollider::CreateCollider(cubeGeometry);
	cubeActor3->AddCollider(cubeCollider3);

	scene->SetCollisionRelationship(0, 1, PScene::CollisionHandle::Trigger, PScene::PairFlag::Default); // collisions between categories 0 & 1 will behave like trigger volumes


	scene->AddActor(ballActor);
	scene->AddActor(cubeActor1);
	scene->AddActor(cubeActor2);
	scene->AddActor(cubeActor3);

	EmptyGameLoop(scene, time);

	ballActor->RemoveCollider(std::move(ballCollider));
	cubeActor1->RemoveCollider(std::move(cubeCollider1));
	cubeActor2->RemoveCollider(std::move(cubeCollider2));
	cubeActor3->RemoveCollider(std::move(cubeCollider3));

	scene->RemoveActor(ballActor);
	scene->RemoveActor(cubeActor1);
	scene->RemoveActor(cubeActor2);
	scene->RemoveActor(cubeActor3);
}
// Joints
void PhysicsTester::Test10(PScene_ptr scene, float time) {
	scene->SetJointVisualization(true);
	
	PSphereGeometry_ptr geometry = PSphereGeometry::CreateGeometry(1.0f);
	PCollider_ptr collider = PCollider::CreateCollider(geometry);

	PDynamicActor_ptr actor1 = PDynamicActor::CreateActor(glm::vec3(0.0f, 3.0f, 0.0f));
	PDynamicActor_ptr actor2 = PDynamicActor::CreateActor(glm::vec3(0.0f, 0.0f, 0.0f));
	
	actor1->SetGravity(false);
	actor2->SetGravity(false);

	actor1->AddCollider(collider);
	actor2->AddCollider(collider);

	scene->AddActor(actor1);
	scene->AddActor(actor2);

	auto reSetTransforms = [&] {
		actor1->SetPosition(glm::vec3(0.0f, 3.0f, 0.0f));
		actor1->SetLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		actor1->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

		actor2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		actor2->SetLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		actor2->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	};
	
	// Fixed Joint
	{
		PFixedJoint_ptr joint = PFixedJoint::CreateJoint(actor1, actor2);
		joint->SetVisualization(true);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->Update(0.01f);
			scene->ApplyUpdateResults(false);

			actor2->ApplyForce(glm::vec3(0.01f, 0.0f, 0.0f));
			actor2->ApplyTorque(glm::vec3(0.01f, 0.0f, 0.0f));

			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;
		}
	}
	reSetTransforms();
	
	// Distance Joint
	{
		PDistanceJoint_ptr joint = PDistanceJoint::CreateJoint(actor1, actor2);
		joint->SetVisualization(true);
		joint->SetLimits(0.0f, 5.0f, 0.1f);
		joint->SetThresholdForceAndTorque(10.0f, 10.0f);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->Update(0.01f);
			scene->ApplyUpdateResults(false);

			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;

			if (elapsedTime.count() < time / 7.5f) {
				actor2->ApplyForce(glm::vec3(0.01f, 0.0f, 0.0f));
				actor2->ApplyTorque(glm::vec3(0.01f, 0.0f, 0.0f));
			}
			else {
				actor2->ApplyForce(glm::vec3(12.0f, 0.0f, 0.0f));
			}
		}
	}
	reSetTransforms();
	
	// Spherical Joint
	{
		PSphericalJoint_ptr joint = PSphericalJoint::CreateJoint(actor1, actor2);
		joint->SetVisualization(true);
		joint->SetLimits(70.0f * 3.14f / 180.0f, 20.0f * 3.14f / 180.0f, 0.1f);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->Update(0.01f);
			scene->ApplyUpdateResults(false);

			actor1->ApplyTorque(glm::vec3(0.01f, 0.01f, 0.01f));
			actor2->ApplyTorque(glm::vec3(0.01f, 0.01f, 0.01f));
			actor2->ApplyForce(glm::vec3(0.01f, 0.0f, 0.0f));

			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;
		}
	}
	reSetTransforms();

	// Revolute Joint
	{
		PRevoluteJoint_ptr joint = PRevoluteJoint::CreateJoint(actor1, actor2);
		joint->SetVisualization(true);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->Update(0.01f);
			scene->ApplyUpdateResults(false);

			actor1->ApplyTorque(glm::vec3(1.0f, 0.1f, 0.1f));
			
			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;
		}
	}
	reSetTransforms();

	// Prismatic Joint
	{
		PPrismaticJoint_ptr joint = PPrismaticJoint::CreateJoint(actor1, actor2);
		joint->SetVisualization(true);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->Update(0.01f);
			scene->ApplyUpdateResults(false);

			actor1->ApplyForce(glm::vec3(-0.01f, 0.0f, 0.0f));
			actor2->ApplyForce(glm::vec3(0.01f, 0.0f, 0.0f));

			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;
		}
	}
}
// Scene queries
void PhysicsTester::Test11(PScene_ptr scene) {
	PStaticActor_ptr actor = PStaticActor::CreateActor();
	PBoxGeometry_ptr geometry = PBoxGeometry::CreateGeometry();
	PCollider_ptr collider = PCollider::CreateCollider(geometry);

	actor->AddCollider(collider);
	scene->AddActor(actor);

	/// Raycasting
	{
		/// Single Hit
		PRaycastHit_ptr RaycastHit = std::make_shared<PRaycastHit>();
		bool bHit = scene->Raycast(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, RaycastHit);
		if (bHit) {
			float dist = RaycastHit->GetDistance();
			glm::vec3 norm = RaycastHit->GetNormal();
			glm::vec3 pos = RaycastHit->GetPosition();

			PActor_ptr hitActor = RaycastHit->GetActor();
			PCollider_ptr hitCollider = RaycastHit->GetCollider();
		}

		/// Multiple Hits
		std::vector<PRaycastHit_ptr> RaycastHits;
		bHit = scene->Raycast(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, RaycastHits);
		if (bHit) {
			for (PRaycastHit_ptr hit : RaycastHits) {
				float dist = hit->GetDistance();
				glm::vec3 norm = hit->GetNormal();
				glm::vec3 pos = hit->GetPosition();

				PActor_ptr hitActor = hit->GetActor();
				PCollider_ptr hitCollider = hit->GetCollider();
			}
		}
	}

	/// Sweeping
	{
		/// Single Hit
		PSweepHit_ptr SweepHit = std::make_shared<PSweepHit>();
		bool bHit = scene->Sweep(geometry, glm::vec3(0.0f, 0.0f, 5.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, SweepHit);
		if (bHit) {
			PActor_ptr hitActor = SweepHit->GetActor();
			PCollider_ptr hitCollider = SweepHit->GetCollider();
			float dist = SweepHit->GetDistance();
			glm::vec3 norm = SweepHit->GetNormal();
			glm::vec3 pos = SweepHit->GetPosition();
		}

		/// Multiple Hits
		std::vector<PSweepHit_ptr> SweepHits;
		bHit = scene->Sweep(geometry, glm::vec3(0.0f, 0.0f, 5.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, SweepHits);
		if (bHit) {
			for (PSweepHit_ptr hit : SweepHits) {
				float dist = hit->GetDistance();
				glm::vec3 norm = hit->GetNormal();
				glm::vec3 pos = hit->GetPosition();

				PActor_ptr hitActor = hit->GetActor();
				PCollider_ptr hitCollider = hit->GetCollider();
			}
		}
	}

	/// Overlap
	{
		PGeometry_ptr OverlapGeometry = PBoxGeometry::CreateGeometry(glm::vec3(2.0f, 2.0f, 2.0f));

		/// SINGLE HIT ALWAYS RETURNS FALSE ?

		/// Multiple Hits
		std::vector<POverlapHit_ptr> OverlapHits;
		bool bHit = scene->Overlap(OverlapGeometry, glm::vec3(0.0f, 0.0f, 2.5f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), OverlapHits);
		if (bHit) {
			for (POverlapHit_ptr hit : OverlapHits) {
				PActor_ptr hitActor = hit->GetActor();
				PCollider_ptr hitCollider = hit->GetCollider();
			}
		}
	}
}
#pragma endregion

void PhysicsTester::RunTests() {
	PhysicsSettings Settings;
	{
		#ifdef _DEBUG
		Settings.bEnableVisualDebugger = true;
		Settings.bTrackMemoryAllocations = true;
		#endif
		Settings.bEnableHeightFields = true;
	}
	PhysicsEngine::GetInstance().Init(Settings);
	{
		PSceneDescriptor desc;
		{
			desc.SetGravityForce(glm::vec3(0.0f, -0.5f, 0.0f));
		}
		PScene_ptr scene = PScene::CreateScene(desc);

		Test0(scene, 2.0f);
		Test1(scene, 2.0f);
		Test2(scene, 2.0f);
		Test3(scene, 2.0f);
		Test4(scene, 2.0f);
		Test5(scene, 2.0f);
		Test6(scene, 2.0f);
		Test7(scene, 2.0f);
		Test8(scene, 2.0f);
		Test9(scene, 2.0f);
		Test10(scene, 10.0f);
		Test11(scene);
	}
	PhysicsEngine::GetInstance().Terminate();
}