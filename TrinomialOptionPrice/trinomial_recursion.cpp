 #include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <map>
#include <tuple>
using namespace std;


float u, pu, d, pd, risk_free_rate, strike_price;
float initial_stock_price, expiration_time, volatility, R;
int no_of_divisions;

float max(float a, float b) 
{
	return (b < a )? a:b;
}

float american_call(int k, int i, float price)
{
	static std::map<std::tuple<int, int>, float> memo;
    if (k == no_of_divisions)
		return max(0.0, (price-strike_price));
	else if (memo.find(make_tuple(k, i)) != memo.end())
	 	return memo[make_tuple(k, i)];	
    else
    {
    	float call_price =  max((price-strike_price), (pu*american_call(k+1, i+1, price*u)+(1-pu-pd)*american_call(k+1, i, price)+pd*american_call(k+1, i-1, price*d))/R);
		memo[make_tuple(k, i)] = call_price;
    	return call_price;

    }
    	
 }

float european_call(int k, int i, float price)
{
	static std::map<std::tuple<int, int>, float> memo;
    if (k == no_of_divisions)
		return max(0.0, (price-strike_price));
	else if (memo.find(make_tuple(k, i)) != memo.end())
	 	return memo[make_tuple(k, i)];	
    else
    {
    	float call_price =  (pu*european_call(k+1, i+1, price*u)+(1-pu-pd)*european_call(k+1, i, price)+pd*european_call(k+1, i-1, price*d))/R;
		memo[make_tuple(k, i)] = call_price;
    	return call_price;
    }
 }

float american_put(int k, int i, float price)
{
	static std::map<std::tuple<int, int>, float> memo;
    if (k == no_of_divisions)
		return max(0.0, -(price-strike_price));
	else if (memo.find(make_tuple(k, i)) != memo.end())
	 	return memo[make_tuple(k, i)];	
    else
    {
    	float put_price =  max(-(price-strike_price), (pu*american_put(k+1, i+1, price*u)+(1-pu-pd)*american_put(k+1, i, price)+pd*american_put(k+1, i-1, price*d))/R);
    	memo[make_tuple(k, i)] = put_price;
    	return put_price;
    }
}

float european_put(int k, int i, float price)
{
	static std::map<std::tuple<int, int>, float> memo;
    if (k == no_of_divisions)
		return max(0.0, -(price-strike_price));
	else if (memo.find(make_tuple(k, i)) != memo.end())
	 	return memo[make_tuple(k, i)];	
    else
    {
    	float put_price = (pu*european_put(k+1, i+1, price*u)+(1-pu-pd)*european_put(k+1, i, price)+pd*european_put(k+1, i-1, price*d))/R;
    	memo[make_tuple(k, i)] = put_price;
    	return put_price;
    }
}

int main (int argc, char* argv[])
{
	
	sscanf (argv[1], "%f", &expiration_time);
	sscanf (argv[2], "%d", &no_of_divisions);
	sscanf (argv[3], "%f", &risk_free_rate);
	sscanf (argv[4], "%f", &volatility);
	sscanf (argv[5], "%f", &initial_stock_price);
	sscanf (argv[6], "%f", &strike_price);
	
	u = exp(volatility*sqrt(2*expiration_time/((float) no_of_divisions)));
    d =1/u;
	R = exp(risk_free_rate*expiration_time/((float) no_of_divisions));
	pu = pow((sqrt(R) - sqrt(d))/(sqrt(u)-sqrt(d)), 2);
    pd = pow((sqrt(u)-sqrt(R))/(sqrt(u)-1/sqrt(u)),2);
    cout << "Recursive Trinomial American-Asian Option Pricing" << endl;
	cout << "Expiration Time (Years) = " << expiration_time << endl;
	cout << "Number of Divisions = " << no_of_divisions << endl;
	cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
	cout << "Volatility (%age of stock value) = " << volatility*100 << endl;
	cout << "Initial Stock Price = " << initial_stock_price << endl;
	cout << "Strike Price = " << strike_price << endl;
	cout << "--------------------------------------" << endl;
	cout << "Up Factor = " << u << endl;
	cout << "Uptick Probability = " <<pu << endl;
    cout << "Down Factor = " << d << endl;
	cout << "Downtick Probability = " <<pd << endl;

	float call_price, put_price;
	cout << "--------------------------------------" << endl;
	call_price = european_call(0, 0,initial_stock_price);
	cout << "Trinomial Price of an European Call Option = " << call_price << endl;
	put_price = european_put(0, 0, initial_stock_price);
	cout << "Trinomial Price of an European Put Option = " << put_price << endl;
	cout << "--------------------------------------" << endl;
	call_price = american_call(0, 0,initial_stock_price);
	cout << "Trinomial Price of an American Call Option = " << call_price << endl;
	put_price = american_put(0, 0, initial_stock_price);
	cout << "Trinomial Price of an American Put Option = " << put_price << endl;
	cout << "--------------------------------------" << endl;
    return 0;
}
