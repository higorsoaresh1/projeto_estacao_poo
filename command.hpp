#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include "ETA.hpp"
#include "CONTROLADOR.hpp"
#include "SENSOR.hpp"
#include "ALARME.hpp"

using namespace std;

class Command
{
public:
    virtual void executar() = 0;
    virtual ~Command() {}
};

class StartCommand : public Command
{
private:
    ETA *eta;

public:
    StartCommand(ETA *nova_eta) : eta(nova_eta) {}

    void executar() override
    {
        /*TRATAMENTO: Garante que a ETA existe antes de ligar*/
        if (!eta)
        {
            throw runtime_error("[ERRO COMANDO] Não foi possível iniciar: ponteiro da ETA é nulo.");
        }
        eta->iniciar_tratamento();
    }
};

class StopCommand : public Command
{
private:
    ETA *eta;

public:
    StopCommand(ETA *nova_eta) : eta(nova_eta) {}

    void executar() override
    {
        /*TRATAMENTO: Garante que a ETA existe antes de desligar*/
        if (!eta)
        {
            throw runtime_error("[ERRO COMANDO] Não foi possível parar: ponteiro da ETA é nulo.");
        }
        eta->parar_tratamento();
    }
};

class SetpointCommand : public Command
{
private:
    Controlador *controlador;
    sensor_nivel *sensor;
    double novo_setpoint;

public:
    SetpointCommand(Controlador *ctrl, sensor_nivel *s, double sp)
        : controlador(ctrl), sensor(s), novo_setpoint(sp) {}

    void executar() override
    {
        /*TRATAMENTO: Evita crash se o hardware do controlador ou sensor sumir da memória*/
        if (!controlador || !sensor)
        {
            throw runtime_error("[ERRO COMANDO] Falha ao alterar Setpoint: Controlador ou Sensor inválido.");
        }

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
        : controlador(novo_controlador), nova_tolerancia(valor) {}

    void executar() override
    {
        /*TRATAMENTO: Garante consistência do ponteiro do controlador*/
        if (!controlador)
        {
            throw runtime_error("[ERRO COMANDO] Falha ao alterar Tolerância: Controlador inválido.");
        }

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

class FalhaConexaoPhCommand : public Command
{
private:
    sensor_ph *sensorPH;
    alarme_ph *alarmePH;

public:
    FalhaConexaoPhCommand(sensor_ph *s, alarme_ph *a) : sensorPH(s), alarmePH(a) {}

    void executar() override
    {
        /*TRATAMENTO: Garante integridade do teste de falha injetada*/
        if (!sensorPH || !alarmePH)
        {
            throw runtime_error("[ERRO COMANDO] Falha ao injetar erro: Sensor ou Alarme de pH nulo.");
        }
        cout << "🚨 FALHA INJETADA: Link Modbus do PH-101 rompido!" << endl;

        sensorPH->ativar_falha();
        alarmePH->disparar();
    }
};

class RepararPhCommand : public Command
{
private:
    sensor_ph *sensorPH;
    alarme_ph *alarmePH;

public:
    RepararPhCommand(sensor_ph *s, alarme_ph *a) : sensorPH(s), alarmePH(a) {}

    void executar() override
    {
        /*TRATAMENTO: Garante integridade do comando de manutenção*/
        if (!sensorPH || !alarmePH)
        {
            throw runtime_error("[ERRO COMANDO] Falha ao reparar: Sensor ou Alarme de pH nulo.");
        }
        cout << "🔧 MANUTENÇÃO: Sensor PH-101 reconectado com sucesso." << endl;

        sensorPH->reparar_falha();
        alarmePH->silenciar();
    }
};

#endif