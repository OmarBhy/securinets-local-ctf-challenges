FROM debian:stable-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends socat coreutils && \
    rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /usr/sbin/nologin ctf
WORKDIR /home/ctf

COPY Behaviour_portal /home/ctf/Behaviour_portal
COPY flag.txt /home/ctf/flag.txt

RUN chown ctf:ctf /home/ctf/Behaviour_portal && chmod 755 /home/ctf/Behaviour_portal && \
    chown ctf:ctf /home/ctf/flag.txt && chmod 444 /home/ctf/flag.txt

USER ctf
EXPOSE 1337

CMD ["socat","TCP-LISTEN:1337,reuseaddr,fork,nodelay","EXEC:timeout 300 ./Behaviour_portal,stderr"]
