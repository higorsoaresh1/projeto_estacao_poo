#ifndef INVERSOR_HPP
#define INVERSOR_HPP

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class Inversor
{
private:
    string tag;
    string area;
    double frequencia;

public:
    Inversor(string tag_nova, string area_nova) : tag(tag_nova), area(area_nova), frequencia(0)
    {
        /*TRATAMENTO DE ERRO: Garante consistência na inicialização do hardware simulado*/
        if (tag_nova.empty() || area_nova.empty())
        {
            throw invalid_argument("[ERRO INVERSOR] A 'tag' e a 'area' do inversor não podem ser vazias.");
        }
    }

    void set_frequencia(double nova)
    {
        /*TRATAMENTO/AVISO: Alerta o operador sobre saturação nos limites físicos*/
        if (nova < 0.0)
        {
            cerr << "[AVISO INVERSOR] " << tag << ": Comando recebido menor que 0% (" << nova << "%). Limitando em 0.0%." << endl;
            nova = 0.0;
        }
        else if (nova > 100.0)
        {
            cerr << "[AVISO INVERSOR] " << tag << ": Comando recebido maior que 100% (" << nova << "%). Limitando em 100.0%." << endl;
            nova = 100.0;
        }

        frequencia = nova;
    }

    double get_frequencia() const
    {
        return frequencia;
    }
};

#endif