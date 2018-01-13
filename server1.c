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
char produse[1000];
int logare = -1;
char logare_user[100];
int verificare_parola(char *logare_user)
{
    char cod[100];
    char ch;
    int nr=0;

    int file=open("utilizatori.txt", O_RDONLY);
    lseek(file, -1L,1);
    while(read(file, &ch,1))
    {
        if(ch!='\n')
            cod[nr++]=ch;
        else
        {
            cod[nr]='\0';
           if(strcmp(logare_user,cod) == 0)
           {
               return 1;
           }
           else
           {
               cod[0]='\0';
               nr = 0;
           }
            
        }
    }
    return -1;
}
int vizualizare_produse(char *categorie)
{
    bzero (produse, 1000);
    produse[0]='\n';
    if(categorie[0]=='a' && categorie[1]=='l' && categorie[2]=='l')
    {
        char ch;
        int nr;
        nr = 1;
        int file=open("Products/AllProducts.txt", O_RDONLY);
        lseek(file, -1L,1);
        while(read(file, &ch,1) && ch!=EOF)
        {
            produse[nr++]=ch;
        }
        produse[strlen(produse)]='\0';
    }
    else
    {
        if(categorie[0]=='c' && categorie[1]=='l' && categorie[2]=='o' && categorie[3]=='t' && categorie[4]=='h' && categorie[5]=='e')
        {
            char ch;
            int nr;
            nr = 1;
            int file=open("Products/Clothes.txt", O_RDONLY);
            lseek(file, -1L,1);
            while(read(file, &ch,1) && ch!=EOF)
            {
                produse[nr++]=ch;
            }
            produse[strlen(produse)]='\0';
        }
        else
        {
            if(categorie[0]=='e' && categorie[1]=='l' && categorie[2]=='e' && categorie[3]=='c' && categorie[4]=='t' && categorie[5]=='r' && categorie[6]=='o' && categorie[7]=='n' && categorie[8]=='i' && categorie[9]=='c')
            {
                char ch;
                int nr;
                nr = 1;
                int file=open("Products/Electronics.txt", O_RDONLY);
                lseek(file, -1L,1);
                while(read(file, &ch,1) && ch!=EOF)
                {
                    produse[nr++]=ch;
                }
                produse[strlen(produse)]='\0';
            }
            else
            {
                if(categorie[0]=='f' && categorie[1]=='o' && categorie[2]=='o' && categorie[3]=='d')
                {
                    char ch;
                    int nr;
                    nr = 1;
                    int file=open("Products/Food.txt", O_RDONLY);
                    lseek(file, -1L,1);
                    while(read(file, &ch,1) && ch!=EOF)
                    {
                        produse[nr++]=ch;
                    }
                    produse[strlen(produse)]='\0';
                }
            }
        }
    }
    return 1;
}
int validare_utilizator(char *utilizator)
{
    char username[100];
    char ch;
    int nr=0;

    int file=open("utilizatori.txt", O_RDONLY);
    lseek(file, -1L,1);
    while(read(file, &ch,1))
    {
        if(ch!=' ' && ch!='\n')
        {
            username[nr++]=ch;
        } 
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
    char msg[1000];
    char comanda[100];
    bzero (msg, 1000);
     
    if(-1 == read (fd, msg, sizeof (msg)))
    {
        perror ("Eroare la read() de la client.\n");
        return 0;
    }
    strcpy(comanda,msg);
    if(logare != 0)
    {
        printf("Comanda: %s\n",comanda);
    }
    
    if(logare == 0)
    {
        strcat(logare_user, " ");
        strcat(logare_user,msg);
        if(-1 == verificare_parola(logare_user))
        {
            strcpy(msg,"Wrong password");
            logare = -1;
        }
        else
        {
            logare =1;
            strcpy(msg,"Successful authentication");
            printf("Un utilizator s-a conectat!\n");
        }
    }
    else
    {
        if(comanda[0]=='l' && comanda[1]=='o' && comanda[2]=='g' && comanda[3]=='i' && comanda[4]=='n' && comanda[5]==' ')
        {
            strcpy(msg,msg+6);
            if(0 == validare_utilizator(msg))
            {
                strcpy(msg,"Invalid user");
                logare_user[0]='\0';
                logare = -1;
            }
            else
            {
                logare = 0;
                strcpy(logare_user,msg);
                printf("A user is trying to connect!\n");
                strcpy(msg,"Password: ");
            }

        }
        else
        {
                if(comanda[0]=='v' && comanda[1]=='i' && comanda[2]=='e' && comanda[3]=='w' && comanda[4]==' ')
                {
                    strcpy(msg,msg+5);
                    if(-1 ==  vizualizare_produse(msg))
                    {
                        return 1;
                    }
                    strcpy(msg,produse);
                }
                else
                {
                    if(strcmp(comanda,"disconnect")==0)
                    {
                        strcpy(msg,"Disconnect successfull");
                        printf("Un utilizator s-a deconectat!\n");
                        logare = -1;
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
