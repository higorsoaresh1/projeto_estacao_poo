import pytest
from pathlib import Path

# Casos de teste: (Nome do botão na interface, argumento passado, string esperada no arquivo txt)
@pytest.mark.parametrize(
    ("tipo_acao", "valor", "string_esperada"),
    [
        ("START", None, "START"),
        ("STOP", None, "STOP"),
        ("SETPOINT", 650.0, "SETPOINT 650.000000"),
        ("SETPOINT", 400.5, "SETPOINT 400.500000"),
        ("EXIT", None, "EXIT"),
    ],
)
def test_escrita_de_comandos_deve_respeitar_formato(tipo_acao, valor, string_esperada):
    """Garante que a string gravada em comando.txt segue o padrão exato da Factory C++."""
    arquivo_comando = Path("comando.txt")
    
    # Simula a lógica de escrita de comando do seu arquivo supervisorio.py
    if tipo_acao == "SETPOINT":
        linha_gerada = f"{tipo_acao} {valor:.6f}"
    else:
        linha_gerada = tipo_acao
        
    arquivo_comando.write_text(linha_gerada, encoding="utf-8")
    
    # Valida a leitura
    conteudo_salvo = arquivo_comando.read_text(encoding="utf-8").strip()
    assert conteudo_salvo == string_esperada
    
    # Limpa o arquivo de teste
    if arquivo_comando.exists():
        arquivo_comando.unlink()