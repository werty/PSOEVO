/* 
 * File:   uzyteczne_funkcje.h
 * Author: werty
 *
 * Created on 19 sierpień 2011, 23:45
 */

#ifndef UZYTECZNE_FUNKCJE_H
#define	UZYTECZNE_FUNKCJE_H
#include <QMessageBox>
#include <QDebug>

#define DEBUG_MODE 0
//#define WITHDINFO

#ifdef WITHDINFO
#define DINFO (qDebug()<<__FILE__<<" at "<<__LINE__<<"\n");  
#else
#define DINFO fprintf(stderr, "%s:%d: \n", __FILE__, \
    __LINE__);
#endif
#define DEBUG(fmt, ...) \
        do { if (DEBUG_MODE) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#define TRACE qDebug()<<__FILE__<<" at "<<__LINE__;


void showWarning(const QString &msg);
double losuj(double min, double max);
int losuj(int min, int max);
template <class T>
void swapWithMove(T& obj1,T& obj2)
{
    T objTmp;
    objTmp=move(obj1);
    obj1=move(obj2);
    obj2=move(objTmp);
}
#endif	/* UZYTECZNE_FUNKCJE_H */
