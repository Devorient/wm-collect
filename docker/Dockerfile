# start from latest ubuntu build
FROM ubuntu:latest

# arm gcc version
ARG GCC_VERSION=7-2018q2
ARG GCC_VERSION_FNAME=7-2018-q2-update

# apt-get env
ENV DEBIAN_FRONTEND noninteractive

# install needed packages
RUN apt-get update -qq &&\
    apt-get install -yqq --no-install-recommends apt-utils &&\
    apt-get install -yqq --no-install-recommends \
        wget git make srecord \
        python python-setuptools python-pip \
        python3  python3-setuptools python3-pip

WORKDIR /home/dev

# fetch msp430gcc
RUN wget --no-check-certificate -qO- https://github.com/pksec/msp430-gcc-4.7.3/raw/master/mspgcc-4.7.3.tar.bz2 | tar xj &&\
    mv mspgcc-4.7.3 msp430-gcc

# download and install arm gcc
RUN wget -qO- https://developer.arm.com/-/media/Files/downloads/gnu-rm/${GCC_VERSION}/gcc-arm-none-eabi-${GCC_VERSION_FNAME}-linux.tar.bz2 | tar xj &&\
    mv gcc-arm-none-eabi-${GCC_VERSION_FNAME} gcc-arm-none-eabi

ENV PATH /home/dev/gcc-arm-none-eabi/bin:/home/dev/msp430-gcc/bin:$PATH

# install cc3200tool
RUN pip2 install --no-cache-dir pyserial git+git://github.com/ALLTERCO/cc3200tool.git
# install python3 requirements
RUN pip3 install --no-cache-dir pyserial numpy matplotlib pandas

# uninstall unneeded tools
RUN apt-get purge -y wget python-pip python-setuptools python3-pip python3-setuptools && apt-get autoremove -y && rm -rf /var/lib/apt/lists/*

WORKDIR /home/proj/