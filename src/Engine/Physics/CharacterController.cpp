/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_PHYSICS_BULLET

#include "Engine/Physics/CharacterController.h"
#include "Engine/Physics/Shape.h"
#include "Engine/Physics/Convert.h"
#include "Engine/Physics/Physics.h"
#include "Engine/Physics/Body.h"
#include "Engine/Physics/btKinematicController.h"

#include "Core/Math/Helpers.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Transform.h"

#include "Engine/Engine.h"
#include "Engine/Input/InputManager.h"
#include "Engine/Input/Keyboard.h"

#include <BulletCollision/CollisionShapes/btConvexShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h> 
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(CharacterController, Controller)
	FIELD_PRIMITIVE(6, float, stepHeight)
	FIELD_PRIMITIVE(7, float, walkVelocity)
	FIELD_PRIMITIVE(8, float, fallSpeed)
	FIELD_PRIMITIVE(9, float, jumpSpeed)
	FIELD_PRIMITIVE(10, float, maxJumpHeight)
	FIELD_PRIMITIVE(11, float, maxSlope)
REFLECT_CLASS_END()

//-----------------------------------//

CharacterController::CharacterController()
	: controller(nullptr)
	, stepHeight(0.3f)
	, walkVelocity(8.0f)
	, fallSpeed(1.0f)
	, jumpSpeed(1.0f)
	, maxJumpHeight(1.0f)
	, maxSlope(45)
{ }

//-----------------------------------//

CharacterController::~CharacterController()
{
	PhysicsManager* physics = GetEngine()->getPhysicsManager();
	
	btDiscreteDynamicsWorld* world = physics->getWorld();
	
	if( ghostObject )	
		world->removeCollisionObject(ghostObject);

	if( controller )
		world->removeAction(controller);

	delete ghostObject;
	delete controller;
}

//-----------------------------------//

void CharacterController::_update( float delta )
{
	if( !createController() )
		return;

	Engine* engine = GetEngine();
	PhysicsManager* physics = engine->getPhysicsManager();
	btDiscreteDynamicsWorld* world = physics->getWorld();

	if( physics->getSimulation() )
	{
		controller->updateAction(world, (btScalar) delta);
		updateCharacterTransform();
	}

	InputManager* input = engine->getInputManager();
	Keyboard* keyboard = input->getKeyboard();
	
	btVector3 walkDirection;
	walkDirection.setZero();

	if( keyboard->isKeyPressed(Keys::I) )
		walkDirection += Convert::toBullet(  Vector3::UnitZ );
	
	if( keyboard->isKeyPressed(Keys::K) )
		walkDirection += Convert::toBullet( -Vector3::UnitZ );

	if( keyboard->isKeyPressed(Keys::J) )
		walkDirection += Convert::toBullet(  Vector3::UnitX );

	if( keyboard->isKeyPressed(Keys::L) )
		walkDirection += Convert::toBullet( -Vector3::UnitX );
	
	if( keyboard->isKeyPressed(Keys::Space) && controller->canJump() )
		controller->jump();

	btScalar walkSpeed = btScalar(walkVelocity * delta);
	controller->setWalkDirection(walkDirection * walkSpeed);
}

//-----------------------------------//

void CharacterController::updateBulletTransform()
{
	TransformPtr transform = getEntity()->getTransform();
	ghostObject->setWorldTransform(Convert::toBullet(transform));
}

//-----------------------------------//

void CharacterController::updateCharacterTransform()
{
	const btTransform& bullet = ghostObject->getWorldTransform();
	TransformPtr transform = getEntity()->getTransform();
	Convert::fromBullet(bullet, transform);
}

//-----------------------------------//

bool CharacterController::createController()
{
	if( controller )
		return true;

	ShapePtr shape = entity->getComponentFromFamily<Shape>();

	if( !shape )
		return false;

	//if( !shape->getBulletShape()->isConvex() )
		//return false;

	btConvexShape* actualShape = (btConvexShape*) shape->getBulletShape();

	if( !actualShape )
		return false;

	ghostObject = new btPairCachingGhostObject();
	ghostObject->setCollisionShape( actualShape );
	ghostObject->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
	ghostObject->setActivationState( DISABLE_DEACTIVATION );

	controller = new btKinematicController(ghostObject, actualShape, stepHeight);

	updateBulletTransform();
	//updateProperties();

	Engine* engine = GetEngine();
	PhysicsManager* physics = engine->getPhysicsManager();
	btDiscreteDynamicsWorld* world = physics->getWorld();

	world->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
	world->addAction(controller);

	return true;
}

//-----------------------------------//

void CharacterController::updateProperties()
{
	if( !controller )
		return;

	controller->setFallSpeed( fallSpeed );
	controller->setJumpSpeed( jumpSpeed );
	controller->setMaxJumpHeight( maxJumpHeight );
	controller->setMaxSlope( MathDegreeToRadian(maxSlope) );
}

//-----------------------------------//

void CharacterController::onFieldChanged(const Field& field)
{
	updateProperties();
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif