//
//  RecordSimulationRunTimes.cpp
//  Script to record lattice simlation run times.

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
const int size(32);
const float J(1);
const float mu(1.0);
int MCC(100000);
float N = size*size;

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


int main(){
    int number = 60;
    float temperatures[number];// 20.0, 30.0, 40.0, 50.0, 75.0, 100.0, 125.0, 150.0, 175.0, 200.0, 225.0, 250.0, 275.0, 300.0, 325.0, 350.0, 375.0, 400.0};
    for(int i=0; i<number; i++){
        temperatures[i] = (float)(i)/10.0 + 0.1;
    }

//    const int length = (sizeof(temperatures)/sizeof(*temperatures));
    float field(0.0);

    vector<vector<float> > Results;
    vector<vector<float> > RunTimes;
    auto begin= std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    float current = 0.0;
    int NumberChecks = 80;
    for(int I=1; I<5*NumberChecks+1; I+= 5){

        begin= std::chrono::system_clock::now();
        std::vector<std::future<std::vector<float> > > futures;
        for(int j=0; j<10; j++){
            futures.push_back(std::async(Compute, 3.0, field));
        }

        for( auto &R: futures ){
            Results.push_back(R.get());
        }
        end = std::chrono::system_clock::now();

        current = (float)I;
        RunTimes.push_back({current, (float)chrono::duration_cast<chrono::milliseconds>(end - begin).count(), (float)chrono::duration_cast<chrono::milliseconds>(end - begin).count() /((float)MCC*I)} );
    }

//    int NumberChecks = 40;
//    for(int I=1; I<5*NumberChecks+1; I+= 5){
//
//        begin= std::chrono::system_clock::now();
//        for(int j=0; j<I; j++){
//            Compute(3.0, field);
//        }
//        end = std::chrono::system_clock::now();
//    //        std::cout << "Simulation Runtime for thread number = " << I << "equals: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << "ms" << "\n";
//    //        std::cout << "Time per observation: " << (float)chrono::duration_cast<chrono::milliseconds>(end - begin).count() /((float)MCC*I) << "ms" << "\n";
//        current = (float)I;
//        RunTimes.push_back({current, (float)chrono::duration_cast<chrono::milliseconds>(end - begin).count(), (float)chrono::duration_cast<chrono::milliseconds>(end - begin).count() /((float)MCC*I)} );
//    }
    
    
//      Save Results to .csv file:
    ofstream outdata;
    string fileloc = "/Users/tommciver/Documents/Crystal190420/ResultsFinal/NEW/Runtimesi/N=32MCC=100000threads=10_error.csv";

    outdata.open(fileloc);
    if( !outdata ) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    for ( int h=0; h< NumberChecks; h++) {
        outdata << RunTimes[h][0] << ',';
        outdata << RunTimes[h][1] << ',';
        outdata << RunTimes[h][2];
        outdata << "\n";
    }

    outdata.close();

    return 0;
}

