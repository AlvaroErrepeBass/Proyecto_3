#include "ManaComponent.h"  
#include "OgreParticleSystem.h"
#include "OgreParticleEmitter.h"

ManaComponent::ManaComponent(Entidad* pEnt) : Componente(pEnt) 
{

	particleSystem = pEntidad->getPEstado()->getScnManager()->createParticleSystem("cargaManaPFX", "Fairy");
	pEntidad->getPEstado()->getScnManager()->getSceneNode(pEntidad->getNombreNodo())->attachObject(particleSystem);
	particleSystem->setEmitting(false);
	playinFX = false;
} 
ManaComponent::~ManaComponent() { 
} 
void ManaComponent::Update(float deltaTime, Mensaje const & msj)
{
	Mensaje msg = msj;
	if (msg.getTipo() == Tipo::Gui) {
		if (msg.getSubTipo() == SubTipo::InitGui) {
			mana = static_cast<CEGUI::ProgressBar*>(pEntidad->getPEstado()->getGUIRoot()->getChild("Power"));
			maxMana = mana->getWidth().d_offset;
		}
		else if (msg.getSubTipo() == SubTipo::CambiaMana && mana != nullptr) {
			if (msg.getMsg()[0] == '+') {
				std::string s;
				s = msg.getMsg().substr(1, msg.getMsg().size());
				CEGUI::UDim dim = mana->getWidth();
				if (dim.d_offset < maxMana) {
					dim += {0, std::stof(s)};
					mana->setWidth(dim);
				}
				
			}
			else if (msg.getMsg()[0] == '-') {
				std::string s;
				s = msg.getMsg().substr(1, msg.getMsg().size());
				CEGUI::UDim dim = mana->getWidth();
					dim -= {0, std::stof(s)};
					mana->setWidth(dim);
			}
		}
	}
	if (msg.getTipo() == Tipo::Mana && mana != nullptr) {
		if (msg.getSubTipo() == SubTipo::DoPower ) {

			if (mana->getWidth().d_offset >= 24) {
				pEntidad->getPEstado()->getEntidad("Alaia")->setAnim("Shoot");
				Entidad* aux1 = new Entidad(pEntidad->getPEstado());
				std::string pos = "500/1000/500";
				Mensaje msEfect(Tipo::Audio, "Play/bolaFuego.wav/" + pos, SubTipo::Effect);
				pEntidad->getPEstado()->addMsg(msEfect);
				pos = "0/0/0";
				Mensaje msEfect2(Tipo::Audio, "Play/bolaFuegoRisa.wav/" + pos, SubTipo::Effect);
				pEntidad->getPEstado()->addMsg(msEfect2);


				string auxBala = pEntidad->getPEstado()->getFactory()->create("triangulo");
				aux1->setNombreNodo(auxBala);

				aux1->aņadeComponenteGrafico("triangulo", auxBala);
				aux1->aņadeComponenteFisico(0, 0, 0, false, tipoFisica::Dinamico, 1);
				aux1->aņadeComponenteLogico("BalaComponent");
				pEntidad->getPEstado()->addEntidad(auxBala, aux1);
				restaPower();
				// create a particle system named explosions using the explosionTemplate
				Ogre::ParticleSystem* particleSystem = pEntidad->getPEstado()->getScnManager()->createParticleSystem(auxBala + "PFX", "Smoke");
				pEntidad->getPEstado()->getScnManager()->getSceneNode(auxBala)->attachObject(particleSystem);
				particleSystem->setEmitting(true);
			}
		}
		else if (msg.getSubTipo() == SubTipo::GetPower) {
			if (msg.getMsg() == "item") {
				
			}
			else if (msg.getMsg() == "carga") {
				charge();
			}
			else
			{
				finCharge();
			}
		}
	}
	/*Mensaje ms(Tipo::Gui, "+0.01", SubTipo::CambiaMana);
	ms.setMsgInfo(pEntidad, pEntidad);
	pEntidad->getPEstado()->addMsg(ms);*/
}

void ManaComponent::restaPower()
{
	Mensaje ms(Tipo::Gui, "-25", SubTipo::CambiaMana);
	ms.setMsgInfo(pEntidad, pEntidad);
	pEntidad->getPEstado()->addMsg(ms, true);
}

void ManaComponent::charge()
{
	Mensaje ms(Tipo::Gui, "+0.5", SubTipo::CambiaMana);
	ms.setMsgInfo(pEntidad, pEntidad);
	pEntidad->getPEstado()->addMsg(ms, true);
	
	if (!playinFX) {
		Mensaje msEfect2(Tipo::Audio, "Play/recargar.wav", SubTipo::Musica);
		pEntidad->getPEstado()->addMsg(msEfect2);
	}
	playinFX = true;
	particleSystem->setEmitting(true);
	
}

void ManaComponent::finCharge()
{
	particleSystem->setEmitting(false);

	Mensaje msEfect2(Tipo::Audio, "Stop/1", SubTipo::Musica);
	pEntidad->getPEstado()->addMsg(msEfect2);

}
void ManaComponent::restaurapS()
{
	Ogre::ParticleEmitter * em = particleSystem->getEmitter(0);
	em->setEnabled(false);
	em->setEnabled(true);

}