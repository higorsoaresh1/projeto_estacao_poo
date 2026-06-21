#ifndef ALARME_HPP
#define ALARME_HPP

#include <iostream>
#include <string>

using namespace std;

class Alarme
{ /*Classe que representa um alarme no sistema de ETA*/
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

    virtual bool esta_ativo()
    {
        return ativo;
    }

    virtual ~Alarme() = default;
};

class alarme_ph : public Alarme
{ /*Classe que representa um alarme de pH no sistema de ETA*/
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

    bool esta_ativo() override
    {
        return ativo;
    }
};

// ALARME DE NÍVEL COM ALERTA DE NÍVEL BAIXO OPERACIONAL E NÍVEL ALTO DE TRANSBORDO
class alarme_nivel : public Alarme
{
public:
    using Alarme::Alarme;

    void disparar() override
    {
        ativo = true;
    }

    void silenciar() override
    {
        ativo = false;
        cout << "Nível do reservatório normalizado." << endl;
    }

    bool esta_ativo() override
    {
        return ativo;
    }
};

class alarme_vazao : public Alarme
{ /*Classe que representa um alarme de vazão no sistema de ETA*/
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

    bool esta_ativo() override
    {
        return ativo;
    }
};

class alarme_turbidez : public Alarme
{ /*Classe que representa um alarme de turbidez no sistema de ETA*/
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

    bool esta_ativo() override
    {
        return ativo;
    }
};

class alarme_racionamento : public Alarme
{ /* Classe que representa o status do racionamento na ETA */
public:
    using Alarme::Alarme;

    void disparar() override
    {
        ativo = true;
        cout << "Racionamento de água ATIVADO devido à alto consumo!" << endl;
    }

    void silenciar() override
    {
        ativo = false;
        cout << "Racionamento de água DESATIVADO. Sistema normalizado." << endl;
    }

    bool esta_ativo() override
    {
        return ativo;
    }
};

#endif