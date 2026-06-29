#include <iostream>
#include <cassert>
#include "../RESERVATORIO.hpp"

using namespace std;

int main()
{
    cout << "[TESTE] Validando Reservatório..." << endl;

    // Tag, Area, Capacidade Máxima, Volume Inicial
    Reservatorio res("TK-101", "Area 1", 1000.0, 500.0);

    assert(res.get_volume_atual() == 500.0);

    res.atualizar_volume(750.0);
    assert(res.get_volume_atual() == 750.0);

    cout << "[OK] Teste do Reservatório passou! ✅" << endl;
    return 0;
}