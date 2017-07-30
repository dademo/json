#ifndef HTMLREQUEST_H_INCLUDED
#define HTMLREQUEST_H_INCLUDED

enum RequestType{
	request_GET,
	request_HEAD,
	request_POST/*,
	request_OPTIONS,
	request_CONNECT,
	request_TRACE,
	request_PUT,
	request_DELETE*/
};

enum ConnexionType{
	HTMLCon_Close,
	HTMLCon_Keep_alive
};

void genHtmlGetHeader(char* out, enum RequestType rqType, char target[], char host[], enum ConnexionType connexion);

int delHtmlHeader(char* htmlFullReceivedPage);	// Return -1 if an error (no HTML header)

#endif
