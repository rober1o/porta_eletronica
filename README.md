# Sistema de Porta Eletrônica com Senha

## Descrição
Este projeto implementa um sistema de porta eletrônica controlado por senha, utilizando uma placa Raspberry Pi Pico (RP2040) ou similar e um display OLED SSD1306. A senha é lida via terminal serial e o sistema reage de acordo com a verificação:

* **Senha Correta:** Ativa um LED verde e aciona um servo motor para simular a abertura da porta.
* **Senha Incorreta:** Ativa um LED vermelho e um buzzer para sinalizar o erro.

O display OLED exibe em tempo real a senha que está sendo digitada pelo usuário.

---

## Objetivo
Criar um sistema de segurança simples e funcional com verificação de senha, integrando diferentes periféricos como LEDs, buzzer, display OLED e servo motor em uma única plataforma de hardware. O projeto é um exemplo prático de comunicação serial e controle de periféricos em sistemas embarcados.

---

## Hardware Utilizado

* **Placa:** Raspberry Pi Pico (RP2040) ou compatível
* **Servo Motor:** (Pino: GP28)
* **Display OLED:** SSD1306 (I²C)
* **LEDs:** LED RGB (Pinos: LED_BLUE=GP12, LED_GREEN=GP11, LED_RED=GP13)
* **Buzzer:** (Pino: GP10)
* **Botão:** Botão B (Pino: GP6)
* **Jumpers** e **Protoboard**
* **Fonte:** USB

---

## Conexões

### I²C – Display OLED SSD1306
| OLED | Raspberry Pi Pico (RP2040) |
|------|----------------------------|
| SDA | GP14 |
| SCL | GP15 |
| VCC | 3V3 |
| GND | GND |

### GPIO – LEDs e Buzzer
| Componente | Raspberry Pi Pico (RP2040) |
|------------|----------------------------|
| LED Verde | GP11 |
| LED Vermelho | GP13 |
| LED Azul | GP12 |
| Buzzer | GP10 |

### PWM – Servo Motor
| Componente | Raspberry Pi Pico (RP2040) |
|------------|----------------------------|
| Servo | GP28 |

---

## Configuração do Ambiente
1.  Instalar o **Pico SDK** e configurar o ambiente de desenvolvimento.
2.  Clonar o repositório do projeto:
    ```bash
    git clone [https://github.com/rober1o/porta_eletronica.git](https://github.com/rober1o/porta_eletronica.git)
    ```
3.  Conectar a placa Raspberry Pi Pico ao PC via USB, mantendo o botão **BOOTSEL** pressionado para entrar no modo de bootloader.
4.  Compilar e carregar o firmware. Dentro da pasta `porta_eletronica`, execute:
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```
5.  Copiar o arquivo `.uf2` gerado na pasta `build` para o dispositivo USB da Raspberry Pi Pico.

---

## Execução
O sistema é controlado através de um terminal serial. Recomenda-se o uso de programas como o **PuTTY** para a comunicação, pois ele processa corretamente os caracteres de "Enter" (`\r\n`), o que pode ser um problema com o terminal padrão do VS Code.

1.  Após carregar o firmware, abra a porta serial correspondente à sua placa.
2.  No terminal, será solicitada a senha. Digite a senha desejada e pressione `Enter`. A senha padrão é `"600"`.
3.  O display OLED mostrará os caracteres digitados em tempo real.
4.  O sistema verificará a senha e acionará os componentes de acordo com o resultado.

---

## Autores
* **Desenvolvimento:** Roberto Cardoso, Heitor Lemos e Luis Filipe.
