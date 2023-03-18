#include <Windows.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include "kayttoliittyma.h"


Kayttoliittyma* Kayttoliittyma::instance = 0;


Kayttoliittyma* Kayttoliittyma::getInstance()
{
	if (instance == 0)
		instance = new Kayttoliittyma();
	return instance;
}


void Kayttoliittyma::piirraLauta(std::list<std::array<int8_t, 5>>& siirrot)
{
	_setmode(_fileno(stdout), _O_U16TEXT);


	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	int numbers = 8;
	bool isGreen = false;

for (int i = 7; i >= 0; i--)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	std::wcout << numbers;
	numbers--;
	for (int j = 0; j < 8; j++)
	{
		if (isGreen && i % 2 != 0 && j % 2 != 0 || i % 2 == 0 && j % 2 == 0)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
			isGreen = false;
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			isGreen = true;
		}

		//Debug väritykset siirtojen mukaan
		for (std::array<int8_t, 5> siirto : siirrot)
		{
			if (siirto[2] == i && siirto[3] == j) {
				if (siirto[0] == 8 || siirto[1] == 8)
				{
					//Merkataan linnoitussiirrot purppuralla taustavärillä
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_RED);
				}
				else if (_asema->_lauta[siirto[2]][siirto[3]] != NULL)
				{
					//Syönnit punaisella
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
				}
				else
				{
					//Tavalliset siirrot sinisellä
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE);
				}
			}
		}

		if (_asema->_lauta[i][j] != NULL)
		{
			std::wcout << " " << _asema->_lauta[i][j]->getUnicode() << " ";
		}
		else
		{
			std::wcout << "   ";
		}

	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	std::wcout << "\n";
}
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
std::wcout << L"  A  B  C  D  E  F  G  H";

SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
}



/*
	Aliohjelma tarkistaa että käyttäjän antama syöte siirroksi on
	muodollisesti korrekti (ei tarkista aseman laillisuutta)
	Ottaa irti myös nappulan kirjaimen (K/D/L/R/T), tarkistaa että kirjain korrekti
*/
std::array<int8_t, 5> Kayttoliittyma::annaVastustajanSiirto(Asema& asema, int vastustajanVari)
{
	std::array<int8_t, 5> palautus;
	std::string vsiirto = "";
	std::list<std::array<int8_t, 5>> vastustajanLailliset;
	asema.annaLaillisetSiirrot(vastustajanLailliset, &asema, vastustajanVari);
	int asarake, arivi, lsarake, lrivi;
	std::wcout << "Anna vastapelaajan siirto: \n";

	while (true) {
		std::cin >> vsiirto;

		//Upseerisiirto
		if (vsiirto.length() > 5) {
			vsiirto.erase(0, 1);
		}
		else if (vsiirto.find("O-O") != std::string::npos) {
			if (vsiirto.find("O-O-O") == std::string::npos) {
				//Lyhyt linnoitus
				
				
				//Tarkistetaan onko mahdollista
				bool mahdollista = false;
				for (std::array<int8_t, 5>& siirto : vastustajanLailliset)
				{
					if (siirto[0] == 8)
					{
						mahdollista = true;
					}
				}

				if (mahdollista)
				{
					palautus [0] = 8;
					return palautus;
				}
				else
				{
					std::wcout << "Lyhyt linnoitus laiton siirto! \n";
				}
			}
			else {
				//Pitkä linnoitus

				//Tarkistetaan onko mahdollista
				bool mahdollista = false;
				for (std::array<int8_t, 5>&siirto : vastustajanLailliset)
				{
					if (siirto[1] == 8)
					{
						mahdollista = true;
					}
				}

				if (mahdollista)
				{
					palautus[1] = 8;
					return palautus;
				}
				else
				{
					std::wcout << "Pitka linnoitus laiton siirto! \n";
				}
			}
		}

		asarake = (int)vsiirto[0] - 97;
		arivi = (int)vsiirto[1] - 49;

		lsarake = (int)vsiirto[3] - 97;;
		lrivi = (int)vsiirto[4] - 49;


		if (asarake < 0 || arivi < 0 || lsarake < 0 || lrivi < 0
			|| asarake >7 || arivi >7 || lsarake > 7 || lrivi >7) {
			std::wcout << "Siirron kuuluu olla oikealla tavalla kirjoitettu:\n" << "Arvo alkusarake(a-h) alkurivi(1-8) - loppusarake(a-h) loppurivi(1-8)\n" << "esim. Ta8-a5\n";
		}
		else {
			palautus[0] = arivi;
			palautus[1] = asarake;
			palautus[2] = lrivi;
			palautus[3] = lsarake;
			palautus[4] = 8;

			//Tarkistetaan että siirto oli laillinen
			for (std::array<int8_t, 5>& siirto : vastustajanLailliset)
			{
				if (siirto[0] == arivi && siirto[1] == asarake)
				{
					if (siirto[2] == lrivi && siirto[3] == lsarake)
					{
						return palautus;
					}
				}
				
			}
		}
		std::wcout << "Anna laillinen vastustajan siirto: \n";
	}
}


int Kayttoliittyma::kysyVastustajanVari()
{
	std::string aloittaja = "";

	while (true) {
		std::wcout << "Aloittaako vastapelaaja? 1 = aloittaa, 0 = ei aloita \n";

		std::cin >> aloittaja;

		if ((int)aloittaja[0] - 48 != 0 || (int)aloittaja[0] - 48 != 1)
		{
			break;
		}
	}

	return (int)aloittaja[0] - 48;
}
