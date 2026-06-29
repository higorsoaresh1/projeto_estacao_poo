import sqlite3
import pytest
from pathlib import Path

# Mock ou função simulando a criação da sua tabela real do supervisorio.py
def inicializar_banco_teste(conn):
    cursor = conn.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS historico_eta (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT,
            ciclo INTEGER,
            nivel REAL,
            vazao_entrada REAL,
            ph REAL,
            frequencia_bomba REAL,
            alarme_nivel INTEGER
        )
    """)
    conn.commit()

def test_salvamento_e_recuperacao_de_dados_sqlite(tmp_path):
    """Garante que a escrita e leitura no SQLite funcionam sem perda de dados."""
    # Cria um arquivo de banco temporário exclusivo para este teste
    caminho_db = tmp_path / "teste_supervisor.db"
    
    # 1. Conecta e cria as tabelas
    conexao = sqlite3.connect(str(caminho_db))
    inicializar_banco_teste(conexao)
    
    # 2. Insere um registro simulado da telemetria da planta
    cursor = conexao.cursor()
    cursor.execute("""
        INSERT INTO historico_eta (timestamp, ciclo, nivel, vazao_entrada, ph, frequencia_bomba, alarme_nivel)
        VALUES ('2026-06-09 10:15:00', 42, 650.5, 14.2, 7.1, 62.5, 0)
    """)
    conexao.commit()
    
    # 3. Consulta o dado para ver se foi gravado exatamente o que enviamos
    cursor.execute("SELECT ciclo, nivel, ph, alarme_nivel FROM historico_eta ORDER BY id DESC LIMIT 1")
    resultado = cursor.fetchone()
    
    assert resultado is not None
    assert resultado[0] == 42           # Ciclo correto
    assert resultado[1] == 650.5        # Nível correto
    assert resultado[2] == 7.1          # pH correto
    assert resultado[3] == 0            # Alarme inativo correto
    
    conexao.close()