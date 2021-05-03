#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <random>
#include <bits/stdc++.h>
#include "data.cpp"
#include <chrono>

using namespace std::chrono;
using namespace std;

int myrandom (int i) { return std::rand()%i;}

// vector<int> values = {732,915,38,5,994,319,197,499,552,740,609,545,757,262,468,892,726,604,126,939,44,811,150,500,688,536,12,203,423,293,113,122,732,545,466,30,763,990,728,481,712,630,136,549,616,191,265,319,369,309,958,884,489,173,922,849,17,814,13,853,595,927,636,145,528,889,391,620,355,666,351,271,6,921,608,892,693,309,71,974,915,904,377,591,751,329,264,260,500,404,867,820,135,291,455,955,837,440,825,125};
// vector<int> values = {60, 100, 120};
// vector<int> weights = {10, 20, 30};
// vector<int> weights{77,908,178,827,913,429,343,41,32,945,871,110,365,669,359,20,937,550,281,902,342,994,677,928,368,608,776,556,596,618,964,917,455,332,792,529,308,341,245,263,482,384,119,892,314,933,696,963,698,946,477,717,620,45,943,759,87,701,646,967,508,88,485,516,550,458,35,427,240,945,325,969,353,369,7,817,86,734,685,562,315,907,757,390,830,11,773,111,599,726,934,97,732,743,569,143,931,36,635,167};
// int capacities = 26396;
// int capacities = 50;
int capacities;
vector<int> values, weights;

void Generate(vector<vector<int>> &a, const int &m,const int &n) {
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

vector<vector<int>> TwopointCrossover(vector<vector<int>> a, const int &m, const int &n) {
    vector<vector<int>> b = a;
    int i = 0;
    while (i < m - 1) {
        int randNum1 = rand() % ((n - 1) - 1 + 1) + 1;
        int randNum2 = rand() % ((n - 2) - 1 + 1);
        if (randNum2 >= randNum1)
            randNum2 += 1;
        else
            swap(randNum1, randNum2);

        for (int j = randNum1; j < randNum2; j++) {
            int tm = b[i][j];
            b[i][j] = b[i + 1][j];
            b[i + 1][j] = tm;
        }
        i += 2;
    }
    return b;
}


vector<vector<int>> UniformCrossover(vector<vector<int>> a, const int &m, const int &n) {
    vector<vector<int>> b = a;
    int i = 0;
    while (i < m - 1) {
        for (int j = 0; j < n; j++) 
            if ((double) rand() / (RAND_MAX) < 0.5) {
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
        iota(si.begin() + 1, si.end(), 1);
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

bool Compare2Individual(vector<int> a, vector<int> b, int n) {
    for (int i = 0; i < n; i++)
        if (a[i] != b[i])
            return false;
        return true;
}

bool CheckConvergence(vector<vector<int>> a, int m, int n) {
    for (int i = 0; i < m - 1; i++)
        if (!Compare2Individual(a[i], a[i + 1], n))
            return false;
    return true;
}

void Mutation(vector<vector<int>> &pool, vector<int> &fitness_pool, int m, int n, double p) {
    for (int i = 0; i < m; i++) 
        for (int j = 0; j < n; j++)
            if ((double) rand() / (RAND_MAX) < p) {
                if (pool[i][j] == 1) {
                    pool[i][j] = 0;
                    fitness_pool[i] -= values[j];
                }
                else {
                    pool[i][j] = 1;
                    fitness_pool[i] += values[j];
                }
            }
}

int main() {
    vector<string> inp = input_data();
    int individualSize, populationSize, v, w;
    double probMutation = 0.01;
    for (int file_index = 0; file_index < inp.size(); file_index++) {
        string file_name_tm = "data/" + inp[file_index] + ".kp";
        char* file_name = &file_name_tm[0];
        freopen(file_name, "r", stdin);
        cin >> individualSize;
        cin >> capacities;
        values.clear();
        values.shrink_to_fit();
        weights.clear();
        weights.shrink_to_fit();

        for (int i = 0; i < individualSize; i++) {
            cin >> v >> w;
            values.push_back(v);
            weights.push_back(w);
        }
        

        if (individualSize < 5000)
            populationSize = individualSize * 10;
        else if (individualSize == 5000)
            populationSize = 10000;
        else
            populationSize = 5000;
        
        vector<vector<int>> parents(populationSize, vector<int>(individualSize , 0));

        vector<int> fitness_offspring, fitness_pool, fitness_parents;
        vector<vector<int>> offspring, pool;
        double time_step_in_iter;
        srand (time(NULL));
        Generate(parents, populationSize, individualSize);
        int ep = 0;
        auto start = high_resolution_clock::now();
        while(!CheckConvergence(parents, populationSize, individualSize) && time_step_in_iter < 300) {
            // cout << "Running...\n"; 
            // offspring = OnepointCrossover(parents, populationSize, individualSize);
            offspring = UniformCrossover(parents, populationSize, individualSize);
            pool = Pool(parents, offspring);
        
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
            
            // Mutation(pool, fitness_pool, populationSize * 2, individualSize, probMutation);
            TournamentSelection(parents, fitness_parents, pool, fitness_pool, populationSize);
            
            ep++;
            auto stop_run = high_resolution_clock::now();
            auto duaration_stop_run = duration_cast<seconds>(stop_run - start);
            time_step_in_iter = duration<double>(duaration_stop_run).count();
            // cout << time_step_in_iter << "\n";
            // if (time_step_in_iter > 10)
                // break;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(stop - start);
        int computedValue = 0, indexIndividual = -1, totalWeight = 0;
        for (int i = 0; i < populationSize; i++) 
            if (fitness_parents[i] > computedValue) {
                computedValue = fitness_parents[i];
                indexIndividual = i;
            }
        
        for (int j = 0; j < individualSize; j++) 
            if (parents[indexIndividual][j] == 1)
                totalWeight += weights[j];

        // string output_filename= "output/Genetic_Algorithm/" + inp[file_index] + ".txt";
        // cout << output_filename << "\n";
        // char* outfile = &output_filename[0];
        // freopen(outfile, "w", stdout);
        cout << "File name: " << inp[file_index] << "\n";
        cout << "Number of items: " << individualSize << "\n";
        
        cout << "Total value: " << computedValue << "\n";
        cout << "Total weight: " << totalWeight << "\n";
        cout << "Capacity: " << capacities << "\n";
        cout << "Runtime: " << duration.count() << " seconds\n";
        cout << "Poluplation size: " << populationSize << "\n";
        cout << "Crossing: Uniform Crossover\n";
        // cout << "Crossing: Onepoint Crossover\n"; 
        cout << "Itertations: " << ep << "\n";
        // cout << "/---------------/\n\n";
    }
}
