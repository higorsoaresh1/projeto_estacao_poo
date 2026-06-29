#ifndef ID_DUPLA
#define ID_DUPLA 80
#endif

#include <iostream>
#include <cassert>
#include <cmath>
#include "../SENSOR.hpp"
#include "../RESERVATORIO.hpp"

using namespace std;

void deve_simular_falha_de_comunicacao_no_ph()
{
    sensor_ph sph("PH-101", "Area 1", 7.2, 6.0, 8.0);

    // Leitura em estado normal
    assert(abs(sph.ler_valor() - 7.2) < 0.001);
    assert(!sph.possui_falha());

    // Injeção da falha via simulação da mesa de testes
    sph.ativar_falha();
    assert(sph.possui_falha());

    // Deve retornar a assinatura de erro correspondente: ID_DUPLA / -100.0 = -0.8
    double valor_erro = (double)ID_DUPLA / (-100.0);
    assert(abs(sph.ler_valor() - valor_erro) < 0.001);

    // Reparo do dispositivo
    sph.reparar_falha();
    assert(!sph.possui_falha());
    assert(abs(sph.ler_valor() - 7.2) < 0.001);
}

void deve_atualizar_leitura_de_nivel_dinamicamente()
{
    Reservatorio res("TK-101", "Area 1", 1000.0, 400.0);
    sensor_nivel sn("LT-101", "Area 1", 0.0, 150.0, 1000.0, &res);

    assert(abs(sn.ler_valor() - 400.0) < 0.001);

    // Alteração física no tanque deve refletir no sensor imediatamente (Polimorfismo)
    res.atualizar_volume(850.5);
    assert(abs(sn.ler_valor() - 850.5) < 0.001);
}

int main()
{
    cout << "[TESTE C++] Iniciando testes dos Sensores..." << endl;
    deve_simular_falha_de_comunicacao_no_ph();
    deve_atualizar_leitura_de_nivel_dinamicamente();
    cout << "[TESTE C++] Todos os testes dos Sensores passaram com sucesso! ✅" << endl;
    return 0;
}