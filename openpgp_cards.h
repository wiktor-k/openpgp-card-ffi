#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct CCards CCards;

uint8_t scan_for_cards(struct CCards **cards);

uintptr_t get_cards_len(const struct CCards *cards);

const uint8_t *get_card_ident(const struct CCards *cards, uintptr_t card_id);

const uint8_t *get_card_sig_fpr(const struct CCards *cards, uintptr_t card_id);

const uint8_t *get_card_dec_fpr(const struct CCards *cards, uintptr_t card_id);

const uint8_t *get_card_aut_fpr(const struct CCards *cards, uintptr_t card_id);

void free_cards(struct CCards *cards);
