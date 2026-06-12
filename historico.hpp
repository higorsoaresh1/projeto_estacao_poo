#ifndef HISTORICO_HPP
#define HISTORICO_HPP

#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

class Historico
{
private:
    sqlite3 *db;

public:
    Historico(const string &nome_banco)
    {
        if (sqlite3_open(nome_banco.c_str(), &db) != SQLITE_OK)
        {
            cerr << "Erro ao abrir banco: "
                 << sqlite3_errmsg(db)
                 << endl;
        }
        else
        {
            cout << "Banco conectado.\n";

            criar_tabela();
        }
    }

    void criar_tabela()
    {
        string sql =
            "CREATE TABLE IF NOT EXISTS historico ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "ciclo INTEGER,"
            "nivel REAL,"
            "vazao REAL,"
            "ph REAL,"
            "turbidez REAL,"
            "consumo REAL,"
            "bomba INTEGER,"
            "valvula INTEGER,"
            "alarme_ph INTEGER,"
            "alarme_nivel INTEGER,"
            "alarme_vazao INTEGER,"
            "alarme_turbidez INTEGER"
            ");";

        char *erro = nullptr;

        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro) != SQLITE_OK)
        {
            cerr << "Erro ao criar tabela: "
                 << erro
                 << endl;

            sqlite3_free(erro);
        }
    }

    void registrar(int ciclo,
                   double nivel,
                   double vazao,
                   double ph,
                   double turbidez,
                   double consumo,
                   bool bomba,
                   bool valvula,
                   bool alarme_ph,
                   bool alarme_nivel,
                   bool alarme_vazao,
                   bool alarme_turbidez)
    {
        string sql =
            "INSERT INTO historico "
            "(ciclo,nivel,vazao,ph,turbidez,consumo,"
            "bomba,valvula,alarme_ph,alarme_nivel,"
            "alarme_vazao,alarme_turbidez) VALUES (" +
            to_string(ciclo) + "," + to_string(nivel) + "," + to_string(vazao) + "," + to_string(ph) + "," + to_string(turbidez) + "," + to_string(consumo) + "," + to_string(bomba) + "," + to_string(valvula) + "," + to_string(alarme_ph) + "," + to_string(alarme_nivel) + "," + to_string(alarme_vazao) + "," + to_string(alarme_turbidez) + ");";

        char *erro = nullptr;

        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro) != SQLITE_OK)
        {
            cerr << "Erro ao inserir: "
                 << erro
                 << endl;

            sqlite3_free(erro);
        }
    }

    ~Historico()
    {
        sqlite3_close(db);
    }
};

#endif