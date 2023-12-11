use std::env;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let mut config: cbindgen::Config = Default::default();
    config.export.prefix = Some("opc_".into());
    config.function.prefix = Some("OPC_API".into());
    config.language = cbindgen::Language::C;
    config.usize_is_size_t = true;
    cbindgen::generate_with_config(crate_dir, config)
        .unwrap()
        .write_to_file("openpgp_cards.h");
}
