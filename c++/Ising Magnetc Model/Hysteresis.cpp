//
//  Hysteresis.cpp
//  Script to measure Hysteresis loops.

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
int number = 11;
float temperature = 2;
float beta = 1.0/temperature;

vector<vector<float>> CrystalSimulator();
void crystalIterator(Crystal &latticeInstance, bool positiveIteration, bool up, vector<vector<float>> &Results);


int main(){

    vector<vector<vector<float>>> TotalResults;
    std::vector<std::future<std::vector<std::vector<float>>>> futures;
    int samples(10);
    for(int i=0; i<samples; i++){
        futures.push_back(std::async(CrystalSimulator));
    }

    for( auto &R: futures ){
        TotalResults.push_back(R.get());
    }
    float appliedH(0.0);
    float total(0.0);
    float average(0.0);
    float error(0.0);
    int Len = TotalResults.size();
    int Leni = TotalResults[0].size();
    for ( int h=0; h< Leni; h++) {
        total=0.0;
        for(int j=0; j< Len; j++){
            if(j==0){
                appliedH =TotalResults[j][h][0];
            }
            total += TotalResults[j][h][1];
        }
        average = total/samples;
        for(int j=0; j< Len; j++){
            error += (average - TotalResults[j][h][1])*(average - TotalResults[j][h][1]);
        }
        cout << appliedH << "  " << average/N << "," << sqrt(error) << endl;
    }


    //      Save Results to .csv file:
    ofstream outdata;
    string fileloc = "/Users/tommciver/Documents/Crystal190420/ResultsFinal/Hysteresis/HysteresisLoopHysteresisL=16J=1T=2i.csv";
    outdata.open(fileloc);
    if( !outdata ) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    for ( int h=0; h< Leni; h++) {
        total=0.0;
        for(int j=0; j< Len; j++){
            if(j==0){
                appliedH =TotalResults[j][h][0];
            }
            total += TotalResults[j][h][1];
        }
        average = total/samples;
        for(int j=0; j< Len; j++){
            error += (average - TotalResults[j][h][1])*(average - TotalResults[j][h][1]);
        }
        outdata << appliedH << "," << average << "," << sqrt(error/samples) << "\n";
    }

    outdata.close();

    return 0;
}





vector<vector<float>> CrystalSimulator(){

    Crystal Lattice(size, J, mu, temperature, 0.0, MCC);
    Lattice.Reset(0.0);
    Lattice.InitialiseState(temperature, 0.0);

    vector<vector<float>> Results;
    crystalIterator(Lattice, true, true, Results);
    crystalIterator(Lattice, true, false, Results);
    crystalIterator(Lattice, false, true, Results);
    crystalIterator(Lattice, false, false, Results);
    crystalIterator(Lattice, true, true, Results);
    return Results;
}

void crystalIterator(Crystal &latticeInstance, bool positiveIteration, bool up, vector<vector<float>> &Results){
    float field[number];
    if (positiveIteration){
        //Get results for lattice in positive field application.
        for(int i=0; i<number; i++){
            field[i] = (float)(i)/50;
        }
    } else{
        //Get results for lattice in negative field application.
        for(int i=0; i<number; i++){
            field[i] =-(float)(i)/50;
        }
    }

    if(up){
        //Iterate through the field array in given order
        for(auto H: field){
            for( int h=0; h < MCC; h++){
                for( int k=0; k < N; k++ ){
                    latticeInstance.UpdatePosition(temperature, beta, H);
                }
                latticeInstance.ObserveDependentVariables(temperature, H);
            }
            Results.push_back({H, latticeInstance.ReturnAverageMagnetisationPerAtom()});
//            latticeInstance.PrintLattice();
            latticeInstance.ResetDependentVariables();
        }
    } else{
        //Iterate through field array from back to front
        for (int i = number-1; i --> 0; ){
            for( int h=0; h < MCC; h++){
                for( int k=0; k < N; k++ ){
                    latticeInstance.UpdatePosition(temperature, beta, field[i]);
                }
                latticeInstance.ObserveDependentVariables(temperature, field[i]);
            }
            Results.push_back({field[i], latticeInstance.ReturnAverageMagnetisationPerAtom()});
//            latticeInstance.PrintLattice();
            latticeInstance.ResetDependentVariables();
        }
    }
}
