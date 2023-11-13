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
