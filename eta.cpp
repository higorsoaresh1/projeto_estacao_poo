#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath> 

/*Desenvolvido por: Guilherme Parreira e Higor Soares.*/

using namespace std;

class ETA
{
private:
    string localizacao;
    bool operando;

public:
    ETA(string localizacao_nova) : localizacao(localizacao_nova), operando(false) {}

    void iniciar_tratamento()
    {
        operando = true;
        cout << "ETA iniciada." << endl;
    }

    void parar_tratamento()
    {
        operando = false;
        cout << "ETA parada." << endl;
    }
};

class Reservatorio
{
private:
    string tag;
    string area;
    double volume_maximo;
    double volume_atual;
    double consumo;

public:
    Reservatorio(string tag_nova, string area_nova, double volume_maximo_novo, double volume_atual_novo, double consumo_novo)
        : tag(tag_nova), area(area_nova), volume_maximo(volume_maximo_novo), volume_atual(volume_atual_novo), consumo(consumo_novo) {}

    double get_volume_atual()
    {
        return volume_atual;
    }

    void encher_reservatorio(double volume)
    {
        if (volume_atual + volume <= volume_maximo)
        {
            volume_atual += volume;
            cout << "Reservatorio " << tag << " enchido. Volume atual: " << volume_atual << " m³." << endl;
        }
        else
        {
            cout << "Não é possível encher o reservatorio " << tag << ". Volume excede o maximo." << endl;
        }
    }

    void esvaziar_reservatorio(double volume)
    {
        if (volume_atual - volume >= 0)
        {
            volume_atual -= volume;
            cout << "Reservatorio " << tag << " esvaziado. Volume atual: " << volume_atual << " m³." << endl;
        }
        else
        {
            cout << "Não é possível esvaziar o reservatorio " << tag << ". Volume insuficiente." << endl;
        }
    }

    void atualizar_volume(double novo_volume)
    {
        volume_atual = novo_volume;
    }

    void set_consumo(double novo_consumo)
    {
        consumo = novo_consumo;
    }

    double get_consumo()
    {
        return consumo;
    }
};

class Bomba
{
private:
    string tag;
    string area;
    double vazao_nominal;
    bool operando;

public:
    Bomba(string tag_nova, string area_nova, double vazao_nominal_nova)
        : tag(tag_nova), area(area_nova), vazao_nominal(vazao_nominal_nova), operando(false) {}

    void ligar()
    {
        operando = true;
        cout << "Bomba " << tag << " operando." << endl;
    }

    void desligar()
    {
        operando = false;
        cout << "Bomba " << tag << " desligada." << endl;
    }

    double get_vazao_nominal()
    {
        if (operando)
            return vazao_nominal;
        else
            return 0.0;
    }
};

class Valvula
{
private:
    string tag;
    string area;
    double vazao_alivio;
    bool aberta;

public:
    Valvula(string tag_nova, string area_nova, double vazao_alivio_nova)
        : tag(tag_nova), area(area_nova), vazao_alivio(vazao_alivio_nova), aberta(false) {}

    void abrir()
    {
        aberta = true;
        cout << "Valvula de alivio " << tag << " aberta." << endl;
    }

    void fechar()
    {
        aberta = false;
        cout << "VValvula de alivio " << tag << " fechada." << endl;
    }

    bool esta_aberta()
    {
        return aberta;
    }

    double get_vazao_alivio()
    {
        if (aberta)
            return vazao_alivio;
        else
            return 0.0;
    }
};

// Classe abstrata p/ sensores.
class Sensor
{
protected:
    string tag;
    string area;
    double valor_lido;
    double valor_minimo;
    double valor_maximo;

public:
    Sensor(string tag_nova, string area_nova, double valor_lido_novo, double valor_minimo_novo, double valor_maximo_novo)
        : tag(tag_nova), area(area_nova), valor_lido(valor_lido_novo), valor_minimo(valor_minimo_novo), valor_maximo(valor_maximo_novo) {}

    virtual double ler_valor()
    {
        cout << "Lendo valor do sensor " << tag << " na area " << area << "." << endl;

        return valor_lido;
    }

    double get_valor_minimo()
    {
        return valor_minimo;
    }

    double get_valor_maximo()
    {
        return valor_maximo;
    }

    virtual ~Sensor() = default;
};

class sensor_ph : public Sensor
{
public:
    using Sensor::Sensor;

    double ler_valor() override
    {
        cout << "Lendo valor do sensor de pH " << tag << " na area " << area << "." << endl;

        return valor_lido;
    }
};

class sensor_turbidez : public Sensor
{
public:
    using Sensor::Sensor;

    double ler_valor() override
    {
        cout << "Lendo valor do sensor de turbidez " << tag << " na area " << area << "." << endl;

        return valor_lido;
    }
};

class sensor_nivel : public Sensor
{
private:
    Reservatorio *reservatorio;

public:
    sensor_nivel(string tag_nova, string area_nova, double valor_lido_novo, double valor_minimo_novo, double valor_maximo_novo, Reservatorio *reservatorio_novo)
        : Sensor(tag_nova, area_nova, valor_lido_novo, valor_minimo_novo, valor_maximo_novo), reservatorio(reservatorio_novo) {}

    double ler_valor() override
    {
        valor_lido = reservatorio->get_volume_atual();
        cout << "Lendo valor do sensor de nivel " << tag << " na area " << area << "." << endl;

        return valor_lido;
    }
};

class sensor_vazao : public Sensor
{
private:
    Bomba *bomba;

public:
    sensor_vazao(string tag_nova, string area_nova, double valor_minimo_novo, double valor_maximo_novo, Bomba *bomba_nova)
        : Sensor(tag_nova, area_nova, 0.0, valor_minimo_novo, valor_maximo_novo), bomba(bomba_nova) {}

    double ler_valor() override
    {
        valor_lido = bomba->get_vazao_nominal();

        cout << "Lendo sensor de vazao "
             << tag
             << ". Vazao = "
             << valor_lido
             << " m³/ciclo"
             << endl;

        return valor_lido;
    }
};

class Alarme
{
protected:
    string tag;
    string area;
    bool ativo;

public:
    Alarme(string tag_nova, string area_nova, bool ativo_novo = false)
        : tag(tag_nova), area(area_nova), ativo(ativo_novo) {}

    virtual void disparar()
    {
        ativo = true;
        cout << "Alarme " << tag << " ativado." << endl;
    }

    virtual void silenciar()
    {
        ativo = false;
        cout << "Alarme " << tag << " desativado." << endl;
    }

    virtual ~Alarme() = default;
};

class alarme_ph : public Alarme
{
public:
    using Alarme::Alarme;

    void disparar() override
    {
        ativo = true;
        cout << "Alarme de pH " << tag << " ativado." << endl;
    }

    void silenciar() override
    {
        ativo = false;
        cout << "Alarme de pH " << tag << " desativado." << endl;
    }
};

class alarme_nivel_alto : public Alarme
{
public:
    using Alarme::Alarme;

    void disparar() override
    {
        ativo = true;
        cout << "Alarme de nivel alto " << tag << " ativado." << endl;
    }

    void silenciar() override
    {
        ativo = false;
        cout << "Alarme de nivel alto " << tag << " desativado." << endl;
    }
};

class alarme_vazao : public Alarme
{
public:
    using Alarme::Alarme;

    void disparar() override
    {
        ativo = true;
        cout << "Alarme de vazao " << tag << " ativado." << endl;
    }

    void silenciar() override
    {
        ativo = false;
        cout << "Alarme de vazao " << tag << " desativado." << endl;
    }
};

class alarme_turbidez : public Alarme
{
public:
    using Alarme::Alarme;

    void disparar() override
    {
        ativo = true;
        cout << "Alarme de turbidez " << tag << " ativado." << endl;
    }

    void silenciar() override
    {
        ativo = false;
        cout << "Alarme de turbidez " << tag << " desativado." << endl;
    }
};

class Historico
{
};

class Controlador
{
private:
    string tag;

public:
    Controlador(string tag_nova) : tag(tag_nova) {}

    void controlar_nivel(sensor_nivel *sensor, Reservatorio *reservatorio, Bomba *bomba, Valvula *valvula)
    {
        double setpoint = 700;
        double tolerancia = 80;
        double nivel = sensor->ler_valor();

        // Controle principal
        if (nivel < setpoint - tolerancia)
        {
            bomba->ligar();
        }
        else if (nivel > setpoint + tolerancia)
        {
            bomba->desligar();
        }

        if (nivel > 950)
        {
            valvula->abrir();
        }
        else if (nivel < 900)
        {
            valvula->fechar();
        }

        /*Essas funções são chamadas para atualizar o volume do reservatório, e não precisa necessariamente,
        estarem reservados aos if, pois o que fiz se o volume esta sendo modificado é a variável bool ativo e bool operando
        da bomba e da válvula presentes nos get.*/
        reservatorio->encher_reservatorio(bomba->get_vazao_nominal());
        reservatorio->esvaziar_reservatorio(valvula->get_vazao_alivio());
    }

    void monitorar(sensor_ph *sensor_ph, sensor_nivel *sensor_nivel, sensor_turbidez *sensor_turbidez, sensor_vazao *sensor_vazao,
                   alarme_ph *alarme_ph, alarme_nivel_alto *alarme_nivel_alto, alarme_vazao *alarme_vazao, alarme_turbidez *alarme_turbidez)
    {
        double ph = sensor_ph->ler_valor();
        double nivel = sensor_nivel->ler_valor();
        double turbidez = sensor_turbidez->ler_valor();
        double vazao = sensor_vazao->ler_valor();

        // Monitoramento sensor pH.
        if (ph > sensor_ph->get_valor_maximo())
        {
            cout << "pH alto detectado! \n"
                 << "pH Lido: " << ph << endl;
            alarme_ph->disparar(); // Melhorar sistema de alarme.
        }
        else if (ph < sensor_ph->get_valor_minimo())
        {
            cout << "pH baixo detectado! \n"
                 << "pH Lido: " << ph << endl;
            alarme_ph->disparar(); // Melhorar sistema de alarme.
        }
        else
        {
            alarme_ph->silenciar();
        }

        // Monitoramento sensor de nível.
        if (nivel > sensor_nivel->get_valor_maximo())
        {
            cout << "Nivel alto detectado! \n"
                 << "Nivel Lido: " << nivel << endl;
            alarme_nivel_alto->disparar(); // Melhorar sistema de alarme.
        }
        else if (nivel < sensor_nivel->get_valor_minimo())
        {
            cout << "Nivel baixo detectado! \n"
                 << "Nivel Lido: " << nivel << endl;
            alarme_nivel_alto->disparar(); // Melhorar sistema de alarme.
        }
        else
        {
            alarme_nivel_alto->silenciar();
        }

        // Monitoramento sensor de vazão.
        if (vazao > sensor_vazao->get_valor_maximo())
        {
            cout << "Vazao alta detectada! \n"
                 << "Vazao Lida: " << vazao << endl;
            alarme_vazao->disparar(); // Melhorar sistema de alarme.
        }
        else if (vazao < sensor_vazao->get_valor_minimo())
        {
            cout << "Vazao baixa detectada! \n"
                 << "Vazao Lida: " << vazao << endl;
            alarme_vazao->disparar(); // Melhorar sistema de alarme.
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
            alarme_turbidez->disparar(); // Melhorar sistema de alarme.
        }
        else if (turbidez < sensor_turbidez->get_valor_minimo())
        {
            cout << "Turbidez baixa detectada! \n"
                 << "Turbidez Lida: " << turbidez << endl;
            alarme_turbidez->disparar(); // Melhorar sistema de alarme.
        }
        else
        {
            alarme_turbidez->silenciar();
        }
    }
};

int main()
{
    srand(time(nullptr));

    // Gera o volume inicial do reservatório de forma aleatória entre 0 e 1000 m³ para simular diferentes condições iniciais.
    double volume_inicial = rand() % 1000;
    Reservatorio reservatorio("TK-101", "Area 1", 1000.0, volume_inicial, 5.0);

    Bomba bomba("P-101", "Area 1", 20.0);

    Valvula valvula("XV-101", "Area 1", 15.0);

    sensor_nivel sensorNivel("LT-101", "Area 1", 0.0, 0.0, 1000.0, &reservatorio);
    sensor_vazao sensorVazao("FT-101", "Area 1", 0.0, 25.0, &bomba);
    sensor_ph sensorPH("PH-101", "Area 1", 7.0, 6.0, 8.0);
    sensor_turbidez sensorTurbidez("TB-101", "Area 1", 0.5, 0.0, 1.0);

    alarme_ph alarmePH("AH-PH", "Area 1");
    alarme_nivel_alto alarmeNivel("AH-NV", "Area 1");
    alarme_vazao alarmeVazao("AH-VZ", "Area 1");
    alarme_turbidez alarmeTurbidez("AH-TB", "Area 1");

    Controlador controlador("CTRL-101");

    double consumo_atual = 5 + rand() % 11; // entre 5 e 15 m³/ciclo

    int ciclo = 0;
    int ciclos_estavel = 0;

    while (ciclo < 100)
    {
        ciclo++;

        cout << "\n===== CICLO " << ciclo << " =====\n";

        // Controle em malha fechada
        controlador.controlar_nivel(&sensorNivel, &reservatorio, &bomba, &valvula);

        // Consumo do processo
        reservatorio.esvaziar_reservatorio(consumo_atual);

        // Verifica se está próximo do setpoint
        double nivel = sensorNivel.ler_valor();

        if (abs(nivel - 700) <= 20)
        {
            ciclos_estavel++;
        }
        else
        {
            ciclos_estavel = 0;
        }

        // Após 5 ciclos estáveis, gera nova perturbação
        if (ciclos_estavel >= 5)
        {
            consumo_atual = 5 + rand() % 21; // entre 5 e 25

            cout << "\n*** PERTURBACAO GERADA ***\n";
            cout << "Novo consumo: " << consumo_atual << " m³/ciclo\n";

            ciclos_estavel = 0;
        }

        // Monitoramento
        controlador.monitorar(&sensorPH, &sensorNivel, &sensorTurbidez, &sensorVazao, &alarmePH, &alarmeNivel, &alarmeVazao, &alarmeTurbidez);

        cout << "Volume atual: " << reservatorio.get_volume_atual() << " m³\n";

        cout << "Consumo atual: " << consumo_atual << " m³/ciclo\n";
    }

    return 0;
}