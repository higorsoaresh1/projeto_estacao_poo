#ifndef COMMAND_FACTORY_HPP
#define COMMAND_FACTORY_HPP

#include <memory>
#include <string>

#include "COMMAND.hpp"
#include "CONTROLADOR.hpp"
#include "ETA.hpp"
#include "SENSOR.hpp"

using namespace std;

class CommandFactory // Essa classe apenas serve para criar os comandos
{
public: /*Utilizar um ponteiro inteligente aqui ajuda no uso do polimorfismo,
        visto que, até seria possivel apenas usar um ponteiro normal, mas assim teriamos que deletar manualmente os comandos criados quando estes estiverem fora de uso */
    static unique_ptr<Command> criarComando(const string &texto, Controlador *controlador, ETA *eta, sensor_nivel *sensor)
    {
        // START

        if (texto == "START")
        {
            return make_unique<StartCommand>(eta);
        }

        // ==========================
        // STOP
        // ==========================
        if (texto == "STOP")
        {
            return make_unique<StopCommand>(eta);
        }

        // ==========================
        // EXIT
        // ==========================
        if (texto == "EXIT")
        {
            return make_unique<ExitCommand>();
        }

        // ==========================
        // SETPOINT
        // ==========================
        if (texto.find("SETPOINT=") == 0)
        {
            double novo_setpoint = stod(texto.substr(9)); // Transforma string para valor inteiro

            return make_unique<SetpointCommand>(controlador, sensor, novo_setpoint);
        }

        // ==========================
        // TOLERÂNCIA
        // ==========================
        if (texto.find("TOLERANCIA=") == 0)
        {
            double nova_tolerancia = stod(texto.substr(11));

            return make_unique<ToleranciaCommand>(controlador, nova_tolerancia);
        }

        // Comando inválido
        return nullptr;
    }
};

#endif