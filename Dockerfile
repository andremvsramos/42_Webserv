FROM ubuntu:latest

RUN apt update

COPY ./var /usr/webserv/var