#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>

#define RCVBUFSIZE 32   /* Size of receive buffer */
int searchdata(long key, char *dp, int size);
long id;

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
  char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
  int recvMsgSize;                    /* Size of received message */
  char money[256];

  /* Receive message from client */
  
  if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
    DieWithError("recv() failed");
  /* Echo message back to client */
  /* Send received string and receive again until end of transmission */
  while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
      id = (long)echoBuffer;
      searchdata(id, money ,RCVBUFSIZE);
      if (send(clntSocket, money, recvMsgSize, 0) != recvMsgSize)
	DieWithError("send() failed");

      /* See if there is more data to receive */
      if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
	DieWithError("recv() failed");
    }

  close(clntSocket);    /* Close client socket */


}

int searchdata(long key,char *dp,int size) {
  FILE *p;
  long ydata,ynum;
  char line[256];

  p=fopen("yokinfile","r");
  while(fgets(line,sizeof(line),p)!=NULL) {
    if (sscanf(line,"%ld %ld",&ynum,&ydata)==2) {
      if (key==ynum) {
        snprintf(dp,size,"%ld",ydata);
        fclose(p);
        return 0;
      }
    }
  }
  fclose(p);
  return EOF;
}