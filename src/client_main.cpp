//========================================================================
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and neighboring rights to this software to the public domain
// worldwide. This software is distributed without any warranty.
// You should have received a copy of the CC0 Public Domain Dedication along
// with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
//========================================================================
/*!
  \file    client_main.cpp
  \brief   An example robocup ssl shared radio protocol client.
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

float Deg2Rad(float deg) {
  return (M_PI * deg / 180.0f);
}

void ClearCommandProtbuf(RadioProtocolCommand* cmd_ptr) {
  RadioProtocolCommand& cmd = *cmd_ptr;
  cmd.set_robot_id(0);
  cmd.set_velocity_x(0);
  cmd.set_velocity_y(0);
  cmd.set_velocity_r(0);
}

void FillCommandProtobuf(uint32_t robot_id,
                         float velocity_x,
                         float velocity_y,
                         float velocity_r,
                         RadioProtocolCommand* cmd_ptr) {
  RadioProtocolCommand& cmd = *cmd_ptr;
  cmd.set_robot_id(robot_id);
  cmd.set_velocity_x(velocity_x);
  cmd.set_velocity_y(velocity_y);
  cmd.set_velocity_r(velocity_r);
}

int main(int argc, char *argv[]) {
  int socket_fd = 0;
  int robot_id = 0;
  const char* server_name = "127.0.0.1";
  int port_number = 10010;

  if (argc > 1) {
    server_name = argv[1];
  }
  if (argc > 2) {
    port_number = atoi(argv[2]);
  }
  if (argc > 3) {
    robot_id = atoi(argv[3]);
  }

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  sockaddr_in server_address;
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(server_name);
  server_address.sin_port = htons(port_number);
  RadioProtocolWrapper wrapper;
  RadioProtocolCommand command;
  std::string buffer;
  while(true) {
    wrapper.clear_command();
    command.Clear();
    ClearCommandProtbuf(&command);
    FillCommandProtobuf(robot_id, 0, 0, Deg2Rad(30.0), &command);
    *wrapper.add_command() = command;
    wrapper.SerializeToString(&buffer);
    sendto(socket_fd,
           buffer.data(),
           buffer.length(),
           0,
           reinterpret_cast<sockaddr*>(&server_address),
           sizeof(server_address));
    usleep(16000);
  }
}
