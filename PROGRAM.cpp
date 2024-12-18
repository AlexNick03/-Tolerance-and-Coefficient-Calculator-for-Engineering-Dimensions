#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <limits>

int dimensiune;

int main() {

    // Declarațiile variabilelor
     do {
         std::cout<<"\nIntroduceti dimensiunea  0 daca vreti sa opriti programul "<<std::endl;
    std::cout << "\nIntroduceti o dimensiune:  ";
    std::cin >> dimensiune;
    if (dimensiune == 0 ) break;
    while (dimensiune >3150 || dimensiune<0)  {
        std::cout << "\nIntroduceti dimensiuni mai mici decat 3150, dar mai mari ca 0" << std::endl;
        std::cout << "\nReintroduceti dimensiunea:  ";
        std::cin >> dimensiune;

    }
         if (dimensiune == 0 ) break;
    double toleranta;
    double abaterea_superiaora;
    double abaterea_inferioara;
    double D1;
    double D2;
    double D;
    double dim_rotunjita;

    std::cout << "\nIntroduceti abaterea inferiaora : ";
    std::cin >> abaterea_inferioara;
    std::cout << "\nIntroduceti abaterea superiaora : ";
    std::cin >> abaterea_superiaora;
    std::cout<<std::endl;
    toleranta = abaterea_superiaora - abaterea_inferioara;
    std::cout << "Toleranta este  " << toleranta<<std::endl;


    // Deschideți fișierul CSV pentru a citi prima linie
    std::ifstream file("tabelIT.csv");

    if (!file.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului!" << std::endl;

        return 1;
    }

    // Ignorați prima linie cu titlurile coloanelor
    std::string line;
    std::getline(file, line);

    // Citirea valorilor de pe prima linie (fără primele două coloane)
    std::istringstream first_line_stream(line);
    std::vector<double> scara_it;
    for (int i = 0; i < 2; ++i) {
        std::getline(first_line_stream, line, ';'); // Ignora primele doua coloane
    }
    while (std::getline(first_line_stream, line, ';')) {
        double valoare;
        std::istringstream val_line(line);
        val_line >> valoare;
        scara_it.push_back(valoare);
    }

    // Închideți fișierul după ce ați citit prima linie
    file.close();

    // Deschideți fișierul CSV pentru a continua cu restul codului
    file.open("tabelIT.csv");

    if (!file.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului!" << std::endl;
        return 1;
    }

    // Ignorați prima linie cu titlurile coloanelor
    std::getline(file, line);

    // Citirea valorilor din fișier
    std::vector<double> val_lim_sup;
    std::vector<std::vector<double>> vector_valori;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double dim_sup;
        iss >> dim_sup;

        val_lim_sup.push_back(dim_sup);

        // Citirea vectorului de valori de pe acea linie (fără a citi din nou dim_sup)
        std::vector<double> v_valori;
        double valoare;

        // Sărim primele două coloane
        for (int i = 0; i < 2; ++i) {
            std::getline(iss, line, ';');
        }

        // Citirea valorilor începând cu a treia coloană
        while (std::getline(iss, line, ';')) {
            std::istringstream val_line(line);
            val_line >> valoare;
            v_valori.push_back(valoare);
        }

        vector_valori.push_back(v_valori);
    }

    // Închideți fișierul după ce ați citit valorile pentru a determina dim_rotunjita
    file.close();

    // Rotunjirea dimensiunii la următoarea valoare din val_lim_sup
    auto it = std::lower_bound(val_lim_sup.begin(), val_lim_sup.end(), dimensiune);
    if (dimensiune > 2500 && dimensiune <= 3150) {
        dim_rotunjita = 3150.0;
        D1 = 2500.0;
        D2 = dim_rotunjita;
    } else {
        // Determinarea dim_rotunjita
        dim_rotunjita = (it != val_lim_sup.end()) ? *it : 0.0;
        auto index_dim_rotunjita = std::distance(val_lim_sup.begin(), it);
        if (dim_rotunjita == val_lim_sup[0]) {
            D2 = dim_rotunjita;
            D1 = 0;
        } else if (dim_rotunjita != val_lim_sup[0]) {
            D2 = dim_rotunjita;
            D1 = val_lim_sup[index_dim_rotunjita - 1];
        }
    }
    // Deschiderea fișierului pentru a căuta valorile corespunzătoare coloanei a doua
    file.open("tabelIT.csv");

    if (!file.is_open()) {
        std::cerr << "Eroare la deschiderea fisierului!" << std::endl;
        return 1;
    }

    // Ignorați prima linie cu titlurile coloanelor
    std::getline(file, line);

    // Găsirea indexului corespunzător valorii rotunjite
    auto index = std::distance(val_lim_sup.begin(), it);

    // Resetează cursorul la începutul fișierului
    file.clear();
    file.seekg(0);

    // Sărim peste liniile până la linia cu indexul corespunzător
    for (size_t i = 0; i <= index; ++i) {
        std::getline(file, line);
    }

    // Citim linia anterioară (linia corespunzătoare dim_rotunjita - 1)
    std::getline(file, line);


    std::cout << std::endl;



    // Calcularea diferenței minime și actualizarea tol_dif
    double tol_dif = std::numeric_limits<double>::infinity();

    int index_valoare_rotunjita = 0;

    for (int i = 0; i < vector_valori[index - 1].size(); ++i) {
        double valoare = vector_valori[index - 1][i];
        double diferenta = std::abs(valoare - toleranta);

        if (diferenta < tol_dif) {
            tol_dif = diferenta;

            index_valoare_rotunjita = i;
        }
    }



    // Obțineți elementul corespunzător din scara_it
    double treapta_de_toleranta = scara_it[index_valoare_rotunjita];
    std::cout << "Se cauta  in baza de date treapta it in functie de dimensiunea si abaterile date ";
    // Afișarea valorii treapta_de_toleranta
    std::cout << "\nTreapta de toleranta corespunzatoare este IT-" << treapta_de_toleranta << std::endl;

    // Închideți fișierul după ce ați terminat de utilizat
    file.close();
    if (treapta_de_toleranta < 5) {
        std::cout << "Nu se calculeaza pentru trepte de toleranta mai mici decat treapta IT-5";
    } else {
        std::cout << "\nDimensiunea inferioara D1= " << D1;
        std::cout << "\nDimensiunea superioara D2= " << D2;
        D = sqrt(D1 * D2);
        std::cout << "\nD= " << D;
        std::cout << std::endl;


        // Deschide fișierul kn.csv pentru a extrage valorile într-un vector
        std::ifstream kn_file("kn.csv");

        // Verificare dacă fișierul s-a deschis cu succes
        if (!kn_file.is_open()) {
            std::cerr << "Eroare la deschiderea fisierului kn.csv!" << std::endl;
            return 1;
        }

        // Citirea primei linii din kn.csv (val_it)
        std::string val_it_line;
        if (!std::getline(kn_file, val_it_line)) {
            std::cerr << "Eroare la citirea primei linii din fisierul kn.csv!" << std::endl;
            kn_file.close();
            return 1;
        }

        // Citirea celei de-a doua linii din kn.csv (val_kn)
        std::string val_kn_line;
        if (!std::getline(kn_file, val_kn_line)) {
            std::cerr << "Eroare la citirea celei de-a doua linii din fisierul kn.csv!" << std::endl;
            kn_file.close();
            return 1;
        }

        // Închide fișierul după ce am terminat de utilizat
        kn_file.close();




        // Căutarea treptei de toleranță în val_it și returnarea valorii corespunzătoare din val_kn

        std::cout << "Se cauta coeficientul kn in fucntie de treapta de  toleranta a dimensiunii tolerate" << "\n";
        std::istringstream val_it_stream(val_it_line);
        std::istringstream val_kn_stream(val_kn_line);

        double val_it, val_kn;

        while (std::getline(val_it_stream, val_it_line, ';') && std::getline(val_kn_stream, val_kn_line, ';')) {
            std::istringstream(val_it_line) >> val_it;
            std::istringstream(val_kn_line) >> val_kn;

            if (val_it == treapta_de_toleranta) {
                std::cout << "Coeficientul de precizie Kn= " << val_kn << std::endl;
                break;
            }
        }
        double tn;
        double fac_i;
        fac_i = (0.45*pow(D,(1.0/3)) + 0.001*D);
        tn = val_kn * fac_i;
        std::cout<<"Factorul de toleranta i= "<<fac_i<<std::endl;
        std::cout<<"Toleranta fundamentala Tn= " << tn <<std::endl;

    }

     }
     while (dimensiune<=3150);

}
