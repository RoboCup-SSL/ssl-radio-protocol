//========================================================================
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
//========================================================================
/*!
  \file    server_main.cpp
  \brief   An example robocup ssl shared radio protocol server.
  \author  Joydeep Biswas [joydeepb@ri.cs.cmu.edu], 2015
*/
//========================================================================

#include <string>
#include <arpa/inet.h>
#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "radio_protocol_command.pb.h"
#include "radio_protocol_wrapper.pb.h"

void PrintRadioCommand(const RadioProtocolCommand& cmd) {
  printf("Robot %X : vx=%6.3f vy=%6.3f vr=%6.3f",
         cmd.robot_id(),
         cmd.velocity_x(),
         cmd.velocity_y(),
         cmd.velocity_r());
  if (cmd.has_flat_kick()) {
    printf(" flat_kick=%6.3f", cmd.flat_kick());
  } else if (cmd.has_chip_kick()) {
    printf(" chip_kick=%6.3f", cmd.chip_kick());
  }
  if (cmd.has_dribbler_spin()) {
    printf(" dribbler=%6.3f", cmd.dribbler_spin());
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  int socket_fd = 0;
  int port_number = 10010;

  if (argc > 1) {
    port_number = atoi(argv[1]);
  }

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  sockaddr_in server_address;
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(port_number);
  bind(socket_fd,
       reinterpret_cast<sockaddr*>(&server_address),
       sizeof(server_address));
  sockaddr_in client_address;
  bzero(&client_address, sizeof(client_address));
  RadioProtocolWrapper radio_message;
  while (true) {
    char buffer[65536];
    socklen_t len = sizeof(client_address);
    const int num_bytes = recvfrom(
        socket_fd,
        buffer,
        sizeof(buffer),
        0,
        reinterpret_cast<sockaddr*>(&client_address),
        reinterpret_cast<socklen_t*>(&len));
    const std::string client_name =
        inet_ntoa(client_address.sin_addr);
    if (radio_message.ParseFromArray(buffer, num_bytes)) {
      const int num_robots = radio_message.command_size();
      printf("%d robot command(s) received from %s\n",
             num_robots,
             client_name.c_str());
      for (int i = 0; i < num_robots; ++i) {
        PrintRadioCommand(radio_message.command(i));
      }
      printf("\n");
    } else {
      printf("Error parsing protobuf received from %s\n",
             client_name.c_str());
    }
  }
}
