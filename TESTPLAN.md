# Plano de testes - Atividade Threads (FRDM-KL25Z)

## Objetivo
Verificar que duas threads Zephyr controlam dois LEDs de forma independente:

| LED  | Período esperado | Tempo aceso    |
|------|------------------|----------------|
| LED0 | 1000 ms (1 Hz)   | 100 ms (`FLASH_MS`) |
| LED1 | 350 ms (~2,86 Hz)| 100 ms (`FLASH_MS`) |

> Versão básica da atividade: `LED1_PERIOD_MS = 500`. Os testes T4/T5 valem para
> ambas as versões; basta trocar o valor esperado.

## Pré-requisitos
- Placa FRDM-KL25Z ligada por USB, Zephyr 4.2 e PlatformIO instalados.
- Branch `threads` do repositório clonada.
- Um cronômetro (celular) ou, melhor, vídeo em câmera lenta / analisador lógico.

## Casos de teste

| ID | Teste | Procedimento | Resultado esperado |
|----|-------|--------------|--------------------|
| T1 | Compilação | `pio run` | Termina com `SUCCESS`, sem erros. Sem warning sobre `led0`/`led1` indefinidos. |
| T2 | Gravação | `pio run -t upload` | Upload concluído; a placa reinicia sozinha. |
| T3 | Dois LEDs distintos acendem | Observar a placa por 10 s após o reset | Dois LEDs diferentes (cores diferentes do LED RGB) piscam. Nenhum fica apagado nem aceso fixo. |
| T4 | Período do LED0 | Contar 30 piscadas do LED0 com cronômetro (ou medir em vídeo) | 30 piscadas em ~30 s (tolerância ±1 s com cronômetro; ±10 ms/piscada em vídeo). |
| T5 | Período do LED1 | Contar 60 piscadas do LED1 | 60 piscadas em ~21 s (350 ms cada; com 500 ms seriam 30 s). Tolerância ±1 s. |
| T6 | Independência | Observar os dois LEDs juntos por 60 s | Um LED nunca "trava" ou atrasa o outro. As piscadas coincidem a cada 7 s (MMC de 1000 e 350 ms). |
| T7 | Sem deriva | Deixar rodando 10 min e repetir T4/T5 no final | Mesmos períodos do início (a espera usa `K_TIMEOUT_ABS_MS`, sem acúmulo de atraso). |
| T8 | Duty cycle | Medir o tempo aceso em vídeo | ~100 ms por piscada nos dois LEDs. |
| T9 | Reset | Apertar o botão de reset | Os dois LEDs voltam a piscar nos mesmos períodos. |
| T10 | Falha de configuração | Trocar `FLASH_MS` por 1000 e compilar | Build **falha** com a mensagem do `BUILD_ASSERT` (comprova a proteção). |

## Como registrar a validação
Quem avalia a PR deve comentar: placa e versão do Zephyr usadas, quais testes
(T1-T10) executou, o método de medição (cronômetro, vídeo, analisador lógico) e os
valores medidos, mesmo quando tudo passar.

## Observações
- Verifique se o `board` do `platformio.ini` corresponde à placa (`frdm_kl25z`).
- Se T1 falhar por `led1` não definido, a placa/overlay não tem o alias `led1`.
