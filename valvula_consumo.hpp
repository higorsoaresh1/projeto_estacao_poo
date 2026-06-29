#ifndef VALVULA_CONSUMO_HPP
#define VALVULA_CONSUMO_HPP

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class ValvulaConsumo
{
private:
    string tag;
    string area;
    double abertura;
    double vazao_maxima;

public:
    ValvulaConsumo(string tag_nova, string area_nova, double abertura_nova, double vazao_maxima_nova)
        : tag(tag_nova), area(area_nova), abertura(abertura_nova), vazao_maxima(vazao_maxima_nova)
    {
        /*TRATAMENTO DE ERRO: Validação dos parâmetros de inicialização da válvula*/
        if (tag_nova.empty() || area_nova.empty())
        {
            throw invalid_argument("[ERRO VÁLVULA] A 'tag' e a 'area' da válvula não podem ser vazias.");
        }
        if (vazao_maxima_nova <= 0.0)
        {
            throw invalid_argument("[ERRO VÁLVULA] A vazão máxima da válvula deve ser maior que zero.");
        }
        if (abertura_nova < 0.0 || abertura_nova > 1.0)
        {
            throw invalid_argument("[ERRO VÁLVULA] A abertura inicial deve estar contida no intervalo entre 0.0 e 1.0.");
        }
    }

    void set_abertura(double nova_abertura)
    {
        /*TRATAMENTO/AVISO: Alerta o operador caso o sinal do controlador tente extrapolar os limites físicos*/
        if (nova_abertura < 0.0)
        {
            cerr << "[AVISO VÁLVULA] " << tag << ": Comando de abertura menor que 0 (" << nova_abertura << "). Limitando em 0.0 (Fechada)." << endl;
            nova_abertura = 0.0;
        }
        else if (nova_abertura > 1.0)
        {
            cerr << "[AVISO VÁLVULA] " << tag << ": Comando de abertura maior que 1 (" << nova_abertura << "). Limitando em 1.0 (Totalmente Aberta)." << endl;
            nova_abertura = 1.0;
        }

        abertura = nova_abertura;
    }

    double get_abertura() const
    {
        return abertura;
    }

    double get_vazao() const
    {
        return vazao_maxima * abertura;
    }
};

#endif