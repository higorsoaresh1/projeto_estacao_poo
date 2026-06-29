#ifndef ETA_HPP
#define ETA_HPP

#include <iostream>
#include <string>

using namespace std;

class ETA
{ /*Classe para representar uma Estação de Tratamento de Água.*/
private:
    string localizacao;
    bool operando;

public:
    ETA(string localizacao_nova) : localizacao(localizacao_nova), operando(false) {}

    void iniciar_tratamento()
    {
        if (!operando)
        {
            operando = true;
            cout << "ETA iniciada." << endl;
        }
    }

    void parar_tratamento()
    {
        if (operando)
        {
            operando = false;
            cout << "ETA parada." << endl;
        }
    }

    bool esta_operando() const
    {
        return operando;
    }
};

#endif