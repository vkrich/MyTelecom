//Server auction

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define PORT 7500
#define BUF_SIZE 1000
#define TITLES "titles.txt"

void SendToClient (int socket, char* message); //Send something to client
void SentErrServer(char *s); //error handling

int main( void )
{

    users =(char*) malloc (sizeof(char));//выделяет блок памяти, размером sizeof(char) байт, и возвращает указатель на начало блока
    if (users == NULL)
    {
        perror( "Neydalos' videlit' pamyat'" );
        exit(1);
    }

    printf("Server auction is working...\n");
    //Initialization
    struct sockaddr_in local, si_other;
    int s1, rc, s, slen=sizeof(si_other);
    //fill local
    local.sin_family = AF_INET;// Internet Protocol v4 addresses
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl( INADDR_ANY );//ip
    //make socket
    s = socket( AF_INET, SOCK_STREAM, 0 );//дескриптор созд сокета
    if ( s < 0 )
        SentErrServer("Socket call failed");
    //attach port
    rc = bind( s, ( struct sockaddr * )&local, sizeof(local) );//декскриптор соединения
    if ( rc < 0 )
        SentErrServer("Bind call fail");

//Working
char pick = '0';
while (1)
{
    switch (pick)
    {
        case '0':
        {
            zero_menu_send : SendToClient ((int) socket, "Hello!\nPlease, select option\n"
                                     "1.See lot titles\n"
                                     "2.New lot *only for manager*\n"
                                     "3.See online users\n4.Exit\n");
            zero_menu: rc = recv( (int)socket, buf, BUF_SIZE, 0 );
            if ( rc <= 0 )
                    SentErrServer("Recv call failed when pick was 0");

            pick = buf[0];//Catch
            break;
        }
        case '1'://See lot titles
        {
            char out[BUF_SIZE] = "If you want to open lot, type name of the lot\n If you want to delete lot - type 1(*only for manager*)\n";
            char name[BUF_SIZE] = {NULL};
            strcat (out, themes_names);
            SendToClient ((int) socket, out);//send tlot names
            rc = recv( (int)socket, buf, BUF_SIZE, 0 );//Reading new point of menu
            if ( rc <= 0 )
                    SentErrServer("Recv call failed");
            //If client want to back
            if (buf[0]=='0')
            {
                   pick ='0';
                   break;
            }

            if (buf[0]=='1')
            {
                if (is_manager==false)
                    goto zero_menu_send;

            SendToClient ((int) socket, "Type name of the lot\n");
            rc = recv( (int)socket, name, BUF_SIZE, 0 );
            if ( rc <= 0 )
                    SentErrServer("Recv call failed");

            DeleteLot (name);
            SendResults (name);
            SeeTitles ();//refresh
            goto zero_menu_send;
            break;
            }

            //reading
            {
                char filename[]="";
                strcat(filename,buf);
                strcat(filename,".txt");

                if (SeeMessages (filename, (int) socket)==1)
                {
                    goto zero_menu_send;
                }
                char buf2[ BUF_SIZE ]; //New buffer only for writing message
                rc = recv( (int)socket, buf2, BUF_SIZE, 0 );//Reading new message to write
                if ( rc <= 0 )
                    SentErrServer("Recv call failed");
                if (buf2[0]=='0') //If client want to back
                    goto zero_menu_send;
                //If client want to write something
                else
                {
                     WriteMessages (filename, (int) socket,  buf2);
                     goto zero_menu;
                }
            }
        }
        case '2'://New lot
        {
            char name[ BUF_SIZE ];
            char price[ BUF_SIZE ];

            if (is_manager==false)
               goto zero_menu_send;

            SendToClient ((int) socket, "Please write name of the lot\n");
            rc = recv( (int)socket, name, BUF_SIZE, 0 );
            if ( rc <= 0 )
                    SentErrServer("Recv call failed");
            printf("%s name\n",name);

            SendToClient ((int) socket, "Please write price of the lot\n");
            rc = recv( (int)socket, price, BUF_SIZE, 0 );
            if ( rc <= 0 )
                    SentErrServer("Recv call failed");
            printf("%s price\n",price);
            printf("%s %s name\n",name,price);
            NewLot(name, price,(int) socket);
            goto zero_menu;
            break;

        }
        case '3'://see online users
        {
            WhoIsOnline ((int)socket);
            //Get new point of menu
            rc = recv( (int)socket, buf, BUF_SIZE, 0 );
            if ( rc <= 0 )
                    SentErrServer("Recv call failed");
            pick = buf[0];
            break;
        }

        default://if client type illegal point in main menu
        {
            SendErrorToClient((int) socket);
            pick = '0';
            buf[0] =='0';
        }
    }
}
//Disconnect client
printf("Disconnect client");
printf("\n");
threads --;
pthread_exit(NULL);
}

void SendToClient (int socket, char* message)
{
    int rc;
    rc = send(socket, message, BUF_SIZE, 0 );
        if ( rc <= 0 )
        perror( "send call failed" );
}

void SentErrServer(char *s)
{
    perror(s);
    exit(1);
}
