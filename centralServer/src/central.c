#include "../inc/socket.h"

int main()
{
  int opt = TRUE;
  int sockfd, len, i;
  int master_socket, addrlen, new_socket, client_socket[4], max_clients = 4, activity, valread, sd;
  int max_sd;
  struct sockaddr_in address;
  char buffer[MAX];

  fd_set readfds;

  for (i = 0; i < max_clients; i++)
  {
    client_socket[i] = 0;
  }

  sockfd = configureSocket();

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  bzero(&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  bindSocket(sockfd, address);
  listenSocket(sockfd);

  addrlen = sizeof(address);

  while (TRUE)
  {
    FD_ZERO(&readfds);

    FD_SET(sockfd, &readfds);
    max_sd = sockfd;

    for (i = 0; i < max_clients; i++)
    {
      sd = client_socket[i];

      if (sd > 0)
        FD_SET(sd, &readfds);

      if (sd > max_sd)
        max_sd = sd;
    }

    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR))
    {
      printf("select error");
    }

    if (FD_ISSET(sockfd, &readfds))
    {
      if ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
      {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      for (i = 0; i < max_clients; i++)
      {
        if (client_socket[i] == 0)
        {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n", i);

          break;
        }
      }
    }

    for (i = 0; i < max_clients; i++)
    {
      sd = client_socket[i];

      if (FD_ISSET(sd, &readfds))
      {
        if ((valread = read(sd, buffer, MAX)) == 0)
        {
          close(sd);
          client_socket[i] = 0;
        }

        else
        {
          int aboveTheAllowedSpeed = buffer[0];
          int redLightAdvance = buffer[1];
          printf("Quantidade de Carros: %d\n", aboveTheAllowedSpeed);
          printf("Velocidade média: %d\n", redLightAdvance);
          bzero(buffer, MAX);
        }
      }
    }
  }

  return 0;
}