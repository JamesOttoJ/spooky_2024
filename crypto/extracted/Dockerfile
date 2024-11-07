FROM debian
RUN apt-get update && \
    apt-get install -y \
        git \
        openssh-server

RUN useradd -m admin && echo "admin:poohbear1" | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN echo "sp00ky{Cr4ckiNg_Fr0M_tH3_5haDowS}" > /home/admin/flag.txt
EXPOSE 22
CMD service ssh start && /bin/bash
