#include <iostream>
#include <iomanip>
// #include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <math.h>  
// #include "./normdist.h"
using namespace std;



double random_uniform_0_1(void) // Odegaard
{
    return double(rand())/double(RAND_MAX); // this uses the C library random number generator. 
};

double random_normal(void) // Odegaard
{
    double U1, U2, V1, V2;
    double S=2;
    while (S>=1) {
	U1 = random_uniform_0_1();
	U2 = random_uniform_0_1();
	V1 = 2.0*U1-1.0;
	V2 = 2.0*U2-1.0;
	S = pow(V1,2)+pow(V2,2);
    };
    double X1=V1*sqrt((-2.0*log(S))/S);
    return X1;
};

int main (int argc, char* argv[])
{
	for(int i=0; i<20; i++)
		cout << random_normal()<<endl;
	// sscanf (argv[1], "%f", &expiration_time);
	// sscanf (argv[2], "%d", &no_of_divisions);
	// sscanf (argv[3], "%f", &risk_free_rate);
	// sscanf (argv[4], "%f", &volatility);
	// sscanf (argv[5], "%f", &initial_stock_price);
	// sscanf (argv[6], "%f", &strike_price);
	// sscanf (argv[7], "%f", &barrier_price);
	// sscanf (argv[8], "%d", &no_of_sampling_instants);
	
	// up_factor = exp(volatility*sqrt(expiration_time/((float) no_of_divisions)));
	// R = exp(risk_free_rate*expiration_time/((float) no_of_divisions));
	// uptick_prob = (R - (1/up_factor))/(up_factor-(1/up_factor));
	
	// cout << "European Down and Out Discrete Barrier Option Pricing" << endl;
	// cout << "Expiration Time (Years) = " << expiration_time << endl;
	// cout << "Number of Divisions = " << no_of_divisions << endl;
	// cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
	// cout << "Volatility (%age of stock value) = " << volatility*100 << endl;
	// cout << "Initial Stock Price = " << initial_stock_price << endl;
	// cout << "Strike Price = " << strike_price << endl;
	// cout << "Barrier Price = " << barrier_price << endl; 
	// cout << "Number of (uniformly spaced) Discrete Barrier-Samples = " << no_of_sampling_instants << endl;
	// cout << "--------------------------------------" << endl;
	// cout << "Up Factor = " << up_factor << endl;
	// cout << "Uptick Probability = " << uptick_prob << endl;
	// cout << "--------------------------------------" << endl;
	// cout << "Price of an European Down and Out Discrete Barrier Call Option = " << european_discrete_down_and_out_call_option() << endl;
	// cout << "Price of an European Down and Out Discrete Barrier Put Option = " << european_discrete_down_and_out_put_option() << endl;
	// cout << "--------------------------------------" << endl;
} 