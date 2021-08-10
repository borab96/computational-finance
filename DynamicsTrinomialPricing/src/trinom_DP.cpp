// Calculating the price of an European Option using Dynamic 
// Programming
// Written by Prof. Sreenivas for IE523: Financial Computing

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "newmatap.h"                // need matrix applications
#include "newmatio.h"   
#include "normdist.h"          // this defines the normal distribution from Odegaard's files
using namespace std;

float uptick_prob, downtick_prob, risk_free_rate, strike_price, u, d;
float initial_stock_price, expiration_time, volatility, R;
int no_of_divisions;

float max(float a, float b) {
	return (b < a) ? a : b;
}

Matrix repeated_squaring(Matrix A, int exponent, int no_rows)
{
	if (exponent == 0) {
		IdentityMatrix I(no_rows);
		return (I);
	}
	else if (exponent % 2 == 0)
		return (repeated_squaring(A * A, exponent / 2, no_rows));
	else
		return (A * repeated_squaring(A * A, (exponent - 1) / 2, no_rows));
}

double option_price_put_black_scholes(const double& S,      // spot price
	const double& K,      // Strike (exercise) price,
	const double& r,      // interest rate
	const double& sigma,  // volatility
	const double& time)
{
	double time_sqrt = sqrt(time);
	double d1 = (log(S / K) + r * time) / (sigma * time_sqrt) + 0.5 * sigma * time_sqrt;
	double d2 = d1 - (sigma * time_sqrt);
	return K * exp(-r * time) * N(-d2) - S * N(-d1);
};

double option_price_call_black_scholes(const double& S,       // spot (underlying) price
	const double& K,       // strike (exercise) price,
	const double& r,       // interest rate
	const double& sigma,   // volatility 
	const double& time)	  // time to maturity 
{
	double time_sqrt = sqrt(time);
	double d1 = (log(S / K) + r * time) / (sigma * time_sqrt) + 0.5 * sigma * time_sqrt;
	double d2 = d1 - (sigma * time_sqrt);
	return S * N(d1) - K * exp(-r * time) * N(d2);
};

double N(const double& z) {
	if (z > 6.0) { return 1.0; }; // this guards against overflow 
	if (z < -6.0) { return 0.0; };
	double b1 = 0.31938153;
	double b2 = -0.356563782;
	double b3 = 1.781477937;
	double b4 = -1.821255978;
	double b5 = 1.330274429;
	double p = 0.2316419;
	double c2 = 0.3989423;
	double a = fabs(z);
	double t = 1.0 / (1.0 + a * p);
	double b = c2 * exp((-z) * (z / 2.0));
	double n = ((((b5 * t + b4) * t + b3) * t + b2) * t + b1) * t;
	n = 1.0 - b * n;
	if (z < 0.0) n = 1.0 - n;
	return n;
};

float american_call_option_dyn_prog()
{
	// create the probability matrix
	Matrix transition_probability(2 * no_of_divisions - 1, 2 * no_of_divisions - 1);

	for (int i = 1; i <= 2 * no_of_divisions - 1; i++)
		for (int j = 1; j <= 2 * no_of_divisions - 1; j++)
			transition_probability(i, j) = 0.0;

	// boundary values of the probabilities need to be entered
	transition_probability(1, 1) = 1 - downtick_prob;
	transition_probability(1, 2) = uptick_prob;
	transition_probability(2 * no_of_divisions - 1, 2 * no_of_divisions - 1) = 1 - downtick_prob;
	transition_probability(2 * no_of_divisions - 1, 2 * no_of_divisions - 2) = downtick_prob;


	for (int i = 2; i <= 2 * no_of_divisions - 2; i++)
		for (int j = 1; j <= 2 * no_of_divisions - 1; j++) {
			if (j == (i - 1))
				transition_probability(i, j) = downtick_prob;
			if (j == (i + 1))
				transition_probability(i, j) = uptick_prob;
			if (j == i)
				transition_probability(i, i) = 1 - downtick_prob - uptick_prob;
		}

	// compute pi^no_of_divisions by method of repeated squaring
	// keep in mind that we are computing Pi^(T-1) not Pi^T 
	// because the PI matrix in itself represents the 1-step state
	// transition probability... for k-step you need to multiply PI
	// with itself (k-1) times 
	
	//final_matrix = repeated_squaring(transition_probability, no_of_divisions - 1, 2 * no_of_divisions - 1);

	// value at expiration
	Matrix V_T(2 * no_of_divisions - 1, 1);
	for (int i = 1; i <= 2 * no_of_divisions - 1; i++)
		V_T(i, 1) = (1.0 / pow(R, no_of_divisions))*max(0.0, (initial_stock_price * pow(u, i - no_of_divisions-0)) - strike_price);

	Matrix pVT(2 * no_of_divisions - 1, 1);
	Matrix V_t(2 * no_of_divisions - 1, 1);
	for (int i = no_of_divisions; i >=0; i--)
	{
		pVT = transition_probability * V_T;
		for (int j = 1; j <= 2 * no_of_divisions - 1; j++)
		{
			V_t(j, 1) =  max(initial_stock_price * pow(u, j-no_of_divisions ) - strike_price, (1.0 / R) * pVT(j, 1));
		}
		V_T = V_t;

	}

	return (V_t(no_of_divisions  , 1));
}

float american_put_option_dyn_prog()
{
	// create the probability matrix
	Matrix transition_probability(2 * no_of_divisions - 1, 2 * no_of_divisions - 1);

	for (int i = 1; i <= 2 * no_of_divisions - 1; i++)
		for (int j = 1; j <= 2 * no_of_divisions - 1; j++)
			transition_probability(i, j) = 0.0;

	// boundary values of the probabilities need to be entered
	transition_probability(1, 1) = 1 - downtick_prob;
	transition_probability(1, 2) = uptick_prob;
	transition_probability(2 * no_of_divisions - 1, 2 * no_of_divisions - 1) = 1 - downtick_prob;
	transition_probability(2 * no_of_divisions - 1, 2 * no_of_divisions - 2) = downtick_prob;


	for (int i = 2; i <= 2 * no_of_divisions - 2; i++)
		for (int j = 1; j <= 2 * no_of_divisions - 1; j++) {
			if (j == (i - 1))
				transition_probability(i, j) = downtick_prob;
			if (j == (i + 1))
				transition_probability(i, j) = uptick_prob;
			if (j == i)
				transition_probability(i, i) = 1 - downtick_prob - uptick_prob;
		}

	// compute pi^no_of_divisions by method of repeated squaring
	// keep in mind that we are computing Pi^(T-1) not Pi^T 
	// because the PI matrix in itself represents the 1-step state
	// transition probability... for k-step you need to multiply PI
	// with itself (k-1) times 

	//final_matrix = repeated_squaring(transition_probability, no_of_divisions - 1, 2 * no_of_divisions - 1);

	// value at expiration
	Matrix V_T(2 * no_of_divisions - 1, 1);
	for (int i = 1; i <= 2 * no_of_divisions - 1; i++)
		V_T(i, 1) = (1.0 / pow(R, no_of_divisions)) * max(0.0, -(initial_stock_price * pow(u, i - no_of_divisions - 0)) - strike_price);

	Matrix pVT(2 * no_of_divisions - 1, 1);
	Matrix V_t(2 * no_of_divisions - 1, 1);
	for (int i = no_of_divisions; i >= 0; i--)
	{
		pVT = transition_probability * V_T;
		for (int j = 1; j <= 2 * no_of_divisions - 1; j++)
		{
			V_t(j, 1) = max(-initial_stock_price * pow(u, j - no_of_divisions) + strike_price, (1.0 / R) * pVT(j, 1));
		}
		V_T = V_t;

	}

	return (V_t(no_of_divisions, 1));
}

int main(int argc, char* argv[])
{

	sscanf(argv[1], "%f", &expiration_time);
	sscanf(argv[2], "%d", &no_of_divisions);
	sscanf(argv[3], "%f", &risk_free_rate);
	sscanf(argv[4], "%f", &volatility);
	sscanf(argv[5], "%f", &initial_stock_price);
	sscanf(argv[6], "%f", &strike_price);

	u = exp(volatility * sqrt(2 * expiration_time / ((float)no_of_divisions)));
	d = 1 / u;
	R = exp(risk_free_rate * expiration_time / ((float)no_of_divisions));
	uptick_prob = pow((sqrt(R) - sqrt(d)) / (sqrt(u) - sqrt(d)), 2);
	downtick_prob = pow((sqrt(u) - sqrt(R)) / (sqrt(u) - 1 / sqrt(u)), 2);


	cout << "European Put Option Pricing by Dynamic Programming" << endl;
	cout << "Expiration Time (Years) = " << expiration_time << endl;
	cout << "Number of Divisions = " << no_of_divisions << endl;
	cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
	cout << "Volatility (%age of stock value) = " << volatility * 100 << endl;
	cout << "Initial Stock Price = " << initial_stock_price << endl;
	cout << "Strike Price = " << strike_price << endl;
	cout << "--------------------------------------" << endl;
	cout << "R = " << R << endl;
	cout << "Up Factor = " << u << endl;
	cout << "Down Factor = " << d << endl;
	cout << "Uptick Probability = " << uptick_prob << endl;
	cout << "Downtick Probability = " << downtick_prob << endl;
	cout << "Notick Probability = " << 1 - downtick_prob - uptick_prob << endl;
	cout << "--------------------------------------" << endl;
	cout << "Price of an American Call Option = " << american_call_option_dyn_prog() << endl;
	//cout << "Call Price according to Black-Scholes = " <<
	//	option_price_call_black_scholes(initial_stock_price, strike_price, risk_free_rate, volatility, expiration_time) << endl;
	
	cout << "Price of an American Put Option = " << american_put_option_dyn_prog() << endl;
	//cout << "Put Price according to Black-Scholes = " <<
	//	option_price_put_black_scholes(initial_stock_price, strike_price, risk_free_rate, volatility, expiration_time) << endl;
	cout << "--------------------------------------" << endl;
}

