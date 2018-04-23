#include "FComponent.h"  
 
FComponent::FComponent(Entidad* pEnt, float altoCaj, float anchoCaj, float profCaj, std::string nombreNodo, bool suelo, tipoFisica type, int masa):Componente(pEnt),masa(masa),altoCaja(altoCaj),anchoCaja(anchoCaj), profCaja(profCaj) {
	tipo = type;

	Ogre::AxisAlignedBox bbox;
	//Si est� vinculado a un componente gr�fico
	if (nombreNodo != " ") {
		//Muestra la caja en Ogre
		pEntidad->getPEstado()->getScnManager()->getSceneNode(nombreNodo)->showBoundingBox(true);
		pEntidad->getPEstado()->getScnManager()->getSceneNode(nombreNodo)->_update(true, true);

		//Le pone al cuerpo coordenadas, orientaci�n y volumen (sacado del componente gr�fico)
		Ogre::Vector3 posN = pEntidad->getPEstado()->getScnManager()->getSceneNode("GNode"+nombreNodo)->getPosition();
		bbox = pEntidad->getPEstado()->getScnManager()->getSceneNode(nombreNodo)->_getWorldAABB();
		Ogre::Vector3  v = bbox.getSize();
		altoCaja = v.y;
		profCaja = v.z;
		anchoCaja = v.x;
		pTransform.setIdentity();
		//Ogre::Vector3 centro = bbox.getCenter();
		pTransform.setOrigin(btVector3(posN.x, posN.y, posN.z));
		Ogre::Quaternion quat = pEntidad->getPEstado()->getScnManager()->getSceneNode("GNode" + nombreNodo)->getOrientation();
		pTransform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));//Tener en cuenta que en ogre el primer valor es w, mientras que en bullet va �ltimo.
		initBody();

		//Vincula el nodo gr�fico al f�sico
		body->setUserPointer(pEntidad->getPEstado()->getScnManager()->getSceneNode(nombreNodo));

		//Para poder acceder desde Fisic a los rigidbodies
		pEntidad->getPEstado()->getFisicManager()->addBodyToMap(nombreNodo, body);
	}

	//Si no est� vinculado a un componente gr�fico se crea por defecto en el 0,0,0 se puede reposicionar con un mensaje
	else {
		tipo = tipoFisica::Estatico;
		pTransform.setIdentity();
		pTransform.setOrigin(btVector3(0.0, 0.0, 0.0));
		initBody();
	}
	actualizaNodo();
} 

FComponent::~FComponent() { 
	delete body;
	delete motionState;
	delete shape;
} 

void FComponent::initBody() {
	//Aqu� ajustamos la masa
	if (tipo != tipoFisica::Estatico)
		mass = masa;
	else
		mass = 0;
	//La inercia inicial siempre es 0
	btVector3 localInertia(0, 0, 0);

	//Aqu� hacemos la forma de la caja
	shape = new btBoxShape(btVector3(btScalar(anchoCaja/2), btScalar(altoCaja/2), btScalar(profCaja/2)));

	//Aqu� se inicializa el cuerpo en base a sus par�metros anteriores
	motionState = new btDefaultMotionState(pTransform);
	shape->calculateLocalInertia(mass, localInertia);
	pEntidad->getPEstado()->getFisicManager()->getCollisionShapes().push_back(shape);
	btRigidBody::btRigidBodyConstructionInfo RBInfo(mass, motionState, shape, localInertia);
	body = new btRigidBody(RBInfo);

	//Elasticidad del material
	body->setRestitution(0);

	pEntidad->getPEstado()->getFisicManager()->getDynamicsWorld()->addRigidBody(body);
}

void FComponent::Update(float deltaTime, Mensaje const & msj) {
	Mensaje msg = msj;
	Componente::Update(deltaTime, msj);

	if (msg.getTipo() == Tipo::Fisica) {
		if (msg.getSubTipo() == SubTipo::Reposicionar) {
			int pos = msg.getMsg().find("/");
			std::string xS = msg.getMsg().substr(0, pos);
			std::string subcad = msg.getMsg().substr(pos + 1);
			pos = subcad.find("/");
			std::string yS = subcad.substr(0, pos);
			std::string zS = subcad.substr(pos + 1);
			//Aqu� le asignamos el transform que deber�a tener la entidad
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3(std::stof(xS), std::stof(yS), std::stof(zS)));
			body->setWorldTransform(t);
			//FALTA LA ORIENTACI�N!!!
		}
	}
	std::string ms = "";
	Mensaje * m;
	
	switch (tipo)
	{
	case Dinamico:
		break;

	//Este ser�a el caso kinematico concrero de la ni�a
	//Para los enemigos habr�a que haacer otro
	case Kinematico:
		
		if (msg.getTipo() == Tipo::Fisica) {
			if (msg.getSubTipo() == SubTipo::Mover) {

				//Aqu� lo movemos con la info procedente del input
				int pos = msg.getMsg().find("/");
				std::string xS = msg.getMsg().substr(0, pos);
				std::string subcad = msg.getMsg().substr(pos + 1);
				pos = subcad.find("/");
				std::string yS = subcad.substr(0, pos);
				std::string zS = subcad.substr(pos + 1);
				float xF= std::stof(xS);
				if (xF > 0) xF *= 50;
				else if (xF < 0) xF *=50;
				float zF = std::stof(zS);
				if (zF > 0) zF *= 50;
				else if (zF < 0) zF *= 50;
				btVector3 vel = body->getLinearVelocity();
				vel = vel + btVector3(xF,0,zF);
				body->setLinearVelocity(vel);
			}
		}
		else {
			btVector3 vel = body->getLinearVelocity();
			vel = vel*btVector3(0, 0, 0);
			body->setLinearVelocity(vel);
		}
		break;
	}
	actualizaNodo();
	
}

//Actualiza el nodo gr�fico en base a los cambios del f�sico
void FComponent:: actualizaNodo() {
	userPointer = body->getUserPointer();
	if (userPointer) {
		btQuaternion orientation = body->getOrientation();
		btVector3 position = body->getWorldTransform().getOrigin();
		Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
		sceneNode->setPosition(Ogre::Vector3(position.getX(), position.getY(), position.getZ()));
		sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
	}
}