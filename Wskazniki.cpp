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



//bool GGA::porownajWartGOL(int ind1, int ind2)
//{

//    if(rodzice[ind1]->GOL>=rodzice[ind2]->GOL)
//    {
//        return true;
//    }

//    return false;

//}


double Wskazniki::Distance(const QVector<double> &vecA, const QVector<double> &vecB)
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






