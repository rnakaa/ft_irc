FROM ubuntu:latest

ARG USER_HOME

RUN apt-get update
RUN apt-get install -y git build-essential clang-format tmux vim
RUN rm -rf /var/lib/apt/lists/*
RUN git config --global commit.template .commit_template

WORKDIR /workspace