#ifndef VALVULA_HPP
#define VALVULA_HPP

#include <iostream>
#include <string>
#include <stdexcept>

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
        : tag(tag_nova), area(area_nova), vazao_alivio(vazao_alivio_nova), aberta(false)
    {
        /*TRATAMENTO DE ERRO: Garante integridade na inicialização do hardware de segurança*/
        if (tag_nova.empty() || area_nova.empty())
        {
            throw invalid_argument("[ERRO VÁLVULA] A 'tag' e a 'area' da válvula não podem ser vazias.");
        }
        if (vazao_alivio_nova <= 0.0)
        {
            throw invalid_argument("[ERRO VÁLVULA] A vazão de alívio deve ser maior que zero para garantir o escoamento de segurança.");
        }
    }

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

    bool esta_aberta() const
    {
        return aberta;
    }

    double get_vazao_alivio() const
    { /*Retorna a vazão da válvula se estiver aberta, caso contrário retorna 0.0*/
        if (aberta)
            return vazao_alivio;
        else
            return 0.0;
    }
};

#endif