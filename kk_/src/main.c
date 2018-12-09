#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifdef MINGW32
   #include <winsock.h>
   #include <winsock2.h>
#else
   #include <sys/un.h>
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <unistd.h>
#endif

#include "CreditUser.h"
#define MAX_USER 3
#define OUT_BUFFER_SIZE 40*1024

typedef struct requestData
{
	float iznos;
	float ucesce;
	char ime1[1024];
	char ime2[1024];
	float kamata1;
	float kamata2;
	float limit1;
	float limit2;
	int usrreport;
	int yearreport;
	int newcalc;

}requestData_t;

void prepareCreditPlan(CCreditUser *usr, int usrNo, int year, char *out)
{

	double *plan;
	double ukupno = year * 12 * usr->getAnuityOnYear(year);

	//creditPlans[usrNo][year] = name;
	plan = usr->getCreditPlanOnYear(year);

	sprintf(out,
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
		"<!DOCTYPE html>	\n\
		<html> \n\
		<body>	\n\
		<h1>Plan Otplate za %d meseci</h1>	\n\
		<p>	\n\
		<br>\n", year*12, year);
	sprintf(out+strlen(out),
			"<table border=\"1\">\n");
	sprintf(out+strlen(out),
			"<tr><td><b>Mesec</td><td><b>Ukupno</td><td><b>Glavnica</td><td><b>Otplata</td><td><b>Rata</td></tr>\n");
	for(int i=0; i < year*12; i++, ukupno-=usr->getAnuityOnYear(year))
		sprintf(out+strlen(out),
		"<tr><td>%d</td><td>%0.1f</td><td>%0.1f</td><td>%0.1f</td><td>%0.1f</td></tr>\n",
			i+1,
			ukupno,
			(i==0) ? usr->getAmmountOnYear(year) : plan[i - 1],
			(i==0) ? usr->getAmmountOnYear(year) - plan[i] : plan[i - 1] - plan[i],
			/*(i == (year*12 - 1)) ? plan[i - 1] : */usr->getAnuityOnYear(year));
	sprintf(out+strlen(out),
			"</table><br><br>\n");

	sprintf(out+strlen(out),
		"<br><a href=\"http://localhost:8887/?newcalc=1\">Napravi Novu Kalkulaciju</a><br>");

	sprintf(out+strlen(out),
		"</p>	\n\
		</body>	\n\
		</html> \n");

}

int displayResult(CCreditUser *usr, requestData_t clientData, char *out)
{
	int creditMatch = 0;

		sprintf(out,
			"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
			"<!DOCTYPE html>	\n\
			<html> \n\
			<body>	\n\
			<h1>Kreditna kalkulacija</h1>	\n\
			<p>	\n\
			UKUPNI IZNOS: %0.0f	<br>\n\
			UCESCE: %0.0f<br>\n", (float)clientData.iznos, (float)clientData.ucesce);

		//2 kredit users
		if(strcmp(clientData.ime2,"") != 0)
		{
			//CStringA creditPlanStr1;
			//CStringA creditPlanStr2;
			sprintf(out+strlen(out),
				"ZAJEDNICKI KREDITNI LIMIT: %0.0f<br><br>\n",clientData.limit1 + clientData.limit2);
			sprintf(out+strlen(out),
				"<table border=\"1\">\n");
			sprintf(out+strlen(out),
					"<tr><td><b>Godina</td><td><b>Korisnik 1</td><td><b>Iznos(Vraceno)</td><td><b>Rata</td><td><b>Kamata</td><td><b>Plan Otplate</td><td><b>Korisnik 2</td><td><b>Iznos(Vraceno)</td><td><b>Rata</td><td><b>Kamata</td><td><b>Plan Otplate</td></tr>\n");
			for (int year = 1; year < 31 ; year++)
			{
					if(clientData.limit2 >= usr[1].getAnuityOnYear(year))
					{
						creditMatch = 1;
						//prepareCreditPlan(&(usr[0]), 0, year, tempDir);
						//prepareCreditPlan(&(usr[1]), 1, year, tempDir);
						//creditPlanStr1 = creditPlans[0][year];
						//creditPlanStr2 = creditPlans[1][year];
						sprintf(out+strlen(out),
							"<tr><td>%d</td><td>%s</td><td>%0.0f(%0.0f)</td><td>%0.1f</td><td>%0.2f</td><td><a href=\"%s%d\">%d_meseci</a></td><td>%s</td><td>%0.0f(%0.0f)</td><td>%0.1f</td><td>%0.2f</td><td><a href=\"%s%d\">%d_meseci</a></td></tr>\n",
							year,
							usr[0].name,
							usr[0].getAmmountOnYear(year),
							year * 12 * usr[0].getAnuityOnYear(year),
							usr[0].getAnuityOnYear(year),
							usr[0].getRateOnYear(year),
							"http://localhost:8887/?usrreport=0&yearreport=",year,
							year*12,
							usr[1].name , usr[1].getAmmountOnYear(year),
							year * 12 * usr[1].getAnuityOnYear(year),
							usr[1].getAnuityOnYear(year),
							usr[1].getRateOnYear(year),
							"http://localhost:8887/?usrreport=1&yearreport=",year,
							year*12);
					}
			}

			sprintf(out+strlen(out),
				"</table><br>\n");

			sprintf(out+strlen(out),
				"<table border=\"1\">\n");
			sprintf(out+strlen(out),
				"<tr><td><b>Godina</td><td><b>Korisnik</td><td><b>Iznos(Vraceno)</td><td><b>Rata</td><td><b>Kamata</td></tr>\n");
			for (int year = 1; year < 31 ; year++)
				if(clientData.limit2 >= usr[1].getAnuityOnYear(year))
					sprintf(out+strlen(out),
					"<tr><td>%d</td><td>Zajedno</td><td>%0.0f(%0.0f)</td><td>%0.1f</td><td>%0.2f</td></tr>\n",
					year,
					usr[2].getAmmountOnYear(year),
					usr[2].getAnuityOnYear(year) * year *12,
					usr[2].getAnuityOnYear(year),
					usr[2].getRateOnYear(year) );
			sprintf(out+strlen(out),
				"</table><br>\n");
		}
		//1 kredit user
		else
		{
			//CStringA creditPlanStr;
			sprintf(out+strlen(out),
				"KREDITNI LIMIT: %0.0f<br><br>\n",clientData.limit1);
			sprintf(out+strlen(out),
				"<table border=\"1\">\n");
			sprintf(out+strlen(out),
				"<tr><td><b>Godina</td><td><b>Korisnik</td><td><b>Iznos(Vraceno)</td><td><b>Rata</td><td><b>Kamata</td><td><b>Plan Otplate</td></tr>\n");
			for (int year = 1; year < 31 ; year++)
				if((clientData.limit1 >= usr[0].getAnuityOnYear(year)) && ((clientData.iznos - clientData.ucesce) <= usr[0].getAmmountOnYear(year)))
				{
					creditMatch = 1;
					//prepareCreditPlan(&(usr[0]), 0, year, tempDir);

					//creditPlanStr = creditPlans[0][year];
					sprintf(out+strlen(out),
					"<tr><td>%d</td><td>%s</td><td>%0.0f(%0.0f)</td><td>%0.1f</td><td>%0.2f</td><td><a href=\"%s%d\">%d_meseci</a></td></tr>\n",
					year,
					usr[0].name,
					usr[0].getAmmountOnYear(year),
					year * 12 * usr[0].getAnuityOnYear(year),
					usr[0].getAnuityOnYear(year),
					usr[0].getRateOnYear(year),
					"http://localhost:8887/?usrreport=0&yearreport=",year,
					year*12);
				}
			sprintf(out+strlen(out),
				"</table><br><br>\n");
		}
		if(!creditMatch)
		{
			sprintf(out+strlen(out),"Kreditni uslovi ne zadovoljavaju. Pokusajte ponovo sa manjom sumom.<br>");
		}

		sprintf(out+strlen(out),
			"<br><a href=\"http://localhost:8887/?newcalc=1\">Napravi Novu Kalkulaciju</a><br>");

		sprintf(out+strlen(out),
			"</p>	\n\
			</body>	\n\
			</html> \n");

	return 0;
}
int parseClientRequest(char* request, requestData_t *data)
{
	char *ptr;
	int index;
	//iznos

	if((ptr = strstr(request, "iznos")) != NULL)
	{
		ptr += strlen("iznos") + 1;
		(*data).iznos = atof(ptr);
	}

	if((ptr = strstr(request, "ucesce")) != NULL)
	{
		ptr += strlen("ucesce") + 1;
		(*data).ucesce = atof(ptr);
	}

	if((ptr = strstr(request, "Ime1")) != NULL)
	{
		ptr += strlen("Ime1") + 1;
		strcpy((*data).ime1, ptr);
		ptr = strchr((*data).ime1, '&');
		*ptr = '\0';
	}

	if((ptr = strstr(request, "Ime2")) != NULL)
	{
		ptr += strlen("Ime2") + 1;
		strcpy((*data).ime2, ptr);
		ptr = strchr((*data).ime2, '&');
		*ptr = '\0';
	}

	if((ptr = strstr(request, "Kamata1")) != NULL)
	{
		ptr += strlen("Kamata1") + 1;
		(*data).kamata1 = atof(ptr);
	}

	if((ptr = strstr(request, "Kamata2")) != NULL)
	{
		ptr += strlen("Kamata2") + 1;
		(*data).kamata2 = atof(ptr);
	}

	if((ptr = strstr(request, "Limit1")) != NULL)
	{
		ptr += strlen("Limit1") + 1;
		(*data).limit1 = atof(ptr);
	}

	if((ptr = strstr(request, "Limit2")) != NULL)
	{
		ptr += strlen("Limit2") + 1;
		(*data).limit2 = atof(ptr);
	}

	if((ptr = strstr(request, "usrreport")) != NULL)
	{
		ptr += strlen("usrreport") + 1;
		(*data).usrreport = atoi(ptr);
	}

	if((ptr = strstr(request, "yearreport")) != NULL)
	{
		ptr += strlen("yearreport") + 1;
		(*data).yearreport = atoi(ptr);
	}

	if((ptr = strstr(request, "newcalc")) != NULL)
	{
		ptr += strlen("newcalc") + 1;
		(*data).newcalc = atoi(ptr);
	}

	return 0;
}
int main()
{

#ifdef MINGW32
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(1,1), &wsadata) == SOCKET_ERROR) {
		printf("Error creating socket.");
		return -1;
	}
#endif

int listen_fd;
requestData_t clientData;
char content[2048];
char out[OUT_BUFFER_SIZE];
CCreditUser usr[MAX_USER];

clientData.iznos = 0;
clientData.ucesce = 0;
clientData.ime1[0] = '\0';
clientData.ime2[0] = '\0';
clientData.kamata1 = 0;
clientData.kamata2 = 0;
clientData.limit1 = 0;
clientData.limit2 = 0;
clientData.usrreport = 0;
clientData.yearreport = 0;
clientData.newcalc = 0;

#ifdef MINGW32
 ShellExecuteA( NULL, "open", "explorer.exe", "http://localhost:8887/", NULL, 1 );
#endif

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		puts("Error opening socket.");
		return -1;
	}

struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(8887);

struct sockaddr_in client_addr;
#ifdef MINGW32
int client_addr_length = sizeof(client_addr);
#else
socklen_t client_addr_length;
#endif

int on = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on,	sizeof(on));

	if ((bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
	{
		puts("Error binding socket.");
		return -1;
	}

	if ((listen(listen_fd, 64)) < 0)
	{
		puts("Error listening on socket.");
		return -1;
	}

	while (1)
	{

		int socket_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_length);

		char request[32768];
		memset(request,'\0',32768);
#ifdef MINGW32
		recv(socket_fd, request, 32768, 0);
#else
		read(socket_fd, request, 32768);
#endif

		puts(request);

		clientData.usrreport = 0;
		clientData.yearreport = 0;

		parseClientRequest(request, &clientData);

		if(clientData.newcalc == 1)
		{
			clientData.iznos = 0;
			clientData.ucesce = 0;
			clientData.ime1[0] = '\0';
			clientData.ime2[0] = '\0';
			clientData.kamata1 = 0;
			clientData.kamata2 = 0;
			clientData.limit1 = 0;
			clientData.limit2 = 0;
			clientData.newcalc = 0;
		}

		sprintf(content, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
				"<form action=\"http://localhost:8887/\" method=post>"
				"<fieldset>"
					"<legend><font size=\"4\" color=\"blue\">Mali kreditni kalkulator</font></legend>"
					"<p>"
						"Iznos:<input type=text name=iznos value=%d><br>"
						"Ucesce:<input type=text name=ucesce value=%d>"
					"</p>""<br>"
					"<p>"
					"<table border=\"1\">"
						"<tr>"
							"<td><font size=\"3\" color=\"blue\">Korisnik 1</font></td>"
							"<td><font size=\"3\" color=\"blue\">Korisnik 2(solidarni duznik)</font></td>"
						"</tr>"
						"<tr>"
							"<td>Ime:<input type=text name=Ime1 value=%s></td>"
							"<td>Ime:<input type=text name=Ime2 value=%s></td>"
						"</tr>"
						"<tr>"
							"<td>Kamata(%c):<input type=text name=Kamata1 value=%0.2f></td>"
							"<td>Kamata(%c):<input type=text name=Kamata2 value=%0.2f></td>"
						"</tr>"
						"<tr>"
							"<td>Limit:<input type=text name=Limit1 value=%0.1f></td>"
							"<td>Limit:<input type=text name=Limit2 value=%0.1f></td>"
						"</tr>"
					"</p>"
					"</table>""<br>"
					"<input type=submit value=Racunaj>"
				"</form>"
				"</fieldset>""<br>"
				"<small>"
					"*  Korisnik 2 ostaviti prazno ukoliko se racuna za jednog korisnika(bez solidarnog duznika)<br>"
					"** Limit obicno iznosi 40%c mesecnih primanja<br>"
				"</small>", 	(int)clientData.iznos,
								(int)clientData.ucesce,
								clientData.ime1,
								clientData.ime2,
								0x25,clientData.kamata1,
								0x25,clientData.kamata2,
								clientData.limit1,
								clientData.limit2,
								0x25);
#ifdef KK_DEBUG
		printf("PARSED CLIENT DATA : iznos:%d ucesce:%d\n", (int)clientData.iznos, (int)clientData.ucesce);
		printf("PARSED CLIENT DATA : ime1:%s ime2:%s\n", clientData.ime1, clientData.ime2);
		printf("PARSED CLIENT DATA : kamata1:%f kamata2:%f\n", clientData.kamata1, clientData.kamata2);
		printf("PARSED CLIENT DATA : limit1:%f limit2:%f\n", clientData.limit1, clientData.limit2);
		printf("PARSED CLIENT DATA : usrreport:%d yearreport:%d\n", clientData.usrreport, clientData.yearreport);
#endif

		usr[0].AddName(clientData.ime1);
		usr[0].rate = ((clientData.kamata1 > 100) || (clientData.kamata1 < 0)) ? 1 : clientData.kamata1;//limit input
		usr[0].anuity_limit = clientData.limit1;
		usr[1].AddName(clientData.ime2);
		usr[1].rate = ((clientData.kamata2 > 100) || (clientData.kamata2 < 0)) ? 1 : clientData.kamata2;//limit input
		usr[1].anuity_limit = clientData.limit2;
		usr[2].AddName((char*)"zajedno");
		usr[2].rate = 1;
		usr[2].anuity_limit = 1;

		// calculate
		// 2 credit users
		if(strcmp(clientData.ime2,"") != 0)
		{
			usr[0].calculateTotalAmmountonOnRate();
			usr[0].calculateTotalAnuityOnRate();
			usr[0].calculateRateOnAmmount();
			usr[1].setTotalAmmount(clientData.iznos - clientData.ucesce);
			usr[1].bindUserOnAmmount(&(usr[0]));
			usr[1].calculateTotalAnuityOnRate();
			usr[1].calculateRateOnAmmount();
			usr[2].setTotalAmmount(clientData.iznos - clientData.ucesce);
			for (int year = 1; year < MAX_YEARS ; year++)
				usr[2].setAnuityOnYear(year, usr[0].getAnuityOnYear(year) + usr[1].getAnuityOnYear(year));
			usr[2].calculateRateOnAmmount();
		}
		// 1 credit user
		else
		{
			usr[0].setTotalAmmount(clientData.iznos - clientData.ucesce);
			usr[0].calculateTotalAnuityOnRate();
			usr[0].calculateRateOnAmmount();
		}

		memset(out,'\0',OUT_BUFFER_SIZE);
		if(clientData.yearreport == 0)
		{
			displayResult(usr, clientData, out);
		}
		else
		{
			prepareCreditPlan(&(usr[clientData.usrreport]), clientData.usrreport, clientData.yearreport, out);
		}

#ifdef MINGW32
		send(socket_fd, (clientData.iznos == 0) ? content : out, (clientData.iznos == 0) ? strlen(content) : strlen(out), 0);
		closesocket(socket_fd);
#else
		write(socket_fd, (clientData.iznos == 0) ? content : out, (clientData.iznos == 0) ? strlen(content) : strlen(out));
		close(socket_fd);
#endif

	}
#ifdef MINGW32
		closesocket(listen_fd);
		WSACleanup();
#endif
		return 0;
}
