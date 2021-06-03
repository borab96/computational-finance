
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include<numeric>
#include<map>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::make_pair;

class Gamble 
{
    private:
        void print_deck()
            {
                for(int i=0; i<deck_size; i++)
                {
                    cout<<deck[i]<<" ";
                }
                cout<<endl;
            }

             void shuffle_deck(unsigned seed = 42)
        {
            if(seed == 42)
                seed = std::chrono::system_clock::now().time_since_epoch().count();
            shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
        }
        
        int round()
        {
            int top_deck = deck.back();
            deck.pop_back();
            num_red = std::accumulate(deck.begin(),deck.end(),0);
            num_black = deck.size()-num_red;
            actual_value = num_red - num_black;
            player_deck.push_back(top_deck);
            winnings =  std::accumulate(player_deck.begin(),player_deck.end(),0);
            return top_deck;
        }

        double value(int nr, int nb)
        {
            static std::map<std::pair<int, int>, double> memo;
            if(nr==0)
                return 0;
            else if(nb==0)
                return nr;
            if (memo.find(make_pair(nr, nb)) != memo.end())
                return (double)memo[make_pair(nr, nb)];
            double expect = 0.5*value(nr-1, nb)+0.5*value(nr, nb-1);
            
            if(expect>(double)actual_value)
            {
                memo[make_pair(nr, nb)] = expect;
                return expect;
            }
            else
            {
                memo[make_pair(nr, nb)] = double(actual_value);
                return double(actual_value);
            }          
        }

    public:
        int deck_size, num_red, num_black, actual_value, winnings;
        vector<int> deck, player_deck;
        void build_deck(int num, unsigned seed=42)
        {   
            deck_size = num;
            if(deck_size%2)
            {
                cout<<"+1 deck size to make it even"<<endl;
                deck_size++;
            }
            cout<<""<<endl;
            cout<<"Total number of cards: "<<deck_size<<endl;
            cout<<"--------------------------------------"<<endl;
            deck.resize(deck_size);
            num_black = deck_size/2;
            num_red = num_black;
            for(int i=0; i<deck_size/2; i++)
            {
                deck[i] = 1;
            }
            shuffle_deck(seed);
        }
        

        double play(int n, bool verbose=false)
        {
            build_deck(n);
            round();
            double val = 1;
            while(val>0)
            {   
                val = value(num_red, num_black);
                if(verbose)
                {
                    log(val);
                    print_deck();
                }
                if(val>winnings)
                    round();
                else
                    return val;
            }
            return 0;
        }

        void log(double v=0)
        {
            cout<< "Num_red: " << num_red << endl;
            cout<< "Num_black: " << num_black << endl;
            if(v)
                cout<< "Val: " << v << endl;
        }  
};

int main(int argc, char* argv[])
{
    Gamble game; 
    double price = game.play(std::atoi(argv[1]));
    cout<< "arbitrage-free price of game: "<<price<<endl<<endl;
    return 0;
}
