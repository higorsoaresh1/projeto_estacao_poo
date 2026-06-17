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

# ---------------------------------------------------
# Sensores
# ---------------------------------------------------

sensores = dados["sensores"]
atuadores = dados["atuadores"]
alarmes = dados["alarmes"]

st.header("Dados de Controle")

col1, col2 = st.columns(2)

col1.metric("Setpoint Nível", f"{dados['setpoint']:.1f} m³")
col2.metric("Toleância", f"{dados['tolerancia']:.1f} m³")

novo_setpoint = st.number_input("Novo Setpoint", min_value=0.0, max_value=1000.0, value=float(dados["setpoint"]), step=10.0)

if st.button("DEFINIR SETPOINT"):
    with open("comando.txt", "w") as f:
        f.write(f"SETPOINT={novo_setpoint}")

nova_tolerancia = st.number_input("Nova Tolerância", min_value=1.0, max_value=500.0, value=float(dados["tolerancia"]), step=5.0)

if st.button("DEFINIR TOLERÂNCIA"):
    with open("comando.txt", "w") as f:
        f.write(f"TOLERANCIA={nova_tolerancia}")

st.header("Sensores")

col1, col2, col3, col4, col5 = st.columns(5)

col1.metric("Nível", f"{sensores['nivel']:.1f} m³")
col2.metric("Vazão", f"{sensores['vazao']:.1f} m³/ciclo")
col3.metric("pH", f"{sensores['ph']:.2f}")
col4.metric("Turbidez", f"{sensores['turbidez']:.2f} NTU")
col5.metric("Consumo", f"{dados['consumo']:.1f} m³/ciclo")

# ---------------------------------------------------
# Atuadores
# ---------------------------------------------------

st.header("Atuadores")

col1, col2 = st.columns(2)

if atuadores["bomba"]:
    col1.success("🟢 Bomba ligada")
else:
    col1.error("🔴 Bomba desligada")

if atuadores["valvula_alivio"]:
    col2.warning("🟡 Válvula aberta")
else:
    col2.success("🟢 Válvula fechada")

# ---------------------------------------------------
# Alarmes
# ---------------------------------------------------

st.header("Alarmes")

if alarmes["ph"]:
    st.error("Alarme de pH ativo")
else:
    st.success("pH normal")

if alarmes["nivel"]:
    st.error("Alarme de nível ativo")
else:
    st.success("Nível normal")

if alarmes["vazao"]:
    st.error("Alarme de vazão ativo")
else:
    st.success("Vazão normal")

if alarmes["turbidez"]:
    st.error("Alarme de turbidez ativo")
else:
    st.success("Turbidez normal")

# ---------------------------------------------------
# Comandos START / STOP
# ---------------------------------------------------

st.sidebar.header("Comandos")

if st.sidebar.button("▶ START"):
    with open("comando.txt", "w") as f:
        f.write("START")

if st.sidebar.button("⏹ STOP"):
    with open("comando.txt", "w") as f:
        f.write("STOP")

if st.sidebar.button("❌ EXIT"):
    with open("comando.txt", "w") as f:
        f.write("EXIT")

# ---------------------------------------------------
# Histórico do SQLite
# ---------------------------------------------------

st.header("Tendência do Nível")

try:
    conexao = sqlite3.connect(
        "historico_eta.db",
        check_same_thread=False
    )

    consulta = """
    SELECT ciclo, nivel
    FROM historico
    ORDER BY ciclo DESC
    LIMIT 100
    """

    df = pd.read_sql_query(consulta, conexao)

    conexao.close()

    if not df.empty:
        df = df.sort_values("ciclo")

        st.write("Quantidade de pontos:", len(df))
        st.write("Último ciclo:", df["ciclo"].max())

        st.line_chart(
            df.set_index("ciclo")["nivel"]
    )

except Exception as e:
    st.info(f"Banco ainda não disponível: {e}")