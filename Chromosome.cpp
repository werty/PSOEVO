#include "Chromosome.h"
#include "QDebug"



Chromosome::Chromosome()
{
    isChanged=true;
}

Chromosome::Chromosome(unsigned int numOfBits, bool inState)
{
    bits.resize(numOfBits);
    bits.fill(inState);
    isChanged=true;
}


const bool& Chromosome::operator [](unsigned int i)const
{
    return bits[i];
}

bool& Chromosome::operator [](unsigned int i)
{
    isChanged=true;
    return bits[i];

}

void Chromosome::Resize(unsigned int size)
{
    bits.resize(size);
}

QString Chromosome::GetString()
{
    QString strTmp;
    for(int i=bits.size()-1;i>=0;i--)
    {
        if(bits[i])
        {
            strTmp+="1";
        }
        else
        {
            strTmp+="0";
        }
    }
    return strTmp;
}

unsigned long Chromosome::GetValue()
{
    static QVector<unsigned long> vPowers=[](unsigned int numOfPowers)->QVector<unsigned long>{
            std::cout<<"lambda "<<numOfPowers<<"\n";
            QVector<unsigned long> vPowers;
            vPowers.resize(numOfPowers);
            vPowers[0]=1;
            for(unsigned int i=1;i<numOfPowers;i++)
            {
                 vPowers[i]=vPowers[i-1]*2;

            }
            return vPowers;
    }(bits.size());


if(!isChanged)
{
    isChanged=false;
    return value;
}

unsigned int valueTmp=0;
for(int i=0;i<bits.size();i++){
    if(bits[i])
    {
        valueTmp+=vPowers[i];
    }
}

return valueTmp;
}

double Chromosome::GetNormalisedValue()
{
    static unsigned int maks=qPow(2.0,(double)bits.size());


    qDebug()<<"maks "<<maks<<endl;

    //if(maks<=0)
   // {
        qDebug()<<"maks "<<maks<<endl;

    double    maksi=qPow((float)2.0,(float)bits.size());
        qDebug()<<"obliczony qPow z  "<<"2.0 i "<<bits.size()<<" ="<<maksi<<endl;
   // }
    qDebug()<<"bits "<<bits<<endl;
    qDebug()<<"GetValue() "<<GetValue()<<endl;
    qDebug()<<"maks "<<maksi<<endl;
    return (double)GetValue()/(double)maksi;
}
