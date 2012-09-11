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


class MOPSO_for_ssmodel :  public MOPSO {
    Q_OBJECT
public:
    MOPSO_for_ssmodel();
 void  virtual WyznaczWartFunkcjiKryterialnych();
};

#endif // MOPSO_FOR_SSMODEL_H
