#ifndef VALVULA_CONSUMO_HPP
#define VALVULA_CONSUMO_HPP

#include <iostream>
#include <string>

using namespace std;

class ValvulaConsumo
{
private:
    string tag;
    string area;
    double abertura;
    double vazao_maxima;

public:
    ValvulaConsumo(string tag_nova, string area_nova, double abertura_nova, double vazao_maxima_nova)
        : tag(tag_nova), area(area_nova), abertura(abertura_nova), vazao_maxima(vazao_maxima_nova) {}

    void set_abertura(double nova_abertura)
    {
        if (nova_abertura < 0)
            nova_abertura = 0;

        if (nova_abertura > 1)
            nova_abertura = 1;

        abertura = nova_abertura;
    }

    double get_abertura() const
    {
        return abertura;
    }

    double get_vazao()
    {
        return vazao_maxima * abertura;
    }
};

#endif