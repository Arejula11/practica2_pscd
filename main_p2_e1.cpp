/* ----------------------------------------------------------------------------
* File:   practica_1_V1.cpp
* Author: Pablo Angusto Delgado 842255 y Miguel Aréjula Aisa 850068
* Date:   octubre 2022
* Coms:   Parte del práctica 2 de PSCD
*         Compilar mediante
*          make -f Makefile_p2_e1
* ----------------------------------------------------------------------------- */



#include <iostream>
#include <thread>
#include <string>
#include <fstream>

using namespace std;


const int N = 512;
const int N_BUSC = 8; //# de procesos buscadores
using VectInt = int[N];
const int N_INTERVALOS = (N)/N_BUSC;


//-----------------------------------------------------
//Pre: v es un vector de N de enteros  
//Post: el vector v es completado con los números contenidos en el fichero "datos.txt"
void leerFichero(VectInt v){
    ifstream f("datos.txt");
    if(f.is_open()){
        for(int i = 0; i < N; i++){
            f >> v[i];
        }
    }else{
        cout << "error" << endl; // en caso de no completar el vector "v" muestra en pantalla un mensaje de error
    }
}

//-----------------------------------------------------
//Pre: 0 <= i <= d <= N−1
//Post: result = Num α ∈ [i, d].value = v[α]
void search(const VectInt v, const int i, const int d, const int value, int& result){
    for(int j = i; j <= d; j++){
        if(v[j]==value){
            result++; // al ser una variable local para cada proceso se puede ejecutar la suma sin posibles errores
        }
    }
}

//-----------------------------------------------------
//Pre: v es un vector de N enteros, fin_procesos es un vector de N_BUSC booleanos, contador es un vector de N_BUSC enteros
//Post: el vector v es completado con los números contenidos en el fichero "datos.txt" habilitando a los procesos buscadores su ejecución. 
// Una vez terminado la busqueda suma las componentes del vector contador que contiene la información obtenida de los procesos buscadores
// y lo muestra en pantalla.

void coordinador(bool& comenzar, VectInt v, bool fin_procesos[], int contador[]){
    leerFichero(v);
    comenzar = true; //Avisa a los procesos buscadores de la carga de datos
    for(int i = 0; i < N_BUSC; i++){
        while(!fin_procesos[i]){ 
            this_thread::yield();//espera activa, hasta la finalización de los procesos buscadores
        }
    }
    int suma = 0;
    for(int i = 0; i < N_BUSC; i++){
        suma += contador[i]; //suma las componentes del vector rellenado por los procesos buscadores
    }
    cout << "Resultado: " + to_string(suma) + '\n';
    
}

//-----------------------------------------------------
//Pre:  v es un vector de N enteros, fin_procesos es un vector N_BUSC de booleanos, contador es un vectorde N_BUSC booleanos, 0 <= i < N_BUSC
//Post: Una vez el proceso coordinador habilite la ejecución de este proceso rellenará la componente i del vector contador con result = Num α ∈ [i, d].value = v[α]
// y pondrá a true la componente i del vector fin_procesos
void buscador(bool& comenzar, const VectInt v, int i, int value, int contador[], bool fin_procesos[]){
    while(!comenzar){
        this_thread::yield();
    }
    int result = 0;
    search(v,(i)*N_INTERVALOS, (i+1)*N_INTERVALOS-1, value, result);
    contador[i] = result;
    fin_procesos[i] = true;
    
}





int main(){
    bool comenzar = false;
    int contador[N_BUSC];
    bool fin_procesos[N_BUSC] = {false};
    VectInt v;
    bool valCorrec = false;
    int value;
    
    cout << "Introduce un valor entre 1 y 25: "<<endl;
    cin >> value;
    do { //comprueba que el valor introducido se encuentra entre el 1 y el 25
        if(value < 1 || value > 25){
            cout << "El valor no se encuentra entre los valores indicados"<< endl;
            cout << "Vuelve a introudcir un valor:" << endl;
            cin >> value;
        }else{
            valCorrec = true;
        }
    }while(!valCorrec);

    thread P[N_BUSC+1]; //declaración procesos sin poner en marcha
    P[N_BUSC]= thread(&coordinador, ref(comenzar), ref(v), ref(fin_procesos), ref(contador)); // el proceso coordinador se pone en marcha
    for (int i = 0; i < N_BUSC; i++){
        P[i]= thread(&buscador,ref(comenzar), v, i, value,contador,fin_procesos); // los procesos buscadores se ponen en marcha
    }
    
   for (int i = 0; i < N_BUSC+1; i++){
         P[i].join();     
    }
   
    return 0;
}
