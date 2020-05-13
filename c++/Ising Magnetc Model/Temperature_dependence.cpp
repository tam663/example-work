//
//  CrystalDebuging.cpp
//  Script to measure behaviour with temperature


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
const int size(16);
const float J(2);
const float mu(1.0);
int MCC(100000);
float N = size*size;


std::vector<float> Compute(float temperature, float field);

int main(){
    int number = 60;
    float temperatures[number];
    for(int i=0; i<number; i++){
        temperatures[i] = (float)(i)/10.0 + 0.3;
    }

    float field(0.0);
    vector<vector<float>> Results;

    auto begin= std::chrono::system_clock::now();
    std::vector<std::future<std::vector<float>>> futures;
    
    for( auto T: temperatures){
        futures.push_back(std::async(Compute, T, field));
    }

    for( auto &R: futures ){
        Results.push_back(R.get());
    }
    
    auto end = std::chrono::system_clock::now();
    std::cout << "Simulation Runtime = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << "\n";

    // Save Results to .csv file:
    ofstream outdata;
    string fileloc = "/Users/tommciver/Documents/Crystal190420/ResultsFinal/NEW/L=16J=2MCC=100000i.csv";
    outdata.open(fileloc);
    if( !outdata ) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    int length = Results.size();
    for ( int h=0; h< length; h++) {

        for( int g =0; g<10; g++){
            outdata << Results[h][g] << ',';
        }
        outdata << Results[h][10];
        outdata << endl;
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
        for( int k=0; k < N; k++ ){
            // MCC monte carlo lattice iterations between each sample.
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
