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