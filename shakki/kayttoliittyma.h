#pragma once
#include "asema.h"
#include "nappula.h"
#include "peli.h"


// Shakkiohjelman käyttöliittymä, joka osaa visualisoida nykyisen aseman
// ja lukea käyttäjän syöttämät siirrot. Singleton.
class Kayttoliittyma
{
public:

	void aseta_asema(Asema* asema) { this->_asema = asema;  }
	void piirraLauta(std::list<std::array<int8_t, 5>>& siirrot);
	std::array<int8_t, 5> annaVastustajanSiirto(Asema& asema, int vari);
	int kysyVastustajanVari();

	static Kayttoliittyma* getInstance();

private:

	Asema* _asema;
	static Kayttoliittyma* instance; // osoitin luokan ainoaan olioon (Singleton).

	Kayttoliittyma() {}
	Kayttoliittyma(Asema* asema) { this->_asema = asema; }
};
