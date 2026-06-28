# Sistema Supervisório para Estação de Tratamento de Água (ETA)

**Disciplina:** Programação Orientada a Objetos

**Instituto:** Instituto Federal do Espírito Santo (IFES) - Campus Serra

**ID da Dupla:** 80 (24 + 56)

## Autores

- Guilherme Parreira
- Higor Soares

# Objetivo do trabalho

Este projeto simula uma Estação de Tratamento de Água (ETA) utilizando programação orientada a objetos em C++, supervisão em Python (Streamlit) e armazenamento histórico em banco de dados SQLite. O sistema implementa uma malha de controle PI responsável pelo controle do nível de um reservatório, permitindo monitoramento em tempo real, alteração de parâmetros operacionais e geração de alarmes.

# Funcionalidades 

- Controle automático do nível do reservatório utilizando controlador PI;
- Simulação de sensores industriais;
- Simulação de bomba com inversor de frequência;
- Simulação de válvula de consumo;
- Simulação de válvula de alívio;
- Supervisório desenvolvido em Streamlit;
- Registro histórico utilizando SQLite;
- Exportação contínua dos dados em JSON Lines;
- Alteração remota de Setpoint e Tolerância;
- Sistema de alarmes;
- Sistema de racionamento automático quando ocorre sobrecarga de demanda;
- Geração de perturbações aleatórias para teste da malha de controle.

# Estrutura do Projeto

- main.cpp
- supervisorio.py
- ALARME.hpp
- BOMBA.hpp
- CONTROLADOR.hpp
- ETA.hpp
- HISTORICO.hpp
- INVERSOR.hpp
- RESERVATORIO.hpp
- SENSOR.hpp
- VALVULA.hpp
- VALVULA_CONSUMO.hpp
- Command.hpp
- Command_Factory.hpp
- dados_eta.jsonl
- historico_eta.db
- comando.txt
- uml_eta.mmd

# Arquitetura do sistema

ETA - Representa a estação de tratamento

Controlador - Executa o controle PI ( Proporcional-Integral)

Reservatório - Armazena o volume de água

Sensores(ph, turbidez, nível, vazão) - Monitoram as variáveis do processo

Alarmes(ph,turbidez, nível, vazão, racionamento) - Alertam sobre mudanças indesejadas dentro do sistema 

Bomba - Realiza o enchimento do reservatório

Inversor - Controla a frequência da bomba

Válvula de Consumo - Simula a demanda externa

válvula de Alívio - Evita transbordamento

Histórico - Armazena dados no SQLite, servindo como a memória do sistema

Supervisório - Interface gráfica do operador

Command - Execução dos comandos de atuação do sistema

Command_Factory - Criação dos comandos de atuação

# Tecnologias utilizadas no projeto

- C++17
- Python 3
- SQLite3
- Streamlit
- JSON Lines
- Mermaid UML

# Padrões de projeto

Os padrões de projeto aplicados nesse sistema foram COMMAND, REPOSITORY e FACTORY. 

1. O padrão de projeto Command, implementado por meio das classes command.hpp, command_factory.hpp e das adaptações realizadas na main.cpp, tem como principal objetivo encapsular cada comando de atuação do sistema em uma classe específica, representando cada ação como um objeto independente. Dessa forma, operações como iniciar, parar, alterar o setpoint, modificar a tolerância e encerrar a aplicação passam a ser tratadas de maneira padronizada. 

A principal motivação para a adoção desse padrão foi melhorar a organização da estrutura da main.cpp, que anteriormente concentrava toda a lógica de processamento dos comandos recebidos pelo supervisório. Com a utilização do padrão Command, a responsabilidade pela execução de cada operação foi transferida para classes especializadas, tornando o código mais modular, de fácil manutenção, além de facilita futuras expansões do sistema, permitindo a inclusão de novos comandos sem a necessidade de modificar significativamente a lógica existente na aplicação.

2. O padrão de projeto Repository foi aplicado por intermédio da classe historico.hpp, que atua como uma camada intermediária entre a aplicação e o banco de dados SQLite. Seu principal objetivo é concentrar todas as operações de acesso e registro dos dados, tirando a necessidade dos detalhes de implementação do banco de dados serem conhecidos nas demais classes do sistema. Nesse projeto, a classe historico é responsável por estabelecer a conexão com o banco de dados SQLite, criar automaticamente a tabela de registros quando necessário e armazenar, a cada ciclo da simulação, as informações referentes aos sensores, atuadores e alarmes da ETA. 

O uso desse padrão proporcionou uma melhor separação de responsabilidades, mantendo a lógica de controle de dados isolada da lógica de controle da planta. Além disso, essa abordagem facilita futuras manutenções e expansões do sistema, permitindo, por exemplo, substituir o banco de dados SQLite por outra tecnologia de armazenamento.

3. O padrão de projeto Factory foi aplicado por intermédio da classe CommandFactory.hpp, responsável por centralizar a criação dos diferentes comandos utilizados pelo sistema supervisório. Seu principal objetivo é separar a lógica de criação dos objetos da lógica de execução da aplicação, evitando que a classe principal main.cpp precise conhecer os detalhes de construção de cada comando disponível.Por exemplo, nesse projeto, a CommandFactory recebe os comandos enviados pelo supervisório na forma de texto (como START, STOP, SETPOINT, TOLERANCIA , EXIT, INICIAR_FALHA_PH, RESOLVER_FALHA_PH) e a partir dessas informações, instancia automaticamente o objeto correspondente, retornando como ponteiro para a classe command.

A utilização desse padrão proporcionou uma melhor organização da estrutura do código, separando a responsabilidade de criação dos comandos da lógica principal da aplicação. Além disso, essa abordagem facilita futuras expansões do sistema, permitindo adicionar novos comandos ao supervisório com poucas modificações.

# Controle PI 

O controle do nível do reservatório é realizado por meio de um Controlador Proporcional-Integral (PI), cuja função é manter o volume do reservatório o mais próximo possível do valor definido pelo operador (setpoint), compensando automaticamente as variações provocadas pelo consumo externo.

A cada ciclo da simulação, o controlador realiza a leitura do sensor de nível, calcula o erro em relação ao valor desejado e determina a frequência de operação do inversor, que por sua vez controla a vazão fornecida pela bomba.

Explicação dos parâmetros utilizados dentro da lógica do PI:

Setpoint - Volume desejado para o reservatório. É o valor que o controlador tenta manter durante toda a operação.

Nível Atual - Valor medido pelo sensor de nível, correspondente ao volume presente no reservatório naquele instante.

Erro - Diferença entre o setpoint e o nível atual do reservatório. É calculado pela expressão: Erro = Setpoint − Nível Atual.

Tolerância - Faixa de operação considerada aceitável ao redor do setpoint. Enquanto o nível permanecer dentro dessa região, considera-se que o sistema está estabilizado.

Ganho Proporcional(Kp) - Determina a intensidade da resposta imediata do controlador diante do erro. Quanto maior seu valor, mais rapidamente o sistema reage às variações do nível.

Ganho Integral(Ki) - Responsável por acumular o erro ao longo do tempo, eliminando erros permanentes e permitindo que o nível atinja o setpoint com maior precisão.

Erro integral - Soma acumulada dos erros ao longo dos ciclos da simulação. Essa variável é utilizada pelo termo integral do controlador PI.


# Sistema de alarmes

Alarmes e suas condições de ativação:

Alarme Nível - Volume fora da faixa

Alarme Vazão - Vazão fora do intervalo 

Alarme PH - pH fora do limite aceitável 

Alarme Turbidez - Turbidez elevada

Alarme Racionamento - Demanda superior à capacidade

# JSONL

Durante o desenvolvimento do projeto, foi necessária a utilização do formato JSON Lines (JSONL), uma vez que os dados da simulação são gerados continuamente dentro de um laço de execução infinito. O formato JSON tradicional não é adequado para esse cenário, pois exige que o documento seja finalizado corretamente com o fechamento de sua estrutura. Dessa forma, o formato JSONL foi adotado por permitir que cada ciclo da simulação seja registrado como um objeto JSON independente em uma nova linha do arquivo.

Um exemplo de JSONL dentro do sistema:

{
  "ciclo": 1,
  "timestamp": "2026-06-28 01:56:20",
  "setpoint": 700,
  "tolerancia": 80,
  "vazao_saida": 5.22706,
  "demanda": 5.22706,

  "sensores": {
    "nivel": 345.773,
    "vazao": 20.0,
    "ph": 7.0,
    "turbidez": 0.5
  },

  "atuadores": {
    "frequencia": 100,
    "vazao_bomba": 20.0,
    "abertura_consumo": 0.209082,
    "valvula_alivio": false
  },

  "alarmes": {
    "ph": false,
    "nivel": false,
    "vazao": false,
    "turbidez": false,
    "racionamento": false
  }
}

Onde: 

- TimeStamp refere-se à data e ao horário em que o ciclo da simulação foi executado.
- Ciclo trata-se do número do ciclo da simulação.
- Setpoint é o nível desejado do reservatório.
- Tolerância representa a faixa aceitável em torno do setpoint.
- Vazao_saida corresponde à vazão efetivamente enviada aos consumidores.
- Demanda representa a vazão solicitada pelo sistema consumidor.
- Sensores.nivel indica o volume atual do reservatório.
- Sensores.vazao indica a vazão medida pelo sensor de vazão, correspondente à vazão fornecida pela bomba.
- Sensores.ph indica o valor do pH da água.
- Sensores.turbidez indica a turbidez da água.
- Atuadores.frequencia representa a frequência aplicada ao inversor de frequência.
- Atuadores.vazao_bomba indica a vazão produzida pela bomba.
- Atuadores.abertura_consumo representa o percentual de abertura da válvula de consumo, variando entre 0 (fechada) e 1 (totalmente aberta).
- Atuadores.valvula_alivio indica o estado da válvula de alívio, sendo true para aberta e false para fechada.
- Alarmes.ph indica a ocorrência de alarme de pH.
- Alarmes.nivel indica a ocorrência de alarme de nível.
- Alarmes.vazao indica a ocorrência de alarme de vazão.
- Alarmes.turbidez indica a ocorrência de alarme de turbidez.
- Alarmes.racionamento indica que o sistema entrou em regime de racionamento devido a uma demanda superior à capacidade de fornecimento.


# Supervisório 

O supervisório foi desenvolvido utilizando Python e a biblioteca Streamlit, sendo responsável pela interface entre o operador e a simulação da Estação de Tratamento de Água (ETA). Sua principal função é apresentar, em tempo real, o estado do processo, permitindo o monitoramento das variáveis da planta, a visualização dos alarmes e o envio de comandos de operação ao sistema.

Por meio do supervisório é possível visualizar:

- Nível atual do reservatório por meio de um tanque animado;
- Leituras dos sensores de nível, vazão, pH e turbidez;
- Frequência aplicada ao inversor de frequência;
- Vazão fornecida pela bomba;
- Vazão de saída para o sistema consumidor;
- Percentual de abertura da válvula de consumo;
- Estado da válvula de alívio;
- Gráfico histórico da evolução do nível do reservatório;
- Timestamp da última atualização do sistema;
- Estado de todos os alarmes da planta.

Além do monitoramento, o supervisório também permite a atuação direta sobre a simulação por meio do envio de comandos, possibilitando alterar parâmetros de operação, iniciar ou interromper a planta, simular falhas e encerrar a aplicação.

# Banco de Dados

O sistema utiliza um banco de dados SQLite para armazenar permanentemente todas as informações geradas durante a simulação da Estação de Tratamento de Água (ETA). A comunicação entre a aplicação e o banco é realizada exclusivamente pela classe Historico, sendo responsável pela criação da tabela e pelo registro automático dos dados a cada ciclo da simulação.

A tabela historico possui a seguinte estrutura:

- ID - Identificador único do registro (gerado automaticamente).
- TimeStamp 
- Ciclo 
- Setpoint 
- Tolerância 
- Demanda 
- Vazao_entrada
- Vazao_saida
- Nível
- Valvula_saida
- Ph
- Turbidez
- Bomba
- Valvula
- Alarme_ph
- Alarme_nivel
- Alarme_vazão
- Alarme_turbidez
- Alarme_racionamento

# Comandos de atuação 

A comunicação entre o supervisório e a aplicação em C++ é realizada por meio do arquivo comando.txt. Sempre que o operador aciona um botão no supervisório, um comando em formato textual é gravado nesse arquivo. Durante cada ciclo da simulação, a aplicação verifica se existe um novo comando disponível e, caso exista, utiliza a classe CommandFactory para identificar o comando recebido e instanciar automaticamente o objeto correspondente.

Os comandos implementados no projeto são:

- START - Inicia o funcionamento da ETA.

- STOP - Interrompe a operação da ETA, mantendo a aplicação em execução.

- SETPOINT - Altera o nível de referência utilizado pelo controlador PI.

- TOLERANCIA - Modifica a faixa de tolerância utilizada pelo controlador.

- EXIT - Encerra completamente a aplicação.

- FALHA_CONEXAO_PH - Simula uma falha de comunicação no sensor de pH, fazendo com que o supervisório deixe de receber leituras válidas desse sensor.

- REPARAR_PH - Restabelece a comunicação do sensor de pH, encerrando a falha simulada e permitindo que as leituras voltem ao funcionamento normal.

# Assinatura operacional da dupla

Alterações do sistema por meio do ID_DUPLA:

- Altera o identificador do controlador, afetando o valor inicial do SetPoint e da Tolerância inicial;

- Participa da falha simulada, sendo passado como parâmetro que indica o erro de conexão do sensor de PH;

-  Afeta também o ganho proporcional utilizado para o sistema do controlador PI;

# Como executar o programa

1. Iniciamente utilize o comando em um terminal: g++ main.cpp -lsqlite3 -o eta

2. Após isso, nesse mesmo terminal, utilize o seguinte comando: ./eta

3. Depois de executar o código c++, abra um novo terminal e aplique o seguinte comando para abrir o supervisório StreamLit: python -m streamlit run supervisorio.py
4. Após todas essas aplicações, o sistema deve rodar sem nenhum problema.

# UML 

// Atualziar a imagem depois//


# Histórico de desenvolvimento do projeto

20/05/2026 - Criação da UML no mermaid (discord compartilhado);

28/05/2026 - Criação do Repositório e arquivos base no github (discord compartilhado);

30/05/2026 - Inicio da construção do projeto em c++ (discord compartilhado);

31/05/2026 - Continuação do código em c++ (discord compartilhado);

06/06/2026 - Continuação do código em c++, aprimorando as classes e atributos já construidos e iniciando a formação de um sistema de malha fechada.(Discord compartilhado- 3 horas seguidas);

07/06/2026 - Continuação do código em c++( Continuação da implementação da malha fechada dentro do sistema de nível do reservatório, junto ao inicio da formação desse processo na main do programa) - (Discord compartilhado, cerca de 1 hora e 40);

08/06/2026 - Separação das classes e da main em arquivos distintos para possibilitar uma melhor organização do código. Além disso, também foi iniciada a construção do JSON dentro da Main - (Discord compartilhado, cerca de 2 horas);

10/06/2026 - Finalização da construção do JSON dentro da Main, junto com a adaptação desse para uma malha fechada - (Discord compartilhado, cerca de 1 hora e 20 minutos);

11/06/2026 - Configuração do SQL e do SQLITE para a criação da classe histórico - (Discord compartilhado, cerca de 1 hora);

12/06/2026 - Inicio da criação da classe histórico e do supervisório - (Discord compartilhado, cerca de 2 horas e 50 minutos);

13/06/2026 - "Finalização" e testes da classe histórico e do supervisório, com implementação do Streamlit - (Cerca de 1 hora e 30 minutos);

16/06/2026 - Reparo de erros dentro do VScode e melhorias na estrtura do código - (Discord compartilhado, cerca de 2 horas);

17/06/2026 - Discussão de estrategias de modelagem para melhorar a planta de nível - (Sala de aula, cerca de 1 hora e 30 minutos);

18/06/2026 - Reeconstrução das classes de válvula e válvula de alívio, juntamente a adaptação da malha fechada a essas novas alterações. Criação da classe inversora, além da remodelação das classes de reservatório, controlador e bomba. Além disso, a main e o supervisório foram modificados para suportarem essas alterações e para melhorar as antigas funcionalidades - (Discord compartilhado, cerca de 5 horas);

21-06-2026 - Reesconstrução da malha fechada e construção do sistema de consumo externo (Discord compartilhado, cerca de 3 horas e 50 minutos);

22-06-2026 - Criação do tanque animado, alterações extras no supervisório, atualizações de histórico e AI_LOG ( Discord, cerca de 3 horas);

27-06-2026- Atualização do README, aplicação dos padrões de projeto Commannd e Factory, ID DUPLA, correção de problemas relacionados ao nível por meio da implementação da lógica Anti-Windup, atualização do banco de dados e criação de falha simulada, além de diversas melhorias no código (Discord, cerca de 7 horas);

28-06-2026 - 