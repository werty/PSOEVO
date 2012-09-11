#ifndef MOPSO_FOR_SSMODEL_H
#define MOPSO_FOR_SSMODEL_H

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
#include "MOPSO.h"
#include "ssmodels.h"


class MOPSO_for_ssmodel :  public MOPSO {
    Q_OBJECT
public:
    MOPSO_for_ssmodel();
    MOPSO_for_ssmodel(int wiel_pop, int wielk_rep, int il_hiperk, bool mutacja, Problem * problem):MOPSO( wiel_pop,  wielk_rep, il_hiperk,  mutacja,  problem){}

    SysClosedLoopYsWithPID sys;
 void  virtual WyznaczWartFunkcjiKryterialnych();
 virtual void Inicjalizuj();
};

#endif // MOPSO_FOR_SSMODEL_H
