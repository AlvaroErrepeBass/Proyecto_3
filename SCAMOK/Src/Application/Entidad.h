#pragma once
#include "Componente.h"
#include <iterator>
#include "Mensaje.h"
#include <vector>


class Estado;
class Entidad{

public:
	Entidad(Estado* pEstado);
	Entidad(Estado* pEstado, std::string prefab);
	~Entidad();
	void Update(float deltaTime, Mensaje  & msg);
	void Awake();
	void Sleep();
	Estado* getPEstado() { return pEstado; };

	bool aņadeComponenteGrafico(std::string mesh);
	bool aņadeComponenteGrafico(std::string mesh, std::string ogreNodeName);
	bool aņadeComponenteFisico(float altoCaja, float anchoCaja, float profCaja, bool suelo, tipoFisica type = tipoFisica::Estatico, int masa = 0);
	bool aņadeAnimacion(std::string mesh, std::ifstream & fe);
	bool aņadeAnimacion(std::string name,bool enabled = true, bool loop = true);
	bool aņadeComponenteLogico(std::string component);
	bool aņadeComponenteSM(std::string component, void* sys);
	void setAnim(std::string const &s, bool loop = false, bool enabled = true, bool idle = false);
	void destroy() { delete this; };
	void destruyeComponenteGrafico();
	std::string getNombreNodo() { return nombreNodo; };
	int cont = 0;

private:
	std::map<std::string, Componente*> componentes;
	Estado* pEstado;
	bool activo;
	Entidad* target; //Usar esta entidad para escuchar los mensajes dirigidos a ella
	std::string nombreNodo;
	
};