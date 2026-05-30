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

    void ajustar_bomba()
    {
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

    void disparar()
    {
        ativo = true;
        cout << "Alarme " << tag << " ativado." << endl;
    }

    void silenciar()
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
};

class alarme_nivel_alto : public Alarme
{
public:
    using Alarme::Alarme;
};

class alarme_vazao : public Alarme
{
public:
    using Alarme::Alarme;
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

    virtual ~Sensor() = default;
};

class sensor_ph : public Sensor
{
public:
    using Sensor::Sensor;
};

class sensor_turbidez : public Sensor
{
public:
    using Sensor::Sensor;
};

class sensor_nivel : public Sensor
{
public:
    using Sensor::Sensor;
};

class sensor_vazao : public Sensor
{
public:
    using Sensor::Sensor;
};

int main()
{
    vector<unique_ptr<Sensor>> sensores;
    vector<unique_ptr<Alarme>> alarmes;
}