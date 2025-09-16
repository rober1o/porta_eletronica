#ifndef PORTA_ELETRONICA
#define PORTA_ELETRONICA

// ------------------------------
// Bibliotecas padrão C
// ------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
// ------------------------------
// SDK do Raspberry Pi Pico
// ------------------------------
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"

// ------------------------------
// Bibliotecas externas do projeto
// ------------------------------
#include "lib/ssd1306.h"
#include "lib/font.h"

// ------------------------------
// Definições e constantes
// ------------------------------
#define LED_BLUE_PIN 12
#define LED_GREEN_PIN 11
#define LED_RED_PIN 13

#define MAX_SENHA 20
#define SENHA_CORRETA "600" // senha definida no início

// Buzzer
#define BUZZER_PIN 10
#define SERVO_MOTOR_PIN 28
#define SERVO_MIN 1250        // pulso para 0 graus (exemplo: 1ms)
#define SERVO_90_DEGREES 2500 // pulso para 90 graus (exemplo: 1.5ms)

// Botões
#define botaoB 6

// I2C - Display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C


// ------------------------------
// Variáveis globais
// ------------------------------
ssd1306_t ssd; // Instância do display OLED
bool alarme = false;
bool alarme_disparado = false;
uint slice_num;
char senha[MAX_SENHA + 1];
bool cor = true;
// ------------------------------
// Protótipos de funções
// ------------------------------


//Funções de inicialização de periferico
void inicializar_perifericos();
void inicializar_leds(void);
void incializar_servo_motor();
void inicializar_display_i2c();
void inicializar_pwm_buzzer();
void configurar_botao_bootsel();

void ler_senha(char *senha, int tamanho_maximo);
void verificar_senha(const char *senha);
//Buzzer
void tocar_pwm_buzzer(uint duracao_ms);

// Display OLED
void atualizar_display();

// GPIO / Interrupções
void gpio_irq_handler(uint gpio, uint32_t events);


#endif // PORTA_ELETRONICA
