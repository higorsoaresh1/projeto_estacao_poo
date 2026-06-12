#ifndef ALARME_HPP
#define ALARME_HPP

#include <iostream>
#include <string>

using namespace std;

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

    bool esta_ativo()
    {
        return ativo;
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

#endif