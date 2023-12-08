use std::{
    ffi::{CStr, CString},
    slice,
};

pub struct CCards {
    cards: Vec<CCard>,
}

pub struct CCard {
    ident: CString,
    signature: CString,
    decryption: CString,
    authentication: CString,
    raw_card: openpgp_card::Card,
}

#[repr(C)]
pub enum CCardError {
    Success,
    UnknownError,
}

#[no_mangle]
pub extern "C" fn opc_scan_for_cards(cards: *mut *mut CCards) -> CCardError {
    //let dest = unsafe { std::slice::from_raw_parts_mut(cards, len) };
    let mut cards_v = vec![];
    for pcsc in card_backend_pcsc::PcscBackend::cards(None).unwrap() {
        let mut card = openpgp_card::Card::new(pcsc.unwrap()).unwrap();
        let (ident, signature, decryption, authentication) = {
            let mut card_tx = card.transaction().unwrap();
            let ard = card_tx.application_related_data().unwrap();
            let card_id = ard.application_id().unwrap().ident();

            let fingerprints = ard.fingerprints().unwrap();
            (
                CString::new(card_id).unwrap(),
                CString::new(
                    fingerprints
                        .signature()
                        .map(|x| x.to_string())
                        .unwrap_or_default(),
                )
                .unwrap(),
                CString::new(
                    fingerprints
                        .decryption()
                        .map(|x| x.to_string())
                        .unwrap_or_default(),
                )
                .unwrap(),
                CString::new(
                    fingerprints
                        .authentication()
                        .map(|x| x.to_string())
                        .unwrap_or_default(),
                )
                .unwrap(),
            )
        };
        cards_v.push(CCard {
            ident,
            signature,
            decryption,
            authentication,
            raw_card: card,
        });
    }
    unsafe { *cards = Box::into_raw(Box::new(CCards { cards: cards_v })) };
    CCardError::Success
}

#[no_mangle]
pub unsafe extern "C" fn opc_get_cards_len(cards: *const CCards) -> usize {
    (*cards).cards.len()
}

#[no_mangle]
pub unsafe extern "C" fn opc_get_card(cards: *const CCards, card_id: usize) -> *const CCard {
    &(*cards).cards[card_id]
}

#[no_mangle]
pub unsafe extern "C" fn opc_get_card_ident(card: *const CCard) -> *const u8 {
    (*card).ident.as_bytes().as_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn opc_get_card_sig_fpr(card: *const CCard) -> *const u8 {
    (*card).signature.as_bytes().as_ptr()
}
#[no_mangle]
pub unsafe extern "C" fn opc_get_card_dec_fpr(card: *const CCard) -> *const u8 {
    (*card).decryption.as_bytes().as_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn opc_get_card_aut_fpr(card: *const CCard) -> *const u8 {
    (*card).authentication.as_bytes().as_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn opc_card_rsa_decipher(
    card: *mut CCard,
    pin: *const i8,
    ciphertext: *const u8,
    ciphertext_len: usize,
    plaintext: *mut u8,
    plaintext_len: usize,
) -> CCardError {
    let mut tx = (*card).raw_card.transaction().unwrap();
    let pin = CStr::from_ptr(pin);
    tx.verify_pw1_user(pin.to_bytes()).unwrap();
    let ciphertext = slice::from_raw_parts(ciphertext, ciphertext_len);
    let decrypted = tx
        .decipher(openpgp_card::crypto_data::Cryptogram::RSA(ciphertext))
        .unwrap();
    let plaintext = slice::from_raw_parts_mut(plaintext, plaintext_len);
    plaintext.copy_from_slice(&decrypted[0..decrypted.len()]);
    CCardError::Success
}

#[no_mangle]
pub unsafe extern "C" fn opc_free_cards(cards: *mut CCards) {
    drop(Box::from_raw(cards));
}
