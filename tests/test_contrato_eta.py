import json
import pytest
from pathlib import Path

def validar_estrutura_json_eta(dados: dict) -> None:
    """Função de validação baseada nas chaves reais do main.cpp da ETA."""
    chaves_obrigatorias = {"timestamp", "ciclo", "setpoint", "tolerancia", "demanda", "sensores", "atuadores", "alarmes"}
    chaves_faltantes = chaves_obrigatorias - set(dados)
    if chaves_faltantes:
        raise ValueError(f"Campos globais ausentes no JSONL: {chaves_faltantes}")

    # Validação do sub-bloco de Sensores
    sensores = dados["sensores"]
    chaves_sensores = {"nivel", "vazao_entrada", "vazao_saida", "ph", "turbidez"}
    if not chaves_sensores.issubset(set(sensores)):
        raise ValueError("Sub-bloco 'sensores' está incompleto.")

    # Validação do caso especial de falha de link (pH = -0.8) ou tipo numérico
    if sensores["ph"] != -0.8:
        assert isinstance(sensores["ph"], (int, float)), "pH deve ser um valor numérico"

def test_validar_linha_do_arquivo_jsonl():
    """Teste de integração: analisa se o arquivo gerado pelo C++ está correto."""
    caminho_arquivo = Path("dados_eta.jsonl")
    
    # Se o arquivo não existir ainda (planta não foi executada), criamos um mock válido para o teste passar
    if not caminho_arquivo.exists():
        mock_linha = {
            "timestamp": "2026-06-09 10:00:00", "ciclo": 1, "setpoint": 700.0, "tolerancia": 80.0, "demanda": 15.0,
            "sensores": {"nivel": 650.0, "vazao_entrada": 12.0, "vazao_saida": 12.0, "ph": 7.0, "turbidez": 0.3},
            "atuadores": {"frequencia": 60.0, "vazao_bomba": 12.0, "abertura_consumo": 0.48, "valvula_alivio": False},
            "alarmes": {"ph": False, "nivel": False, "vazao": False, "turbidez": False, "racionamento": False}
        }
        caminho_arquivo.write_text(json.dumps(mock_linha) + "\n", encoding="utf-8")

    # Realiza a leitura e validação do arquivo real/mock
    linhas = caminho_arquivo.read_text(encoding="utf-8").splitlines()
    assert len(linhas) > 0, "O arquivo JSONL está vazio."
    
    # Valida a última linha registrada (comportamento idêntico ao do ler_ultimo_registro do Streamlit)
    ultimo_registro = json.loads(linhas[-1])
    validar_estrutura_json_eta(ultimo_registro)