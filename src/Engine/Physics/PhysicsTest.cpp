
#include "Pch.h"
#include "Engine/Physics/PhysicsTest.h"


void PhysicsTester::EmptyGameLoop(PScene_ptr scene, float time) {
	auto startTime = std::chrono::high_resolution_clock::now();
	while (true) {
		scene->update(0.01f);
		scene->applyUpdateResults(true);

		auto crntTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = crntTime - startTime;
		if (elapsedTime.count() > time) break;
	}
}

#pragma region Tests
// Pointers, Destructors, & Shit
void PhysicsTester::Test0(PScene_ptr scene, float time) {
	PBoxGeometry_ptr geometry = PBoxGeometry::createGeometry(glm::vec3(1.0f, 1.0f, 1.0f));
	// A
	{
		PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
		PCollider_ptr sharedCollider = PCollider::createCollider(geometry);
		actor->addCollider(sharedCollider);
		actor->removeCollider(sharedCollider);
	}
	// B
	{
		PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
		PCollider_ptr sharedCollider = PCollider::createCollider(geometry);
		actor->addCollider(sharedCollider);
	}
	// C
	{
		PCollider_ptr sharedCollider = PCollider::createCollider(geometry);
		{
			PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
			actor->addCollider(sharedCollider);
		}
	}
	// D
	{
		PCollider_ptr sharedCollider = PCollider::createCollider(geometry);
		{
			PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
			actor->addCollider(sharedCollider);
			actor->removeCollider(sharedCollider);
		}
	}
	// E
	{
		PStaticActor_ptr actor1 = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
		{
			PStaticActor_ptr actor2 = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
			PCollider_ptr sharedCollider = PCollider::createCollider(geometry);
			actor1->addCollider(sharedCollider);
			actor2->addCollider(sharedCollider);
			actor1->removeCollider(sharedCollider);
		}
	}
	// F 
	{
		PStaticActor_ptr actor1 = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
		PStaticActor_ptr actor2 = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
		{
			PCollider_ptr sharedCollider = PCollider::createCollider(geometry);
			actor1->addCollider(sharedCollider);
			actor2->addCollider(sharedCollider);
			scene->addActor(actor1);
			scene->addActor(actor2);
			EmptyGameLoop(scene, time);
		}
		EmptyGameLoop(scene, time);
		scene->removeActor(actor1);
		scene->removeActor(actor2);
	}
}
// Capsule
void PhysicsTester::Test1(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));

	PCapsuleGeometry_ptr geometry = PCapsuleGeometry::createGeometry(1.0f, 2.0f);
	PCollider_ptr collider = PCollider::createCollider(geometry);
	collider->rotate(90.0f * 3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	actor->addCollider(collider);
	scene->addActor(actor);

	EmptyGameLoop(scene, time);

	scene->removeActor(actor);
}
 //Plane
void PhysicsTester::Test2(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f)); // must be static

	PPlaneGeometry_ptr geometry = PPlaneGeometry::createGeometry();

	PCollider_ptr collider = PCollider::createCollider(geometry); // collider ref cnt = 1 (exclusive collider)
	collider->rotate(90.0f * 3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	actor->addCollider(collider);
	
	scene->addActor(actor);

	EmptyGameLoop(scene, time);
	
	scene->removeActor(actor);
	actor->removeCollider(std::move(collider)); // DON'T FORGET
}// collider ref cnt = 0
// Actor with multiple colliders
void PhysicsTester::Test3(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));

	PSphereGeometry_ptr geometry1 = PSphereGeometry::createGeometry(1.0f);
	PCollider_ptr collider1 = PCollider::createCollider(geometry1);
	collider1->setLocalPosition(glm::vec3(0.0f, 12.0f, 0.0f));
	actor->addCollider(collider1);

	PSphereGeometry_ptr geometry2 = PSphereGeometry::createGeometry(3.0f);
	PCollider_ptr collider2 = PCollider::createCollider(geometry2);
	collider2->setLocalPosition(glm::vec3(0.0f, 8.0f, 0.0f));
	actor->addCollider(collider2);

	PSphereGeometry_ptr geometry3 = PSphereGeometry::createGeometry(5.0f);
	PCollider_ptr collider3 = PCollider::createCollider(geometry3);
	actor->addCollider(collider3);

	scene->addActor(actor);

	EmptyGameLoop(scene, time);

	scene->removeActor(actor);
}
// Convex Mesh
void PhysicsTester::Test4(PScene_ptr scene, float time) {
	PDynamicActor_ptr actor = PDynamicActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
	actor->setGravity(false);

	PConvexMeshDescriptor desc;
	{
		desc.setPointsCount(5);
		desc.setPointsStride(3 * sizeof(float));
		float vertices[] = {
			+0.0f, +1.0f, +0.0f,
			+1.0f, +0.0f, +0.0f,
			-1.0f, +0.0f, +0.0f,
			+0.0f, +0.0f, +1.0f,
			+0.0f, +0.0f, -1.0f,
		};
		desc.setPointsData(vertices);
		desc.enableAutomaticHullComputation();
	}
	PConvexMeshGeometry_ptr geometry = PConvexMeshGeometry::createGeometry(desc);
	PCollider_ptr collider = PCollider::createCollider(geometry);
	
	actor->addCollider(collider);
	scene->addActor(actor);

	EmptyGameLoop(scene, time);

	actor->removeCollider(std::move(collider));
	scene->removeActor(actor);
}
// Triangle Mesh
void PhysicsTester::Test5(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f)); // must be static

	PTriangleMeshDescriptor desc;
	{
		desc.setPointsCount(5);
		desc.setPointsStride(3 * sizeof(float));
		float vertices[] {
			+0.0f, +1.0f, +0.0f,
			+1.0f, +0.0f, +0.0f,
			-1.0f, +0.0f, +0.0f,
			+0.0f, +0.0f, +1.0f,
			+0.0f, +0.0f, -1.0f,
		};
		desc.setPointsData(vertices);

		desc.setTrianglesCount(6);
		desc.setTrianglesStride(3 * sizeof(uint32_t));
		uint32_t triangles[] {
			0, 1, 4,
			0, 4, 2,
			0, 2, 3,
			0, 3, 1,
			1, 2, 4,
			1, 3, 2
		};
		desc.setTrianglesData(triangles);
	}
	PTriangleMeshGeometry_ptr geometry = PTriangleMeshGeometry::createGeometry(desc); // the actor must be static or (dynamic & kinematic)
	PCollider_ptr collider = PCollider::createCollider(geometry);

	actor->addCollider(collider);
	scene->addActor(actor);

	EmptyGameLoop(scene, time);

	actor->removeCollider(std::move(collider));
	scene->removeActor(actor);
}
// Height Field
void PhysicsTester::Test6(PScene_ptr scene, float time) {
	PStaticActor_ptr actor = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f)); // the actor must be static

	PHeightFieldStructure structure = PHeightFieldStructure(64, 64, 0.05f);
	{
		for (int r = 0; r < structure.getNumOfRows(); r++) {
			for (int c = 0; c < structure.getNumOfCols(); c++) {
				structure.setVertexHeight(r, c, rand() % 15);
				structure.setVertexMaterials(r, c, rand() % 3, rand() % 3);
			}
		}
	}
	PHeightFieldDescriptor desc = PHeightFieldDescriptor(structure);
	PHeightFieldGeometry_ptr geometry = PHeightFieldGeometry::createGeometry(desc);
	PMaterialSet_ptr materials = PMaterialSet::createMaterialSet(std::vector<PMaterialProperties>{{0.5f, 0.5f, 0.5}, {1.0f, 0.2f, 0.4f}, {0.6f, 0.8f, 0.8f}});
	PCollider_ptr collider = PCollider::createCollider(geometry, materials);

	actor->addCollider(collider);
	scene->addActor(actor);

	EmptyGameLoop(scene, time);

	actor->removeCollider(std::move(collider));
	scene->removeActor(actor);
}
// Falling sphere on boxes
void PhysicsTester::Test7(PScene_ptr scene, float time) {
	PDynamicActor_ptr ballActor = PDynamicActor::createActor(glm::vec3(0.0f, 10.0f, 0.0f));
	ballActor->setGravity(true);
	PSphereGeometry_ptr ballGeometry = PSphereGeometry::createGeometry(1.0f);
	PMaterial_ptr ballMaterial = PMaterial::createMaterial(0.5f, 0.5f, 1.5f); // play with parameters
	PCollider_ptr ballCollider = PCollider::createCollider(ballGeometry, ballMaterial);
	ballActor->addCollider(ballCollider);
	scene->addActor((ballActor));


	PStaticActor_ptr cubeActor1 = PStaticActor::createActor(glm::vec3(-1.1f, 0.0f, 0.0f));
	PStaticActor_ptr cubeActor2 = PStaticActor::createActor(glm::vec3(4.0f, 0.0f, 0.0f));
	
	PBoxGeometry_ptr cubeGeometry = PBoxGeometry::createGeometry(glm::vec3(1.0f, 1.0f, 1.0f));
	PMaterial_ptr cubeMaterial = PMaterial::createMaterial(0.5f, 0.5f, 0.0f);
	PCollider_ptr cubeCollider = PCollider::createCollider(cubeGeometry);
	
	cubeActor1->addCollider(cubeCollider);
	cubeActor2->addCollider(cubeCollider);
	scene->addActor(cubeActor1);
	scene->addActor(cubeActor2);

	EmptyGameLoop(scene, time);

	cubeActor1->removeCollider(cubeCollider);
	cubeActor2->removeCollider(cubeCollider);
	ballActor->removeCollider(std::move(ballCollider));

	scene->removeActor((ballActor));
	scene->removeActor((cubeActor1));
	scene->removeActor((cubeActor2));
}
// Applying forces & torques
void PhysicsTester::Test8(PScene_ptr scene, float time) {
	PDynamicActor_ptr actor = PDynamicActor::createActor(glm::vec3(0.0f, 10.0f, 0.0f));
	actor->setGravity(false);

	PSphereGeometry_ptr geometry = PSphereGeometry::createGeometry(1.0f);
	PCollider_ptr collider = PCollider::createCollider(geometry);
	actor->addCollider(collider);
	scene->addActor(actor);

	auto startTime = std::chrono::high_resolution_clock::now();
	while (true) {
		scene->update(0.01f);
		scene->applyUpdateResults(true);

		auto crntTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = crntTime - startTime;
		float elapsedTime = (float)duration.count();

		actor->lockMotion(PDynamicActor::MotionAxis::ANGULAR_X);
		if (elapsedTime < 1.0f)	{
			actor->applyTorque(glm::vec3(1.0f, 0.0f, 0.0f));
		} 
		else if (elapsedTime < 2.0f) {	
			actor->applyTorque(glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (elapsedTime > time) break;
	}

	scene->removeActor(actor);
}
// Collision filteration
void PhysicsTester::Test9(PScene_ptr scene, float time) {
	PDynamicActor_ptr ballActor = PDynamicActor::createActor(glm::vec3(0.0f, 8.0f, 0.0f));
	PSphereGeometry_ptr ballGeometry = PSphereGeometry::createGeometry(1.0f);
	PCollider_ptr ballCollider = PCollider::createCollider(ballGeometry);
	ballActor->addCollider(ballCollider);

	PStaticActor_ptr cubeActor1 = PStaticActor::createActor(glm::vec3(0.0f, 5.0f, 0.0f));
	PStaticActor_ptr cubeActor2 = PStaticActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
	PStaticActor_ptr cubeActor3 = PStaticActor::createActor(glm::vec3(0.0f, -5.0f, 0.0f));
	PBoxGeometry_ptr cubeGeometry = PBoxGeometry::createGeometry(glm::vec3(0.5f, 0.5f, 0.5f));
	
	/// Use seperate colliders because we'll need different filter data
	PCollider_ptr cubeCollider1 = PCollider::createCollider(cubeGeometry);
	cubeCollider1->setSimulation(false);
	cubeCollider1->setTrigger(true);
	cubeActor1->addCollider(cubeCollider1);

	PCollider_ptr cubeCollider2 = PCollider::createCollider(cubeGeometry);
	cubeCollider2->setCollisionCategory(1);
	cubeActor2->addCollider(cubeCollider2);
	PCollider_ptr cubeCollider3 = PCollider::createCollider(cubeGeometry);
	cubeActor3->addCollider(cubeCollider3);

	scene->setCollisionRelationship(0, 1, PScene::CollisionHandle::Trigger, PScene::PairFlag::Default); // collisions between categories 0 & 1 will behave like trigger volumes


	scene->addActor(ballActor);
	scene->addActor(cubeActor1);
	scene->addActor(cubeActor2);
	scene->addActor(cubeActor3);

	EmptyGameLoop(scene, time);

	ballActor->removeCollider(std::move(ballCollider));
	cubeActor1->removeCollider(std::move(cubeCollider1));
	cubeActor2->removeCollider(std::move(cubeCollider2));
	cubeActor3->removeCollider(std::move(cubeCollider3));

	scene->removeActor(ballActor);
	scene->removeActor(cubeActor1);
	scene->removeActor(cubeActor2);
	scene->removeActor(cubeActor3);
}
// Joints
void PhysicsTester::Test10(PScene_ptr scene, float time) {
	scene->setJointVisualization(true);
	
	PSphereGeometry_ptr geometry = PSphereGeometry::createGeometry(1.0f);
	PCollider_ptr collider = PCollider::createCollider(geometry);

	PDynamicActor_ptr actor1 = PDynamicActor::createActor(glm::vec3(0.0f, 3.0f, 0.0f));
	PDynamicActor_ptr actor2 = PDynamicActor::createActor(glm::vec3(0.0f, 0.0f, 0.0f));
	
	actor1->setGravity(false);
	actor2->setGravity(false);

	actor1->addCollider(collider);
	actor2->addCollider(collider);

	scene->addActor(actor1);
	scene->addActor(actor2);

	auto resetTransforms = [&] {
		actor1->setPosition(glm::vec3(0.0f, 3.0f, 0.0f));
		actor1->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		actor1->setAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

		actor2->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		actor2->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		actor2->setAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	};
	
	// Fixed Joint
	{
		PFixedJoint_ptr joint = PFixedJoint::createJoint(actor1, actor2);
		joint->setVisualization(true);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->update(0.01f);
			scene->applyUpdateResults(false);

			actor2->applyForce(glm::vec3(0.01f, 0.0f, 0.0f));
			actor2->applyTorque(glm::vec3(0.01f, 0.0f, 0.0f));

			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;
		}
	}
	resetTransforms();
	
	// Distance Joint
	{
		PDistanceJoint_ptr joint = PDistanceJoint::createJoint(actor1, actor2);
		joint->setVisualization(true);
		joint->setLimits(0.0f, 5.0f, 0.1f);
		joint->setThresholdForceAndTorque(10.0f, 10.0f);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->update(0.01f);
			scene->applyUpdateResults(false);

			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;

			if (elapsedTime.count() < time / 7.5f) {
				actor2->applyForce(glm::vec3(0.01f, 0.0f, 0.0f));
				actor2->applyTorque(glm::vec3(0.01f, 0.0f, 0.0f));
			}
			else {
				actor2->applyForce(glm::vec3(12.0f, 0.0f, 0.0f));
			}
		}
	}
	resetTransforms();
	
	// Spherical Joint
	{
		PSphericalJoint_ptr joint = PSphericalJoint::createJoint(actor1, actor2);
		joint->setVisualization(true);
		joint->setLimits(70.0f * 3.14f / 180.0f, 20.0f * 3.14f / 180.0f, 0.1f);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->update(0.01f);
			scene->applyUpdateResults(false);

			actor1->applyTorque(glm::vec3(0.01f, 0.01f, 0.01f));
			actor2->applyTorque(glm::vec3(0.01f, 0.01f, 0.01f));
			actor2->applyForce(glm::vec3(0.01f, 0.0f, 0.0f));

			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;
		}
	}
	resetTransforms();

	// Revolute Joint
	{
		PRevoluteJoint_ptr joint = PRevoluteJoint::createJoint(actor1, actor2);
		joint->setVisualization(true);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->update(0.01f);
			scene->applyUpdateResults(false);

			actor1->applyTorque(glm::vec3(1.0f, 0.1f, 0.1f));
			
			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;
		}
	}
	resetTransforms();

	// Prismatic Joint
	{
		PPrismaticJoint_ptr joint = PPrismaticJoint::createJoint(actor1, actor2);
		joint->setVisualization(true);

		auto startTime = std::chrono::high_resolution_clock::now();
		while (true) {
			scene->update(0.01f);
			scene->applyUpdateResults(false);

			actor1->applyForce(glm::vec3(-0.01f, 0.0f, 0.0f));
			actor2->applyForce(glm::vec3(0.01f, 0.0f, 0.0f));

			auto crntTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = crntTime - startTime;
			if (elapsedTime.count() > time / 5.0f) break;
		}
	}
}
// Scene queries
void PhysicsTester::Test11(PScene_ptr scene) {
	PStaticActor_ptr actor = PStaticActor::createActor();
	PBoxGeometry_ptr geometry = PBoxGeometry::createGeometry();
	PCollider_ptr collider = PCollider::createCollider(geometry);

	actor->addCollider(collider);
	scene->addActor(actor);

	/// Raycasting
	{
		/// Single Hit
		PRaycastHit_ptr raycastHit = std::make_shared<PRaycastHit>();
		bool bHit = scene->raycast(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, raycastHit);
		if (bHit) {
			float dist = raycastHit->getDistance();
			glm::vec3 norm = raycastHit->getNormal();
			glm::vec3 pos = raycastHit->getPosition();

			PActor_ptr hitActor = raycastHit->getActor();
			PCollider_ptr hitCollider = raycastHit->getCollider();
		}

		/// Multiple Hits
		std::vector<PRaycastHit_ptr> raycastHits;
		bHit = scene->raycast(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, raycastHits);
		if (bHit) {
			for (PRaycastHit_ptr hit : raycastHits) {
				float dist = hit->getDistance();
				glm::vec3 norm = hit->getNormal();
				glm::vec3 pos = hit->getPosition();

				PActor_ptr hitActor = hit->getActor();
				PCollider_ptr hitCollider = hit->getCollider();
			}
		}
	}

	/// Sweeping
	{
		/// Single Hit
		PSweepHit_ptr sweepHit = std::make_shared<PSweepHit>();
		bool bHit = scene->sweep(geometry, glm::vec3(0.0f, 0.0f, 5.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, sweepHit);
		if (bHit) {
			PActor_ptr hitActor = sweepHit->getActor();
			PCollider_ptr hitCollider = sweepHit->getCollider();
			float dist = sweepHit->getDistance();
			glm::vec3 norm = sweepHit->getNormal();
			glm::vec3 pos = sweepHit->getPosition();
		}

		/// Multiple Hits
		std::vector<PSweepHit_ptr> sweepHits;
		bHit = scene->sweep(geometry, glm::vec3(0.0f, 0.0f, 5.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 10.0f, sweepHits);
		if (bHit) {
			for (PSweepHit_ptr hit : sweepHits) {
				float dist = hit->getDistance();
				glm::vec3 norm = hit->getNormal();
				glm::vec3 pos = hit->getPosition();

				PActor_ptr hitActor = hit->getActor();
				PCollider_ptr hitCollider = hit->getCollider();
			}
		}
	}

	/// Overlap
	{
		PGeometry_ptr overlapGeometry = PBoxGeometry::createGeometry(glm::vec3(2.0f, 2.0f, 2.0f));

		/// SINGLE HIT ALWAYS RETURNS FALSE ?

		/// Multiple Hits
		std::vector<POverlapHit_ptr> overlapHits;
		bool bHit = scene->overlap(overlapGeometry, glm::vec3(0.0f, 0.0f, 2.5f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), overlapHits);
		if (bHit) {
			for (POverlapHit_ptr hit : overlapHits) {
				PActor_ptr hitActor = hit->getActor();
				PCollider_ptr hitCollider = hit->getCollider();
			}
		}
	}
}
#pragma endregion

void PhysicsTester::RunTests() {
	g_pPhysicsEngine = new PhysicsEngine();
	PhysicsSettings settings;
	{
		#ifdef _DEBUG
		settings.bEnableVisualDebugger = true;
		settings.bTrackMemoryAllocations = true;
		#endif
		settings.bEnableHeightFields = true;
	}
	g_pPhysicsEngine->init(settings);
	{
		PSceneDescriptor desc;
		{
			desc.setGravityForce(0.0f, -0.5f, 0.0f);
		}
		PScene_ptr scene = PScene::createScene(desc);

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
	g_pPhysicsEngine->terminate();
}