# ssl-radio-protocol

A proposed shared radio protocol for the SSL

This repository provides the protobuf definitions for the shared radio protocol
technical challenge of the Small Size League at RoboCup 2015.
Also included is source code to implement a simple client and server to
communicate using the specified protocol.

For details regarding the protobuf definitions, see [protobuf](protobuf).

## Requirements

Apart from the usual compiler toolchain and cmake, you will need the Google protobuf
compiler and libraries to compile the code.
On Ubuntu or any of its variants, the following command should install the
dependencies:
```bash
sudo apt-get install gcc make cmake libprotobuf-dev protobuf-compiler
```

## Compilation

Run `make` .

## Running the Examples

Run the server with the optional port number argument:
```bash
./bin/server [port_number]
```
Run the client with the optional server, port, and robot ID arguments:
```bash
./bin/client [server [port_number [robot_id]]]
```
