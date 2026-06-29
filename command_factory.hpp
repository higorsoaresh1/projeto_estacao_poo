#ifndef COMMAND_FACTORY_HPP
#define COMMAND_FACTORY_HPP

#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

#include "COMMAND.hpp"
#include "CONTROLADOR.hpp"
#include "ETA.hpp"
#include "SENSOR.hpp"
#include "ALARME.hpp"

using namespace std;

class CommandFactory
{       /*Classe responsável para criar comandos.*/
public: /*Utilizar um ponteiro inteligente aqui ajuda no uso do polimorfismo,
        visto que, até seria possivel apenas usar um ponteiro normal, mas assim teriamos que deletar manualmente os comandos criados quando estes estiverem fora de uso */
    static unique_ptr<Command> criarComando(const string &texto, Controlador *controlador, ETA *eta, sensor_nivel *sensor, sensor_ph *sensorPH, alarme_ph *alarmePH)
    {
        /*Iniciar*/
        if (texto == "START")
        {
            return make_unique<StartCommand>(eta);
        }

        /*Parar*/
        if (texto == "STOP")
        {
            return make_unique<StopCommand>(eta);
        }

        /*Fechar*/
        if (texto == "EXIT")
        {
            return make_unique<ExitCommand>();
        }

        /*Definir Setpoint*/
        if (texto.find("SETPOINT=") == 0)
        {
            try
            {
                double novo_setpoint = stod(texto.substr(9)); /*Transforma string para valor double*/
                return make_unique<SetpointCommand>(controlador, sensor, novo_setpoint);
            }
            catch (const invalid_argument &e)
            {
                cerr << "[AVISO] Erro de concorrência: Comando SETPOINT incompleto ou truncado. Ignorado neste ciclo.\n";
                return nullptr;
            }
            catch (const out_of_range &e)
            {
                cerr << "[AVISO] Valor de SETPOINT fora dos limites numéricos suportados.\n";
                return nullptr;
            }
        }

        /*Definir Tolerância */
        if (texto.find("TOLERANCIA=") == 0)
        {
            try
            {
                double nova_tolerancia = stod(texto.substr(11));
                return make_unique<ToleranciaCommand>(controlador, nova_tolerancia);
            }
            catch (const invalid_argument &e)
            {
                cerr << "[AVISO] Erro de concorrência: Comando TOLERANCIA incompleto ou truncado. Ignorado neste ciclo.\n";
                return nullptr;
            }
            catch (const out_of_range &e)
            {
                cerr << "[AVISO] Valor de TOLERANCIA fora dos limites numéricos suportados.\n";
                return nullptr;
            }
        }

        if (texto == "FALHA_CONEXAO_PH")
        {
            return make_unique<FalhaConexaoPhCommand>(sensorPH, alarmePH); // <-- Passa o alarme
        }

        // REPARAR_PH
        if (texto == "REPARAR_PH")
        {
            return make_unique<RepararPhCommand>(sensorPH, alarmePH); // <-- Passa o alarme
        }

        // Comando inválido
        return nullptr;
    }
};

#endif