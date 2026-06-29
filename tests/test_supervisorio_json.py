import json
import pytest

def gerar_payload_comando(comando, valor=None):
    """Simula a função do seu supervisório que gera a string JSON para o C++."""
    payload = {
        "status": "requisicao",
        "action": comando
    }
    if valor is not None:
        payload["value"] = valor
    return json.dumps(payload)

def test_estrutura_do_json_de_comando():
    """Garante que o formato de comunicação JSON não quebre."""
    # Testa comando de Setpoint
    json_setpoint = gerar_payload_comando("SETPOINT", 550)
    dados = json.loads(json_setpoint)
    
    assert "action" in dados
    assert dados["action"] == "SETPOINT"
    assert dados["value"] == 550

    # Testa comando de START
    json_start = gerar_payload_comando("START")
    dados_start = json.loads(json_start)
    
    assert dados_start["action"] == "START"
    assert "value" not in dados_start # START não precisa de valor externo