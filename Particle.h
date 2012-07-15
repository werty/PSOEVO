
/*
 * File:   Particle.h
 * Author: werty
 *
 * Created on 7 sierpień 2011, 17:17
 */
#ifndef PARTICLE_H
#define PARTICLE_H

#include <QDebug>

class Particle
{
    public:
        QVector<double> v, x, pbest_pos;
        QVector<double> fitness_pbest;
        QVector<double> wartFunkcjiKryterialnych;
        QVector<double> przystosowanie;
        QVector<double> przystosowaniePrzeskalowane;
        int      il_przek_ograniczen;    // ilosc przekroczonych ograniczen
        int      gbest_ind;
        int      il_zmiennych;
        int      il_funkcji;
        int      il_ograniczen;
        bool     zdominowana;
        int      licz_kostki;//il czastek ktora lacznie znajduje sie w hip. danej czastki

        double GOL;
        Particle(int il_zmiennych, int il_funkcji, int il_ograniczen);

        Particle(double * v_tmp, double * x_pos_tmp);

        Particle(const Particle & oryg);

        // bool operator>(const Particle & b); //do maksymalizacji
        // bool operator<(const Particle & b); //do minimalizacji
        Particle & operator = (const Particle & b);
};
#endif   /* PARTICLE_H */
