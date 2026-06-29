#include <iostream>
#include <cassert>
#include "../ALARME.hpp"

using namespace std;

int main()
{
    cout << "[TESTE] Validando Inicialização e Estados dos Alarmes..." << endl;

    alarme_nivel al_nivel("ALM-NVL", "Area 1");

    // Garante que o alarme instancia desativado por segurança
    assert(al_nivel.esta_ativo() == false);

    cout << "[OK] Teste de Alarmes passou! ✅" << endl;
    return 0;
}