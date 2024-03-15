#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define BYTE_SIZE         8
#define MASK_SIZE         512
#define ERROR_CODE        1
#define PADDING_LENGTH    448
#define MESSAGE_LENGTH    64
#define ITERATIONS        80
#define GENERATE_HEX_SIZE 5

void str_to_binary(uint8_t *mask, uint16_t *used_bits, const char *message) {
  printf("Starting converting %s to bits and storing them in the mask array.\n", message);
  const char *c = message;
  while (*c != '\0') {
    for (int i = BYTE_SIZE - 1; i >= 0; --i) {
      mask[(*used_bits)++] = *c >> i & 1;
    }
    ++c;
  }
  printf("Finished conversion.\n");
}

void int_to_binary (const uint64_t num, uint8_t *mask, uint16_t *used_bits) {
  for (int i = MESSAGE_LENGTH - 1; i >= 0; --i) {
    mask[(*used_bits)++] = num >> i & 1;
  }
}

void append_bits(uint8_t *mask, uint16_t *used_bits, const uint32_t message_len) {
  printf("Starting appending the necessary bits to mask.\n");
  mask[(*used_bits)++] = 1;
  while (*used_bits < PADDING_LENGTH) {
    mask[(*used_bits)++] = 0;
  }
  int_to_binary(message_len * BYTE_SIZE, mask, used_bits);
  printf("Finished appending.\n");
}

uint8_t *create_mask(const char *message) {
  uint8_t *mask = malloc(MASK_SIZE * sizeof(uint8_t));
  if (mask == NULL) {
    printf("The memory allocation for mask was not properly done.\n");
    exit(ERROR_CODE);
  }
  uint16_t used_bits = 0;

  str_to_binary(mask, &used_bits, message);
  append_bits(mask, &used_bits, strlen(message));
  return mask;
}

uint32_t circular_shift(const uint64_t number, const uint8_t magnitude) {
  return number << magnitude | number >> 32 - magnitude;
}

void perform_iterations(uint8_t *mask) {

}

void print_mask(const uint8_t *mask) {
  for (uint16_t i = 0; i < MASK_SIZE; ++i) {
    if (i % 64 == 0) {
      printf("\n");
    }
    printf("%d", mask[i]);

  }
}

uint32_t *generate_hex(const uint8_t size) {
  uint32_t *hex = malloc(size * sizeof(uint32_t));
  if (hex == NULL) {
    printf("The memory allocation for hex was not properly done.\n");
    exit(ERROR_CODE);
  }

  srand(time(NULL));
  for (uint8_t i = 0; i < size; ++i) {
    hex[i] = rand();
  }
  return hex;
}

int main(void) {
  const char *message = "abc\0";
  uint32_t *hex = generate_hex(GENERATE_HEX_SIZE);
  uint8_t *mask = create_mask(message);
  print_mask(mask);
  free(mask);
  free(hex);
  return 0;
}