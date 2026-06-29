import pytest

# -----------------------------------------------------------------------------
# 1. Funções de lógica que costumam existir dentro de um supervisório.py
# (Se as suas funções tiverem nomes diferentes, ajuste abaixo)
# -----------------------------------------------------------------------------
def calcular_percentual_tanque(volume_atual, capacidade_maxima):
    """Calcula a porcentagem de preenchimento do reservatório para a barra de progresso."""
    if capacidade_maxima <= 0:
        return 0.0
    percentual = (volume_atual / capacidade_maxima) * 100
    return min(max(percentual, 0.0), 100.0) # Garante limite entre 0% e 100%

def classificar_status_ph(ph):
    """Define a cor ou status do alerta de pH na tela do Streamlit."""
    if ph < 6.5:
        return "Ácido - Crítico"
    elif ph > 8.5:
        return "Alcalino - Crítico"
    else:
        return "Normal"

# -----------------------------------------------------------------------------
# 2. Os Testes Unitários com Pytest
# -----------------------------------------------------------------------------
def test_calculo_percentual_reservatorio():
    # Caso padrão: metade do tanque
    assert calcular_percentual_tanque(500, 1000) == 50.0
    
    # Casos de limites (travas de segurança da interface)
    assert calcular_percentual_tanque(-50, 1000) == 0.0  # Não deve ser negativo
    assert calcular_percentual_tanque(1200, 1000) == 100.0 # Não deve passar de 100%

def test_classificacao_alertas_ph():
    assert classificar_status_ph(7.0) == "Normal"
    assert classificar_status_ph(5.2) == "Ácido - Crítico"
    assert classificar_status_ph(9.0) == "Alcalino - Crítico"