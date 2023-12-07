#define OPC_API
#include "openpgp_cards.h"
#include <stdio.h>

int main() {
  opc_CCards *cards = 0;
  printf("result: %d\n", opc_scan_for_cards(&cards));
  int cards_num = opc_get_cards_len(cards);
  printf("Cards: %d\n", cards_num);
  for (int i = 0; i < cards_num; i++) {
    const opc_CCard* const card = opc_get_card(cards, i);
    printf("  - %s\n", opc_get_card_ident(card));
    printf("    SIG: %s\n", opc_get_card_sig_fpr(card));
    printf("    DEC: %s\n", opc_get_card_dec_fpr(card));
    printf("    AUT: %s\n", opc_get_card_aut_fpr(card));
  }
  opc_free_cards(cards);
  return 0;
}
