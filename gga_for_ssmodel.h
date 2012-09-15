#ifndef GGA_FOR_SSMODEL_H
#define GGA_FOR_SSMODEL_H

#include <QTime>
#include <QThread>
//#include <QTimer>
#include <QDateTime>
#include <QVector>
#include <QMutex>
#include <QtAlgorithms>
#include "Particle.h"
#include "muParser.h"
#include "muParserInt.h"
#include "Problem.h"
#include "uzyteczne_funkcje.h"
#include "GGA.h"
#include "ssmodels.h"


class GGA_for_ssmodel : public GGA
{
public:
    GGA_for_ssmodel();
    GGA_for_ssmodel(unsigned int wielk_pop,unsigned  int il_bitow,double prawd_mutacji,double prawd_krzyz,unsigned int ilOsobElit, QVector<QString> nazwyRodzajnikow, QVector<QVector<unsigned int> > funkcjeRodzajnikow, Problem * problem):GGA(  wielk_pop,   il_bitow, prawd_mutacji, prawd_krzyz, ilOsobElit,  nazwyRodzajnikow, funkcjeRodzajnikow,  problem){};


    SysClosedLoopYsWithPID sys;
    void  virtual WyznaczWartFunkcjiKryterialnych();
    virtual void Inicjalizuj();
};

#endif // GGA_FOR_SSMODEL_H
