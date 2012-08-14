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
    
    virtual ~Wskazniki();
    bool Cmp1ElOfVectors(QVector<double> vec1, QVector<double> vec2);

    template <class T>
    void GOL(QVector<T *> &P, QVector<unsigned int> &indSortGOL);

    template <class T>
    double D_EC(QVector<T *> &P);

    template <class T>
    double D_EP_GOL(QVector<T *> &P, T *gol);

    template <class T>
    double D_EC_GOL(QVector<T *> &P, T *gol);
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
    QVector<float> maksWartoscPrzystosowania(P[0]->przystosowaniePrzeskalowane.size(),0);
    //wyznaczamy maksymalne wartosci przystosowania przeskalowanego dla kazdej funckcji kryterialnej
    for(unsigned int i=0;i<P.size();i++)
    {
        qDebug()<<"przystosowanie "<<P[i]->przystosowaniePrzeskalowane;
        for(unsigned int j=0;j<P[0]->przystosowaniePrzeskalowane.size();j++)
        {
            if(P[i]->przystosowaniePrzeskalowane[j]>maksWartoscPrzystosowania[j])
            {
                maksWartoscPrzystosowania[j]=P[i]->przystosowaniePrzeskalowane[j];
            }
        }
    }

    qDebug()<<"maksWartoscPrzystosowania "<<maksWartoscPrzystosowania;
    //wyznaczamy gol czyli najmniejsza wartosc ze znormalizowanych wartosci przesk przyst
    float wartoscGOL;

    float tmp;
    indSortGOL.resize(P.size());//indeksy osobnikow posortowanych od najw do najm GOL'a
    for(unsigned int i=0;i<P.size();i++)
    {
        indSortGOL[i]=i;
        wartoscGOL=P[i]->przystosowaniePrzeskalowane[0]/maksWartoscPrzystosowania[0];
        for(unsigned int j=1;j<P[i]->przystosowaniePrzeskalowane.size();j++)
        {
            tmp=P[i]->przystosowaniePrzeskalowane[j]/maksWartoscPrzystosowania[j];
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


#endif	/* WSKAZNIKI_H */

