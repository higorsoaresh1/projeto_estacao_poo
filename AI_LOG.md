# AI_LOG

Preencha uma linha por interacao relevante com IA.

| Data | Pedido ao agente | Aceito | Rejeitado | Justificativa tecnica |
|---|---|---|---|---|
|  |  |  |  |  |

- 30/05/2026: Revisão de modelo de construção das classes.
->aceito: Atributos reservado às classes abstratas;
->rejeitado: Especificações pouco usuais: ex: alarme de turbidez;
->justificativa: Melhorar a estrutura do código e evitar erros no desenvolvimento do projeto e aprimorar a eficiência.

- 30/05/2026: Herança de construtores. ex: using Sensor::Sensor;
->aceito: Estrutura de herança de construtores;
->justificativa: Melhorar a fluidez do código.

- 06/06/2026: Auxilio na construção de um sistema de malha fechada
->aceito: A melhor utilização da classe de reservatório e do sensor de vazão, a partir da criação de atributos e métodos mais úteis para um sistema de malha fechada que seja funcional;
->rejeitado: Transformação da classe de sensor de vazão em uma estrutura mais complexa;
->justificativa: Aprimorar o funcionamento do código, buscar maior fidelidade a um sistema real e evitar sistemas complexos que sejam desconexos com a integridade do projeto.

- 07/06/2026: Continuação da construção do sistema de malha fechada referente ao nível do reservatório
->aceito: Auxilio na sintaxe do código e possíveis incrementos na organização da classes reservátorio, bomba e valvula;
->rejeitado: Modificações desnecessárias em classes de vazão e na retroalimentação da malha de nível;
->justificativa: Buscar o maior desempenho do código, juntamente com um funcionamento verosímil à realidade, mas evitando implementações não essenciais.

- 08/ 06/2026:: Construção do JSON e criação de arquivos .hpp
->aceito: Auxilio na criação dos arquivos .hpp e na construção do JSON dentro do arquivo main.cpp;
->rejeitado: Separação de classes derivadas(como por exemplo sensor_ph) em outros arquivos de cabeçalho e construção do JSON por meio não funcional para a utilização da malha fechada dentro do projeto;
->justificativa: Evitar complexidade sem motivo dentro do projeto, além de melhor adaptar o contrato JSON dentro da funcionalidade pensada para o sistema.

-10/ 06/2026: Finalização da construçaõ do contrato JSON, utilizando JSONL
->aceito: Auxilio na construção do JSONL;
->rejeitado: Tentar aplicar outros tipos de contrato mais complexos e menos funcionais para o caso;
->justificativa: Ajuda na construção do contrato, evitando erros sintáticos e melhorando na otimização do código dentro da main;

-11/06/2026: Configuração e aplicação do SQL dentro do sistema
->aceito: Ajuda para configurar o SQLITE dentro do sistema operacional e suas funções dentro do código c++ e python;
->rejeitado: Configurações desnecessárias dentro do SQLITE;
->justificativa: Boa configuração do sistema necessário para a criação do banco de dados em SQLITE para a classe histórico;

-12/06/2026: Auxilio na construção da classe histórico
->aceito: Amparo nos comandos SQL dentro da classe histórico e aplicação dessa entre c++ e python;
->rejeitado: Aplicar classe históricos em momentos não necessários
->justificativa: Possibilitar o bom funcionamento do banco de dados e criando lógica de funcionamento entre as partes do programa;

-12/06/2026: Apoio no inicio da construção do supervisório
->aceito: Endicação de bibliotecas como PANDA e STREAMLIT_AUTOREFRESH e auxilio com a sintaxe de python;
->rejeitado: Criação do supervisório antes da construção de todo o código e utilização de funções muito complexas;
->justificativa:

-16/06/2026: Ajuda na configuração e aplicação do streamlit
->aceito: Auxilio na implementação de interfaces gráficas dentro do supervisório ;
->rejeitado: Utilizar funções desconhecidas de python ou que não tivessem grande impacto dentro do projeto;
->justificativa: Atender as demandas solicitadas pelo projeto, evitando utilizar funções desnecessárias;

-17/06/2026: Implementação do botão de mudar setpoint dentro do supervisório
->aceito: Ajuda na criaçao do botão dentro do supervisório e na criação do arquivo comando.txt para criar a comunicação entre o supervisório e o sistema c++;
->rejeitado: Aplicar um padrão de projeto command apenas para esse botão, visto que, seria melhor aplicar esse padrão com todos os comandos prontos;
->justificativa: Evitar quebrar linha de raciocínio lógico com alterações mais complexas, por enquanto;

-17/06/2026: Alteração da interface gráfica do supervisório streamlit
->aceito: Alterar a posição das colunas, organizar a sidebar e melhorar a construção do gráfico dentro do supervisório;
->rejeitado: Criar espaços personalizados para todas as variáveis do projeto;
->justificativa: Melhorar a interface e funcionamento do supervisório sem gastar muito tempo com aplicações desnecessárias;

-19/06/2026: Criação da classe inversor.hpp
->aceito: Criar a classe inversor.hpp para alterar a lógica de funcionamento da bomba, assim controlando melhor a vazão de entrada;
->rejeitado: Continuar com a lógica de off/on na bomba;
->justificativa: Melhorar o funcionamento da bomba e da variável de vazão de entrada dentro da malha fechada, tornando o sistema mais realista;

-21/06/2026: Alteração do funcionamento do controle de nível
->aceito: Criar variável de demanda para auxiliar no controle do nível do reservatório, permitindo melhor relação entre vazão de entrada e vazão de saída;
->rejeitado: Deixar o controle da vazão de saída como estava;
->justificativa: Melhorar o funcionamento da malha fechada, tornando o sistema mais realista e evitando erros dentro da simulação;

-21/06/2026: Criação da classe Alarme_racionamento
->aceito: Auxilio na implementação do alarme de racionamento em relação a variável de demanda;
->rejeitado: Mundanças de sintaxe dentro na classe base alarme;
->justificativa: Apenas implementar o alarme de racionamento sem grande impacto dentro do código;

-22/06/2026: 
->aceito: Auxilio na criação de um tanque animado dentro do supervisório streamlit;
->rejeitado: Alterações dentro das outras variáveis na interface sem motivos plausíveis;
->justificativa: Melhorar a interface do programa, tornando o sistema mais interativo;

-27/06/2026: Aplicação do padrão de projeto Command para os comandos de atuação do sistema
->aceito: Auxilio na implementação do padrão Command para os comandos de start, stop, mudar setpoint e tolerância, 
além de aplicar o padrão de projeto factory ao mesmo tempo;
->rejeitado: Utilizar vetores dinâmicos dentro das classes command;
->justificativa: Aplicar padrões de projeto para melhorar a estrutura do código e facilitar futuras alterações deste. Além disso, também evitou-se utilizar 
lógicas mais complexas que não teriam eficiência para o atual porte do projeto;

-27/06/2026: Criação e aplicação do timestamp 
->aceito: Utilizar as bibliotecas iomanip e sstream, criação da função de calculo do timestamp e aplicação da variavel time dentro do JSON e no supervisório;
->rejeitado: Realizar o calculo do timestamp apenas dentro da classe histórico;
->justificativa: Criar e aplicar o timestamp dentro do projeto, facilitando a leitura das atualizações dos dados em tempo real e evitando 
criar situações mais complexas sem justificativas tão necessárias;

-27/06/2026: Auxilio na criação da falha simulada
->aceito: Criar a falha simulada utilizando o PH como referência;
->rejeitado: Utilizar alguma variavel pertecente a malha fechada como referêncial da falha simulada;
->justificativa: Criar uma falha simulada simples que demonstre o funcionamento do sistema e que não impacte o restante do sistema de forma tão abrupta;

-28/06/2026: Ideias para a aplicação do ID_DUPLA
->aceito: Utilizar o ID como constante para a alteração do comportamento de diversos campos do código;
->rejeitado: Utilizar o ID dentro do calculo da malha fechada;
->justificativa: Alterar o funcionamento do sistema utilizando o ID próprio da dupla, sem criar nenhum problema no funcionamento do projeto;

-28/06/2026: Reparação de erro dentro da lógica do cálculo de nível 
->aceito: Utilizar a lógica anti-windup dentro do controlador;
->rejeitado: Alterar a lógica da bomba para um sistema off/on;
->justificativa: Evitar erros na lógica do sistema de nível e deixar o projeto mais realista;















