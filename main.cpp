#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
/*Desenvolvido por: Guilherme Parreira e Higor Soares.*/

#include "CONTROLADOR.hpp"
#include "HISTORICO.hpp"
#include "ETA.hpp"
#include "VALVULA_CONSUMO.hpp"
#include "COMMAND.hpp"
#include "COMMAND_FACTORY.hpp"

using namespace std;

const int ID_DUPLA = 80; // ID próprio da Dupla (24 + 56) 

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
     ValvulaConsumo valvulaConsumo("XV-CONS", "Area 1", 1.0, 25.0);

     sensor_nivel sensorNivel("LT-101", "Area 1", 0.0, 150.0, 1000.0, &reservatorio);
     sensor_vazao sensorVazao("FT-101", "Area 1", 0.0, 25.0, &bomba);
     sensor_ph sensorPH("PH-101", "Area 1", 7.0, 6.0, 8.0);
     sensor_turbidez sensorTurbidez("TB-101", "Area 1", 0.5, 0.0, 1.0);

     alarme_ph alarmePH("AH-PH", "Area 1");
     alarme_nivel alarmeNivel("AH-NV", "Area 1");
     alarme_vazao alarmeVazao("AH-VZ", "Area 1");
     alarme_turbidez alarmeTurbidez("AH-TB", "Area 1");
     alarme_racionamento alarmeRac("AH-RAC", "Area 1");

     Controlador controlador("CTRL-101", (620 + ID_DUPLA), ID_DUPLA, 0.8, 0.03); // Substituir 80 pelo ID_DUPLA e também o setpoint para 620 + ID_DUPLA
     Historico historico("historico_eta.db");

     double consumo_externo_solicitado = 5.0 + ((double)rand() / RAND_MAX) * 20.0;

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

          if (arquivo_comando.is_open()){
               
               string texto_comando;

               getline(arquivo_comando, texto_comando);

               arquivo_comando.close();

          if (!texto_comando.empty()){
        
            auto comando = CommandFactory::criarComando(texto_comando, &controlador, &eta, &sensorNivel);

               if (comando){
            
                    comando->executar();

                    // Limpa o arquivo
                    ofstream limpar("comando.txt");
                    limpar.close();

                    // Se o comando executado foi EXIT,
                    // encerra o programa.
                    if (dynamic_cast<ExitCommand *>(comando.get()) != nullptr){
                    break;
                    }
               } 
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

          // Nova perturbação uniforme após estabilizar
          if (ciclos_estavel >= 5)
          {
               consumo_externo_solicitado = 5.0 + ((double)rand() / RAND_MAX) * 20.0;

               cout << "\n*** PERTURBACAO GERADA (Nova Demanda Externa) ***\n";
               cout << "Consumo Externo Solicitado: " << consumo_externo_solicitado << " m³/ciclo\n";

               ciclos_estavel = 0;
          }

          // Controle em malha fechada
          controlador.controlar_nivel(&sensorNivel, &reservatorio, &bomba, &valvula, &inversor, &valvulaConsumo, consumo_externo_solicitado);

          // Verificação e execução da falha simulada no sensor de PH
          if (ciclo == 20) {
               cout << "\n=====================================\n";
               cout << "FALHA SIMULADA" << endl;
               cout << "Sensor de pH perdeu comunicação." << endl;
               cout << "=====================================\n";

               sensorPH.ativar_falha();
          }

          if(ciclo == 25){ // Colocar um botão no supervisório para reparar falha
               cout << "\n==========================\n";
               cout << " A FALHA FOI REPARADA" << endl;
               cout << "Sensor de pH voltou a ter comunicação." << endl;

               sensorPH.reparar_falha();
          }

          // Verifica estabilidade
          double nivel = sensorNivel.ler_valor();

          if (abs(nivel - controlador.get_setpoint()) <= controlador.get_tolerancia())
          {
               ciclos_estavel++;
          }
          else
          {
               ciclos_estavel = 0;
          }

          // Monitoramento avaliando diretamente o consumo solicitado
          controlador.monitorar(&sensorPH, &sensorNivel, &sensorTurbidez, &sensorVazao, &alarmePH, &alarmeNivel, &alarmeVazao, &alarmeTurbidez, &alarmeRac, consumo_externo_solicitado);

          // Criação do TimeStamp para a o processo de registrar no histórico
          
          time_t agora = time(nullptr);

          tm *tempo_local = localtime(&agora);

          stringstream ss;

          ss << put_time(tempo_local, "%Y-%m-%d %H:%M:%S");

          string timestamp = ss.str();
          
          // Registro atualizado incluindo o alarme de racionamento no banco de dados SQLite
          historico.registrar(timestamp, ciclo, sensorNivel.ler_valor(), sensorVazao.ler_valor(), sensorPH.ler_valor(), sensorTurbidez.ler_valor(), valvulaConsumo.get_abertura(),
                              bomba.esta_operando(), valvula.esta_aberta(), alarmePH.esta_ativo(), alarmeNivel.esta_ativo(), alarmeVazao.esta_ativo(), alarmeTurbidez.esta_ativo(), alarmeRac.esta_ativo());

          // Escrita no JSON
          /*Criação da lógica da escrita do JSON LINHA, pois o padrão JSON não permite trabalhar em
          loop infinito, pois nunca fecha a lógica []. O JSON Linha foi criado justamente para cobrir essa parte.*/
          
          // Criação de um novo objeto JSONL referente ao ciclo atual
          json << "{";

          // Número do ciclo da simulação
          json << "\"ciclo\":" << ciclo << ",";
          // TimeStamp referente a quando o ciclo aconteceu
          json << "\"timestamp\":\"" << timestamp << "\",";
          // Setpoint desejado para o controlador PI
          json << "\"setpoint\":" << controlador.get_setpoint() << ",";
          // Faixa desejada de tolerância aceitável em torno do setpoint 
          json << "\"tolerancia\":" << controlador.get_tolerancia() << ",";
          // Vazão efetiva fornecida à rede de distribuição
          json << "\"vazao_saida\":" << valvulaConsumo.get_vazao() << ",";
          // Demanda externa solicitada pelo sistema consumidor
          json << "\"demanda\":" << consumo_externo_solicitado << ",";

          // Bloco referente ao sensores do ETA
          json << "\"sensores\":{";
          // Volume atual do reservatório
          json << "\"nivel\":" << sensorNivel.ler_valor() << ",";
          // Vazão medida na saída da bomba
          json << "\"vazao\":" << sensorVazao.ler_valor() << ",";
          // Valor atual do pH
          json << "\"ph\":" << sensorPH.ler_valor() << ",";
          // Valor atual da turbidez
          json << "\"turbidez\":" << sensorTurbidez.ler_valor();
          json << "},";

          // Bloco dos Atuadores do ETA
          json << "\"atuadores\":{";
          // Frequência aplicada pelo inversor à bomba
          json << "\"frequencia\":" << inversor.get_frequencia() << ",";
          // Vazão produzida pela bomba
          json << "\"vazao_bomba\":" << bomba.get_vazao() << ",";
          // Percentual de abertura da válvula de consumo
          json << "\"abertura_consumo\":" << valvulaConsumo.get_abertura() << ",";
          // Estado da válvula de alívio
          json << "\"valvula_alivio\":" << (valvula.esta_aberta() ? "true" : "false");
          json << "},";

          // Bloco de alarmes do Eta
          json << "\"alarmes\":{";
          // Alarme PH
          json << "\"ph\":" << (alarmePH.esta_ativo() ? "true" : "false") << ",";
          // Alarme nivel
          json << "\"nivel\":" << (alarmeNivel.esta_ativo() ? "true" : "false") << ",";
          // Alarme vazao
          json << "\"vazao\":" << (alarmeVazao.esta_ativo() ? "true" : "false") << ",";
          // Alarme trubidez
          json << "\"turbidez\":" << (alarmeTurbidez.esta_ativo() ? "true" : "false") << ",";
          // Alarme indicando regime de racionamento  
          json << "\"racionamento\":" << (alarmeRac.esta_ativo() ? "true" : "false");
          json << "}";

          json << "}\n";

          json.flush();

          this_thread::sleep_for(chrono::seconds(1));

          cout << "Volume atual: " << reservatorio.get_volume_atual() << " m3\n";
          cout << "Consumo Solicitado: " << consumo_externo_solicitado << " m3/ciclo\n";
          cout << "Vazao Efetiva de Saida: " << valvulaConsumo.get_vazao() << " m3/ciclo\n";
     }
     json.close();

     cout << "Sistema encerrado com sucesso." << endl;

     return 0;
}