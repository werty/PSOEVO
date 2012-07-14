/*
 * File:   Wskazniki.cpp
 * Author: marcin
 * 
 * Created on 5 wrzesień 2011, 05:13
 */

#include "Wskazniki.h"
#include "Problem.h"

Wskazniki::Wskazniki() {
     
}

Wskazniki::Wskazniki(const Wskazniki& orig) {
}

Wskazniki::~Wskazniki() {
}

double Wskazniki::GOL(QVector<Particle *> repozytorium, Problem * problem) {
    QVector<double> tab_skrajnych_war = QVector<double>(problem->parseryFunkcji.size());
    QVector<double> tab_wart = QVector<double>(problem->parseryFunkcji.size());
    QVector<double> tab_gol = QVector<double>(repozytorium.size());
    double** tab_wart_funk_celu = new double*[repozytorium.size()];

    for (int i = 0; i < repozytorium.size(); i++) {
        tab_wart_funk_celu[i] = new double[problem->parseryFunkcji.size()];
    }


    //wyznaczamy mak / min wartosci funkcji kryterialnych aby wyznaczyc wartosci funkcji celu 
    for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {


        if (problem->tab_minmax[i_fun])//maksymalizacja wiec szukamy minimalnej wartosci
        {

            tab_skrajnych_war[i_fun] = repozytorium[0]->fitness[i_fun];
            for (int i_rep = 0; i_rep < repozytorium.size(); i_rep++) {
                if (repozytorium[i_rep]->fitness[i_fun] < tab_skrajnych_war[i_fun]) {
                    tab_skrajnych_war[i_fun] = repozytorium[i_rep]->fitness[i_fun];
                }
            }
        } else//minimalizacja wiec szukamy maksymalnej wartosci
        {
            tab_skrajnych_war[i_fun] = repozytorium[0]->fitness[i_fun];
            for (int i_rep = 0; i_rep < repozytorium.size(); i_rep++) {
                if (repozytorium[i_rep]->fitness[i_fun] > tab_skrajnych_war[i_fun]) {
                    tab_skrajnych_war[i_fun] = repozytorium[i_rep]->fitness[i_fun];
                }
            }
        }
    }

   // qDebug() << "tab warto skrajnych " << tab_skrajnych_war << "\n";
    //wyznaczamy wart funkcji celu 
    for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {
        if (problem->tab_minmax[i_fun])//maksymalizacja f-min
        {
            for (int i_rep = 0; i_rep < repozytorium.size(); i_rep++) {
                tab_wart_funk_celu[i_rep][i_fun] = repozytorium[i_rep]->fitness[i_fun] - tab_skrajnych_war[i_fun];
            }
        } else//minimalizacja wiec maks-f
        {
            for (int i_rep = 0; i_rep < repozytorium.size(); i_rep++) {
                tab_wart_funk_celu[i_rep][i_fun] = tab_skrajnych_war[i_fun] - repozytorium[i_rep]->fitness[i_fun];
            }
        }
    }

    for (int i_rep = 0; i_rep < repozytorium.size(); i_rep++) {
       // qDebug() << "i " << i_rep << " ";
        for (int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {
         //   qDebug() << " fc = " << tab_wart_funk_celu[i_rep][i_fun];


        }

      //  qDebug() << "\n";

    }



    //wyznaczamy maks. wartosci funkcji celu
    for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {
        tab_skrajnych_war[i_fun] = tab_wart_funk_celu[0][i_fun];
        for (int i_rep = 0; i_rep < repozytorium.size(); i_rep++) {
            if (tab_wart_funk_celu[i_rep][i_fun] > tab_skrajnych_war[i_fun]) {
                tab_skrajnych_war[i_fun] = tab_wart_funk_celu[i_rep][i_fun];
            }
        }

    }

  //  qDebug() << "tab warto skrajnych fun celu " << tab_skrajnych_war << "\n";

    //wyznaczmy gol dla kazdego rozwiazania
    for (int i_rep = 0; i_rep < repozytorium.size(); i_rep++) {
        for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {
            tab_wart[i_fun] = tab_wart_funk_celu[i_rep][i_fun] / tab_skrajnych_war[i_fun];
        }

        qSort(tab_wart.begin(), tab_wart.end(), qLess<double>());
   //     qDebug() << "tab_wart sort "<<i_rep<<" " << tab_wart << "\n";

        tab_gol[i_rep] = tab_wart[0];





    }

    //sortujemy i wzracamy najwyzsza wartosc GOL sposrod wszystkich os. rep.
  //  qDebug() << "tab_gol " << tab_gol << "\n";
    qSort(tab_gol.begin(), tab_gol.end(), qGreater<double>());
   // qDebug() << "tab_gol sort" << tab_gol << "\n";

    return tab_gol[0];

}

double Wskazniki::Distance(const QVector<double> &vecA, const QVector<double> &vecB)
{
    double dist=0;

   // qDebug()<<vecA;
   // qDebug()<<vecB;
    for (int i = 0; i < vecA.size(); ++i) {
        dist+=qAbs(vecA[i]-vecB[i]);
    }


    return dist;
}
bool Wskazniki::Cmp1ElOfVectors(QVector<double> vec1, QVector<double> vec2)
{
    if(vec1[0]<vec2[0])
    {
        return true;
    }
    return false;
}
double Wskazniki::Spacing(QList<QVector<double> > &population)
{

      using namespace std::placeholders;
    qSort(population.begin(),population.end(),bind(&Wskazniki::Cmp1ElOfVectors,this,_1,_2));
    double spacing=0,distance,distanceTmp;
    QList<QVector<double> >::iterator itTmp;
    for (auto it1 = population.begin(); it1 != population.end();) {

        distanceTmp=0;
        distance=std::numeric_limits<double>::max();;
         for (auto it2 = population.begin(); it2 != population.end(); ++it2) {

             if(it1!=it2)
             {

                 distanceTmp=Distance(*it1,*it2);


                 if(distanceTmp<distance)
                 {
                     distance=distanceTmp;


                 }


             }


         }


         if(distance!=std::numeric_limits<double>::max())
         {
            // qDebug()<<distanceTmp;
            // qDebug()<<"spac "<<spacing;
             spacing+=distance;
         }
         it1=population.erase(it1);


    }
    return spacing;
}






