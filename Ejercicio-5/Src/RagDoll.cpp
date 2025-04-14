#include "RagDoll.h"
#include <box2d/box2d.h>
#include "../Src/Box2DHelper.h"

RagDoll::RagDoll(b2World * world)
{
	float density = 1.0f;
	float friction = 0.5f;
	float restitution = 0.5f;

	torso = Box2DHelper::CreateRectangularDynamicBody(world, 10, 20, density, friction, restitution);
	torso->SetTransform(b2Vec2(100.0f, 36.0f), 0.0f);

	head = Box2DHelper::CreateCircularDynamicBody(world, 5, density, friction, restitution);
	head->SetTransform(b2Vec2(100.0f, 20.0f), 0.0f);
	b2Vec2 headAnchor(head->GetPosition());
	headAnchor.y += 5;

	l_arm = Box2DHelper::CreateRectangularDynamicBody(world, 5, 20, density, friction, restitution);
	l_arm->SetTransform(b2Vec2(90.0f, 36.0f), 0.0f);
	b2Vec2 l_armAnchor(l_arm->GetPosition());
	l_armAnchor.y -= 10;

	r_arm = Box2DHelper::CreateRectangularDynamicBody(world, 5, 20, density, friction, restitution);
	r_arm->SetTransform(b2Vec2(110.0f, 36.0f), 0.0f);
	b2Vec2 r_armAnchor(r_arm->GetPosition());
	r_armAnchor.y -= 10;

	l_Leg = Box2DHelper::CreateRectangularDynamicBody(world, 5, 20, density, friction, restitution);
	l_Leg->SetTransform(b2Vec2(95.0f, 60.0f), 0.0f);
	b2Vec2 l_LegAnchor(l_Leg->GetPosition());
	l_LegAnchor.y -= 10;
	
	r_Leg = Box2DHelper::CreateRectangularDynamicBody(world, 5, 20, density, friction, restitution);
	r_Leg->SetTransform(b2Vec2(105.0f, 60.0f), 0.0f);
	b2Vec2 r_LegAnchor(r_Leg->GetPosition());
	r_LegAnchor.y -= 10;

	b2Vec2 torsoHeadAnchor(torso->GetPosition());
	torsoHeadAnchor.y -= 10;
	b2Vec2 torsoLArmAnchor(torso->GetPosition());
	torsoLArmAnchor.x -= 5;
	torsoLArmAnchor.y -= 10;
	b2Vec2 torsoRArmAnchor(torso->GetPosition());
	torsoRArmAnchor.x += 5;
	torsoRArmAnchor.y -= 10;
	b2Vec2 torsoLLegAnchor(torso->GetPosition());
	torsoLLegAnchor.x -= 5;
	torsoLLegAnchor.y += 10;
	b2Vec2 torsoRLegAnchor(torso->GetPosition());
	torsoRLegAnchor.x += 5;
	torsoRLegAnchor.y += 10;

	b2DistanceJointDef headTorsoJointDef;
	headTorsoJointDef.Initialize(head, torso, headAnchor, torsoHeadAnchor);
	headTorsoJointDef.length = (headAnchor - torsoHeadAnchor).Length();
	headTorsoJointDef.collideConnected = true;
	world->CreateJoint(&headTorsoJointDef);


	b2DistanceJointDef l_armTorsoJointDef;
	l_armTorsoJointDef.Initialize(l_arm, torso, l_armAnchor, torsoLArmAnchor);
	l_armTorsoJointDef.length = (l_armAnchor - torsoLArmAnchor).Length();
	l_armTorsoJointDef.collideConnected = true;
	world->CreateJoint(&l_armTorsoJointDef);

	b2DistanceJointDef r_armTorsoJointDef;
	r_armTorsoJointDef.Initialize(r_arm, torso, r_armAnchor, torsoRArmAnchor);
	r_armTorsoJointDef.length = (r_armAnchor - torsoRArmAnchor).Length();
	r_armTorsoJointDef.collideConnected = true;
	world->CreateJoint(&r_armTorsoJointDef);

	b2DistanceJointDef l_LegTorsoJointDef;
	l_LegTorsoJointDef.Initialize(l_Leg, torso, l_LegAnchor, torsoLLegAnchor);
	l_LegTorsoJointDef.length = (l_LegAnchor - torsoLLegAnchor).Length();
	l_LegTorsoJointDef.collideConnected = true;
	world->CreateJoint(&l_LegTorsoJointDef);

	b2DistanceJointDef r_LegTorsoJointDef;
	r_LegTorsoJointDef.Initialize(r_Leg, torso, r_LegAnchor, torsoRLegAnchor);
	r_LegTorsoJointDef.length = (r_LegAnchor - torsoRLegAnchor).Length();
	r_LegTorsoJointDef.collideConnected = true;
	world->CreateJoint(&r_LegTorsoJointDef);
}