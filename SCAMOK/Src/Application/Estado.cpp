#include "Estado.h"

Estado::Estado(Ogre::SceneManager * mng, Ogre::RenderWindow* mWindow){

	//Aqui montamos el mundo f�sico
	fisicaManager = new Fisic();

	#pragma region InitOgre 
	mWin = mWindow;
	scnMgr = mng;
	
	scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	scnMgr->setShadowFarDistance(200);


	entidades.insert(std::make_pair("Ogro", new Entidad(this, "sinbad")));
	entidades.insert(std::make_pair("MainCamera", new Entidad(this, "camera")));

	//Este mensaje deber�a ser de transform y configurarlo para que cambie sus referencias locales

	Mensaje msg (Tipo::Fisica, "0/30/0", SubTipo::Reposicionar);
	msg.setMsgInfo(entidades.at("Ogro"), entidades.at("Ogro"));
	mensajes.push(msg);
	 
	Entidad *aux = new Entidad(this); aux->a�adeComponenteGrafico("Arena_draft.lwo");
	aux->a�adeComponenteFisico(0, 0, 0, true);
	entidades.insert(std::make_pair("Arena", aux));
	//delete aux;

	Entidad* aux1 = new Entidad(this);
	aux1->a�adeComponenteGrafico("Greymon");
	aux1->a�adeComponenteFisico(0, 0, 0, false, tipoFisica::Dinamico, 1);
	entidades.insert(std::make_pair("Greymon", aux1));
	Mensaje ms (Tipo::Fisica, "0/150/0", SubTipo::Reposicionar);
	ms.setMsgInfo(entidades.at("Greymon"), entidades.at("Greymon"));
	mensajes.push(ms); 

/*	//Aqu� va la parte gr�fica del suelo
	std::vector<int> auxV = {0,0,0};
	
	Entidad * aux = new Entidad(this); aux->a�adeComponenteGrafico("Arena_draft.lwo", auxV );
	//Aqu� le meto la base f�sica al suelo
	aux->a�adeComponenteFisico(0, 0, 0, true);
	entidades.insert(std::make_pair("Arena", aux));

	//Aqu� meto un greymon din�mico para colisionar y hacer pruebas
	Entidad* aux1 = new Entidad(this);
	aux1->a�adeComponenteGrafico("Greymon", auxV);
	aux1->a�adeComponenteFisico(0, 0, 0, false, tipoFisica::Dinamico, 1);
	entidades.insert(std::make_pair("Greymon", aux1));
	Mensaje * ms = new Mensaje(Tipo::Fisica, "0/150/0", SubTipo::Reposicionar);
	ms->setMsgInfo(entidades.at("Greymon"), entidades.at("Greymon"));
	mensajes.push(ms);*/

	// Luz por defecto
	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	#pragma endregion InitOgre

}

Estado::~Estado(){
	/*scnMgr->clearScene();
	scnMgr->~SceneManager();
	scnMgr->destroyAllEntities();
	scnMgr->destroyAllLights();
	scnMgr->destroyAllCameras();*/
	//delete scnMgr;
	//delete cam;
	//delete mWin;
	//delete vp;
	for (std::pair<std::string,Entidad*> n : entidades) {
		delete n.second;
	}
}

bool Estado::update(float delta){

	this->getFisicManager()->getDynamicsWorld()->stepSimulation(1.0f / 60.0f);
	if (mensajes.size() > 0){
		Mensaje aux = mensajes.top();
		mensajes.pop();
		for (std::pair<std::string, Entidad*> ent : entidades){
			ent.second->Update(delta, aux);
		}
	}
	else {
		for (std::pair<std::string, Entidad*> ent : entidades) 
			ent.second->Update(delta, Mensaje(Tipo::Fisica, " ", SubTipo::Nulo));
	}
	swapMsgBufer();

	return true;
}

void Estado::joystickMoved(float x, float y, int js) {

	std::string sx, sy; sx = std::to_string(x); sy = std::to_string(y);
	std::string s = sx + "/"+ "0/" + sy ;

	
	if (js == 0) {
		Mensaje msgI (Tipo::Input, s, SubTipo::Mover);
		mensajes.push(msgI);
		Mensaje msgR(Tipo::Render, s, SubTipo::Orientar); //Look at de la camara
		msgR.setMsgInfo(entidades.at("Ogro"), entidades.at("Ogro"));
		mensajes.push(msgR);
		
	}
	else {
		Mensaje msgI(Tipo::Input, s, SubTipo::OrientaCamara);
		msgI.setMsgInfo(entidades.at("MainCamera"), entidades.at("MainCamera"));
		mensajes.push(msgI);
	}

}
void Estado::keyPressed(std::string s) {
	if (s == "0") {
		Mensaje msg(Tipo::Fisica, "", SubTipo::Salto);
		msg.setMsgInfo(entidades.at("Ogro"), entidades.at("Ogro"));
		mensajes.push(msg);
	}
	/*if (s == "der" || s == "izq" || s == "arr" || s == "aba") {
		Mensaje* msg = new Mensaje(Tipo::Input, s, SubTipo::Mover);
		msg->setMsgInfo(entidades.at("MainCamera"), entidades.at("MainCamera"));
		mensajes.push(msg);
	}*/
	
}
void Estado::keyReleased(std::string s) {
	
}
Entidad* Estado::getEntidad(std::string s) {
	auto flag = entidades.find(s);
	if (flag != entidades.end()) return entidades.at(s);
	
	return nullptr;
}