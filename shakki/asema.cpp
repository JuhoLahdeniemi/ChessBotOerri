#pragma once
#include <iostream>
#include "asema.h"
#include "siirtopaluu.h"
#include "alphaBetaPaluu.h"
#include "nappula.h"


//Säikeet
static std::deque<std::future<double>> saieJono;
static std::atomic<unsigned> varatutSaikeet;
static std::atomic<unsigned> valmiitSaikeet;
static std::atomic<unsigned> katkaisut;


static std::deque<unsigned> valmisSaieNumeroJono;
static std::atomic<bool> lopetaHaku;

//Muut tietorakenteet
static std::map<std::array<char, 65>, PeliTilanne> sailio;
static std::array<std::array<int8_t, 5>, 500> juuritasonSiirrotArray;
static std::list<Asema> seuraavaAsemaLista;
static bool onkoAlkuperainenJuuriMax;



//Liput tietorakenteiden muokkaukseen
//static std::atomic_bool sailiotaLuetaan = false;
//static std::atomic_bool sailioonKirjoitetaan = false;
static std::atomic<bool> valmisSaieNumeroJonoVapaa;


//Alpha ja Beetta
static double juurenMinAlpha;
static double juurenMaxBeta;


Nappula* Asema::vk = new Kuningas(L"\u2654", 0, VK);
Nappula* Asema::vd = new Daami(L"\u2655", 0, VD);
Nappula* Asema::vt = new Torni(L"\u2656", 0, VT);
Nappula* Asema::vl = new Lahetti(L"\u2657", 0, VL);
Nappula* Asema::vr = new Ratsu(L"\u2658", 0, VR);
Nappula* Asema::vs = new Sotilas(L"\u2659", 0, VS);

Nappula* Asema::mk = new Kuningas(L"\u265A", 1, MK);
Nappula* Asema::md = new Daami(L"\u265B", 1, MD);
Nappula* Asema::mt = new Torni(L"\u265C", 1, MT);
Nappula* Asema::ml = new Lahetti(L"\u265D", 1, ML);
Nappula* Asema::mr = new Ratsu(L"\u265E", 1, MR);
Nappula* Asema::ms = new Sotilas(L"\u265F", 1, MS);

Asema::Asema()
{
}


void Asema::paivitaAsema(std::array<int8_t, 5>& siirto)
{
	int8_t alkurivi = siirto[0];
	int8_t alkusarake = siirto[1];

	int8_t loppurivi = siirto[2];
	int8_t loppusarake = siirto[3];

	//Tarkastetaan onko siirto lyhyt linna
	if (alkurivi == 8)
	{
		if (_siirtovuoro == 0)
		{
			//Valkean lyhyt linnoitus
			_lauta[0][4] = NULL;
			_lauta[0][5] = vt;
			_lauta[0][6] = vk;
			_lauta[0][7] = NULL;

			_vkSarake = 6;
			_onkoValkeaKuningasLiikkunut = true;
			_onkoValkeaKTliikkunut = true;
		}
		else
		{
			//Mustan lyhyt linnoitus
			_lauta[7][4] = NULL;
			_lauta[7][5] = mt;
			_lauta[7][6] = mk;
			_lauta[7][7] = NULL;

			_mkSarake = 6;
			_onkoMustaKuningasLiikkunut = true;
			_onkoMustaKTliikkunut = true;
		}
	}
	else if (alkusarake == 8)
	{
		// Pitkä linna
		if (_siirtovuoro == 0)
		{
			//Valkean pitkä linnoitus				
			_lauta[0][0] = NULL;
			_lauta[0][2] = vk;
			_lauta[0][3] = vt;
			_lauta[0][4] = NULL;

			_vkSarake = 2;
			_onkoValkeaKuningasLiikkunut = true;
			_onkoValkeaDTliikkunut = true;
		}
		else
		{
			//Mustan pitkä linnoitus
			_lauta[7][0] = NULL;
			_lauta[7][2] = mk;
			_lauta[7][3] = mt;
			_lauta[7][4] = NULL;

			_mkSarake = 2;
			_onkoMustaKuningasLiikkunut = true;
			_onkoMustaDTliikkunut = true;
		}
	}
	else
	{
		// Kaikki muut siirrot

		//Ottaa siirron alkuruudussa olleen nappulan talteen 
		Nappula* siirrettava = _lauta[alkurivi][alkusarake];

		//Tyhjennetään alkuruutu
		_lauta[alkurivi][alkusarake] = NULL;



		//Sotilaan käsittely
		if (siirrettava->getKoodi() == VS || siirrettava->getKoodi() == MS)
		{

			// Ohestalyönti on tyhjään ruutuun. Vieressä oleva (sotilas) poistetaan.
			if (kaksoisaskelSarakkeella == loppusarake && alkusarake != loppusarake)
			{
				if ((siirrettava->getKoodi() == VS && alkurivi == 4) || (siirrettava->getKoodi() == MS && alkurivi == 3))
				{
					//Poistetaan siirron alkuruudun viereinen sotilas
					_lauta[alkurivi][kaksoisaskelSarakkeella] = NULL;
				}

			}

			// Tarkistetaan oliko sotilaan kaksoisaskel
			if ((loppurivi - alkurivi) % 2 == 0)
			{
				kaksoisaskelSarakkeella = loppusarake;
			}
			else
			{
				kaksoisaskelSarakkeella = -1;
			}

			// Mahdollinen korotus 
			if ((loppurivi == 0 && siirrettava->getKoodi() == MS) || (loppurivi == 7 && siirrettava->getKoodi() == VS)) {
				int8_t korotettava = siirto[4];

				switch (korotettava)
				{
				case 0:
					siirrettava = (getSiirtovuoro() == 0) ? vd : md;
					break;
				case 1:
					siirrettava = (getSiirtovuoro() == 0) ? vt : mt;
					break;
				case 2:
					siirrettava = (getSiirtovuoro() == 0) ? vl : ml;
					break;
				case 3:
					siirrettava = (getSiirtovuoro() == 0) ? vr : mr;
					break;

				default:
					siirrettava = getKorotus(getSiirtovuoro());
					break;
				}

			}
		}
		else
		{
			kaksoisaskelSarakkeella = -1;

			// Tornin onkoTorniLiikkunut arvo (molemmille väreille ja molemmille torneille)
			if (siirrettava->getKoodi() == VT)
			{
				if (alkurivi == 0 && alkusarake == 0)
				{
					_onkoValkeaDTliikkunut = true;
				}
				else if (alkurivi == 0 && alkusarake == 7)
				{
					_onkoValkeaKTliikkunut = true;
				}
			}
			else if (siirrettava->getKoodi() == MT) {
				if (alkurivi == 7 && alkusarake == 0)
				{
					_onkoMustaDTliikkunut = true;
				}
				else if (alkurivi == 7 && alkusarake == 7)
				{
					_onkoMustaKTliikkunut = true;
				}
			}
			// Kuninkaan onkoKuningasLiikkunut arvo ja sijainti (molemmille väreille)
			else if (siirrettava->getKoodi() == VK)
			{
				_onkoValkeaKuningasLiikkunut = true;
				_vkRivi = loppurivi;
				_vkSarake = loppusarake;
			}
			else if (siirrettava->getKoodi() == MK)
			{
				_onkoMustaKuningasLiikkunut = true;
				_mkRivi = loppurivi;
				_mkSarake = loppusarake;
			}

		}

		//Laittetaan talteen otettu nappula uuteen ruutuun
		_lauta[loppurivi][loppusarake] = siirrettava;



		//DEBUG
		//std::wcout << _onkoValkeaKuningasLiikkunut << " " << _onkoValkeaKTliikkunut << " " << _onkoValkeaDTliikkunut << " " << _onkoMustaKuningasLiikkunut << " " << _onkoMustaDTliikkunut << " " << _onkoMustaKTliikkunut << "\n";

	}

	//päivitetään _siirtovuoro
	setSiirtovuoro((getSiirtovuoro() == 0) ? 1 : 0);
}


int Asema::getSiirtovuoro()
{
	return _siirtovuoro;
}


void Asema::setSiirtovuoro(int vuoro)
{
	_siirtovuoro = vuoro;
}


//Jos vastustajan sotilas korotetaan
Nappula* Asema::getKorotus(int vuoro) {
	std::string korotus = "";

	while (true) {
		std::wcout << "Miksi sotilas korotetaan? 0 = daami, 1 = torni, 2 = lahetti, 3 = ratsu\n";

		std::cin >> korotus;

		if ((int)korotus[0] - 48 > -1 && (int)korotus[0] - 48 < 4)
		{
			break;
		}
	}

	int korotusnro = (int)korotus[0] - 48;

	switch (korotusnro) {
	case 0: return (vuoro == 0) ? Asema::vd : Asema::md;
	case 1: return (vuoro == 0) ? Asema::vt : Asema::mt;
	case 2: return (vuoro == 0) ? Asema::vl : Asema::ml;
	case 3: return (vuoro == 0) ? Asema::vr : Asema::mr;
	default: break;
	}
	return nullptr;
}


/* 1. Laske nappuloiden arvo
Daami = 9
Torni = 5
Lähetti = 3,25
Ratsu = 3
Sotilas = 1

2. Kuninkaan hyvyys
Jos avaus tai keskipeli, niin hyvä että kunigas g1 tai b1/c1
Loppupelissä vaikea sanoa halutaanko olla auttamassa omaa sotilasta korottumaan
vai olla estämässä vastustajan korotusta siksi ei oteta kantaa
3. Arvosta keskustaa sotilailla ja ratsuilla
4. Arvosta pitkiä linjoja daami, torni ja lähetti
*/
double Asema::evaluoi(const std::list<std::array<int8_t, 5>>& nykyLista, bool onkoMax)
{
	double ev_arvo = 0;

	//kertoimet asetettu sen takia että niiden avulla asioiden painoarvoa voidaan säätää helposti yhdestä paikasta

	//1. Nappuloiden arvo
	ev_arvo += laskeNappuloidenArvo();

	//2. Kuningas turvassa
	bool loppupeli = onkoLoppupeli();
	double kunkkuarvo = 0;
	double turva_arvo = 1;

	if (!loppupeli)
	{
		//katsotaan, onko kuningas linnoittunut & onko kuninkaan edessä 3 sotilasta 
		// (+ onko torni kuninkaan kanssa samalla rivillä suojaamassa?)

		if (onkoMax)
		{
			//valkoinen kunkku turvassa - kaksi laitaa    
			if (_lauta[0][2] == vk && _lauta[0][0] == NULL)
			{
				//Pitkä linna (toivon mukaan)

				//onko kolme sotilasta rivissä kunkun edessä..
				if ((_lauta[1][0] == vs || _lauta[2][0] == vs) && (_lauta[1][1] == vs || _lauta[2][1] == vs) && (_lauta[1][2] == vs || _lauta[2][2] == vs)) {
					kunkkuarvo += turva_arvo;
				}
			}
			else if (_lauta[0][6] == vk && _lauta[0][7] == NULL)
			{
				//Lyhyt linna (toivon mukaan)

				//onko kolme sotilasta kunkun edessä samalla laidalla..
				if ((_lauta[1][5] == vs || _lauta[2][5] == vs) && (_lauta[1][6] == vs || _lauta[2][6] == vs) && (_lauta[1][7] == vs || _lauta[2][7] == vs)) {
					kunkkuarvo += turva_arvo;
				}
			}
			else if (_onkoValkeaKuningasLiikkunut)
			{
				//Huono juttu
				kunkkuarvo -= turva_arvo;
			}
		}
		else
		{
			//Musta kunkku turvassa - kaksi laitaa    

			if (_lauta[7][2] == mk && _lauta[7][0] == NULL)
			{
				//Pitkä linna (toivon mukaan)

				//onko kolme sotilasta rivissä kunkun edessä..
				if ((_lauta[6][0] == ms || _lauta[5][0] == ms) && (_lauta[6][1] == ms || _lauta[5][1] == ms) && (_lauta[6][2] == ms || _lauta[5][2] == ms)) {
					kunkkuarvo -= turva_arvo;
				}
			}
			else if (_lauta[7][6] == mk && _lauta[7][7] == NULL)
			{
				//Lyhyt linna (toivon mukaan)

				//onko kolme sotilasta kunkun edessä samalla laidalla..
				if ((_lauta[6][5] == ms || _lauta[5][5] == ms) && (_lauta[6][6] == ms || _lauta[5][6] == ms) && (_lauta[6][7] == ms || _lauta[5][7] == ms)) {
					kunkkuarvo -= turva_arvo;
				}
			}
			else if (_onkoMustaKuningasLiikkunut)
			{
				//Huono juttu
				kunkkuarvo += turva_arvo;
			}
		}
	}

	ev_arvo += kunkkuarvo;



	//3. Arvosta keskustaa
	ev_arvo += 1 * nappuloitaKeskella();

	// 4. Arvosta linjoja
	double linjat = 0;

	for (const std::array<int8_t, 5>&siirto : nykyLista)
	{
		if (siirto[4] == 1)
		{
			//Torni (Daamin suorat)
			linjat += 0.03;
		}
		else if (siirto[4] == 2)
		{
			//Lähetti (Daamin vinot)
			linjat += 0.02;
		}
		else
		{
			//Muut siirrot
			linjat += 0.01;
		}
	}
	//Kerroin
	linjat *= 1;

	ev_arvo = (onkoMax) ? ev_arvo + linjat : ev_arvo - linjat;

	return ev_arvo;
}


double Asema::laskeNappuloidenArvo()
{
	/*
	Sotilas = 1
	Daami = 9
	Torni = 5
	Lähetti = 3,25
	Ratsu = 3
	*/

	double arvo = 0;
	int koodi = 0;
	// Käydään lauta läpi ja lasketaan arvo. Mustat negatiivisia, valkoiset positiivisia
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//Skipataan tyhjät ruudut
			if (_lauta[i][j] == NULL)
			{
				continue;
			}

			koodi = _lauta[i][j]->getKoodi();

			if (koodi == VS)
			{
				arvo += 1;
			}
			else if (koodi == MS)
			{
				arvo -= 1;
			}
			else if (koodi == VD)
			{
				arvo += 9;
			}
			else if (koodi == MD)
			{
				arvo -= 9;
			}
			else if (koodi == VT)
			{
				arvo += 5;
			}
			else if (koodi == MT)
			{
				arvo -= 5;
			}
			else if (koodi == VL)
			{
				arvo += 3.25;
			}
			else if (koodi == ML)
			{
				arvo -= 3.25;
			}
			else if (koodi == VR)
			{
				arvo += 3;
			}
			else if (koodi == MR)
			{
				arvo -= 3;
			}
		}
	}


	return arvo;
}


bool Asema::onkoLoppupeli()
{
	// Jos upseereita 3 tai vähemmän on loppupeli
	// mutta jos daami laudalla on loppupeli vasta kun kuin vain daami jäljellä

	//Jos vari on 0 eli valkoiset
	//niin on keskipeli jos mustalla upseereita yli 2 tai jos daami+1

	uint8_t upseereita = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (_lauta[i][j] == NULL)
			{
				continue;
			}

			if (_lauta[i][j]->getKoodi() == VD || _lauta[i][j]->getKoodi() == MD)
			{
				upseereita++;
			}
			else if ((_lauta[i][j]->getKoodi() == VT || _lauta[i][j]->getKoodi() == MT))
			{
				upseereita++;
			}
			else if ((_lauta[i][j]->getKoodi() == VL || _lauta[i][j]->getKoodi() == ML))
			{
				upseereita++;
			}
			else if ((_lauta[i][j]->getKoodi() == VR || _lauta[i][j]->getKoodi() == MR))
			{
				upseereita++;
			}
		}
	}

	return (upseereita > 4) ? false : true;
}


double Asema::nappuloitaKeskella()
{
	double arvo = 0;

	//sotilaat ydinkeskustassa + 0.2/nappula
	//ratsut ydinkeskustassa + 0.25/nappula
	//sotilaat laitakeskustassa + 0.11/nappula
	//ratsut laitakeskustassa + 0.15/nappula

	// Käydään keskilauta läpi ja lasketaan arvot. Mustat negatiivisia, valkoiset positiivisia
	for (int i = 3; i < 5; i++)
	{
		for (int j = 2; j < 6; j++)
		{
			//Skipataan tyhjät ruudut
			if (_lauta[i][j] == NULL)
			{
				continue;
			}

			if (j > 2 && j < 5)
			{
				//Ydinkeskusta
				if (_lauta[i][j]->getVari() == 0)
				{
					//Valkoiset
					if (_lauta[i][j]->getKoodi() == VS)
					{
						arvo += 0.2;
					}
					else if (_lauta[i][j]->getKoodi() == VR)
					{
						arvo += 0.25;
					}

				}
				else
				{
					//Mustat
					if (_lauta[i][j]->getKoodi() == MS)
					{
						arvo -= 0.2;
					}
					else if (_lauta[i][j]->getKoodi() == MR)
					{
						arvo -= 0.25;
					}
				}
			}
			//Laitakeskusta
			else if (_lauta[i][j]->getVari() == 0)
			{
				//Valkoiset
				if (_lauta[i][j]->getKoodi() == VS)
				{
					arvo += 0.11;
				}
				else if (_lauta[i][j]->getKoodi() == VR)
				{
					arvo += 0.15;
				}

			}
			else
			{
				//Mustat
				if (_lauta[i][j]->getKoodi() == MS)
				{
					arvo -= 0.11;
				}
				else if (_lauta[i][j]->getKoodi() == MR)
				{
					arvo -= 0.15;
				}
			}
		}
	}
	return arvo;
}



//--------ALPHA-BETA----------------------------
AlphaBetaPaluu hoidaMonisaikeistys(Asema* hetkellinenAsema, std::list<std::array<int8_t, 5>>& juuriTasonSiirrot, int hetkellinenSyvyys, double min, double max);
double alphaBetaSaie(Asema* hetkellinenAsema, unsigned saienro, int juuriSiirtoLkm, int syvyys);
double minMaxAB(Asema* hetkellinenAsema, int hetkellinenSyvyys, double min, double max, bool onkoMax, bool onkoSiirtosarjaKesken);

PeliTilanne Asema::parametrisoiTilanne(int hetkellinenSyvyys, bool max)
{
	PeliTilanne tilanne;
	std::array<char, 65> hash;


	//Lisätään alkuun eri merkki jos kyseessä maksimi vs. minimi
	hash[64] = (max) ? /*(char)hetkellinenSyvyys + */20 : /*(char) hetkellinenSyvyys +*/ 50;


	uint8_t indeksi = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//Tyhjät ruudut
			if (_lauta[i][j] == NULL)
			{
				hash[indeksi] = '_';
				continue;
			}

			hash[indeksi] = (char)_lauta[i][j]->getKoodi() + 100;
			indeksi++;
		}
	}


	tilanne._lautaHash = hash;
	//Evaluointiarvo annetaan vasta lisättäessä

	return tilanne;
}

AlphaBetaPaluu Asema::alphaBeta(int vari)
{
	AlphaBetaPaluu paluuarvo;

	Asema hetkellinenAsema = *this;

	double minArvo = -111111;
	double maxArvo = 111111;

	//Arvioidaan, kannattaako tehdä vielä syvempi haku kuin keskimäärin
	std::list<std::array<int8_t, 5>> juuritasonSiirrot;

	annaLaillisetSiirrot(juuritasonSiirrot, this, vari);


	//Onko järkevää? Pitäisikö prosessoida seuraavia siirtoja vielä
	if (juuritasonSiirrot.size() == 1)
	{
		paluuarvo._evaluointiArvo = 404;
		paluuarvo._parasSiirto = juuritasonSiirrot.front();
		return paluuarvo;
	}

	/*if (juuritasonSiirrot.size() > 30)
	{
		_hakusyvyys = _matalahaku;
	}
	else*/ if (juuritasonSiirrot.size() < 10)
	{
		_hakusyvyys = _syvahaku;
	}
	else
	{
		_hakusyvyys = _keskim_haku;
	}

	if (vari == 0) {
		//Pyritään maksimoimaan evaluointiarvo
		paluuarvo = hoidaMonisaikeistys(&hetkellinenAsema, juuritasonSiirrot, _hakusyvyys, minArvo, maxArvo);
	}
	else {
		//Pyritään minimoimaan evaluointiarvo
		paluuarvo = hoidaMonisaikeistys(&hetkellinenAsema, juuritasonSiirrot, _hakusyvyys, minArvo, maxArvo);
	}

	return paluuarvo;
}

AlphaBetaPaluu hoidaMonisaikeistys(Asema* hetkellinenAsema, std::list<std::array<int8_t, 5>>& juuriTasonSiirrot, int hetkellinenSyvyys, double min, double max)
{
	//Juuritason rekursio säikeiden hallinnalla
	AlphaBetaPaluu paluu;

	saieJono.clear();
	seuraavaAsemaLista.clear();
	valmisSaieNumeroJono.clear();
	lopetaHaku = false;

	//Laukaistaan jokaista juuritason siirtoa kohden 
	Asema seuraavaAS;
	unsigned saikeetJuuresta = 0;
	
	onkoAlkuperainenJuuriMax = (hetkellinenAsema->getSiirtovuoro() == 0) ? true : false;
	juurenMinAlpha = -90000;
	juurenMaxBeta = 90000;

	valmisSaieNumeroJonoVapaa.store(true);
	valmiitSaikeet.store(0);
	varatutSaikeet.store(0);
	katkaisut.store(0);


	//Talletetaan siirrot ja asemat ja laukaistaan säie niiden pohjalta
	for (std::array<int8_t, 5>&siirto : juuriTasonSiirrot)
	{
		seuraavaAS = *hetkellinenAsema;
		seuraavaAS.paivitaAsema(siirto);
		seuraavaAsemaLista.push_back(seuraavaAS);

		//Laukaistaan säie kyseiselle siirrolle
		saieJono.push_back(std::async(alphaBetaSaie, &seuraavaAsemaLista.back(), saikeetJuuresta, (unsigned)juuriTasonSiirrot.size(), hetkellinenSyvyys - 1));
		juuritasonSiirrotArray[saikeetJuuresta] = siirto;
		saikeetJuuresta++;
	}

	int jonoIndeksi = saikeetJuuresta - 1;
	bool katkaistiin = false;



	paluu._evaluointiArvo = (hetkellinenAsema->getSiirtovuoro() == 0) ? min : max;
	unsigned kasitellyt = 0;

	bool kuulutettu_75_p = false;
	bool kuulutettu_50_p = false;
	bool kuulutettu_25_p = false;
	std::chrono::milliseconds aikaJakso = std::chrono::milliseconds(1000);
	std::chrono::milliseconds aikaaKulunut = std::chrono::milliseconds(0);

	double testipaluu;

	//Odotetaan valmistumista
	while (kasitellyt < saikeetJuuresta)
	{
		aikaaKulunut += aikaJakso;
		std::this_thread::sleep_for(aikaJakso);



		//Käsitellään valmiit säikeet
		while (!valmisSaieNumeroJono.empty())
		{
			testipaluu = saieJono.at(valmisSaieNumeroJono.front()).get();

			if (hetkellinenAsema->getSiirtovuoro() == 0)
			{
				if (testipaluu > paluu._evaluointiArvo)
				{
					//std::wcout << "EV muuttui " << paluu._evaluointiArvo << " -> " << testipaluu << "\n";
					paluu._evaluointiArvo = testipaluu;
					paluu._parasSiirto = juuritasonSiirrotArray[valmisSaieNumeroJono.front()];
					
					//Päivitetään minimiä
					juurenMinAlpha = testipaluu;
				}
			}
			else
			{
				if (testipaluu < paluu._evaluointiArvo)
				{
					//std::wcout << "EV muuttui: " << paluu._evaluointiArvo << " -> " << testipaluu << "\n";
					paluu._evaluointiArvo = testipaluu;
					paluu._parasSiirto = juuritasonSiirrotArray[valmisSaieNumeroJono.front()];

					//Päivitetään maksimia
					juurenMaxBeta = testipaluu;
				}
			}

			valmisSaieNumeroJono.pop_front();
			kasitellyt++;
		}

		//Printataan montako ollaan käsitelty ja säiliöstä löytyneet asemat
		/*std::wcout << "\r" << kasitellyt << "/" << saikeetJuuresta
			<< "  minA = " << juurenMinAlpha
			<< "  maxB = " << juurenMaxBeta
			<< "  katk = " << katkaisut << std::flush;*/



			//Huvin ja hyödyn vuoksi prosentit kuulutetaan
		if (!kuulutettu_25_p)
		{
			if (valmiitSaikeet.load() > juuriTasonSiirrot.size() / 4)
			{
				std::wcout << "25% laskettu\n";
				kuulutettu_25_p = true;
			}
		}
		else if (!kuulutettu_50_p)
		{
			if (valmiitSaikeet.load() > juuriTasonSiirrot.size() / 2)
			{
				std::wcout << "50% laskettu\n";
				kuulutettu_50_p = true;
			}
		}
		else if (!kuulutettu_75_p)
		{
			if (valmiitSaikeet.load() > juuriTasonSiirrot.size() / 4 * 3)
			{
				std::wcout << "75% laskettu\n";
				kuulutettu_75_p = true;
			}
		}


		//Jos siirto on kestänyt yli 25 sekuntia, niin valitaan paras toistaiseksi laskettu siirto
		if (aikaaKulunut > std::chrono::milliseconds(25000) && kasitellyt > 0)
		{
			//Varmuudeksi odotetaan ylimääräiset 5 sekuntia
			std::wcout << "Lopetetaan haku 30 sekunnin kohdalla... " << kasitellyt << " / " << saikeetJuuresta << " siirtoa saatiin laskettua.\n";
			katkaistiin = true;
			break;
		}
	}

	//Jos haku lopetettiin kesken
	if (katkaistiin)
	{
		//Kutsutaan kaikissa säikeissä lopetusta ja odotetaan 5 sekuntia
		lopetaHaku = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}

	//std::wcout << "Sailion koko = " << sailio.size() << " asemaa." << "\n";

	return paluu;
}









double alphaBetaSaie(Asema* hetkellinenAsema, unsigned saienro, int juuriSiirtoLkm, int hetkellinenSyvyys)
{
	double paluu = (hetkellinenAsema->getSiirtovuoro() == 0) ? -111000 : 111000;

	//Lisätään tämän aseman siirrot listaan
	std::list<std::array<int8_t, 5>> siirrotTestiAsemassa;
	hetkellinenAsema->annaLaillisetSiirrot(siirrotTestiAsemassa, hetkellinenAsema, hetkellinenAsema->getSiirtovuoro());


	//Laukaistaan uusi säie jokaista siirtoa kohden

	std::deque<std::future<double>> alipuunSaikeet;
	Asema testiAsema;
	std::list<Asema> seuraavaAsemaLista;
	double min = -111110;
	double max = 111110;

	for (std::array<int8_t, 5>&siirto : siirrotTestiAsemassa)
	{
		testiAsema = *hetkellinenAsema;
		testiAsema.paivitaAsema(siirto);
		seuraavaAsemaLista.push_back(testiAsema);

		//HUOM: ollaan nyt jo 1 askel juuresta eli etsitään vastakkaista arvoa juureen verrattuna
		alipuunSaikeet.push_back(std::async(minMaxAB, &seuraavaAsemaLista.back(), hetkellinenSyvyys - 1, min, max, (hetkellinenAsema->getSiirtovuoro() == 0) ? false : true, true));
	}



	//Käsitellään säikeet
	//HUOM: ollaan nyt jo 1 askel juuresta eli otetaan vastakkainen arvo juureen verrattuna
	double alipuunPaluuArvo;
	while (!alipuunSaikeet.empty())
	{
		alipuunPaluuArvo = alipuunSaikeet.back().get();

		if (hetkellinenAsema->getSiirtovuoro() == 0)
		{
			if (alipuunPaluuArvo > paluu)
			{
				paluu = alipuunPaluuArvo;
			}
		}
		else
		{

			if (alipuunPaluuArvo < paluu)
			{
				paluu = alipuunPaluuArvo;
			}
		}
		alipuunSaikeet.pop_back();
	}

	//Ilmoitetaan valmistumisesta main threadille
	bool ilmoitettu = false;
	while (!ilmoitettu)
	{
		if (valmisSaieNumeroJonoVapaa.load())
		{
			valmisSaieNumeroJonoVapaa.store(false);
			valmisSaieNumeroJono.push_back(saienro);
			valmisSaieNumeroJonoVapaa.store(true);
			ilmoitettu = true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10 * saienro));
	}

	valmiitSaikeet += 1;
	return paluu;
}










double minMaxAB(Asema* hetkellinenAsema, int hetkellinenSyvyys, double min, double max, bool onkoMax, bool onkoSiirtosarjaKesken)
{
	if (lopetaHaku)
	{
		return 504;
	}

	std::list<std::array<int8_t, 5>> siirrot;
	Asema testiAsema;

	//Generoidaan kaikki mahdolliset siirrot
	hetkellinenAsema->annaLaillisetSiirrot(siirrot, hetkellinenAsema, (onkoMax) ? 0 : 1);


	//Alustetaan evaluointiarvo
	double paluu = (onkoMax) ? -111000 : 111000;
	double vertailuArvo;

	//Tarkastetaan onko matti tai patti
	if (siirrot.size() == 0)
	{
		//katsotaan saako vastapuoli lyötyä oman kuninkaan
		hetkellinenAsema->annaRaakaSiirrot(siirrot, hetkellinenAsema, (onkoMax) ? 1 : 0);

		if ((onkoMax && hetkellinenAsema->_vkShakattu) || (!onkoMax && hetkellinenAsema->_mkShakattu))
		{
			//Matti
			return (onkoMax) ? -100000 - hetkellinenSyvyys : 100000 + hetkellinenSyvyys;

		}
		else
		{
			//Patti
			return 0;
		}
	}


	//Kantatapaus: Ollaan katkaisusyvyydessä
	if (hetkellinenSyvyys == 0)
	{
		//Täällä varmistetaan myös ettei siirtosarjat jää kesken

		//Jotta ei lopetettaisi liian lyhytnäköisesti arviointia omaan vuoroon
		if ((!onkoMax && onkoAlkuperainenJuuriMax) || (onkoMax && !onkoAlkuperainenJuuriMax))
		{
			if (onkoSiirtosarjaKesken)
			{
				//Tehdään vielä yksi rekursiivinen kutsu tasolla 0
				double testipaluu = -paluu;
				for (std::array<int8_t, 5>&siirto : siirrot)
				{
					testiAsema = *hetkellinenAsema;
					testiAsema.paivitaAsema(siirto);

					vertailuArvo = minMaxAB(&testiAsema, 0, min, max, (onkoMax) ? false : true, false);

					//Otetaan vastustajan paras siirto
					if (onkoMax)
					{
						if (vertailuArvo < testipaluu)
						{
							testipaluu = vertailuArvo;
						}
					}
					else
					{
						if (vertailuArvo > testipaluu)
						{
							testipaluu = vertailuArvo;
						}
					}
				}
				//Lasketaan hetkellisen aseman ja vihollisen parhaan siirron tuottaman aseman evaluoinnin summa
				paluu = hetkellinenAsema->evaluoi(siirrot, onkoMax) + testipaluu;
				//evaluoinnitS++;
				return paluu;
			}
		}

		return hetkellinenAsema->evaluoi(siirrot, onkoMax);
	}

	
	//Tarkistetaan ollaanko jo oltu tässä asemassa tällä syvyydellä maximisiirrolla

	//PeliTilanne tilanne;
	//bool luettu = false;
	//if (!sailioonKirjoitetaan.load())
	//{
	//	tilanne = hetkellinenAsema->parametrisoiTilanne(hetkellinenSyvyys, onkoMax);
	//	auto sailioIterator = sailio.find(tilanne._lautaHash);

	//	if (sailioIterator != sailio.end())
	//	{
	//		//Tilanne löytyi hajautustaulusta
	//		hakuOsumatS++;
	//		return sailioIterator->second._evaluointiArvo;
	//	}
	//	luettu = true;
	//}

	unsigned moneskoSiirto = 0;

	//Kokeillaan kaikki siirrot läpi ja asetetaan paluuarvoksi parhaiten evaluoituva
	bool tallennetaan = true;
	for (std::array<int8_t, 5>&siirto : siirrot)
	{

		// Päivitetään tarvittaessa minimiä / maksimia samaksi kuin juuressa
		if (onkoAlkuperainenJuuriMax)
		{
			if (min < juurenMinAlpha)
			{
				min = juurenMinAlpha;
			}
		}
		else
		{
			if (max > juurenMaxBeta)
			{
				max = juurenMaxBeta;
			}
		}


		testiAsema = *hetkellinenAsema;
		testiAsema.paivitaAsema(siirto);

		vertailuArvo = minMaxAB(&testiAsema, hetkellinenSyvyys - 1, min, max, (onkoMax) ? false : true, onkoSiirtosarjaKesken);


		if (onkoMax)
		{
			if (vertailuArvo > paluu)
			{
				paluu = vertailuArvo;
				min = vertailuArvo;

				if (min >= max)
				{
					//katkaisu
					//tallennetaan = false;
					katkaisut++;
					break;
				}
			}
		}
		else
		{

			if (vertailuArvo < paluu)
			{
				paluu = vertailuArvo;
				max = vertailuArvo;

				if (max <= min)
				{
					//katkaisu
					//tallennetaan = false;
					katkaisut++;
					break;
				}
			}
		}

		moneskoSiirto++;
	}

	//Lisätään testattu tilanne hajautustauluun jos:
	//1. Ei olla liian syvällä tai aloitussyvyydessä
	//2. Ketjua ei katkaistu ennen kuin kaikki haarat tutkittiin

	//if (tallennetaan)
	//{
	//	tilanne._evaluointiArvo = paluu;

	//	if (true/*!sailioonKirjoitetaan.load()*/)
	//	{
	//		//sailioonKirjoitetaan.store(true);
	//		sailio.insert({ tilanne._lautaHash, tilanne });
	//		//sailioonKirjoitetaan.store(false);
	//	}
	//}

	return paluu;
}

//-------/ALPHA-BETA----------------------------

void Asema::huolehdiKuninkaanShakeista(std::list<std::array<int8_t, 5>>& lista, Asema* hetkellinenAsema, int vari)
{
	// Tehdään pseudosiirto ja tutkitaan jääkö kuningas uhatuksi
	Asema testiAsema;
	std::list<std::array<int8_t, 5>> pseudoSiirronJalkeen;
	std::list<std::array<int8_t, 5>> siivottuLista;

	for (std::array<int8_t, 5>&siirto : lista)
	{
		//Asetetaan testiasema nykyhetkeen
		testiAsema = *hetkellinenAsema;

		//Pseudosiirto
		testiAsema.paivitaAsema(siirto);


		//Tarkistetaan jäikö kuningas shakkiin (katsotaan vastapelaajan raakasiirroista seuraavalla vuorolla)
		testiAsema.annaRaakaSiirrot(pseudoSiirronJalkeen, &testiAsema, (vari == 0) ? 1 : 0);

		if (vari == 0 && !testiAsema._vkShakattu || vari == 1 && !testiAsema._mkShakattu)
		{
			siivottuLista.push_back(siirto);
		}

	}
	lista = siivottuLista;
}

//Antaa raakasiirrot valitulle värille (ei tarkistettu kuninkaan uhatuksi jäämistä)
void Asema::annaRaakaSiirrot(std::list<std::array<int8_t, 5>>& raakasiirtoLista, Asema* hetkellinenAsema, int vari) {
	//Resetoidaan kuninkaiden shakkaustilanne ja lista
	raakasiirtoLista.clear();
	_vkShakattu = false;
	_mkShakattu = false;
	valkoisenLyhytMahdollinen = true;
	valkoisenPitkaMahdollinen = true;
	mustanLyhytMahdollinen = true;
	mustanPitkaMahdollinen = true;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//Skipataan tyhjät ruudut
			if (hetkellinenAsema->_lauta[i][j] == NULL)
			{
				continue;
			}

			if (hetkellinenAsema->_lauta[i][j]->getVari() == vari)
			{
				hetkellinenAsema->_lauta[i][j]->annaSiirrot(raakasiirtoLista, i, j, hetkellinenAsema, vari);
			}
		}
	}
}

void Asema::annaLaillisetSiirrot(std::list<std::array<int8_t, 5>>& lista, Asema* hetkellinenAsema, int vari) {

	//Nakkaa listaan kaikki omat raakasiirrot
	annaRaakaSiirrot(lista, hetkellinenAsema, vari);

	//Lisätään raakasiirtolistaan mahdolliset linnoitussiirrot
	//Ensin haetaan vastustajan raakasiirrot, mitkä triggeröi liput linnoitusten estäville siirroille
	std::list<std::array<int8_t, 5>> vastustajanSiirrot;
	annaRaakaSiirrot(vastustajanSiirrot, hetkellinenAsema, (vari == 0) ? 1 : 0);

	annaLinnoitusSiirrot(lista, hetkellinenAsema, vari);

	//Poistetaan ne (laittomat) siirrot, joissa kuningas jäisi shakkiin
	huolehdiKuninkaanShakeista(lista, hetkellinenAsema, vari);
	if (vari == 0)
	{
		hetkellinenAsema->_vkShakattu = 0;
	}
	else
	{
		hetkellinenAsema->_mkShakattu = 0;
	}
}


//Annetaan linnoitussiirrot
void Asema::annaLinnoitusSiirrot(std::list<std::array<int8_t, 5>>& omatSiirrot, Asema* hetkellinenAsema, int vari) {

	std::array<int8_t, 5> linnoitussiirto;

	// Tarkistetaan että ehdot pitävät paikkansa

	if (!hetkellinenAsema->_onkoValkeaKuningasLiikkunut && vari == 0)
	{
		if (!hetkellinenAsema->_onkoValkeaKTliikkunut && hetkellinenAsema->valkoisenLyhytMahdollinen)
		{
			//Katsotaan onko kuninkaan ja KT:n välissä nappuloita ja torni on olemassa
			if (hetkellinenAsema->_lauta[0][5] == NULL && hetkellinenAsema->_lauta[0][6] == NULL && hetkellinenAsema->_lauta[0][7] == vt)
			{
				//Valkean lyhyt linnoitus mahdollinen
				linnoitussiirto[0] = 8;		// 8 == lyhyt linna
				linnoitussiirto[1] = 10;	// 10 == tyhjä
				linnoitussiirto[2] = 0;
				linnoitussiirto[3] = 6;
				linnoitussiirto[4] = 0;

				omatSiirrot.push_back(linnoitussiirto);
			}
		}

		if (!hetkellinenAsema->_onkoValkeaDTliikkunut && hetkellinenAsema->valkoisenPitkaMahdollinen)
		{
			//Katsotaan onko kuninkaan ja DT:n välissä nappuloita ja torni on olemassa
			if (hetkellinenAsema->_lauta[0][1] == NULL && hetkellinenAsema->_lauta[0][2] == NULL && hetkellinenAsema->_lauta[0][3] == NULL && hetkellinenAsema->_lauta[0][0] == vt)
			{
				//Valkean pitkä linnoitus mahdollinen
				linnoitussiirto[0] = 10;	// 10 == tyhjä	
				linnoitussiirto[1] = 8;		// 8 == pitkä linna
				linnoitussiirto[2] = 0;
				linnoitussiirto[3] = 2;
				linnoitussiirto[4] = 0;
				omatSiirrot.push_back(linnoitussiirto);
			}
		}
	}

	if (!hetkellinenAsema->_onkoMustaKuningasLiikkunut && vari == 1)
	{
		if (!hetkellinenAsema->_onkoMustaKTliikkunut && hetkellinenAsema->mustanLyhytMahdollinen)
		{
			//Katsotaan onko kuninkaan ja KT:n välissä nappuloita ja torni on olemassa
			if (hetkellinenAsema->_lauta[7][5] == NULL && hetkellinenAsema->_lauta[7][6] == NULL && hetkellinenAsema->_lauta[7][7] == mt)
			{
				//Mustan lyhyt linnoitus mahdollinen
				linnoitussiirto[0] = 8;		// 8 == lyhyt linna
				linnoitussiirto[1] = 10;	// 10 == tyhjä
				linnoitussiirto[2] = 7;
				linnoitussiirto[3] = 6;
				linnoitussiirto[4] = 0;
				omatSiirrot.push_back(linnoitussiirto);
			}
		}

		if (!hetkellinenAsema->_onkoMustaDTliikkunut && hetkellinenAsema->mustanPitkaMahdollinen)
		{
			//Katsotaan onko kuninkaan ja DT:n välissä nappuloita ja torni on olemassa
			if (hetkellinenAsema->_lauta[7][1] == NULL && hetkellinenAsema->_lauta[7][2] == NULL && hetkellinenAsema->_lauta[7][3] == NULL && hetkellinenAsema->_lauta[7][0] == mt)
			{
				//Mustan pitkä linnoitus mahdollinen
				linnoitussiirto[0] = 10;	// 10 == tyhjä
				linnoitussiirto[1] = 8;		// 8 == pitkä linna
				linnoitussiirto[2] = 7;
				linnoitussiirto[3] = 2;
				linnoitussiirto[4] = 0;
				omatSiirrot.push_back(linnoitussiirto);
			}
		}
	}
}