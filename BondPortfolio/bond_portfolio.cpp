// Written by Prof. Sreenivas for IE523: Financial Computing

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

#include "lp_lib.h"

using namespace std;

const double ERROR = 1e-10;
int number_of_cash_flows;
vector <double> price_list;
vector <int> maturity_list;
vector <double> yield_to_maturity;
vector <double> duration;
vector <double> convexity;
double debt_obligation_amount;
double time_when_debt_is_due;
vector <double> percentage_of_cash_flow_to_meet_debt_obligation;


class in_file
{
public:
	int input_line_no;
	vector<vector<double>> v;
	vector<double> vv;
	void enumerate(char * const argv[])
	{
		input_line_no = 1;
		ifstream input(argv[1]);
		string line;
		while (getline(input, line))
			{
				stringstream ss(line);
		    double hlp;
		    while(ss >> hlp)
		    {
		        vv.push_back(hlp);
		    }
				v.push_back(vv);
				vv.clear();
				input_line_no ++;
			}
	}

	double traverse(int i, int j)
	{
		return (v.at(i)).at(j);
	}

	vector<double> cash_flow_vector(int cf_idx)
	{
		int cf_size = v.at(cf_idx).size();
		vector<double> cf;
		for (int i=2;i<cf_size;i++)
		{
			cf.push_back(v.at(cf_idx).at(i));
		}
		return cf;
	}

	// template<class T> vector<vector<T>> const &mat
	void print_vec()
	{
	    for (vector<double> row: v)
	    {
	        for (double val: row) {
	            cout << val << " ";
	        }
	        cout << endl;
	    }
	}
};


double f(vector <double> cash_flow, double price, int m, double rate)
{
	double temp_1 = price*pow((1+rate), m);
	double temp_2 = 0;
	for (int i=1; i<=m; i++)
	{
			temp_2 += cash_flow[i-1]*pow((1+rate), m-i);
	}
	return temp_1-temp_2;
}

double df(vector <double> cash_flow, double price, int m, double rate)
{
	double temp_1 = m*price*pow(1+rate, m-1);
	double temp_2 = 0;
	for (int i=1; i<=m-1; i++)
	{
			temp_2 += cash_flow[i-1]*(m-i)*pow((1+rate), m-i-1);
	}
	return temp_1-temp_2;
}

double Newton_Raphson(vector <double> cash_flow, double price, int maturity, double rate, double acc = ERROR, int iter_max = pow(10,6))
{
	double rn = rate;
	int iter = 0;
	while(abs(f(cash_flow, price, maturity, rn))>acc)
	{
			iter++;
			if(iter>iter_max)
			{
					cout<< "Max iterations reached";
					return rn;
			}
			if(df(cash_flow, price, maturity, rn)==0)
			{
					cout<<"Potential instability, quitting";
					return rn;
			}
			rn = rn -f(cash_flow, price, maturity, rn)/df(cash_flow, price, maturity, rn);
	}
	return rn;
}

double get_duration(vector <double> cash_flow, double price, int maturity, double rate)
{
	double duration = 0;
	for (int t=1; t<=maturity;t++)
	{
		duration += t*cash_flow[t-1]/pow(1+rate, t);
	}
	return duration/price;
}

double get_convexity(vector <double> cash_flow, double price, int maturity, double rate)
{
	double convexity = 0;
	for (int t=1; t<=maturity;t++)
	{
		convexity += t*(t+1)*cash_flow[t-1]/pow(1+rate, t+2);
	}
	return convexity/price;
}

// double present_value_of_debt()
// {
// 	// compute PV of future debt obligation
//     // using the average-value-of-the-YTMs
// }

void print_data(char *filename)
{
	cout << "Input File: " << filename << endl;
	cout << "We owe " << debt_obligation_amount << " in " << time_when_debt_is_due << " years" << endl;
	cout << "Number of Cash Flows: " << number_of_cash_flows << endl;
	for (int i = 0; i < number_of_cash_flows; i++)
	{
		cout << "---------------------------" << endl;
		cout << "Cash Flow #" << i+1 << endl;
		cout << "Price = " << price_list[i] << endl;
		cout << "Maturity = " << maturity_list[i] << endl;
		cout << "Yield to Maturity = " << yield_to_maturity[i] << endl;
		cout << "Duration = " << duration[i] << endl;
		cout << "Convexity = " << convexity[i] << endl;
		cout << "Percentage of Face Value that would meet the obligation = " <<
		percentage_of_cash_flow_to_meet_debt_obligation[i] << endl;
	}
	cout << "---------------------------" << endl;
}


void get_data(char* argv[])
{
	in_file input;
	vector<double> cf;
	input.enumerate(argv);
	input.print_vec();
	cout<<"----------------"<<endl;
	number_of_cash_flows = input.traverse(0, 0);
	debt_obligation_amount = input.traverse(number_of_cash_flows+1, 0);
	time_when_debt_is_due = input.traverse(number_of_cash_flows+1, 1);
	input.cash_flow_vector(1);
	for(int i =1; i<=number_of_cash_flows; i++)
	{
		price_list.push_back(input.traverse(i, 0));
		maturity_list.push_back((int)input.traverse(i, 1));
		cf = input.cash_flow_vector(i);
		yield_to_maturity.push_back(Newton_Raphson(cf, price_list[i-1], maturity_list[i-1], 0.01 ));
		duration.push_back(get_duration(cf, price_list[i-1], maturity_list[i-1], yield_to_maturity[i-1]));
		convexity.push_back(get_convexity(cf, price_list[i-1], maturity_list[i-1], yield_to_maturity[i-1]));
		percentage_of_cash_flow_to_meet_debt_obligation.push_back(cf.back()/price_list[i-1]);
	}

}

void get_optimal_portfolio()
{
	//modelled on http://lpsolve.sourceforge.net/5.5/index.htm -- Formulation of an lp problem in lpsolve
	lprec *lp;
	int n_col = number_of_cash_flows; // Number of variables in LP
	REAL row[n_col+1];     /* must be 1 more than number of columns ! */
  int colno[n_col];

	lp = make_lp(0, n_col);
	if(lp == NULL)
	{
    fprintf(stderr, "Unable to create new LP model\n");
  }
	set_add_rowmode(lp, TRUE);
	for(int i=1; i<=n_col; i++)  // \sum_i\lambda_i=1
	{
		colno[i-1] = i;
		row[i-1] = 1.0;
	}
	add_constraintex(lp, n_col, row, colno, EQ, 1.0);
	for(int i=1; i<=n_col; i++)  // \sum_i\lambda_iD_i=Debt duration
	{
		colno[i-1] = i;
		row[i-1] = duration[i-1];
	}
	add_constraintex(lp, n_col, row, colno, EQ, time_when_debt_is_due);

	for(int i=1; i<=n_col; i++) // Long only
	{
		for (int j=0; j<=n_col; j++){row[j] = 0;}
		colno[i-1] = i;
		row[i-1] = 1;
		add_constraintex(lp, n_col, row, colno, GE, 0);
	}
	set_add_rowmode(lp, FALSE);
	for(int i=1; i<=n_col; i++ ) // Maximize convexity
	{
		colno[i-1] = i;
		row[i-1] = convexity[i-1];
	}
	set_obj_fnex(lp, n_col, row, colno);
	set_maxim(lp);
	write_LP(lp, stdout);
	set_verbose(lp, IMPORTANT);
	int ret = solve(lp);
  if(ret == OPTIMAL)
	{
		printf("Largest convexity: %f\n", get_objective(lp));
		get_variables(lp, row);
		cout << "-------------------------------" << endl;
		printf("Optimal bond portfolio weights\n");
		cout << "-------------------------------" << endl;
		cout << "For each $1 of PV owed, buy"<<endl;
		for(int j = 0; j < n_col; j++)
			printf("cash flow %i: %f\n", j+1, row[j]);
	}
	else
	{
		cout<<"No portfolio that meats duration constraint..."<<endl;
	}

}

int main (int argc, char* argv[])
{
	if (argc == 1) {
		cout << "Input filename missing" << endl;
	}
	else
	{
		get_data(argv);
		print_data(argv[1]);
		get_optimal_portfolio();
	}
	return (0);
}
