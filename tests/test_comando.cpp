#ifndef ID_DUPLA
#define ID_DUPLA 80
#endif

#include <iostream>
#include <cassert>
#include <string>
#include <memory>
#include "../COMMAND.hpp"
#include "../COMMAND_FACTORY.hpp"
#include "../CONTROLADOR.hpp"
#include "../ETA.hpp"
#include "../SENSOR.hpp"

using namespace std;

void deve_processar_comando_setpoint_valido()
{
    Controlador ctrl("CTRL-101", 700.0, 80, 0.8, 0.03);
    ETA eta("Area 1");
    Reservatorio res("TK-101", "Area 1", 1000.0, 500.0);
    sensor_nivel sn("LT-101", "Area 1", 0.0, 150.0, 1000.0, &res);
    sensor_ph sph("PH-101", "Area 1", 7.0, 6.0, 8.0);
    alarme_ph alh("AH-PH", "Area 1");

    // DICA: Altere esta string abaixo para o formato EXATO que a sua
    // CommandFactory::criarComando trata dentro do arquivo "COMMAND_FACTORY.hpp".
    // Exemplos comuns: "SETPOINT 550.0", "setpoint 550.0", "SETPOINT=550.0"
    string texto_comando = "SETPOINT=550.0";

    auto comando = CommandFactory::criarComando(texto_comando, &ctrl, &eta, &sn, &sph, &alh);

    // Se a sua factory exige uma ordem diferente de parâmetros,
    // alinhe-os com a assinatura do seu método criarComando().

    assert(comando != nullptr);
    comando->executar();

    assert(ctrl.get_setpoint() == 550.0);
}

void deve_processar_comando_stop_e_start()
{
    Controlador ctrl("CTRL-101", 700.0, 80, 0.8, 0.03);
    ETA eta("Area 1");
    Reservatorio res("TK-101", "Area 1", 1000.0, 500.0);
    sensor_nivel sn("LT-101", "Area 1", 0.0, 150.0, 1000.0, &res);
    sensor_ph sph("PH-101", "Area 1", 7.0, 6.0, 8.0);
    alarme_ph alh("AH-PH", "Area 1");

    // 1. A ETA começa DESLIGADA no construtor. Vamos validar isso:
    assert(eta.esta_operando() == false);

    // 2. Testando o comando START para ligar a ETA
    auto cmd_start = CommandFactory::criarComando("START", &ctrl, &eta, &sn, &sph, &alh);
    assert(cmd_start != nullptr);
    cmd_start->executar();
    assert(eta.esta_operando() == true); // Agora deve estar ligada!

    // 3. Testando o comando STOP para desligar a ETA novamente
    auto cmd_stop = CommandFactory::criarComando("STOP", &ctrl, &eta, &sn, &sph, &alh);
    assert(cmd_stop != nullptr);
    cmd_stop->executar();
    assert(eta.esta_operando() == false); // Voltou a desligar!
}

void deve_ignorar_comando_mal_formatado()
{
    Controlador ctrl("CTRL-101", 700.0, 80, 0.8, 0.03);
    ETA eta("Area 1");
    Reservatorio res("TK-101", "Area 1", 1000.0, 500.0);
    sensor_nivel sn("LT-101", "Area 1", 0.0, 150.0, 1000.0, &res);
    sensor_ph sph("PH-101", "Area 1", 7.0, 6.0, 8.0);
    alarme_ph alh("AH-PH", "Area 1");

    // Strings corrompidas ou desconhecidas não devem quebrar o programa (devem retornar nullptr)
    auto cmd_invalido1 = CommandFactory::criarComando("SETPOINT texto_errado", &ctrl, &eta, &sn, &sph, &alh);
    assert(cmd_invalido1 == nullptr);

    auto cmd_invalido2 = CommandFactory::criarComando("COMANDO_INEXISTENTE", &ctrl, &eta, &sn, &sph, &alh);
    assert(cmd_invalido2 == nullptr);
}

int main()
{
    cout << "[TESTE C++] Iniciando testes de COMANDOS..." << endl;
    deve_processar_comando_setpoint_valido();
    deve_processar_comando_stop_e_start();
    deve_ignorar_comando_mal_formatado();
    cout << "[TESTE C++] Todos os testes de COMANDOS passaram! ✅" << endl;
    return 0;
}