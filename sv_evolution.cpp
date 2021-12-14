#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <cmath>
#include <numeric>
#include <string>
#include <algorithm>
#include <tuple>

#include <math.h> 
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;
namespace py = pybind11;

using namespace std;

#include "utils.h"

/*
Basic structural variation evolution
*/

class Cell {     
    public:
        string barcode;    
        double mut_rate;
        vector<string> mut_ids;
        vector<int> sv_lengths;
    
    // Mutate cell
    template< class rng >
    void mutate(string sv_dist, rng &generator) {
                
        // int n_mutations = sample_poisson(generator, mut_rate);
        // Mutation rate is really low per genome in blood so use an approximate sampling method
        double r = sample_uniformi(generator, 0, 1);
        int n_mutations;
        if (r < exp(-mut_rate)) {
            n_mutations = 0;
        } else {
            n_mutations = 1;
        }

        if (n_mutations > 0) {
            for (int i=0; i < n_mutations; i++) {                
                
                int sv_length = 0;
                if (sv_dist == "uniform") {
                    sv_length = sample_uniformi(generator, 1000, 100000);
                    sv_lengths.push_back(sv_length);
                } else if (sv_dist == "normal") {
                    sv_length = (int)sample_normal(generator, 3000, 5000);
                    sv_lengths.push_back(sv_length);
                } else if (sv_dist == "bimodal") {
                    sv_length = (int)sample_bimodal(generator, 3000, 5000, 10000, 5000);
                    sv_lengths.push_back(sv_length);
                } else {
                    cerr << "Error: invalid sv_dist" << endl;
                    exit(1);
                }

                string mut_id = barcode + ":" + std::to_string(sv_length);
                mut_ids.push_back(mut_id);
            }
        } else {
            string mut_id = "N";
            mut_ids.push_back(mut_id);
            sv_lengths.push_back(0);
        }
    }
};

template< class rng >
auto simulate(double popsize, double gens, double mut_rate, string sv_dist, rng &generator) {
    
    // Initialize population
    vector<Cell> population;
    for (int i=0; i < popsize; i++) {
        Cell cell;
        cell.barcode = std::to_string(i);
        cell.mut_rate = mut_rate;
        population.push_back(cell);
    }
    
    // Simulate a Moran process; randomly select two individuals, one dies, one is born
    int track_barcodes = (int)popsize;
    for (int i=0; i < gens; i++) {        

        for (int j=0; j < population.size(); j++) {
            // Randomly select two individuals (cell1 divides, cell2 dies)
            int cell1 = sample_uniformi(generator, 0, popsize-1);        
            int cell2 = sample_uniformi(generator, 0, popsize-1);
            while (cell2 == cell1) {
                cell2 = sample_uniformi(generator, 0, popsize-1);
            }
            
            // Update duplicated cell
            Cell cell;
            cell.barcode = population[cell1].barcode + ":" + std::to_string(track_barcodes);
            cell.mut_rate = mut_rate;
            cell.mut_ids = population[cell1].mut_ids;
            cell.sv_lengths = population[cell1].sv_lengths;
            population[cell2] = cell;
            track_barcodes += 1;

            // Mutate duplicated cells
            population[cell1].mutate(sv_dist, generator);
            population[cell2].mutate(sv_dist, generator);

        }
    }
    
    // Return
    return population;
}

auto sv_evolution(
    double population_size,
    double generations,
    double per_genome_mutation_rate,
    string sv_length_distribution) -> tuple<vector<string>, vector<int>, vector<double>>
{
        
        // Initialize random number generator
        //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        //default_random_engine generator(seed);
        random_device generator;
        
        // Simulate
        auto population = simulate(population_size, generations, per_genome_mutation_rate, "uniform", generator);        

        // Store variants
        vector<string> cell_ids;
        vector<string> mut_ids;
        vector<int> sv_lengths;
        
        for (int i=0; i < population.size(); i++) {            
            for (int j=0; j < population[i].mut_ids.size(); j++) {
                if ((population[i].mut_ids[j] != "N") & (population[i].sv_lengths[j] > 0)) {
                    cell_ids.push_back(population[i].barcode);
                    mut_ids.push_back(population[i].mut_ids[j]);
                    sv_lengths.push_back(population[i].sv_lengths[j]);
                }
            }
        }
        
        vector<double> freq;
        vector<string> unique_mutations = remove_duplicates(mut_ids);
        vector<int> unique_sv_lengths;
        cout << unique_sv_lengths.size() << endl;
        cout << unique_mutations.size() << endl;
        for (int i = 0; i < unique_mutations.size(); i++) {

            // Count occurrences of each unique mutation
            int count = std::count(mut_ids.begin(), mut_ids.end(), unique_mutations[i]);
            freq.push_back((double)count/(double)population_size);
            
            // Get corresponding SV
            for (int j = 0; j < mut_ids.size(); j++) {
                if (mut_ids[j] == unique_mutations[i]) {
                    unique_sv_lengths.push_back(sv_lengths[j]);
                    break;
                }
            }            
        }

        return {unique_mutations, unique_sv_lengths, freq};
}

PYBIND11_MODULE(sv_evolution, m) {
    m.doc() = "Basic structural variation evolution";

    m.def("sv_evolution", &sv_evolution, "SV evolution",
    	py::arg("population_size") = 10000,
        py::arg("generations") = 0.01,
        py::arg("per_genome_mutation_rate") = 1.0,
        py::arg("sv_length_distribution") = "uniform");
};