#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <iostream>
#include <string>

#include "ALARME.hpp"
#include "BOMBA.hpp"
#include "RESERVATORIO.hpp"
#include "SENSOR.hpp"
#include "VALVULA.hpp"

using namespace std;

class Controlador
{ /*Classe para representar o controlador no sistema de ETA*/
private:
    string tag;

public:
    Controlador(string tag_nova) : tag(tag_nova) {}

    void controlar_nivel(sensor_nivel *sensor, Reservatorio *reservatorio, Bomba *bomba, Valvula *valvula)
    {
        double setpoint = 700;
        double tolerancia = 80;
        double nivel = sensor->ler_valor();

        // Controle principal
        if (nivel < setpoint - tolerancia)
        { /*Verifica se o nível está abaixo do setpoint menos a tolerância*/
            bomba->ligar();
        }
        else if (nivel > setpoint + tolerancia)
        {
            bomba->desligar();
        }

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
        reservatorio->encher_reservatorio(bomba->get_vazao_nominal());
        reservatorio->esvaziar_reservatorio(valvula->get_vazao_alivio());
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