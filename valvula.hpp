#ifndef VALVULA_HPP
#define VALVULA_HPP

#include <iostream>
#include <string>

using namespace std;

class Valvula
{ /*Classe que representa uma válvula no sistema de ETA*/
private:
    string tag;
    string area;
    double vazao_alivio;
    bool aberta;

public:
    Valvula(string tag_nova, string area_nova, double vazao_alivio_nova)
        : tag(tag_nova), area(area_nova), vazao_alivio(vazao_alivio_nova), aberta(false) {}

    void abrir()
    {
        aberta = true;
        cout << "Valvula de alivio " << tag << " aberta." << endl;
    }

    void fechar()
    {
        aberta = false;
        cout << "Valvula de alivio " << tag << " fechada." << endl;
    }

    bool esta_aberta()
    {
        return aberta;
    }

    double get_vazao_alivio()
    { /*Retorna a vazão da válvula se estiver aberta, caso contrário retorna 0.0*/
        if (aberta)
            return vazao_alivio;
        else
            return 0.0;
    }
};

#endif