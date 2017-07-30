#include <stdio.h>
#include <string.h>
#include <unistd.h> // sleep()

#include "fct_utiles.h"
#include "htmlRequest.h"

//void genHtmlGetHeader(char* out, enum RequestType rqType, char* target, char* host, enum ConnexionType connexion)
void genHtmlGetHeader(char* out, enum RequestType rqType, char target[], char host[], enum ConnexionType connexion)
{
	int error = 0;
	strcpy(out, "");	// Reset out
	// First line //
	switch(rqType){
		case request_GET:
		strcat(out, "GET ");
	break;
		case request_HEAD:
		strcat(out, "HEAD ");
	break;
		case request_POST:
		strcat(out, "POST ");
	break;
		/*case request_OPTIONS:
		strcat(out, "OPTIONS ");
	break;
		case request_CONNECT:
		strcat(out, "CONNECT ");
	break;
		case request_TRACE:
		strcat(out, "TRACE ");
	break;
		case request_PUT:
		strcat(out, "PUT ");
	break;
		case request_DELETE:
		strcat(out, "DELETE ");
	break;*/
		default:
		error++;
	break;
	}


	if(error == 0)
	{
		strcat(out, target);
		strcat(out, " HTTP/1.1\r\n");

		// 2nd line
		strcat(out, "Host: ");
		strcat(out, host);
		strcat(out, "\r\n");
		// 3rd line
		strcat(out, "Connexion: ");
		switch(connexion)
		{
			case HTMLCon_Close:
			strcat(out, "close\r\n\r\n");
		break;
			case HTMLCon_Keep_alive:
			strcat(out, "keep-alive\r\n\r\n");
		break;
			default:
			printf("Error : Step 2\n");
			error++;
		break;
		}
	}
	else { printf("Error : Step 1\n"); }
}

int delHtmlHeader(char* htmlFullReceivedPage)	// Return -1 if an error (no HTML header)
{
	char out[1024*1024];
	//strcut(htmlFullReceivedPage, (char*)"\r\n\r\n", out);
	//if (strcut(htmlFullReceivedPage, (char*)"Azerty", out) == -1)
	if(strcut(htmlFullReceivedPage, (char*)"\r\n\r\n", out) == -1)
	{ return -1; }
	else
	{
		strcpy(htmlFullReceivedPage, out);
		return 0;
	}
}
