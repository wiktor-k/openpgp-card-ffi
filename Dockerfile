FROM registry.gitlab.com/openpgp-card/virtual-cards/opcard-rs-tools

RUN apt-get update -y -qq && \
    apt-get install -y -qq --no-install-recommends pcscd libpcsclite-dev && \
    apt-get clean
RUN cargo install --locked openpgp-card-tools
WORKDIR /app
COPY . /app
#ENV RUST_LOG=debug
RUN echo 12345678 > pin
RUN echo 123456 > user-pin
RUN sh /start.sh && \
    ./opgpcard admin --card 0000:00000000 --admin-pin pin import sample-rsa-key.asc && \
    ./opgpcard status --card 0000:00000000 && \
    bash run.sh && \
    ./opgpcard admin --card 0000:00000000 --admin-pin pin import sample-ec-key.asc && \
    ./opgpcard status --card 0000:00000000 && \
    bash run.sh
