# ===== Base =====
FROM ubuntu:22.04

# ===== System Tools =====
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 python3-pip python3-dev \
    ninja-build \
    git \
    wget \
    pybind11-dev \
    vim \
    coreutils \
    && rm -rf /var/lib/apt/lists/*

# ===== Python Dependencies =====
RUN pip3 install --upgrade pip
RUN pip3 install pybind11 scikit-build cmake ninja pytest numpy

# ===== Eigen (ヘッダオンリー) =====
RUN git clone https://gitlab.com/libeigen/eigen.git /opt/eigen \
    && cd /opt/eigen && mkdir build && cd build \
    && cmake .. && make install

# ===== Set CMAKE_PREFIX_PATH for Eigen =====
ENV CMAKE_PREFIX_PATH=/usr/local

# ===== Work Directory =====
WORKDIR /workspace

# ===== Default Command =====
CMD ["/bin/bash"]
