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
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <time.h> 

#define PORT 2056

int validare_utilizator(char *utilizator)
{
    char username[100];
    char ch;
    int nr=0;

    int file=open("utilizatori.txt", O_RDONLY);
    lseek(file, -1L,1);
    while(read(file, &ch,1))
    {
        if(ch!='\n')
            username[nr++]=ch;
        else
        {
            username[nr]='\0';
            if(strcmp(utilizator,username)==0)
                return 1;
            nr=0;
            username[0]='\0';
        }
    }
    return 0;
}
int verificare_comanda(int fd)
{		
    char msg[100];
    char comanda[100];
    bzero (msg, 100);
     
    if(-1 == read (fd, msg, sizeof (msg)))
    {
        perror ("Eroare la read() de la client.\n");
        return 0;
    }
    strcpy(comanda,msg);
    if(comanda[0]=='l' && comanda[1]=='o' && comanda[2]=='g' && comanda[3]=='i' && comanda[4]=='n' && comanda[5]==' ')
    {
        strcpy(msg,msg+6);
        if(0 == validare_utilizator(msg))
        {
            strcpy(msg,"Utilizator nevalid");
        }
        else
        {
            printf("Utilizator logat: %s\n",msg);
            strcpy(msg,"Logare cu succes");
        }

    }
    else
    {
        if(strcmp(comanda,"deconectare")==0)
        {
            strcpy(msg,"Deconectare cu succes");
        }
        else
        {
           if(strcmp(comanda,"exit")==0)
            {
                strcpy(msg,"Decuplat de la server");
            }
            else
            {
                strcpy(msg,"Comanda nerecunoscuta");

            } 
        }
    }
        
    if (write (fd, msg, strlen(msg)) < 0)
    {
        perror ("[server] Eroare la write() catre client.\n");
        return 0;
    }
    if(strcmp(comanda,"exit")==0)
        return -1;
    
    return 1;
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

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server] Eroare la socket().\n");
        return errno;
    }

    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

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
    
    FD_ZERO (&actfds);		
    FD_SET (sd, &actfds);	

    tv.tv_sec = 1;		
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

            printf("[server] S-a conectat clientul un client\n");
            fflush (stdout);
        }
        for (fd = 0; fd <= nfds; fd++)	/* parcurgem multimea de descriptori */
        {
        
            if (fd != sd && FD_ISSET (fd, &readfds))
            {	
                if (-1 ==verificare_comanda(fd))
                {
                    printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
                    fflush (stdout);
                    close (fd);		/* inchidem conexiunea cu clientul */
                    FD_CLR (fd, &actfds);/* scoatem si din multime */
                   return 1;
                }
            }			/* for */
        }
    }				/* while */
}				/* main */
