#ifndef RESERVATORIO_HPP
#define RESERVATORIO_HPP

#include <iostream>
#include <string>

using namespace std;

class Reservatorio
{
private:
    string tag;
    string area;
    double volume_maximo;
    double volume_atual;
    double consumo;

public:
    Reservatorio(string tag_nova, string area_nova, double volume_maximo_novo, double volume_atual_novo, double consumo_novo)
        : tag(tag_nova), area(area_nova), volume_maximo(volume_maximo_novo), volume_atual(volume_atual_novo), consumo(consumo_novo) {}

    double get_volume_atual()
    {
        return volume_atual;
    }

    void encher_reservatorio(double volume)
    {
        if (volume_atual + volume <= volume_maximo)
        {
            volume_atual += volume;
            cout << "Reservatorio " << tag << " enchido. Volume atual: " << volume_atual << " m³." << endl;
        }
        else
        {
            cout << "Não é possível encher o reservatorio " << tag << ". Volume excede o maximo." << endl;
        }
    }

    void esvaziar_reservatorio(double volume)
    {
        if (volume_atual - volume >= 0)
        {
            volume_atual -= volume;
            cout << "Reservatorio " << tag << " esvaziado. Volume atual: " << volume_atual << " m³." << endl;
        }
        else
        {
            cout << "Não é possível esvaziar o reservatorio " << tag << ". Volume insuficiente." << endl;
        }
    }

    void atualizar_volume(double novo_volume)
    {
        volume_atual = novo_volume;
    }

    void set_consumo(double novo_consumo)
    {
        consumo = novo_consumo;
    }

    double get_consumo()
    {
        return consumo;
    }
};

#endif