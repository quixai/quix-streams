# Create a virtual environment with all tools installed
FROM quay.io/pypa/manylinux2010_x86_64:latest AS env
LABEL maintainer="mizux.dev@gmail.com"
# Install system build dependencies
ENV PATH=/usr/local/bin:$PATH

RUN yum -y update \
&& yum -y install git wget openssl-devel pcre-devel \
&& yum -y groupinstall "Development Tools" \
&& yum clean all \
&& rm -rf /var/cache/yum

# Install CMake 3.18.1
RUN wget "https://cmake.org/files/v3.18/cmake-3.18.1-Linux-x86_64.sh" \
&& chmod a+x cmake-3.18.1-Linux-x86_64.sh \
&& ./cmake-3.18.1-Linux-x86_64.sh --prefix=/usr/local --skip-license \
&& rm cmake-3.18.1-Linux-x86_64.sh
# CMake version
RUN cmake -version

# Install SWIG 4.0.2
RUN wget "https://downloads.sourceforge.net/project/swig/swig/swig-4.0.2/swig-4.0.2.tar.gz" \
&& tar xvf swig-4.0.2.tar.gz \
&& rm swig-4.0.2.tar.gz \
&& cd swig-4.0.2 \
&& ./configure --prefix=/usr \
&& make -j 4 \
&& make install \
&& cd .. \
&& rm -rf swig-4.0.2

FROM env AS devel
WORKDIR /home/project
COPY . .

COPY ci/build_manylinux.sh .
RUN chmod a+x build_manylinux.sh

FROM devel AS build
RUN ./build_manylinux.sh
