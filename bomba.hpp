#ifndef BOMBA_HPP
#define BOMBA_HPP

#include <iostream>
#include <string>

using namespace std;

class Bomba
{
private:
    string tag;
    string area;
    double vazao_nominal;
    bool operando;

public:
    Bomba(string tag_nova, string area_nova, double vazao_nominal_nova)
        : tag(tag_nova), area(area_nova), vazao_nominal(vazao_nominal_nova), operando(false) {}

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

    double get_vazao_nominal()
    {
        if (operando)
            return vazao_nominal;
        else
            return 0.0;
    }

    bool esta_operando()
    {
        return operando;
    }
};

#endif