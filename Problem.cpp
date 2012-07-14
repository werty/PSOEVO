/* 
 * File:   Problem.cpp
 * Author: werty
 * 
 * Created on 14 lipiec 2011, 01:18
 */

#include "Problem.h"

Problem::Problem() {
    zmienne.clear();
    parseryFunkcji.clear();
    cos = 5;
}

Problem::~Problem() {
}

value_type* Problem::AddVariable(const char_type *a_szName, void *a_pUserData) {
    // I don't want dynamic allocation here, so i used this static buffer
    // If you want dynamic allocation you must allocate all variables dynamically
    // in order to delete them later on. Or you find other ways to keep track of 
    // variables that have been created implicitely.
    Problem* p = (Problem*) a_pUserData;

    qDebug() << "cos " << p->cos << "\n";
    qDebug() << "add variable " << a_szName << "\n";

    qDebug() << "next " << "\n";

    qDebug() << " zmienne.size() " << p->zmienne.size() << "\n";
    string nazwa = string(a_szName);
    for (unsigned int i = 0; i < p->zmienne.size(); i++) {
        qDebug() << "porownuje" << "\n";
        if (nazwa.compare(p->zmienne[i]->nazwa) == 0) {
            qDebug() << "zwracam wskaznik zmiennej " << p->zmienne[i]->nazwa.c_str() << "\n";
            return &p->zmienne[i]->zmienna;


        }

    }
    qDebug() << "Generating new variable " << a_szName;
    p->zmienne.push_back(new Zmienna(string(a_szName)));

    return &(p->zmienne[p->zmienne.size() - 1]->zmienna);

};

value_type* Problem::AddVariable2(const char_type *a_szName, void *a_pUserData) {
    // I don't want dynamic allocation here, so i used this static buffer
    // If you want dynamic allocation you must allocate all variables dynamically
    // in order to delete them later on. Or you find other ways to keep track of 
    // variables that have been created implicitely.
    Problem* p = (Problem*) a_pUserData;


    qDebug() << "add variable " << a_szName << "\n";

    qDebug() << "next " << "\n";

    qDebug() << "zmienne.size() " << p->zmienne.size() << "\n";
    string nazwa = string(a_szName);
    for (unsigned int i = 0; i < p->zmienne.size(); i++) {
        qDebug() << "porownuje";
        if (nazwa.compare(p->zmienne[i]->nazwa) == 0) {
            qDebug() << "zwracam wskaznik zmiennej " << p->zmienne[i]->nazwa.c_str() << "\n";
            return &p->zmienne[i]->zmienna;


        }


    }


   // qDebug() << "Nie znalazlem .Blad!!! " << a_szName;
    // p->zmienne.push_back(new Zmienna(string(a_szName)));

   // mu::Parser::exception_type e;
    throw mu::Parser::exception_type("Zmienna \""+nazwa+"\" nie istnieje w zadnej funkcji kryterialnej");

    return NULL;

};

bool Problem::Dominuje(const Particle & a, const Particle & b) {
    //qDebug() << "Dominuje\n";
    if(a.il_ograniczen>0 && a.il_przek_ograniczen<b.il_przek_ograniczen)
    {
        return true;
        
    }
    else if(a.il_ograniczen>0 && a.il_przek_ograniczen>b.il_przek_ograniczen)
    {
        return false;
        
    }
    else
    {
        
    
    for (int i = 0; i < parseryFunkcji.size(); i++) {
        // qDebug() << b.fitness[i] << " " << fitness[i] << "\n";
        
        if (tab_minmax[i]) {//max
            if (b.fitness[i] >= a.fitness[i]) {

                return 0;
            }
        } else {//min
            if (b.fitness[i] <= a.fitness[i]) {

                return 0;
            }


        }

    }

    }
    return 1;

}
