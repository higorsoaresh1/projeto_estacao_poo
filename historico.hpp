#ifndef HISTORICO_HPP
#define HISTORICO_HPP

#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

class Historico
{ /*Classe que representa o histórico de dados no sistema de ETA*/
private:
    sqlite3 *db;

public:
    Historico(const string &nome_banco)
    { /*Abertura do banco de dados*/
        if (sqlite3_open(nome_banco.c_str(), &db) != SQLITE_OK)
        {
            cerr << "Erro ao abrir banco: " << sqlite3_errmsg(db) << endl;
        }
        else
        {
            cout << "Banco conectado.\n";

            sqlite3_exec(db,
                         "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);

            criar_tabela();
        }
    }

    void criar_tabela()
    { /*Criação da tabela com os comandos SQL*/
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
        /*Verificação de erro*/
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro) != SQLITE_OK)
        {
            cerr << "Erro ao criar tabela: " << erro << endl;

            sqlite3_free(erro);
        }
    }

    void registrar(int ciclo, double nivel, double vazao, double ph, double turbidez, double consumo, bool bomba,
                   bool valvula, bool alarme_ph, bool alarme_nivel, bool alarme_vazao, bool alarme_turbidez, bool alarme_rac)
    { /*Registro dos dados no banco de dados*/
        string sql = "INSERT INTO historico "
                     "(ciclo,nivel,vazao,ph,turbidez,consumo,"
                     "bomba,valvula,alarme_ph,alarme_nivel,"
                     "alarme_vazao,alarme_turbidez) VALUES (" +
                     to_string(ciclo) + "," + to_string(nivel) + "," + to_string(vazao) + "," + to_string(ph) + "," + to_string(turbidez) + "," + to_string(consumo) + "," + to_string(bomba) + "," + to_string(valvula) + "," + to_string(alarme_ph) + "," + to_string(alarme_nivel) + "," + to_string(alarme_vazao) + "," + to_string(alarme_turbidez) + ");";

        char *erro = nullptr;
        /*Verificação de erro*/
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro) != SQLITE_OK)
        {
            cerr << "Erro ao inserir: " << erro << endl;

            sqlite3_free(erro);
        }
    }

    ~Historico()
    { /*Fecha a conexão com o banco de dados*/
        sqlite3_close(db);
    }
};

#endif