/*
 *  alice_and_bob.h
 *  Loosing as little as possible
 *
 *  Created by Ramavarapu Sreenivas on 9/2/12.
 *  Copyright 2012 University of Illinois at Urbana-Champaign. All rights reserved.
 *
 */
#ifndef ALICE_AND_BOB
#define ALICE_AND_BOB

#include <cmath>
#include <map>
#include <cstdlib> 
#include <fstream>


using namespace std;

class I_have_nothing_apropos_for_this_class
{
private:
	double alice_probability, bob_probability;
	
	// private member function: uniform RV generator
	double get_uniform()
	{
		return (double)rand()/RAND_MAX;
	}
	
	// private member function: nCi (i.e. n-take-i) 
	int take(int n, int q)
	{
		static std::map<std::pair<int, int>, int> memo;
		if (q == 0 || n == q)
	    	return 1;
	 	else if (memo.find(make_pair(n, q)) != memo.end())
	 		return memo[make_pair(n, q)];
	  	else 
	  	{
	    	int entry =  take(n - 1,q)+take(n - 1,q - 1);
	     	memo[make_pair(n, q)] = entry;
	    	return entry;
	  	}
	}
	
	
	// this routine implements the probability that Alice has more 
	// heads than Bob after n-many coin tosses
double theoretical_value(double q, double p, int n)
{
    double sum = 0;
    for (int r=0; r<=n;r++)
    {
       for (int s=r+1; s<=n; s++) 
       {
        sum += take(n, r)*pow(p, r)*pow(1-p,n-r)*take(n, s)*pow(q, s)*pow(1-q,n-s);
       }
    }
	return sum;
}


public: 
	// public function: 
	void set_probability(double alice_p, double bob_p)
	{
		alice_probability = alice_p;
		bob_probability = bob_p;
	}
	
	// probability of Alice winning the game.
	double simulated_value(int number_of_coin_tosses_in_each_game, int no_of_trials)
	{
		int no_of_wins_for_alice = 0;
		for (int i = 0; i < no_of_trials; i++) 
		{
			int number_of_heads_for_alice = 0;
			int number_of_heads_for_bob = 0;
			for (int j = 0; j < number_of_coin_tosses_in_each_game; j++) 
			{
				if (get_uniform() < alice_probability) 
					number_of_heads_for_alice++;
				if (get_uniform() < bob_probability)
					number_of_heads_for_bob++;
			}
			if (number_of_heads_for_alice > number_of_heads_for_bob)
				no_of_wins_for_alice++;
		}
		return (((double) no_of_wins_for_alice)/((double) no_of_trials));
	}
		
	int search_result()
	{
		ofstream file("sim_results.dat");
		int running_arg_max = 0;
		double running_max = 0;
		int no_of_trials = 5*pow(10, 5);
		for (int n=0; n<=75; n++)
		{
			double val = simulated_value(n, no_of_trials);
			file << val << ",";
			if (val>running_max)
			{
				running_max = val;
				running_arg_max = n;
			}
		}
		file.close();
		cout << endl;
		return running_arg_max;
	}

	int combinatoric_result()
	{
		ofstream file("comb_results.dat");
		int running_arg_max = 0;
		double running_max = 0;
		for (int n = 0; n <= 30; n++)
		{
			double val = theoretical_value(alice_probability, bob_probability, n);
			file << val << ",";
			if (val > running_max)
			{
				running_max = val;
				running_arg_max = n;
			}
		}
		file.close();
		cout << endl;
		return running_arg_max;
	}
};
#endif









