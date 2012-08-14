/* 
 * File:   GGA.h
 * Author: marcin
 *
 * Created on 8 wrzesień 2011, 03:24
 */

#ifndef NSGA_II_H
#define	NSGA_II_H

#include <QTime>
#include <QThread>
#include <QMutex>
//#include <QTimer>
//#include <QDoubleValidator>
#include <limits>
#include <Chromosome.h>
#include "Problem.h"
#include "Solution.h"
#include "struktury.h"
#include "uzyteczne_funkcje.h"

class NSGA_II : public QThread {
    Q_OBJECT
public:
    int il_bitow, wiel_pop, il_iter, num_iter;
    double prawd_mutacji, prawd_krzyz, c;
    QVector<Solution*> rodzice, rodzenstwo, pokolenie;
    QVector<QVector<Solution*> >* F;
    double zakres;
    Problem *problem;
    bool stopIteration;
    QMutex  mutex;
    bool zainicjalizowany;
    QVector<unsigned int> indSortGOL;

    void Inicjalizuj();
    void DekodujGenotyp();
    void WyznaczWartFunkcjiKryterialnych();
    void WyznaczPrzystosowanie();
    void PrzeskalujPrzystosowanie();
    void WyswietlPrzystosowanie();
    void WyswietlPrzystosowaniePrzeskalowane(QVector<Solution*>& P);
    void WyswietlRangiZatloczenia();
    void WyswietlFenotyp();
    QVector<QVector<Solution*> > * FastNonDominatedSort();
    void CrowdingDistanceAssignement(QVector<Solution*>&F);
    void MakeNewPop();
    void Krzyzuj(Solution * S1, Solution* S2);
    void Mutuj(Solution * S);
    void Iteruj();
    NSGA_II(QObject *parent = 0);
    NSGA_II(int wielk_pop, int il_bitow, double prawd_mutacji, double prawd_krzyz, Problem * problem);
    virtual ~NSGA_II();
protected:
    void run();
signals:
    void done();
    void finished();
public slots:
    void doTheWork();
    void stop();
    void setNum_iter(int num_iter);
    int getNum_iter() const;

};

#endif	/* GGA_H */

