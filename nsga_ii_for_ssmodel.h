#ifndef NSGA_II_FOR_SSMODEL_H
#define NSGA_II_FOR_SSMODEL_H

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
#include "NSGA_II.h"
#include "ssmodels.h"

class NSGA_II_for_ssmodel : public NSGA_II{
    Q_OBJECT
public:
    NSGA_II_for_ssmodel();
    NSGA_II_for_ssmodel(int wielk_pop, int il_bitow, double prawd_mutacji, double prawd_krzyz, Problem * problem) :NSGA_II( wielk_pop,  il_bitow,  prawd_mutacji,  prawd_krzyz,  problem){};
    SysClosedLoopYsWithPID sys;
    void  virtual WyznaczWartFunkcjiKryterialnych();
    virtual void Inicjalizuj();
};

#endif // NSGA_II_FOR_SSMODEL_H
