#include "nsga_ii_for_ssmodel.h"

NSGA_II_for_ssmodel::NSGA_II_for_ssmodel()
{

}

void NSGA_II_for_ssmodel::WyznaczWartFunkcjiKryterialnych()
{

    QVector<QPair<double,double> > dataXY;

    for (int i = 0; i <pokolenie.size(); i++) {
        if(CheckStabilityOfMySystem(pokolenie[i]->x[0],pokolenie[i]->x[1],pokolenie[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
        GetStepResponse(&sys,pokolenie[i]->x[0],pokolenie[i]->x[1],pokolenie[i]->x[2],30,0.01,dataXY);

       // GetStepResponseCharacteristics( dataXY,maxOvershoot,riseTime,settlingTime);
        GetStepResponseCharacteristics( dataXY,pokolenie[i]->wartFunkcjiKryterialnych[0],pokolenie[i]->wartFunkcjiKryterialnych[1],pokolenie[i]->wartFunkcjiKryterialnych[2]);

        if(pokolenie[i]->wartFunkcjiKryterialnych[2]>20)
        {
            pokolenie[i]->wartFunkcjiKryterialnych[2]+=100.0;
            pokolenie[i]->wartFunkcjiKryterialnych[1]+=100.0;
            pokolenie[i]->wartFunkcjiKryterialnych[0]+=100.0;
        }

        }
        else
        {
            pokolenie[i]->wartFunkcjiKryterialnych[0]=10000;
            pokolenie[i]->wartFunkcjiKryterialnych[1]=100;
            pokolenie[i]->wartFunkcjiKryterialnych[2]=100;
        }


    }

}

void NSGA_II_for_ssmodel::Inicjalizuj()
{

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
    for (int i = 0; i < wiel_pop; i++) {
        pokolenie[i] = rodzice[i];
        pokolenie[i + wiel_pop] = rodzice[i];
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

}
