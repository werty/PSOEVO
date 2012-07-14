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
    double GOL( QVector<Particle *> repozytorium,Problem * problem);
    double Distance(const QVector<double>& vecA,const QVector<double>& vecB);
    double Spacing(QList<QVector<double> >& population);
    
    virtual ~Wskazniki();
    bool Cmp1ElOfVectors(QVector<double> vec1, QVector<double> vec2);
private:

};

#endif	/* WSKAZNIKI_H */

