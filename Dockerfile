FROM ubuntu:22.04
RUN apt-get update
RUN apt-get install --reinstall make
RUN apt-get update
RUN apt-get install -y g++ vim git
RUN apt install -y git vim wget emacs pciutils
RUN apt-get update
# Install dieharder
RUN apt-get install -y dieharder 
RUN apt-get update

