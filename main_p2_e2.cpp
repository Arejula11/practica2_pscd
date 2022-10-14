/* ----------------------------------------------------------------------------
* File:   practica_1_V1.cpp
* Author: Pablo Angusto Delgado 842255 y Miguel Aréjula Aisa 850068
* Date:   octubre 2022
* Coms:   Parte del práctica 2 de PSCD
*         Compilar mediante
*           make -f Makefile_p2_e2
* ----------------------------------------------------------------------------- */

#include <iostream>
#include <thread>
#include <string>
#include <atomic>
#include <fstream>


using namespace std;
const int N = 512;
const int N_BUSC = 8; //# de procesos buscadores
using VectInt = int[N];
const int N_INTERVALOS = (N)/N_BUSC;

//-----------------------------------------------------
//Pre: v es un vector de N de enteros  
//Post: v es completado con los números contenidos en el fichero "datos.txt"
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
//Pre: v es un vector de N enteros, 
//Post: maxVeces = num de repeticiones del intervalo en el que mas aparece repetido value, intMin = menor indice del vector v en el que se encuentra value e 
//intMin = mayor indice del vector v en el que se encuentra value
void search(const VectInt v, const int i, const int d, const int value, int& maxVeces,
            int& indMin, int& indMax, atomic_flag& tas){
        int cont = 0;
        
        for(int j = i; j <= d; j++){
           
            if(v[j]==value){
                cont++; // al ser una variable local en la función de cada proceso se puede ejecutar la suma sin posibles errores
                while(tas.test_and_set()){
                        this_thread::yield();
                    }
                    if (cont>maxVeces){ 
                        
                        maxVeces = cont;
                    
                    }
                tas.clear();
                while(tas.test_and_set()){
                        this_thread::yield();
                    }
                if (indMax < j){
                    
                    indMax = j;
                    
                }
                tas.clear();
                while(tas.test_and_set()){
                        this_thread::yield();
                }        
                if(indMin>j){
                    indMin = j;
                    
                }
                tas.clear();
            }
            
    }
       
}

//-----------------------------------------------------
//Pre: v es un vector de N enteros, fin_procesos es un vector N_BUSC booleanos
//Post: el vector v es completado con los números contenidos en el fichero "datos.txt" habilitando a los procesos buscadores su ejecución. 
// Una vez terminado la busqueda muesetra en pantalla las variables maxVeces, indMin e indMax
void coordinador(bool& comenzar, VectInt v, bool fin_procesos[], int& maxVeces,
            int& indMin, int& indMax){
    leerFichero(v);
    comenzar = true; //Avisa a los procesos buscadores de la carga de datos
    for(int i = 0; i < N_BUSC; i++){
        while(!fin_procesos[i]){
            this_thread::yield(); //espera activa, hasta la finalización de los procesos buscadores
        }
    }
    cout << "Máximas : " + to_string(maxVeces) + '\n';
    cout << "Índice mínimo: " + to_string(indMin) + '\n';
    cout << "Índice máximo: " + to_string(indMax) + '\n';
}

//-----------------------------------------------------
//Pre: v es un vector de N enteros, fin_procesos es un vector N_BUSC booleanos  y 0 <= i < N_BUSC
//Post: Una vez el proceso coordinador habilite la ejecución de este proceso llamará a la función search, una vez acabda la busqueda
// pondrá a true la componente i del vector fin_procesos
void buscador(bool& comenzar, const VectInt v, int i, int value, bool fin_procesos[],  int& maxVeces,
            int& indMin, int& indMax, atomic_flag& tas){
                
    while(!comenzar){
        this_thread::yield();
    }
    search(v,(i)*N_INTERVALOS, (i+1)*N_INTERVALOS-1, value, maxVeces, indMin, indMax, tas); 
    fin_procesos[i]= true;
    
    
}

int main(){
    int maxVeces = 0;
    int indMin = N;
    int indMax = 0;
    bool comenzar = false;
    bool fin_procesos[N_BUSC] = {false};
    VectInt v;
    bool valCorrec = false;
    int value;
    atomic_flag tas  = ATOMIC_FLAG_INIT;
    
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
    P[N_BUSC]= thread(&coordinador, ref(comenzar), ref(v), ref(fin_procesos), ref(maxVeces), ref(indMin), ref(indMax)); // el proceso coordinador se pone en marcha
    for (int i = 0; i < N_BUSC; i++){
        P[i]= thread(&buscador,ref(comenzar), v, i, value,fin_procesos,ref(maxVeces), ref(indMin), ref(indMax),ref(tas)); // los procesos buscadores se ponen en marcha
    }
    
   for (int i = 0; i < N_BUSC+1; i++){
         P[i].join(); 
    }

    return 0;
}