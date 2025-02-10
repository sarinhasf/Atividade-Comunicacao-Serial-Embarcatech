#include <stdio.h>
#include <stdint.h>

/* Essa classe pega o número que vc desenhar na matriz e te da em hexadecimal */
/* OBS. A matriz de exibição do display é INVERTIDA, por isso cuidado na hora de desenhar os caracters */

static const uint32_t data[8][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 1, 0, 0, 0, 1, 0, 0 },
    { 0, 0, 0, 0, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 }
};


/* Função para converter a matriz em valores hexadecimais */
void print_hex(const uint32_t matrix[8][8]) {
    for (int row = 0; row < 8; row++) {
        uint8_t hex_value = 0;
        for (int col = 0; col < 8; col++) {
            hex_value |= (matrix[row][col] & 1) << (7 - col); // Monta o byte
        }
        printf("0x%02x, ", hex_value);
    }
    printf("// Caracter\n");
}

int main() {
    print_hex(data);
    return 0;
}
