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
    const opc_CCard* const card = opc_get_card(cards, i);
    const char* ident = opc_get_card_ident(card);
    printf("  - %s\n", ident);
    printf("    SIG: %s\n", opc_get_card_sig_fpr(card));
    printf("    DEC: %s\n", opc_get_card_dec_fpr(card));
    printf("    AUT: %s\n", opc_get_card_aut_fpr(card));
    if (strcmp(ident, "0000:00000000") == 0) {
      // only the dummy card has correct key imported
        unsigned char* ciphertext = malloc(64);
        unsigned char* plaintext = malloc(64);
        printf("decipher: %s", opc_card_rsa_decipher(card, "123456", ciphertext, sizeof(ciphertext), plaintext, sizeof(plaintext)));
        free(plaintext);
        free(ciphertext);
    }
  }
  opc_free_cards(cards);
  return 0;
}
