/* ----------------------------------------------------------------------------
* File:   practica_1_V1.cpp
* Author: PSCD-Unizar
* Date:   septiembre 2020
* Coms:   Parte del práctica 1 de PSCD
*         Compilar mediante
*           g++ practica_1_V1.cpp -o practica_1_V1 -std=c++11 -pthread
* ----------------------------------------------------------------------------- */

#include <iostream>
#include <thread>
#include <string>
#include <fstream>

using namespace std;


const int N = 512;
const int N_BUSC = 8; //# de procesos buscadores
using VectInt = int[N];
const int N_INTERVALOS = (N-1)/N_BUSC;



void leerFichero(VectInt v){
    ifstream f("datos.txt");
    if(f.is_open()){
        for(int i=0; i<N; i++){
            f >> v[i];
            f.ignore();
        }
    }else{
        cout << "error" << endl;
    }
}


void search(const VectInt v, const int i, const int d, const int value, int& result){
    for(int j=i; j <= d; j++){
        if(v[j]==value){
            result++;
        }
    }
}

void coordinador(bool& comenzar, VectInt v, bool fin_procesos[], int contador[]){
    leerFichero(v);
    comenzar = true;
    for(int i=0; i<N_BUSC; i++){
        while(!fin_procesos[i]){
        }
    }
    int suma=0;
    for(int i=0; i < N_BUSC; i++){
        suma+=contador[i];
    }
    cout << "Resultado: " +to_string(suma) + '\n';
    
}

void buscador(bool& comenzar, const VectInt v, int i, int value, int contador[], bool fin_procesos[]){
    while(!comenzar){

    }
    int result = 0;
    search(v,(i-1)*N_INTERVALOS, i*N_INTERVALOS-1, value, result);
    contador[i]= result;
    fin_procesos[i]= true;
    //cout<<"FIN PROCESO: "+ to_string(i) + '\n';
}





int main(){
    bool comenzar = false;
    int contador[N_BUSC];
    bool fin_procesos[N_BUSC]={false};
    VectInt v;
    bool valCorrec = false;
    int value;
    cout<<"Introduce un valor entre 1 y 25: "<<endl;
    cin>>value;
    do {
        if(value<1 || value >25){
        cout<< "El valor no se encuentra entre los valores indicados"<<endl;
        cout<< "Vuelve a introudcir un valor:" <<endl;
        cin>>value;
        }else{
            valCorrec = true;
        }
    }while(!valCorrec);

    thread P[N_BUSC+1];
    P[N_BUSC]= thread(&coordinador, ref(comenzar), ref(v), ref(fin_procesos), ref(contador));
    for (int i = 0; i< N_BUSC; i++){
        P[i]= thread(&buscador,ref(comenzar), v, i, value,contador,fin_procesos);
    }
    
   for (int i = 0; i < N_BUSC+1; i++){
         P[i].join();
         
    }
    cout<<"FIN";
    return 0;
}