#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum CCardError {
  Success,
  UnknownError,
} CCardError;

typedef struct CCard CCard;

typedef struct CCards CCards;

enum CCardError opc_scan_for_cards(struct CCards **cards);

uintptr_t opc_get_cards_len(const struct CCards *cards);

const struct CCard *opc_get_card(const struct CCards *cards, uintptr_t card_id);

const uint8_t *opc_get_card_ident(const struct CCard *card);

const uint8_t *opc_get_card_sig_fpr(const struct CCard *card);

const uint8_t *opc_get_card_dec_fpr(const struct CCard *card);

const uint8_t *opc_get_card_aut_fpr(const struct CCard *card);

void opc_free_cards(struct CCards *cards);
