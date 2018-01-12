#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 2056
int sd,client,reuse_socket;
char comanda[2];
/*
void login()
{
    FILE* users;
    char user[32],pass[32];

    memset(user, 0, 32);
    memset(pass,0,32);

    if(read(client, user, 32)<=0)
    {
        perror("[server] Eroare la citire\n");
        close(client);
        exit(-1);
    }

    if(read(client, pass, 32)<=0)
    {
        perror("[server] Eroare la citire\n");
        close(client);
        exit(-1);
    }

    if(users = fopen("users.txt","r"))
    {
        while(!feof(users))
        {
            char line [128];

            memset(line,0,128);

            if(fgets(line,128,users)<=0)
            {
                break;
            }
            else
            {
                
            }
        }
    }

}
void log()
{
    char user[32],pass[32],citire[150];
    file *users;

    memset(user, 0, 32);
    memset(pass,0,32);

    if(read(client, user, 32)<=0)
    {
        perror("[server] Eroare la citire\n");
        close(client);
        exit(-1);
    }

    if(read(client, pass, 32)<=0)
    {
        perror("[server] Eroare la citire\n");
        close(client);
        exit(-1);
    }

    users= fopen("users.txt","a+");

    sprintf(citire, "%s|%s|%d\n",user,pass,type);

    fputs(citire,users);
    fclose(users);


}

void signal_handler(int sign) 
{
	
    while (waitpid(-1, NULL, WNOHANG) > 0);

}*/
int sayHello(int fd)
{
  char buffer[100];		/* mesajul */
  int bytes;			/* numarul de octeti cititi/scrisi */
  char msg[100];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client

  bytes = read (fd, msg, sizeof (buffer));
  if (bytes < 0)
    {
      perror ("Eroare la read() de la client.\n");
      return 0;
    }
  printf ("[server]Mesajul a fost receptionat...%s\n", msg);
      
  /*pregatim mesajul de raspuns */
  bzero(msgrasp,100);
  strcat(msgrasp,"Hello ");
  strcat(msgrasp,msg);
      
  printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
      
  if (bytes && write (fd, msgrasp, bytes) < 0)
    {
      perror ("[server] Eroare la write() catre client.\n");
      return 0;
    }
  
  return bytes;
}
int main()
{
    struct sockaddr_in server;	
    struct sockaddr_in from;

    fd_set readfds;	
    fd_set actfds;

    struct timeval tv;	
    int sd, client;	
    int optval=1; 	
    int fd;		
            
    int nfds;	
    int len;	

    /* creare socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server] Eroare la socket().\n");
        return errno;
    }

    /*setam pentru socket optiunea SO_REUSEADDR */ 
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

    /* pregatim structurile de date */
    bzero (&server, sizeof (server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server] Eroare la bind().\n");
        return errno;
    }

    if (listen (sd, 5) == -1)
    {
        perror ("[server] Eroare la listen().\n");
        return errno;
    }
    
    FD_ZERO (&actfds);		/* initial, multimea este vida */
    FD_SET (sd, &actfds);		/* includem in multime socketul creat */

    tv.tv_sec = 1;		/* se va astepta un timp de 1 sec. */
    tv.tv_usec = 0;
    
    nfds = sd;

    printf ("[server] Asteptam la portul %d...\n", PORT);
    fflush (stdout);
            
    while (1)
    {
        /* ajustam multimea descriptorilor activi (efectiv utilizati) */
        bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

        /* apelul select() */
        if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
        {
            perror ("[server] Eroare la select().\n");
            return errno;
        }
        /* vedem daca e pregatit socketul pentru a-i accepta pe clienti */
        if (FD_ISSET (sd, &readfds))
        {
            len = sizeof (from);
            bzero (&from, sizeof (from));

            client = accept (sd, (struct sockaddr *) &from, &len);

            if (client < 0)
            {
                perror ("[server] Eroare la accept().\n");
                continue;
            }

            if (nfds < client)
            {
                nfds = client;
            } 
                    
            FD_SET (client, &actfds);

            printf("[server] S-a conectat clientul un client");
            fflush (stdout);
        }
        for (fd = 0; fd <= nfds; fd++)	/* parcurgem multimea de descriptori */
        {
        
            if (fd != sd && FD_ISSET (fd, &readfds))
            {	
                if (sayHello(fd))
                {
                    printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
                    fflush (stdout);
                    close (fd);		/* inchidem conexiunea cu clientul */
                    FD_CLR (fd, &actfds);/* scoatem si din multime */
                    
                }
            }			/* for */
        }
    }				/* while */
}				/* main */
