#include <iostream>
#include <cassert>
#include <cmath>
#include "../RESERVATORIO.hpp"

using namespace std;

int main()
{
    cout << "[TESTE ESTRESSE] Iniciando teste de carga no Reservatório..." << endl;

    // Reservatório com 1000m³ de capacidade e 500m³ inicial
    Reservatorio res("TK-101", "Area 1", 1000.0, 500.0);

    // Simula 10.000 alterações rápidas de volume (oscilação da planta)
    for (int i = 0; i < 10000; i++)
    {
        res.atualizar_volume(550.5);
        res.atualizar_volume(499.5);
    }

    // O volume final após as oscilações deve ser exatamente o último setado
    // Usamos aproximação para evitar problemas de precisão de ponto flutuante (double)
    assert(abs(res.get_volume_atual() - 499.5) < 0.0001);

    cout << "[OK] O componente resistiu a 10.000 ciclos de leitura/escrita sem degradação! ✅" << endl;
    return 0;
}