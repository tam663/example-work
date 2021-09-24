//
//  This script measure the total runtime required to produce a full set of
//  simulations for a given variable paramater, e.g. the simulated behaviour
//  with respect to temperature, and calulates the run-time speed ups
//  observed when the program is run in multiple threads.

//  The physical constants used in the simulation are of importance in this
//  script, but the are set to default values using the constants:
//
//    -> Size of simulated crystal, set by N
//    -> Strength of the applied external field, set by field
//    -> Strength of the atomic interaction constant, set by mu
//    -> Strength of magnetic interaction set by J
//
//  In addition to this, the simulation also takes variables which
//  determine the accuracy and length of the simulation. MCC is the primary
//  variable which determines this, and it is by default set to 100,000.
//  There is a trade off between simulating the crystal on practical time-
//  scales, and reproducing acceptible levels of accuracy in the simulation.
//  100,000 is chosen as it satisfies both these constraints within a
//  reasonabe limit.
//
//  The script will return a .csv file and save it within a file specified by
//  the variable save_loc.



#include <vector>
#include <iostream>
#include <fstream>
#include "Crystal.hpp"
#include <chrono>
#include <future>

using namespace std;

//simulation constants:
const int size(8);
const float J(1);
const float field(0.0);
const float mu(1.0);
const int MCC(100000);
const float N(size*size);
const string save_loc("/Users/tommciver/Documents/ising_model_simulator/output/N=32MCC=100000threads=10_error.csv");
const int number(5);

//  Returns a vector containing the simulated physical crystal parameters
//  for a given temperature and applied external field. The returned vector
//  includes the average absolute magnetisation, squared magnetisation,
//  energy and squared energy per atom, in addition to the specific heat
//  capacity implied by these simulated values. The properties of the
//  simulated crystal are set by the global parameters 'J', 'mu' and 'N',
//  and the size (i.e. the number of samples carried out in the markov
//  chain) of the simulation is set by 'MCC'.
std::vector<float> compute(float temperature, float field);


int main(){
  
  float temperatures[number];
  for(int i = 0; i < number; i++){
    temperatures[i] = (float)(i)/10.0 + 0.1;
  }

  vector<vector<float> > Results;
  vector<vector<float> > RunTimes;
  auto begin = std::chrono::system_clock::now();
  auto end = std::chrono::system_clock::now();
  float current(0.0);
  int num_observations = 10;
  for(int I = 1; I < 5*num_observations+1; I += 5){

    begin= std::chrono::system_clock::now();
    std::vector<std::future<std::vector<float> > > futures;
    for(int j = 0; j < 10; j++){
    futures.push_back(std::async(compute, 3.0, field));
    }

    for( auto &R : futures ){
      Results.push_back(R.get());
    }
    end = std::chrono::system_clock::now();

    current = (float)I;
    RunTimes.push_back({current, (float)chrono::duration_cast<chrono::milliseconds>(end - begin).count(), (float)chrono::duration_cast<chrono::milliseconds>(end - begin).count() /((float)MCC*I)} );
  }

    
    
//      Save Results to .csv file:
  ofstream outdata;
  string fileloc = save_loc;

  outdata.open(fileloc);
  if( !outdata ) {
    cerr << "Error: file could not be opened" << endl;
    exit(1);
  }
  for ( int h = 0; h < num_observations; h++) {
    outdata << RunTimes[h][0] << ',';
    outdata << RunTimes[h][1] << ',';
    outdata << RunTimes[h][2];
    outdata << "\n";
  }

 outdata.close();

 return 0;
}


//  An instance of the Crystal class is initialised within this function.
//  The main logic of this function consists of sampling and iterating
//  this instance of Crystal, the details of which are determined by
//  the global variabes MCC and N. The details of the Markov chain
//  mathematics to which this iteration and sampling corresponds can be
//  found in the accompanying report.
std::vector<float> compute(float temperature, float field){

  Crystal Lattice(size, J, mu, temperature, field, MCC);
  float beta(1.0/temperature);
  vector<float> results(11, 0.0);

  Lattice.rest_lattice(field);
  Lattice.initialise_state(temperature, field);

  for( int h = 0; h < MCC; h++){  // MCC samples of MCMC metropolis cycles
    for( int k = 0; k < N; k++ ){  // MCC monte carlo lattice iterations between each sample.
      Lattice.update_position(temperature, beta, field);  // iterate crystal
    }
    Lattice.observe_dependent_variables(temperature, field);  // sample crystal
  }

  auto [av_magnetisation, magnetisation_error, average_squared_magnetisation, squared_magnetisation_error, average_lattice_energy, lattice_energy_error, average_squared_lattice_energy, sqd_lattice_energy_error, heat_capacity, heat_capacity_error] = Lattice.return_dependent_variables(temperature);  // retrieve the sampled values, which are tracked within the Crystal class.
  results.assign({temperature, av_magnetisation, magnetisation_error, average_squared_magnetisation, squared_magnetisation_error, average_lattice_energy, lattice_energy_error, average_squared_lattice_energy, sqd_lattice_energy_error, heat_capacity, heat_capacity_error});
  return results;
}
