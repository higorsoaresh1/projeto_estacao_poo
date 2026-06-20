import streamlit as st
import json
import sqlite3
import pandas as pd
from streamlit_autorefresh import st_autorefresh

# Atualiza automaticamente a cada 1 segundo
st_autorefresh(interval=1000, key="eta_refresh")

st.set_page_config(
    page_title="Supervisório ETA",
    layout="wide"
)

st.title("🏭 Supervisório ETA")

# ---------------------------------------------------
# Função para ler o último registro do JSONL
# ---------------------------------------------------

def ler_ultimo_registro():
    try:
        with open("dados_eta.jsonl", "r", encoding="utf-8") as arquivo:
            linhas = arquivo.readlines()

            if linhas:
                return json.loads(linhas[-1])

    except:
        return None

    return None


dados = ler_ultimo_registro()

# ---------------------------------------------------
# Caso ainda não exista dado
# ---------------------------------------------------

if dados is None:
    st.warning("Aguardando dados da ETA...")
    st.stop()

sensores = dados["sensores"]
atuadores = dados["atuadores"]
alarmes = dados["alarmes"]

# Bloco 1: Monitoramento Principal (Sensores)
with st.container(border=True):
    st.subheader("📊 Monitoramento de Sensores")

col1, col2, col3, col4, col5 = st.columns(5)

col1.metric("Nível", f"{sensores['nivel']:.1f} m³")
col2.metric("Vazão de Entrada", f"{atuadores['vazao_bomba']:.1f} m³/ciclo")
col3.metric("Consumo", f"{dados['consumo']:.1f} m³/ciclo")
col4.metric("pH", f"{sensores['ph']:.2f}")
col5.metric("Turbidez", f"{sensores['turbidez']:.2f} NTU")

st.header("Controle PI")

# Bloco 2: Processo e Malha de Controle PI
erro = dados["setpoint"] - sensores["nivel"]

# Usando colunas assimétricas para balancear os Atuadores ao lado do PI
col_pi, col_at = st.columns([1.2, 1])

with col_pi:
    with st.container(border=True):
        st.subheader("Dados PI")
        c1, c2, c3 = st.columns(3)
        c1.metric("Setpoint", f"{dados['setpoint']:.1f} m³")
        # O delta exibe o desvio de forma visual (positivo/negativo)
        c2.metric("Nível Atual", f"{sensores['nivel']:.1f} m³", delta=f"Erro: {erro:.1f} m³", delta_color="inverse")
        c3.metric("Frequência Inversor", f"{atuadores['frequencia']:.1f} %")

with col_at:
    with st.container(border=True):
        st.subheader("Atuadores")
        c1, c2, c3 = st.columns(3)
        
        # Uso de markdown simples para simular tags de status limpas
        with c1:
            st.caption("Bomba")
            if atuadores["vazao_bomba"] > 0:
                st.markdown("🟢 **LIGADA**")
            else:
                st.markdown("🔴 **DESLIGADA**")
                
        with c2:
            st.metric("Válvula Saída", f"{atuadores['abertura_consumo']*100:.0f}%")
            
        with c3:
            st.caption("Válvula Alívio")
            if atuadores['valvula_alivio']:
                st.markdown("🟢 **ABERTA**")
            else:
                st.markdown("🔴 **FECHADA**")

# Bloco 3: Gráfico de Tendência
with st.container(border=True):
    st.subheader("📈 Tendência do Nível")
    try:
        conexao = sqlite3.connect("historico_eta.db", check_same_thread=False)
        consulta = "SELECT ciclo, nivel FROM historico ORDER BY ciclo DESC LIMIT 100"
        df = pd.read_sql_query(consulta, conexao)
        conexao.close()

        if not df.empty:
            df = df.sort_values("ciclo")
            
            # Sub-métricas discretas para o gráfico
            c_info1, c_info2 = st.columns(2)
            c_info1.caption(f"**Pontos em tela:** {len(df)}")
            c_info2.caption(f"**Último Ciclo:** {df['ciclo'].max()}")
            
            st.line_chart(df.set_index("ciclo")["nivel"], height=250)
    except Exception as e:
        st.info(f"Banco ainda não disponível: {e}")

#Dados de controle
st.header("Dados de Controle")

col1, col2 = st.columns(2)

col1.metric("Setpoint Nível", f"{dados['setpoint']:.1f} m³")
col2.metric("Toleância", f"{dados['tolerancia']:.1f} m³")

# Bloco 4: Alarmes e Parâmetros Operacionais
col_param, col_alarm = st.columns([1.2, 1])

with col_param:
    with st.container(border=True):
        st.subheader("🔧 Parâmetros de Operação")
        
        c1, c2 = st.columns(2)
        with c1:
            novo_setpoint = st.number_input("Definir Novo Setpoint (m³)", min_value=0.0, max_value=1000.0, value=float(dados["setpoint"]), step=10.0)
            if st.button("ATUALIZAR SETPOINT", use_container_width=True):
                with open("comando.txt", "w") as f:
                    f.write(f"SETPOINT={novo_setpoint}")
                    
        with c2:
            nova_tolerancia = st.number_input("Definir Nova Tolerância (m³)", min_value=1.0, max_value=500.0, value=float(dados["tolerancia"]), step=5.0)
            if st.button("ATUALIZAR TOLERÂNCIA", use_container_width=True):
                with open("comando.txt", "w") as f:
                    f.write(f"TOLERANCIA={nova_tolerancia}")

with col_alarm:
    with st.container(border=True):
        st.subheader("🚨 Central de Alarmes")
        
        # Layout compacto usando os expansores de status nativos do Streamlit
        c1, c2 = st.columns(2)
        with c1:
            if alarmes["ph"]: st.error("❌ Falha de pH", icon="⚠️")
            else: st.success("✔️ pH Normal")
            
            if alarmes["nivel"]: st.error("❌ Falha de Nível", icon="⚠️")
            else: st.success("✔️ Nível Normal")
            
        with c2:
            if alarmes["vazao"]: st.error("❌ Falha de Vazão", icon="⚠️")
            else: st.success("✔️ Vazão Normal")
            
            if alarmes["turbidez"]: st.error("❌ Falha de Turbidez", icon="⚠️")
            else: st.success("✔️ Turbidez Normal")

# Menu Lateral (Configurações do Sistema)
st.sidebar.header("Comandos do Sistema")
if st.sidebar.button("▶ START SYSTEM", use_container_width=True, type="primary"):
    with open("comando.txt", "w") as f: f.write("START")

if st.sidebar.button("⏹ STOP SYSTEM", use_container_width=True):
    with open("comando.txt", "w") as f: f.write("STOP")

st.sidebar.markdown("---")
if st.sidebar.button("❌ EXIT APPLICATION", use_container_width=True):
    with open("comando.txt", "w") as f: f.write("EXIT")
    