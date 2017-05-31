//sortowanie naturalne
// Katarzyna Sidorczak 132870


#include <iostream>
#include <string>
#include<fstream>
#include<conio.h>
using namespace std;

#define ZAKRES_A 100
#define ZAKRES_Y 5
#define ZAKRES_C 5
#define ZAKRES_Z 5
#define ZAKRES_X 10

#define MAX_ILOSC_SERII 100
#define ROZMIAR_BUFORA 
#define SCHEMAT_WEJ 2
#define ILE_BAJTOW_W_BUFORZE 10*5*8
#define ILE_DOUBLE_W_BUFORZE 10*5
#define ILE_REKORDOW_W_BUFORZE 10

#define WYSWIETLANIE 1

char plik[40] = "test.bin";
int ilosc_tasm_wejsciowych = 2;
int ilosc_tasm_wyjsciowych=1;

struct rekord {
	double a;
	double y;
	double c;
	double z;
	double x;
};

long double policz_wartosc_rekord(rekord liczony)
{
	long double wynik = 0;
	double a = liczony.a;
	double y = liczony.y;
	double c = liczony.c;
	double z = liczony.z;
	double x = liczony.x;
	wynik = 10 * a*(x*x + 3 * c*c*c*z*z*z*z - 5 * y*y*y*y*y*y*y);
	return wynik;
}


long double policz_wartosc(double a, double y, double c, double z, double x)
{
	long double wynik = 0;
	wynik = 10 * a*(x*x + 3 * c*c*c*z*z*z*z - 5 * y*y*y*y*y*y*y);
	return wynik;
}

int ile_rekordow_bin(string nazwa) {
	ifstream ccc(nazwa, ios::binary | ios::ate);
	int wielkosc = (ccc.tellg())/40;//rekord to 5 doubli ka¿dy po 8 bajtów
	ccc.close();
	return wielkosc;
}

//zwraca ile seri by³o w pliku
int ile_serii(string nazwa) {
	int licznik = 0;
	int serie = 0;
	bool nowa = true;
	double uporzadkowanie = 0;
	double poprzednie_uporzadkowanie = DBL_MAX;
	double wartosc_a, wartosc_y, wartosc_c, wartosc_z, wartosc_x;
	string line;
	std::ifstream wyswietlany(nazwa);
	if (wyswietlany.is_open())
	{
		while (wyswietlany >> wartosc_a >> wartosc_y >> wartosc_c >> wartosc_z >> wartosc_x)
		{
			uporzadkowanie = policz_wartosc(wartosc_a, wartosc_y, wartosc_c, wartosc_z, wartosc_x);
			licznik++;
			if (poprzednie_uporzadkowanie <= uporzadkowanie)
				nowa = false;
			else
			{
				nowa = true;
				serie++;
			}
			poprzednie_uporzadkowanie = uporzadkowanie;
		}
		wyswietlany.close();
		return serie;
	}
	else
	{
		cout << "Blad z otwarciem pliku- w funkcji ile serii"<<endl;
		return -1;
	}
}


void wyswietl_plik_bin(char *nazwa, bool wyswietlanie) {
	int ilosc_rekordow_pozostala_do_wyswietlenia = ile_rekordow_bin(nazwa);
	int licznik = 0;
	int serie = 0;
	bool nowa = true;
	double uporzadkowanie = 0;
	double poprzednie_uporzadkowanie = DBL_MAX;
	double wartosc_a, wartosc_y, wartosc_c, wartosc_z, wartosc_x;

	ifstream binarka;
	binarka.open(nazwa, ios::binary | ios::in);
	
	if (binarka.is_open())
	{
		while (ilosc_rekordow_pozostala_do_wyswietlenia>0)
		{
			binarka.read((char*)& wartosc_a, sizeof(wartosc_a));
			binarka.read((char*)& wartosc_y, sizeof(wartosc_y));
			binarka.read((char*)& wartosc_c, sizeof(wartosc_c));
			binarka.read((char*)& wartosc_z, sizeof(wartosc_z));
			binarka.read((char*)& wartosc_x, sizeof(wartosc_x));
			ilosc_rekordow_pozostala_do_wyswietlenia--;
			uporzadkowanie = policz_wartosc(wartosc_a, wartosc_y, wartosc_c, wartosc_z, wartosc_x);
			licznik++;
			if (poprzednie_uporzadkowanie <= uporzadkowanie)
				nowa = false;
			else
				nowa = true;
			if (nowa == true)
			{
				serie++;
				if (wyswietlanie == true)cout << "\n\n------SERIA NR " << serie << "-------" << endl;
			}
			if (wyswietlanie == true)cout << wartosc_a << " " << wartosc_y << " " << wartosc_c << " " << wartosc_z << " " << wartosc_x
				<< "\t\twartosc = " << uporzadkowanie << endl;
			poprzednie_uporzadkowanie = uporzadkowanie;
		}
		if (wyswietlanie == true)cout << endl;
		if (wyswietlanie == true)cout << "Plik zawiera " << licznik << " rekordów" << endl;
		if (wyswietlanie == true)cout << "Zawartych w " << serie << " seriach" << endl;



		binarka.close();

	}
	else
	{
		cout << "Blad z otwarciem pliku w wyswietlaniu pliku " << nazwa << endl;;

	}
}



void generowanie_pliku(char *nazwa, int ilosc) {
	//std::ofstream tworzony(nazwa);
	ofstream binarka;
	binarka.open(nazwa, ios::binary | ios::out);
	double a, y, c, z, x;
	double bufor[5];

	for (int i = 0; i < ilosc; i++)
	{
		a = rand()*ZAKRES_A /(double) RAND_MAX;
		y = (rand()*ZAKRES_Y) /(double) RAND_MAX;
		c = (rand()*ZAKRES_C) / (double)RAND_MAX;
		z = (rand()*ZAKRES_Z) / (double)RAND_MAX;
		x = (rand()*ZAKRES_X) / (double)RAND_MAX;
		//tworzony << a<<" "<<y<<" "<<c<<" "<<z<<" "<<x<< std::endl;
		bufor[0] = a;
		bufor[1] = y;
		bufor[2] = c;
		bufor[3] = z;
		bufor[4] = x;

		binarka.write((char*)&bufor, sizeof(bufor));
	}
	//tworzony.close();
	binarka.close();
	wyswietl_plik_bin(nazwa, WYSWIETLANIE);
};

void utworz_plik(char *nazwa) {//recznie wklep rekordy
	int liczba;
	cout << "ile rekordow? ";
	cin >> liczba;

	//std::ofstream tworzony(nazwa);
	ofstream binarka;
	binarka.open(nazwa, ios::binary | ios::out);
	double bufor[5];
	double a, y, c, z, x;
	for (int i = 1; i <= liczba; i++) {
		cout << "\npodaj " << i << " rekord z "<<liczba<<" rekordow"<< endl;
		cin >> a >> y >> c >> z >> x;
		bufor[0] = a;
		bufor[1] = y;
		bufor[2] = c;
		bufor[3] = z;
		bufor[4] = x;
		binarka.write((char*)&bufor, sizeof(bufor));
		
		//tworzony << a << " " << y << " " << c << " " << z << " " << x << "\n";
	}
	//tworzony.close();
	binarka.close();
	wyswietl_plik_bin(nazwa,false);//////////////////////////////////////////////////////////////////////////////

}



void sortowanie_start() {
	cout << "Zaczynam sortowanie naturalne w schemacie 2 + 1" << endl;
	bool koniec = false;
	
	//FAZA PODZIA£U
	ifstream wejsciowy;
	ofstream * tasmy = new ofstream[SCHEMAT_WEJ];//tasmy podzialu
	rekord  obrabiany;	
	double bufor_odczytu[ILE_DOUBLE_W_BUFORZE];
	double bufor_zapisu[SCHEMAT_WEJ][ILE_DOUBLE_W_BUFORZE];
	int wsk_buf_odczytu;
	int wsk_buf_zapisu[SCHEMAT_WEJ];
	long double poprzednie_uporzadkowanie = DBL_MAX;

	//FAZA SCALANIA
	ofstream scalony;
	ifstream *odczyt_tasm=new ifstream[SCHEMAT_WEJ];
	rekord rekord_z_tasmy[SCHEMAT_WEJ];
	double bufor_odczytu_scalanie[SCHEMAT_WEJ][ILE_DOUBLE_W_BUFORZE];
	double bufor_zapisu_scalanie[ILE_DOUBLE_W_BUFORZE];
	int ile_zostalo_do_odczytu[SCHEMAT_WEJ];
	int wsk_buf_odczytu_scalanie[SCHEMAT_WEJ];
	int wsk_buf_zapisu_scalanie;
	long double wartosc_uporzadkowania[SCHEMAT_WEJ];
	long double poprzednia_wartosc_uporzadkowania = DBL_MAX;
	int mniejsze;
	int wieksze;
	int skad_birzemy_serie;
	
	int ilosc_zapisow = 0;
	int ilosc_odczytow = 0;



	int ile_rekordow_na_wejsciu;
	int ktory_rekord_odczytuje = 1;

	//int ile_rekordow_na_tasmie[SCHEMAT_WEJ];


	bool nowa = true;
	int serie = 0;
	double licznik=0;
	double uporzadkowanie;

	int gdzie_zapisuje_serie = 0;


	cout << endl;
	for (int faza = 1; koniec == false; faza++)//kolejne fazy sortowania
	{
		cout << "******************\t\tFAZA " << faza <<"\t\t*******************"<< endl;
		cout << "ETAP PODZIALU:" << endl;
		wsk_buf_odczytu = 0;
		poprzednie_uporzadkowanie = DBL_MAX;
		ktory_rekord_odczytuje = 1;
		gdzie_zapisuje_serie = 0;

		ile_rekordow_na_wejsciu = ile_rekordow_bin(plik);
		//cout << "mam do zrobienia" << ile_rekordow_na_wejsciu << endl;
		wejsciowy.open(plik, ios::binary | ios::in);
		for (int i = 0; i < SCHEMAT_WEJ; i++)
		{
			tasmy[i].open((to_string(i) + "podzial.bin"), ios::binary | ios::out);
			wsk_buf_zapisu[i] = 0;
		}
		if (wejsciowy.is_open())
		{
			while (ktory_rekord_odczytuje<= ile_rekordow_na_wejsciu)
			{
				if (wsk_buf_odczytu == 0)
				{
					wejsciowy.read((char*)& bufor_odczytu, sizeof(bufor_odczytu));
					//cout << "³aduje nowy bufor odczytu " << endl;
					ilosc_odczytow++;
				}
				
				obrabiany.a = bufor_odczytu[wsk_buf_odczytu * 5 + 0];
				obrabiany.y = bufor_odczytu[wsk_buf_odczytu * 5 + 1];
				obrabiany.c = bufor_odczytu[wsk_buf_odczytu * 5 + 2];
				obrabiany.z = bufor_odczytu[wsk_buf_odczytu * 5 + 3];
				obrabiany.x = bufor_odczytu[wsk_buf_odczytu * 5 + 4];

				uporzadkowanie = policz_wartosc(obrabiany.a, obrabiany.y, obrabiany.c, obrabiany.z, obrabiany.x);
				licznik++;
				if (poprzednie_uporzadkowanie <= uporzadkowanie)
					nowa = false;
				else
				{
					nowa = true;
					serie++;
					//cout << "----SERIA NR " << serie << "---zapisuje w tasmie " << gdzie_zapisuje_serie << endl;
					gdzie_zapisuje_serie++;
					gdzie_zapisuje_serie = gdzie_zapisuje_serie%SCHEMAT_WEJ;

				}

				bufor_zapisu[gdzie_zapisuje_serie][wsk_buf_zapisu[gdzie_zapisuje_serie] * 5 + 0] = obrabiany.a;
				bufor_zapisu[gdzie_zapisuje_serie][wsk_buf_zapisu[gdzie_zapisuje_serie] * 5 + 1] = obrabiany.y;
				bufor_zapisu[gdzie_zapisuje_serie][wsk_buf_zapisu[gdzie_zapisuje_serie] * 5 + 2] = obrabiany.c;
				bufor_zapisu[gdzie_zapisuje_serie][wsk_buf_zapisu[gdzie_zapisuje_serie] * 5 + 3] = obrabiany.z;
				bufor_zapisu[gdzie_zapisuje_serie][wsk_buf_zapisu[gdzie_zapisuje_serie] * 5 + 4] = obrabiany.x;
				if (wsk_buf_zapisu[gdzie_zapisuje_serie] == (ILE_REKORDOW_W_BUFORZE - 1))
				{
					tasmy[gdzie_zapisuje_serie].write((char*)& bufor_zapisu[gdzie_zapisuje_serie], sizeof(bufor_zapisu[gdzie_zapisuje_serie]));
					//cout << "\t\tprzepelnienie bufor zapisu na tasmie" << gdzie_zapisuje_serie << endl;
					ilosc_zapisow++;
				}

				wsk_buf_zapisu[gdzie_zapisuje_serie]++;
				wsk_buf_zapisu[gdzie_zapisuje_serie] = wsk_buf_zapisu[gdzie_zapisuje_serie] % ILE_REKORDOW_W_BUFORZE;
				wsk_buf_odczytu++;
				wsk_buf_odczytu = wsk_buf_odczytu%ILE_REKORDOW_W_BUFORZE;

				poprzednie_uporzadkowanie = uporzadkowanie;
				ktory_rekord_odczytuje++;

			}

			for (int i = 0; i < SCHEMAT_WEJ; i++)
			{
			
				if (wsk_buf_zapisu[i] != 0)
				{
					tasmy[i].write((char*)& bufor_zapisu[i], wsk_buf_zapisu[i] * 5*8);
					//cout << "zapisano " << wsk_buf_zapisu[i] << "brakujacych rekordow na tasmie " << i << endl;
					ilosc_zapisow++;
				}
			}
			//cout << endl;
			//cout << "Posortowano naturalne w schemacie " << SCHEMAT_WEJ << " + " << 1 << endl;
			//cout << "podzielono " << licznik << " rekordów" << endl;
			//cout << "Zawartych w " << serie << " seriach" << endl;
			wejsciowy.close();
		}
		else
			cout << "Blad z otwarciem pliku do sortowania";

		for (int i = 0; i < SCHEMAT_WEJ; i++)
		{
			tasmy[i].close();
		}
		//tu powinnam wyswietliæ tasmy
		cout << "\n******************\tTASMA 0:\t**********************" << endl << endl;
		wyswietl_plik_bin("0podzial.bin", WYSWIETLANIE);/////////////////////////////////////////////////////////////////////////////////////
		cout << "\n******************\tTASMA 1:\t**********************" << endl << endl;
		wyswietl_plik_bin("1podzial.bin", WYSWIETLANIE);//////////////////////////////////////////////////////////////////////////////////

		cout << "\n\nETAP SCALANIA:" << endl;
		ile_zostalo_do_odczytu[0] = ile_rekordow_bin("0podzial.bin");
		ile_zostalo_do_odczytu[1] = ile_rekordow_bin("1podzial.bin");
		 wsk_buf_zapisu_scalanie=0;
		 koniec = true;
		scalony.open(plik, ios::binary | ios::out);//zapisujemy tu
		for (int i = 0; i < SCHEMAT_WEJ; i++)
		{
			odczyt_tasm[i].open ((to_string(i) + "podzial.bin"), ios::binary | ios::in);
			/*double wartosc_a;
			odczyt_tasm[i].read((char*)& wartosc_a, sizeof(wartosc_a));
			cout << "!!!!!!!odczytano " << wartosc_a << endl;*/
			wsk_buf_odczytu_scalanie[i] = 0;
		}

		if (odczyt_tasm[0].is_open() && odczyt_tasm[1].is_open())
		{
			poprzednia_wartosc_uporzadkowania = -DBL_MAX;
			//wlasciwy algorytm
			for (int i = 0; i < SCHEMAT_WEJ; i++)
				{
					odczyt_tasm[i].read((char*)& bufor_odczytu_scalanie[i], sizeof(bufor_odczytu_scalanie[i]));
					//cout << "³aduje nowy bufor odczytu z tasmy" << i << endl;
					ilosc_odczytow++;
				}

			while ((ile_zostalo_do_odczytu[0] + ile_zostalo_do_odczytu[1])>0)
			{
				
				for (int i = 0; i < SCHEMAT_WEJ; i++)
				{
					rekord_z_tasmy[i].a = bufor_odczytu_scalanie[i][wsk_buf_odczytu_scalanie[i] * 5 + 0];
					rekord_z_tasmy[i].y = bufor_odczytu_scalanie[i][wsk_buf_odczytu_scalanie[i] * 5 + 1];
					rekord_z_tasmy[i].c = bufor_odczytu_scalanie[i][wsk_buf_odczytu_scalanie[i] * 5 + 2];
					rekord_z_tasmy[i].z = bufor_odczytu_scalanie[i][wsk_buf_odczytu_scalanie[i] * 5 + 3];
					rekord_z_tasmy[i].x = bufor_odczytu_scalanie[i][wsk_buf_odczytu_scalanie[i] * 5 + 4];
					wartosc_uporzadkowania[i] = policz_wartosc_rekord(rekord_z_tasmy[i]);
					//cout << "mam a= " << rekord_z_tasmy[i].a << " o waretosci= " << wartosc_uporzadkowania[i] << endl;
				}



				if(ile_zostalo_do_odczytu[0]>0 && ile_zostalo_do_odczytu[1]>0)
				{
					if (wartosc_uporzadkowania[0] > wartosc_uporzadkowania[1])
					{
						mniejsze = 1;
						wieksze = 0;
					}
					else {
						mniejsze = 0;
						wieksze = 1;
					}

					if (poprzednia_wartosc_uporzadkowania <= wartosc_uporzadkowania[0] &&
						poprzednia_wartosc_uporzadkowania <= wartosc_uporzadkowania[1])//wpisujemy mniejsz
						skad_birzemy_serie = mniejsze;
					else if (poprzednia_wartosc_uporzadkowania >= wartosc_uporzadkowania[0] &&
						poprzednia_wartosc_uporzadkowania >= wartosc_uporzadkowania[1])
						skad_birzemy_serie = mniejsze;
					else
						skad_birzemy_serie = wieksze;
				}
				else if(ile_zostalo_do_odczytu[0]==0)skad_birzemy_serie =1;
				else if (ile_zostalo_do_odczytu[1]== 0)skad_birzemy_serie = 0;
					

				//cout << "\tbierzemy do bufora rekord z taœmy " << skad_birzemy_serie<<endl;
				//cout << "wybieram a =" << rekord_z_tasmy[skad_birzemy_serie].a << endl;
				bufor_zapisu_scalanie[wsk_buf_zapisu_scalanie * 5 + 0] = rekord_z_tasmy[skad_birzemy_serie].a;
				bufor_zapisu_scalanie[wsk_buf_zapisu_scalanie * 5 + 1] = rekord_z_tasmy[skad_birzemy_serie].y;
				bufor_zapisu_scalanie[wsk_buf_zapisu_scalanie * 5 + 2] = rekord_z_tasmy[skad_birzemy_serie].c;
				bufor_zapisu_scalanie[wsk_buf_zapisu_scalanie * 5 + 3] = rekord_z_tasmy[skad_birzemy_serie].z;
				bufor_zapisu_scalanie[wsk_buf_zapisu_scalanie * 5 + 4] = rekord_z_tasmy[skad_birzemy_serie].x;

				if (wsk_buf_zapisu_scalanie == (ILE_REKORDOW_W_BUFORZE - 1))
				{
					scalony.write((char*)& bufor_zapisu_scalanie, sizeof(bufor_zapisu_scalanie));
					//cout << "\t\tprzepelnienie bufor zapisu " << endl;
					ilosc_zapisow++;
				}
				
				wsk_buf_zapisu_scalanie++;
				wsk_buf_zapisu_scalanie= wsk_buf_zapisu_scalanie%ILE_REKORDOW_W_BUFORZE;

				ile_zostalo_do_odczytu[skad_birzemy_serie]--;
				wsk_buf_odczytu_scalanie[skad_birzemy_serie]++;
				wsk_buf_odczytu_scalanie[skad_birzemy_serie] = wsk_buf_odczytu_scalanie[skad_birzemy_serie] % ILE_REKORDOW_W_BUFORZE;
				//cout << wsk_buf_odczytu_scalanie[0] <<"\t" <<wsk_buf_odczytu_scalanie[1] << endl;
				
					if (wsk_buf_odczytu_scalanie[skad_birzemy_serie] == 0)
					{
						odczyt_tasm[skad_birzemy_serie].read((char*)& bufor_odczytu_scalanie[skad_birzemy_serie], sizeof(bufor_odczytu_scalanie[skad_birzemy_serie]));
						//cout << "³aduje nowy bufor odczytu z tasmy" << skad_birzemy_serie << endl;
						ilosc_odczytow++;
					}

					if (poprzednia_wartosc_uporzadkowania > wartosc_uporzadkowania[skad_birzemy_serie])
						koniec = false;
					poprzednia_wartosc_uporzadkowania = wartosc_uporzadkowania[skad_birzemy_serie];



			}//koniec while
			if (wsk_buf_zapisu_scalanie != 0)
			{
				scalony.write((char*)& bufor_zapisu_scalanie, wsk_buf_zapisu_scalanie * 5 * 8);
				//cout << "zapisano " << wsk_buf_zapisu[i] << "brakujacych rekordow na tasmie " << i << endl;
				ilosc_zapisow++;
			}




		}
		else
			cout << "Blad z otwarciem pliku";

		for (int i = 0; i < SCHEMAT_WEJ; i++)
		{
			odczyt_tasm[i].close();
		}
		scalony.close();
		cout << "******************\tPO SCALENIU:\t**********************" << endl << endl;
		wyswietl_plik_bin(plik, WYSWIETLANIE);////////////////////////////////////////////////////////////////////////////////////////////////////


		//koniec = true;
	}//koniec fazy



	cout << "zapisano " << ilosc_zapisow << endl;
	cout << "odczytów " << ilosc_odczytow << endl;

}





int main(){
	srand(0);
	char znak;
	bool petla = true;
	int liczba;
	char nazwa[40];
	while (petla)
	{
		cout << "\n\n\nNaciœnij wybrany prezycisk" << endl;
		cout << "g\t-wygeneruj plik wejsciowy" << endl;
		cout << "p\t wpowadz plik wejsciowy" << endl;
		cout << "w\t-wyjscie z programu" << endl;
		cout << "v\t-wyswietl plik" << endl;
		cout << "Zmien opcje domyslne pliku do sortowania:" << endl;
		cout << "\tn\t-nazwa pliku :\t\t\t" <<plik<< endl;
		cout << "\ti\t-ilosc tasm wejsciowych:\t" << ilosc_tasm_wejsciowych << endl;
		cout << "\tj\t-ilosc tasm wyjsciowych :\t" << ilosc_tasm_wyjsciowych << endl;
		cout << "s\t- SORTUJ PLIK DOMYSLNY" << endl;
		//cout << "d\t- SORTUJ PLIK DOMYSLNY (jedna faza)" << endl;

		cin >> znak;

		switch (znak)
		{
		case 'w':
			petla = false;
			break;

		case 'g':
			cout << "generuje"<<endl;
			cout << "ile rekordow? ";
			cin >> liczba;
			cout << "\npodaj nazwe dla pliku  (np test.bin) : ";
			cin >> nazwa;
			generowanie_pliku(nazwa, liczba);
			break;
		case 'v':
			cout << "podaj nazwe pliku do wyswietlenia" << endl;
			cin>>nazwa;
			//wyswietl_plik(nazwa,true);
			wyswietl_plik_bin(nazwa,true);
			//ile_rekordow_bin();
			//ile_rekordow(nazwa);
			break;
		case 'p':
			cout << "podaj nazwe pliku do utworzenia" << endl;
			cin >> nazwa;
			utworz_plik(nazwa);
			break;
		case 'n':
			cout << "podaj nowa nazwe pliku do sortowania" << endl;
			cin >> plik;
			break;
		case 'i':
			cout << "podaj nowa ilosc taœm wejsciowych" << endl;
			cin >>ilosc_tasm_wyjsciowych;
			break;
		case 'j':
			cout << "podaj nowa ilosc tasm_wyjsciowych" << endl;
			cin >> ilosc_tasm_wyjsciowych;
			break;
		case 's':
			sortowanie_start();
			break;
		default:
			cout << "Nie ma takiego klawisza"<<endl;
			break;
		}

	}

	
return 0;
}