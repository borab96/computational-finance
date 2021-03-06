// IE523: Financial Computation
// "How to lose as little as possible" by Addona, Wagon and Wilf
// Written by Prof. Sreenivas
// 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include "min_loss.h"
using namespace std;
	
int main (int argc, char* argv[])
{
	I_have_nothing_apropos_for_this_class x;
	double alice_success_prob, bob_success_prob;
	sscanf (argv[1], "%lf", &alice_success_prob);
	sscanf (argv[2], "%lf", &bob_success_prob);
	
	cout << "Probability of success for Alice = " << alice_success_prob << endl;
	cout << "Probability of success for Bob = " << bob_success_prob << endl;
	
	x.set_probability(alice_success_prob, bob_success_prob);


	
	int optimal = x.search_result();
	int optimal_comb = x.combinatoric_result();
	if (optimal > 0)
	{
		cout << "The optimal number of coin tosses in each game is " << optimal << endl;
		cout << "The combinatoric optimal number of coin tosses is " << optimal_comb << endl;
	}
		
	else {
		cout << "The optimal number of coin tosses in each game exceeds 100... Quitting" << endl;
	}
}
	
	
	
