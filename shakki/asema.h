#pragma once

#include <list>
#include <string>
#include "siirtopaluu.h"
#include "alphaBetaPaluu.h"
#include "pelitilanne.h"
#include <thread>
#include <future>
#include <atomic>
#include <deque>
#include <array>
#include <map>

// Ns. "forward declaration". Nyt Asema-luokassa voidaa esitell‰ Nappula-osoittimia ilman,
// ett‰ nappula.h -tiedostoa t‰ytyy includoida.
class Nappula;


// Asema sis‰lt‰‰ kaiken tarvittavan informaation pelitilanteen kuvaamiseksi
// (nappuloiden sijainti, siirtovuoro, linnoitusoikeudet jne.).
class Asema
{

public:
	// Pelilauta sis‰lt‰‰ osoittimet kunkin ruudun nappula-olioon (nullptr/NULL/0 jos ruutu on tyhj‰).
	// Public-m‰‰reell‰, koska t‰t‰ k‰ytet‰‰n paljon muualla.
	Nappula* _lauta[8][8];

	// Nappula-oliot. Huomaa, ett‰ samaa nappulaa voidaan k‰ytt‰‰ useissa eri ruuduissa.
	// M‰‰ritelty static-m‰‰reell‰, joten nappulat ovat kaikkien lauta-olioiden "yhteisk‰ytˆss‰"
	// (suorituskyvyn vuoksi).
	static Nappula *vk, *vd, *vt, *vl, *vr, *vs;	// Valkeat nappulat.
	static Nappula *mk, *md, *mt, *ml, *mr, *ms;	// Mustat nappulat.

	//Kuninkaiden sijainnit myˆs globaalit
	int _vkRivi;
	int _vkSarake;
	int _mkRivi;
	int _mkSarake;

	//Shakkaukset
	bool _vkShakattu;
	bool _mkShakattu;


	// Ohestalyˆnti‰ varten (-1 = sotilaan kaksoisaskelta ei tapahtunut edellisell‰ siirrolla).
	int kaksoisaskelSarakkeella = -1;

	//Pelipuun hakusyvyys
	int _hakusyvyys;
	int _keskim_haku;
	int _syvahaku;
	int _matalahaku;

	int _minTallennusSyvyys;


	Asema();															// Asettaa alkuaseman.
	void paivitaAsema(std::array<int8_t, 5>&);							// P‰ivitt‰‰ aseman annetulla siirrolla.
	Nappula* getKorotus(int vuoro);													// Kysyy korotuksen vastapelaajan korottaessa sotilaansa.
	double evaluoi(const std::list<std::array<int8_t, 5>>& nykyLista, bool onkoMax);										// Aseman numeerinen arviointi.



	//double minMaxAB(Asema* hetkellinenAsema, std::unordered_map<double, PeliTilanne>* sailio, int hetkellinenSyvyys, double minArvo, double maxArvo, bool onkoMax);	// Alphabeta (max:n siirtovuoro).
	AlphaBetaPaluu alphaBeta(int vari);													// Alphabeta-algoritmi.
	PeliTilanne parametrisoiTilanne(int hetkellinenSyvyys, bool onkoMax);						// Luo mm. hash-merkkijonon nykyisest‰ pelitilanteesta


	void annaLaillisetSiirrot(std::list<std::array<int8_t, 5>>& lista, Asema* hetkellinenAsema, int vari);	// Siirtogeneraattori.
	void annaRaakaSiirrot(std::list<std::array<int8_t, 5>>& raakasiirtoLista, Asema* hetkellinenAsema, int vari);					// Siirtogeneraattorin raakasiirtojen tuottaja.

	//bool onkoRuutuUhattu(Ruutu*, int vastustajanVari);
	void annaLinnoitusSiirrot(std::list<std::array<int8_t, 5>>& omatSiirrot, Asema* asema, int vari);

	// Karsii siirrot, jotka j‰tt‰v‰t oman K:n shakkiin.
	void huolehdiKuninkaanShakeista(std::list<std::array<int8_t, 5>>& lista, Asema* hetkellinenAsema, int vari);

	int getSiirtovuoro();									// Palauttaa siirtovuoron.
	void setSiirtovuoro(int);								// Asettaa siirtovuoron.
	bool valkoisenLyhytMahdollinen, valkoisenPitkaMahdollinen, mustanLyhytMahdollinen, mustanPitkaMahdollinen;
	bool _onkoValkeaKuningasLiikkunut;	// Linnoitus ei ole sallittu, jos kuningas on liikkunut.
	bool _onkoMustaKuningasLiikkunut;	// Linnoitus ei ole sallittu, jos kuningas on liikkunut.
	bool _onkoValkeaDTliikkunut;		// Linnoitus ei ole sallittu, jos daamisivustan torni on liikkunut.
	bool _onkoValkeaKTliikkunut;		// Linnoitus ei ole sallittu, jos kuningassivustan torni on liikkunut.
	bool _onkoMustaDTliikkunut;			// Linnoitus ei ole sallittu, jos daamisuvustan torni on liikkunut.	
	bool _onkoMustaKTliikkunut;			// Linnoitus ei ole sallittu, jos kuningassivustan torni on liikkunut.
	
private:

	// Lis‰informaatio pelitilanteesta.
	int _siirtovuoro;					// 0 = valkea, 1 = musta.
	

	double laskeNappuloidenArvo();
	bool onkoLoppupeli();
	double nappuloitaKeskella();
};