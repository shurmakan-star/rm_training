#include"Angle.hpp"
#include<cmath>
#include<iostream>
using namespace std;

Angle::Angle(double init_v0,double grav)
{
    v0=init_v0;
    g=grav;
}

double Angle::solver(double d)
{
    double sin_2theta=(g*d)/(v0*v0);
     if(sin_2theta>1)
     {
        cout<<"无解"<<endl;
        return -1;
     }

     double theta2=asin(sin_2theta)/2;
     return theta2;
}