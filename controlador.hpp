#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>

#include "ALARME.hpp"
#include "BOMBA.hpp"
#include "RESERVATORIO.hpp"
#include "SENSOR.hpp"
#include "VALVULA.hpp"
#include "VALVULA_CONSUMO.hpp"

using namespace std;

class Controlador
{ /*Classe para representar o controlador no sistema de ETA*/
private:
    string tag;
    double setpoint;
    double tolerancia;
    double kp;
    double ki;
    double erro_integral;

public:
    Controlador(string tag_nova, double setpoint_novo, double tolerancia_nova, double kp_novo, double ki_novo)
        : tag(tag_nova), setpoint(setpoint_novo), tolerancia(tolerancia_nova), kp(kp_novo), ki(ki_novo), erro_integral(0) {}

    void set_setpoint(double novo_setpoint, double nivel_atual)
    {
        /*TRATAMENTO DE ERRO: Validação de consistência física do Setpoint*/
        if (novo_setpoint < 0.0)
        {
            throw invalid_argument("[ERRO CONTROLADOR] O Setpoint não pode ser negativo.");
        }

        double erro_novo = novo_setpoint - nivel_atual;

        /*SÓ reseta a integral se o erro for negativo (redução brusca de setpoint onde há risco de transbordo)
        Se o erro for positivo (precisa encher), NUNCA zeramos a integral para não derrubar a bomba.*/
        if (erro_novo < 0.0 && abs(erro_novo) > tolerancia)
        {
            erro_integral = 0;
        }
        /*Opcional: Se o nível for muito crítico (perto do piso de 100), mantém a integral trabalhando*/
        else if (nivel_atual > 200.0 && abs(erro_novo) > (tolerancia * 2.0))
        {
            /*Se o tanque estiver confortável e mudou muito o setpoint,
            em vez de zerar, reduzimos a integral pela metade para suavizar, mas sem desligar a bomba.*/
            erro_integral = erro_integral * 0.5;
        }

        setpoint = novo_setpoint;
    }

    double get_setpoint() const { return setpoint; }
    double get_tolerancia() const { return tolerancia; }
    void set_tolerancia(double nova_tolerancia) { tolerancia = nova_tolerancia; }

    void controlar_nivel(sensor_nivel *sensor, Reservatorio *reservatorio, Bomba *bomba, Valvula *valvula, Inversor *inversor, ValvulaConsumo *consumo, double consumo_externo_solicitado)
    {
        /*TRATAMENTO DE ERRO: Proteção defensiva contra ponteiros nulos*/
        if (!sensor || !reservatorio || !bomba || !valvula || !inversor || !consumo)
        {
            throw runtime_error("[ERRO CRÍTICO] Um ou mais componentes necessários passados ao Controlador são nulos (nullptr).");
        }

        double nivel = sensor->ler_valor();

        /*TRATAMENTO DE ERRO: Verificação de falha de hardware ou quebra do sensor de nível*/
        if (nivel < 0.0)
        {
            cerr << "[FALHA HARDWARE] Sensor de nível corrompido ou desconectado (Leitura negativa: " << nivel << " m³). Parando atuadores por segurança!" << endl;
            inversor->set_frequencia(0.0);
            bomba->desligar();
            return; /*Interrompe o ciclo imediatamente sem aplicar a malha matemática PI*/
        }

        double erro = setpoint - nivel;

        /* Cálculo base da malha Proporcional-Integral (PI)*/
        double proporcional = kp * erro;
        double integral = ki * erro_integral;
        double saida = proporcional + integral;

        /*Anti-Windup por Clamping (Bloqueio Inteligente da Integral)*/
        bool saturado_alto = (saida >= 100.0 && erro > 0.0);
        bool saturado_baixo = (saida <= 0.0 && erro < 0.0);

        if (!saturado_alto && !saturado_baixo)
        {
            erro_integral += erro;
        }

        /*Recalcula a saída com a integral updated e aplica as travas físicas do inversor (0% a 100%)*/
        saida = proporcional + (ki * erro_integral);
        if (saida > 100.0)
        {
            saida = 100.0;
        }
        if (saida < 0.0)
        {
            saida = 0.0;
        }

        inversor->set_frequencia(saida);

        if (saida > 0)
            bomba->ligar();
        else
            bomba->desligar();

        double vazao_saida_real = 0.0;

        if (nivel <= 100.0)
        {
            consumo->set_abertura(0.0);
            vazao_saida_real = 0.0;
            cout << " [BLOQUEIO CRÍTICO] Nível atingiu o piso mínimo (" << nivel
                 << " m³). Válvula de saída FECHADA TOTALMENTE para recuperação!" << endl;
        }
        else
        {
            if (consumo_externo_solicitado >= 20.0)
            {
                consumo->set_abertura(19.5 / 25.0);
                vazao_saida_real = consumo->get_vazao();

                cout << " [RACIONAMENTO ATIVO] Demanda (" << consumo_externo_solicitado
                     << " m³). Valvula estrangulada para 78% (" << vazao_saida_real << " m³)." << endl;
            }
            else
            {
                consumo->set_abertura(consumo_externo_solicitado / 25.0);
                vazao_saida_real = consumo->get_vazao();
            }
        }

        if (nivel > 950)
        {
            valvula->abrir();
        }
        else if (nivel < 900)
        {
            valvula->fechar();
        }

        reservatorio->encher_reservatorio(bomba->get_vazao());
        reservatorio->esvaziar_reservatorio(valvula->get_vazao_alivio());
        reservatorio->esvaziar_reservatorio(vazao_saida_real);
    }

    void monitorar(sensor_ph *sensor_ph, sensor_nivel *sensor_nivel, sensor_turbidez *sensor_turbidez, sensor_vazao *sensor_vazao, alarme_ph *alarme_ph,
                   alarme_nivel *alarme_nivel, alarme_vazao *alarme_vazao, alarme_turbidez *alarme_turbidez, alarme_racionamento *alarme_rac, double consumo_solicitado)
    {
        /*TRATAMENTO DE ERRO: Proteção defensiva para evitar falha se o subsistema de alarmes falhar*/
        if (!sensor_ph || !sensor_nivel || !sensor_turbidez || !sensor_vazao ||
            !alarme_ph || !alarme_nivel || !alarme_vazao || !alarme_turbidez || !alarme_rac)
        {
            cerr << "[ERRO CRÍTICO MONITORAMENTO] Um ou mais Sensores/Alarmes passados estão nulos! Ignorando ciclo." << endl;
            return;
        }

        double ph = sensor_ph->ler_valor();
        double nivel = sensor_nivel->ler_valor();
        double turbidez = sensor_turbidez->ler_valor();
        double vazao = sensor_vazao->ler_valor();

        if (consumo_solicitado >= 20.0)
        {
            alarme_rac->disparar();
        }
        else
        {
            alarme_rac->silenciar();
        }

        /*Monitoramento sensor pH.*/
        if (ph > sensor_ph->get_valor_maximo())
        {
            cout << "pH alto detectado! \n"
                 << "pH Lido: " << ph << endl;
            alarme_ph->disparar();
        }
        else if (ph < sensor_ph->get_valor_minimo())
        {
            cout << "pH baixo detectado! \n"
                 << "pH Lido: " << ph << endl;
            alarme_ph->disparar();
        }
        else
        {
            alarme_ph->silenciar();
        }

        /*MOnitoramento sensor de nível com alerta de nivel baixo e nivel alto*/
        if (nivel <= sensor_nivel->get_valor_minimo())
        {
            cout << "[ALERTA] Nível Baixo Operacional! Volume em " << nivel << " m³." << endl;
            alarme_nivel->disparar();
        }
        else if (nivel > sensor_nivel->get_valor_maximo())
        {
            cout << "[ALERTA] Nível Alto de Transbordo! Volume em " << nivel << " m³." << endl;
            alarme_nivel->disparar();
        }
        else
        {
            if (alarme_nivel->esta_ativo())
            {
                alarme_nivel->silenciar();
            }
        }

        /*Monitoramento sensor de vazão.*/
        if (vazao > sensor_vazao->get_valor_maximo())
        {
            cout << "Vazao alta detectada! \n"
                 << "Vazao Lida: " << vazao << endl;
            alarme_vazao->disparar();
        }
        else if (vazao < sensor_vazao->get_valor_minimo())
        {
            cout << "Vazao baixa detectada! \n"
                 << "Vazao Lida: " << vazao << endl;
            alarme_vazao->disparar();
        }
        else
        {
            alarme_vazao->silenciar();
        }

        /*Monitoramento sensor de turbidez*/
        if (turbidez > sensor_turbidez->get_valor_maximo())
        {
            cout << "Turbidez alta detectada! \n"
                 << "Turbidez Lida: " << turbidez << endl;
            alarme_turbidez->disparar();
        }
        else if (turbidez < sensor_turbidez->get_valor_minimo())
        {
            cout << "Turbidez baixa detectada! \n"
                 << "Turbidez Lida: " << turbidez << endl;
            alarme_turbidez->disparar();
        }
        else
        {
            alarme_turbidez->silenciar();
        }
    }
};

#endif