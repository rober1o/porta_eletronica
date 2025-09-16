#include "porta_eletronica.h"

int main()
{
    stdio_init_all();
    inicializar_perifericos();

    sleep_ms(3000);
    while (true)
    {
        printf("Digite a senha: ");
        ler_senha(senha, MAX_SENHA + 1);
        printf("Você digitou: \"%s\"\n", senha);
        verificar_senha(senha);
    }

    return 0;
}

// Função que lê a senha digitada pelo usuário
void ler_senha(char *senha, int tamanho_maximo)
{
    int pos = 0;
    memset(senha, 0, tamanho_maximo);

    while (1)
    {
        int c = getchar();

        // Se não houver caractere, mas já tem algo na senha, continua
        if (c == EOF)
        {
            if (pos == 0)
            {
                // String vazia recebida → considera Enter
                break;
            }
            sleep_ms(1);
            continue;
        }

        // Enter finaliza a senha
        if (c == '\r' || c == '\n')
        {
            break;
        }

        // Backspace ou Delete
        if (c == 8 || c == 127)
        {
            if (pos > 0)
            {
                pos--;
                senha[pos] = '\0';
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
        }
        else
        {
            // Armazena no buffer
            if (pos < tamanho_maximo - 1)
                senha[pos++] = (char)c;
            putchar(c);
        }

        // Atualiza display a cada caractere
        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);
        ssd1306_draw_string(&ssd, "SUA SENHA:", 20, 10);
        ssd1306_draw_string(&ssd, senha, 20, 30);
        ssd1306_send_data(&ssd);
    }

    senha[pos] = '\0';
}

// Função que verifica a senha e acende/apaga o LED
void verificar_senha(const char *senha)
{
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);
    ssd1306_draw_string(&ssd, "SUA SENHA:", 20, 10);

    if (strcmp(senha, SENHA_CORRETA) == 0)
    {
        printf("Senha correta! LED verde aceso.\n");
        gpio_put(LED_GREEN_PIN, true);
        ssd1306_draw_string(&ssd, "CORRETA", 10, 30);
        ssd1306_send_data(&ssd);
        uint slice_num = pwm_gpio_to_slice_num(SERVO_MOTOR_PIN);
        // Abre a porta (90 graus)
        pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_MOTOR_PIN), SERVO_90_DEGREES);
        sleep_ms(3000); // espera 1 segundo

        // Fecha a porta
        pwm_set_chan_level(slice_num, pwm_gpio_to_channel(SERVO_MOTOR_PIN), SERVO_MIN);
        gpio_put(LED_GREEN_PIN, false); // apaga LED amarelo

        // Apaga LED verde e limpa display
        gpio_put(LED_GREEN_PIN, false);
        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);
        ssd1306_draw_string(&ssd, "SUA SENHA:", 20, 10);
        ssd1306_send_data(&ssd);
    }
    else
    {
        printf("Senha incorreta! LED vermelho e buzzer acesos.\n");
        gpio_put(LED_RED_PIN, true);
        ssd1306_draw_string(&ssd, "INCORRETA", 10, 30);
        ssd1306_send_data(&ssd);

        // Ativa buzzer
        uint buzzer_slice = pwm_gpio_to_slice_num(BUZZER_PIN);
        pwm_set_enabled(buzzer_slice, true);

        sleep_ms(1000); // espera 1 segundo

        // Desliga buzzer e LED vermelho
        pwm_set_enabled(buzzer_slice, false);
        gpio_put(LED_RED_PIN, false);

        // Limpa display
        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);
        ssd1306_draw_string(&ssd, "SUA SENHA:", 20, 10);
        ssd1306_send_data(&ssd);
    }
}

void inicializar_perifericos()
{
    // Inicializa a comunicação serial para depuração
    stdio_init_all();
    // Inicializa os periféricos agora que o Wi-Fi foi estabelecido
    inicializar_display_i2c();  // Inicializa o display I2C
    inicializar_leds();         // Configura o LED (se necessário)
    inicializar_pwm_buzzer();   // Inicializa o PWM para o buzzer
    atualizar_display();        // Atualiza o conteúdo do display
    configurar_botao_bootsel(); // Configura o botão BOOTSEL
    incializar_servo_motor();
}

// Inicializar os Pinos GPIO para acionamento dos LEDs da BitDogLab
void inicializar_leds()
{
    // Configuração dos LEDs como saída
    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    gpio_put(LED_BLUE_PIN, false);

    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_put(LED_GREEN_PIN, false);

    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_put(LED_RED_PIN, false);
}

void incializar_servo_motor()
{
    gpio_set_function(SERVO_MOTOR_PIN, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(SERVO_MOTOR_PIN); // Agora está correto!

    pwm_set_wrap(slice_num, 20000);    // 20 ms período (50Hz)
    pwm_set_clkdiv(slice_num, 125.0f); // Clock de 1MHz
    pwm_set_enabled(slice_num, true);
}

void inicializar_display_i2c()
{ // FUNÇÃO PARA INICIALIZAR O I2C DO DISPLAY
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

void inicializar_pwm_buzzer()
{
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    pwm_set_clkdiv(slice_num, 10.0f);                                      // Reduz clock base para 12.5 MHz
    pwm_set_wrap(slice_num, 31250);                                        // 12.5 MHz / 31250 = 400 Hz
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BUZZER_PIN), 15625); // 50% duty cycle
    pwm_set_enabled(slice_num, false);                                     // Começa desligado
}

void configurar_botao_bootsel() // Função para Bootsel no botão B
{
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}

void atualizar_display() // Atualizar as informações do display
{

    ssd1306_fill(&ssd, !cor);                     // limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor); // Moldura
    ssd1306_draw_string(&ssd, "SUA SENHA:", 20, 10);
    ssd1306_send_data(&ssd); // Envia os dados para o display
}

#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}