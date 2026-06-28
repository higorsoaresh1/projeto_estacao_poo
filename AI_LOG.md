# AI_LOG

Preencha uma linha por interacao relevante com IA.

| Data | Pedido ao agente | Aceito | Rejeitado | Justificativa tecnica |
|---|---|---|---|---|
| 30/05/2026 | Revisão do modelo de construção das classes | Organização dos atributos nas classes abstratas | Especificações pouco usuais, como um alarme específico de turbidez naquele momento | Melhorar a estrutura do código, reduzir erros de desenvolvimento e aumentar a organização do projeto. |

| 30/05/2026 | Herança de construtores (using Sensor::Sensor) | Utilização da herança de construtores nas classes derivadas | — | Melhorar a reutilização de código e simplificar a implementação das classes derivadas. |

| 06/06/2026 | Construção da malha fechada de controle | Aprimoramento da classe Reservatório e do Sensor de Vazão para suportar o controle em malha fechada | Transformação do sensor de vazão em uma estrutura excessivamente complexa | Tornar o sistema mais realista sem comprometer a simplicidade e a manutenção do projeto. |

| 07/06/2026 | Continuação da implementação da malha fechada | Auxílio na sintaxe e reorganização das classes Reservatório, Bomba e Válvula | Alterações desnecessárias na lógica da vazão e da retroalimentação | Melhorar o desempenho e manter a lógica coerente com o funcionamento esperado da ETA. |

| 08/06/2026 | Construção do JSON e criação dos arquivos.hpp | Auxílio na criação dos arquivos de cabeçalho e do contrato JSON | Separação desnecessária de classes derivadas e estrutura JSON incompatível com a malha fechada | Manter o projeto organizado sem aumentar a complexidade da implementação. |

| 10/06/2026 | Construção do contrato JSONL | Estruturação do arquivo JSON Lines para registro contínuo dos dados | Utilização de formatos de armazenamento mais complexos | Permitir gravação contínua dos ciclos da simulação de forma simples e eficiente. |

| 11/06/2026 | Configuração do SQLite | Configuração das bibliotecas SQLite e integração entre C++ e Python | Configurações adicionais desnecessárias | Garantir o correto funcionamento do banco de dados utilizado pelo sistema. |

| 12/06/2026 | Construção da classe Histórico | Implementação dos comandos SQL e integração da classe com o restante do sistema | Utilização da classe em partes onde não era necessária | Centralizar o acesso ao banco de dados e separar responsabilidades. |

| 12/06/2026 | Construção inicial do supervisório | Indicação das bibliotecas Pandas, Streamlit e Streamlit-Autorefresh, além de auxílio na sintaxe Python | Desenvolvimento prematuro do supervisório e utilização de funções excessivamente complexas | Desenvolver a interface de forma gradual e compatível com o restante do projeto. |

| 16/06/2026 | Configuração do Streamlit | Implementação da interface gráfica e organização da aplicação | Utilização de recursos pouco conhecidos ou sem impacto relevante | Atender aos requisitos do projeto mantendo a simplicidade da implementação. |

| 17/06/2026 | Implementação do botão de alteração do Setpoint | Criação do botão e utilização do arquivo comando.txt para comunicação entre C++ e Python | Aplicação prematura do padrão Command apenas para um comando | Evitar retrabalho e aplicar o padrão apenas quando todos os comandos estivessem definidos. |

| 17/06/2026 | Organização da interface gráfica | Reorganização das colunas, sidebar e melhoria dos gráficos | Criação de componentes específicos para todas as variáveis | Melhorar a experiência do usuário sem adicionar elementos desnecessários. |

| 19/06/2026 | Criação da classe Inversor | Implementação do inversor de frequência para controlar a vazão da bomba | Permanecer utilizando bomba apenas em modo liga/desliga | Tornar o comportamento da planta mais próximo de um sistema industrial real. |

| 21/06/2026 | Alteração da lógica de controle do nível | Criação da variável Demanda para representar o consumo externo | Manter a lógica antiga da vazão de saída | Melhorar a fidelidade da simulação e o comportamento da malha fechada. |

| 21/06/2026 | Implementação do alarme de racionamento | Criação do alarme baseado na variável Demanda | Alterações estruturais na classe base Alarme | Implementar a nova funcionalidade preservando a arquitetura existente. |

| 22/06/2026 | Desenvolvimento do tanque animado no supervisório | Implementação do tanque animado utilizando HTML no Streamlit | Alterações desnecessárias em outros elementos da interface | Tornar a visualização do processo mais intuitiva e interativa. |

| 27/06/2026 | Aplicação dos padrões Command e Factory | Implementação dos padrões Command e Factory para todos os comandos do supervisório | Utilização de vetores dinâmicos nas classes de comando | Melhorar a organização do código e facilitar futuras expansões. |

| 27/06/2026 | Implementação do Timestamp | Utilização das bibliotecas iomanip e sstream para geração do Timestamp | Cálculo realizado apenas na classe Histórico | Disponibilizar o horário em todas as partes do sistema de forma padronizada. |

| 27/06/2026 | Implementação da falha simulada | Simulação de falha de comunicação utilizando o sensor de pH | Aplicação da falha em variáveis pertencentes à malha de controle | Demonstrar o funcionamento dos alarmes sem comprometer o controle do processo. |

| 28/06/2026 | Aplicação do ID da dupla | Utilização do ID_DUPLA para alterar o comportamento de elementos específicos do sistema | Utilização do ID dentro do algoritmo do controlador PI | Atender ao requisito da disciplina sem interferir na estabilidade do controle da planta. |

| 28/06/2026 | Correção da lógica do controlador PI | Implementação da técnica Anti-Windup no controlador | Retorno ao controle liga/desliga da bomba | Evitar saturação da ação integral e melhorar a estabilidade do sistema. |