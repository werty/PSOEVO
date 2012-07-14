/* 
 * File:   NSGA_II.cpp
 * Author: marcin
 * 
 * Implementacja algorytmu opisanego w artykule pt. 
 * "A Fast and Elitist Multiobjective Genetic Algorithm: NSGA-II"
 * 
 * Created on 8 wrzesień 2011, 03:24
 */

#include "NSGA_II.h"
#include "uzyteczne_funkcje.h"

NSGA_II::NSGA_II(QObject *parent)
: QThread(parent) {
    //moveToThread(this);
}

NSGA_II::NSGA_II(int wielk_pop, int il_bitow, double prawd_mutacji, double prawd_krzyz, Problem * problem) {

    this->num_iter = 0;
    this->F = NULL;
    this->c = 2.0;
    this->wiel_pop = wielk_pop;
    this->il_bitow = il_bitow;
    this->prawd_mutacji = prawd_mutacji;
    this->prawd_krzyz = prawd_krzyz;
    this->problem = problem;

    for (int i = 0; i < wiel_pop; i++) {
        rodzice.push_back(new Solution(problem->zmienne.size(), il_bitow, problem->parseryFunkcji.size()));
        rodzenstwo.push_back(new Solution(problem->zmienne.size(), il_bitow, problem->parseryFunkcji.size()));
    }

    for (int i = 0; i < 2 * wiel_pop; i++) {
        pokolenie.push_back(new Solution(problem->zmienne.size(), il_bitow, problem->parseryFunkcji.size()));


    }




    this->zakres = pow(2.0,il_bitow);
}

NSGA_II::~NSGA_II() {
    qDebug() << "NSGA__II destructor\n";

    for (int i = 0; i < rodzice.size(); i++) {
        delete rodzice[i];


    }

    rodzice.clear();
    DINFO;
    for (int i = 0; i < rodzice.size(); i++) {
        delete rodzenstwo[i];


    }
    rodzenstwo.clear();
    DINFO;

    for (int i = 0; i < rodzice.size(); i++) {
        delete pokolenie[i];


    }
    pokolenie.clear();

    DINFO;
    if (F != NULL) {
        qDebug() << "size " << (*F).size() << "\n";
        for (int i = 0; i < (*F).size(); i++) {
            //DINFO;
            qDebug() << "size " << (*F)[i].size() << "\n";
            if ((*F)[i].size() > 0) {
                (*F)[i].clear();
            }


        }
        // DINFO;
        F->clear();
    }

    wait();
}

void NSGA_II::DekodujGenotyp() {
    for (int i_pop = 0; i_pop <pokolenie.size(); i_pop++) {
        for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
//            double wartosc = 0;
//            for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
//                if (pokolenie[i_pop]->genotyp[i_zmienna][i_bit]) {
//                    wartosc += tab_poteg[i_bit];
//                }
//            }
            // qDebug() << "min " << problem->zmienne[i_zmienna]->min << " max " << problem->zmienne[i_zmienna]->max << " zakres " << zakres << " wartosc " << wartosc << "\n";
           pokolenie[i_pop]->fenotyp[i_zmienna] = problem->zmienne[i_zmienna]->min + (problem->zmienne[i_zmienna]->max - problem->zmienne[i_zmienna]->min) / (zakres - 1) * pokolenie[i_pop]->genotyp[i_zmienna].GetValue();
        }
    }
}

void NSGA_II::WyznaczWartFunkcjiKryterialnych() {
    for (int i = 0; i <pokolenie.size(); i++) {
        for (unsigned int x_ind = 0; x_ind < problem->zmienne.size(); x_ind++) {
            problem -> zmienne[x_ind] -> zmienna =pokolenie[i] ->fenotyp[x_ind];
        }

        for (unsigned int f_ind = 0; f_ind < problem->parseryFunkcji.size(); f_ind++) {
           pokolenie[i] -> wartFunkcjiKryterialnych[f_ind] = problem -> parseryFunkcji[f_ind] -> Eval();
        }
    }
}

void NSGA_II::WyznaczPrzystosowanie() {
    double skrajnaWartosc;
    for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {


        if (problem->tab_minmax[i_fun])//maksymalizacja wiec szukamy minimalnej wartosci
        {

            skrajnaWartosc =pokolenie[0]->wartFunkcjiKryterialnych[i_fun];
            for (int i_rep = 0; i_rep <pokolenie.size(); i_rep++) {
                if (pokolenie[i_rep]->wartFunkcjiKryterialnych[i_fun] < skrajnaWartosc) {
                    skrajnaWartosc =pokolenie[i_rep]->wartFunkcjiKryterialnych[i_fun];
                }
            }

            for (int i_rep = 0; i_rep <pokolenie.size(); i_rep++) {
               pokolenie[i_rep]->przystosowanie[i_fun] = skrajnaWartosc -pokolenie[i_rep]->wartFunkcjiKryterialnych[i_fun];
            }


        } else//minimalizacja wiec szukamy maksymalnej wartosci
        {
            skrajnaWartosc =pokolenie[0]->wartFunkcjiKryterialnych[i_fun];
            for (int i_rep = 0; i_rep <pokolenie.size(); i_rep++) {
                if (pokolenie[i_rep]->wartFunkcjiKryterialnych[i_fun] > skrajnaWartosc) {
                    skrajnaWartosc =pokolenie[i_rep]->wartFunkcjiKryterialnych[i_fun];
                }
            }

            for (int i_rep = 0; i_rep <pokolenie.size(); i_rep++) {
               pokolenie[i_rep]->przystosowanie[i_fun] =pokolenie[i_rep]->wartFunkcjiKryterialnych[i_fun] - skrajnaWartosc;
            }
        }
    }




}

void NSGA_II::PrzeskalujPrzystosowanie() {
    double min, max, srednia, a, b;
    for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {
        srednia = 0;
        min =pokolenie[0]->przystosowanie[i_fun];
        max = min;
        for (int i_rep = 0; i_rep <pokolenie.size(); i_rep++) {
            if (pokolenie[i_rep]->przystosowanie[i_fun] < min) {
                min =pokolenie[i_rep]->przystosowanie[i_fun];
            }
            if (pokolenie[i_rep]->przystosowanie[i_fun] > max) {
                max =pokolenie[i_rep]->przystosowanie[i_fun];
            }

            srednia +=pokolenie[i_rep]->przystosowanie[i_fun];
        }
        //  qDebug() << "suma " << srednia << "\n";
        srednia = srednia / (double)pokolenie.size();
        //  qDebug() << "srednia " << srednia << "\n";

        if (min > ((c * srednia - max) / (c - 1))) {
            a = srednia * (c - 1) / (max - srednia);
            b = -srednia * (a + 1.0);
        } else {
            a = srednia / (srednia - min);
            b = -a*min;
        }

        // qDebug() << "a " << a << " b " << b << "\n";
        for (int i_rep = 0; i_rep <pokolenie.size(); i_rep++) {

           pokolenie[i_rep]->przystosowaniePrzeskalowane[i_fun] = a *pokolenie[i_rep]->przystosowanie[i_fun] + b;
        }



    }



}

void NSGA_II::Inicjalizuj() {
    qsrand(QTime::currentTime().msec());
    qDebug() << "P.size() " <<rodzice.size() << "\n";
    for (int i_pop = 0; i_pop <rodzice.size(); i_pop++) {
        // DINFO;
        for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
            //  DINFO;
            for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
                // DINFO;
                // qDebug()<<"i_pop "<<i_pop<<" i_zmienna "<<i_zmienna<<" i_bit"<<i_bit<<"\n";
                if (((float) qrand() / (float) RAND_MAX) > 0.5) {
                    //   DINFO;
                   rodzice[i_pop]->genotyp[i_zmienna][i_bit] = true;
                } else {
                    // DINFO;
                   rodzice[i_pop]->genotyp[i_zmienna][i_bit] = false;
                }
                //  DINFO;
            }
        }
    }

    // DINFO;
}

void NSGA_II::WyswietlFenotyp() {
    qDebug() << "Fenotyp" << "\n";
    for (int i = 0; i <pokolenie.size(); i++) {
        qDebug() << i << " : ";
        for (unsigned int j = 0; j < problem->parseryFunkcji.size(); j++) {
            qDebug() <<pokolenie[i]->fenotyp[j] << " ";
        }
        qDebug() << "\n";
    }
}

void NSGA_II::WyswietlPrzystosowaniePrzeskalowane(QVector<Solution*>& P) {
    qDebug() << "PrzystosowaniePrzeskalowane" << "\n";
    for (int i = 0; i <P.size(); i++) {
        qDebug() << i << " : ";
        for (unsigned int j = 0; j < problem->parseryFunkcji.size(); j++) {
            qDebug() <<P[i]->przystosowaniePrzeskalowane[j] << " ";
        }
        qDebug() << "\n";
    }
}

void NSGA_II::WyswietlPrzystosowanie() {
    qDebug() << "Przystosowanie" << "\n";
    for (int i = 0; i <pokolenie.size(); i++) {
        qDebug() << i << " : ";
        for (unsigned int j = 0; j < problem->parseryFunkcji.size(); j++) {
            qDebug() <<pokolenie[i]->przystosowanie[j] << " ";
        }
        qDebug() << "\n";
    }

}

void NSGA_II::WyswietlRangiZatloczenia() {
    qDebug() << "Rangi i zatloczenia" << "\n";
    for (int i = 0; i <pokolenie.size(); i++) {
        qDebug() << i << " : ";

        qDebug() <<pokolenie[i]->ranga << " " <<pokolenie[i]->zatloczenie << " Fs " <<pokolenie[i]->przystosowaniePrzeskalowane[0] << " " <<pokolenie[i]->przystosowaniePrzeskalowane[1];

        qDebug() << "\n";
    }


}

QVector< QVector<Solution*> > * NSGA_II::FastNonDominatedSort() {

    QVector< QVector<int> > Fi;
    QVector< QVector<int> > S;
    QVector<int> n(pokolenie.size(), 0);

    Fi.push_back(QVector<int>());
    for (int p_pop = 0; p_pop < pokolenie.size(); p_pop++) {
        S.push_back(QVector<int>());
        for (int q_pop = 0; q_pop < pokolenie.size(); q_pop++) {
            if ((*pokolenie[p_pop]) > (*pokolenie[q_pop])) {
                S[p_pop].push_back(q_pop);
            } else if ((*pokolenie[q_pop]) > (*pokolenie[p_pop])) {
                n[p_pop]++;

            }

        }
        // qDebug() << "n[pop]= " << n[p_pop] << "\n";
        if (n[p_pop] == 0) {

            pokolenie[p_pop]->ranga = 1;
            Fi[0].push_back(p_pop);

        }

    }
    DEBUG("size of Fi= %i\n", Fi.size());
    DEBUG("size of Fi[0] = %i\n",Fi[0].size());

    int i = 0;
    QVector<int> Q;
    while (i < Fi.size() && Fi[i].size() > 0) {
        Q.clear();
        for (int i_p = 0; i_p < Fi[i].size(); i_p++) {
            for (int i_q = 0; i_q < S[Fi[i][i_p]].size(); i_q++) {
                n[ S[Fi[i][i_p]][i_q]]--;
                if (n[ S[Fi[i][i_p]][i_q]] == 0) {
                    pokolenie[S[Fi[i][i_p]][i_q]]->ranga = i + 2;
                    Q.push_back(S[Fi[i][i_p]][i_q]);
                }
            }
        }

        i++;
        if (Q.size() > 0) {
            //qDebug()<<"size Q = "<<Q.size()<<"\n";
            Fi.push_back(QVector<int>());
            for (int j = 0; j < Q.size(); j++) {
                Fi[i].push_back(Q[j]);
            }
        }
    }


    //qDebug()<<"Fi size "<<Fi.size()<<"\n";
    QVector< QVector<Solution*> >* F = new QVector< QVector<Solution*> >();

    for (int i = 0; i < Fi.size(); i++) {
        (*F).push_back(QVector<Solution*>());
        for (int j = 0; j < Fi[i].size(); j++) {
            (*F)[i].push_back(new Solution(*((pokolenie[Fi[i][j]]))));
        }
    }
    //qDebug()<<"il frontow "<<(F)->size()<<"\n";
    return F;
}

void NSGA_II::CrowdingDistanceAssignement(QVector<Solution*>&F) {
    double max_min;
    for (int i = 0; i < F.size(); i++) {
        F[i]->zatloczenie = 0;
    }
    for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {
        Solution::tmp = 0;
        qSort(F.begin(), F.end(), lessThenSolution);
        F[0]->zatloczenie = numeric_limits<double>::max();
        F[F.size() - 1]->zatloczenie = numeric_limits<double>::max();
        max_min = F[F.size() - 1]->przystosowaniePrzeskalowane[i_fun]-F[0]->przystosowaniePrzeskalowane[i_fun];
        for (int i_sol = 1; i_sol < F.size() - 1; i_sol++) {
            if (F[i_sol]->zatloczenie != numeric_limits<double>::max()) {

                F[i_sol]->zatloczenie += (F[i_sol + 1]->przystosowaniePrzeskalowane[i_fun]-F[i_sol - 1]->przystosowaniePrzeskalowane[i_fun]) / max_min;
            }
        }
    }
}

void NSGA_II::Iteruj() {
    for (int i = 0; i < wiel_pop; i++) {
        pokolenie[i] = rodzice[i];
        pokolenie[i + wiel_pop] = rodzenstwo[i];
    }

    DEBUG("%s\n","dekoduje genotyp ... ");
    DekodujGenotyp();
    DEBUG("%s\n","ok");

    // WyswietlFenotyp(&pokolenie);
    DEBUG("%s\n","wyznaczam wart funkcji kryt ... ");
    WyznaczWartFunkcjiKryterialnych();

    DEBUG("%s\n","wyznaczam przystosowanie ... ");
    WyznaczPrzystosowanie();


    // WyswietlPrzystosowanie(&pokolenie);
    DEBUG("%s\n","przeskalowywuje przystosowanie ... ");
    PrzeskalujPrzystosowanie();


    // WyswietlPrzystosowaniePrzeskalowane(&pokolenie);
    // DINFO;
    DEBUG("%s\n","fast non dominated sort ... ");
    F = FastNonDominatedSort();

    DEBUG("il frontow %i\n",(*F).size());
    DEBUG("size of F = %i", F->size());

    // rodzice.clear();
    int i_front = 0;
    int i_sol = 0;
    // DINFO;
    while ((i_sol + (*F)[i_front].size()) <= wiel_pop) {
        //DINFO;
        CrowdingDistanceAssignement((*F)[i_front]);
        // DINFO;
        for (int i = 0; i < (*F)[i_front].size(); i++) {
            rodzice[i_sol] = (*F)[i_front][i];
            i_sol++;
        }

        // DINFO;
        i_front++;
    }
    // DINFO;
    //    i_front=0;
    if (i_sol < wiel_pop) {
        CrowdingDistanceAssignement((*F)[i_front]);
        //  DINFO;
        //    qSort((*F)[i_front].begin(), (*F)[i_front].end(), CrowdedComparisonOperator);

        DEBUG("rozmiar ostatniego frontu %i\n", (*F)[i_front].size());
        DEBUG("wiel_pop - i_sol %i", wiel_pop - i_sol);
        for (int i = 0; i < wiel_pop - i_sol; i++) {
            rodzice[i_sol] = (*F)[i_front][i];
            i_sol++;
        }

    }
    //DINFO;
    //MakeNewPop(&(*F)[i_front], &rodzenstwo
    MakeNewPop();
    //  DINFO;
    // WyswietlRangiZatloczenia(&(*F)[i_front]);

    // this->num_iter++;
}

void NSGA_II::MakeNewPop() {
    //  QVector<Solution*>* Q = new QVector<Solution*>();
    int ind1, ind2;
    for (int i = 0; i < wiel_pop; i++) {
        ind1 = losuj(0, rodzice.size() - 1);
        ind2 = ind1;
        while (ind1 == ind2) {
            ind2 = losuj(0, rodzice.size() - 1);
        }
        // DINFO;

        // qDebug() << "ind1 " << ind1 << " ind2 " << ind2 << "\n";
        if (CrowdedComparisonOperator(rodzice[ind1], rodzice[ind2])) {
            //  DINFO;
            rodzenstwo.push_back(new Solution(*(rodzice[ind1])));
        } else {
            //  DINFO;
            rodzenstwo.push_back(new Solution(*(rodzice[ind2])));
        }

        //  DINFO;
        if (i > 0 && ((i % 2) == 0))//dla parzystego indesku bierzemy 2 ostatnie osob. i wyk. mut. krzyz.
        {
            if (losuj(0.0, 1.0) < prawd_krzyz) {
                Krzyzuj(rodzenstwo[i], rodzenstwo[i - 1]);

            }

            Mutuj(rodzenstwo[i]);
            Mutuj(rodzenstwo[i - 1]);
        }

        //  DINFO;
    }


}

void NSGA_II::Krzyzuj(Solution * S1, Solution* S2) {
    int maks = il_bitow * problem->zmienne.size();
    int m = losuj(1, maks - 1);

    for (int i = m; i < maks; i++) {


    }
    int indeks = 0;
    bool tmp;
    for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
        for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
            if (indeks >= m) {
                tmp = S1->genotyp[i_zmienna][i_bit];
                S1->genotyp[i_zmienna][i_bit] = S2->genotyp[i_zmienna][i_bit];
                S2->genotyp[i_zmienna][i_bit] = tmp;
            }
            indeks++;

        }
    }
}

void NSGA_II::Mutuj(Solution * S) {
    for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
        for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
            if (losuj(0.0, 1.0) < prawd_mutacji) {
                S->genotyp[i_zmienna][i_bit] = !S->genotyp[i_zmienna][i_bit];

            }
        }
    }

}

void NSGA_II::doTheWork() {
    num_iter = 0;
    stopIteration = false;
    for (int i = 0; i < il_iter; i++) {
        QMutexLocker locker(&mutex);
        if (stopIteration) {
            break;
        }
        Iteruj();
        num_iter++;
        emit done();
    }
    qWarning("finish\n");
    emit finished();
}

void NSGA_II::setNum_iter(int num_iter) {
    this->num_iter = num_iter;
}

int NSGA_II::getNum_iter() const {
    return num_iter;
}

void NSGA_II::run() {
    //qDebug()<<"lol\n";
    doTheWork();
}

void NSGA_II::stop() {
    QMutexLocker locker(&mutex);
    stopIteration = true;
}
