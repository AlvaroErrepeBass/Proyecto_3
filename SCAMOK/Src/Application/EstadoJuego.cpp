#pragma once
#include "EstadoJuego.h"

EstadoJuego::EstadoJuego(Ogre::SceneManager * mng, Ogre::RenderWindow* mWindow, FMOD::System* sys): Estado(mng, mWindow, sys)
{
	
	noInput = true; contInput = 0;
	cargaGui();
	
}
void EstadoJuego::init() {
#pragma region InitOgre 
	
	scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	scnMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);


	scnMgr->setShadowFarDistance(200);


	entidades.insert(std::make_pair("Ogro", new Entidad(this, "sinbad")));
	entidades.insert(std::make_pair("MainCamera", new Entidad(this, "camera")));


	Mensaje msg(Tipo::Fisica, "0/30/0", SubTipo::Reposicionar);
	msg.setMsgInfo(entidades.at("Ogro"), entidades.at("Ogro"));
	mensajes.push(msg);

	Entidad *aux = new Entidad(this); aux->aņadeComponenteGrafico("arena");
	aux->aņadeComponenteFisico(0, 0, 0, true);
	entidades.insert(std::make_pair("Arena", aux));


	Entidad *aux2 = new Entidad(this);
	aux2->aņadeComponenteGrafico("compcube");
	aux2->aņadeComponenteFisico(0, 0, 0, false);
	entidades.insert(std::make_pair("MetalBox", aux2));
	Mensaje ms1(Tipo::Fisica, "0/15/10", SubTipo::Reposicionar);
	ms1.setMsgInfo(entidades.at("MetalBox"), entidades.at("MetalBox"));
	mensajes.push(ms1);

	Entidad* aux1 = new Entidad(this);
	aux1->aņadeComponenteGrafico("stone");
	aux1->aņadeComponenteFisico(0, 0, 0, false, tipoFisica::Dinamico, 1);
	entidades.insert(std::make_pair("Stone", aux1));
	Mensaje ms(Tipo::Fisica, "0/150/0", SubTipo::Reposicionar);
	ms.setMsgInfo(entidades.at("Stone"), entidades.at("Stone"));
	mensajes.push(ms);


	Entidad* aux3 = new Entidad(this); aux3->aņadeComponenteSM("SoundManager", system);
	entidades.insert(std::make_pair("SoundManager", aux3));
	Mensaje playM(Tipo::Audio, "Play/wii.mp3", SubTipo::Musica);
	mensajes.push(playM);


	// Luz por defecto
	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 50, 50);

#pragma endregion InitOgre
	destroy();
	initCEGUI();
}
void EstadoJuego::cargaGui()
{
	Estado::initCEGUI();
	guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("loadScreen.layout");
	m_gui.getRoot()->addChild(guiRoot);

}
bool EstadoJuego::initCEGUI() {
	//Estado::initCEGUI();

	guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("Hud.layout");
	m_gui.getRoot()->addChild(guiRoot);
	
	return true;
}
bool EstadoJuego::update(float delta) {
	CEGUI::System::getSingleton().injectTimePulse(0.016f);
	//CEGUI::System::getSingleton().injectTimePulse(0.016f);
	this->getFisicManager()->getDynamicsWorld()->stepSimulation(1.0f / 60.0f);
	if (mensajes.size() > 0) {
		Mensaje aux = mensajes.top();
		mensajes.pop();
		for (std::pair<std::string, Entidad*> ent : entidades) {
			ent.second->Update(delta, aux);
		}
	}
	else {
		for (std::pair<std::string, Entidad*> ent : entidades)
			ent.second->Update(delta, Mensaje(Tipo::Fisica, " ", SubTipo::Nulo));
		if (contInput == 30) {
			entidades.at("Ogro")->setAnim("IdleTop", true, true, true);
			entidades.at("Ogro")->setAnim("IdleBase", true, true, true);
			contInput = 0;
		}

	}
	contInput++;
	m_gui.draw();
	swapMsgBufer();

	return true;
}

void EstadoJuego::joystickMoved(float x, float y, int js) {

	std::string sx, sy; sx = std::to_string(x); sy = std::to_string(y);
	std::string s = sx + "/" + "0/" + sy;


	if (js == 0) {
		Mensaje msgI(Tipo::Input, s, SubTipo::Mover);
		mensajes.push(msgI);
		Mensaje msgR(Tipo::Render, s, SubTipo::Orientar); //Look at de la camara
		msgR.setMsgInfo(entidades.at("Ogro"), entidades.at("Ogro"));
		mensajes.push(msgR);
		entidades.at("Ogro")->setAnim("RunTop", true);
		entidades.at("Ogro")->setAnim("RunBase", true);
		contInput = 0;

	}
	else {
		Mensaje msgI(Tipo::Input, s, SubTipo::OrientaCamara);
		msgI.setMsgInfo(entidades.at("MainCamera"), entidades.at("MainCamera"));
		mensajes.push(msgI);
	}

}

void EstadoJuego::keyPressed(std::string s) {
	if (s == "0" || s == "salto") {
		Mensaje msg(Tipo::Fisica, "", SubTipo::Salto);
		msg.setMsgInfo(entidades.at("Ogro"), entidades.at("Ogro"));
		mensajes.push(msg);
		entidades.at("Ogro")->setAnim("JumpLoop");
	}
	else if (s == "1") {
		entidades.at("Ogro")->setAnim("SliceHorizontal");

	}
	else if (s == "2") {
		entidades.at("Ogro")->setAnim("Dance");
	}
	else if (s == "3") {
		entidades.at("Ogro")->setAnim("SliceVertical");
	}

}
void EstadoJuego::keyReleased(std::string s) {


}