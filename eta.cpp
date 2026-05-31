#include <iostream>
#include <vector>
#include <memory>
#include <string>

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

class Controlador
{
private:
    string tag;

public:
    Controlador(string tag_nova) : tag(tag_nova) {}

    void monitorar_bomba(Bomba *b)
    {
        b->ligar();
        b->desligar();
    }

    void acionar_alarme()
    {
    }

    void acessar_historico()
    {
    }

    void consultar_sensores()
    {
    }

    void calibrar_sensores()
    {
    }
};

class Reservatorio
{
protected:
    string tag;
    string area;

public:
    Reservatorio(string tag_nova, string area_nova)
        : tag(tag_nova), area(area_nova) {}
};

class Tanque : public Reservatorio
{
public:
    using Reservatorio::Reservatorio;
};

class Bomba
{
private:
    string tag;
    string area;
    bool operando;

public:
    Bomba(string tag_nova, string area_nova)
        : tag(tag_nova), area(area_nova), operando(false) {}

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
        cout << "Alarme de nível alto " << tag << " ativado." << endl;
    }

    void silenciar() override
    {
        ativo = false;
        cout << "Alarme de nível alto " << tag << " desativado." << endl;
    }
};

class alarme_vazao : public Alarme
{
public:
    using Alarme::Alarme;

    void disparar() override
    {
        ativo = true;
        cout << "Alarme de vazão " << tag << " ativado." << endl;
    }

    void silenciar() override
    {
        ativo = false;
        cout << "Alarme de vazão " << tag << " desativado." << endl;
    }
};

class Historico
{
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

    virtual void ler_valor()
    {
        cout << "Lendo valor do sensor " << tag << " na área " << area << "." << endl;
    }

    virtual ~Sensor() = default;
};

class sensor_ph : public Sensor
{
public:
    using Sensor::Sensor;

    void ler_valor() override
    {
        cout << "Lendo valor do sensor de pH " << tag << " na área " << area << "." << endl;
    }
};

class sensor_turbidez : public Sensor
{
public:
    using Sensor::Sensor;

    void ler_valor() override
    {
        cout << "Lendo valor do sensor de turbidez " << tag << " na área " << area << "." << endl;
    }
};

class sensor_nivel : public Sensor
{
public:
    using Sensor::Sensor;

    void ler_valor() override
    {
        cout << "Lendo valor do sensor de nível " << tag << " na área " << area << "." << endl;
    }
};

class sensor_vazao : public Sensor
{
public:
    using Sensor::Sensor;

    void ler_valor() override
    {
        cout << "Lendo valor do sensor de vazão " << tag << " na área " << area << "." << endl;
    }
};

int main()
{
    vector<unique_ptr<Sensor>> sensores;

    sensor_nivel nivel_sensor("LT-101", "Área 2", 5.0, 0.0, 10.0);
    sensor_turbidez turbidez_sensor("TB-001", "Área 1", 0.5, 0.0, 1.0);
    sensor_vazao vazao_sensor("FT-301", "Área 3", 100.0, 0.0, 200.0);
    sensor_ph ph_sensor("PH-001", "Área 1", 7.0, 6.0, 8.0);

    sensores.push_back(make_unique<sensor_nivel>("LT-101"));
    sensores.push_back(make_unique<sensor_turbidez>("TB-001"));
    sensores.push_back(make_unique<sensor_vazao>("FT-301"));
    sensores.push_back(make_unique<sensor_ph>("PH-001"));

    for (const auto &sensor : sensores)
    {
        sensor->ler_valor();
    }
}