#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void *scan_for_cards(void);

uintptr_t get_cards_len(const void *cards);

const uint8_t *get_card_ident(const void *cards, uintptr_t card_id);

const uint8_t *get_card_sig_fpr(const void *cards, uintptr_t card_id);

const uint8_t *get_card_dec_fpr(const void *cards, uintptr_t card_id);

const uint8_t *get_card_aut_fpr(const void *cards, uintptr_t card_id);

void free_cards(void *cards);
