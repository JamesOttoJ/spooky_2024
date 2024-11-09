FROM ubuntu:latest
RUN apt-get update && \
    apt-get install -y \
        git \
        openssh-server && \
    apt-get clean

RUN useradd -m admin && echo "admin:poohbear1" | chpasswd
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config
RUN mkdir -p /run/sshd
RUN echo "sp00ky{Cr4ckiNg_Fr0M_tH3_5haDowS}" > /home/admin/flag.txt
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
