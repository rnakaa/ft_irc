FROM ubuntu:latest

ARG USER_HOME

RUN apt-get update
RUN apt-get install -y git make build-essential
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /workspace