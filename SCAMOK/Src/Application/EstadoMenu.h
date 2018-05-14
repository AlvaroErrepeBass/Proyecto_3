#pragma once 
#include "Estado.h"
#include "EstadoJuego.h"
class EstadoMenu: public Estado 
{ 
public: 
	EstadoMenu(Ogre::SceneManager * mng, Ogre::RenderWindow* mWindow, FMOD::System* sys, Juego* game = nullptr, std::string type = "ppal");
	virtual ~EstadoMenu(); 

	virtual bool initCEGUI();
	
	void opciones();
	void jugar();
	void level1();
	void salir();
	void creditos();
private:
		std::string type_;
		void initMenuPause();
		void initCreditos();
		void initOpciones();
		void initPpal();
}; 
