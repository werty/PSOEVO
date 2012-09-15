/*
 * File:   Wskazniki.h
 * Author: marcin
 *
 * Created on 5 wrzesień 2011, 05:13
 */

#ifndef WSKAZNIKI_H
#define	WSKAZNIKI_H
#include <QVector>
#include "Particle.h"
#include "Problem.h"


class Wskazniki {
public:
    Wskazniki();
    
    Wskazniki(const Wskazniki& orig);
    

    
    /* GOL - global optimality level
     *globalny poziom optymalnosci
     *
     *
     */


    double Distance(const QVector<double>& vecA,const QVector<double>& vecB);
    double Spacing(QList<QVector<double> >& population);

    template <class T>
    double Distance2(const T& vecA,const T& vecB);

    template <class T>
    double Spacing2(QVector<T*>&P,unsigned int fun1,unsigned int fun2);
    template <class T>
    double Spacing2(QVector<T*>&P,unsigned int fun1,unsigned int fun2,T*P_ext1,T*P_ext2);


    bool Cmp1ElOfVectors(QVector<double> &vec1, QVector<double> &vec2);



    template <class T>
    void GOL(QVector<T *> &P, QVector<unsigned int> &indSortGOL);

    template <class T>
    double D_EC(QVector<T *> &P);

    template <class T>
    double D_EP_GOL(QVector<T *> &P, T *gol);

    template <class T>
    double D_EC_GOL(QVector<T *> &P, T *gol);

    virtual ~Wskazniki();
private:

};



/**
 * @brief D_EC Średnia odległość euklidesowa ocen osobników
 * @param P populacja
 * @return
 */
template <class T>
double Wskazniki::D_EC(QVector<T*> &P)
{
    double d_ec=0;

    if(P.size()<2)
    {
        qWarning()<<"P.size<2";
        return 0;
    }
    for (auto it1 = P.begin(); it1 != P.end()-1;it1++) {
        for (auto it2 = it1+1; it2 != P.end();it2++) {
            d_ec+=Distance((*it1)->wartFunkcjiKryterialnych,(*it2)->wartFunkcjiKryterialnych);
        }
    }

    return 2.0*d_ec/(P.size()*(P.size()-1.0));
}


/**
 * @brief D_EC_GOL Średnia odległość euklidesowa od jednostki najlepszej weg GOL w przestrzeni kryterialnej
 * @param P populacja
 * @return
 */
template <class T>
double Wskazniki::D_EC_GOL(QVector<T*> &P,T*  gol)
{
    double d_ec_gol=0;

    if(P.size()<2)
    {
        qWarning()<<"P.size<2";
        return 0;
    }
    for (auto it1 = P.begin(); it1 != P.end();it1++) {
        d_ec_gol+=Distance((*it1)->wartFunkcjiKryterialnych,gol->wartFunkcjiKryterialnych);

    }

    return d_ec_gol/(P.size());
}

/**
 * @brief D_EP_GOL Średnia odległość euklidesowa od jednostki najlepsze wg GOL w przestrzeni parametrycznej
 * @param P populacja
 * @return
 */
template <class T>
double Wskazniki::D_EP_GOL(QVector<T*> &P,T* gol)
{
    double d_ep_gol=0;

    if(P.size()<2)
    {
        qWarning()<<"P.size<2";
        return 0;
    }
    for (auto it1 = P.begin(); it1 != P.end();it1++) {
        d_ep_gol+=Distance((*it1)->x,gol->x);

    }

    return d_ep_gol/(P.size());
}


template <class T>
void Wskazniki::GOL(QVector<T*>&P,QVector<unsigned int> &indSortGOL) {
    indSortGOL.clear();
    QVector<float> maksWartoscPrzystosowania(P[0]->przystosowanie.size(),0);
    //wyznaczamy maksymalne wartosci przystosowania przeskalowanego dla kazdej funckcji kryterialnej
    for(unsigned int i=0;i<P.size();i++)
    {
      //  qDebug()<<"przystosowanie "<<P[i]->przystosowaniePrzeskalowane;
        for(unsigned int j=0;j<P[0]->przystosowanie.size();j++)
        {
            if(P[i]->przystosowanie[j]>maksWartoscPrzystosowania[j])
            {
                maksWartoscPrzystosowania[j]=P[i]->przystosowanie[j];
            }
        }
    }

    //qDebug()<<"maksWartoscPrzystosowania "<<maksWartoscPrzystosowania;
    //wyznaczamy gol czyli najmniejsza wartosc ze znormalizowanych wartosci przesk przyst
    float wartoscGOL;

    float tmp;
    indSortGOL.resize(P.size());//indeksy osobnikow posortowanych od najw do najm GOL'a
    for(unsigned int i=0;i<P.size();i++)
    {
        indSortGOL[i]=i;
        wartoscGOL=P[i]->przystosowanie[0]/maksWartoscPrzystosowania[0];
        for(unsigned int j=1;j<P[i]->przystosowanie.size();j++)
        {
            tmp=P[i]->przystosowanie[j]/maksWartoscPrzystosowania[j];
            if(tmp<wartoscGOL)
            {
                wartoscGOL=tmp;
            }
        }
        P[i]->GOL=wartoscGOL;
    }

    using namespace std::placeholders;

    // qSort(indSortGOL.begin(),indSortGOL.end(),bind(&GGA::porownajWartGOL,this,_1,_2));

    qSort(indSortGOL.begin(),indSortGOL.end(),[&](int ind1, int ind2)->bool{
        if(P[ind1]->GOL>=P[ind2]->GOL)
        {
            return true;
        }
        return false;
    }
    );


    //qDebug()<<P[indSortGOL[0]]->GOL;
    //qDebug()<<P[indSortGOL[1]]->GOL;
    //qDebug()<<P[indSortGOL[2]]->GOL;


}



template <class T>
double Wskazniki::Distance2(const T &vecA, const T &vecB)
{
    double dist=0;

   // qDebug()<<vecA;
   // qDebug()<<vecB;
    double tmp;
    for (int i = 0; i < vecA.size(); ++i) {
        tmp=qAbs(vecA[i]-vecB[i]);
        dist+=tmp*tmp;
    }


    return qSqrt(dist);
}


/**
 * @brief wyznacza wskaznik oznaczajacy rozproszenie osobnikow na froncie Pareto optymalnym
 *        w przypadu gdy nie znamy prawdziwego frontu Pareto, nie podajemy 2 ostatnich parametrow
 *        a wskaznik jest okrojony wtedy tzn nie uwzglednia rozpietosci prawdziwego frontu Pareto
 *
 *
 * @param P wektor rozwiazań niezdominowanych
 * @param fun1 indeks pierwszej funkcji kryterialnej
 * @param fun2 indeks drugiej funkcji kryterialnej
 */
template <class T>
double Wskazniki::Spacing2(QVector<T*>&P,unsigned int fun1,unsigned int fun2)
{
    using namespace std::placeholders;
    //qSort(P.begin(),P.end(),bind(&Wskazniki::Cmp1ElOfVectors2,this,_1,_2));

    QVector<QVector<double>> solutions;

    for (int i = 0; i < P.size(); ++i) {
        solutions.push_back(P[i]->wartFunkcjiKryterialnych);
    }

    qDebug()<<"solutions.size() "<<solutions.size()<<endl;
    qSort(solutions.begin(),solutions.end(),bind(&Wskazniki::Cmp1ElOfVectors,this,_1,_2));

    double spacing=0,distance_mean=0,distance_sum=0;

    for (auto it1 = solutions.begin(); it1 != solutions.end()-1;it1++) {
        distance_mean=Distance2(*it1,*(it1+1));
    }
    distance_mean=distance_mean/(P.size()-1);

    for (auto it1 = solutions.begin(); it1 != solutions.end()-1;it1++) {
        distance_sum+=qAbs(distance_mean-Distance2(*it1,*(it1+1)));
    }

    return distance_sum/((P.size()-1)*distance_mean);
}

/**
 * @brief wyznacza wskaznik oznaczajacy rozproszenie osobnikow na froncie Pareto optymalnym
 *        w przypadu gdy nie znamy prawdziwego frontu Pareto, nie podajemy 2 ostatnich parametrow
 *        a wskaznik jest okrojony wtedy tzn nie uwzglednia rozpietosci prawdziwego frontu Pareto
 *
 *
 * @param P wektor rozwiazań niezdominowanych
 * @param fun1 indeks pierwszej funkcji kryterialnej
 * @param fun2 indeks drugiej funkcji kryterialnej
 * @param P_ext1 rozwiazanie skrajne pierwsze wyznaczone za pomoca true Pareto front
 * @param P_ext2 rozwiazanie skrajne drugie wyznaczone za pomoca true Pareto front
 */
template <class T>
double Wskazniki::Spacing2(QVector<T*>&P,unsigned int fun1,unsigned int fun2,T*P_ext1,T*P_ext2)
{
    //    using namespace std::placeholders;
    //    qSort(population.begin(),population.end(),bind(&Wskazniki::Cmp1ElOfVectors,this,_1,_2));
    //    double spacing=0,distance,distanceTmp;
    //    QList<QVector<double> >::iterator itTmp;
    //    for (auto it1 = population.begin(); it1 != population.end();) {
    //        distanceTmp=0;
    //        distance=std::numeric_limits<double>::max();;
    //         for (auto it2 = population.begin(); it2 != population.end(); ++it2) {
    //             if(it1!=it2)
    //             {

    //                 distanceTmp=Distance(*it1,*it2);
    //                 if(distanceTmp<distance)
    //                 {
    //                     distance=distanceTmp;
    //                 }
    //             }
    //         }

    //         if(distance!=std::numeric_limits<double>::max())
    //         {
    //            // qDebug()<<distanceTmp;
    //            // qDebug()<<"spac "<<spacing;
    //             spacing+=distance;
    //         }
    //         it1=population.erase(it1);
    //    }
    //    return spacing;
}

#endif	/* WSKAZNIKI_H */

