#ifndef BOMBA_HPP
#define BOMBA_HPP

#include <iostream>
#include <string>
#include "INVERSOR.hpp"

using namespace std;

class Bomba
{ /*Classe que representa uma bomba no sistema de ETA*/
private:
    string tag;
    string area;
    double vazao_maxima;
    bool operando;

    Inversor *inversor;

public:
    Bomba(string tag_nova, string area_nova, double vazao_maxima_nova, Inversor *inversor_novo = nullptr)
        : tag(tag_nova), area(area_nova), vazao_maxima(vazao_maxima_nova), operando(false), inversor(inversor_novo) {}

    void ligar()
    {
        operando = true;
        cout << "Bomba " << tag << " operando." << endl;
    }

    void desligar()
    {
        operando = false;
        cout << "Bomba " << tag << " desligada." << endl;
    }

    double get_vazao() const
    {
        if (inversor == nullptr)
            return 0.0;

        return vazao_maxima * inversor->get_frequencia() / 100.0;
    }

    bool esta_operando() const
    {
        return operando;
    }
};

#endif