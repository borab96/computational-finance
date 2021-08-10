
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include<numeric>
#include<map>

using namespace std;

template<typename F, typename DF>
double newton_raphson(double r0, F f, DF df, double acc=0.0001, int iter_max=pow(10, 6))
{
    double rn = r0;
    int iter = 0;
    while(abs(f(rn))>acc)
    {
        iter++;
        if(iter>iter_max)
        {
            cout<< "Max iterations reached";
            exit;
        }
        if(df(rn)==0)
        {
            cout<<"Potential instability, quitting";
            exit;
        }
        rn = rn -f(rn)/df(rn); 
    }
    return rn;
}



int main(int argc, char* argv[])
{
    double vb = 50;
    const int m = 10;
    double p[m] = {3} ; 
    
    auto f = [=] (double x) -> double 
    {
        double temp_1 = vb*pow((1+x), m);
        double temp_2 = 0;
        for (int i=1; i<=m; i++)
        {
            temp_2 += p[i-1]*pow((1+x), m-i);
        }
        return temp_1-temp_2;
    };
   auto df = [=] (double x) -> double 
    {
        double temp_1 = m*vb*pow(1+x, m-1);
        double temp_2 = 0;
        for (int i=1; i<=m-1; i++)
        {
            temp_2 += p[i-1]*(m-i)*pow((1+x), m-i-1);
        }
        return temp_1-temp_2;
    };
    cout << newton_raphson(.2, f, df)<<endl;
    return 0;
}