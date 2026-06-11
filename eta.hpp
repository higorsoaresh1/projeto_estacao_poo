#ifndef ETA_HPP
#define ETA_HPP

#include <iostream>
#include <string>

using namespace std;

class ETA
{
private:
    string localizacao;
    bool operando;

public:
    ETA(string localizacao_nova) : localizacao(localizacao_nova), operando(false) {}

    void iniciar_tratamento()
    {
        operando = true;
        cout << "ETA iniciada." << endl;
    }

    void parar_tratamento()
    {
        operando = false;
        cout << "ETA parada." << endl;
    }
};

#endif