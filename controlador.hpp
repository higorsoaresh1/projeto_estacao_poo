#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <iostream>
#include <string>

#include "ALARME.hpp"
#include "BOMBA.hpp"
#include "RESERVATORIO.hpp"
#include "SENSOR.hpp"
#include "VALVULA.hpp"
#include "VALVULA_CONSUMO.hpp"

using namespace std;

class Controlador
{ /*Classe para representar o controlador no sistema de ETA*/
private:
    string tag;
    double setpoint;
    double tolerancia;
    double kp;
    double ki;
    double erro_integral;

public:
    Controlador(string tag_nova, double setpoint_novo, double tolerancia_nova, double kp_novo, double ki_novo)
        : tag(tag_nova), setpoint(setpoint_novo), tolerancia(tolerancia_nova), kp(kp_novo), ki(ki_novo), erro_integral(0) {}

    void set_setpoint(double novo_setpoint)
    {
        setpoint = novo_setpoint;
        erro_integral = 0;
    }

    double get_setpoint() const
    {
        return setpoint;
    }

    double get_tolerancia() const
    {
        return tolerancia;
    }

    void set_tolerancia(double nova_tolerancia)
    {
        tolerancia = nova_tolerancia;
    }

    void controlar_nivel(sensor_nivel *sensor, Reservatorio *reservatorio, Bomba *bomba, Valvula *valvula, Inversor *inversor, ValvulaConsumo *consumo)
    {
        double nivel = sensor->ler_valor();

        double erro = setpoint - nivel;

        double proporcional = kp * erro;

        double integral = ki * erro_integral;

        double saida = proporcional + integral;

        if (saida > 100)
        {
            saida = 100;
        }
        else if (saida < 0)
        {
            saida = 0;
        }
        else
        {
            erro_integral += erro;
        }

        inversor->set_frequencia(saida);

        if (saida > 0)
            bomba->ligar();
        else
            bomba->desligar();

        if (nivel > 950)
        {
            valvula->abrir();
        }
        else if (nivel < 900)
        {
            valvula->fechar();
        }

        /*Essas funções são chamadas para atualizar o volume do reservatório, e não precisa necessariamente,
        estarem reservados aos if, pois o que fiz se o volume esta sendo modificado é a variável bool ativo e bool operando
        da bomba e da válvula presentes nos get.*/
        reservatorio->encher_reservatorio(bomba->get_vazao());
        reservatorio->esvaziar_reservatorio(valvula->get_vazao_alivio());

        reservatorio->esvaziar_reservatorio(consumo->get_vazao());
    }

    void monitorar(sensor_ph *sensor_ph, sensor_nivel *sensor_nivel, sensor_turbidez *sensor_turbidez, sensor_vazao *sensor_vazao,
                   alarme_ph *alarme_ph, alarme_nivel_alto *alarme_nivel_alto, alarme_vazao *alarme_vazao, alarme_turbidez *alarme_turbidez)
    { /*Método responsável por monitorar os sensores do sistema*/
        double ph = sensor_ph->ler_valor();
        double nivel = sensor_nivel->ler_valor();
        double turbidez = sensor_turbidez->ler_valor();
        double vazao = sensor_vazao->ler_valor();

        // Monitoramento sensor pH.
        if (ph > sensor_ph->get_valor_maximo())
        {
            cout << "pH alto detectado! \n"
                 << "pH Lido: " << ph << endl;
            alarme_ph->disparar();
        }
        else if (ph < sensor_ph->get_valor_minimo())
        {
            cout << "pH baixo detectado! \n"
                 << "pH Lido: " << ph << endl;
            alarme_ph->disparar();
        }
        else
        {
            alarme_ph->silenciar();
        }

        // Monitoramento sensor de nível.
        if (nivel > sensor_nivel->get_valor_maximo())
        {
            cout << "Nivel alto detectado! \n"
                 << "Nivel Lido: " << nivel << endl;
            alarme_nivel_alto->disparar();
        }
        else if (nivel < sensor_nivel->get_valor_minimo())
        {
            cout << "Nivel baixo detectado! \n"
                 << "Nivel Lido: " << nivel << endl;
            alarme_nivel_alto->disparar();
        }
        else
        {
            alarme_nivel_alto->silenciar();
        }

        // Monitoramento sensor de vazão.
        if (vazao > sensor_vazao->get_valor_maximo())
        {
            cout << "Vazao alta detectada! \n"
                 << "Vazao Lida: " << vazao << endl;
            alarme_vazao->disparar();
        }
        else if (vazao < sensor_vazao->get_valor_minimo())
        {
            cout << "Vazao baixa detectada! \n"
                 << "Vazao Lida: " << vazao << endl;
            alarme_vazao->disparar();
        }
        else
        {
            alarme_vazao->silenciar();
        }

        // Monitoramento sensor de turbidez.
        if (turbidez > sensor_turbidez->get_valor_maximo())
        {
            cout << "Turbidez alta detectada! \n"
                 << "Turbidez Lida: " << turbidez << endl;
            alarme_turbidez->disparar();
        }
        else if (turbidez < sensor_turbidez->get_valor_minimo())
        {
            cout << "Turbidez baixa detectada! \n"
                 << "Turbidez Lida: " << turbidez << endl;
            alarme_turbidez->disparar();
        }
        else
        {
            alarme_turbidez->silenciar();
        }
    }
};

#endif