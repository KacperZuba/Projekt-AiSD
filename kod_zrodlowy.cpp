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

template <typename T>
struct WynikBenchmarku {
    long long czas_ms;
    T wartosc;
};

template <typename F, typename... Args>
auto benchmark(F&& funkcja, Args&&... args)
-> WynikBenchmarku<typename std::result_of<F(Args...)>::type>
{
    using namespace std::chrono;
    using ReturnType = typename std::result_of<F(Args...)>::type;

    auto start = high_resolution_clock::now();

    ReturnType wynik =
        std::forward<F>(funkcja)(std::forward<Args>(args)...);

    auto stop = high_resolution_clock::now();

    return {
        duration_cast<milliseconds>(stop - start).count(),
        std::move(wynik)
    };
}

vector<pair<int, int>> algorytmBruteForce(vector<int> tab) {
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
	
	return pary_czynnikow;
}

vector<pair<int, int>> algorytmOptymalny(vector<int> &tab) {
    if(tab.size() == 2) {
        return vector<pair<int, int>>{
			{tab.at(0), tab.at(1)}
		};
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

    if(iloczyn_dodatnie > iloczyn_ujemne) return vector<pair<int, int>>{ para_dodatnia };
    else if(iloczyn_ujemne > iloczyn_dodatnie) return vector<pair<int, int>>{ para_ujemna };
    else return vector<pair<int, int>> {para_dodatnia, para_ujemna};
}


int main() {
	srand(time(NULL)); // inicjalizacja ziarna generatora liczb pseudolosowych
	
	int n = 20;
	
	vector<int> tab(n);
	wypelnijTabliceLosowymiLiczbamiZPrzedzialu(tab.data(), n, -10, 10);
	
	if (tab.size() < 2) {
		cerr << "Tablica musi miec co najmniej 2 elementy\n";
		return 1;
	}
	
	cout<<"Wygenerowana tablica:"<<endl;
	wypiszTablice(tab.data(), n);
	cout<<endl;
	
	cout<<"Algorytm optymalny:"<<endl;
	auto wynik_benchmarku_optymalnego = benchmark(algorytmOptymalny, tab);
	cout<<"Czas wykonania: "<< wynik_benchmarku_optymalnego.czas_ms<<" ms."<<endl;
	cout<<"------"<<endl;
	
	cout<<"Algorytm bruteforce:"<<endl;
	auto wynik_benchmarku_brute_force = benchmark(algorytmBruteForce, tab);
	cout<<"Czas wykonania: "<< wynik_benchmarku_brute_force.czas_ms<<" ms."<<endl;
	cout<<"------"<<endl;
	
	vector<pair<int, int>> pary_czynnikow = wynik_benchmarku_brute_force.wartosc;
	int maks_iloczyn = pary_czynnikow.at(0).first * pary_czynnikow.at(0).second;
	
	cout<<"Maksymalny iloczyn: "<<maks_iloczyn<<endl;
	cout<<"Pary czynnikow: ";
	for(int i = 0; i < pary_czynnikow.size(); i++) {
		pair<int, int> para = pary_czynnikow.at(i);
		cout<<"("<<para.first<<", "<<para.second<<") ";
	}
	                        
	return 0;
}

