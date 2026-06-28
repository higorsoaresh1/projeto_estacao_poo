#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <iostream>
#include <string>
#include "BOMBA.hpp"
#include "RESERVATORIO.hpp"
#include "VALVULA_CONSUMO.hpp"

using namespace std;

class Sensor
{ /*Classe abstrata para representar sensores no sistema de ETA*/
protected:
    string tag;
    string area;
    double valor_lido;
    double valor_minimo;
    double valor_maximo;

public:
    Sensor(string tag_nova, string area_nova, double valor_lido_novo, double valor_minimo_novo, double valor_maximo_novo)
        : tag(tag_nova), area(area_nova), valor_lido(valor_lido_novo), valor_minimo(valor_minimo_novo), valor_maximo(valor_maximo_novo) {}

    virtual double ler_valor()
    {
        cout << "Lendo valor do sensor " << tag << " na area: " << area << "." << endl;

        return valor_lido;
    }

    double get_valor_minimo() const
    {
        return valor_minimo;
    }

    double get_valor_maximo() const
    {
        return valor_maximo;
    }

    virtual ~Sensor() = default;
};

class sensor_ph : public Sensor // Classe utilizada para testar a falha simulada do programa
{
private:
    bool falha_comunicacao;

public:
    sensor_ph(string tag_nova, string area_nova, double valor_lido_novo, double valor_minimo_novo, double valor_maximo_novo)
        : Sensor(tag_nova, area_nova, valor_lido_novo, valor_minimo_novo, valor_maximo_novo), falha_comunicacao(false)
    {
    }

    void ativar_falha()
    {
        falha_comunicacao = true;
    }

    void reparar_falha()
    {
        falha_comunicacao = false;
    }

    bool possui_falha() const
    {
        return falha_comunicacao;
    }

    double ler_valor() override
    {
        cout << "Lendo valor do sensor de pH "
             << tag
             << endl;

        if (falha_comunicacao)
        {
            cout << "ERRO: Sensor sem comunicacao!" << endl;

            return (double)ID_DUPLA / (-100.0);
        }

        return valor_lido;
    }
};

class sensor_turbidez : public Sensor
{ /*Classe para representar sensor de turbidez*/
public:
    using Sensor::Sensor;

    double ler_valor() override
    {
        cout << "Lendo valor do sensor de turbidez " << tag << " na area: " << area << "." << endl;

        return valor_lido;
    }
};

class sensor_nivel : public Sensor
{ /*Classe para representar sensor de nível*/
private:
    Reservatorio *reservatorio;

public:
    sensor_nivel(string tag_nova, string area_nova, double valor_lido_novo, double valor_minimo_novo, double valor_maximo_novo, Reservatorio *reservatorio_novo)
        : Sensor(tag_nova, area_nova, valor_lido_novo, valor_minimo_novo, valor_maximo_novo), reservatorio(reservatorio_novo) {}

    double ler_valor() override
    { /*Lê o valor do sensor de nível*/
        valor_lido = reservatorio->get_volume_atual();
        cout << "Lendo valor do sensor de nivel " << tag << " na area: " << area << "." << endl;

        return valor_lido;
    }
};

class sensor_vazao : public Sensor
{ /*Classe para representar sensor de vazão*/
private:
    Bomba *bomba = nullptr;
    ValvulaConsumo *valvula = nullptr;

public:
    // Construtor 1: Para monitorar a Bomba (Entrada)
    sensor_vazao(string tag_nova, string area_nova, double valor_minimo_novo, double valor_maximo_novo, Bomba *bomba_nova)
        : Sensor(tag_nova, area_nova, 0.0, valor_minimo_novo, valor_maximo_novo), bomba(bomba_nova) {}

    // Construtor 2: Para monitorar a Válvula de Consumo (Saída)
    sensor_vazao(string tag_nova, string area_nova, double valor_minimo_novo, double valor_maximo_novo, ValvulaConsumo *valvula_nova)
        : Sensor(tag_nova, area_nova, 0.0, valor_minimo_novo, valor_maximo_novo), valvula(valvula_nova) {}

    double ler_valor() override
    {
        if (bomba != nullptr)
        {
            valor_lido = bomba->get_vazao();
            cout << "Lendo sensor de vazao de entrada " << tag << ".\nVazao bomba = " << valor_lido << " m3/ciclo" << endl;
        }
        else if (valvula != nullptr)
        {
            valor_lido = valvula->get_vazao();
            cout << "Lendo sensor de vazao de saida " << tag << ".\nVazao consumo = " << valor_lido << " m3/ciclo" << endl;
        }

        return valor_lido;
    }
};

#endif