#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>

#include "ETA.hpp"
#include "CONTROLADOR.hpp"

using namespace std;

class Command
{
public:
    virtual void executar() = 0; // Essa linha força que toda classe que herda a classe Command é obrigada a ter a função executar

    virtual ~Command() {}
};

class StartCommand : public Command
{
private:
    ETA *eta; // Ponteiro utilizado para endicar qual ETA esta sendo ativada

public:
    StartCommand(ETA *nova_eta)
        : eta(nova_eta) // Guarda o indereço da Eta nessa variável
    {
    }

    void executar() override
    {
        eta->iniciar_tratamento();
    }
};

class StopCommand : public Command
{
private:
    ETA *eta;

public:
    StopCommand(ETA *nova_eta)
        : eta(nova_eta)
    {
    }

    void executar() override
    {
        eta->parar_tratamento();
    }
};

class SetpointCommand : public Command
{
private:
    Controlador *controlador; // Para alterar o SetPoint no nosso projeto precisamos do nível atual e do novo Setpoint
    sensor_nivel *sensor;
    double novo_setpoint;

public:
    SetpointCommand(Controlador *ctrl, sensor_nivel *s, double sp)
        : controlador(ctrl), sensor(s), novo_setpoint(sp)
    {
    }

    void executar() override
    {
        controlador->set_setpoint(novo_setpoint, sensor->ler_valor());

        cout << "Novo setpoint: " << controlador->get_setpoint() << " m3" << endl;
    }
};

class ToleranciaCommand : public Command
{
private:
    Controlador *controlador;
    double nova_tolerancia;

public:
    ToleranciaCommand(Controlador *novo_controlador, double valor)
        : controlador(novo_controlador), nova_tolerancia(valor)
    {
    }

    void executar() override
    {
        controlador->set_tolerancia(nova_tolerancia);

        cout << "Nova tolerancia: " << controlador->get_tolerancia() << " m3" << endl;
    }
};

class ExitCommand : public Command
{
public:
    ExitCommand() = default;

    void executar() override
    {
        cout << "Encerrando sistema..." << endl;
    }
};

#endif