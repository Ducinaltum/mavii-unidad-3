#pragma once
#include <box2d/box2d.h>

class RagDoll
{
private:
	// Cuerpo de box2d
	b2Body* head;
	b2Body* torso;
	b2Body* l_Leg;
	b2Body* r_Leg;
	b2Body* l_arm;
	b2Body* r_arm;
public:
	RagDoll(b2World * world);

	b2DistanceJointDef* CreateJointDefinition(b2Body* partA, b2Body* partB, b2Vec2 anchorA, b2Vec2 anchorB);
};

