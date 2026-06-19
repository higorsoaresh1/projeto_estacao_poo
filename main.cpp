#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <thread>
#include <chrono>

/*Desenvolvido por: Guilherme Parreira e Higor Soares.*/

#include "CONTROLADOR.hpp"
#include "HISTORICO.hpp"
#include "ETA.hpp"
#include "VALVULA_CONSUMO.hpp"

using namespace std;

int main()
{
     remove("historico_eta.db");
     remove("historico_eta.db-wal");
     remove("historico_eta.db-shm");

     remove("dados_eta.jsonl");
     remove("comando.txt");

     srand(time(nullptr));

     // Gera o volume inicial do reservatório de forma aleatória entre 0 e 1000 m³ para simular diferentes condições iniciais.
     double volume_inicial = rand() % 1000;
     ETA eta("Area 1");

     Reservatorio reservatorio("TK-101", "Area 1", 1000.0, volume_inicial);

     Inversor inversor("INV-101", "Area 1");

     Bomba bomba("P-101", "Area 1", 20.0, &inversor);

     Valvula valvula("XV-101", "Area 1", 15.0);
     ValvulaConsumo valvulaConsumo("XV-CONS", "Area 1", 0.0, 25.0);

     sensor_nivel sensorNivel("LT-101", "Area 1", 0.0, 0.0, 1000.0, &reservatorio);
     sensor_vazao sensorVazao("FT-101", "Area 1", 0.0, 25.0, &bomba);
     sensor_ph sensorPH("PH-101", "Area 1", 7.0, 6.0, 8.0);
     sensor_turbidez sensorTurbidez("TB-101", "Area 1", 0.5, 0.0, 1.0);

     alarme_ph alarmePH("AH-PH", "Area 1");
     alarme_nivel_alto alarmeNivel("AH-NV", "Area 1");
     alarme_vazao alarmeVazao("AH-VZ", "Area 1");
     alarme_turbidez alarmeTurbidez("AH-TB", "Area 1");

     Controlador controlador("CTRL-101", 700.0, 80.0, 0.8, 0.03);
     Historico historico("historico_eta.db");

     double abertura_consumo = 20;
     valvulaConsumo.set_abertura(abertura_consumo / 100.0);

     int ciclo = 0;
     int ciclos_estavel = 0;

     /*Escrita do JSON.*/
     ofstream json("dados_eta.jsonl");

     if (!json.is_open())
     {
          cerr << "Erro ao abrir o arquivo JSON para escrita!" << endl;
          return 1;
     }

     eta.iniciar_tratamento();

     while (true)
     {
          ifstream arquivo_comando("comando.txt");

          string comando;

          if (arquivo_comando.is_open())
          {
               getline(arquivo_comando, comando);
               arquivo_comando.close();

               if (comando == "STOP")
               {
                    eta.parar_tratamento();

                    ofstream limpar("comando.txt");
                    limpar << "";
                    limpar.close();
               }
               else if (comando == "START")
               {
                    eta.iniciar_tratamento();

                    ofstream limpar("comando.txt");
                    limpar << "";
                    limpar.close();
               }
               else if (comando.find("SETPOINT=") == 0)
               {
                    string valor = comando.substr(9);

                    controlador.set_setpoint(stod(valor));

                    cout << "Novo setpoint recebido: " << controlador.get_setpoint() << " m3\n";

                    ofstream limpar("comando.txt");
                    limpar << "";
                    limpar.close();
               }
               else if (comando.find("TOLERANCIA=") == 0)
               {
                    string valor = comando.substr(11);

                    controlador.set_tolerancia(stod(valor));

                    cout << "Nova tolerancia recebida: " << controlador.get_tolerancia() << " m3\n";

                    ofstream limpar("comando.txt");
                    limpar << "";
                    limpar.close();
               }
               else if (comando == "EXIT")
               {
                    cout << "Encerrando sistema..." << endl;

                    ofstream limpar("comando.txt");
                    limpar << "";
                    limpar.close();

                    break;
               }
          }

          if (!eta.esta_operando())
          {
               cout << "ETA parada pelo supervisório.\n";

               this_thread::sleep_for(chrono::seconds(1));

               continue;
          }

          ciclo++;

          cout << "\n===== CICLO " << ciclo << " =====\n";

          // Controle em malha fechada
          controlador.controlar_nivel(&sensorNivel, &reservatorio, &bomba, &valvula, &inversor, &valvulaConsumo);

          // Verifica se está próximo do setpoint
          double nivel = sensorNivel.ler_valor();

          if (abs(nivel - controlador.get_setpoint()) <= controlador.get_tolerancia())
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
               abertura_consumo = 20 + rand() % 61;
               valvulaConsumo.set_abertura(abertura_consumo / 100.0);

               cout << "\n*** PERTURBACAO GERADA ***\n";
               cout << "Nova abertura: " << abertura_consumo << " %\n";

               ciclos_estavel = 0;
          }

          // Monitoramento
          controlador.monitorar(&sensorPH, &sensorNivel, &sensorTurbidez, &sensorVazao, &alarmePH, &alarmeNivel, &alarmeVazao, &alarmeTurbidez);

          historico.registrar(ciclo, sensorNivel.ler_valor(), sensorVazao.ler_valor(), sensorPH.ler_valor(), sensorTurbidez.ler_valor(), valvulaConsumo.get_abertura(),
                              bomba.esta_operando(), valvula.esta_aberta(), alarmePH.esta_ativo(), alarmeNivel.esta_ativo(), alarmeVazao.esta_ativo(), alarmeTurbidez.esta_ativo());

          // Escrita no JSON
          /*Criação da lógica da escrita do JSON LINHA, pois o padrão JSON não permite trabalhar em
          loop infinito, pois nunca fecha a lógica []. O JSON Linha foi criado justamente para cobrir essa parte.*/
          json << "{";

          json << "\"ciclo\":" << ciclo << ",";
          json << "\"setpoint\":" << controlador.get_setpoint() << ",";
          json << "\"tolerancia\":" << controlador.get_tolerancia() << ",";
          json << "\"consumo\":" << valvulaConsumo.get_vazao() << ",";

          json << "\"sensores\":{";
          json << "\"nivel\":" << sensorNivel.ler_valor() << ",";
          json << "\"vazao\":" << sensorVazao.ler_valor() << ",";
          json << "\"ph\":" << sensorPH.ler_valor() << ",";
          json << "\"turbidez\":" << sensorTurbidez.ler_valor();
          json << "},";

          json << "\"atuadores\":{";
          json << "\"frequencia\":" << inversor.get_frequencia() << ",";
          json << "\"vazao_bomba\":" << bomba.get_vazao() << ",";
          json << "\"abertura_consumo\":" << valvulaConsumo.get_abertura() << ",";
          json << "\"valvula_alivio\":" << (valvula.esta_aberta() ? "true" : "false");
          json << "},";

          json << "\"alarmes\":{";
          json << "\"ph\":" << (alarmePH.esta_ativo() ? "true" : "false") << ",";
          json << "\"nivel\":" << (alarmeNivel.esta_ativo() ? "true" : "false") << ",";
          json << "\"vazao\":" << (alarmeVazao.esta_ativo() ? "true" : "false") << ",";
          json << "\"turbidez\":" << (alarmeTurbidez.esta_ativo() ? "true" : "false");
          json << "}";

          json << "}\n";

          json.flush();

          this_thread::sleep_for(chrono::seconds(1));

          cout << "Volume atual: " << reservatorio.get_volume_atual() << " m3\n";

          cout << "Consumo atual: " << valvulaConsumo.get_vazao() << " m3/ciclo\n";
     }
     json.close();

     cout << "Sistema encerrado com sucesso." << endl;

     return 0;
}