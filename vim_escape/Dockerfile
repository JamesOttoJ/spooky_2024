FROM ubuntu:latest
RUN apt-get update && \
    apt-get install -y \
        git \
        openssh-server \
	vim && \
    apt-get clean

# RUN echo "/bin/vi" > /etc/shells
RUN useradd -m -s /bin/vi user && echo "user:MyPassword2024" | chpasswd
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config
RUN mkdir -p /run/sshd
RUN mkdir /flag
RUN echo "sp00ky{j41LbR3ak!}" > /flag/flag.txt
RUN chmod 744 /flag/flag.txt
RUN echo "Welcome to vim! Enjoy your stay" > /home/user/file.txt
RUN echo "user ALL=(ALL) nopasswd: vi" >> /etc/sudoers
EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
