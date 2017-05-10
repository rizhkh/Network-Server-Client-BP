/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include<pthread.h>  
#include <errno.h>
#include "libnetfiles.h"


int n = -1;			// utility variable - for monitoring reading/writing from/to the socket
static int number = 1;
pthread_mutex_t lock;
static int check = 0; //to keep the threads in balance they dont overlap
	
struct threadstruct
{
	int socket;
	struct sockaddr_in *addr;
	char *buffer;
	int connectionfd;
};
struct threadstruct t[5];

struct fileserverz
{
	int socket;
	int flag;
	int readflag;
	int writeflag;
	int readwriteflag;
	int fd;
	int byte;
	char *filen;
	
};
struct fileserverz fs[5];

void error(char *msg)
{
    perror(msg);
    exit(1);
}



void * multconnecthandler(int socket)
{	
	int nsock = socket;//*(int*)sockfd;
	fs[0].socket = nsock;
	char buffer[256];
	
	{
	printf("Status: A new connection is now connected : \n");
	printf("Connection  : %d \n",number);
	number++;
	}
	
    bzero(buffer,256);
	n = read(nsock,buffer,255);
	
    if (n < 0)
	{
		error("ERROR reading from socket");
	}
			
	handler(buffer);
	
    n = write(nsock,"I got your message",18);
	
    if (n < 0)
	{
		error("ERROR writing to socket");
	}	
	
pthread_mutex_unlock(&lock);
}

int handler(char buff[])
{
	char buffer[256];
	strcpy(buffer,buff);
	decoder(buffer);
	//bzero(buffer,255);
	
	//////////////////////////////
	///			OPEN
	
	if( fs[0].flag == 1 ) 
	{
		// open(const char *path, int oflag,
		printf("pathname in handler : %s",fs[0].filen); //remove this later
		
		//////
		// NOTE : DIRECTORY ERROR OR IF FILE DONT EXISTS | OR IF USING WRITE FLAG WITH NETREAD ERROR
		//////
		
		int fd1;
		
		if (fs[0].readflag==1)fd1= open(fs[0].filen,O_RDONLY);
		
		if (fs[0].writeflag==1)fd1= open(fs[0].filen,O_WRONLY);
		
		if (fs[0].readwriteflag==1)fd1= open(fs[0].filen,O_RDWR);	
				
		if(fd1>0 || fd1==-1)
		fd1=fd1*(-1);
		printf("filedescp: %d \n",fd1);
		write(fs[0].socket,fd1,10);
	}

	//////////////////////////////
	///			READ	
	
	if( fs[0].flag == 2 )  //THIS PART HANDLES JUST READ
	{
		//////
		// NOTE : DIRECTORY ERROR OR IF FILE DONT EXISTS | OR IF USING WRITE FLAG WITH NETREAD ERROR
		//////

		int fd2;
		int nbytes = fs[0].byte;


		//////
		// NOTE : YOOU CAN REMOVE THIS PART NOW 
		//////
		
////////////////////////////////////////////////////////////////////
//REMOVE THIS PART AT THE END
printf("\n--------------------------------------:- \n");
printf("FILE OUTPUT:- \n");
		int c;
		FILE *file;
		file = fopen(fs[0].filen, "r");
		if (file) {
			while ((c = getc(file)) != EOF)
				putchar(c);
			fclose(file);
		}	
printf("\n END OF FILE \n");	
printf("--------------------------------------:- \n");	
////////////////////////////////////////////////////////////////////		
		
		buffer[nbytes] = '\0'; //indicates where to stop after reading those bytes
		
		// PLACES ALL THE FLAGS HERE
		
		if (fs[0].readflag==1)fd2= open(fs[0].filen,O_RDONLY);
		
		if (fs[0].writeflag==1)fd2= open(fs[0].filen,O_WRONLY);
		
		if (fs[0].readwriteflag==1)fd2= open(fs[0].filen,O_RDWR);		
		
		else{
			// ERROR HERE FOR THE FLAG // input can only take r w or rw
		}
		
		//fd2= open(fs[0].filen,O_RDONLY);
		int fd22=fd2;
		if(fd2>0 || fd2==-1)
		fd2=fd2*(-1);
		bzero(buffer,255);
		sprintf(buffer, "%d", fd2);		
		write(fs[0].socket,buffer,10);		
		read(fs[0].socket,buffer,255);	
		ssize_t retbytes = read(fd22,fs[0].filen,nbytes);	
		printf("Bytes read(in handler): %d \n",retbytes);
		//convert INT BYTES to string again to send it back 
		char *byteString = malloc(30);
		sprintf(byteString,"%d",retbytes);
		//trying to send the int STRING byteSTRING to the client side as a message
		bzero(buffer,256);	
		strcpy(buffer,byteString);
		//printf("[buffer: %s]\n",buffer);		
		write(fs[0].socket,buffer,10);	
	}	
	
	//////////////////////////////
	///			WRITE	
	
	if( fs[0].flag == 3 )
	{
		int fd3;
		int nbytes = fs[0].byte;
		buffer[nbytes] = '\0'; //indicates where to stop after writing those bytes
				
		if (fs[0].readflag==1)fd3= open(fs[0].filen,O_RDONLY);
		
		if (fs[0].writeflag==1)fd3= open(fs[0].filen,O_WRONLY);
		
		if (fs[0].readwriteflag==1)fd3= open(fs[0].filen,O_RDWR);		
		
		else{
			// ERROR HERE FOR THE FLAG // input can only take r w or rw
		}		
		
		int fd33=fd3;
		if(fd3>0 || fd3==-1)
		fd3=fd3*(-1);
		bzero(buffer,255);
		sprintf(buffer, "%d", fd3);
		write(fs[0].socket,buffer,10);		
		read(fs[0].socket,buffer,255);	
		
		int ret;
		ret = lseek(fd33,0,SEEK_END);
		if (ret == -1)
		{
			error("ERROR:could not change FD position, lseek");
		}

		int ret2 = lseek(fd33,ret,SEEK_END);
		if (ret == -1)
		{
			error("ERROR:could not change FD position, fseek");
		}
		
		ssize_t writBytes = write(fd33,buffer,nbytes);
		char *byteString = malloc(30);
		sprintf(byteString,"%d",writBytes);	
		bzero(buffer,256);	
		strcpy(buffer,byteString);
		write(fs[0].socket,buffer,10);	
	}	

	//////////////////////////////
	///			CLOSE

	if( fs[0].flag == 4 )
	{
	int i = 6;
	int max = strlen(buffer);
	char* num = (char*)malloc(sizeof(int)*2);
	bzero(num, sizeof(int)*2);
	int j = 0;
	
		while (i < max)
		{
			num[j] = buffer[i];
			j++;
			i++;
		}
		
	int fd  = atoi(num);
	fd=fd*(-1);	
	int n = close(fd);
	
	bzero(num, sizeof(int)*2);
	int newErrno = errno;
	bzero(buffer, 100);
	
	if (n == -1)
	{
		strcat(buffer, "-1;");
		sprintf(num, "%d;", newErrno);
		strcat(buffer, num);
		write(fs[0].socket,buffer,strlen(buffer));	
	}
	
	sprintf(num, "%d", n);
	strcat(buffer, num);
	//printf("buffer: %s\n", buffer);
	write(fs[0].socket,buffer,strlen(buffer));
	}
	
	return 0; 
}


void decoder(const char * pathname)
{
	char buffer[256];
	char *abc = malloc(100);
	strcpy(buffer,pathname);
	strcpy(abc,buffer);


// NOTE : THIS PART IS FOR THE FLAGS
int b=0;
while(buffer[b]!='\0')
{
if( buffer[b]=="~" && (isdigit(buffer[b+1])) )
{
	printf("hooooooaaaaaahhhh \n");
}
	b++;
}

int a=0;
while(buffer[a]!='\0')
{
	if(buffer[a]=='r' && buffer[a-1]==' ')
	{
		fs[0].readflag = 1;
	}
	
	if(buffer[a]=='w' && buffer[a-1]==' ')
	{
		fs[0].writeflag = 1;
	}

	if(buffer[a]=='r' && buffer[a-1]==' ' && buffer[a+1]=='w')
	{
		fs[0].readwriteflag = 1;
	}
	
	a++;
}

if(buffer[0]=='o' || buffer[0]=='O' || buffer[1]=='p' )
{
	fs[0].flag = 1;		//OPEN IS 1
}	
	
else if(buffer[0]=='r' || buffer[0]=='R')
{
	fs[0].flag = 2;		//READ IS 2
}
	
else if(buffer[0]=='w' || buffer[0]=='W' || buffer[1]=='r' || buffer[1]=='R' )
{
	fs[0].flag = 3;		//write IS 3
}	
	
else if(buffer[0]=='C' || buffer[0]=='c')
{
	fs[0].flag = 4;		//CLOSE IS 3
}		
	
else
{
	error("no read-write directed input.Try again.");
}

//buffer0-3 R/W
//buffer5-onwards filename
//--------------*------------RIGHT HERE NEED TO IMPLEMENT DIRECTORY SEARCHER
//ENOENT error too

char pathname2[50];
char pathname3[50];
char * numb = malloc(10);	
int index = 0;
	
if( buffer[0]=='w' || buffer[0]=='W' || buffer[1]=='r' || buffer[1]=='R' ||
buffer[2]=='I' || buffer[2]=='I')
{
int i=6;
int i1=0;
int j=0;
int check = 0;
	
	while(buffer[i]!='\0')
	{
		if( (buffer[i]==' ') && (isdigit(buffer[i+1])) )
		{
			numb[index]=buffer[i+1];
			check = 1;
		}
			
		else if(check==0)
		{
			pathname2[i1]=buffer[i];
			pathname3[j]=pathname2[j];	
		}
		
		i++;
		i1++;
		j++;
	}
}

else if( buffer[0]=='o' || buffer[0]=='O' || buffer[0]=='r' || buffer[0]=='R')
{
	int i=5;
	int i1=0;
	int j=0;
	int check = 0;

	while(buffer[i]!='\0')
	{

		if( (buffer[i]==' ') && (isdigit(buffer[i+1])) )
		{
			numb[index]=buffer[i+1];
			check = 1;
		}
		
		else if(check==0)
		{
					pathname2[i1]=buffer[i];
					pathname3[j]=pathname2[j];	
		}
		i++;
		i1++;
		j++;
	}
}

int numb2 = atoi(numb);
fs[0].byte=numb2;
fs[0].filen = malloc(30);
strcpy(buffer,pathname3);
strcpy(fs[0].filen,pathname3);
}

int main(int argc, char *argv[])
{
	int newsockfd = -1;
	int sockfd = -1;
	int portno = -1;
	int clilen = -1;
	int n = -1;	
	int *new_sock;
	int client_sock;
		
	struct sockaddr_in serverAddressInfo;  //use this in struct
	struct sockaddr_in clientAddressInfo; 

	
	portno = 14354;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
    if (sockfd < 0)	{error("ERROR opening socket");}
	
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	serverAddressInfo.sin_port = htons(portno);
    serverAddressInfo.sin_family = AF_INET;
    serverAddressInfo.sin_addr.s_addr = INADDR_ANY;
  
    if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0)
	{
		error("ERROR on binding");
	}

	//allows 5 pending network connections
    listen(sockfd,5);

    clilen = sizeof(clientAddressInfo);


	int i=0; 


	while(1)
	{
	pthread_mutex_lock(&lock);

		
		newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo,&clilen);	
	
		pthread_t pt[i];

			
		
		printf(" - trying to ");			printf("connect -  \n");		

		

		
		if (pthread_create(&pt[i],NULL,(void*)multconnecthandler,newsockfd) < 0)
		{
			printf("cannot make a thread");
			return 1;
		}
		
		//pthread_join(pt[i], newsockfd);  
		
		//number of connects
		//number = number - 1;
		
		//if(number<=0)
		
	
	pthread_exit(pt);
	
		i++;
	}

    if (newsockfd < 0) 
	{
        error("ERROR on accept");
	}	
   
	return 0; 
}
