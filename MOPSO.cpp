
/*
 * File:   PSO.cpp
 * Author: werty
 *
 * Created on 27 kwiecień 2011, 23:10
 */

#include "MOPSO.h"
#include <stdlib.h>
#include <QDebug>
//#include <qt4/QtCore/qglobal.h>

MOPSO::MOPSO(QObject *parent) : QThread(parent){
    // parser.DefineVar("x", &this->xVal);
    // parser.DefineVar("y", &this->yVal);
}

MOPSO::MOPSO(int wiel_pop, int wiel_rep, int il_hiperk, bool mutacja, Problem * problem) {
    qsrand(QTime::currentTime().msec());

    // parser.DefineVar("x", &this->xVal);
    // parser.DefineVar("y", &this->yVal);
    this->c=2.0;
    zainicjalizowany=false;
    this->wielk_rep=wiel_rep;
    this->num_iter=0;
    this->z_mutacja = mutacja;
    this -> il_hiperkostek = il_hiperk;
    this -> problem = problem;
    this -> il_funkcji = problem -> funkcje.size();
    this -> il_ograniczen = problem -> ograniczenia.size();
    this -> min_fitness = new float[problem -> funkcje.size()];
    this -> max_fitness = new float[problem -> funkcje.size()];
    this -> dx_fitness = new float[problem -> funkcje.size()];
    this -> il_zmiennych = problem -> zmienne.size();

    for (int i = 0; i < wiel_pop; i++) {
        populacja.push_back(new Particle(il_zmiennych, il_funkcji, il_ograniczen));
    }
}

MOPSO::~MOPSO() {
}

void MOPSO::WyznaczWartFunkcjiKryterialnych() {
    for (int i = 0; i < populacja.size(); i++) {
        for (int x_ind = 0; x_ind < this -> il_zmiennych; x_ind++) {
            problem -> zmienne[x_ind] -> zmienna = populacja[i] -> x[x_ind];
        }

        for (int f_ind = 0; f_ind < this -> il_funkcji; f_ind++) {
            populacja[i] -> wartFunkcjiKryterialnych[f_ind] = problem -> parseryFunkcji[f_ind] -> Eval();
        }
    }
}

/*
 *
 *
 *
 */
void MOPSO::Inicjalizuj() {
   // il_iter = 30;
  //  num_iter = 0;
  //  x_width = 50;
    //xmin = -100;
   // xmax = 100;
   /// wielk_rep = 30;

    qDebug() << "inicjuje...\n";

    for (int i = 0; i < populacja.size(); i++) {
        for (int x_ind = 0; x_ind < this -> il_zmiennych; x_ind++) {
            x_width = problem -> zmienne[x_ind] -> max - problem -> zmienne[x_ind] -> min;
            xmin = problem -> zmienne[x_ind] -> min;
            populacja[i] -> v[x_ind] = 0.0; // (float)rand() / (float)RAND_MAX * vmax - 0.5 * vmax;
            populacja[i] -> x[x_ind] = (float) rand() / (float) RAND_MAX * x_width + xmin;

            // qDebug()<<"x "<<populacja[i]->x[x_ind] <<"\n";
            problem -> zmienne[x_ind] -> zmienna = populacja[i] -> x[x_ind];
        }

        for (int f_ind = 0; f_ind < this -> il_funkcji; f_ind++) {
            populacja[i] -> wartFunkcjiKryterialnych[f_ind] = problem -> parseryFunkcji[f_ind] -> Eval();
            populacja[i] -> fitness_pbest[f_ind] = populacja[i] -> wartFunkcjiKryterialnych[f_ind];
        }

        pbests.push_back(new Particle(*populacja[i]));

        // populacja[i]->v[1] = (float)rand() / (float)RAND_MAX * vmax - 0.5 * vmax;
        // populacja[i]->x[1] = (float)rand() / (float)RAND_MAX * x_width + xmin;
        // qDebug()<<i<<" x1 "<<particles_vec[i]->x_pos[0]<<" x2 "<<particles_vec[i]->x_pos[1]<<"\n";
        // populacja[i]->pbest_pos[0] = populacja[i]->x[0];
        // populacja[i]->pbest_pos[1] = populacja[i]->x[1];
        // xVal = populacja[i]->x[0];
        // yVal = populacja[i]->x[1];
        // populacja[i]->fitness = (double)parser.Eval();
        // populacja[i]->fitness_pbest = populacja[i]->fitness;
    }

    // wyznaczanie niezdominowanych rozwiazan

    WyznaczPrzystosowanie();

    PrzeskalujPrzystosowanie();


    qDebug() << "Sprawdzam zdominowanie\n";
    sprawdz_zdominowanie();

    qDebug() << "Aktualizuje repozytorium\n";
    aktualizuj_repozytorium();

    qDebug() << "Generuje kostki\n";
    generuj_kostki();


    qDebug() << "po Generuje kostki\n";

    for (int i_pop = 0; i_pop < populacja.size(); i_pop++) {
        if (!populacja[i_pop] -> zdominowana) {
            repozytorium.push_back(new Particle(*populacja[i_pop]));
        } else {
        }
    }

     qDebug() << " Generuje kostki\n";

    //    for (int i = 0; i < populacja.size(); i++)
    //    {
    //        qDebug() << "rozw " << i << " (";
    //
    //        for (int j = 0; j < il_funkcji; j++)
    //        {
    //            qDebug() << populacja[i] -> fitness[j] << " ";
    //        }
    //
    //        qDebug() << ")\n";
    //    }
    //
    //    qDebug() << "\n\n";
    //
    //    for (int i = 0; i < repozytorium.size(); i++)
    //    {
    //        qDebug() << "rozw niezdominowane " << i << " (";
    //
    //        for (int j = 0; j < il_funkcji; j++)
    //        {
    //            qDebug() << repozytorium[i] -> fitness[j] << " ";
    //        }
    //
    //        qDebug() << ")\n";
    //    }
    //
    //    qDebug() << "zainicjowalem\n";
}

void MOPSO::Iteruj() {
    TRACE;
    if (z_mutacja) {
        DEBUG("%s\n","Mutuje");

        mutuj(il_iter, num_iter, 0.5);
    }

    DEBUG("%s\n","Aktualizuje predkosci"); 
    aktualizuj_predkosci();

    TRACE;
    //  pso->wyswietl_populacje();
    DEBUG("%s\n","Aktualizuje pozycje");
    aktualizuj_pozycje();
TRACE;
    DEBUG("%s\n","Aktualizuje wartosci");
    WyznaczWartFunkcjiKryterialnych();
    sprawdz_ograniczenia();
TRACE;
    WyznaczPrzystosowanie();

    PrzeskalujPrzystosowanie();
TRACE;
    DEBUG("%s\n","Aktualizuje pbest");
    aktualizuj_pbest();

    DEBUG("%s\n","Sprawdzam zdominowanie");
    sprawdz_zdominowanie();
TRACE;
    DEBUG("%s\n","Aktualizuje repozytorium");
    aktualizuj_repozytorium();

    DEBUG("%s\n","Generuje kostki");
    generuj_kostki();
TRACE;
    if (wielk_rep > 0 && repozytorium.size() > wielk_rep) {
        RedukujRepozytorium();
        generuj_kostki();

    }
   // this->num_iter++;
}

void MOPSO::generuj_kostki() {
    hiperkostki.clear();

    // wyznaczamy min i max wartosci funkcji kryterialnych w naszej populacji
    for (int i = 0; i < il_funkcji; i++) {
        max_fitness[i] = populacja[0] -> wartFunkcjiKryterialnych[i];
        min_fitness[i] = populacja[0] -> wartFunkcjiKryterialnych[i];
    }

    for (int i = 0; i < populacja.size(); i++) {
        for (int j = 0; j < il_funkcji; j++) {
            if (populacja[i] -> wartFunkcjiKryterialnych[j] > max_fitness[j]) {
                max_fitness[j] = populacja[i] -> wartFunkcjiKryterialnych[j];
            }

            if (populacja[i] -> wartFunkcjiKryterialnych[j] < min_fitness[j]) {
                min_fitness[j] = populacja[i] -> wartFunkcjiKryterialnych[j];
            }
        }
    }

    // wyznaczamy szerokosc jednej hiperkostki
    for (int i = 0; i < il_funkcji; i++) {
          qDebug() << "max  " << i << " =" << max_fitness[i];
         qDebug() << "min  " << i << " =" << min_fitness[i];

        dx_fitness[i] = (max_fitness[i] - min_fitness[i]) / il_hiperkostek;

          qDebug() << "dx  " << i << " =" << dx_fitness[i] << "\n";
    }

    int tmp;
    qDebug() << "rep size " << repozytorium.size() << "\n";
    for (int i = 0; i < repozytorium.size(); i++) {
        hiperkostki.push_back(Hiperkostka());
        hiperkostki[i].czasteczki.push_back(i);

        // qDebug() << "rep " << i << " (";
        for (int j = 0; j < il_funkcji; j++) {
            tmp = (int) ((repozytorium[i] -> wartFunkcjiKryterialnych[j] - min_fitness[j]) / dx_fitness[j]);

            // qDebug() << tmp << " ";
            hiperkostki[i].polozenie.push_back(tmp);
        }

        // qDebug() << ")\n";
    }

    qSort(hiperkostki.begin(), hiperkostki.end(), lessThenHiperkostka);

    // qDebug() << "il hip " << hiperkostki.size();
    // qDebug() << "po sortowaniu\n";
    for (int i = 0; i < hiperkostki.size(); i++) {
        // qDebug() << "h " << i << "(";
        for (int j = 0; j < il_funkcji; j++) {
            // qDebug() << hiperkostki[i].polozenie[j];
        }

        // qDebug() << " ind " << hiperkostki[i].czasteczki[0];
        // qDebug() << ")\n";
    }

    // Hiperkostka h1, h2, h3, h4;
    // h1.polozenie << 1 << 3 << 66;
    // h2.polozenie << 1 << 3 << 4;
    // h3.polozenie << 2 << 7 << 4;
    // h4.polozenie << 1 << 5 << 4;
    // QVector<Hiperkostka> hip;
    // hip << h1 << h2 << h3 << h4;
    // qSort(hip.begin(), hip.end(), lessThenHiperkostka);
    //
    // for (int i = 0; i < hip.size(); i++) {
    // qDebug() << "ht " << i << "(";
    // for (int j = 0; j < 3; j++) {
    // qDebug() << hip[i].polozenie[j];
    //
    // }
    //
    // qDebug() << ")\n";
    //
    // }
    //
    // qDebug() << "compare  2 7 4 to  1 3 66 = " << lessThenHiperkostka(h3, h1) << "\n";
    //
    // QVector<Hiperkostka>::iterator it = qBinaryFind(hip.begin(), hip.end(), hip[1], lessThenHiperkostka);
    // qDebug() << "it " << it->polozenie[0] << " " << it->polozenie[1] << "\n";
    // qSort(hip.begin(), hip.end(), qGreater< QVector<int> >());
    // QVector<int> v1,v2;
    // v1<<1<<3<<3;
    // v2<<1<<3<<4;
    joinHiperkostki(hiperkostki); //laczymy powtarzajace sie hiperkostki 


    //usuwam nadmiarowe oso z repozytorium



    for (int i = 0; i < hiperkostki.size(); i++) {
        // qDebug() << "ht " << i << "(pol ";
        for (int j = 0; j < hiperkostki[i].polozenie.size(); j++) {
            // qDebug() << hiperkostki[i].polozenie[j] << " ";
        }

        // qDebug() << " ind: ";
        for (int j = 0; j < hiperkostki[i].czasteczki.size(); j++) {
            // qDebug() << hiperkostki[i].czasteczki[j] << " ";
        }

        // qDebug() << ")\n";
    }

    // qDebug() << "v[1]=" << hip[0].polozenie[1] << hip[1].polozenie[1] << hip[2].polozenie[1];
}

int MOPSO::losujKostkeRuletka() {
    //qDebug() << "hip size " << hiperkostki.size() << "\n";
    float * tab = new float[hiperkostki.size()];
    float maks = 0;
    //DINFO;
    for (int i = 0; i < hiperkostki.size(); i++) {
        // qDebug()<<"size "<<hiperkostki[i].ind_czastek.size()<<"\n";
        tab[i] = maks + 10.0 / (float) hiperkostki[i].czasteczki.size();

        // qDebug()<<"tab i "<<tab[i]<<"\n";
        maks = tab[i];
    }
   // DINFO;
    // qDebug()<<"maks "<<maks<<"\n";
    // qDebug()<<"time_t "<<time.toTime_t()<<"\n";
    float random_float = maks * qrand() / RAND_MAX;
   // DINFO;
    // qDebug()<<"random float "<<random_float<<"\n";
    for (int i = 0; i < hiperkostki.size(); i++) {
       // DINFO;
        if (random_float <= tab[i]) {
            return i;
        }
    }
}

void MOPSO::aktualizuj_predkosci() {
    int h;
    int h_rep;

   // DINFO;
    for (int i = 0; i < populacja.size(); i++) {
       // DINFO;
        // particles_vec[i]->v[0]=czi*(particles_vec[i]->v[0]+
        // c_p*((float)rand() / (float)RAND_MAX * vmax - 0.5 * vmax)*0.1*(particles_vec[i]->pbest_pos[0]-particles_vec[i]->x_pos[0])+
        // c_g*((float)rand() / (float)RAND_MAX * vmax - 0.5 * vmax)*0.1*(gbest_pos[0]-particles_vec[i]->x_pos[0]));
        //
        // particles_vec[i]->v[1]=czi*(particles_vec[i]->v[1]+
        // c_p*((float)rand() / (float)RAND_MAX * vmax - 0.5 * vmax)*0.1*(particles_vec[i]->pbest_pos[1]-particles_vec[i]->x_pos[1])+
        // c_g*((float)rand() / (float)RAND_MAX * vmax - 0.5 * vmax)*0.1*(gbest_pos[1]-particles_vec[i]->x_pos[1]));
        //
        h = losujKostkeRuletka();
        h_rep = (int) ((float) hiperkostki[h].czasteczki.size() * qrand() / RAND_MAX);

       // qDebug() << "h_rep " << h_rep << " z " << hiperkostki[h].czasteczki.size() << "\n";
        w = 1;

        for (unsigned int i_v = 0; i_v < problem -> zmienne.size(); i_v++) {
            qDebug() << " przed populacja[i]->v[i_v] " << populacja[i] -> v[i_v] << "\n";

           // DINFO;
            populacja[i] ->
                    v[i_v] = czi*(w * populacja[i] -> v[i_v]
                    + (c1*(float) qrand() / (float) RAND_MAX) * (pbests[i] -> x[i_v] - populacja[i] -> x[i_v])
                    + (c2*(float) qrand() / (float) RAND_MAX)
                    * (repozytorium[hiperkostki[h].czasteczki[h_rep]] -> x[i_v] - populacja[i] -> x[i_v]));



                        qDebug() << "populacja[i]->v[i_v]" << populacja[i] -> v[i_v] << "\n";
                        qDebug() << "populacja[i]->x[i_v]" << populacja[i] -> x[i_v] << "\n";
                        qDebug() << "repozytorium[hiperkostki[h].czasteczki[h_rep]]-> x[i_v] = "
                                 << repozytorium[hiperkostki[h].czasteczki[h_rep]] -> x[i_v] << "\n";
                        qDebug() << "pbests[i]->x[i_v] " << pbests[i] -> x[i_v] << "\n";
                        qDebug() << "w " << w << "\n";

            if (populacja[i] -> v[i_v] > vmax) {
                populacja[i] -> v[i_v] = vmax;
            }

            if (populacja[i] -> v[i_v] < -vmax) {
                populacja[i] -> v[i_v] = -vmax;
            }

              qDebug() << "populacja[i]->v[i_v] " << populacja[i] -> v[i_v] << "\n";
        }
    }
}

void MOPSO::aktualizuj_pozycje() {
    for (unsigned int i = 0; i < populacja.size(); i++) {
        for (unsigned int i_v = 0; i_v < problem -> zmienne.size(); i_v++) {
            populacja[i] -> x[i_v] = populacja[i] -> x[i_v] + populacja[i] -> v[i_v];

               qDebug() << "x_old " << populacja[i] -> x[i_v] << " v " << populacja[i] -> v[i_v] << "\n";

            if (populacja[i] -> x[i_v] < problem -> zmienne[i_v] -> min) {
                populacja[i] -> x[i_v] = problem -> zmienne[i_v] -> min;
                populacja[i] -> v[i_v] = -populacja[i] -> v[i_v];
            }

            if (populacja[i] -> x[i_v] > problem -> zmienne[i_v] -> max) {
                populacja[i] -> x[i_v] = problem -> zmienne[i_v] -> max;
                populacja[i] -> v[i_v] = -populacja[i] -> v[i_v];
            }
        }
    }
}

void MOPSO::aktualizuj_pbest() {
    for (int i = 0; i < populacja.size(); i++) {
        if (populacja[i] > pbests[i]) // dominuje nowe przystosowanie
        {
            (*pbests[i]) = (*populacja[i]);
        } else {
            if (!(pbests[i] > populacja[i])) // zaden nie dominuje wiec losujemy
            {
                if ((qrand() / RAND_MAX) > 0.5) {
                    (*pbests[i]) = (*populacja[i]);
                }
            }
        }
    }
}
void MOPSO::WyznaczPrzystosowanie()
{
    double skrajnaWartosc;
    for (unsigned int i_fun = 0; i_fun < problem->funkcje.size(); i_fun++) {


        if (problem->tab_minmax[i_fun])//maksymalizacja wiec szukamy minimalnej wartosci
        {

            skrajnaWartosc =populacja[0]->wartFunkcjiKryterialnych[i_fun];
            for (int i_rep = 0; i_rep <populacja.size(); i_rep++) {
                if (populacja[i_rep]->wartFunkcjiKryterialnych[i_fun] < skrajnaWartosc) {
                    skrajnaWartosc =populacja[i_rep]->wartFunkcjiKryterialnych[i_fun];
                }
            }

            for (int i_rep = 0; i_rep <populacja.size(); i_rep++) {
               // populacja[i_rep]->przystosowanie[i_fun] = skrajnaWartosc -populacja[i_rep]->wartFunkcjiKryterialnych[i_fun];
                populacja[i_rep]->przystosowanie[i_fun] = populacja[i_rep]->wartFunkcjiKryterialnych[i_fun]-skrajnaWartosc;

            }


        } else//minimalizacja wiec szukamy maksymalnej wartosci
        {
            skrajnaWartosc =populacja[0]->wartFunkcjiKryterialnych[i_fun];
            for (int i_rep = 0; i_rep <populacja.size(); i_rep++) {
                if (populacja[i_rep]->wartFunkcjiKryterialnych[i_fun] > skrajnaWartosc) {
                    skrajnaWartosc =populacja[i_rep]->wartFunkcjiKryterialnych[i_fun];
                }
            }

            for (int i_rep = 0; i_rep <populacja.size(); i_rep++) {
               // populacja[i_rep]->przystosowanie[i_fun] =populacja[i_rep]->wartFunkcjiKryterialnych[i_fun] - skrajnaWartosc;
                populacja[i_rep]->przystosowanie[i_fun] =skrajnaWartosc-populacja[i_rep]->wartFunkcjiKryterialnych[i_fun]  ;


            }
        }
    }


}
void MOPSO::PrzeskalujPrzystosowanie()
{
    double min, max, srednia, a, b;
    for (unsigned int i_fun = 0; i_fun < problem->funkcje.size(); i_fun++) {
        srednia = 0;
        min =populacja[0]->przystosowanie[i_fun];
        max = min;
        for (int i_rep = 0; i_rep <populacja.size(); i_rep++) {
            if (populacja[i_rep]->przystosowanie[i_fun] < min) {
                min =populacja[i_rep]->przystosowanie[i_fun];
            }
            if (populacja[i_rep]->przystosowanie[i_fun] > max) {
                max =populacja[i_rep]->przystosowanie[i_fun];
            }

            srednia +=populacja[i_rep]->przystosowanie[i_fun];
        }
        //  qDebug() << "suma " << srednia << "\n";
        srednia = srednia / (double)populacja.size();
        //  qDebug() << "srednia " << srednia << "\n";

        if (min > ((c * srednia - max) / (c - 1))) {
            a = srednia * (c - 1) / (max - srednia);
            b = -srednia * (a + 1.0);
        } else {
            a = srednia / (srednia - min);
            b = -a*min;
        }

        // qDebug() << "a " << a << " b " << b << "\n";
        for (int i_rep = 0; i_rep <populacja.size(); i_rep++) {

            populacja[i_rep]->przystosowaniePrzeskalowane[i_fun] =populacja[i_rep]->przystosowanie[i_fun]/max;// a *populacja[i_rep]->przystosowanie[i_fun] + b;
        }



    }


}
void MOPSO::sprawdz_zdominowanie() {
    // wyznaczanie niezdominowanych rozwiazan
    bool zdominowany = false;

    for (int i_pop = 0; i_pop < populacja.size(); i_pop++) {
        zdominowany = false;

        for (int i_pop2 = 0; i_pop2 < populacja.size(); i_pop2++) {
            // qDebug() << "prownuje ------";
            if (problem -> Dominuje((*populacja[i_pop2]), (*populacja[i_pop]))) {
                zdominowany = true;

                // qDebug() << "zdominowany";
                populacja[i_pop] -> zdominowana = true;

                // qDebug() << "117";
                break;
            }

           //  qDebug() << "niezdominowany";
            populacja[i_pop] -> zdominowana = false;

            // qDebug() << "127";
        }
    }
}
/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */
void MOPSO::aktualizuj_repozytorium() {
    bool zdominowany;

    for (int i = 0; i < populacja.size(); i++) {
        if (!populacja[i] -> zdominowana) // niezdominowany wsrod osobnikow aktualnej pop
        {
            // sprawdzamy czy nie jest zdominowany przez roz. z rep.
            zdominowany = false;

            for (int j = 0; j < repozytorium.size(); j++) {
                if (!zdominowany && problem -> Dominuje((*repozytorium[j]), (*populacja[i]))) {
                    // zdominowany wiec przechodzimy do nast. osob. z pop.
                    // continue;
                    zdominowany = true;
                }

                // nowy dominuje czastke z repozytorum wiec usuwamy ja z repozytorium
                if (problem -> Dominuje((*populacja[i]), (*repozytorium[j]))) {
                    repozytorium.remove(j);

                    j--;
                }
            }

            if (!zdominowany) {
                // niezdominowany wiec dodajemy do repozytorium
                repozytorium.push_back(new Particle(*populacja[i]));
            }
        }
    }
}

void MOPSO::wyswietl_repozytorium() {
    for (int i = 0; i < repozytorium.size(); i++) {
        qDebug() << "rep[" << i << "]:";

        for (int j = 0; j < il_zmiennych; j++) {
            qDebug() << "\tx:" << repozytorium[i] -> x[j] << " ";
        }

        for (int j = 0; j < il_zmiennych; j++) {
            qDebug() << "\tv:" << repozytorium[i] -> v[j] << " ";
        }

        for (int j = 0; j < il_funkcji; j++) {
            qDebug() << "\tf:" << repozytorium[i] ->wartFunkcjiKryterialnych[j] << " ";
        }
        qDebug() << "\n";
    }
}

void MOPSO::wyswietl_populacje() {
    for (int i = 0; i < populacja.size(); i++) {
        qDebug() << "pop[" << i << "]:";

        for (int j = 0; j < il_zmiennych; j++) {
            qDebug() << "x:" << populacja[i] -> x[j] << " ";
        }

        for (int j = 0; j < il_zmiennych; j++) {
            qDebug() << "v:" << populacja[i] -> v[j] << " ";
        }

        qDebug() << "\n";
    }
}

void MOPSO::sprawdz_ograniczenia() {
    double val_left, val_right;

    for (unsigned int ind_pop = 0; ind_pop < populacja.size(); ind_pop++) {
        for (unsigned int ind_ogr = 0; ind_ogr < problem -> ograniczenia.size(); ind_ogr++) {
            populacja[ind_pop] -> il_przek_ograniczen = 0;

            for (int x_ind = 0; x_ind < this -> il_zmiennych; x_ind++) {
                problem -> zmienne[x_ind] -> zmienna = populacja[ind_pop] -> x[x_ind];
            }

            val_left = problem -> ograniczenia[ind_ogr] -> lewa_funkcja -> Eval();
            val_right = problem -> ograniczenia[ind_ogr] -> prawa_funkcja -> Eval();

            switch (problem -> ograniczenia[ind_ogr] -> rodzaj) { // sprawdzamy czy ograniczenie nie zostalo przekroczone
                case 0: // <
                    if (val_left >= val_right) {
                        populacja[ind_pop] -> il_przek_ograniczen++;
                    }

                    break;

                case 1: // <=
                    if (val_left > val_right) {
                        populacja[ind_pop] -> il_przek_ograniczen++;
                    }

                    break;

                case 2: // =
                    if (val_left != val_right) {
                        populacja[ind_pop] -> il_przek_ograniczen++;
                    }

                    break;

                case 3: // >=
                    if (val_left < val_right) {
                        populacja[ind_pop] -> il_przek_ograniczen++;
                    }

                    break;

                case 4: // >
                    if (val_left <= val_right) {
                        populacja[ind_pop] -> il_przek_ograniczen++;
                    }

                    break;
            }
        }
    }
}

void MOPSO::mutuj(int gen, int il_gen, double wsp_mutacji) {
    double wsp = pow((1.0 - (double) gen / (double) il_gen), 5.0 / wsp_mutacji);
    if (wsp <= 0) {
        return;

    }

    int wyb_wym;
    double zakres_mutacji;
    double min, max;

    for (int i_pop = 0; i_pop < populacja.size(); i_pop++) {


        wyb_wym = losuj(0, problem->zmienne.size() - 1);
        zakres_mutacji = (problem->zmienne[wyb_wym]->max - problem->zmienne[wyb_wym]->min) * wsp;
        max = populacja[i_pop]->x[wyb_wym] + zakres_mutacji;
        min = populacja[i_pop]->x[wyb_wym] - zakres_mutacji;

        if (min < problem->zmienne[wyb_wym]->min) {
            min = problem->zmienne[wyb_wym]->min;

        }

        if (max > problem->zmienne[wyb_wym]->max) {
            max = problem->zmienne[wyb_wym]->max;

        }


        populacja[i_pop]->x[wyb_wym] = losuj(min, max);
    }





}

void MOPSO::RedukujRepozytorium() {


    wek_usun.clear();

    float maks = 0;
    int licz_czast = 0;
    for (int i_hip = 0; i_hip < hiperkostki.size(); i_hip++) {
        maks += (float) hiperkostki[i_hip].czasteczki.size()*(float) hiperkostki[i_hip].czasteczki.size();
        for (int i_czast = 0; i_czast < hiperkostki[i_hip].czasteczki.size(); i_czast++) {
            repozytorium[hiperkostki[i_hip].czasteczki[i_czast]]->licz_kostki = hiperkostki[i_hip].czasteczki.size();
            licz_czast++;
        }
    }


   /// qDebug() << "rep size " << repozytorium.size() << " licz_czast " << licz_czast << "\n";


    float wsp = (float) wielk_rep / (float) hiperkostki.size() - 1.0;
   // qDebug() << "wielk rep " << wielk_rep << "\n";
   // qDebug() << "repozytorium.size() = " << repozytorium.size() << " hiperkostki.size()= " << hiperkostki.size() << "\n";
   // qDebug() << "wsp =" << wsp << "\n";
    int il_usun = repozytorium.size() - wielk_rep;
   // qDebug() << "usuwam " << il_usun << "\n";
    float tmp, los;

    bool ok;
    int wybor = 0;
    while (wek_usun.size() != il_usun) {
        los = losuj(0.0, maks);
        tmp = 0;
        for (int i_czast = 0; i_czast < repozytorium.size(); i_czast++) {
            tmp += (float) repozytorium[i_czast]->licz_kostki;
            if (los < tmp) {
                wybor = i_czast;
                break;
            }
        }
        ok = true;
        for (int i = 0; i < wek_usun.size(); i++) {
            if (wybor == wek_usun[i]) {
                ok = false;
                break;
            }

        }

        if (ok) {
            wek_usun.push_back(wybor);
            // hiperkostki[tmp].czasteczki.remove(los);
            // licz++;
            // qDebug()<<"il "<<licz<<"\n";

        }
    }


    //qDebug() << "wek usun \n";
    qSort(wek_usun.begin(), wek_usun.end(), qGreater<int>());
    for (int i = 0; i < wek_usun.size(); i++) {
    //    qDebug() << i << " = " << wek_usun[i] << "\n";
        repozytorium.remove(wek_usun[i]);


    }

  //  qDebug() << "rep size " << repozytorium.size() << "\n";



}

void MOPSO::run() {
    qWarning("odpalam do the work");
    doTheWork();
}

void MOPSO::doTheWork() {
   // qWarning("doTheWork !!!\n");
    num_iter = 0;
    stopIteration=false;
    for (int i = 0; i < il_iter; i++) {
       QMutexLocker locker(&mutex);
        if(stopIteration)break;
       // qDebug()<<"--"<<i<<"\n";
       // qWarning("i  %i\n",i);
        Iteruj();
        num_iter++;
        emit done();
    }
    emit finished();
}

void MOPSO::stop()
{
    QMutexLocker locker(&mutex);
    stopIteration=true;
   
}
