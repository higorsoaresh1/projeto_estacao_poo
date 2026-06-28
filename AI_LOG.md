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

-13/06/2026: Ajuda na configuração e aplicação do streamlit
->aceito:
->rejeitado:
->justificativa:


