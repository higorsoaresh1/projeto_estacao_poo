#ifndef INVERSOR_HPP
#define INVERSOR_HPP

#include <iostream>
#include <string>

using namespace std;

class Inversor
{
private:
    string tag;
    string area;

    double frequencia;

public:
    Inversor(string tag_nova, string area_nova) : tag(tag_nova), area(area_nova), frequencia(0) {}

    void set_frequencia(double nova)
    {
        if (nova < 0)
            nova = 0;

        if (nova > 100)
            nova = 100;

        frequencia = nova;
    }

    double get_frequencia() const
    {
        return frequencia;
    }
};

#endif