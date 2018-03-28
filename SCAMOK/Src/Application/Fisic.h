#pragma once 
#include "btBulletDynamicsCommon.h"
#include <vector>
#include <map>

//Esta clase tendr� una instancia en el estado
//Regular� todo lo relacionado con el mundo f�sico
class Fisic { 
public: 
	Fisic(); 
	~Fisic(); 
	btDiscreteDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; };
	std::vector<btCollisionShape *> getCollisionShapes() { return collisionShapes; };

private:
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;
}; 
