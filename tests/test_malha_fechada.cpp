#ifndef ID_DUPLA
#define ID_DUPLA 80
#endif

#include <iostream>
#include <cassert>
#include "../CONTROLADOR.hpp"
#include "../RESERVATORIO.hpp"
#include "../INVERSOR.hpp"
#include "../BOMBA.hpp"
#include "../VALVULA.hpp"
#include "../VALVULA_CONSUMO.hpp"
#include "../SENSOR.hpp"

using namespace std;

int main()
{
    cout << "[TESTE AVANÇADO] Iniciando Simulação de Malha Fechada..." << endl;

    // Configuração da Planta Física
    // Setpoint = 700.0 m³, Tolerância = 80.0 m³
    Controlador ctrl("CTRL-101", 700.0, 80, 0.5, 0.02);
    Reservatorio res("TK-101", "Area 1", 1000.0, 400.0); // Começa abaixo do setpoint
    Inversor inv("INV-101", "Area 1");
    Bomba bomba("P-101", "Area 1", 50.0, &inv); // Bomba potente de 50 m³/h
    Valvula valvula_alivio("XV-101", "Area 1", 10.0);
    ValvulaConsumo valvula_consumo("XV-CONS", "Area 1", 0.2, 20.0); // Consumo baixo (20% de 20 = 4 m³/h)
    sensor_nivel sn("LT-101", "Area 1", 0.0, 150.0, 1000.0, &res);

    bomba.ligar();
    double dt = 1.0; // Simula passos de 1 hora por ciclo para acelerar o teste
    double volume_inicial = res.get_volume_atual();

    // Executa 5 ciclos de controle
    for (int i = 0; i < 5; i++)
    {
        // O controlador calcula o erro e ajusta a frequência do inversor
        ctrl.controlar_nivel(&sn, &res, &bomba, &valvula_alivio, &inv, &valvula_consumo, 4.0);

        // Simula o balanço hídrico no reservatório: Volume = Volume + (Entrada - Saída) * dt
        double vazao_entrada = bomba.get_vazao();
        double vazao_saida = valvula_consumo.get_vazao();
        double novo_volume = res.get_volume_atual() + (vazao_entrada - vazao_saida) * dt;
        res.atualizar_volume(novo_volume);
    }

    // Como o nível inicial era 400 e o setpoint é 700, a bomba DEVE ter injetado água
    cout << "Volume Inicial: " << volume_inicial << " m³ | Volume Final após controle: " << res.get_volume_atual() << " m³" << endl;
    assert(res.get_volume_atual() > volume_inicial);

    cout << "[OK] Teste de Malha Fechada passou com sucesso! 🎯" << endl;
    return 0;
}