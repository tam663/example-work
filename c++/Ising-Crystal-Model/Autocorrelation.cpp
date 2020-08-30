//
//  Script which calculates the autocorrelation of successive measurements
//  within the simulation. This is an important variable for investigating
//  how often a simulated crystal should be sampled when running a full
//  simulation, and thus this script is for investigating properties of
//  simulation rather than physical properites simulated. A full
//  interpretation of what this variable means can be found in the
//  accompanying report.
//

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
const float mu(1.0);
const float field(0.0);
const int MCC(100000);
const string save_loc = "/Users/tommciver/Documents/ising_model_simulator/output/auto_correlationN=8MCC=100kT=7.0.csv";


//  Returns a vector containing the autocorrelation and the time interval
//  between measurements for which that autocorrelation corresponds.
//  Theoretically, it is expected that the applied external field should
//  impact the correlation, and it is passed using the field variable.
vector<float> auto_correlation(int lag, float temperature, float field);


int main(){

  vector<vector<float>> Results;
  float temperature(7.0);
  std::vector<std::future<std::vector<float>>> futures;
        
  for(int j = 1; j < 100; j++){
    int x = (int)(5*j);
    futures.push_back(std::async(auto_correlation, x, temperature, field));
  }
    
  for( auto &R: futures ){
    Results.push_back(R.get());
  }
    
    // Save Results to .csv file:
  ofstream outdata;
  outdata.open(save_loc);
  if( !outdata ) {
    cerr << "Error: file could not be opened" << endl;
    exit(1);
  }
  for ( int h = 1; h < 99; h++) {
    outdata << Results[h][0] << ',';
    outdata << Results[h][1]/Results[0][1];
    outdata << "\n";
  }
  outdata.close();

  return 0;
}


//  The main logic of this function consists of sampling and iterating
//  this instance of Crystal, the details of which are determined by
//  the global variabes MCC and N. The details of the autocorrelation
//  calculation which is carried out can be found in the accompanying
//  report.
vector<float> auto_correlation(int lag, float temperature, float field){
  Crystal Lattice(size, J, mu, temperature, field, MCC);
  float beta = 1.0/temperature;
  vector<float> results(2, 0.0);
  float autoCorr(0.0);
      
  Lattice.Reset(field);
  Lattice.InitialiseState(temperature, field);
  for( int h = 0; h < MCC; h++){  // MCC samples of MCMC metropolis cycles
    for( int k = 0; k < lag; k++ ){  // MCC monte carlo lattice iterations between each sample.
      Lattice.UpdatePosition(temperature, beta, field);  // iterate crystal
    }
    Lattice.ObserveDependentVariables(temperature, field);  // sample crystal
  }
  auto simulationResults = Lattice.ReturnCumulativeMagnetisationVector();
  auto Av = Lattice.ReturnAverageMagnetisation();
  
  for(int i = 0; i < MCC-1; i++){
    autoCorr += (simulationResults[i] - Av)*(simulationResults[i+1]-Av);
  }
  results.assign({(float)lag, autoCorr/(MCC-1)});
  return results;
}
