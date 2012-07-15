
#include "Particle.h"

Particle::Particle(int il_zmiennych, int il_funkcji, int il_ograniczen) {
    this -> zdominowana = false;
    this -> il_zmiennych = il_zmiennych;
    this -> il_funkcji = il_funkcji;
    this -> il_ograniczen = il_ograniczen;
    v.resize(il_zmiennych);
    x.resize(il_zmiennych);
    pbest_pos.resize(il_zmiennych);
    fitness_pbest.resize(il_funkcji);
    wartFunkcjiKryterialnych.resize(il_funkcji);
    przystosowanie.resize(il_funkcji);
    przystosowaniePrzeskalowane.resize(il_funkcji);
}

Particle::Particle(double * v_tmp, double * x_pos_tmp) {
    v[0] = v_tmp[0];
    v[1] = v_tmp[1];
    x[0] = x_pos_tmp[0];
    x[1] = x_pos_tmp[1];
}

Particle::Particle(const Particle & oryg) {
    int il_zmiennych = oryg.il_zmiennych;
    int il_funkcji = oryg.il_funkcji;

    this -> zdominowana = false;
    this -> il_zmiennych = il_zmiennych;
    this -> il_funkcji = il_funkcji;
    this -> il_ograniczen = oryg.il_ograniczen;
    this -> il_przek_ograniczen = oryg.il_przek_ograniczen;
    v=oryg.v;
    x=oryg.x;
    pbest_pos=oryg.pbest_pos;
    fitness_pbest=oryg.fitness_pbest;
    wartFunkcjiKryterialnych=oryg.wartFunkcjiKryterialnych;
    przystosowanie=oryg.przystosowanie;
    przystosowaniePrzeskalowane=oryg.przystosowaniePrzeskalowane;

//    v = new double[il_zmiennych];
//    x = new double[il_zmiennych];
//    pbest_pos = new double[il_zmiennych];
//    fitness_pbest = new double[il_funkcji];
//    fitness = new double[il_funkcji];

//    for (int i = 0; i < il_zmiennych; i++) {
//        v[i] = orig.v[i];
//        x[i] = orig.x[i];
//        pbest_pos[i] = orig.pbest_pos[i];
//    }

//    for (int i = 0; i < il_funkcji; i++) {
//        fitness_pbest[i] = orig.fitness_pbest[i];
//        fitness[i] = orig.fitness[i];
//    }
}

// //operator dominacji
// bool Particle::operator>(const Particle & b) {
// //qDebug() << "operator>\n";
// for (int i = 0; i < il_funkcji; i++) {
// // qDebug() << b.fitness[i] << " " << fitness[i] << "\n";
// if (b.fitness[i] >= fitness[i]) {
//
// return 0;
// }
//
// }
//
// return 1;
// }
// //operator dominacji
// bool Particle::operator<(const Particle & b) {
// //qDebug() << "operator>\n";
// for (int i = 0; i < il_funkcji; i++) {
// // qDebug() << b.fitness[i] << " " << fitness[i] << "\n";
// if (b.fitness[i] <= fitness[i]) {
//
// return 0;
// }
//
// }
//
// return 1;
// }

Particle & Particle::operator =(const Particle & oryg) {
    il_zmiennych = oryg.il_zmiennych;
    il_funkcji = oryg.il_funkcji;

    v=oryg.v;
    x=oryg.x;
    pbest_pos=oryg.pbest_pos;
    fitness_pbest=oryg.fitness_pbest;
    wartFunkcjiKryterialnych=oryg.wartFunkcjiKryterialnych;
    przystosowanie=oryg.przystosowanie;
    przystosowaniePrzeskalowane=oryg.przystosowaniePrzeskalowane;

    return *this;

}
