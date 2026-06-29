#ifndef ETA_HPP
#define ETA_HPP

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class ETA
{ /*Classe para representar uma Estação de Tratamento de Água.*/
private:
    string localizacao;
    bool operando;

public:
    ETA(string localizacao_nova) : localizacao(localizacao_nova), operando(false)
    {
        /*TRATAMENTO DE ERRO: Garante que a ETA possua uma localização válida*/
        if (localizacao_nova.empty())
        {
            throw invalid_argument("[ERRO ETA] A localização da ETA não pode ser vazia.");
        }
    }

    void iniciar_tratamento()
    {
        /*TRATAMENTO DE ERRO: Alerta se houver tentativa de iniciar algo já ativo*/
        if (operando)
        {
            throw runtime_error("[AVISO ETA] Comando inválido: A ETA já está em operação.");
        }

        operando = true;
        cout << "ETA iniciada." << endl;
    }

    void parar_tratamento()
    {
        /*TRATAMENTO DE ERRO: Alerta se houver tentativa de parar algo já desligado*/
        if (!operando)
        {
            throw runtime_error("[AVISO ETA] Comando inválido: A ETA já está parada.");
        }

        operando = false;
        cout << "ETA parada." << endl;
    }

    bool esta_operando() const
    {
        return operando;
    }
};

#endif