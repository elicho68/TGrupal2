#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Estructura para almacenar una entrada de traducción
struct Traduccion {
    string palabra;
    string traduccion;
};

// Función para cargar las traducciones desde el archivo
map<string, string> cargarTraducciones() {
    map<string, string> traducciones;
    ifstream archivo("traducciones.txt");
    if (!archivo) {
        cerr << "No se pudo abrir el archivo de traducciones." << endl;
        return traducciones;
    }
    string palabra, traduccion;
    while (archivo >> palabra >> traduccion) {
        traducciones[palabra] = traduccion;
    }
    archivo.close();
    return traducciones;
}

// Función para traducir una línea de código
string traducirLinea(const string& linea, const map<string, string>& diccionario) {
    stringstream ss(linea);
    string palabra;
    string nuevaLinea;
    bool dentroDeMetodo = false;

    while (ss >> palabra) {
        if (palabra == "{") {
            dentroDeMetodo = true;
            nuevaLinea += "{ ";
        } else if (palabra == "}") {
            dentroDeMetodo = false;
            nuevaLinea += "} ";
        } else {
            if (!dentroDeMetodo) {
                if (diccionario.count(palabra)) {
                    nuevaLinea += diccionario.at(palabra) + " ";
                } else {
                    nuevaLinea += palabra + " ";
                }
            } else {
                nuevaLinea += palabra + " ";
            }
        }
    }
    return nuevaLinea;
}


// Función para traducir el código completo
string traducirCodigo(const string& codigo, const map<string, string>& diccionario) {
    stringstream ss(codigo);
    string linea;
    string codigoTraducido;
    bool dentroDeFuncion = false;

    while (getline(ss, linea)) {
        if (linea.find("function") != string::npos || linea.find("void") != string::npos) {
            dentroDeFuncion = true;
        } else if (linea.find("}") != string::npos) {
            dentroDeFuncion = false;
        }

        if (!dentroDeFuncion) {
            codigoTraducido += traducirLinea(linea, diccionario) + "\n";
        } else {
            codigoTraducido += linea + "\n";
        }
    }
    return codigoTraducido;
}

int main() {
    map<string, string> diccionario = cargarTraducciones();

    cout << "Ingrese el código C++ (presione Ctrl + D para finalizar la entrada):" << endl;
    string codigo;
    string linea;
    while (getline(cin, linea)) {
        codigo += linea + "\n";
    }

    string codigoTraducido = traducirCodigo(codigo, diccionario);
    cout << "Código traducido:" << endl;
    cout << codigoTraducido << endl;

    return 0;
}

