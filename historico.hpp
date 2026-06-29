#ifndef HISTORICO_HPP
#define HISTORICO_HPP

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <stdexcept>

using namespace std;

class Historico
{ /*Classe que representa o histórico de dados no sistema de ETA*/
private:
    sqlite3 *db;

public:
    Historico(const string &nome_banco) : db(nullptr)
    { /*Abertura do banco de dados*/
        if (sqlite3_open(nome_banco.c_str(), &db) != SQLITE_OK)
        {
            /*Se falhar, avisa o programa principa*/
            string erro = sqlite3_errmsg(db);
            if (db)
            {
                sqlite3_close(db);
                db = nullptr;
            }
            throw runtime_error("[ERRO BANCO] Não foi possível abrir o banco: " + erro);
        }

        cout << "Banco conectado.\n";
        sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
        criar_tabela();
    }

    void criar_tabela()
    { /*Criação da tabela com os comandos SQL*/
        string sql =
            "CREATE TABLE IF NOT EXISTS historico ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "timestamp TEXT,"
            "ciclo INTEGER,"
            "setpoint REAL,"
            "tolerancia REAL,"
            "demanda REAL,"
            "vazao_entrada REAL,"
            "vazao_saida REAL,"
            "nivel REAL,"
            "valvula_saida REAL,"
            "ph REAL,"
            "turbidez REAL,"
            "bomba INTEGER,"
            "valvula INTEGER,"
            "alarme_ph INTEGER,"
            "alarme_nivel INTEGER,"
            "alarme_vazao INTEGER,"
            "alarme_turbidez INTEGER,"
            "alarme_racionamento INTEGER"
            ");";

        char *erro = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro) != SQLITE_OK)
        {
            cerr << "Erro ao criar tabela: " << erro << endl;
            sqlite3_free(erro);
        }
    }

    void registrar(string timestamp, int ciclo, double nivel, double vazao_entrada, double vazao_saida, double ph, double setpoint, double tolerancia,
                   double demanda, double turbidez, double valvula_saida, bool bomba, bool valvula, bool alarme_ph, bool alarme_nivel,
                   bool alarme_vazao, bool alarme_turbidez, bool alarme_rac)
    { /*Registro dos dados usando a sua concatenação original*/

        /*Se o banco não estiver aberto, não faz nada*/
        if (!db)
            return;

        string sql = "INSERT INTO historico "
                     "(timestamp, ciclo, setpoint, tolerancia, demanda, vazao_entrada, vazao_saida, nivel, valvula_saida, ph, turbidez, "
                     "bomba, valvula, alarme_ph, alarme_nivel, alarme_vazao, alarme_turbidez, alarme_racionamento) VALUES ('" +
                     timestamp + "'," +
                     to_string(ciclo) + "," +
                     to_string(setpoint) + "," +
                     to_string(tolerancia) + "," +
                     to_string(demanda) + "," +
                     to_string(vazao_entrada) + "," +
                     to_string(vazao_saida) + "," +
                     to_string(nivel) + "," +
                     to_string(valvula_saida) + "," +
                     to_string(ph) + "," +
                     to_string(turbidez) + "," +
                     to_string(bomba) + "," +
                     to_string(valvula) + "," +
                     to_string(alarme_ph) + "," +
                     to_string(alarme_nivel) + "," +
                     to_string(alarme_vazao) + "," +
                     to_string(alarme_turbidez) + "," +
                     to_string(alarme_rac) +
                     ");";

        char *erro = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &erro) != SQLITE_OK)
        {
            cerr << "Erro ao inserir dados: " << erro << endl;
            sqlite3_free(erro);
        }
    }

    ~Historico()
    { /*Fecha a conexão com o banco de dados de forma segura*/
        if (db)
        {
            sqlite3_close(db);
        }
    }
};

#endif