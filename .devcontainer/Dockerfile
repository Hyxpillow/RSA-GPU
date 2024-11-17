FROM mcr.microsoft.com/devcontainers/cpp:1-debian-11

RUN apt-get update && \
     apt-get install -y libssl-dev git && \
     apt-get clean


# RUN mkdir -p /usr/local/cuda/include && \
#     chmod -R 777 /usr/local/cuda/include

# RUN curl -L https://gitlab.com/nvidia/headers/cuda-individual/cudart/-/raw/main/cuda.h > /usr/local/cuda/include/cuda.h

# ENV CPLUS_INCLUDE_PATH=/usr/local/cuda/include:$CPLUS_INCLUDE_PATH

