//
//  Crystal.cpp
//  debug_pracice
//
//  Created by Tom McIver on 08/04/2020.
//  Copyright © 2020 Tom McIver. All rights reserved.
//

#include "Crystal.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <tuple>
#include <map>


Crystal::Crystal(int dims, float interactionE, float MU, float temperature, float field, int NumSamples)
: mu(MU), J(interactionE), size(dims), N(NumSamples), total(0.0), accepted(0.0), generator(rd()),
distribution(0.0, 1.0), NumMeasurements(0), CumulativeMagnetisation(N, 0.0),
CumulativeSquaredMagnetisation(N, 0.0), CumulativeLatticeEnergy(N, 0.0), CumulativeSquaredLatticeEnergy(N, 0.0),
Magnetisation(0.0), LatticeEnergy(0.0), SquaredLatticeEnergy(0.0)
{
    float rando;
    for (int i = 0; i < size; i++)
    {
        // construct a vector of ints with given default value
        std::vector<float> v;
        for (int j = 0; j < size; j++){
            rando = distribution(generator);
            if( rando <= 0.5 ){
                v.push_back(-1.0);
            }
            else if( rando > 0.5 ){
                v.push_back(1.0);
            }
        }
        lattice.push_back(v);
    }
    accepted = 0.0;
    total = 0.0;
    LatticeEnergy = total_lattice_energy(field);
    SquaredLatticeEnergy = LatticeEnergy*LatticeEnergy;
    Magnetisation = net_magnetisation();
    
}



Crystal::~Crystal(){
    
}



void Crystal::print_lattice(){
    for(int i = 0; i < size; i++){
        for( int j = 0; j < size; j++){
            std::cout << lattice[i][j];
        }
        std::cout << std::endl;
    }
}



void Crystal::lattice_to_csv(std::string version){
    std::ofstream outdata;
    std::string fileloc = "/Users/tommciver/Documents/Crystal190420/" + version + ".csv";
    outdata.open(fileloc);
    if( !outdata ) {
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }
    for ( int r = 0; r < size ; r++) {
        for(int w=0; w < size; w++){
            if( w == (size - 1)){outdata << lattice[r][w];}
            else{ outdata << lattice[r][w] << ',';}
        }
        outdata << std::endl;
    }
    outdata.close();
}



void Crystal::lattice_to_pbm(std::string version){
    std::map<float, int> mymap = { { 1.0, 1 }, { -1.0, 0 } };
    std::string fname = "/Users/tommciver/Documents/Crystal190420/" + version + ".pbm";
    std::ofstream outdata(fname.c_str(), std::ios::out | std::ios::binary);
    if (outdata.is_open()) {

        outdata << "P1\n";
        
        outdata << std::to_string(size) << " " << std::to_string(size) << "\n";
        
        for ( int r = 0; r < size ; r++) {
            for(int w = 0; w < size; w++){
                
                if( w == (size - 1)){
                    outdata << mymap.at(lattice[r][w]);
                }
                
                else{
                    outdata << mymap.at(lattice[r][w]) << " ";
                }
            }
            outdata << "\n";
        }
    } else {
        std::cout << "Error. Unable to open " << fname << std::endl;
    }
    outdata.close();
}



void Crystal::update_position(float T, float Beta, float H){
    int i = int(distribution(generator) * size);
    int j = int(distribution(generator) * size);
    float EnergyChange = flip_energy_position(i, j, T, Beta, H);
    total += 1.0;
    if(EnergyChange < 0){
        lattice[i][j] *= -1.0;
        accepted += 1.0;
        Magnetisation += -2*lattice[i][j];
        LatticeEnergy += EnergyChange;
        SquaredLatticeEnergy = LatticeEnergy*LatticeEnergy;

    }
    else if( exp(-EnergyChange*Beta) > distribution(generator)){
        lattice[i][j] *= -1.0;
        accepted += 1.0;
        Magnetisation += -2*lattice[i][j];
        LatticeEnergy += EnergyChange;
        SquaredLatticeEnergy = LatticeEnergy*LatticeEnergy;
    }
}



void Crystal::rest_lattice(float field){
    float rando;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            rando = distribution(generator);
            if(rando <= 0.5){
                lattice[i][j] = -1.0;
            }
            else if(rando > 0.5){
                lattice[i][j] = 1.0;
            }
        }
    }
    accepted = 0.0;
    total = 0.0;
    LatticeEnergy = total_lattice_energy(field);
    SquaredLatticeEnergy = LatticeEnergy*LatticeEnergy;
    Magnetisation = net_magnetisation();
}



void Crystal::initialise_state( float temperature, float field){
    float beta = 1.0 / temperature;
    for( int k = 0; k < int(size*size*100); k++ ){
        // Update Random point:
        update_position(temperature, beta, field);
    }
}



float Crystal::return_acceptance_ratio(){
    return accepted/total;
}



void Crystal::current_lattice_values(float field){
    std::cout << "Running agnetisation: " << Magnetisation << "\n";
    std::cout << "Measured Magnetisation: " << net_magnetisation() << "\n";
    std::cout << "Running Squared Magnetisation: " << Magnetisation*Magnetisation << "\n";
    std::cout << "Measured Squared MagnetisationL " << net_magnetisation()*net_magnetisation() << "\n";
    std::cout << "Lattice Energy: " << LatticeEnergy << "\n";
    std::cout << "Measured Energy: " << total_lattice_energy(field) << "\n";
    std::cout << "Squared Lattice Energy: " << SquaredLatticeEnergy << "\n";
    std::cout << "Measured Squared Energy: " << total_lattice_energy(field)*total_lattice_energy(field) << "\n";
    std::cout << NumMeasurements << "\n";
    std::cout << "\n";
}



void Crystal::observe_dependent_variables(float temperature, float field){
    CumulativeMagnetisation[NumMeasurements] = Magnetisation;
    CumulativeSquaredMagnetisation[NumMeasurements] = Magnetisation*Magnetisation;
    CumulativeLatticeEnergy[NumMeasurements] = LatticeEnergy;
    CumulativeSquaredLatticeEnergy[NumMeasurements] = SquaredLatticeEnergy;
    NumMeasurements +=1;
}



void Crystal::rest_dependent_variables(){
    CumulativeMagnetisation.assign(N, 0.0);
    CumulativeSquaredMagnetisation.assign(N, 0.0);
    CumulativeLatticeEnergy.assign(N, 0.0);
    CumulativeSquaredLatticeEnergy.assign(N, 0.0);
    NumMeasurements = 0;
}



float Crystal::average_magnetisation_per_atom(){
    float AvMag(0.0);
    for (auto& n : CumulativeMagnetisation)
        AvMag += n;
    return AvMag/ (float)(NumMeasurements*size*size);
}



float Crystal::average_absolute_magnetisation_per_atom(){
    float AvMag(0.0);
    for (auto& n : CumulativeMagnetisation)
        AvMag += abs(n);
    return AvMag/ (float)(NumMeasurements*size*size);
}



float Crystal::average_squared_magnetisation_per_atom(){
    float AvSqMag(0.0);
    for (auto& n : CumulativeSquaredMagnetisation)
        AvSqMag += n;
    return abs(AvSqMag)/ (float)(NumMeasurements*size*size);
}



float Crystal::average_net_magnetisation(){
    float AvMag(0.0);
    for (auto& n : CumulativeMagnetisation)
        AvMag += n;
    return (AvMag)/ (float)(NumMeasurements);
}



float Crystal::average_absolute_magnetisation(){
    float AvMag(0.0);
    for (auto& n : CumulativeMagnetisation)
        AvMag += abs(n);
    return (AvMag)/ (float)(NumMeasurements);
}



float Crystal::average_squared_magnetisation(){
    float AvSqMag(0.0);
    for (auto& n : CumulativeSquaredMagnetisation)
        AvSqMag += n;
    return (AvSqMag)/ (float)(NumMeasurements);
}



float Crystal::average_energy_per_atom(){
    float AvEn(0.0);
    for (auto& n : CumulativeLatticeEnergy)
        AvEn += n;
    return AvEn/ (float)(NumMeasurements*size*size);
}



float Crystal::average_squared_energy_per_atom(){
    float AvSqEn(0.0);
    for (auto& n : CumulativeSquaredLatticeEnergy)
        AvSqEn += n;
    return AvSqEn/ (float)(NumMeasurements*size*size);
}



float Crystal::average_energy(){
    float AvEn(0.0);
    for (auto& n : CumulativeLatticeEnergy)
        AvEn += n;
    return AvEn/ (float)(NumMeasurements);
}



float Crystal::average_squared_energy(){
    float AvSqEn(0.0);
    for (auto& n : CumulativeSquaredLatticeEnergy)
        AvSqEn += n;
    return AvSqEn/ (float)(NumMeasurements);
}


float Crystal::heat_capacity(float temperature){
    // Return Heat capacity per atom.
    return ( average_squared_energy() - average_energy()*average_energy() ) / (temperature*temperature*size*size) ;
}



float Crystal::error_magnetisation(){
    float norm = (float)(size*size);
    float average = average_absolute_magnetisation();
    float error(0.0);
    for (auto n : CumulativeMagnetisation)
        error += (abs(n) - average)*(abs(n) - average);
    return sqrt(error/NumMeasurements)/norm;
}



float Crystal::error_squared_magnetisation(){
    float average = average_net_magnetisation();
    float error(0.0);
    for (auto& n : CumulativeSquaredMagnetisation)
        error += (abs(n) - average)*(abs(n) - average);
    return sqrt(error/N)/((float)(size*size));
}



float Crystal::error_energy(){
    float average = average_energy();
    float error(0.0);
    for (auto& n : CumulativeLatticeEnergy)
        error += (n - average)*(n - average);
    return sqrt(error/N)/((float)(size*size));
}



float Crystal::error_squared_energy(){
    float average = average_squared_energy();
    float error(0.0);
    for (auto& n : CumulativeSquaredLatticeEnergy)
        error += (abs(n) - average)*(abs(n) - average);
    return sqrt(error/N)/((float)(size*size));
}


float Crystal::error_heat_capacity(float temperature){
    float beta = 1 / temperature;
    float beta4 = beta*beta*beta*beta;
    float errorsqd = (beta4*pow(error_squared_energy(), 2)) + (4*beta4*pow(error_energy(),2)*pow(average_energy(),2));
    return sqrt(errorsqd)/((float)(size*size));
}



std::tuple<float, float, float, float, float, float, float, float, float, float> Crystal::return_dependent_variables(float temperature){
    return {average_absolute_magnetisation_per_atom(), error_magnetisation(), average_squared_magnetisation_per_atom(), error_squared_magnetisation(), average_energy_per_atom(), error_energy(), average_squared_energy_per_atom(), error_squared_energy(), heat_capacity(temperature), error_heat_capacity(temperature)};
}

float Crystal::return_average_magnetisation(){
    return average_net_magnetisation();
}

float Crystal::return_average_magnetisation_per_atom(){
    return average_magnetisation_per_atom();
}

std::vector<float>  Crystal::return_cumulative_magnetisation_vector(){
    return CumulativeSquaredMagnetisation;
}



// Private functions:


float Crystal::total_lattice_energy(float H){
    float TotalLatEnergy = 0.0;
    for( int i = 0; i < size; i++){
        for( int j = 0; j < size; j++){
            TotalLatEnergy += energy_position(i, j, H);
        }
    }
    return TotalLatEnergy / 2.0;
    //Divide by two to account for float counting- in the above loop the lattice sites are being cycles through
    // meaning the bonds at each lattice site are counted in the energy. Every bond involves two atoms however,
    // meanng that the energy is over counted by a factor of two which must be accounted for.
}



float Crystal::energy_position(int i, int j, float H){
    float Energy;

    if( i ==0 and j == 0 ){
    //        Left hand top corner
        Energy = -J*lattice[0][0]*(lattice[0][1] + lattice[1][0] + lattice[0][size-1] + lattice[size-1][0] - mu*H);
        return Energy;
    }

    else if( i == 0 and j == (size - 1)){
        //        Right hand top corner
        Energy = -J*lattice[0][size - 1]*(lattice[0][size - 2] + lattice[1][size - 1] + lattice[0][0] + lattice[size - 1][size - 1] - mu*H);
        return Energy;
    }

    else if( i == ( size - 1 ) and j == 0){
    //        Left hand bottom corner
        Energy = -J*lattice[size - 1][0]*( lattice[size - 1][1] + lattice[size - 2][0] + lattice[0][0] + lattice[size - 1][size-1] - mu*H);
        return Energy;
    }

    else if( i == (size - 1) and j == (size - 1)){
        //        Right hand bottom corner
        Energy = -J*lattice[size - 1][size - 1]*( lattice[size - 1][size - 2] + lattice[size - 2][size - 1] + lattice[size-1][0] + lattice[0][size - 1] - mu*H);
        return Energy;
    }

    else if( i== 0 ){
    //        Top edge
        Energy = -J*lattice[0][j]*(lattice[0][j + 1] + lattice[0][j - 1] + lattice[1][j] + lattice[size-1][j] - mu*H);
        return Energy;
    }

    else if( i == (size - 1) ){
    //        Bottom edge
        Energy = -J*lattice[size - 1][j]*(lattice[size - 1][j + 1] + lattice[size - 1][j - 1] + lattice[size - 2][j] + lattice[0][j] - mu*H);
        return Energy;
    }

    else if( j == 0 ){
    //        Left egde
        Energy = -J*lattice[i][0]*(lattice[i][1] + lattice[i-1][0] + lattice[i+1][0] + lattice[i][size-1] - mu*H);
        return Energy;
    }

    else if( j == (size - 1) ){
    //        Right edge
        Energy = -J*lattice[i][size - 1]*(lattice[i][size - 2] + lattice[i + 1][size- 1] + lattice[i-1][size - 1] + lattice[i][0] - mu*H);
        return Energy;
    }

    else {
    //        Point in body
        Energy = -J*lattice[i][j]*(lattice[i][j + 1] + lattice[i][j - 1] + lattice[i + 1][j] + lattice[i - 1][j] - mu*H);
        return Energy;
    }
}
    


float Crystal::flip_energy_position(int i, int j, float T, float Beta, float H){
    float InitialEnergy;
    float FlippedEnergy;

    if( i == 0 and j == 0 ){
    //        Left hand top corner
        InitialEnergy = -J*lattice[0][0]*(lattice[0][1] + lattice[1][0] + lattice[0][size-1] + lattice[size-1][0] - mu*H);
        FlippedEnergy = J*lattice[0][0]*(lattice[0][1] + lattice[1][0] + lattice[0][size-1] + lattice[size-1][0] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i == 0 and j == (size - 1)){
    //        Right hand top corner
        InitialEnergy = -J*lattice[0][size - 1]*(lattice[0][size - 2] + lattice[1][size - 1] + lattice[0][0] + lattice[size - 1][size - 1] - mu*H);
        FlippedEnergy = J*lattice[0][size - 1]*(lattice[0][size - 2] + lattice[1][size - 1] + lattice[0][0] + lattice[size - 1][size - 1] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i == ( size - 1 ) and j == 0){
    //        Left hand bottom corner
        InitialEnergy = -J*lattice[size - 1][0]*( lattice[size - 1][1] + lattice[size - 2][0] + lattice[0][0] + lattice[size - 1][size-1] - mu*H);
        FlippedEnergy = J*lattice[size - 1][0]*( lattice[size - 1][1] + lattice[size - 2][0] + lattice[0][0] + lattice[size - 1][size-1] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i == (size - 1) and j == (size - 1)){
    //        Right hand bottom corner
        InitialEnergy = -J*lattice[size - 1][size - 1]*( lattice[size - 1][size - 2] + lattice[size - 2][size - 1] + lattice[size-1][0] + lattice[0][size - 1] - mu*H);
        FlippedEnergy = J*lattice[size - 1][size - 1]*( lattice[size - 1][size - 2] + lattice[size - 2][size - 1] + lattice[size-1][0] + lattice[0][size - 1] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i == 0 ){
    //        Top edge
        InitialEnergy = -J*lattice[0][j]*(lattice[0][j + 1] + lattice[0][j - 1] + lattice[1][j] + lattice[size-1][j] - mu*H);
        FlippedEnergy = J*lattice[0][j]*(lattice[0][j + 1] + lattice[0][j - 1] + lattice[1][j] + lattice[size-1][j] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i == (size - 1) ){
    //        Bottom edge
        InitialEnergy = -J*lattice[size - 1][j]*(lattice[size - 1][j + 1] + lattice[size - 1][j - 1] + lattice[size - 2][j] + lattice[0][j] - mu*H);
        FlippedEnergy = J*lattice[size - 1][j]*(lattice[size - 1][j + 1] + lattice[size - 1][j - 1] + lattice[size - 2][j] + lattice[0][j] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( j == 0 ){
    //        Left egde
        InitialEnergy = -J*lattice[i][0]*(lattice[i][1] + lattice[i-1][0] + lattice[i+1][0] + lattice[i][size-1] - mu*H);
        FlippedEnergy = J*lattice[i][0]*(lattice[i][1] + lattice[i-1][0] + lattice[i+1][0] + lattice[i][size-1] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( j == (size - 1) ){
    //        Right edge
        InitialEnergy = -J*lattice[i][size - 1]*(lattice[i][size - 2] + lattice[i + 1][size- 1] + lattice[i-1][size - 1] + lattice[i][0] - mu*H);
        FlippedEnergy = J*lattice[i][size - 1]*(lattice[i][size - 2] + lattice[i + 1][size- 1] + lattice[i-1][size - 1] + lattice[i][0] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else {
    //        Point in body
        InitialEnergy = -J*lattice[i][j]*(lattice[i][j + 1] + lattice[i][j - 1] + lattice[i + 1][j] + lattice[i - 1][j] - mu*H);
        FlippedEnergy = J*lattice[i][j]*(lattice[i][j + 1] + lattice[i][j - 1] + lattice[i + 1][j] + lattice[i - 1][j] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }
}



float Crystal::count_spins_up(){
    float count1 = 0.0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++){
            if( lattice[i][j] == -1.0){
                count1 += 1.0;
            }
        }
    }
    return count1;
}



float Crystal::count_spins_down(){
    int count1 = 0.0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++){
            if( lattice[i][j] == 1.0){
                count1 += 1.0;
            }
        }
    }
    return count1;
}



float Crystal::net_magnetisation(){
    return (count_spins_up() - count_spins_down());
}
