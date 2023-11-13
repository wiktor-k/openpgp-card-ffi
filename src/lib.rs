use std::ffi::{c_void, CString};

pub struct CCards {
    cards: Vec<CCard>,
}

pub struct CCard {
    ident: CString,
    signature: CString,
    decryption: CString,
    authentication: CString,
}

#[no_mangle]
pub unsafe extern "C" fn scan_for_cards() -> *mut c_void {
    //let dest = unsafe { std::slice::from_raw_parts_mut(cards, len) };
    let mut cards = vec![];
    for pcsc in card_backend_pcsc::PcscBackend::cards(None).unwrap() {
        let mut card = openpgp_card::Card::new(pcsc.unwrap()).unwrap();
        let mut card_tx = card.transaction().unwrap();
        let ard = card_tx.application_related_data().unwrap();
        let card_id = ard.application_id().unwrap().ident();

        let fingerprints = ard.fingerprints().unwrap();
        //dest[0..card_id.len()].copy_from_slice(CString::new(card_id).unwrap().as_bytes());
        cards.push(CCard {
            ident: CString::new(card_id).unwrap(),
            signature: CString::new(
                fingerprints
                    .signature()
                    .map(|x| x.to_string())
                    .unwrap_or_default(),
            )
            .unwrap(),
            decryption: CString::new(
                fingerprints
                    .decryption()
                    .map(|x| x.to_string())
                    .unwrap_or_default(),
            )
            .unwrap(),
            authentication: CString::new(
                fingerprints
                    .authentication()
                    .map(|x| x.to_string())
                    .unwrap_or_default(),
            )
            .unwrap(),
        });
    }
    Box::into_raw(Box::new(CCards { cards })) as _
}

#[no_mangle]
pub unsafe extern "C" fn get_cards_len(cards: *const c_void) -> usize {
    let cards = cards as *const CCards;
    (*cards).cards.len()
}

#[no_mangle]
pub unsafe extern "C" fn get_card_ident(cards: *const c_void, card_id: usize) -> *const u8 {
    let cards = cards as *const CCards;
    (*cards).cards[card_id].ident.as_bytes().as_ptr() as _
}

#[no_mangle]
pub unsafe extern "C" fn get_card_sig_fpr(cards: *const c_void, card_id: usize) -> *const u8 {
    let cards = cards as *const CCards;
    (*cards).cards[card_id].signature.as_bytes().as_ptr() as _
}
#[no_mangle]
pub unsafe extern "C" fn get_card_dec_fpr(cards: *const c_void, card_id: usize) -> *const u8 {
    let cards = cards as *const CCards;
    (*cards).cards[card_id].decryption.as_bytes().as_ptr() as _
}

#[no_mangle]
pub unsafe extern "C" fn get_card_aut_fpr(cards: *const c_void, card_id: usize) -> *const u8 {
    let cards = cards as *const CCards;
    (*cards).cards[card_id].authentication.as_bytes().as_ptr() as _
}

#[no_mangle]
pub unsafe extern "C" fn free_cards(cards: *mut c_void) {
    drop(Box::from_raw(cards));
}
