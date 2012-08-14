//#include <qt4/QtCore/qglobal.h>

#include "uzyteczne_funkcje.h"




double losuj(double min,double max)
{
   return min+(double)qrand()/(double)RAND_MAX*(max-min);
    
    
}
int losuj(int min,int max)
{
    return min+(int)((double)rand()/(double)RAND_MAX*(double)(max-min+1));
    
    
}
//template <class T>
//QDebug operator<<(QDebug dbg, const QVector<T> &c)
// {
//    dbg.nospace() << "(";
//    for (int i = 0; i < c.size(); ++i) {


//        dbg.nospace()<< c[0] << ", "
//    }

//     dbg.nospace()<< ")";


//     return dbg.space();
// }

