FROM debian
MAINTAINER amerelo
RUN apt-get update
RUN apt-get install git make g++ htop vim netcat net-tools -y
CMD "/usr/bin/bash"