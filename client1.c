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
int login;
char mesaj[1000];
int main ()
{
    int sd;	
    char user[100];
    login = -1;
    struct sockaddr_in server;
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
    while(1)
    {
        bzero (mesaj, 1000);
        fflush (stdout);
        if(login == 1)
        {
            printf("%s: ", user);
        }
        else
        {
            if(login == -1)
                printf("User: ");
        }
        scanf("%[^\n]%*c", mesaj);
        if(mesaj[0]=='l' && mesaj[1]=='o' && mesaj[2]=='g' && mesaj[3]=='i' && mesaj[4]=='n' && mesaj[5]==' ')
        {
            strcpy(user, mesaj);
            strcpy(user,user+6);
        }
        if(mesaj[0]=='l' && mesaj[1]=='o' && mesaj[2]=='g' && mesaj[3]=='i' && mesaj[4]=='n' && mesaj[5]==' ' && login == 1)
        {
            printf("Sunt deja logat :o OMG!!!\n");
        }
        else
        {
            if(strcmp(mesaj,"exit")==0)
            {
                return 1;
            }
            else
            {
                if (write (sd, mesaj, strlen(mesaj)) <= 0)
                {
                    perror ("[client]Eroare la write() spre server.\n");
                    return errno;
                }
                
                bzero (mesaj, 1000);
                if (read (sd, mesaj, sizeof(mesaj)) < 0)
                {
                    perror ("[client]Eroare la read() de la server.\n");
                    return errno;
                }
                
                if(strcmp(mesaj,"Password: ")==0)
                {
                    login = 0;
                    printf("%s",mesaj);
                }
                else
                {
                    if(strcmp(mesaj,"Successful authentication")==0)
                    {
                        login=1;
                    }
                    else
                    {
                        if(strcmp(mesaj,"Wrong password")==0)
                        {
                            user[0]='\0';
                            login = -1;
                        }
                        else
                        {
                            if(strcmp(mesaj,"Invalid user")==0)
                            {
                                user[0]='\0';
                            }
                        }

                    }
                    if(login == 1)
                    {
                        printf("%s: ", user);
                    }
                    else
                    {
                        printf("User: ");
                    }
                    printf("%s\n",mesaj);
                    if(strcmp(mesaj,"Disconnect successfull") == 0)
                    {
                            login=-1;
                    }
                }
                
                
                
            }
        }
        
    }
    

    close (sd);
}