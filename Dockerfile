FROM ubuntu:latest
FROM gcc:latest
FROM python:3

# cmake
RUN apt-get update && apt-get install -y cmake libgtest-dev libboost-test-dev && rm -rf /var/lib/apt/lists/* 

# copy necessary files
COPY ./CMakeLists.txt /usr/src/
COPY ./cmake/. /usr/src/cmake/
COPY ./src/. /usr/src/src/
COPY ./api/. /usr/src/api/

# Install the dependencies
WORKDIR /usr/src/api/scripts/
RUN apt-get -y update
RUN pip3 install -r requirements.txt

# build C++ engine
WORKDIR /usr/src/
RUN mkdir -p /usr/src/build/
WORKDIR /usr/src/build
RUN cmake .. && make

# run the server
WORKDIR /usr/src/api
CMD ["python3", "server.py"]
