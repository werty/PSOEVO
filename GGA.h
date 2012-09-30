#ifndef GGA_H
#define GGA_H
#include <QTime>
#include <QThread>
#include <QMutex>
#include <QList>
#include <QDebug>

#include "Problem.h"
#include "Wskazniki.h"

/**
 * @brief
 *
 */
class GGA  : public QThread {
    Q_OBJECT
public:

/**
 * @brief
 *
 * @param parent
 */
    GGA(QObject *parent = 0);
/**
 * @brief
 *
 * @param wielk_pop
 * @param il_bitow
 * @param prawd_mutacji
 * @param prawd_krzyz
 * @param nazwyRodzajnikow
 * @param funkcjeRodzajnikow
 * @param problem
 */
    GGA(unsigned int wielk_pop,unsigned  int il_bitow,double prawd_mutacji,double prawd_krzyz,unsigned int ilOsobElit, QVector<QString> nazwyRodzajnikow, QVector<QVector<unsigned int> > funkcjeRodzajnikow, Problem * problem);
/**
 * @brief
 *
 * @param orig
 */
    GGA(const GGA & orig);
    /**
     * @brief
     *
     */
    virtual ~GGA();

    int il_bitow, wiel_pop, il_iter, num_iter; /**< TODO */
    unsigned int ilOsElitarnych;//ile osobnikow z najwyzszym GOL przechodzi do nastepnego pokolenia.Czy jest elitaryzm
    unsigned int ilOsNieElit;
    double prawd_mutacji, prawd_krzyz, c; /**< TODO */
    bool zainicjalizowany;
    /**
     * @brief rodzice l,l,l,l
     */
    QVector<Solution*> rodzice, rodzenstwo; /**< TODO */
    QVector<QVector<unsigned int> > solutionsInGender;//wektory rozwiazan ktore maja nadane ity rodzajnik /**< TODO */
    QVector<QVector<unsigned int> > rangiSuboptymalnosci;//zawiera dla kazdego rozwiazania wektor rang suboptymalnosci /**< TODO */
    QVector<QVector<float> > znormRangiSuboptymalnosci;//zawiera dla kazdego rozwiazania wektor znormalizowanych rang suboptymalnosci /**< TODO */
    QVector<float> maksZnormRangiSuboptymalnosci;
    QVector<QVector<unsigned int> > stopZdominowaniaSubopt;// /**< TODO */ /**< TODO */
    QVector<unsigned int > stopZdominowania;
    QVector<QString> nazwyRodzajnikow; /**< TODO */





    QVector<unsigned int> indSortGOL;
    void GOL(QVector<unsigned int> &indSortGOL);
    /**
     * @brief funkcjeRodzajnikow kmkmkmkmkmkmkmkmkm
     */
    QVector<QVector<unsigned int> > funkcjeRodzajnikow;//dla kazdego rodzajnika lista przydzielonych funkcji kryterialnych /**< TODO */
    QVector< int> przydzieloneRodzajniki;//indeksy rodzajnikow przydzielone kazdemu rozwiazaniu /**< TODO */
    QVector<QVector<unsigned int> > rozwPrzydzRodzajnikom;//dla kazdego rodzajnika lista przydzielonych rozwiazan (ich indeksy) /**< TODO */
    QVector<QVector<Solution*> >* F; /**< TODO */
  //  QVector<QString> genderNames; /**< TODO */
  //  QVector<QVector<unsigned int> > genderFunctions; /**< TODO */
    double zakres; /**< TODO */
    double RozpFrontParWarFunKryt();
    Problem *problem; /**< TODO */
    bool stopIteration; /**< TODO */
    QMutex  mutex; /**< TODO */
    Wskazniki wskazniki;




    void StworzNowaPopulacje();
    void Iteruj();
    /**
     * @brief
     *
     */
    void Inicjalizuj();
    /**
     * @brief
     *
     */
    void DekodujGenotyp();
    /**
     * @brief
     *
     */
    void WyznaczWartFunkcjiKryterialnych();
    /**
     * @brief
     *
     */
    QVector< QVector<Solution*> > * FastNonDominatedSort();

    void WyznaczPrzystosowanie();
    /**
     * @brief
     *
     */
    void WyznaczStopnieZdominowania();
    /**
     * @brief WybMetSDR wyselekcjonowanie N osobnikow z puli
     *        rodzajnikowej za pomoca metody stochastycznego doboru resztowego
     */
    void WybMetSDR(unsigned int numRodzajnika,unsigned int ilRozw,vector<unsigned int>& wysRozwiazania);

    //porownujemy operatorem <= zeby miec posortowane od naj do najm
    bool porownajZnormRangiSubopt(int a,int b,int indRodzajnika);

    void WyznaczRangi();
    /**
     * @brief
     *
     */
    void PrzeskalujPrzystosowanie();
    /**
     * @brief
     *
     */
    void WyswietlPrzystosowanie();
    /**
     * @brief
     *
     * @param P
     */
    void WyswietlPrzystosowaniePrzeskalowane(QVector<Solution*>& P);

    bool porownajWartGOL(int ind1, int ind2);
    void Mutuj(Solution *S);
    void Krzyzuj(Solution *S1, Solution *S2);
protected:
    /**
     * @brief
     *
     */
    void run();
signals:
    /**
     * @brief
     *
     */
    void done();
    /**
     * @brief
     *
     */
    void finished();
public slots:
    /**
     * @brief
     *
     */
    void doTheWork();
    /**
     * @brief
     *
     */
    void stop();
    /**
     * @brief
     *
     * @param num_iter
     */
    void setNum_iter(int num_iter);
    /**
     * @brief
     *
     * @return int
     */
    int getNum_iter() const;
    void sprawdz_ograniczenia();

};

#endif // GGA_H
