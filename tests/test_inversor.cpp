#include <iostream>
#include <cassert>
#include "../INVERSOR.hpp"

using namespace std;

int main()
{
    cout << "[TESTE] Validando Inversor de Frequência..." << endl;

    Inversor inv("INV-101", "Area 1");

    // Verifica inicialização em 0 Hz
    assert(inv.get_frequencia() == 0.0);

    // Testa alteração de frequência utilizando o método correto da sua classe
    inv.set_frequencia(60.0);
    assert(inv.get_frequencia() == 60.0);

    cout << "[OK] Teste do Inversor passou! ✅" << endl;
    return 0;
}