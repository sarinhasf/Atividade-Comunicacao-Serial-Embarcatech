#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pio_matrix.pio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"


#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define MATRIZ_PIN 7 //pino de saída para comunicação com leds
#define BTNA_PIN 5 //botão a
#define BTNB_PIN 6 // botão b
#define DEBOUNCE_DELAY 200000 //  Definição do tempo de debounce (ajustado para 50ms)
#define LED_GREEN 11
#define LED_BLUE 12


ssd1306_t ssd; // Inicializa a estrutura do display
PIO pio = pio0;
uint offset, sm;
char caracter;
static volatile uint32_t last_time = 0; // Armazena o tempo da última interrupção do botão
bool ledBlue = false;
bool ledGreen = false;

// Matriz representando os numeros em leds 5x5
double numeros[11][25] = {
  { // desenho do numero 0
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0
  },
  { // desenho do numero 1
      0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 1.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0
  },
  { // desenho do numero 2
      0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0
  },
  { // desenho do numero 3
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0
  },
  { // desenho do numero 4
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 1.0, 0.0
  },
  { // desenho do numero 5
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0
  },
  { // desenho do numero 6
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0
  },
  { // desenho do numero 7
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0, 0.0
  },
  { // desenho do numero 8
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0
  },
  { // desenho do numero 9
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 1.0, 0.0,
      0.0, 1.0, 1.0, 1.0, 0.0,
      0.0, 1.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 1.0, 0.0
  },
  { // nada
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
  }
};

//Declarção de Funções
void config_I2C();
uint32_t matrix_rgb(double b, double r, double g);
void init_pio_matriz();
void print_valor(int valor, PIO pio, uint sm);
void init_componentes();
void button_Event(uint gpio, uint32_t events);

//O fluxo do display é simples, fazemos as alterações necessários e depois chamamos 
//a função ssd1306_send_data para mandar tudo pro dispaly.
int main()
{
  config_I2C();
  init_pio_matriz();
  init_componentes();

  // Configuração dos botões como interrupções
  gpio_set_irq_enabled_with_callback(BTNA_PIN, GPIO_IRQ_EDGE_FALL, true, &button_Event);
  gpio_set_irq_enabled_with_callback(BTNB_PIN, GPIO_IRQ_EDGE_FALL, true, &button_Event);

  print_valor(10, pio, sm); //inicializando com a matriz vazia, 10 = nada
  while (true)
  { 
    scanf("%c", &caracter);
    
    printf("O caracter digitado foi: %c\n", caracter);

    if ((caracter >= 'a' && caracter <= 'z') || (caracter >= 'A' && caracter <= 'Z') || (caracter >= '0' && caracter <= '9')) {
      
      if(caracter >= '0' && caracter <= '9'){
        int num = caracter - '0';
        print_valor(num, pio, sm);
      }

      // Limpa o display
      ssd1306_fill(&ssd, false);

      // Desenha o caractere na posição desejada
      ssd1306_draw_string(&ssd, &caracter, 40, 30); 
      ssd1306_send_data(&ssd);

    } else {
      printf("Caracter Inválido!\n");
    }
  
    sleep_ms(1000);
  }
}


// Função de interrupção para os botões
void button_Event(uint gpio, uint32_t events){

  uint32_t current_time = to_us_since_boot(get_absolute_time()); //captura tempo atual
      if (current_time - last_time > DEBOUNCE_DELAY){
          last_time = current_time; // atualiza o tempo da ultima interrupção válida
          
          ssd1306_fill(&ssd, false);

          if(gpio == BTNA_PIN){
            ledGreen = !ledGreen;
            if(ledGreen){
              ssd1306_draw_string(&ssd, "LED GREEN: ON", 10, 30); 
              ssd1306_send_data(&ssd);
              printf("[LED VERDE LIGADO]\n");
            }else{
              ssd1306_draw_string(&ssd, "LED GREEN: OFF", 10, 30); 
              ssd1306_send_data(&ssd);
              printf("[LED VERDE DESLIGADO]\n");
            }
            gpio_put(LED_GREEN, ledGreen);

          } else if(gpio == BTNB_PIN){
            ledBlue = !ledBlue;
            if(ledBlue){
              ssd1306_draw_string(&ssd, "LED BLUE: ON", 10, 30); 
              ssd1306_send_data(&ssd);
              printf("[LED AZUL LIGADO]\n");
            }else{
              ssd1306_draw_string(&ssd, "LED BLUE: OFF", 10, 30); 
              ssd1306_send_data(&ssd);
              printf("[LED AZUL DESLIGADO]\n");
            }
            gpio_put(LED_BLUE, ledBlue);
            
        }

  }
  else{
     return; // Ignora se estiver dentro do tempo de debounce 
  }

}

void init_componentes(){
  gpio_init(BTNA_PIN);
  gpio_set_dir(BTNA_PIN, GPIO_IN);
  gpio_pull_up(BTNA_PIN);

  gpio_init(BTNB_PIN);
  gpio_set_dir(BTNB_PIN, GPIO_IN);
  gpio_pull_up(BTNB_PIN);

  gpio_init(LED_BLUE);
  gpio_set_dir(LED_BLUE, GPIO_OUT);

  gpio_init(LED_GREEN);
  gpio_set_dir(LED_GREEN, GPIO_OUT);
}

void init_pio_matriz()
{
  // Configuração da PIO, necessario para matriz
  offset = pio_add_program(pio, &pio_matrix_program);
  sm = pio_claim_unused_sm(pio, true);
  pio_matrix_program_init(pio, sm, offset, MATRIZ_PIN);
}

void config_I2C(){
  stdio_init_all();

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000); // o parametro baudrate é a velocidade com que em bits por segun (bps) com que o mestre e escravo se comunicam

  //setando as linhas de comunicação do I2C
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); 
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); 

  //define como pull up
  gpio_pull_up(I2C_SDA); 
  gpio_pull_up(I2C_SCL); 

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);
}

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

// Função para exibir um número na matriz de LEDs
void print_valor(int valor, PIO pio, uint sm){

  uint32_t valor_led;
  for (int16_t i = 0; i < 25; i++) {
    // Define a cor vermelha para cada LED
    valor_led = matrix_rgb(0.0, 0.1*(numeros[valor][24 - i]), 0.0); // Apenas o valor vermelho está ativo com intensidade baixa
    pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para o LED
  }
}