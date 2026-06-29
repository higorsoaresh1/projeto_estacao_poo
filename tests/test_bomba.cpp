#include <iostream>
#include <cassert>
#include "../INVERSOR.hpp"
#include "../BOMBA.hpp"

using namespace std;

int main()
{
    cout << "[TESTE] Validando Bomba Hidráulica..." << endl;

    Inversor inv("INV-101", "Area 1");
    Bomba bomba("P-101", "Area 1", 20.0, &inv); // Capacidade de 20 m³/h

    // 1. Define a frequência no inversor
    inv.set_frequencia(50.0);

    // 2. Com a bomba desligada, a vazão DEVE ser 0.0
    assert(bomba.get_vazao() == 0.0);

    // 3. Liga a bomba para permitir o fluxo
    bomba.ligar();
    assert(bomba.esta_operando() == true);

    // 4. Agora a vazão deve ser calculada (50% de 20.0 = 10.0)
    assert(bomba.get_vazao() == 10.0);

    // 5. Desliga a bomba e garante que voltou a ser zero
    bomba.desligar();
    assert(bomba.get_vazao() == 0.0);

    cout << "[OK] Teste da Bomba passou! ✅" << endl;
    return 0;
}