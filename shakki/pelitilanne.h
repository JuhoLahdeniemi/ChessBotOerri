#pragma once

class PeliTilanne
{
public:
	std::array<char, 65> _lautaHash;
	double _evaluointiArvo;
};

//TÄÄLTÄ LÖYTYY PELISKENAARIOITA

//// Ensin alustetaan kaikki laudan ruudut nappulla "NULL", koska muuten ruuduissa satunnaista tauhkaa
	//for (int i = 0; i < 8; i++)
	//{	
	//	for (int j = 0; j < 8; j++)
	//	{
	//		_lauta[i][j] = NULL;	
	//	}
	//}
	//// Asetetaan alkuaseman mukaisesti nappulat ruuduille

	//_lauta[0][0] = vt;
	//_lauta[0][1] = vr;
	//_lauta[0][2] = vl;
	//_lauta[0][3] = vd;
	//_lauta[0][4] = vk;
	//_lauta[0][5] = vl;
	//_lauta[0][6] = vr;
	//_lauta[0][7] = vt;

	//_lauta[7][0] = mt;
	//_lauta[7][1] = mr;
	//_lauta[7][2] = ml;
	//_lauta[7][3] = md;
	//_lauta[7][4] = mk;
	//_lauta[7][5] = ml;
	//_lauta[7][6] = mr;
	//_lauta[7][7] = mt;

	//for (int j = 0; j < 8; j++)
	//{
	//	_lauta[1][j] = vs;
	//}

	//for (int j = 0; j < 8; j++)
	//{
	//	_lauta[6][j] = ms;
	//}
	//
	////Asetetaan kaikki liput epätosiksi
	//_onkoValkeaKuningasLiikkunut = false;
	//_onkoValkeaKTliikkunut = false;
	//_onkoValkeaDTliikkunut = false;

	//_onkoMustaKuningasLiikkunut = false;
	//_onkoMustaKTliikkunut = false;
	//_onkoMustaDTliikkunut = false;

	////Asetetaan kuninkaiden arvot oikeiksi
	//_vkShakattu = false;
	//_vkRivi = 0;
	//_vkSarake = 4;

	//_mkShakattu = false;
	//_mkRivi = 7;
	//_mkSarake = 4;



	////Asetetaan siirtovuoro valkoiselle
	//setSiirtovuoro(0);

	/*
	//PELISKENAARIO 1

	// Ensin alustetaan kaikki laudan ruudut nappulla "NULL", koska muuten ruuduissa satunnaista tauhkaa
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			_lauta[i][j] = NULL;
		}
	}


	// Asetetaan aseman mukaisesti nappulat ruuduille


	_lauta[4][0] = vk;

	for (int j = 0; j < 8; j++)
	{
		_lauta[5][j] = vs;
	}

	for (int j = 0; j < 8; j++)
	{
		_lauta[2][j] = ms;
	}

	_lauta[4][7] = mk;

	//Asetetaan kaikki liput
	_onkoValkeaKuningasLiikkunut = true;
	_onkoValkeaKTliikkunut = true;
	_onkoValkeaDTliikkunut = true;

	_onkoMustaKuningasLiikkunut = true;
	_onkoMustaKTliikkunut = true;
	_onkoMustaDTliikkunut = true;

	//Asetetaan kuninkaiden arvot oikeiksi
	_vkShakattu = false;
	_vkRivi = 4;
	_vkSarake = 0;

	_mkShakattu = false;
	_mkRivi = 4;
	_mkSarake = 7;



	//Asetetaan siirtovuoro mustalle
	setSiirtovuoro(1);
	*/


	/*
	//PELISKENAARIO 2

	// Ensin alustetaan kaikki laudan ruudut nappulla "NULL", koska muuten ruuduissa satunnaista tauhkaa
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			_lauta[i][j] = NULL;
		}
	}


	// Asetetaan aseman mukaisesti nappulat ruuduille


	_lauta[0][0] = vt;
	_lauta[0][1] = vr;
	_lauta[0][2] = vl;
	_lauta[0][3] = vd;
	_lauta[0][4] = vk;

	_lauta[0][6] = vr;
	_lauta[0][7] = vt;




	_lauta[1][0] = vs;
	_lauta[1][1] = vs;


	_lauta[1][5] = vs;
	_lauta[1][6] = vs;
	_lauta[1][7] = vs;


	_lauta[6][0] = ms;
	_lauta[6][1] = ml;
	_lauta[6][2] = ms;


	_lauta[6][5] = ms;
	_lauta[6][6] = vs;
	_lauta[6][7] = ms;


	_lauta[7][3] = mt;
	_lauta[7][4] = mk;


	_lauta[7][7] = mt;

	//Asetetaan kaikki liput
	_onkoValkeaKuningasLiikkunut = false;
	_onkoValkeaKTliikkunut = false;
	_onkoValkeaDTliikkunut = false;

	_onkoMustaKuningasLiikkunut = true;
	_onkoMustaKTliikkunut = true;
	_onkoMustaDTliikkunut = true;

	//Asetetaan kuninkaiden arvot oikeiksi
	_vkShakattu = false;
	_vkRivi = 0;
	_vkSarake = 4;

	_mkShakattu = false;
	_mkRivi = 7;
	_mkSarake = 4;



	//Asetetaan siirtovuoro mustalle
	setSiirtovuoro(1);
	*/