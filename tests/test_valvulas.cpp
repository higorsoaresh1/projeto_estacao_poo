#include <iostream>
#include <cassert>
#include "../VALVULA.hpp"
#include "../VALVULA_CONSUMO.hpp"

using namespace std;

int main()
{
    cout << "[TESTE] Validando Válvulas (Alívio e Consumo)..." << endl;

    // -------------------------------------------------------------------------
    // 1. Teste da Válvula de Alívio (ON/OFF)
    // -------------------------------------------------------------------------
    Valvula valvula_alivio("XV-101", "Area 1", 15.0); // Vazão máxima = 15.0

    // Deve iniciar fechada (vazão 0.0)
    assert(valvula_alivio.esta_aberta() == false);
    assert(valvula_alivio.get_vazao_alivio() == 0.0);

    // Abrindo a válvula
    valvula_alivio.abrir();
    assert(valvula_alivio.esta_aberta() == true);
    assert(valvula_alivio.get_vazao_alivio() == 15.0);

    // Fechando a válvula
    valvula_alivio.fechar();
    assert(valvula_alivio.esta_aberta() == false);
    assert(valvula_alivio.get_vazao_alivio() == 0.0);

    // -------------------------------------------------------------------------
    // 2. Teste da Válvula de Consumo (Proporcional)
    // -------------------------------------------------------------------------
    // Tag, Area, Abertura Inicial (1.0 = 100%), Vazão Máxima (25.0)
    ValvulaConsumo valvula_consumo("XV-CONS", "Area 1", 1.0, 25.0);

    // Inicialmente com 100% de abertura -> vazão deve ser a máxima (25.0)
    assert(valvula_consumo.get_abertura() == 1.0);
    assert(valvula_consumo.get_vazao() == 25.0);

    // Testando abertura em 50% (0.5) -> vazão deve ser metade (12.5)
    valvula_consumo.set_abertura(0.5);
    assert(valvula_consumo.get_abertura() == 0.5);
    assert(valvula_consumo.get_vazao() == 12.5);

    // Testando travas de segurança (Clamping) da abertura
    valvula_consumo.set_abertura(1.5);             // Acima do limite máximo
    assert(valvula_consumo.get_abertura() == 1.0); // Deve travar em 1.0

    valvula_consumo.set_abertura(-0.5);            // Abaixo do limite mínimo
    assert(valvula_consumo.get_abertura() == 0.0); // Deve travar em 0.0

    cout << "[OK] Teste das Válvulas passou com sucesso! ✅" << endl;
    return 0;
}