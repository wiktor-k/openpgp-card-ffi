FROM registry.gitlab.com/openpgp-card/virtual-cards/opcard-rs-tools

RUN apt-get update -y -qq && \
    apt-get install -y -qq --no-install-recommends pcscd libpcsclite-dev && \
    apt-get clean
RUN cargo install --locked openpgp-card-tools
WORKDIR /app
RUN gpg --batch --pinentry-mode loopback --passphrase '' --quick-gen-key no-passwd@example.com future-default && \
    gpg --batch --pinentry-mode loopback --passphrase '' --export-secret-key no-passwd@example.com > no-passwd.pgp
COPY . /app
RUN echo 12345678 > pin
RUN sh /start.sh && opgpcard admin --card 0000:00000000 --admin-pin pin import no-passwd.pgp && bash run.sh
