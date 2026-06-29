#ifndef RESERVATORIO_HPP
#define RESERVATORIO_HPP

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class Reservatorio
{ /*Classe que representa um reservatório no sistema de ETA*/
private:
    string tag;
    string area;
    double volume_maximo;
    double volume_atual;

public:
    Reservatorio(string tag_nova, string area_nova, double volume_maximo_novo, double volume_atual_novo)
        : tag(tag_nova), area(area_nova), volume_maximo(volume_maximo_novo), volume_atual(volume_atual_novo)
    {
        /*TRATAMENTO DE ERRO: Garante consistência física na criação do tanque*/
        if (tag_nova.empty() || area_nova.empty())
        {
            throw invalid_argument("[ERRO RESERVATÓRIO] A 'tag' e a 'area' não podem ser vazias.");
        }
        if (volume_maximo_novo <= 0.0)
        {
            throw invalid_argument("[ERRO RESERVATÓRIO] O volume máximo deve ser maior que zero.");
        }
        if (volume_atual_novo < 0.0 || volume_atual_novo > volume_maximo_novo)
        {
            throw invalid_argument("[ERRO RESERVATÓRIO] O volume atual inicial é inconsistente com os limites físicos do tanque.");
        }
    }

    double get_volume_atual() const
    { /*Retorna o volume atual do reservatório*/
        return volume_atual;
    }

    void encher_reservatorio(double volume)
    { /*Enche o reservatório com tratamento de saturação física*/
        if (volume < 0.0)
            return; /*Proteção contra valores bizarros de vazão*/

        if (volume_atual + volume <= volume_maximo)
        {
            volume_atual += volume;
            cout << "Reservatorio " << tag << " enchido. Volume atual: " << volume_atual << " m³." << endl;
        }
        else
        {
            /*TRATAMENTO: O tanque atinge o limite máximo e o resto transborda*/
            volume_atual = volume_maximo;
            cerr << "[ALERTA TRANSBORDO] Reservatório " << tag << " atingiu a capacidade máxima (" << volume_maximo << " m³)! Água excedente foi perdida." << endl;
        }
    }

    void esvaziar_reservatorio(double volume)
    { /*Esvazia o reservatório com tratamento de saturação física (Seca)*/
        if (volume < 0.0)
            return; // Proteção contra valores bizarros de vazão

        if (volume_atual - volume >= 0.0)
        {
            volume_atual -= volume;
            cout << "Reservatorio " << tag << " esvaziado. Volume atual: " << volume_atual << " m³." << endl;
        }
        else
        {
            /*TRATAMENTO: O tanque seca completamente, não permitindo volume negativo*/
            volume_atual = 0.0;
            cerr << "[AVISO CRÍTICO] Reservatório " << tag << " esvaziou completamente (0.0 m³). Demanda não pôde ser totalmente atendida!" << endl;
        }
    }

    void atualizar_volume(double novo_volume)
    { /*Atualiza o volume atual do reservatório com validação de segurança*/
        if (novo_volume < 0.0 || novo_volume > volume_maximo)
        {
            cerr << "[ERRO] Tentativa de atualizar volume para valor fora dos limites físicos (" << novo_volume << " m³). Comando ignorado." << endl;
            return;
        }
        volume_atual = novo_volume;
    }
};

#endif