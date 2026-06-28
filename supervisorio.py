import json
import sqlite3
import pandas as pd
import streamlit as st
import streamlit.components.v1 as components
from streamlit_autorefresh import st_autorefresh

import warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
warnings.filterwarnings("ignore", message=".*components.v1.html.*")

# Atualização em tempo real (1 segundo)
st_autorefresh(interval=1000, key="eta_refresh")

st.set_page_config(
    page_title="Supervisório ETA",
    layout="wide"
)

st.title("🏭 Supervisório ETA")

# Funções Auxiliares de Renderização e Componentes

def renderizar_tanque_animado(nivel_atual, capacidade_maxima=1000.0):
    setpoint_valor = float(dados.get("setpoint", 700.0))
    tolerancia_valor = float(dados.get("tolerancia", 80.0))
    
    # Definição dos limites operacionais (baseado nas travas do seu CONTROLADOR.hpp)
    limite_baixo_operacional = 100.0  # Piso mínimo de bloqueio crítico
    limite_alto_transbordo = 950.0    # Teto máximo para abertura da válvula de alívio
    
    # Conversão para porcentagens de altura física (0% a 100%)
    pct_fluido = max(0.0, min(100.0, (nivel_atual / capacidade_maxima) * 100))
    pct_sp = max(0.0, min(100.0, (setpoint_valor / capacidade_maxima) * 100))
    pct_tol_sup = max(0.0, min(100.0, ((setpoint_valor + tolerancia_valor) / capacidade_maxima) * 100))
    pct_tol_inf = max(0.0, min(100.0, ((setpoint_valor - tolerancia_valor) / capacidade_maxima) * 100))
    
    # Nova conversão para as linhas de limites fixos da ETA
    pct_lim_baixo = max(0.0, min(100.0, (limite_baixo_operacional / capacidade_maxima) * 100))
    pct_lim_alto = max(0.0, min(100.0, (limite_alto_transbordo / capacidade_maxima) * 100))

    # Cores dinâmicas do fluido (Regra Dupla 80)
    if nivel_atual <= limite_baixo_operacional: cor_agua = "#ff4b4b"      # Vermelho Crítico
    elif pct_fluido <= 25.0: cor_agua = "#ffa500"                         # Laranja Atenção
    elif nivel_atual >= limite_alto_transbordo: cor_agua = "#004080"      # Azul Escuro (Transbordo)
    else: cor_agua = "#4da6ff"                                            # Azul Normal

    html_conteudo = f"""
    <div style="position: relative; width: 140px; height: 210px; margin: 0 auto; font-family: sans-serif;">
        <div style="position: absolute; top: 10px; left: 0; width: 130px; height: 190px; border: 4px solid #4f5b66; border-radius: 5px 5px 12px 12px; background: #e1e5eb; overflow: hidden; display: flex; align-items: flex-end; z-index: 2; box-shadow: inset 3px 3px 8px rgba(0,0,0,0.15);">
            
            <div style="position: absolute; width: 100%; text-align: center; top: 40%; left: 0; font-size: 14px; font-weight: bold; color: #1c2833; z-index: 5; text-shadow: 1px 1px 2px white;">
                {nivel_atual:.1f} m³<br><span style="font-size:11px; font-weight:normal; color:#444;">({pct_fluido:.0f}%)</span>
            </div>

            <div style="position: absolute; width: 100%; bottom: {pct_sp}%; border-bottom: 2px dashed #1db954; z-index: 4;"><span style="position:absolute; right:1px; background:#1db954; color:white; font-size:7px; padding:0 1px; border-radius:1px; transform:translateY(-50%);">SP</span></div>
            <div style="position: absolute; width: 100%; bottom: {pct_tol_sup}%; border-bottom: 1px dotted #ffa500; z-index: 4;"><span style="position:absolute; right:1px; background:#ffa500; color:white; font-size:7px; padding:0 1px; border-radius:1px; transform:translateY(-50%);">+TOL</span></div>
            <div style="position: absolute; width: 100%; bottom: {pct_tol_inf}%; border-bottom: 1px dotted #ffa500; z-index: 4;"><span style="position:absolute; right:1px; background:#ffa500; color:white; font-size:7px; padding:0 1px; border-radius:1px; transform:translateY(-50%);">-TOL</span></div>

            <div style="position: absolute; width: 100%; bottom: {pct_lim_alto}%; border-bottom: 2px solid #ff1a1a; z-index: 4;"><span style="position:absolute; left:1px; background:#ff1a1a; color:white; font-size:7px; padding:0 2px; border-radius:1px; transform:translateY(-50%); font-weight:bold;">MAX ({limite_alto_transbordo:.0f})</span></div>
            <div style="position: absolute; width: 100%; bottom: {pct_lim_baixo}%; border-bottom: 2px solid #ff1a1a; z-index: 4;"><span style="position:absolute; left:1px; background:#ff1a1a; color:white; font-size:7px; padding:0 2px; border-radius:1px; transform:translateY(-50%); font-weight:bold;">MIN ({limite_baixo_operacional:.0f})</span></div>

            <div style="width: 100%; height: {pct_fluido}%; background: {cor_agua}; transition: height 0.5s ease-out; z-index: 3;"></div>
        </div>
    </div>
    """
    return components.html(html_conteudo, height=210, scrolling=False)

def renderizar_status_atuador(condicao, texto_ativo, texto_inativo):
    """Gera o badge estilizado para os atuadores evitando repetição de HTML."""
    cor, texto = ("#1db954", texto_ativo) if condicao else ("#ff4b4b", texto_inativo)
    html = f'<div style="background:{cor}; color:white; text-align:center; padding:6px; border-radius:6px; font-weight:bold; font-size:14px;">{texto}</div>'
    return st.markdown(html, unsafe_allow_html=True)

# Lógica de Leitura do Banco de Dados / JSONL
def ler_ultimo_registro():
    try:
        with open("dados_eta.jsonl", "r", encoding="utf-8") as arquivo:
            linhas = arquivo.readlines()
            if linhas:
                return json.loads(linhas[-1])
    except Exception:
        return None
    return None

dados = ler_ultimo_registro()

if dados is None:
    st.warning("Aguardando dados da ETA...")
    st.stop()

sensores = dados["sensores"]
atuadores = dados["atuadores"]
alarmes = dados["alarmes"]

# TimeStamp
col1, col2 = st.columns(2)

with col1:
    st.metric("Última Atualização", dados["timestamp"])

with col2:
    st.metric("Ciclo Atual", dados["ciclo"])


# Alertas de Segurança no Topo
if alarmes.get("racionamento", False):
    st.error("🚨 **SISTEMA EM REGIME DE URGÊNCIA:** Racionamento ativo por sobrecarga de demanda!", icon="⚠️")

if alarmes.get("nivel", False):
    st.warning("🚨 **ALERTA DE NÍVEL CRÍTICO:** Verificar o nível do reservatório!", icon="⚠️")

if sensores.get("ph") == -0.8:
    st.error("🚨 **CRÍTICO:** Link de comunicação Modbus com o sensor PH-101 foi interrompido! Sistema operando às cegas.", icon="🔌")
elif alarmes.get("ph", False):
    st.warning("⚠️ **ALERTA DE PROCESSO:** O valor do pH está fora da faixa segura de operação!", icon="🧪")

# Interface Gráfica (Painel de Controle)

# Bloco 1: Monitoramento Principal (Sensores)
with st.container(border=True):
    st.subheader("📊 Monitoramento de Sensores e Processo")
    col_tanque, col_grandezas = st.columns([1, 4])

    with col_tanque:
        renderizar_tanque_animado(sensores['nivel'])

    with col_grandezas:
        c_linha1_1, c_linha1_2, c_linha1_3 = st.columns(3)
        c_linha1_1.metric("Nível", f"{sensores['nivel']:.1f} m³")
        c_linha1_2.metric("Vazão de Entrada", f"{atuadores['vazao_bomba']:.1f} m³/ciclo")
        c_linha1_3.metric("Vazão de Saída", f"{sensores['vazao_saida']:.1f} m³/ciclo")
        
        st.markdown("---")
        
        c_linha2_1, c_linha2_2, c_linha2_3 = st.columns(3)
        c_linha2_1.metric("Turbidez", f"{sensores['turbidez']:.2f} NTU")
        c_linha2_2.metric("pH", f"{sensores['ph']:.2f}")
        
        with c_linha2_3:
            demanda_valor = dados['demanda']
            if demanda_valor >= 20.0:
                st.markdown(
                    f"""
                    <div style="background-color: #ff4b4b; border-radius: 0.5rem; padding: 0.75rem; border-left: 4px solid #b71c1c; font-family: var(--font); color: white; margin-top: -4px; margin-bottom: 0;">
                        <div style="font-size: 14px; opacity: 0.9; margin-bottom: 2px;">Demanda</div>
                        <div style="font-size: 28px; font-weight: 600; line-height: 1.2;">
                            🚨 {demanda_valor:.1f} <span style="font-size: 16px; font-weight: 400;">m³/ciclo</span>
                        </div>
                        <div style="font-size: 11px; font-weight: 700; margin-top: 4px;">SOBRECARGA DETECTADA!</div>
                    </div>
                    """, 
                    unsafe_allow_html=True
                )
            else:
                st.metric("Demanda", f"{demanda_valor:.1f} m³/ciclo")

# Bloco 2: Processo e Malha de Controle PI
st.header("Controle PI")
erro = dados["setpoint"] - sensores["nivel"]
col_pi, col_at = st.columns([1.2, 1])

with col_pi:
    with st.container(border=True):
        st.subheader("Dados PI")
        c1, c2, c3, c4 = st.columns(4)
        c1.metric("Setpoint", f"{dados['setpoint']:.1f} m³")
        c2.metric("Tolerância", f"{dados['tolerancia']:.1f} m³")
        c3.metric("Nível Atual", f"{sensores['nivel']:.1f} m³", delta=f"Erro: {erro:.1f} m³", delta_color="inverse")
        c4.metric("Frequência Inversor", f"{atuadores['frequencia']:.1f} %")

with col_at:
    with st.container(border=True):
        st.subheader("Atuadores")
        c1, c2, c3 = st.columns(3)
        
        with c1:
            st.caption("Bomba")
            renderizar_status_atuador(atuadores["vazao_bomba"] > 0, "🟢 LIGADA", "🔴 DESLIGADA")
                
        with c2:
            st.metric("Válvula Saída", f"{atuadores['abertura_consumo']*100:.0f}%")
            
        with c3:
            st.caption("Válvula Alívio")
            renderizar_status_atuador(atuadores['valvula_alivio'], "🟢 ABERTA", "🔴 FECHADA")

# Bloco 3: Gráfico de Tendência
with st.container(border=True):
    st.subheader("📈 Tendência de Processo")
    try:
        conexao = sqlite3.connect("historico_eta.db", check_same_thread=False)
        df = pd.read_sql_query("SELECT ciclo, nivel, setpoint, tolerancia, vazao_entrada, vazao_saida FROM historico ORDER BY ciclo DESC LIMIT 100", conexao)
        conexao.close()

        if not df.empty:
            df = df.sort_values("ciclo")
            
            c_info1, c_info2 = st.columns(2)
            c_info1.caption(f"**Pontos em tela:** {len(df)}")
            c_info2.caption(f"**Último Ciclo:** {df['ciclo'].max()}")
            
            # --- MEMÓRIA ANTI-RESET DA SELEÇÃO DE TELA ---
            # Se a variável não existir na sessão, define a primeira tela como padrão
            if "tela_ativa" not in st.session_state:
                st.session_state["tela_ativa"] = "Nível do Reservatório"

            opcoes_tela = ["Nível do Reservatório", "Balanço de Vazões"]
            
            # Criamos um seletor em formato de botões horizontais imitando as abas
            tela_selecionada = st.radio(
                "Visualizar Gráfico:", 
                opcoes_tela, 
                index=opcoes_tela.index(st.session_state["tela_ativa"]),
                horizontal=True,
                key="seletor_grafico_fixo"
            )
            # Salvamos a escolha atual na memória do Streamlit
            st.session_state["tela_ativa"] = tela_selecionada
            
            st.markdown("---")
            
            # --- RENDERIZAÇÃO BASEADA NA MEMÓRIA ---
            if tela_selecionada == "Nível do Reservatório":
                import altair as alt
                
                # Pegamos o valor do último setpoint para destacar na escala lateral
                ultimo_sp = float(df["setpoint"].iloc[-1])

                # Configuração da linha do nível com o seu destaque no eixo Y
                grafico_nivel = alt.Chart(df).mark_line(color="#4da6ff", strokeWidth=2.5).encode(
                    x=alt.X("ciclo:Q", title="Ciclo"),
                    y=alt.Y(
                        "nivel:Q", 
                        title="Nível (m³)", 
                        scale=alt.Scale(domain=[0, 1000]),
                        axis=alt.Axis(
                            values=[0, 200, 400, 600, 800, 1000, ultimo_sp], 
                            labelColor=alt.condition(
                                f"datum.value == {ultimo_sp}", 
                                alt.value("#1db954"), 
                                alt.value("#777777")  
                            ),
                            labelFontWeight=alt.condition(
                                f"datum.value == {ultimo_sp}", 
                                alt.value("bold"),    
                                alt.value("normal")
                            )
                        )
                    ),
                    tooltip=["ciclo", "nivel"]
                ).properties(height=250)

                st.altair_chart(grafico_nivel, use_container_width=True)
                
            elif tela_selecionada == "Balanço de Vazões":
                # Plota ambas as vazões juntas para comparação visual
                st.line_chart(df.set_index("ciclo")[["vazao_entrada", "vazao_saida"]], height=250)
                
    except Exception as e:
        st.info(f"Banco ainda não disponível: {e}")

# Bloco 4: Alarmes e Parâmetros Operacionais
col_param, col_alarm = st.columns([1.2, 1])

with col_param:
    with st.container(border=True):
        st.subheader("🔧 Parâmetros de Operação")
        c1, c2 = st.columns(2)
        
        with c1:
            # 1. O operador escolhe o Setpoint dentro da faixa segura
            novo_setpoint = st.number_input(
                "Definir Novo Setpoint (m³)", 
                min_value=150.0, 
                max_value=900.0, 
                value=float(dados["setpoint"]), 
                step=10.0
            )
            if st.button("ATUALIZAR SETPOINT", use_container_width=True):
                with open("comando.txt", "w") as f: f.write(f"SETPOINT={novo_setpoint}")
                    
        with c2:
            # 2. LÓGICA DE TRAVA DA TOLERÂNCIA:
            # Se o Setpoint estiver nos extremos (perto de 150 ou de 900), o teto da tolerância vira 50.
            # Caso contrário, permite até 100 (ou o valor que preferir para o meio do tanque).
            if novo_setpoint <= 150.0 or novo_setpoint >= 900.0:
                tolerancia_maxima = 50.0
            else:
                tolerancia_maxima = 100.0  # Limite padrão seguro para o resto do tanque

            # Se a tolerância atual gravada for maior que o novo teto, ajustamos o padrão para não dar erro
            valor_padrao_tol = min(float(dados["tolerancia"]), tolerancia_maxima)

            nova_tolerancia = st.number_input(
                "Definir Nova Tolerância (m³)", 
                min_value=1.0, 
                max_value=tolerancia_maxima, # O teto agora é dinâmico!
                value=valor_padrao_tol, 
                step=5.0,
                help=f"Para o setpoint atual, a tolerância máxima permitida é {tolerancia_maxima} m³."
            )
            if st.button("ATUALIZAR TOLERÂNCIA", use_container_width=True):
                with open("comando.txt", "w") as f: f.write(f"TOLERANCIA={nova_tolerancia}")

#Bloco 5: Alarmes e Status de Segurança
with col_alarm:
    with st.container(border=True):
        st.subheader("🚨 Central de Alarmes")
        c1, c2 = st.columns(2)
        
        with c1:
            if sensores['ph'] == -0.8:
                st.error("🔌 Link PH-101 Offline (Queda de Conexão)", icon="🚨")
            elif alarmes["ph"]:
                st.error("❌ Falha de pH (Fora da Faixa)", icon="⚠️")
            else:
                st.success("✔️ pH Normal")
            
            if alarmes["nivel"]: st.error("❌ Falha de Nível", icon="⚠️")
            else: st.success("✔️ Nível Normal")
            
        with c2:
            if alarmes["vazao"]: st.error("❌ Falha de Vazão", icon="⚠️")
            else: st.success("✔️ Vazão Normal")
            
            if alarmes["turbidez"]: st.error("❌ Falha de Turbidez", icon="⚠️")
            else: st.success("✔️ Turbidez Normal")

        st.markdown("---")
        if alarmes.get("racionamento", False):
            st.warning("⚠️ **RACIONAMENTO:** Restrição de Vazão Ativa", icon="⚡")
        else:
            st.info("✔️ Distribuição Normal (Sem Racionamento)")

# Menu Lateral (Configurações do Sistema)
st.sidebar.header("Comandos do Sistema")
if st.sidebar.button("▶ INICIAR SISTEMA", use_container_width=True, type="primary"):
    with open("comando.txt", "w") as f: f.write("START")

if st.sidebar.button("⏹ PARAR SISTEMA", use_container_width=True):
    with open("comando.txt", "w") as f: f.write("STOP")

st.sidebar.markdown("---")
if st.sidebar.button("❌ FECHAR DISPOSITIVO", use_container_width=True):
    with open("comando.txt", "w") as f: f.write("EXIT")

st.sidebar.markdown("---")
st.sidebar.subheader("🛠️ Injeção de Falhas (Mesa de Testes)")

# Botão para provocar a falha de conexão
if st.sidebar.button("🚨 SIMULAR QUEDA DE CONEXÃO PH", use_container_width=True, type="secondary"):
    with open("comando.txt", "w") as f: 
        f.write("FALHA_CONEXAO_PH")

# Botão para reparar a falha
if st.sidebar.button("🔧 REPARAR SENSOR PH", use_container_width=True, type="primary"):
    with open("comando.txt", "w") as f: 
        f.write("REPARAR_PH")