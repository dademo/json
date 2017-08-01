#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <math.h>
#include <string.h>

#include <sys/socket.h>	// socket()
#include <netinet/in.h>
#include <arpa/inet.h>	// htons
#include <netdb.h>	// hostent struct -> getting IP with DNS

#include <unistd.h>	// write(), usleep()

#include "htmlRequest.h"
#include "json.h"

#define uchar unsigned char
#define ushort unsigned short
#define uint unsigned int
#define ulong unsigned long

// voir getopt
// voir assert
// voir abort

void argcargv(int argc, char** argv)
{
	printf("-----Args-----\n");
	for(int i=0; i<argc; i++)
	{ printf("%d.\t%s\n", i, argv[i]); }
	printf("\n");
}

int getAddressWithDNS(char* hostname, struct in_addr* out)
{
	printf("Acquisition de l'adresse IP ... ... ... ");
	struct hostent* host = gethostbyname(hostname);
	printf("Fait !\n");
	//printf("%s", inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
	//if(host->h_addr_list[0] != 0){ return inet_addr(host->h_addr_list[0]); }
	if(host->h_addr_list[0] != 0){ *out = *((struct in_addr *)host->h_addr_list[0]); return 0; }
	else { return -1; }
}


int main(int argc, char** argv)
{
	argcargv(argc, argv);
	
	// Declarations

	char dnsTarget[] = "api.geonames.org";
	//char dnsTarget[] = "api.openweathermap.org";
	//char dnsTarget[] = "localhost";
	int sockHttpClient = 0;
	//char buff[4096] = "";
	char* buff = (char*)malloc(1024*1024*sizeof(char));	// 1Mo
	char* finalmsg = (char*)malloc(1024*1024*sizeof(char));	// 1Mo
	strcpy(finalmsg, "");
	ssize_t receivedSize = 0;

	struct sockaddr_in netTarget;
		netTarget.sin_family = AF_INET;
		netTarget.sin_port = htons(80);	// web
		//netTarget.sin_addr = getAddressWithDNS(dnsTarget, netTarget.sin_addr);
		getAddressWithDNS(dnsTarget, &netTarget.sin_addr);

	//printf("--%s--\n", inet_ntoa(*((struct in_addr*) &netTarget.sin_addr)));

	sockHttpClient = socket(AF_INET, SOCK_STREAM, 0);
	if(sockHttpClient == -1)
	{
		perror("Creation socket");
		exit(-1);
	}

	//char* temp = (char*) &netTarget;
	/*for(ulong i=0; i < sizeof(netTarget); i++)
	{
		putchar(*(((char*) &netTarget)+i) );
	}*/

	if(connect(sockHttpClient, (struct sockaddr*) &netTarget, sizeof(netTarget)) < 0)
	{
		perror("sockHttpClient");
		exit(-1);
	}
	else
	{
		//genHtmlGetHeader(buff, request_GET, (char*) "/index.php", (char*) "localhost", HTMLCon_Close);
		genHtmlGetHeader(buff, request_GET, (char*) "/search?username=dademo&name=Genetouze&type=json", (char*) "api.geonames.org", HTMLCon_Close);
		//genHtmlGetHeader(buff, request_GET, (char*) "/data/2.5/weather?q=Mions,fr&appid=8d616429ad6ed72eb87ea6efe9aa17f1", (char*) "api.openweathermap.org", HTMLCon_Close);
		//genHtmlGetHeader(buff, request_GET, (char*) "/search?username=dademo&name=Genetouze&type=json", (char*) "api.geonames.org", HTMLCon_Keep_alive);
		//genHtmlGetHeader(buff, request_GET, (char*) "/index.php", (char*) "localhost", HTMLCon_Keep_alive);
		printf("Html header :\n%s", buff);
	}
	// Sending the request
	write(sockHttpClient, buff, strlen(buff));

	//sleep(1);

	//ssize_t receivedSize1 = read(sockHttpClient, buff, 1024*1024*sizeof(char));
	int alreadyReceived = 0;
	do
	{
		//usleep(500000);
		usleep(500 * 1000);	// 500 ms = 1/2s
		//sleep(1);
		//strcpy(buff, "");	// Reset buffer
		//buff[0] = '\0';
		//receivedSize = recv(sockHttpClient, buff, 1024*1024*sizeof(char), 0);
		//receivedSize = recv(sockHttpClient, buff, 1024*1024*sizeof(char), 0);
		receivedSize = recv(sockHttpClient, buff, 1024*1024*sizeof(char), MSG_DONTWAIT);

		alreadyReceived = (receivedSize != 0)? 1: 0;	// if receivedSize != 0 -> received smthg, transmission beginned

		buff[receivedSize] = '\0';

		strcat(finalmsg, buff);

		//strcat(finalmsg, buff);
		//printf("%s", buff);
	//ssize_t receivedSize2 = read(sockHttpClient, buff, sizeof(buff));
	//ssize_t receivedSizebis = read(sockHttpClient, buff, sizeof(buff));
	//ssize_t receivedSize = recv(sockHttpClient, buff, sizeof(buff), 0);
	} while (receivedSize >  0 && alreadyReceived == 0);

	//printf("%s", finalmsg);

	if(delHtmlHeader(finalmsg) != 0)
	{
		printf ("Error (delHtmlHeader)");
		exit(-1);
	}

	printf("finalmsg:\n%s\n\n", finalmsg);


	struct json* myRESTservice = mkJsonData(finalmsg);

	//readAllJsonData(myRESTservice);

	//getJsonData(myRESTservice, "coucou.coucou1[1].coucou2.testeuh");
	struct jsonData test = getJsonData(myRESTservice, "totalResultsCount");
	printf("Result:\n\tname : %s\n\tdata : %s\n", test.name, test.data);
	struct jsonData test2 = getJsonData(myRESTservice, "geonames[1].geonameId");
	printf("Result:\n\tname : %s\n\tdata : %s\n", test2.name, test2.data);

	close(sockHttpClient);
	free(buff);

	return 0;
}
