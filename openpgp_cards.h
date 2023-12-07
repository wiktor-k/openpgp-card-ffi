#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#define OPC_API

typedef enum opc_CCardError {
  Success,
  UnknownError,
} opc_CCardError;

typedef struct opc_CCard opc_CCard;

typedef struct opc_CCards opc_CCards;

enum opc_CCardError opc_scan_for_cards(struct opc_CCards **cards);

uintptr_t opc_get_cards_len(const struct opc_CCards *cards);

const struct opc_CCard *opc_get_card(const struct opc_CCards *cards, uintptr_t card_id);

const uint8_t *opc_get_card_ident(const struct opc_CCard *card);

const uint8_t *opc_get_card_sig_fpr(const struct opc_CCard *card);

const uint8_t *opc_get_card_dec_fpr(const struct opc_CCard *card);

const uint8_t *opc_get_card_aut_fpr(const struct opc_CCard *card);

void opc_free_cards(struct opc_CCards *cards);
