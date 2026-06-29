#include <iostream>
#include <cassert>
#include "../ETA.hpp"

using namespace std;

int main()
{
    cout << "[TESTE] Validando Gerenciamento da ETA..." << endl;

    // Instancia a ETA na localização informada
    ETA eta("Estacao Central");

    // 1. Deve iniciar desligada/parada por padrão (operando = false)
    assert(eta.esta_operando() == false);

    // 2. Testa o método para iniciar o tratamento
    eta.iniciar_tratamento();
    assert(eta.esta_operando() == true);

    // 3. Testa o método para parar o tratamento
    eta.parar_tratamento();
    assert(eta.esta_operando() == false);

    cout << "[OK] Teste da ETA passou com sucesso! ✅" << endl;
    return 0;
}