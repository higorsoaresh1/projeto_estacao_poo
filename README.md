# ID_dupla = 80( 24 + 56)

# Sistema Supervisório para Estação de Tratamento de Água (ETA)

Projeto desenvolvido para a disciplina de Programação Orientada a Objetos do Instituto Federal do Espírito Santo, Serra.
    
Autores:

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

├── main.cpp
├── supervisorio.py
├── ALARME.hpp
├── BOMBA.hpp
├── CONTROLADOR.hpp
├── ETA.hpp
├── HISTORICO.hpp
├── INVERSOR.hpp
├── RESERVATORIO.hpp
├── SENSOR.hpp
├── VALVULA.hpp
├── VALVULA_CONSUMO.hpp
│
├── dados_eta.jsonl
├── historico_eta.db
├── comando.txt
│
└── uml_eta.mmd

# Arquitetura do sistema

// Criar uma tabela aqui explicando cada sistema do projeto ( Ex: Bomba, controlador, etc...); //

# Tecnologias utilizadas no projeto

- C++17
- Python 3
- SQLite3
- Streamlit
- JSON Lines
- Mermaid UML

# Padrões de projeto

// Comentar os padrões de projetos utilizados e suas funcionalidades // 

# Controle PI 

// Explicar melhor o funcionamento de dados como setpoint, erro, tolerância, ganho integral, etc... //

# Sistema de alarmes

// Fazer uma tabela falando sobre os alarmes e quais são suas condições de funcionamento//

# Supervisório 

// Explicar o que o Supervisório permite visualizar//

# Banco de Dados

// Explicar a tabela do banco de dados//

# Comandos de atuação 

// Citar os comandos de atuação que existem no projeto //

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

20-06-2026 - Reesconstrução da malha fechada e construçã o do sistema de consumo externo (Discord compartilhado, cerca de 3 horas e 50 minutos);