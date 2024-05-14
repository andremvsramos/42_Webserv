# Use an appropriate base image
FROM ubuntu:latest

# Set the working directory
WORKDIR .

RUN sudo apt update

# Copy the current directory contents into the container at /app
COPY /var /webserv/www/var
