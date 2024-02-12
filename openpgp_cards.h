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

typedef enum opc_CCardSignMode {
  opc_CCardSignMode_RsaSha256,
  opc_CCardSignMode_RsaSha384,
  opc_CCardSignMode_RsaSha512,
  opc_CCardSignMode_EdDSA,
  opc_CCardSignMode_ECDSA,
} opc_CCardSignMode;

typedef struct opc_CCard opc_CCard;

typedef struct opc_CCards opc_CCards;

/**
 * Scans for cards.
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API enum opc_CCardError opc_scan_for_cards(struct opc_CCards **cards);

/**
 * Returns the number of detected cards.
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API size_t opc_get_cards_len(const struct opc_CCards *cards);

/**
 * Returns a pointer to one of the cards
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API struct opc_CCard *opc_get_card(struct opc_CCards *cards, size_t card_id);

/**
 * Returns card identifier (ident)
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API const uint8_t *opc_get_card_ident(const struct opc_CCard *card);

/**
 * Returns card's signing key's fingerprint.
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API const uint8_t *opc_get_card_sig_fpr(const struct opc_CCard *card);

/**
 * Returns card's decryption key's fingerprint.
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API const uint8_t *opc_get_card_dec_fpr(const struct opc_CCard *card);

/**
 * Returns card's authentication key's fingerprint.
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API const uint8_t *opc_get_card_aut_fpr(const struct opc_CCard *card);

/**
 * Decrypts `ciphertext` into `plaintext`.
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API
enum opc_CCardError opc_card_decipher(struct opc_CCard *card,
                                      const int8_t *pin,
                                      enum opc_CCardDecipherMode mode,
                                      const uint8_t *ciphertext,
                                      size_t ciphertext_len,
                                      uint8_t *plaintext,
                                      size_t *plaintext_len);

/**
 * Signs `digest` into `signature`.
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API
enum opc_CCardError opc_card_sign(struct opc_CCard *card,
                                  const int8_t *pin,
                                  enum opc_CCardSignMode mode,
                                  const uint8_t *digest,
                                  size_t digest_len,
                                  uint8_t *signature,
                                  size_t *signature_len);

/**
 * Releases memory taken by the cards.
 *
 * # Safety
 * This function dereferences `cards`.
 */
OPC_API void opc_free_cards(struct opc_CCards *cards);
