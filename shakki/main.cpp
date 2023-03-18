#pragma once
#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include "kayttoliittyma.h"
#include "asema.h"



int main()
{
	std::wcout << "Tervetuloa pelaamaan!\n";

	bool lopetus = false;
	Asema asema;

//-----------------Asetetaan arvot--------------------------

// Ensin alustetaan kaikki laudan ruudut nappulla "NULL", koska muuten ruuduissa satunnaista tauhkaa
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			asema._lauta[i][j] = NULL;
		}
	}
	// Asetetaan alkuaseman mukaisesti nappulat ruuduille

	asema._lauta[0][0] = Asema::vt;
	asema._lauta[0][1] = Asema::vr;
	asema._lauta[0][2] = Asema::vl;
	asema._lauta[0][3] = Asema::vd;
	asema._lauta[0][4] = Asema::vk;
	asema._lauta[0][5] = Asema::vl;
	asema._lauta[0][6] = Asema::vr;
	asema._lauta[0][7] = Asema::vt;

	asema._lauta[7][0] = Asema::mt;
	asema._lauta[7][1] = Asema::mr;
	asema._lauta[7][2] = Asema::ml;
	asema._lauta[7][3] = Asema::md;
	asema._lauta[7][4] = Asema::mk;
	asema._lauta[7][5] = Asema::ml;
	asema._lauta[7][6] = Asema::mr;
	asema._lauta[7][7] = Asema::mt;

	for (int j = 0; j < 8; j++)
	{
		asema._lauta[1][j] = Asema::vs;
	}

	for (int j = 0; j < 8; j++)
	{
		asema._lauta[6][j] = Asema::ms;
	}

	//Asetetaan kaikki liput epätosiksi
	asema._onkoValkeaKuningasLiikkunut = false;
	asema._onkoValkeaKTliikkunut = false;
	asema._onkoValkeaDTliikkunut = false;

	asema._onkoMustaKuningasLiikkunut = false;
	asema._onkoMustaKTliikkunut = false;
	asema._onkoMustaDTliikkunut = false;

	//Asetetaan kuninkaiden arvot oikeiksi
	asema._vkShakattu = false;
	asema._vkRivi = 0;
	asema._vkSarake = 4;

	asema._mkShakattu = false;
	asema._mkRivi = 7;
	asema._mkSarake = 4;

	//Asetetaan siirtovuoro valkoiselle
	asema.setSiirtovuoro(0);

//-----------------/Asetetaan arvot--------------------------
	
	
	//Toivottu hakusyvyys
	asema._keskim_haku = 4;

	//Justeeraus
	asema._syvahaku = asema._keskim_haku + 1;

	//Muut haun parametrit
	asema._hakusyvyys = asema._keskim_haku;
	asema._minTallennusSyvyys = 1;
	
	Kayttoliittyma::getInstance()->aseta_asema(&asema);

	Peli peli(Kayttoliittyma::getInstance()
		->kysyVastustajanVari());

	std::list<std::array<int8_t, 5>> lista;

	int koneenVari = peli.getKoneenVari();
	
	//Ainoastaan botin mitellessä itseään vastaan
	bool bottipelaaItseaanVastaan = false;
	int bottivsbotti = 0;

	//Ajat
	std::time_t kokonaisaika = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::time_t aika_vuoron_alussa;

	while (lopetus != true) {
		lista.clear();

		// Tarkasta onko peli loppu?
		asema.annaLaillisetSiirrot(lista, &asema, asema.getSiirtovuoro());
		Kayttoliittyma::getInstance()->piirraLauta(lista);
		std::wcout << "\n";

		if (lista.size() == 0) {
			lopetus = true;
			std::wcout << "Peli loppui";
			continue;
		}

		aika_vuoron_alussa = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		////debug
		//int i = 0;
		//for (auto s : lista) {
		//	i++;
		//	std::wcout << "alku = " << s[0] << " " << s[1] << " loppu = " << s[2] << " " << s[3] << " lyhyt = " << s[0] << " pitka = " << s[1] << "\n";
		//	
		//}
		//std::wcout << "yht. " << i << " siirtoa \n";

		std::array<int8_t, 5> siirto;

		if (asema.getSiirtovuoro() == koneenVari) {

			AlphaBetaPaluu paluu;
			paluu = asema.alphaBeta(koneenVari);
			siirto = paluu._parasSiirto;

			char asarake = paluu._parasSiirto[1] + 65;
			char arivi = paluu._parasSiirto[0] + 49;

			char lsarake = paluu._parasSiirto[3] + 65;
			char lrivi = paluu._parasSiirto[2] + 49;


			//Linnoitussiirrot
			if (asarake == 8 || arivi == 8)
			{
				arivi = (asema.getSiirtovuoro() == 0) ? 49 : 7 + 49;
				asarake = 4 + 65;
			}

			std::wcout << "\n" << asarake << arivi << " - " << lsarake << lrivi << "		Vari " << koneenVari << " : EV = " << paluu._evaluointiArvo << "\n\n";
		}
		else {
			if (!bottipelaaItseaanVastaan)
			{
				siirto = Kayttoliittyma::getInstance()->annaVastustajanSiirto(asema, asema.getSiirtovuoro());
			}
			else
			{
				AlphaBetaPaluu paluu;
				int tamanKoneenVari = (koneenVari == 1) ? 0 : 1;
				paluu = asema.alphaBeta(tamanKoneenVari);
				siirto = paluu._parasSiirto;

				char asarake = paluu._parasSiirto[1] + 65;
				char arivi = paluu._parasSiirto[0] + 49;

				char lsarake = paluu._parasSiirto[3] + 65;
				char lrivi = paluu._parasSiirto[2] + 49;

				//Linnoitussiirrot
				if (asarake == 8 || arivi == 8)
				{
					arivi = (asema.getSiirtovuoro() == 0) ? 49 : 7 + 49;
					asarake = 4 + 65;
				}

				std::wcout << "\n" << asarake << arivi << " - " << lsarake << lrivi << "		Vari " << tamanKoneenVari << " : EV = " << paluu._evaluointiArvo << "\n\n";
			}
		}
		asema.paivitaAsema(siirto);
		
		//Bottipelissä
		bottivsbotti++;
		std::wcout << "Pelattu " << bottivsbotti << " vuoroa." << "\n";
		
		//Alkeellinen aikaseuranta
		std::wcout << "Aikaa kului siirtoon " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - aika_vuoron_alussa << " sekuntia" << "\n";
		std::wcout << "Kokonaisaika " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - kokonaisaika << " sekuntia" << "\n";

	}

	
	return 0;
}
