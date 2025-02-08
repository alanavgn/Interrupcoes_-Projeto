# Controle de Matriz de LEDs WS2812 com Raspberry Pi Pico  

## Descrição  

Este projeto usa um *Raspberry Pi Pico* para exibir números de *0 a 9* em uma matriz *5x5 de LEDs WS2812. Dois botões permitem incrementar ou decrementar o número exibido. Além disso, um LED vermelho pisca a cada **200 ms* para indicar que o sistema está ativo.  

## Componentes  

- *Raspberry Pi Pico W*  
- *Matriz de LEDs WS2812 (5x5)*  
- *LEDs individuais (vermelho, verde e azul)*  
- *Botões de entrada (A e B)*  

## Funcionalidades  

- *Exibição de números na matriz de LEDs* (0 a 9)  
- *Botão A*: Aumenta o número exibido  
- *Botão B*: Diminui o número exibido  
- *LED vermelho piscante* (indica funcionamento)  
- *Mecanismo de debounce* para evitar leituras erradas dos botões  

## Ligações  

| Componente      | Pino do Pico |
|----------------|-------------|
| Matriz WS2812  | GP7         |
| LED Vermelho   | GP13        |
| LED Verde      | GP11        |
| LED Azul       | GP12        |
| Botão A        | GP5         |
| Botão B        | GP6         |

## Como Usar  

1. *Compilar e carregar o código* no Raspberry Pi Pico.  
2. *Pressionar os botões* para mudar o número na matriz.  
3. *Observar o LED vermelho piscando* para indicar funcionamento.

Video Explicativo: https://youtube.com/shorts/BqFW36AFgHE?feature=share
