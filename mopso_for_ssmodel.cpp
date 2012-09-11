#include "mopso_for_ssmodel.h"

MOPSO_for_ssmodel::MOPSO_for_ssmodel()
{
}

void MOPSO_for_ssmodel::WyznaczWartFunkcjiKryterialnych()
{
    qDebug()<<"ok"<<endl;
    QVector<QPair<double,double>> dataXY;

    for (int i = 0; i < populacja.size(); ++i) {

        if(CheckStabilityOfMySystem(populacja[i]->x[0],populacja[i]->x[1],populacja[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
        GetStepResponse(&sys,populacja[i]->x[0],populacja[i]->x[1],populacja[i]->x[2],30,0.01,dataXY);

       // GetStepResponseCharacteristics( dataXY,maxOvershoot,riseTime,settlingTime);
        GetStepResponseCharacteristics( dataXY,populacja[i]->wartFunkcjiKryterialnych[0],populacja[i]->wartFunkcjiKryterialnych[1],populacja[i]->wartFunkcjiKryterialnych[2]);
        }
        else
        {
            populacja[i]->wartFunkcjiKryterialnych[0]=10000;
            populacja[i]->wartFunkcjiKryterialnych[1]=100;
            populacja[i]->wartFunkcjiKryterialnych[2]=100;
        }
    }


}

void MOPSO_for_ssmodel::Inicjalizuj()
{
    qDebug() << "inicjuje...\n";
    QVector<QPair<double,double>> dataXY;


    for (int i = 0; i < populacja.size(); i++) {
        for (int x_ind = 0; x_ind < this -> il_zmiennych; x_ind++) {
            x_width = problem -> zmienne[x_ind] -> max - problem -> zmienne[x_ind] -> min;
            xmin = problem -> zmienne[x_ind] -> min;
          //  qDebug()<<"x_width "<<x_width<<" xmin "<<xmin<<"\n";
            populacja[i] -> v[x_ind] =0;//  (float)rand() / (float)RAND_MAX * vmax - 0.5 * vmax;
            populacja[i] -> x[x_ind] = (float) rand() / (float) RAND_MAX * x_width + xmin;

             //qDebug()<<"x "<<populacja[i]->x[x_ind] <<"\n";
            problem -> zmienne[x_ind] -> zmienna = populacja[i] -> x[x_ind];
        }

//        for (int f_ind = 0; f_ind < this -> il_funkcji; f_ind++) {
//            populacja[i] -> wartFunkcjiKryterialnych[f_ind] = problem -> parseryFunkcji[f_ind] -> Eval();
//            populacja[i] -> fitness_pbest[f_ind] = populacja[i] -> wartFunkcjiKryterialnych[f_ind];
//        }



        if(CheckStabilityOfMySystem(populacja[i]->x[0],populacja[i]->x[1],populacja[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {

        GetStepResponse(&sys,populacja[i]->x[0],populacja[i]->x[1],populacja[i]->x[2],30,0.01,dataXY);

       // GetStepResponseCharacteristics( dataXY,maxOvershoot,riseTime,settlingTime);
        GetStepResponseCharacteristics( dataXY,populacja[i]->wartFunkcjiKryterialnych[0],populacja[i]->wartFunkcjiKryterialnych[1],populacja[i]->wartFunkcjiKryterialnych[2]);
        qDebug()<<"stability for pid "<<populacja[i]->x[0]<<" "<<populacja[i]->x[1]<<" "<<populacja[i]->x[2]<<" is "<<CheckStabilityOfMySystem(populacja[i]->x[0],populacja[i]->x[1],populacja[i]->x[2])<<endl;


        qDebug()<<"MaxOvershoot "<<populacja[i]->wartFunkcjiKryterialnych[0]<<" RiseTime "<<populacja[i]->wartFunkcjiKryterialnych[1]<<" SettlingTime "<<populacja[i]->wartFunkcjiKryterialnych[2]<<endl;

        }
        else
        {
            populacja[i]->wartFunkcjiKryterialnych[0]=10000;
            populacja[i]->wartFunkcjiKryterialnych[1]=100;
            populacja[i]->wartFunkcjiKryterialnych[2]=100;

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

    qDebug()<<"ilosc niezdom "<<repozytorium.size()<<endl;

    qDebug() << "Generuje kostki\n";
    generuj_kostki();

    qDebug()<<"ilosc hiperkostek "<<hiperkostki.size()<<endl;

    qDebug() << "po Generuje kostki\n";

    for (int i_pop = 0; i_pop < populacja.size(); i_pop++) {
        if (!populacja[i_pop] -> zdominowana) {
            repozytorium.push_back(new Particle(*populacja[i_pop]));
        } else {
        }
    }

     qDebug() << " Generuje kostki\n";

}
