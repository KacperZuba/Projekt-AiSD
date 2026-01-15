#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>
#include <vector>
#include <chrono>

using namespace std;

bool czyParyZawierajaTeSameElementy (const pair<int,int>& a, const pair<int,int>& b) {
	return (a.first == b.first && a.second == b.second) || (a.first == b.second && a.second == b.first);
}

template <typename T>
void wypelnijTabliceLosowymiLiczbamiZPrzedzialu(
	T* tab, int dlugosc_tablicy,
	T dolna_granica, T gorna_granica
	) {
		for(int i = 0; i < dlugosc_tablicy; i++) {
			tab[i] = dolna_granica + (rand() % (gorna_granica - dolna_granica + 1));
		}
	}
	
template <typename T>
void wypiszTablice(T* tablica, int dlugosc_tablicy) {
	cout<<'[';
	for(int i = 0; i < dlugosc_tablicy; i++) {
		cout<<tablica[i];
		if(i < (dlugosc_tablicy - 1)) {
			cout<<',';
		}
	}
	cout<<']';
}

template <typename F, typename... Args>
long long zmierz_czas_wykonania(F&& funkcja, Args&&... args){
    auto start = chrono::high_resolution_clock::now();

    std::forward<F>(funkcja)(std::forward<Args>(args)...);

    auto stop = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::milliseconds>(stop - start).count();
}

void algorytmBruteForce(vector<int> tab) {
	int dlugosc_tablicy = tab.size(), iloczyn_maks = tab[0] * tab[1];
	vector<pair<int, int>> pary_czynnikow;
	
	for (int i = 0; i < dlugosc_tablicy - 1; ++i) {
	    for (int j = i + 1; j < dlugosc_tablicy; ++j) {
	        int iloczyn = tab[i] * tab[j];
	
	        if (iloczyn > iloczyn_maks) {
	            iloczyn_maks = iloczyn;
	            pary_czynnikow.clear();
	            pary_czynnikow.push_back({tab[i], tab[j]});
	        } else if (iloczyn == iloczyn_maks) {
	            pair<int,int> nowa_para = {tab[i], tab[j]};
	
	            bool czy_para_zostala_juz_zawarta = false;
	            for (const auto& p : pary_czynnikow) {
	                 if (czyParyZawierajaTeSameElementy(p, nowa_para)) {
	                    czy_para_zostala_juz_zawarta = true;
	                    break;
	                }
	            }
	
	            if (!czy_para_zostala_juz_zawarta) {
	                pary_czynnikow.push_back(nowa_para);
	            }
	        }
	    }
	 }
	
	cout << "Maksymalny iloczyn: " << iloczyn_maks << "\n";
	cout << "Pary czynnikow:\n";
	
	for (const auto &p : pary_czynnikow) {
		cout<<"["<<p.first<<", "<< p.second << "]\n";
	}
}

void algorytmOptymalny(vector<int> &tab) {
    if(tab.size() == 2) {
        cout<<"Maksymalny iloczyn: "<<tab[0] * tab[1]<<endl;
        cout<<"["<<tab[0]<<", "<<tab[1]<<"]";
        return;
    }

    pair<int,int> para_dodatnia = {INT_MIN, INT_MIN}; 
    pair<int,int> para_ujemna   = {INT_MAX, INT_MAX};

    for (int x : tab) {
    	// na potrzeby działania algorytmu traktujemy 0 jako liczbe dodatnią
        if (x >= 0) {
            if (x > para_dodatnia.first) {
                para_dodatnia.second = para_dodatnia.first;
                para_dodatnia.first = x;
            } else if (x > para_dodatnia.second) {
                para_dodatnia.second = x;
            }
        }

        if (x < 0) {
            if (x < para_ujemna.first) {
                para_ujemna.second = para_ujemna.first;
                para_ujemna.first = x;
            } else if (x < para_ujemna.second) {
                para_ujemna.second = x;
            }
        }
    }

    int iloczyn_dodatnie = (para_dodatnia.second != INT_MIN
                            ? para_dodatnia.first * para_dodatnia.second
                            : INT_MIN);

    int iloczyn_ujemne = (para_ujemna.second != INT_MAX
                          ? para_ujemna.first * para_ujemna.second
                          : INT_MIN);

    cout<<"Maksymalny iloczyn: "<<(iloczyn_dodatnie > iloczyn_ujemne
        ? iloczyn_dodatnie
        : iloczyn_ujemne)<<endl;

    cout << "Pary czynnikow:\n";
    if(iloczyn_dodatnie >= iloczyn_ujemne) {
        cout<<"["<<para_dodatnia.first<<", "<< para_dodatnia.second << "]\n";
    }
    if(iloczyn_ujemne >= iloczyn_dodatnie) {
        cout<<"["<<para_ujemna.first<<", "<< para_ujemna.second << "]\n";
    }
}


int main() {
	srand(time(NULL)); // inicjalizacja ziarna generatora liczb pseudolosowych
	
	int n = 10;
	
	vector<int> tab(n);
	wypelnijTabliceLosowymiLiczbamiZPrzedzialu(tab.data(), n, -10, 10);
	
	if (tab.size() < 2) {
		cerr << "Tablica musi miec co najmniej 2 elementy\n";
		return 1;
	}
	
	cout<<"Wygenerowana tablica: ";
	wypiszTablice(tab.data(), n);
	cout<<endl<<endl;
	
	cout<<"Algorytm optymalny:"<<endl;
	long long czas_wykonania_optymalny = zmierz_czas_wykonania(algorytmOptymalny, tab);
	cout<<"Czas wykonania: "<< czas_wykonania_optymalny<<" ms."<<endl;
	cout<<"------"<<endl;
	
	cout<<"Algorytm bruteforce:"<<endl;
	long long czas_wykonania_brute_force = zmierz_czas_wykonania(algorytmBruteForce, tab);
	cout<<"Czas wykonania: "<< czas_wykonania_brute_force<<" ms."<<endl;
	cout<<"------";
	                        
	return 0;
}

