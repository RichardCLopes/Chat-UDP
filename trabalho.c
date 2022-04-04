#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <json-c/json.h>
#include <ifaddrs.h>
#include <pthread.h>
#define SIZE 2000

void *receba(void *arg)
{
int sockint, s, namelen, client_address_size, porta;
   struct sockaddr_in client, server;
   char comando[200];
   char aux[SIZE];
	porta=htons(5000);
   /*
    * Cria um socket UDP (dgram). 
    */
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       perror("socket()");
       exit(1);
   }
   /*
    * Define a qual endere�o IP e porta o servidor estar� ligado.
    * Porta = 0 -> faz com que seja utilizada uma porta qualquer livre.
    * IP = INADDDR_ANY -> faz com que o servidor se ligue em todos
    * os endere�os IP
    */
   server.sin_family = AF_INET;   /* Tipo do endere�o             */
   server.sin_port = porta;        /* Escolhe uma porta dispon�vel */
   server.sin_addr.s_addr = INADDR_ANY;/* Endere�o IP do servidor      */

   /*
    * Liga o servidor � porta definida anteriormente.
    */ 
   if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
       perror("bind()");
       exit(1);
   }

   /* Consulta qual porta foi utilizada. */
   namelen = sizeof(server);
   if (getsockname(s, (struct sockaddr *) &server, &namelen) < 0)
   {
       perror("getsockname()");
       exit(1);
   }

   /*
    * Recebe uma mensagem do cliente.
    * O endere�o do cliente ser� armazenado em "client".
    */
char a[4]="sair";
int z=0;
while(strcmp(a, comando)!=0){
for(int i=0;i<2000;i++)
{
aux[i]='\0';
}
/*
if(z==1){
    return 0;
}
z++;
*/
    FILE *fp2;
    char *filename = "mensagem2.json";
    int n;
    char buffer[SIZE];

    // Creating a file.
    fp2 = fopen(filename, "w");

    // Receiving the data and writing it into the file.
    while(1){

    client_address_size = sizeof(client);
    n =recvfrom(s, buffer, SIZE, 0, (struct sockaddr *) &client, &client_address_size);
    
    if (strcmp(buffer, "END") == 0){
      break;
    }
    
    //printf("[RECEVING] Data: %s", buffer);
    fprintf(fp2, "%s", buffer);
    bzero(buffer, SIZE);
    }

    fclose(fp2);

    FILE *fp4 = NULL;
    fp4 = fopen("mensagem2.json" ,"r");
    int tkm=0, j=0, l=0;
    char mensa[SIZE], aux2;
    for(int i=0; i<SIZE;i++){
        mensa[i] = '\0';
    }

    while(tkm!=7){
			aux2 = fgetc(fp4);
			if(tkm==6 && aux2!='\n' && aux2!='	' && aux2!='\"'){
				mensa[l] = aux2;
                l++;
			}
			if(aux2=='\n'){
				tkm++;
			}
			if (feof(fp4)){
				break;
			} 
			if(j==SIZE)
			{
				break;
			} 
		j++;
	}
    fclose(fp4);

    printf("%s", mensa);
    /*Meu IP-----------------------------------------*/
	struct ifaddrs *ifap, *ifa;
	struct sockaddr_in *sa;
	char *addr;

	getifaddrs (&ifap);
	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr && ifa->ifa_addr->sa_family==AF_INET) {
		    sa = (struct sockaddr_in *) ifa->ifa_addr;
		    addr = inet_ntoa(sa->sin_addr);
		}
	}
	freeifaddrs(ifap);
	//-----------------------------------------------

    /*Criação do arquivo json de ACK*/
		FILE *fp = NULL;
		fp = fopen("ack.json" ,"w");
		
		/*Parte do timestamp de saida da mensagem*/
		time_t data_hora_segundos; // guarda os segundos deste 01/01/1970
		struct tm *timeinfo; // declara uma estrutura tm
		time(&data_hora_segundos); // preenche a vari�vel data_hora_segundos
		timeinfo = localtime(&data_hora_segundos);
		int dia = timeinfo->tm_mday;
		int mes = timeinfo->tm_mon + 1;
		int ano = timeinfo->tm_year + 1900;
		int hora = timeinfo->tm_hour;
		int minuto = timeinfo->tm_min;
		int segundo = timeinfo->tm_sec;

        //pega timestamp da msg e ip_origem
		FILE *fp3 = NULL;
		fp3 = fopen("mensagem.json" ,"r");

        int cont=0, ind=0, ind2=0, ind3=0, dp=0;
		char tmmsg[SIZE], ipori[SIZE], aux;

		for(int i=0; i<SIZE;i++){
			tmmsg[i] = '\0';
			ipori[i] = '\0';
		}

		while(cont!=6){
			aux = fgetc(fp3);
			if(cont==1 && dp==1 && aux!=' ' && aux!='\"' && aux!='\n'){
				ipori[ind3]=aux;
				ind3++;
			}
			if(cont==5 && aux!='\n' && aux!='	'){
				tmmsg[ind2] = aux;
				ind2++;
			}
			if(aux==':'){
				dp++;
			}
			if(aux=='\n'){
				cont++;
			}
			if (feof(fp2)){
				break;
			} 
			if(ind==SIZE)
			{
				break;
			} 
		ind++;
		}

        fclose(fp3);
		fprintf(fp, "{\n\t\"ip_origem\": \"%s\"\n", addr);
		fprintf(fp, "\t\"ip_destino\": \"%s\"", ipori);
		fprintf(fp, "\n\t\"porta_origem\": 5000");
		fprintf(fp, "\n\t\"porta_destino\": 5000");
		fprintf(fp, "\n\t%s", tmmsg);
		fprintf(fp, "\n\t\"timestamp_ack\": ");
		fprintf(fp, "\"Data: %i/%i/%i - Hora: %i:%i:%i\"", dia, mes, ano, hora, minuto, segundo);
		fprintf(fp, "\n\t\"ack\": true\n}");
		
		fclose(fp);
    /*
    * Imprime a mensagem recebida, o endere�o IP do cliente
    * e a porta do cliente 
    */
       //printf("\n\nRecebida a mensagem %s do endereco IP %s da porta %d\n", comando,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    //tira o \n da mensagem
    for(int i=0; i<strlen(comando);i++){
        if(comando[i]=='\n'){
            comando[i] = '\0'; 
        }
    }

    if(strcmp(comando,"sair")==0){
        return 0;
    }
    
    //strcpy(aux,"tesdad");
    fp = fopen("ack.json", "r");

    int n2;
    char buffer2[SIZE];

    // Sending the data
    while(fgets(buffer2, SIZE, fp) != NULL){
        //printf("[SENDING] Data: %s", buffer2);

        if ((n2 = sendto(s, buffer2, SIZE, 0, (struct sockaddr*)&client, sizeof(client))) < 0){
            perror("[ERROR] sending data to the server.");
            exit(1);
        }
        
        bzero(buffer2, SIZE);
    }

    // Sending the 'END'
    strcpy(buffer2, "END");
    sendto(s, buffer2, SIZE, 0, (struct sockaddr*)&client, sizeof(client));

    fclose(fp);
}
   /*
    * Fecha o socket.
    */
   close(s);
}

void *envia(void *arg)
{
    int s, server_address_size, namelen2;
	int port;
	struct sockaddr_in server;
	char mensagem[200], aux[200];
	char saida[10];
	char ipdestino[16];

    port=htons(5000);


	/*
	* Cria um socket UDP (dgram).
	*/
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
	   perror("socket()");
	   exit(1);
	}

    /*Meu IP-----------------------------------------*/
	struct ifaddrs *ifap, *ifa;
	struct sockaddr_in *sa;
	char *addr;

	getifaddrs (&ifap);
	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr && ifa->ifa_addr->sa_family==AF_INET) {
		    sa = (struct sockaddr_in *) ifa->ifa_addr;
		    addr = inet_ntoa(sa->sin_addr);
		}
	}
	freeifaddrs(ifap);
	//-----------------------------------------------

strcpy(saida,"sair");
   
  char ipconhecido[20]; 
do{
    for(int i=0;i<16;i++)
	{
		ipdestino[i]='\0';
	}
	/*Recebe ip destino, onde ser� enviado a mensagem*/
    printf("Destino: ");
    scanf("%s", ipdestino);

    if(strcmp(ipdestino,"me")==0){
        strcpy(ipdestino,ipconhecido);
    }
    strcpy(ipconhecido,ipdestino);
	/* Define o endere�o IP e a porta do servidor */
	server.sin_family      = AF_INET;            /* Tipo do endere�o         */
	server.sin_port        = port;               /* Porta do servidor        */
	server.sin_addr.s_addr = inet_addr(ipdestino); /* Endere�o IP do servidor  */
	
	for(int i=0;i<200;i++)
	{
		mensagem[i]='\0';
	}

    /*recebe mensagem*/
    printf("Msg: ");
    setbuf(stdin, 0);
    fgets(mensagem, sizeof(mensagem), stdin);

    //tira o \n da mensagem
    for(int i=0; i<strlen(mensagem);i++){
        if(mensagem[i]=='\n'){
            mensagem[i] = '\0'; 
        }
    }


    /*Criação do arquivo json de envio de mensagem*/
    FILE *fp = NULL;
    fp = fopen("mensagem.json" ,"w");
    
    /*Parte do timestamp de saida da mensagem*/
    time_t data_hora_segundos; // guarda os segundos deste 01/01/1970
    struct tm *timeinfo; // declara uma estrutura tm
    time(&data_hora_segundos); // preenche a vari�vel data_hora_segundos
    timeinfo = localtime(&data_hora_segundos);
    int dia = timeinfo->tm_mday;
    int mes = timeinfo->tm_mon + 1;
    int ano = timeinfo->tm_year + 1900;
    int hora = timeinfo->tm_hour;
    int minuto = timeinfo->tm_min;
    int segundo = timeinfo->tm_sec;
    
    fprintf(fp, "{\n\t\"ip_origem\": \"%s\"\n", addr);
    fprintf(fp, "\t\"ip_destino\": \"%s\"", ipdestino);
    fprintf(fp, "\n\t\"porta_origem\": 5000");
    fprintf(fp, "\n\t\"porta_destino\": 5000");
    fprintf(fp, "\n\t\"timestamp_msg\": ");
    fprintf(fp, "\"Data: %i/%i/%i - Hora: %i:%i:%i\"", dia, mes, ano, hora, minuto, segundo);
    fprintf(fp, "\n\t\"mensagem\": \"%s\"\n}", mensagem);
    fclose(fp);
    
	// Reading the text file
    fp = fopen("mensagem.json", "r");
    
    int n;
    char buffer[SIZE];

    // Sending the data
    while(fgets(buffer, SIZE, fp) != NULL){
        //printf("[SENDING] Data: %s", buffer);

        if ((n = sendto(s, buffer, SIZE, 0, (struct sockaddr*)&server, sizeof(server))) < 0){
            perror("[ERROR] sending data to the server.");
            exit(1);
        }
        
        bzero(buffer, SIZE);
    }

    // Sending the 'END'
    strcpy(buffer, "END");
    sendto(s, buffer, SIZE, 0, (struct sockaddr*)&server, sizeof(server));


    fclose(fp);

    if(strcmp(mensagem,"sair")==0){
        return 0;
    }

    FILE *fp2;
    char *filename = "ack2.json";
    char buffer3[SIZE];

    // Creating a file.
    fp2 = fopen(filename, "w");

    // Receiving the data and writing it into the file.
    while(1){

    n =recvfrom(s, buffer3, SIZE, 0, (struct sockaddr *) &server, &server_address_size);
    
    if (strcmp(buffer3, "END") == 0){
      break;
    }

    //printf("[RECEVING] Data: %s", buffer3);
    fprintf(fp2, "%s", buffer3);
    bzero(buffer, SIZE);
    }

    fclose(fp2);

    // Reading the text file
    fp = fopen("ack2.json", "r");
    if (fp != NULL){
        printf("\n>>Enviado>>");
    }
    
    printf("%s", aux);
    printf("\n");

}while(strcmp(saida, mensagem)!=0);

   /* Fecha o socket */
   close(s);
}

int main()
{
	int i;
	pthread_t tid;

	pthread_create(&tid, NULL, receba, (void *)&tid);
	pthread_create(&tid, NULL, envia, (void *)&tid);
	   
	pthread_exit(NULL);
   return 0;
}
