
/*
 * File:   PSO.h
 * Author: werty
 *
 * Created on 27 kwiecień 2011, 23:10
 */
#ifndef MOPSO_H
#define MOPSO_H

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

class MOPSO : public QThread {
    Q_OBJECT
public:
    // bool                maximize;
    bool koniec;
    bool z_mutacja;
    double fitness_gbest;
    double gbest_pos[2];
    double c_g, c_p;
    double vmax;
    double xmin, xmax, ymin, ymax;
    double x_width, y_width;
    double xVal, yVal; // zmienne dla parsera
    double czi, w;
    int il_zmiennych;
    int il_funkcji;
    int il_ograniczen;
    int il_iter;
    int num_iter;
    mu::Parser parser;
    Problem * problem;
    QVector<Particle *> populacja;
    QVector<Particle *> pbests;
    QVector<Particle *> repozytorium;
    int il_hiperkostek; // na ile czesci dzielimy kazdy wymiar w przestrzeni funkcji kryt.
    int wielk_rep;
    float * min_fitness, *max_fitness, *dx_fitness;
    QVector<int> wek_usun;
    bool stopIteration;
    QMutex  mutex;
    double c;
    

    QVector<unsigned int> indSortGOL;

    void generuj_kostki();

    QDateTime time;
    QVector<Hiperkostka> hiperkostki;

    MOPSO(QObject *parent = 0);
    MOPSO(int wiel_pop, int wielk_rep, int il_hiperk, bool mutacja, Problem * problem);
    MOPSO(const MOPSO & orig);

    virtual ~MOPSO();
    int losujKostkeRuletka();
    void WyznaczWartFunkcjiKryterialnych();
    void WyznaczPrzystosowanie();
    void PrzeskalujPrzystosowanie();
    void aktualizuj_predkosci();
    void aktualizuj_pozycje();
    void aktualizuj_pbest();
    void aktualizuj_repozytorium();
    void mutuj(int gen, int il_gen, double wsp_mutacji);
    void sprawdz_ograniczenia();
    void sprawdz_zdominowanie();
    void wyswietl_repozytorium();
    void wyswietl_populacje();
    void RedukujRepozytorium();
    void Inicjalizuj();
    void Iteruj();
protected:
    void run();
signals:
    void done();
    void finished();
public slots:
    void doTheWork();
    void stop();

};
#endif   /* PSO_H */
