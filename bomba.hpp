#ifndef BOMBA_HPP
#define BOMBA_HPP

#include <iostream>
#include <string>
#include <stdexcept>
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
        : tag(tag_nova), area(area_nova), vazao_maxima(vazao_maxima_nova), operando(false), inversor(inversor_novo)
    {
        /*TRATAMENTO DE ERRO: Valida os dados de inicialização da bomba*/
        if (tag_nova.empty() || area_nova.empty())
        {
            throw invalid_argument("[ERRO BOMBA] A 'tag' e a 'area' da bomba não podem ser vazias.");
        }
        if (vazao_maxima_nova <= 0.0)
        {
            throw invalid_argument("[ERRO BOMBA] A vazão máxima da bomba deve ser maior que zero.");
        }
    }

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
        /*Se abomba estiver desligada, a vazão é zero.*/
        if (!operando)
            return 0.0;

        /*TRATAMENTO DE ERRO: Lança uma exceção se a bomba tentar funcionar sem um inversor associado*/
        if (inversor == nullptr)
        {
            throw runtime_error("[ERRO CRÍTICO] Falha física: Tentativa de ler vazão da bomba " + tag + " sem inversor acoplado.");
        }

        return vazao_maxima * inversor->get_frequencia() / 100.0;
    }

    bool esta_operando() const
    {
        return operando;
    }
};

#endif