#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <iostream>
#include <string>
#include <cmath> // Adicionado para garantir o funcionamento do abs() ou std::abs() se necessário

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
        double erro_novo = novo_setpoint - nivel_atual;

        // Se a alteração de setpoint for brusca (maior que a tolerância do processo),
        // limpamos o histórico acumulado para o inversor calcular a rampa do zero,
        // evitando que o tanque extrapole o limite desejado.
        if (abs(erro_novo) > tolerancia)
        {
            erro_integral = 0;
        }

        setpoint = novo_setpoint;
    }

    double get_setpoint() const
    {
        return setpoint;
    }

    double get_tolerancia() const
    {
        return tolerancia;
    }

    void set_tolerancia(double nova_tolerancia)
    {
        tolerancia = nova_tolerancia;
    }

    void controlar_nivel(sensor_nivel *sensor, Reservatorio *reservatorio, Bomba *bomba, Valvula *valvula, Inversor *inversor, ValvulaConsumo *consumo, double consumo_externo_solicitado)
    {
        double nivel = sensor->ler_valor();
        double erro = setpoint - nivel;

        // 1. Cálculo base da malha Proporcional-Integral (PI)
        double proporcional = kp * erro;
        double integral = ki * erro_integral;
        double saida = proporcional + integral;

        // 2. Anti-Windup por Clamping (Bloqueio Inteligente da Integral)
        // Impede que a integral acumule valores infinitos se o inversor já estiver saturado nos extremos.
        bool saturado_alto = (saida >= 100.0 && erro > 0.0);
        bool saturado_baixo = (saida <= 0.0 && erro < 0.0);

        if (!saturado_alto && !saturado_baixo)
        {
            erro_integral += erro;
        }

        // Recalcula a saída com a integral atualizada e aplica as travas físicas do inversor (0% a 100%)
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
            // Bloqueio Total Efetivo:
            consumo->set_abertura(0.0);

            // Forçamos a vazão de saída para 0 para interromper a simulação física de esvaziamento
            vazao_saida_real = 0.0;

            cout << " [BLOQUEIO CRÍTICO] Nível atingiu o piso mínimo (" << nivel
                 << " m³). Válvula de saída FECHADA TOTALMENTE para recuperação!" << endl;
        }
        else
        {
            // Limita e raciona se a demanda externa for maior ou igual a 20.0 m³
            if (consumo_externo_solicitado >= 20.0)
            {
                // Limita a vazão real de saída em 19.5 m³ (Margem segura abaixo da bomba de 20)
                // 19.5 m³ / 25.0 m³ capacidade máxima da válvula = 0.78 (78% de abertura)
                consumo->set_abertura(19.5 / 25.0);
                vazao_saida_real = consumo->get_vazao();

                cout << " [RACIONAMENTO ATIVO] Demanda (" << consumo_externo_solicitado
                     << " m³). Valvula estrangulada para 78% (" << vazao_saida_real << " m³)." << endl;
            }
            else
            {
                // Fluxo normal flutuante
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

    // O método agora recebe o resultado lógico 'tem_deficit' e não precisa mais do ponteiro do inversor
    void monitorar(sensor_ph *sensor_ph, sensor_nivel *sensor_nivel, sensor_turbidez *sensor_turbidez, sensor_vazao *sensor_vazao, alarme_ph *alarme_ph,
                   alarme_nivel *alarme_nivel, alarme_vazao *alarme_vazao, alarme_turbidez *alarme_turbidez, alarme_racionamento *alarme_rac, double consumo_solicitado)
    { /*Método responsável por monitorar os sensores do sistema*/
        double ph = sensor_ph->ler_valor();
        double nivel = sensor_nivel->ler_valor();
        double turbidez = sensor_turbidez->ler_valor();
        double vazao = sensor_vazao->ler_valor();

        // Alarme baseado puramente no gatilho de requisição de sobrecarga externa
        if (consumo_solicitado >= 20.0)
        {
            alarme_rac->disparar();
        }
        else
        {
            alarme_rac->silenciar();
        }

        // Monitoramento sensor pH.
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

        // MONITORAMENTO SENSOR DE NÍVEL COM ALERTA DE NÍVEL BAIXO OPERACIONAL E NÍVEL ALTO DE TRANSBORDO
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
            // Só silencia se o alarme estava ativo antes, para evitar spam de "Nível normalizado"
            if (alarme_nivel->esta_ativo())
            {
                alarme_nivel->silenciar();
            }
        }
        // Monitoramento sensor de vazão.
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

        // Monitoramento sensor de turbidez.
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