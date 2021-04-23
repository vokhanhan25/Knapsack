#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <bits/stdc++.h>
using namespace std;

int myrandom (int i) { return std::rand()%i;}

vector<int> values = {60, 100, 120};

vector<int> weights {10, 20, 30};
int capacities = 50;


void Generate(vector<vector<int>> &a, const int &m, const int &n) {
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            double randNum = (float) rand()/RAND_MAX;
            if (randNum >= 0.5)
                a[i][j] = 1;
            else a[i][j] = 0;
        }
}

void Print(const vector<vector<int>> &a, const int &m, const int &n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            cout << a[i][j] << " ";
        cout << '\n';
    } 
}

vector<vector<int>> OnepointCrossover(vector<vector<int>> a, const int &m, const int &n) {
    vector<vector<int>> b = a;
    int i = 0;
    while (i < m - 1) {
        int randNum = rand() % ((n - 1) - 1 + 1) + 1;
        // cout << randNum << "\n";
        for (int j = randNum; j < n; j++) {
            int tm = b[i][j];
            b[i][j] = b[i + 1][j];
            b[i + 1][j] = tm;
        }
        i += 2;
    }
    return b;
}

vector<vector<int>> Pool(vector<vector<int>> a, vector<vector<int>> b) {
    vector<vector<int>> tm = a;
    tm.insert( tm.end(), b.begin(), b.end() );
    return tm;
}

int ValueMax(vector<int> a, int n) {
    int value = 0, weight = 0;

    for (int i = 0; i < n; i++) 
        if (a[i] == 1) {
            value += values[i];
            weight += weights[i];
        }
    if (weight <= capacities)
        return value;
    return 0;
}
void TournamentSelection(vector<vector<int>> &parents, vector<int> &fitness_parents, vector<vector<int>> pool, vector<int> fitness_pool, const int &m) {
    parents.clear();
    parents.shrink_to_fit();
    fitness_parents.clear();
    fitness_parents.shrink_to_fit();
    for (int k = 0; k < 2; k++) {
        vector<int> si(2 * m);
        iota(si.begin(), si.end(), 1);
        srand(unsigned(time(0)));
        random_shuffle (si.begin(),si.end());
        random_shuffle (si.begin(),si.end(), myrandom);
        int max_fitness = 0;
        int max_index = -1;

        for (int i = 0; i < 2 * m; i++)
            if (i % 4 == 3) {
                parents.push_back(pool[max_index]);
                fitness_parents.push_back(fitness_pool[max_index]);
                max_fitness = 0;
                max_index = -1;
            }
            else if (fitness_pool[si[i]] >= max_fitness) {
                max_fitness = fitness_pool[si[i]];
                max_index = si[i];
            }
    }
}

int main() {
    int populationSize = 6, individualSize = values.size();
    vector<vector<int>> parents(populationSize, vector<int>(individualSize , 0));

    vector<int> fitness_offspring, fitness_pool, fitness_parents;
    vector<vector<int>> offspring, pool;
    
    srand (time(NULL));
    Generate(parents, populationSize, individualSize);
    // Print(parents, populationSize, individualSize);
    // cout << "/--------------------------/\n";
    
    // vector<vector<int>> b;
    // b = OnepointCrossover(parents, populationSize, individualSize);
    // Print(b, populationSize, individualSize);
    // cout << "/--------------------------/\n";

    // vector<vector<int>> c;
    // c = Pool(parents, b);
    // Print(c, populationSize * 2, individualSize);

    // cout << "/--------------------------/\n";
    // Print(b, populationSize, individualSize);

    // cout << "/--------------------------/\n";

    int ep = 0;
    // Print(parents, populationSize, individualSize);
    // cout << "/--------/\n";

    while(ep < 10) {
        offspring = OnepointCrossover(parents, populationSize, individualSize);

        // Print(offspring, populationSize, individualSize);
        // cout << "/--------/\n\n";
        pool = Pool(parents, offspring);
        cout << pool.size() << "\n";
        // Print(parents, populationSize, individualSize);
        // cout << "/--------/\n\n";

        // for (int i = 0; i < populationSize; i++)
        //     cout << fitness_parents[i] << " ";
        // cout << "------------------\n";

        fitness_parents.clear();
        fitness_parents.shrink_to_fit();
        fitness_offspring.clear();
        fitness_offspring.shrink_to_fit();
        for (int i = 0; i < populationSize; i++) {
            fitness_parents.push_back(ValueMax(parents[i], individualSize));
            fitness_offspring.push_back(ValueMax(offspring[i], individualSize));
        }
    
        fitness_pool = fitness_parents;
        fitness_pool.insert(fitness_pool.end(), fitness_offspring.begin(), fitness_offspring.end());
        
        // return 0;
        TournamentSelection(parents, fitness_parents, pool, fitness_pool, populationSize);
        ep++;
    }
    
    // for (int i = 0; i < populationSize; i++) {
    //     for (int j = 0; j < individualSize; j++)
    //         cout << parents[i][j] << " ";
    //     cout << " /-----/ ";
    //     cout << fitness_parents[i] << "\n";
    // }
    cout << "Done!\n";
}