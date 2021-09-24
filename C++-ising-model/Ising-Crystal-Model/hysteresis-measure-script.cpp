//
//  This script computes the hysteresis behaviour for a lattice under given
//  physical conditions specified by the variables:
//
//    -> size : float
//         Length of the dimensions of the simulated 2.d, square lattice
//    -> J : float
//         Strength of magnetic interaction constant
//    -> mu : float
//         Strength of the atomic interaction constant
//    -> temperature : float
//
//
//  The simulation constants are specified by:
//
//    -> MCC : int
//         Number of monte carlo cycles used in simulation - see
//         accompanying report for in depth discussion.
//    -> number : int
//         Specifies the number of observations of the simulated crystal
//         to be taken under each section of the hysteresis cycle.
//
//  To obtain one set of observations of the hysteresis behaviour of a ferromagnet
//  multiple threads cannot be used due to the path dependent nature of this
//  observable. The crystal_simulator function used in this script contains the
//  logic to retrieve one set of measurements of the hysteresis, and within
//  crystal_simulator, iterate_crystal is called sequentially to account for this
//  fact.
//  This script does still use multiple threads to create a full set of data for
//  the hysteresis behaviour however by running the crystal_simulator in multiple
//  threads for each temperature observed. The variable samples determines the
//  number of threads used for each observation.
//  The logic implemented in this script is generally quite complex due to the
//  nature of hysteresis behaviour and it is recommended to consult the detailed
//  discussion of hysteresis in the accompanying report before running this
//  program.

#include <vector>
#include <iostream>
#include <fstream>
#include "Crystal.hpp"
#include <chrono>
#include <future>

using namespace std;

//simulation constants:
const int size(16);
const float J(1);
const float mu(1.0);
const int MCC(100000);
float N(size*size);
int number(11);
float temperature(2);
float beta(1.0/temperature);
string save_loc("/Users/tommciver/Documents/ising_model_simulator/output/HysteresisLoopHysteresisL=16J=1T=2i.csv");
int samples(10);


//  Returns observations of the hysteresis behaviour for a single simulation of a
//  crystal. A crystal is initialised using the global variables for physical
//  paramaters.
vector<vector<float>> crystal_simulator();

//  This iterates a given Crystal under an external applied field which is specified
//  by positive_iteration and 'up'. The results of the sampling in these conditions
//  are placed in the vector results.
void iterate_crystal(Crystal &latticeInstance, bool positive_iteration, bool up, vector<vector<float>> &results);


int main(){

  vector<vector<vector<float>>> total_results;
  std::vector<std::future<std::vector<std::vector<float>>>> futures;
  
  for(int i = 0; i < samples; i++){
    futures.push_back(std::async(crystal_simulator));
  }

  for( auto &R: futures ){
    total_results.push_back(R.get());
  }
  float applied_field(0.0);
  float total(0.0);
  float average(0.0);
  float error(0.0);
  int Len = total_results.size();
  int Len_i = total_results[0].size();
  
  for ( int h = 0; h < Len_i; h++) {
    total = 0.0;
    for(int j = 0; j < Len; j++){
      if(j == 0){
        applied_field =total_results[j][h][0];
      }
      total += total_results[j][h][1];
    }
    average = total/samples;
    for(int j = 0; j < Len; j++){
      error += (average - total_results[j][h][1])*(average - total_results[j][h][1]);
    }
    cout << applied_field << "  " << average/N << "," << sqrt(error) << endl;
  }


//      Save Results to .csv file:
  ofstream outdata;
  outdata.open(save_loc);
  if( !outdata ) {
    cerr << "Error: file could not be opened" << endl;
    exit(1);
  }
  for ( int h = 0; h < Len_i; h++) {
    total = 0.0;
    for(int j = 0; j < Len; j++){
      if(j == 0){
        applied_field =total_results[j][h][0];
      }
      total += total_results[j][h][1];
    }
    average = total/samples;
    for(int j = 0; j < Len; j++){
      error += (average - total_results[j][h][1])*(average - total_results[j][h][1]);
    }
    outdata << applied_field << "," << average << "," << sqrt(error/samples) << "\n";
  }

  outdata.close();
  return 0;
}




//  For a simulated crystal initialised within the function, the crystal is iterated
//  four times, along different paths of the hysteresis loops to produce a full
//  set of observations.
vector<vector<float>> crystal_simulator(){

  Crystal Lattice(size, J, mu, temperature, 0.0, MCC);
  Lattice.rest_lattice(0.0);
  Lattice.initialise_state(temperature, 0.0);
  vector<vector<float>> results;
    
  iterate_crystal(Lattice, true, true, results);
  iterate_crystal(Lattice, true, false, results);
  iterate_crystal(Lattice, false, true, results);
  iterate_crystal(Lattice, false, false, results);
  iterate_crystal(Lattice, true, true, results);
  return results;
}


//  Iterates a given lattice along a specified branch of the hysteresis loop.
void iterate_crystal(Crystal &latticeInstance, bool positive_iteration, bool up, vector<vector<float>> &results){
  float field[number];
  if (positive_iteration){
        //Get results for lattice in positive field application.
    for(int i = 0; i < number; i++){
      field[i] = (float)(i)/50;
    }
  } else{
        //Get results for lattice in negative field application.
    for(int i = 0; i < number; i++){
      field[i] =-(float)(i)/50;
    }
  }

  if(up){
        //Iterate through the field array in given order
    for(auto H : field){
      for( int h = 0; h < MCC; h++){
        for( int k = 0; k < N; k++ ){
          latticeInstance.update_position(temperature, beta, H);
        }
        latticeInstance.observe_dependent_variables(temperature, H);
      }
      results.push_back({H, latticeInstance.return_average_magnetisation_per_atom()});
      latticeInstance.rest_dependent_variables();
    }
    } else{
        //Iterate through field array from back to front
        for (int i = number-1; i --> 0; ){
          for( int h = 0; h < MCC; h++){
            for( int k = 0; k < N; k++ ){
              latticeInstance.update_position(temperature, beta, field[i]);
            }
            latticeInstance.observe_dependent_variables(temperature, field[i]);
          }
          results.push_back({field[i], latticeInstance.return_average_magnetisation_per_atom()});
          latticeInstance.rest_dependent_variables();
        }
    }
}
