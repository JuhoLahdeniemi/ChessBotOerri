#include <list>
#include <string>
#include <iostream>
#include "asema.h"
#include "nappula.h"


Nappula::Nappula(std::wstring unicode, int vari, int koodi)
{
	setUnicode(unicode);
	setVari(vari);
	setKoodi(koodi);
}

void lisaaSiirtoTarkistaShakki(std::list<std::array<int8_t, 5>>& lista, std::array<int8_t, 5>& uusiSiirto, Asema* asema, int vari)
{
	//Kuninkaan shakin tarkistaminen
	if (vari == 0)
	{

		if (uusiSiirto[2] == asema->_mkRivi && uusiSiirto[3] == asema->_mkSarake) {
			asema->_mkShakattu = true;
		}
	}
	else
	{
		if (uusiSiirto[2] == asema->_vkRivi && uusiSiirto[3] == asema->_vkSarake) {
			asema->_vkShakattu = true;
		}
	}

	//Tarkistetaan estääkö tämä siirto linnoittamisen
	if (vari == 1) {
		//Valkoisen rivi
		if (uusiSiirto[2] == 0)
		{
			if (uusiSiirto[3] == 4)
			{
				asema->valkoisenLyhytMahdollinen = false;
				asema->valkoisenPitkaMahdollinen = false;
			}

			if (uusiSiirto[3] == 2 || uusiSiirto[3] == 3)
			{
				asema->valkoisenPitkaMahdollinen = false;
			}

			if (uusiSiirto[3] == 5 || uusiSiirto[3] == 6)
			{
				asema->valkoisenLyhytMahdollinen = false;
			}
		}
	}
	else
	{
		//Mustan rivi
		if (uusiSiirto[2] == 7)
		{
			if (uusiSiirto[3] == 4)
			{
				asema->mustanLyhytMahdollinen = false;
				asema->mustanPitkaMahdollinen = false;
			}

			if (uusiSiirto[3] == 2 || uusiSiirto[3] == 3)
			{
				asema->mustanPitkaMahdollinen = false;
			}

			if (uusiSiirto[3] == 5 || uusiSiirto[3] == 6)
			{
				asema->mustanLyhytMahdollinen = false;
			}
		}
	}

	lista.push_back(uusiSiirto);
}

void Torni::annaSiirrot(std::list<std::array<int8_t, 5>>& lista, int alkurivi, int alkusarake, Asema* asema, int vari)
{
	std::array<int8_t, 5> uusiSiirto;
	bool syodaanVihollinen = false;
	bool lopetetaanA = false;
	bool lopetetaanB = false;
	bool lopetetaanC = false;
	bool lopetetaanD = false;


	for (int i = 1; i < 8; i++)
	{

		//Siirrot ylös:

		if (!lopetetaanA)
		{
			//Jos ruutu laudalla
			if (alkurivi + i < 8)
			{
				if (asema->_lauta[alkurivi + i][alkusarake] != NULL)
				{
					if (asema->_lauta[alkurivi + i][alkusarake]->getVari() != vari)
					{
						syodaanVihollinen = true;
					}
					else
					{
						lopetetaanA = true;
					}
				}

				if (!lopetetaanA)
				{
					//Ruutu on tyhjä tai siinä on vihollinen joka syödään
					if (syodaanVihollinen)
					{
						lopetetaanA = true;
						syodaanVihollinen = false;
					}

					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi + i;
					uusiSiirto[3] = alkusarake;
					uusiSiirto[4] = 1;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
			else
			{
				//Mentiin laudan ulkopuolelle
				lopetetaanA = true;
			}
		}



		//Siirrot alas:


		if (!lopetetaanB)
		{
			//Jos ruutu laudalla
			if (alkurivi - i >= 0)
			{
				if (asema->_lauta[alkurivi - i][alkusarake] != NULL)
				{
					if (asema->_lauta[alkurivi - i][alkusarake]->getVari() != vari)
					{
						syodaanVihollinen = true;
					}
					else
					{
						lopetetaanB = true;
					}
				}

				if (!lopetetaanB)
				{
					//Ruutu on tyhjä tai siinä on vihollinen joka syödään
					if (syodaanVihollinen)
					{
						lopetetaanB = true;
						syodaanVihollinen = false;
					}

					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi - i;
					uusiSiirto[3] = alkusarake;
					uusiSiirto[4] = 1;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
			else
			{
				//Mentiin laudan ulkopuolelle
				lopetetaanB = true;
			}
		}



		//Siirrot oikealle:

		if (!lopetetaanC)
		{
			//Jos ruutu laudalla
			if (alkusarake + i < 8)
			{
				if (asema->_lauta[alkurivi][alkusarake + i] != NULL)
				{
					if (asema->_lauta[alkurivi][alkusarake + i]->getVari() != vari)
					{
						syodaanVihollinen = true;
					}
					else
					{
						lopetetaanC = true;
					}
				}

				if (!lopetetaanC)
				{
					//Ruutu on tyhjä tai siinä on vihollinen joka syödään
					if (syodaanVihollinen)
					{
						lopetetaanC = true;
						syodaanVihollinen = false;
					}

					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi;
					uusiSiirto[3] = alkusarake + i;
					uusiSiirto[4] = 1;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
			else
			{
				//Mentiin laudan ulkopuolelle
				lopetetaanC = true;
			}
		}

		//Siirrot vasemmalle:

		if (!lopetetaanD)
		{
			//Jos ruutu laudalla
			if (alkusarake - i >= 0)
			{
				if (asema->_lauta[alkurivi][alkusarake - i] != NULL)
				{
					if (asema->_lauta[alkurivi][alkusarake - i]->getVari() != vari)
					{
						syodaanVihollinen = true;
					}
					else
					{
						lopetetaanD = true;
					}
				}

				if (!lopetetaanD)
				{
					//Ruutu on tyhjä tai siinä on vihollinen joka syödään
					if (syodaanVihollinen)
					{
						lopetetaanD = true;
						syodaanVihollinen = false;
					}

					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi;
					uusiSiirto[3] = alkusarake - i;
					uusiSiirto[4] = 1;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
			else
			{
				//Mentiin laudan ulkopuolelle
				lopetetaanD = true;
			}
		}
	}
}

void Ratsu::annaSiirrot(std::list<std::array<int8_t, 5>>& lista, int alkurivi, int alkusarake, Asema* asema, int vari)
{
	if (getVari() == vari) {

		std::array<int8_t, 5> uusiSiirto;
		bool voiSiirtya = false;

		int loppusarake;
		int loppurivi;

		//kahdeksan eri hyppyä
		int hsarakkeet[] = { 2,2,-2,-2,1,1,-1,-1 };
		int hrivit[] = { 1,-1,1,-1,2,-2,2,-2 };



		//loopataan hypyt ja katsotaan onko niiden loppuruutu laillinen
		for (int i = 0; i < 8; i++) {
			voiSiirtya = false;

			loppusarake = alkusarake + hsarakkeet[i];
			loppurivi = alkurivi + hrivit[i];

			if (loppusarake < 8 && loppusarake >= 0 && loppurivi < 8 && loppurivi >= 0)
			{

				//jos ruutu tyhjä, sanotaan annetaan siirto siirtojen listaan..
				if (asema->_lauta[loppurivi][loppusarake] == NULL)
				{
					voiSiirtya = true;
				}
				//(ruutu ei ole null) onko ruudussa olevan väri eri kuin oma väri
				else if (asema->_lauta[loppurivi][loppusarake]->getVari() != vari)
				{
					voiSiirtya = true;
				}

				if (voiSiirtya)
				{
					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = loppurivi;
					uusiSiirto[3] = loppusarake;
					uusiSiirto[4] = 8;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
				//jos ei ole eri väriä, ei tehdä mitään(ei voida antaa siirtoehdotusta ruutuun)
			}

		}
	}
}


void Lahetti::annaSiirrot(std::list<std::array<int8_t, 5>>& lista, int alkurivi, int alkusarake, Asema* asema, int vari)
{
	std::array<int8_t, 5> uusiSiirto;

	bool syodaanVihollinen = false;
	bool lopetetaanA = false;
	bool lopetetaanB = false;
	bool lopetetaanC = false;
	bool lopetetaanD = false;

	int j = 0;
	for (int i = 1; i < 8; i++)
	{
		j++;

		//Siirrot yläoikeaan:

		if (!lopetetaanA)
		{
			//Jos ruutu laudalla
			if (alkurivi + i < 8 && alkusarake + j < 8)
			{
				if (asema->_lauta[alkurivi + i][alkusarake + j] != NULL)
				{
					if (asema->_lauta[alkurivi + i][alkusarake + j]->getVari() != vari)
					{
						syodaanVihollinen = true;
					}
					else
					{
						lopetetaanA = true;
					}
				}

				if (!lopetetaanA)
				{
					//Ruutu on tyhjä tai siinä on vihollinen joka syödään
					if (syodaanVihollinen)
					{
						lopetetaanA = true;
						syodaanVihollinen = false;
					}

					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi + i;
					uusiSiirto[3] = alkusarake + j;
					uusiSiirto[4] = 2;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
			else
			{
				//Mentiin laudan ulkopuolelle
				lopetetaanA = true;
			}
		}


		//Siirrot alaoikeaan:

		if (!lopetetaanB)
		{
			//Jos ruutu laudalla
			if (alkurivi - i >= 0 && alkusarake + j < 8)
			{
				if (asema->_lauta[alkurivi - i][alkusarake + j] != NULL)
				{
					if (asema->_lauta[alkurivi - i][alkusarake + j]->getVari() != vari)
					{
						syodaanVihollinen = true;
					}
					else
					{
						lopetetaanB = true;
					}
				}

				if (!lopetetaanB)
				{
					//Ruutu on tyhjä tai siinä on vihollinen joka syödään
					if (syodaanVihollinen)
					{
						lopetetaanB = true;
						syodaanVihollinen = false;
					}

					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi - i;
					uusiSiirto[3] = alkusarake + j;
					uusiSiirto[4] = 2;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
			else
			{
				//Mentiin laudan ulkopuolelle
				lopetetaanB = true;
			}
		}


		//Siirrot ylävasemmalle:

		if (!lopetetaanC)
		{
			//Jos ruutu laudalla
			if (alkurivi + i < 8 && alkusarake - j >= 0)
			{
				if (asema->_lauta[alkurivi + i][alkusarake - j] != NULL)
				{
					if (asema->_lauta[alkurivi + i][alkusarake - j]->getVari() != vari)
					{
						syodaanVihollinen = true;
					}
					else
					{
						lopetetaanC = true;
					}
				}

				if (!lopetetaanC)
				{
					//Ruutu on tyhjä tai siinä on vihollinen joka syödään
					if (syodaanVihollinen)
					{
						lopetetaanC = true;
						syodaanVihollinen = false;
					}

					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi + i;
					uusiSiirto[3] = alkusarake - j;
					uusiSiirto[4] = 2;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
			else
			{
				//Mentiin laudan ulkopuolelle
				lopetetaanC = true;
			}
		}


		//Siirrot alavasemmalle:

		if (!lopetetaanD)
		{
			//Jos ruutu laudalla
			if (alkurivi - i >= 0 && alkusarake - j >= 0)
			{
				if (asema->_lauta[alkurivi - i][alkusarake - j] != NULL)
				{
					if (asema->_lauta[alkurivi - i][alkusarake - j]->getVari() != vari)
					{
						syodaanVihollinen = true;
					}
					else
					{
						lopetetaanD = true;
					}
				}

				if (!lopetetaanD)
				{
					//Ruutu on tyhjä tai siinä on vihollinen joka syödään
					if (syodaanVihollinen)
					{
						lopetetaanD = true;
						syodaanVihollinen = false;
					}
					//Lisätään siirto listaan
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi - i;
					uusiSiirto[3] = alkusarake - j;
					uusiSiirto[4] = 2;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
			else
			{
				//Mentiin laudan ulkopuolelle
				lopetetaanD = true;
			}
		}
		//Testimielessä lisätty
		if (lopetetaanA && lopetetaanB && lopetetaanC && lopetetaanD)
		{
			break;
		}
	}
}


void Daami::annaSiirrot(std::list<std::array<int8_t, 5>>& lista, int alkurivi, int alkusarake, Asema* asema, int vari)
{
	Torni::annaSiirrot(lista, alkurivi, alkusarake, asema, vari);
	Lahetti::annaSiirrot(lista, alkurivi, alkusarake, asema, vari);
}


void Kuningas::annaSiirrot(std::list<std::array<int8_t, 5>>& lista, int alkurivi, int alkusarake, Asema* asema, int vari)
{
	/*perusidea on että kaikki viereiset ruudut ovat sallittuja. kuten tornilla ja lähetillä,
	oman nappulan päälle ei voi mennä ja vastustajan nappulan voi syödä.

	Kaikki muu kuninkaaseen liittyvä tarkistus tehdään eri paikassa*/



	//Kuningas voi siirtyä aina yhden ruudun mihin tahansa suuntaan, paitsi jos jokin on edessä. (3x3 ruudukko - keskiosa
	std::array<int8_t, 5> uusiSiirto;
	//Rivit
	for (int i = -1; i < 2; i++) {

		//Tarkastetaan  rivin olemassaolo
		if (alkurivi + i < 8 && alkurivi + i > -1)
		{

			//Sarakkeet
			for (int j = -1; j < 2; j++)
			{

				//Tarkastetaan sarakkeen olemassaolo
				if (alkusarake + j < 8 && alkusarake + j > -1)
				{

					//Tutkitaan siirtymismahdollisuudet
					if (asema->_lauta[alkurivi + i][alkusarake + j] != NULL)
					{
						//Jos oma nappula, ei voida siirtyä kyseiseen ruutuun
						if (asema->_lauta[alkurivi + i][alkusarake + j]->getVari() == vari)
						{
							continue;
						}
					}

					//Jos kaikki ok, lisätään siirto listaan (Alkuruutu, loppuruutu)
					uusiSiirto[0] = alkurivi;
					uusiSiirto[1] = alkusarake;
					uusiSiirto[2] = alkurivi + i;
					uusiSiirto[3] = alkusarake + j;
					uusiSiirto[4] = 8;

					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
		}
	}
}


void Sotilas::annaSiirrot(std::list<std::array<int8_t, 5>>& lista, int alkurivi, int alkusarake, Asema* asema, int vari)
{
	std::array<int8_t, 5> uusiSiirto;
	int suunta = (vari == 0) ? 1 : -1;

	//Sotilas voi siirtyä aina yhden ruudun eteen, paitsi jos jokin on edessä
	if (asema->_lauta[alkurivi + (1 * suunta)][alkusarake] == NULL)
	{

		//Lisätään siirto listaan
		uusiSiirto[0] = alkurivi;
		uusiSiirto[1] = alkusarake;
		uusiSiirto[2] = alkurivi + (1 * suunta);
		uusiSiirto[3] = alkusarake;
		uusiSiirto[4] = 8;

		//Tarkastetaan ollaanko päästy vastakkaiseen päätyyn saakka
		if ((vari == 0 && alkurivi == 6) || (vari == 1 && alkurivi == 1))
		{
			lisaaSotilaanKorotuksetJaTarkistaShakki(uusiSiirto, lista, asema, vari);
		}
		else
		{
			lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
		}
	}

	//Tarkastetaan onko sotilas alkuasemassa eli voidaanko tehdä 2 ruudun siirto
	if ((vari == 0 && alkurivi == 1) || (vari == 1 && alkurivi == 6))
	{

		//Voidaan siirtyä 2 ruutua, tarkastetaan onko 1 tai 2 ruudun päässä estettä
		if ((asema->_lauta[alkurivi + (1 * suunta)][alkusarake] == NULL) && (asema->_lauta[alkurivi + (2 * suunta)][alkusarake] == NULL))
		{
			//Lisätään siirto listaan
			uusiSiirto[0] = alkurivi;
			uusiSiirto[1] = alkusarake;
			uusiSiirto[2] = alkurivi + (2 * suunta);
			uusiSiirto[3] = alkusarake;
			uusiSiirto[4] = 8;

			lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
		}
	}

	//Tarkastetaan voiko sotilas syödä viistoon oikealle
	if (alkusarake + 1 < 8) {


		//Ei menty laudan ulkopuolelle, tarkastetaan että ruudussa on nappula
		if ((asema->_lauta[alkurivi + (1 * suunta)][alkusarake + 1] != NULL))
		{
			//Katsotaan onko nappulan väri eri kuin oma
			if (asema->_lauta[alkurivi + (1 * suunta)][alkusarake + 1]->getVari() != vari)
			{

				//Lisätään siirto listaan
				uusiSiirto[0] = alkurivi;
				uusiSiirto[1] = alkusarake;
				uusiSiirto[2] = alkurivi + (1 * suunta);
				uusiSiirto[3] = alkusarake + 1;
				uusiSiirto[4] = 8;

				//Tarkastetaan ollaanko päästy vastakkaiseen päätyyn saakka
				if ((vari == 0 && alkurivi == 6) || (vari == 1 && alkurivi == 1))
				{
					lisaaSotilaanKorotuksetJaTarkistaShakki(uusiSiirto, lista, asema, vari);
				}
				else
				{
					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
		}
		//Jos ohestalyönti
		else if (asema->kaksoisaskelSarakkeella == alkusarake + 1 && ((vari == 0 && alkurivi == 4) || (vari == 1 && alkurivi == 3)))
		{
			//Lisätään siirto listaan
			uusiSiirto[0] = alkurivi;
			uusiSiirto[1] = alkusarake;
			uusiSiirto[2] = alkurivi + (1 * suunta);
			uusiSiirto[3] = alkusarake + 1;
			uusiSiirto[4] = 8;

			lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
		}
	}

	//Tarkastetaan voiko sotilas syödä viistoon vasemmalle
	if (alkusarake - 1 > -1)
	{

		//Ei menty laudan ulkopuolelle, tarkastetaan että ruudussa on nappula tai ohestalyönti on mahdollista
		if (asema->_lauta[alkurivi + (1 * suunta)][alkusarake - 1] != NULL)
		{
			// Katsotaan onko nappulan väri eri kuin oma
			if (asema->_lauta[alkurivi + (1 * suunta)][alkusarake - 1]->getVari() != vari)
			{
				//Lisätään siirto listaan
				uusiSiirto[0] = alkurivi;
				uusiSiirto[1] = alkusarake;
				uusiSiirto[2] = alkurivi + (1 * suunta);
				uusiSiirto[3] = alkusarake - 1;
				uusiSiirto[4] = 8;

				//Tarkastetaan ollaanko päästy vastakkaiseen päätyyn saakka
				if ((vari == 0 && alkurivi == 6) || (vari == 1 && alkurivi == 1))
				{
					lisaaSotilaanKorotuksetJaTarkistaShakki(uusiSiirto, lista, asema, vari);
				}
				else
				{
					lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
				}
			}
		}

		//Jos ohestalyönti
		else if (asema->kaksoisaskelSarakkeella == alkusarake - 1 && ((vari == 0 && alkurivi == 4) || (vari == 1 && alkurivi == 3)))
		{
			//Lisätään siirto listaan
			uusiSiirto[0] = alkurivi;
			uusiSiirto[1] = alkusarake;
			uusiSiirto[2] = alkurivi + (1 * suunta);
			uusiSiirto[3] = alkusarake - 1;
			uusiSiirto[4] = 8;

			lisaaSiirtoTarkistaShakki(lista, uusiSiirto, asema, vari);
		}
	}
}


//Kutsutaan jos sotilas on päässyt vastakkaiseen päätyyn saakka
void Sotilas::lisaaSotilaanKorotuksetJaTarkistaShakki(std::array<int8_t, 5> siirto, std::list<std::array<int8_t, 5>>& lista, Asema* asema, int vari)
{
	//Kuninkaan shakin tarkistaminen
	if (vari == 0)
	{

		if (siirto[2] == asema->_mkRivi && siirto[3] == asema->_mkSarake) {
			asema->_mkShakattu = true;
		}
	}
	else
	{
		if (siirto[2] == asema->_vkRivi && siirto[3] == asema->_vkSarake) {
			asema->_vkShakattu = true;
		}
	}

	//Daami
	siirto[4] = 0;
	lista.push_back(siirto);

	//Torni
	siirto[4] = 1;
	lista.push_back(siirto);

	//Lähetti
	siirto[4] = 2;
	lista.push_back(siirto);

	//Ratsu
	siirto[4] = 3;
	lista.push_back(siirto);
}
