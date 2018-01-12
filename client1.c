/* cliTCP.c - Exemplu de client TCP 
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009

*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

#define Port 2056

extern int inet_addr(char *);
extern int errno;
int port;

int main ()
{
    int sd;	
    struct sockaddr_in server;
    char msg[100];
    /* stabilim portul */
    port = Port;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[client] Eroare la socket().\n");
        return errno;
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons (port);
    
    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
        {
        perror ("[client]Eroare la connect().\n");
        return errno;
        }

    bzero (msg, 100);
    printf ("[client]Introduceti un nume: ");
    fflush (stdout);
    read (0, msg, 100);

    if (write (sd, msg, 100) <= 0)
    {
        perror ("[client]Eroare la write() spre server.\n");
        return errno;
    }

    if (read (sd, msg, 100) < 0)
    {
        perror ("[client]Eroare la read() de la server.\n");
        return errno;
    }
    printf ("[client]Mesajul primit este: %s\n", msg);

    close (sd);
}