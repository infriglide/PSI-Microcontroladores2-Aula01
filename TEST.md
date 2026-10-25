
# Plano de testes - Atividade Bare-metal (FRDM-KL25Z)

## Objetivo
Verificar que um único laço principal controla dois LEDs com períodos independentes:

| LED  | Período esperado | Comportamento |
|------|------------------|---------------|
| LED0 | 1000 ms (1 Hz)   | alterna (toggle) a cada 1000 ms |
| LED1 | 500 ms (2 Hz)    | alterna (toggle) a cada 500 ms  |

> Como o código usa `gpio_pin_toggle_dt`, os períodos acima são o intervalo
> entre trocas de estado (aceso/apagado), não o ciclo completo de piscada.

## Pré-requisitos
- Placa FRDM-KL25Z ligada por USB, Zephyr e PlatformIO instalados.
- Branch correta do repositório clonada.
- Cronômetro (celular) ou, melhor, vídeo em câmera lenta.

## Casos de teste

| ID | Teste | Procedimento | Resultado esperado |
|----|-------|--------------|--------------------|
| T1 | Compilação | `pio run` | Termina com `SUCCESS`, sem erros nem warnings sobre `led0`/`led1`. |
| T2 | Gravação | `pio run -t upload` | Upload concluído; a placa reinicia sozinha. |
| T3 | Dois LEDs distintos | Observar a placa por 10 s | Dois LEDs diferentes (cores distintas do LED RGB) mudam de estado. Nenhum fica parado. |
| T4 | Período do LED0 | Contar 30 trocas de estado do LED0 | ~30 s (tolerância ±1 s). |
| T5 | Período do LED1 | Contar 60 trocas de estado do LED1 | ~30 s (tolerância ±1 s). |
| T6 | Independência | Observar os dois LEDs por 60 s | Um LED não atrasa nem trava o outro. As trocas coincidem a cada 1 s. |
| T7 | Sem deriva | Deixar rodando 10 min e repetir T4/T5 | Mesmos períodos do início (o código usa `next += PERIOD`, sem acúmulo de atraso). |
| T8 | Mensagem serial | Abrir o monitor serial (`pio device monitor`) após o reset | Aparece a linha com os pinos e períodos dos LEDs. |
| T9 | Reset | Apertar o botão de reset | Os dois LEDs voltam a funcionar nos mesmos períodos. |

## Como registrar a validação
Quem avalia deve anotar: placa e versão do Zephyr, quais testes (T1-T9)
executou, o método de medição e os valores medidos, mesmo quando tudo passar.

## Observações
- Confira se o `board` do `platformio.ini` é `frdm_kl25z`.
- Se T1 falhar por `led1` não definido, a placa não tem o alias `led1`.