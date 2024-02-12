<!--
SPDX-FileCopyrightText: 2024 Wiktor Kwapisiewicz <wiktor@metacode.biz>
SPDX-License-Identifier: CC0-1.0
-->
# OpenPGP Cards Shared Library

This library exposes OpenPGP Cards functions through a shared library.

See [`openpgp_cards.h`](openpgp_cards.h) file for functions that are exported by this library.

There is also a sample C program attached that executes them.

To recompile everything see `run.sh` which should build it and print card info.

For example:

```
Cards: 1
  - 0006:15422467
    SIG: 0C7C54912FD932BCDF13726A767CE224DB311B3C
    DEC: F99A81E09CD8814B571DBF4AEB0BE68CD9CF08F1
    AUT: 3BA4FE02BF714A7789CB2E0051F23D6C0529CE0A
```

Build dependencies are specified in the `Dockerfile` that is also used in CI.

## Funding

This project is funded through [NGI Assure](https://nlnet.nl/assure), a fund established by [NLnet](https://nlnet.nl) with financial support from the European Commission's [Next Generation Internet](https://ngi.eu) program. Learn more at the [NLnet project page](https://nlnet.nl/project/OpenPGPCA-HSM/).

[<img src="https://nlnet.nl/logo/banner.png" alt="NLnet foundation logo" width="20%" />](https://nlnet.nl)
[<img src="https://nlnet.nl/image/logos/NGIAssure_tag.svg" alt="NGI Assure Logo" width="20%" />](https://nlnet.nl/assure)

## License

This project is licensed under the [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0).

### Contribution

Unless you explicitly state otherwise, any contribution intentionally
submitted for inclusion in this crate by you, as defined in the
Apache-2.0 license, shall be licensed as above, without any
additional terms or conditions.
