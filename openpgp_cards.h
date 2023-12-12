#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum opc_CCardDecipherMode {
  opc_CCardDecipherMode_Rsa,
  opc_CCardDecipherMode_Ecdh,
} opc_CCardDecipherMode;

typedef enum opc_CCardError {
  opc_CCardError_Success,
  opc_CCardError_UnknownError,
  opc_CCardError_TooShortBuffer,
} opc_CCardError;

typedef struct opc_CCard opc_CCard;

typedef struct opc_CCards opc_CCards;

OPC_API enum opc_CCardError opc_scan_for_cards(struct opc_CCards **cards);

OPC_API size_t opc_get_cards_len(const struct opc_CCards *cards);

OPC_API struct opc_CCard *opc_get_card(struct opc_CCards *cards, size_t card_id);

OPC_API const uint8_t *opc_get_card_ident(const struct opc_CCard *card);

OPC_API const uint8_t *opc_get_card_sig_fpr(const struct opc_CCard *card);

OPC_API const uint8_t *opc_get_card_dec_fpr(const struct opc_CCard *card);

OPC_API const uint8_t *opc_get_card_aut_fpr(const struct opc_CCard *card);

OPC_API
enum opc_CCardError opc_card_decipher(struct opc_CCard *card,
                                      const int8_t *pin,
                                      enum opc_CCardDecipherMode mode,
                                      const uint8_t *ciphertext,
                                      size_t ciphertext_len,
                                      uint8_t *plaintext,
                                      size_t *plaintext_len);

OPC_API void opc_free_cards(struct opc_CCards *cards);
