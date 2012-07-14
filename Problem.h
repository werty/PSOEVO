
/*
 * File:   Problem.h
 * Author: werty
 *
 * Created on 14 lipiec 2011, 01:18
 */
#ifndef PROBLEM_H
#define PROBLEM_H

#include <QDebug>
#include "Particle.h"
#include "muParser.h"
#include "muParserInt.h"
#include "struktury.h"
#include <vector>

using namespace std;
using namespace mu;

struct Zmienna
{
    double zmienna;
    string nazwa;
    double min, max;

    Zmienna()
    {
    }

    Zmienna(string nazwa)
    {
        qDebug() << "tworze zmienna " << nazwa.c_str();

        this -> nazwa = nazwa;
    }
};


struct Ograniczenie
{
    Parser * lewa_funkcja, *prawa_funkcja;
    int      rodzaj;    // 0  < ; 1 <= ;2 = ;3>=;4>

    Ograniczenie()
    {
    }

    Ograniczenie(Parser * lewa, int rodz, Parser * prawa)
    {
        lewa_funkcja  = lewa;
        rodzaj        = rodz;
        prawa_funkcja = prawa;
    }
};


class Problem
{
    public:
        int cos;

        value_type * AddVariable(const char_type * a_szName, void * a_pUserData);
        value_type * AddVariable2(const char_type * a_szName, void * a_pUserData);    // przy ograniczeniach

        vector<Parser *>       parseryFunkcji;
        vector<QString>         funkcje;
        vector<Zmienna *>      zmienne;
        vector<Ograniczenie *> ograniczenia;
        bool *                 tab_minmax;    // okresla czy max. funkcje kryt czy min. true oznacza max.

        static value_type * CCallback2(const char_type * a_szName, void * a_pUserData)
        {                                                                             // przy ograniczeniach
            Problem * h = static_cast<Problem *>(a_pUserData);

            return h -> AddVariable2(a_szName, h);
        }

        static value_type * CCallback(const char_type * a_szName, void * a_pUserData)
        {
            Problem * h = static_cast<Problem *>(a_pUserData);

            return h -> AddVariable(a_szName, h);
        }

        Problem();

        bool Dominuje(const Particle & a, const Particle & b);    // czy a dominuje b

        virtual ~Problem();

    private:
};
#endif   /* PROBLEM_H */
