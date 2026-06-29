#ifndef ID_DUPLA
#define ID_DUPLA 80
#endif

#include <iostream>
#include <cassert>
#include <cmath>
#include "../CONTROLADOR.hpp"
#include "../BOMBA.hpp"
#include "../INVERSOR.hpp"
#include "../RESERVATORIO.hpp"
#include "../VALVULA.hpp"
#include "../VALVULA_CONSUMO.hpp"
#include "../SENSOR.hpp"

using namespace std;

void deve_aplicar_anti_windup_no_inversor()
{
    // ID_DUPLA = 80 -> Setpoint = 700.0, Tolerância = 80.0
    Controlador ctrl("CTRL-TESTE", 700.0, 80, 0.8, 0.03);

    assert(abs(ctrl.get_setpoint() - 700.0) < 0.001);
    assert(abs(ctrl.get_tolerancia() - 80.0) < 0.001);

    // Força cenário estável com nível bem baixo fixado
    Reservatorio res("TK-TESTE", "Area 1", 1000.0, 150.0);
    Inversor inv("INV-TESTE", "Area 1");
    Bomba bomba("P-TESTE", "Area 1", 20.0, &inv);
    Valvula valvula("XV-TESTE", "Area 1", 15.0);
    ValvulaConsumo consumo("XV-CONS-TESTE", "Area 1", 1.0, 25.0);
    sensor_nivel sn("LT-TESTE", "Area 1", 0.0, 150.0, 1000.0, &res);

    // DICA DE CORREÇÃO: Forçamos ciclos calculando o PI, mas resetamos o volume do
    // reservatório para manter o erro grande e testar se a integral trava (Clamping)
    for (int i = 0; i < 50; ++i)
    {
        ctrl.controlar_nivel(&sn, &res, &bomba, &valvula, &inv, &consumo, 5.0);
        res.atualizar_volume(150.0); // Impede o tanque de encher durante o teste unitário
    }

    // Agora o erro persistiu de forma forçada e a frequência DEVE estar saturada no teto
    assert(inv.get_frequencia() == 100.0);
}

void deve_ativar_racionamento_sob_alta_demanda()
{
    Controlador ctrl("CTRL-TESTE", 700.0, 80, 0.8, 0.03);
    Reservatorio res("TK-TESTE", "Area 1", 1000.0, 700.0);
    Inversor inv("INV-TESTE", "Area 1");
    Bomba bomba("P-TESTE", "Area 1", 20.0, &inv);
    Valvula valvula("XV-TESTE", "Area 1", 15.0);
    ValvulaConsumo consumo("XV-CONS-TESTE", "Area 1", 1.0, 25.0);
    sensor_nivel sn("LT-TESTE", "Area 1", 0.0, 150.0, 1000.0, &res);

    // Se a demanda solicitada for severa (>= 20.0 m³), a válvula deve ser estrangulada para no máximo 19.5 m³
    double demanda_sobrecarga = 22.0;
    ctrl.controlar_nivel(&sn, &res, &bomba, &valvula, &inv, &consumo, demanda_sobrecarga);

    assert(consumo.get_vazao() <= 19.5);
}

int main()
{
    cout << "[TESTE C++] Iniciando testes do Controlador..." << endl;
    deve_aplicar_anti_windup_no_inversor();
    deve_ativar_racionamento_sob_alta_demanda();
    cout << "[TESTE C++] Todos os testes do Controlador passaram com sucesso! ✅" << endl;
    return 0;
}