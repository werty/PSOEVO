
#include "struktury.h"

int Solution::tmp;

  bool lessThenVectorInt(const QVector<int> &v1, const QVector<int> &v2) {

      
        for (int i = 0; i < v1.size(); i++) {
           // qDebug()<<v1[i]<<"<"<<v2[i]<<"\n";
            if (v1[i] < v2[i])
            {
                //qDebug()<<"true\n";
                return true;
            }
            else if((v1[i] > v2[i]))
            {
                
                return false;
            }


        }
      //  qDebug()<<"false\n";
        return false;

    };

    bool  lessThenHiperkostka(const Hiperkostka & h1, const Hiperkostka & h2) {
        return lessThenVectorInt(h1.polozenie, h2.polozenie);

    };
    
    
    
    bool equalVectorInt(const QVector<int> &v1, const QVector<int> &v2) {

      
        for (int i = 0; i < v1.size(); i++) {
           // qDebug()<<v1[i]<<"<"<<v2[i]<<"\n";
            if (v1[i] != v2[i])
            {
                //qDebug()<<"true\n";
                return false;
            }
           


        }
      //  qDebug()<<"false\n";
        return true;

    };

    bool  equalHiperkostka(const Hiperkostka & h1, const Hiperkostka & h2) {
        return equalVectorInt(h1.polozenie, h2.polozenie);

    };
    
    void joinHiperkostki(QVector<Hiperkostka> &h)
    {
        for (int i = 1; i < h.size(); i++) {
            if(equalHiperkostka(h[i],h[i-1]))
            {
                h[i-1].czasteczki+=h[i].czasteczki;
                h.erase(h.begin()+i);
                i--;
                
            }


    }

        
        
        
        
    }
    
    
    bool  lessThenSolution(const Solution * s1, const Solution * s2) {
        if(s1->przystosowaniePrzeskalowane[Solution::tmp]>s2->przystosowaniePrzeskalowane[Solution::tmp])
        {
            return true;
            
        }
        else
        {
            return false;
            
        }
    };
    
    
    bool CrowdedComparisonOperator(const Solution * s1, const Solution * s2)
    {
        if(s1->ranga<s2->ranga || (s1->ranga==s2->ranga && s1->zatloczenie>s2->zatloczenie))
        {
            return true;
            
        }
        
        return false;
    }