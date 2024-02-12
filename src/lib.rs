// SPDX-FileCopyrightText: 2024 Wiktor Kwapisiewicz <wiktor@metacode.biz>
// SPDX-License-Identifier: Apache-2.0
use std::{
    ffi::{CStr, CString},
    slice,
};

use openpgp_card::crypto_data::Hash;

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
    TooShortBuffer,
}

#[repr(C)]
pub enum CCardDecipherMode {
    Rsa,
    Ecdh,
}

#[repr(C)]
pub enum CCardSignMode {
    RsaSha256,
    RsaSha384,
    RsaSha512,
    EdDSA,
    ECDSA,
}

/// Scans for cards.
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_scan_for_cards(cards: *mut *mut CCards) -> CCardError {
    env_logger::init(); // FIXME: drop this as soon as debugging is done
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

/// Returns the number of detected cards.
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_get_cards_len(cards: *const CCards) -> usize {
    (*cards).cards.len()
}

/// Returns a pointer to one of the cards
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_get_card(cards: *mut CCards, card_id: usize) -> *mut CCard {
    &mut (*cards).cards[card_id]
}

/// Returns card identifier (ident)
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_get_card_ident(card: *const CCard) -> *const u8 {
    (*card).ident.as_bytes().as_ptr()
}

/// Returns card's signing key's fingerprint.
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_get_card_sig_fpr(card: *const CCard) -> *const u8 {
    (*card).signature.as_bytes().as_ptr()
}

/// Returns card's decryption key's fingerprint.
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_get_card_dec_fpr(card: *const CCard) -> *const u8 {
    (*card).decryption.as_bytes().as_ptr()
}

/// Returns card's authentication key's fingerprint.
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_get_card_aut_fpr(card: *const CCard) -> *const u8 {
    (*card).authentication.as_bytes().as_ptr()
}

/// Decrypts `ciphertext` into `plaintext`.
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_card_decipher(
    card: *mut CCard,
    pin: *const i8,
    mode: CCardDecipherMode,
    ciphertext: *const u8,
    ciphertext_len: usize,
    plaintext: *mut u8,
    plaintext_len: *mut usize,
) -> CCardError {
    let mut tx = (*card).raw_card.transaction().unwrap();
    let pin = CStr::from_ptr(pin);
    tx.verify_pw1_user(pin.to_bytes()).unwrap();
    let ciphertext = slice::from_raw_parts(ciphertext, ciphertext_len);
    let dm = match mode {
        CCardDecipherMode::Rsa => openpgp_card::crypto_data::Cryptogram::RSA(ciphertext),
        CCardDecipherMode::Ecdh => openpgp_card::crypto_data::Cryptogram::ECDH(ciphertext),
    };
    let decrypted = tx.decipher(dm).unwrap();
    if decrypted.len() > *plaintext_len {
        CCardError::TooShortBuffer
    } else {
        let plaintext = slice::from_raw_parts_mut(plaintext, *plaintext_len);
        plaintext[0..decrypted.len()].copy_from_slice(&decrypted);
        *plaintext_len = decrypted.len();
        CCardError::Success
    }
}

/// Signs `digest` into `signature`.
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_card_sign(
    card: *mut CCard,
    pin: *const i8,
    mode: CCardSignMode,
    digest: *const u8,
    digest_len: usize,
    signature: *mut u8,
    signature_len: *mut usize,
) -> CCardError {
    let mut tx = (*card).raw_card.transaction().unwrap();
    let pin = CStr::from_ptr(pin);
    tx.verify_pw1_sign(pin.to_bytes()).unwrap();
    let digest = slice::from_raw_parts(digest, digest_len);
    let hash = match mode {
        CCardSignMode::RsaSha256 => Hash::SHA256(digest.try_into().unwrap()),
        CCardSignMode::RsaSha384 => Hash::SHA384(digest.try_into().unwrap()),
        CCardSignMode::RsaSha512 => Hash::SHA512(digest.try_into().unwrap()),
        CCardSignMode::ECDSA => Hash::ECDSA(digest),
        CCardSignMode::EdDSA => Hash::EdDSA(digest),
    };
    let signed = tx.signature_for_hash(hash).unwrap();
    if signed.len() > *signature_len {
        CCardError::TooShortBuffer
    } else {
        let signature = slice::from_raw_parts_mut(signature, *signature_len);
        signature[0..signed.len()].copy_from_slice(&signed);
        *signature_len = signed.len();
        CCardError::Success
    }
}

/// Releases memory taken by the cards.
///
/// # Safety
/// This function dereferences `cards`.
#[no_mangle]
pub unsafe extern "C" fn opc_free_cards(cards: *mut CCards) {
    drop(Box::from_raw(cards));
}
