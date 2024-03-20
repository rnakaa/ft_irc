FROM ubuntu:latest

ARG USER_HOME

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get --no-install-recommends install -y git build-essential clang-format tmux vim-gtk3 netcat locales

RUN rm -rf /var/lib/apt/lists/*
RUN ln -fs /usr/share/zoneinfo/Asia/Tokyo /etc/localtime
RUN dpkg-reconfigure --frontend noninteractive tzdata
RUN locale-gen ja_JP.UTF-8

ENV LANG ja_JP.UTF-8
ENV LANGUAGE ja_JP:ja
ENV LC_ALL ja_JP.UTF-8

WORKDIR /workspace
