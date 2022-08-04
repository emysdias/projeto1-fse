#include "../inc/socket.h"

int main()
{
  int opt = TRUE;
  int master_socket, addrlen, new_socket, client_socket[4], max_clients = 4, activity, valread, sd, i;
  int max_sd;
  struct sockaddr_in address;
  char buffer[MAX];

  fd_set readfds;

  for (i = 0; i < max_clients; i++)
  {
    client_socket[i] = 0;
  }

  master_socket = configureSocket();

  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  bzero(&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);

  bindSocket(master_socket, address);
  listenSocket(master_socket);

  addrlen = sizeof(address);

  while (TRUE)
  {
    FD_ZERO(&readfds);

    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

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

    if (FD_ISSET(master_socket, &readfds))
    {
      if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
      {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      for (i = 0; i < max_clients; i++)
      {
        if (client_socket[i] == 0)
        {
          client_socket[i] = new_socket;
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
          int countVelocity = buffer[2];
          int carLength = buffer[3];

          printf("Fluxo de trânsito nas vias principais: %d carros\n", carLength);
          printf("Velocidade media da via: %d hm/h\n", countVelocity);
          printf("Quantidade de carros com velocidade acima de 60 km/h: %d\n", aboveTheAllowedSpeed);
          printf("Quantidade de carros com avanço no sinal vermelho: %d\n\n", redLightAdvance);
          bzero(buffer, MAX);
        }
      }
    }
  }

  return 0;
}