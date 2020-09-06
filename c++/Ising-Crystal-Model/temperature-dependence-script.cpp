//
//  This script simulates the physical paramaters of a crystal lattice
//  according to the Ising model of a ferromagnetic crystal. The simulated
//  parameters are average absolute magnetisation, squared magnetisation,
//  energy and squared energy per atom, in addition to the specific heat
//  capacity implied by these simulated values. The Ising model of a
//  ferromagnet makes predictions of crystal behaviour under various
//  external conditions, and this script reproduces these predictions
//  for variable external temperature. This means that all other variables
//  are set as constants during the simulation, including:
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
//  Other scripts in this project examine the effect of changing MCC on the
//  accuracy and run-time of simulation, and the effect of changing the other
//  physical paramaters for a constant temperature.
//
//  The script will return a .csv file and save it within a file specified by
//  the variable save_loc.



#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <future>
#include "Crystal.hpp"

using namespace std;

//simulation constants:
const int size(4);
const float J(1);
const float mu(1.0);
float field(0.0);
int MCC(100000);
float N(size*size);
string save_loc("/Users/tommciver/Documents/ising_model_simulator/output/TEST.csv");

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
  int number = 60;
  float temperatures[number];
  for(int i = 0; i < number; i++){
    temperatures[i] = (float)(i)/10.0 + 0.3;
  }


  vector<vector<float>> results;

  auto begin = std::chrono::system_clock::now();
  std::vector<std::future<std::vector<float>>> futures;

  for( auto T : temperatures){
    futures.push_back(std::async(compute, T, field));
  }

  for( auto &R : futures ){
    results.push_back(R.get());
  }

  auto end = std::chrono::system_clock::now();
  std::cout << "Simulation Runtime = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << "\n";

  // Save results to .csv file:
  ofstream outdata;
  outdata.open(save_loc);
  if( !outdata ) {
    cerr << "Error: file could not be opened" << endl;
    exit(1);
  }
  int length = results.size();

  for ( int h = 0; h < length; h++) {
    for( int g = 0; g < 10; g++){
      outdata << results[h][g] << ',';
    }
    outdata << results[h][10];
    outdata << endl;
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
