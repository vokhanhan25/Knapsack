#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <bits/stdc++.h>
using namespace std;

int myrandom (int i) { return std::rand()%i;}

vector<int> values = {360, 83, 59, 130, 431, 67, 230, 52, 93, 125, 670, 892, 600, 38, 48, 147,
    78, 256, 63, 17, 120, 164, 432, 35, 92, 110, 22, 42, 50, 323, 514, 28,
    87, 73, 78, 15, 26, 78, 210, 36, 85, 189, 274, 43, 33, 10, 19, 389, 276,
    312};

vector<int> weights {7, 0, 30, 22, 80, 94, 11, 81, 70, 64, 59, 18, 0, 36, 3, 8, 15, 42, 9, 0,
    42, 47, 52, 32, 26, 48, 55, 6, 29, 84, 2, 4, 18, 56, 7, 29, 93, 44, 71,
    3, 86, 66, 31, 65, 0, 79, 20, 65, 52, 13};

int capacities = 850;

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

vector<vector<int>> OnepointCrossover(const vector<vector<int>> &a, const int &m, const int &n) {
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

int ValueMax(vector<int> a) {
    int value = 0, weight = 0;
    for (int i = 0; i < a.size(); i++) 
        if (a[i] == 1) {
            value += values[i];
            weight += weights[i];
        }
    if (weight <= capacities)
        return value;
    return 0;
}
vector<vector<int>> TournamentSelection(vector<vector<int>> parents, vector<int> fitness_parents, vector<vector<int>> pool, vector<int> fitness_pool, int m) {
    int cnt = -1;
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
                cnt += 1;
                parents[cnt] = pool[max_index];
                fitness_parents[cnt] = fitness_pool[max_index];
                max_fitness = 0;
                max_index = -1;
            }
            else if (fitness_pool[si[i]] >= max_fitness) {
                max_fitness = fitness_pool[si[i]];
                max_index = si[i];
            }
    }
    return parents;
}

int main() {
    int populationSize = 30, individualSize = values.size();
    vector<vector<int>> parents(populationSize, vector<int>(individualSize , 0));
    
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
    vector<int> fitness_parents, fitness_offspring, fitness_pool;
    vector<vector<int>> offspring, pool;
    while(ep < 5000) {
        offspring = OnepointCrossover(parents, populationSize, individualSize);
        pool = Pool(parents, offspring);
        
        for (int i = 0; i < populationSize; i++) {
            fitness_parents.push_back(ValueMax(parents[i]));
            fitness_offspring.push_back(ValueMax(offspring[i]));
        }

        fitness_pool = fitness_parents;
        fitness_pool.insert(fitness_pool.end(), fitness_offspring.begin(), fitness_offspring.end());
        parents = TournamentSelection(parents, fitness_parents, pool, fitness_pool, populationSize);
        ep++;
    }
    
    for (int i = 0; i < populationSize; i++) {
        for (int j = 0; j < individualSize; j++)
            cout << parents[i][j] << " ";
        cout << " /-----/ ";
        cout << fitness_parents[i] << "\n";
    }
}