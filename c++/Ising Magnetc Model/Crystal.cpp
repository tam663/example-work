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
    accepted =0.0;
    total = 0.0;
    LatticeEnergy = TotalLatticeEnergy(field);
    SquaredLatticeEnergy = LatticeEnergy*LatticeEnergy;
    Magnetisation = NetMagnetisation();
    
}



void Crystal::PrintLattice(){
    for(int i =0; i<size; i++){
        for( int j=0; j<size; j++){
            std::cout << lattice[i][j];
        }
        std::cout << std::endl;
    }
}



void Crystal::SaveLatticeCSV(std::string version){
    std::ofstream outdata;
    std::string fileloc = "/Users/tommciver/Documents/Crystal190420/" + version + ".csv";
    outdata.open(fileloc);
    if( !outdata ) {
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }
    for ( int r=0; r<size ; r++) {
        for(int w=0; w<size; w++){
            if( w == (size -1)){outdata << lattice[r][w];}
            else{ outdata << lattice[r][w] << ',';}
        }
        outdata << std::endl;
    }
    outdata.close();
}



void Crystal::SaveLatticePBM(std::string version){
    // Define map to convert the +/-1.0s in lattice to a binary 1/0 representation for the PBM format.
    std::map<float, int> mymap = { { 1.0, 1 }, { -1.0, 0 } };
    std::string fname = "/Users/tommciver/Documents/Crystal190420/" + version + ".pbm";
    std::ofstream outdata(fname.c_str(), std::ios::out | std::ios::binary);
    if (outdata.is_open()) {

        outdata << "P1\n";
        
        outdata << std::to_string(size) << " " << std::to_string(size) << "\n";
        
        for ( int r=0; r<size ; r++) {
            for(int w=0; w<size; w++){
                
                if( w == (size -1)){
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



void Crystal::UpdatePosition(float T, float Beta, float H){
    int i = int(distribution(generator) * size);
    int j = int(distribution(generator) * size);
    float EnergyChange = FlipEnergyPosition(i, j, T, Beta, H);
    total +=1.0;
    if(EnergyChange < 0){
        lattice[i][j] *= -1.0;
        accepted +=1.0;
        Magnetisation += -2*lattice[i][j];
        LatticeEnergy += EnergyChange;
        SquaredLatticeEnergy = LatticeEnergy*LatticeEnergy;

    }
    else if( exp(-EnergyChange*Beta) > distribution(generator)){
        lattice[i][j] *= -1.0;
        accepted +=1.0;
        Magnetisation += -2*lattice[i][j];
        LatticeEnergy += EnergyChange;
        SquaredLatticeEnergy = LatticeEnergy*LatticeEnergy;
    }
}



void Crystal::Reset(float field){
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
    accepted =0.0;
    total = 0.0;
    LatticeEnergy = TotalLatticeEnergy(field);
    SquaredLatticeEnergy = LatticeEnergy*LatticeEnergy;
    Magnetisation = NetMagnetisation();
}



void Crystal::InitialiseState( float temperature, float field){
    float beta = 1.0 / temperature;
    for( int k = 0; k < int(size*size*100); k++ ){
        // Update Random point:
        UpdatePosition(temperature, beta, field);
    }
}



float Crystal::ReturnAcceptanceRatio(){
    return accepted/total;
}



void Crystal::CurrentLatticeValues(float field){
    std::cout << "Running agnetisation: " << Magnetisation << "\n";
    std::cout << "Measured Magnetisation: " << NetMagnetisation() << "\n";
    std::cout << "Running Squared Magnetisation: " << Magnetisation*Magnetisation << "\n";
    std::cout << "Measured Squared MagnetisationL " << NetMagnetisation()*NetMagnetisation() << "\n";
    std::cout << "Lattice Energy: " << LatticeEnergy << "\n";
    std::cout << "Measured Energy: " << TotalLatticeEnergy(field) << "\n";
    std::cout << "Squared Lattice Energy: " << SquaredLatticeEnergy << "\n";
    std::cout << "Measured Squared Energy: " << TotalLatticeEnergy(field)*TotalLatticeEnergy(field) << "\n";
    std::cout << NumMeasurements << "\n";
    std::cout << "\n";
}



void Crystal::ObserveDependentVariables(float temperature, float field){
    // Observe the total lattice properties, not per atom.
//    CumulativeMagnetisation[NumMeasurements] = abs(NetMagnetisation());
//    CumulativeSquaredMagnetisation[NumMeasurements] = abs(NetMagnetisation())*abs(NetMagnetisation());
//    CumulativeLatticeEnergy[NumMeasurements] = TotalLatticeEnergy(field);
//    CumulativeSquaredLatticeEnergy[NumMeasurements] = TotalLatticeEnergy(field)*TotalLatticeEnergy(field);
    CumulativeMagnetisation[NumMeasurements] = Magnetisation;
    CumulativeSquaredMagnetisation[NumMeasurements] = Magnetisation*Magnetisation;
    CumulativeLatticeEnergy[NumMeasurements] = LatticeEnergy;
    CumulativeSquaredLatticeEnergy[NumMeasurements] = SquaredLatticeEnergy;
    NumMeasurements +=1;
}



void Crystal::ResetDependentVariables(){
    CumulativeMagnetisation.assign(N, 0.0);
    CumulativeSquaredMagnetisation.assign(N, 0.0);
    CumulativeLatticeEnergy.assign(N, 0.0);
    CumulativeSquaredLatticeEnergy.assign(N, 0.0);
    NumMeasurements =0;
}



float Crystal::AverageMagnetisationPerAtom(){
    float AvMag(0.0);
    for (auto& n : CumulativeMagnetisation)
        AvMag += n;
    return AvMag/ (float)(NumMeasurements*size*size);
}



float Crystal::AverageAbsMagnetisationPerAtom(){
    float AvMag(0.0);
    for (auto& n : CumulativeMagnetisation)
        AvMag += abs(n);
    return AvMag/ (float)(NumMeasurements*size*size);
}



float Crystal::AverageSquaredMagnetisationPerAtom(){
    float AvSqMag(0.0);
    for (auto& n : CumulativeSquaredMagnetisation)
        AvSqMag += n;
    return abs(AvSqMag)/ (float)(NumMeasurements*size*size);
}



float Crystal::AverageMagnetisation(){
    float AvMag(0.0);
    for (auto& n : CumulativeMagnetisation)
        AvMag += n;
    return (AvMag)/ (float)(NumMeasurements);
}



float Crystal::AverageAbsMagnetisation(){
    float AvMag(0.0);
    for (auto& n : CumulativeMagnetisation)
        AvMag += abs(n);
    return (AvMag)/ (float)(NumMeasurements);
}



float Crystal::AverageSquaredMagnetisation(){
    float AvSqMag(0.0);
    for (auto& n : CumulativeSquaredMagnetisation)
        AvSqMag += n;
    return (AvSqMag)/ (float)(NumMeasurements);
}



float Crystal::AverageEnergyPerAtom(){
    float AvEn(0.0);
    for (auto& n : CumulativeLatticeEnergy)
        AvEn += n;
    return AvEn/ (float)(NumMeasurements*size*size);
}



float Crystal::AverageSquaredEnergyPerAtom(){
    float AvSqEn(0.0);
    for (auto& n : CumulativeSquaredLatticeEnergy)
        AvSqEn += n;
    return AvSqEn/ (float)(NumMeasurements*size*size);
}



float Crystal::AverageEnergy(){
    float AvEn(0.0);
    for (auto& n : CumulativeLatticeEnergy)
        AvEn += n;
    return AvEn/ (float)(NumMeasurements);
}



float Crystal::AverageSquaredEnergy(){
    float AvSqEn(0.0);
    for (auto& n : CumulativeSquaredLatticeEnergy)
        AvSqEn += n;
    return AvSqEn/ (float)(NumMeasurements);
}


float Crystal::HeatCapacity(float temperature){
    // Return Heat capacity per atom.
    return ( AverageSquaredEnergy() - AverageEnergy()*AverageEnergy() ) / (temperature*temperature*size*size) ;
}



float Crystal::ErrorMagentisation(){
    float norm = (float)(size*size);
    float average = AverageAbsMagnetisation();
    float error(0.0);
    for (auto n : CumulativeMagnetisation)
        error += (abs(n) - average)*(abs(n) - average);
    return sqrt(error/NumMeasurements)/norm;
}



float Crystal::ErrorSquaredMagnetisation(){
    float average = AverageSquaredMagnetisation();
    float error(0.0);
    for (auto& n : CumulativeSquaredMagnetisation)
        error += (abs(n) - average)*(abs(n) - average);
    return sqrt(error/N)/((float)(size*size));
}



float Crystal::ErrorEnergy(){
    float average = AverageEnergy();
    float error(0.0);
    for (auto& n : CumulativeLatticeEnergy)
        error += (n - average)*(n - average);
    return sqrt(error/N)/((float)(size*size));
}



float Crystal::ErrorSquaredEnergy(){
    float average = AverageSquaredEnergy();
    float error(0.0);
    for (auto& n : CumulativeSquaredLatticeEnergy)
        error += (abs(n) - average)*(abs(n) - average);
    return sqrt(error/N)/((float)(size*size));
}


float Crystal::ErrorHeatCapacity(float temperature){
    float beta = 1/temperature;
    float beta4 = beta*beta*beta*beta;
    float errorsqd = (beta4*pow(ErrorSquaredEnergy(), 2)) + (4*beta4*pow(ErrorEnergy(),2)*pow(AverageEnergy(),2));
    return sqrt(errorsqd)/((float)(size*size));
}



std::tuple<float, float, float, float, float, float, float, float, float, float> Crystal::ReturnDependentVariables(float temperature){
    return {AverageAbsMagnetisationPerAtom(), ErrorMagentisation(), AverageSquaredMagnetisationPerAtom(), ErrorSquaredMagnetisation(), AverageEnergyPerAtom(), ErrorEnergy(), AverageSquaredEnergyPerAtom(), ErrorSquaredEnergy(), HeatCapacity(temperature), ErrorHeatCapacity(temperature)};
}

float Crystal::ReturnAverageMagnetisation(){
    return AverageMagnetisation();
}

float Crystal::ReturnAverageMagnetisationPerAtom(){
    return AverageMagnetisationPerAtom();
}

std::vector<float>  Crystal::ReturnCumulativeMagnetisationVector(){
    return CumulativeSquaredMagnetisation;
}



// Private functions:


float Crystal::TotalLatticeEnergy(float H){
    float TotalLatEnergy = 0.0;
    for( int i =0; i < size; i++){
        for( int j = 0; j < size; j++){
            TotalLatEnergy += EnergyPosition(i, j, H);
        }
    }
    return TotalLatEnergy / 2.0;
    //Divide by two to account for float counting- in the above loop the lattice sites are being cycles through
    // meaning the bonds at each lattice site are counted in the energy. Every bond involves two atoms however,
    // meanng that the energy is over counted by a factor of two which must be accounted for.
}



float Crystal::EnergyPosition(int i, int j, float H){
    float Energy;

    if( i ==0 and j ==0 ){
    //        Left hand top corner
        Energy = -J*lattice[0][0]*(lattice[0][1] + lattice[1][0] + lattice[0][size-1] + lattice[size-1][0] - mu*H);
        return Energy;
    }

    else if( i == 0 and j ==(size - 1)){
        //        Right hand top corner
        Energy = -J*lattice[0][size - 1]*(lattice[0][size - 2] + lattice[1][size - 1] + lattice[0][0] + lattice[size - 1][size - 1] - mu*H);
        return Energy;
    }

    else if( i == ( size - 1 ) and j==0){
    //        Left hand bottom corner
        Energy = -J*lattice[size - 1][0]*( lattice[size - 1][1] + lattice[size - 2][0] + lattice[0][0] + lattice[size - 1][size-1] - mu*H);
        return Energy;
    }

    else if( i == (size - 1) and j==(size - 1)){
        //        Right hand bottom corner
        Energy = -J*lattice[size - 1][size - 1]*( lattice[size - 1][size - 2] + lattice[size - 2][size - 1] + lattice[size-1][0] + lattice[0][size - 1] - mu*H);
        return Energy;
    }

    else if( i==0 ){
    //        Top edge
        Energy = -J*lattice[0][j]*(lattice[0][j + 1] + lattice[0][j - 1] + lattice[1][j] + lattice[size-1][j] - mu*H);
        return Energy;
    }

    else if( i==(size - 1) ){
    //        Bottom edge
        Energy = -J*lattice[size - 1][j]*(lattice[size - 1][j + 1] + lattice[size - 1][j - 1] + lattice[size - 2][j] + lattice[0][j] - mu*H);
        return Energy;
    }

    else if( j==0 ){
    //        Left egde
        Energy = -J*lattice[i][0]*(lattice[i][1] + lattice[i-1][0] + lattice[i+1][0] + lattice[i][size-1] - mu*H);
        return Energy;
    }

    else if( j== (size - 1) ){
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
    


float Crystal::FlipEnergyPosition(int i, int j, float T, float Beta, float H){
    float InitialEnergy;
    float FlippedEnergy;

    if( i ==0 and j ==0 ){
    //        Left hand top corner
        InitialEnergy = -J*lattice[0][0]*(lattice[0][1] + lattice[1][0] + lattice[0][size-1] + lattice[size-1][0] - mu*H);
        FlippedEnergy = J*lattice[0][0]*(lattice[0][1] + lattice[1][0] + lattice[0][size-1] + lattice[size-1][0] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i == 0 and j ==(size - 1)){
    //        Right hand top corner
        InitialEnergy = -J*lattice[0][size - 1]*(lattice[0][size - 2] + lattice[1][size - 1] + lattice[0][0] + lattice[size - 1][size - 1] - mu*H);
        FlippedEnergy = J*lattice[0][size - 1]*(lattice[0][size - 2] + lattice[1][size - 1] + lattice[0][0] + lattice[size - 1][size - 1] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i == ( size - 1 ) and j==0){
    //        Left hand bottom corner
        InitialEnergy = -J*lattice[size - 1][0]*( lattice[size - 1][1] + lattice[size - 2][0] + lattice[0][0] + lattice[size - 1][size-1] - mu*H);
        FlippedEnergy = J*lattice[size - 1][0]*( lattice[size - 1][1] + lattice[size - 2][0] + lattice[0][0] + lattice[size - 1][size-1] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i == (size - 1) and j==(size - 1)){
    //        Right hand bottom corner
        InitialEnergy = -J*lattice[size - 1][size - 1]*( lattice[size - 1][size - 2] + lattice[size - 2][size - 1] + lattice[size-1][0] + lattice[0][size - 1] - mu*H);
        FlippedEnergy = J*lattice[size - 1][size - 1]*( lattice[size - 1][size - 2] + lattice[size - 2][size - 1] + lattice[size-1][0] + lattice[0][size - 1] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i==0 ){
    //        Top edge
        InitialEnergy = -J*lattice[0][j]*(lattice[0][j + 1] + lattice[0][j - 1] + lattice[1][j] + lattice[size-1][j] - mu*H);
        FlippedEnergy = J*lattice[0][j]*(lattice[0][j + 1] + lattice[0][j - 1] + lattice[1][j] + lattice[size-1][j] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( i==(size - 1) ){
    //        Bottom edge
        InitialEnergy = -J*lattice[size - 1][j]*(lattice[size - 1][j + 1] + lattice[size - 1][j - 1] + lattice[size - 2][j] + lattice[0][j] - mu*H);
        FlippedEnergy = J*lattice[size - 1][j]*(lattice[size - 1][j + 1] + lattice[size - 1][j - 1] + lattice[size - 2][j] + lattice[0][j] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( j==0 ){
    //        Left egde
        InitialEnergy = -J*lattice[i][0]*(lattice[i][1] + lattice[i-1][0] + lattice[i+1][0] + lattice[i][size-1] - mu*H);
        FlippedEnergy = J*lattice[i][0]*(lattice[i][1] + lattice[i-1][0] + lattice[i+1][0] + lattice[i][size-1] - mu*H);
        return FlippedEnergy - InitialEnergy ;
    }

    else if( j== (size - 1) ){
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



float Crystal::CountSpinsUp(){
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



float Crystal::CountSpinsDown(){
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



float Crystal::NetMagnetisation(){
    return (CountSpinsUp() - CountSpinsDown());
}
