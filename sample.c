#define OPC_API
#include "openpgp_cards.h"
#include <stdio.h>
#include <string.h>

int main() {
  opc_CCards *cards = 0;
  printf("result: %d\n", opc_scan_for_cards(&cards));
  int cards_num = opc_get_cards_len(cards);
  printf("Cards: %d\n", cards_num);
  for (int i = 0; i < cards_num; i++) {
    opc_CCard *const card = opc_get_card(cards, i);
    const char *ident = opc_get_card_ident(card);
    printf("  - %s\n", ident);
    printf("    SIG: %s\n", opc_get_card_sig_fpr(card));
    const char *dec = opc_get_card_dec_fpr(card);
    printf("    DEC: %s\n", dec);
    printf("    AUT: %s\n", opc_get_card_aut_fpr(card));
    if (strcmp(dec, "4AF56C624C9F9D877EB5954E95C3E680713DCCA9") == 0) {
      // this subkey is an RSA key
      unsigned char ciphertext[] = {
          51,  115, 163, 167, 75,  157, 165, 94,  98,  179, 163, 80,  131, 203,
          163, 96,  9,   129, 13,  58,  134, 175, 247, 19,  226, 250, 93,  248,
          160, 159, 27,  236, 14,  115, 38,  74,  33,  124, 199, 244, 5,   62,
          195, 155, 112, 160, 252, 209, 16,  199, 167, 114, 196, 159, 235, 22,
          53,  70,  205, 197, 50,  96,  152, 83,  156, 244, 18,  186, 138, 37,
          232, 167, 101, 218, 219, 140, 169, 37,  75,  197, 212, 105, 145, 70,
          234, 79,  229, 15,  76,  188, 214, 173, 192, 209, 63,  61,  161, 65,
          15,  182, 146, 33,  92,  28,  113, 101, 214, 59,  197, 219, 208, 176,
          123, 255, 126, 11,  11,  228, 57,  157, 100, 29,  196, 154, 166, 217,
          27,  220, 157, 78,  91,  80,  75,  77,  44,  100, 207, 125, 147, 249,
          91,  90,  61,  29,  72,  92,  215, 234, 151, 24,  51,  41,  137, 143,
          208, 156, 229, 166, 29,  31,  52,  57,  144, 250, 134, 188, 215, 46,
          254, 58,  13,  111, 240, 1,   75,  227, 216, 184, 123, 243, 34,  81,
          88,  58,  234, 102, 52,  136, 226, 101, 109, 1,   95,  41,  190, 65,
          254, 127, 12,  224, 195, 9,   10,  109, 73,  51,  174, 183, 13,  130,
          54,  167, 53,  142, 212, 111, 27,  188, 22,  59,  141, 133, 215, 185,
          188, 156, 115, 152, 96,  160, 205, 172, 251, 53,  194, 26,  235, 133,
          204, 184, 188, 151, 51,  93,  113, 23,  64,  12,  35,  255, 109, 212,
          154, 184, 58,  222, 120, 96,  7,   198, 39,  3,   232, 96,  218, 94,
          87,  205, 198, 160, 56,  164, 201, 240, 218, 55,  103, 136, 241, 34,
          36,  226, 140, 216, 232, 5,   3,   143, 138, 193, 3,   120, 191, 201,
          243, 240, 51,  1,   77,  33,  66,  241, 2,   192, 85,  98,  68,  133,
          176, 85,  162, 160, 34,  67,  175, 119, 70,  116, 127, 222, 9,   223,
          9,   4,   169, 13,  111, 35,  127, 0,   36,  121, 61,  164, 165, 231,
          142, 226, 120, 202, 73,  226, 168, 77,  95,  200, 41,  20,  73,  240,
          71,  168, 128, 126, 121, 189, 145, 135, 228, 196, 229, 154, 133, 58,
          235, 210, 98,  16,  127, 17,  228, 204, 100, 3,   67,  215, 7,   12,
          224, 129, 55,  211, 211, 14};
      size_t plaintext_len = sizeof ciphertext;
      unsigned char *plaintext = malloc(plaintext_len);
      printf("RSA decipher: %d: ",
             opc_card_decipher(card, "123456", opc_CCardDecipherMode_Rsa,
                               ciphertext, sizeof(ciphertext), plaintext,
                               &plaintext_len));
      for (int i = 0; i < plaintext_len; i++) {
        printf("%x ", plaintext[i]);
      }
      printf("\n");
      free(plaintext);
    } else if (strcmp(dec, "4918FADB50535EE23248DF7F874416307B2472BD") == 0) {
      // this subkey is an cv25519 subkey
      // if the MPI contains 0x40 as its first byte and the algorithm is 25519
      // the first byte needs to be stripped
      unsigned char ciphertext[] = {156, 17,  181, 77,  127, 208, 205, 214,
                                    180, 37,  54,  234, 90,  212, 59,  146,
                                    178, 191, 150, 127, 22,  220, 234, 146,
                                    58,  116, 165, 139, 179, 80,  7,   31};
      size_t plaintext_len = sizeof ciphertext;
      unsigned char *plaintext = malloc(plaintext_len);
      printf("ECDH decipher: %d: ",
             opc_card_decipher(card, "123456", opc_CCardDecipherMode_Ecdh,
                               ciphertext, sizeof(ciphertext), plaintext,
                               &plaintext_len));
      for (int i = 0; i < plaintext_len; i++) {
        printf("%x ", plaintext[i]);
      }
    }
  }
  opc_free_cards(cards);
  return 0;
}
