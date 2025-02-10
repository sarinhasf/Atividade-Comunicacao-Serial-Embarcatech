<img width=100% src="https://capsule-render.vercel.app/api?type=waving&color=6959CD&height=120&section=header"/>


<h1 align="center">EmbarcaTech | Atividade Comunicação Serial</h1>

Para consolidar os conceitos sobre o uso de interfaces de comunicação serial no RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab, propõe-se a realização da seguinte tarefa individual prática. 

## Objetivos:

- Compreender o funcionamento e a aplicação de comunicação serial em microcontroladores. 
- Aplicar os conhecimentos adquiridos sobre UART e I2C na prática. 
- Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812. 
- Fixar o estudo do uso botões de acionamento, interrupções e Debounce. 
- Desenvolver um projeto funcional que combine hardware e software.

## Materiais utilizados:

Neste projeto, são utilizados os seguintes componentes conectados à placa BitDogLab:

- Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7. 
- LED RGB, com os pinos conectados às GPIOs (11, 12 e 13). 
- Botão A conectado à GPIO 5. 
- Botão B conectado à GPIO 6. 
- Display SSD1306 conectado via I2C (GPIO 14 e GPIO15).

## Lista de requisitos:

1. Modificação da Biblioteca font.h 
- Adicionar caracteres minúsculos à biblioteca font.h. Use sua criatividade para criar os novos caracteres. 

2. Entrada de caracteres via PC  
- Utilize o Serial Monitor do VS Code para digitar os caracteres. 
- Cada caractere digitado no Serial Monitor deve ser exibido no display SSD1306. 
  Observação: Apenas um caractere será enviado de cada vez, não é necessário suportar o envio de strings completas. 
- Quando um número entre 0 e 9 for digitado, um símbolo correspondente ao número deve ser exibido, também, na matriz 5x5 WS2812.  

3. Interação com o Botão A  
- Pressionar o botão A deve alternar o estado do LED RGB Verde (ligado/desligado). 
- A operação deve ser registrada de duas formas: 
    - Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 
    - Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor. 

4. Interação com o Botão B 
- Pressionar o botão A deve alternar o estado do LED RGB Azul (ligado/desligado). 
- A operação deve ser registrada de duas formas: 
   - Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 
   - Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.

## Tecnologias:

1. Git e Github;
2. VScode;
3. Linguagem C;
4. Simulador Wokwi;
5. Placa de desenvolvimento BitDogLab

## 💻 Instruções para Importar, Compilar e Rodar o Código Localmente:

Siga os passos abaixo para clonar o repositório, importar no VS Code usando a extensão do **Raspberry Pi Pico Project**, compilar e executar o código.

1. **Clone o repositório para sua máquina local**  
   Abra o terminal e execute os comandos abaixo:
   ```bash
   git clone https://github.com/sarinhasf/Atividade-Comunicacao-Serial-Embarcatech.git
   cd projeto

2. **Abra o VS Code e instale a extensão "Raspberry Pi Pico Project" (caso não já a tenha instalada)**
 - No VS Code, vá até "Extensões" (Ctrl+Shift+X)
 - Pesquise por "Raspberry Pi Pico Project"
 - Instale a extensão oficial

3. **Importe o projeto no VS Code**
 - No VS Code, na barra lateral do lado esquerdo clique em "Raspberry Pi Pico Project" <img src="images/icon_raspberry_pico_project.png" width="25px">
 - No menu que aparecer clique em <img src="images/icon_import_project.png" height="25px">
 - Clicando em "Change" escolha a pasta clonada do repositório
 - Escolha a versão do SDK 2.1.0
 - Clique em "Import"

## Desenvolvedora:
 
<table>
  <tr>
    <td align="center"><img style="" src="https://avatars.githubusercontent.com/u/143294885?v=4" width="100px;" alt=""/><br /><sub><b> Sara Souza </b></sub></a><br />👨‍💻</a></td>
</table>

## Link do video de apresentação: 

<table>
  <tr>
    <td align="center"> <sub><b> https://youtube.com/shorts/46I2yqSNuhE?si=9NXFCTr12a_v_WfA </b></td>
    </tr>
</table>
