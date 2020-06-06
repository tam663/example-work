//
//  Autocorrelation.cpp
//  Script to measure autocorrelation:

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <cstdlib>
#include "Crystal.hpp"
#include <chrono>
#include <future>

using namespace std;

//simulation constants:
const int size(8);
const float J(1);
const float mu(1.0);
int MCC(100000);
float N = size*size;

// Two functions used in this script: Compute, which iterates the simulated lattice, and returns simulated
// measurements of the macroscopic variables of interest, and AutoCorrelation, which returns the average
// auto-correlation for a given time lag. See accompanying pdf for definition of this value and why it
// is of interest.
std::vector<float> Compute(float temperature, float field);
vector<float> AutoCorrelation(int lag, float temperature, float field);


int main(){

    float field(0.0);
    vector<vector<float>> Results;
    float temperature(7.0);
    std::vector<std::future<std::vector<float>>> futures;
        
    for(int j =1; j<100; j++){
        int x = (int)(5*j);
        futures.push_back(std::async(AutoCorrelation, x, temperature, field));
    }
    
    for( auto &R: futures ){
        Results.push_back(R.get());
    }
    
    // Save Results to .csv file:
    ofstream outdata;
    string fileloc = "/Users/tommciver/Documents/Crystal190420/ResultsFinal/autocorrelationN=8MCC=100kT=7.0.csv";
    outdata.open(fileloc);
    if( !outdata ) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    for ( int h=1; h< 99; h++) {
        outdata << Results[h][0] << ',';
        outdata << Results[h][1]/Results[0][1];
        outdata << "\n";
    }
    outdata.close();

    return 0;
}


std::vector<float> Compute(float temperature, float field){
    Crystal Lattice(size, J, mu, temperature, field, MCC);
    float beta = 1.0/temperature;
    vector<float> results(11, 0.0);
    
    Lattice.Reset(field);
    Lattice.InitialiseState(temperature, field);
    for( int h=0; h < MCC; h++){ // MCC samples of MCMC metropolis cycles
    
        for( int k=0; k < N; k++ ){ // MCC monte carlo lattice iterations between each sample.
        // Update position and measure physical paramaters on each cycle:
            Lattice.UpdatePosition(temperature, beta, field);
        }

        Lattice.ObserveDependentVariables(temperature, field);
        
    }
    // The two Return functions below access the results vector and compute the various averages from the observations collected above:
    auto [AvMagnetisation, MagErr, AvSquaredMagnetisation, SqMagErr, AvLatticeEnergy, EnErr, AvSquaredLatticeEnergy, SqEnErr, HeatCapacity, HeatCapacityErr] = Lattice.ReturnDependentVariables(temperature);
    results.assign({temperature, AvMagnetisation, MagErr, AvSquaredMagnetisation, SqMagErr, AvLatticeEnergy, EnErr, AvSquaredLatticeEnergy, SqEnErr, HeatCapacity, HeatCapacityErr});
    return results;
}



vector<float> AutoCorrelation(int lag, float temperature, float field){
    Crystal Lattice(size, J, mu, temperature, field, MCC);
    float beta = 1.0/temperature;
    vector<float> results(2, 0.0);
    float autoCorr = 0.0;
      
    Lattice.Reset(field);
    Lattice.InitialiseState(temperature, field);
    for( int h=0; h < MCC; h++){ // MCC samples of MCMC metropolis cycles
        for( int k=0; k < lag; k++ ){ // MCC monte carlo lattice iterations between each sample.
          // Update position and measure physical paramaters on each cycle:
            Lattice.UpdatePosition(temperature, beta, field);
        }
        Lattice.ObserveDependentVariables(temperature, field);
    }
    auto simulationResults = Lattice.ReturnCumulativeMagnetisationVector();
    auto Av = Lattice.ReturnAverageMagnetisation();
    for(int i=0; i<MCC-1; i++){
        autoCorr += (simulationResults[i] - Av)*(simulationResults[i+1]-Av);
    }
    results.assign({(float)lag, autoCorr/(MCC-1)});
    return results;
}
