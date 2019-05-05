
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <errno.h>
#include <fcntl.h>
#include <iterator>
#include <vector>
#include <map>
#include <arpa/inet.h>

using namespace std;

int server_fd, *new_socket, valread;
struct sockaddr_in address;
int opt ;
int addrlen ;
char buffer[1024];
int datalen;
int ret1;
int temp;

 char reqfilepath[1024] ;
 char reqipaddress[1024] ;
 char dwnldfile[1024];
 char currdir[1024];
 int flagexe;


 char  user_alias[30];
 char  user_cip[30];
 char  user_cport[30];
 char  user_sip[30];
 char  user_sport[30];
 char  user_dport[30];
 char  user_root[30];


struct fileinfo
{

  char fileName[30];
  char filePath[40];
  char clientAliasf[30];
  //char clientAliasc[30];
  char cip[20];
  char cp[6];
  char dp[6];

};

 map<int, struct fileinfo  > output;
//#define PORTS1 10004
//#define PORTC1 10006

int portsn1;
int portcn1;


void  receieve_file(int sockc);

  	struct sockaddr_in addressc;
	int sockc = 0, valreadc;
	struct sockaddr_in serv_addrc;
	
	char bufferc[1024] = {0};
	char outc[1024] = {0};
  	//int datalenc;

// for sending file to client requesting

void *send_msg(void *sockt)
{


	int sock = *(int*)sockt;
    int datalen;
    
    
    
	// printf("r %d\n", strlen(c));
	//	bzero(temp, 1024);
  		    	
   				
  		    	char recvfilename[1024];

  		    	

   				datalen = recv(sock,recvfilename,1024,0);
   				recvfilename[datalen] = '\0';

   				if(datalen<=0)
   		    	{
   		    		free(sockt);
    				close(sock);
    				pthread_exit(NULL); 
     
   					return NULL;
   		    	}

   				char temp[30];

    			char req_file_name[40];

   				strcpy(req_file_name,user_root);

   				strcpy(temp,recvfilename);   

   				strcat(req_file_name,temp);	

   				printf("\ndatalen :%d \n",datalen);

 				printf("\nrequest from client bufferc :%s \n",recvfilename);
   						
				
				printf("\nrequest from client req_file_name :%s \n",req_file_name);
   				
   				//strcpy(req_file_name,temproot);


   
    			int fp; 
    			fp = open(req_file_name, O_RDONLY);
    			//printf("\nrecv msg node 2 %d $$$$$$ :%s \n",sock,req_file_name);



    			 while(1)
				 {				   

				      int datalen=1;
				      ssize_t read_bytes,sent_bytes;
				      bzero(buffer, 1024);

				      char sendbuffer[256];
					  bzero(sendbuffer,256);

				      while((read_bytes = read(fp, sendbuffer, 256)) > 0 )
				      {
				   			
						if(read_bytes==1)
				      	  {
				      	  	  perror("send error buffer issue");
				              return NULL;
				      	  }	
				          else if( (sent_bytes = send(sock, sendbuffer, read_bytes,0))< read_bytes )
				          {
				              perror("send error");
				              return NULL;
				          }
				              
				      }
				       close(fp);
				       bzero(sendbuffer, 256);
				 }
				  
				   
  

 	//bzero(buffer, 1024);
	//bzero(recvfilename, 1024);

   // bzero(bufferc, 1024); 
    free(sockt);
    shutdown(sock,SHUT_RDWR);
    close(sock);
    pthread_exit(NULL); 
     
    return NULL;
    
}

void *start_node_server(void *arg)
{
			
  	 opt = 1;
   	 addrlen = sizeof(address);
     buffer[1024];
	 int pid;
     temp =0;
	

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(user_sip);
    address.sin_port = htons( portsn1 );
    

    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    		
    if (listen(server_fd, 20) < 0)
    {
      	  perror("listen");
      	  exit(EXIT_FAILURE);
   	 }

      int temp;

       while((temp = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen)))
       {
                           
               //printf("1\n");
               new_socket = (int *)malloc(1);
               *new_socket = temp;
  
                pthread_t thread1;
              

                ret1 = pthread_create( &thread1 , NULL ,  send_msg , (void *)new_socket) ;    

        
        	 // pthread_join(ret1 ,NULL);

              printf("client connected");


       }



    printf("out of while");

    if (new_socket < 0)
    {
        perror("accept failed");
        return NULL;
    }

}

void *client_req_msg(void *socktc)
{
  
    return NULL;
}




char * get_relative_path(char bufferrpn[50])
{

	char *token;
	char *rest = bufferrpn;
	int f=1;

	//char relative_path[40];
	char *relative_path = (char*)malloc(sizeof(char)*40);
	char file_name[25];

	//to get relative path
	while ((token = strtok_r(rest, "\"", &rest)))
	{
		strcpy(relative_path,token);
		if(f==2)
			break;
		f++;
	}

	return relative_path;

}


char *get_file_path(char bufferrpn[50])
{

	char *token;
	char *rest = bufferrpn;
	int f=1;
	//char file_path[40];
	char *file_name = (char*)malloc(sizeof(char)*40);

	while ((token = strtok_r(rest, "/", &rest)))
	{
		//printf("%s\n", token1);
		strcpy(file_name,token);
	}
	return file_name;

}

char *get_exe_command(char bufferrpn[50])
{

	char *token;
	char *rest = bufferrpn;
	int f=1;
	//char file_path[40];
	char *cmnd = (char*)malloc(sizeof(char)*40);

	while ((token = strtok_r(rest, "\"", &rest)))
	{
		//printf("%s\n", token);
		strcpy(cmnd,token);
		if(f==2)
			break;
		f++;
	}

	//printf("cmnd %s\n",cmnd);
	return cmnd;

}



//connecting with crs
void send_msg_to_client(int sockr)
{


	//printf(" send msg  node %c  %c\n",bufferc[0],bufferc[1]);

		 	if(bufferc[0]=='s' && bufferc[1]=='h')
		 	{

		 		printf("\nsharing file.....\n");

			 	char *relative_path = (char*)malloc(sizeof(char)*40);
				char relative_patht[40];
				char *file_name = (char*)malloc(sizeof(char)*40);

				relative_path = get_relative_path(bufferc);

				strcpy(relative_patht,relative_path);
				file_name = get_file_path(relative_patht);

				char ca[20];
				char cip[20];
				char sip[20];
				char cp[5];
				//char sp[5];
				char dp[5];

				strcpy(ca,user_alias);
				strcpy(cip,user_sip);
				//strcpy(sip,"192.168.1.20");
				strcpy(cp,user_cport);
				//strcpy(sp,"8754");
				strcpy(dp,user_dport);

				char data[ 1024 ];
				snprintf( data, sizeof( data ), "%s###%s###%s###%s###%s###%s###%s", relative_path, file_name, ca ,cip,cp,dp,"share");
				send(sockr , data , strlen(data) , 0 );
		   		int len = recv(sockr,data,strlen(data),0);
		   		// bufferc[len] = '\0';
		    	
		   		printf("\n........FILE SHARED.....\n\n");
		    	//printf("recv msg node 1 %d $$$$$$ :%s \n",sockr,data);

		    	bzero(data, 1024);


		 	}
		 	else if(bufferc[0]=='d' && bufferc[1]=='e')
		 	{

		 	}
		 	else if(bufferc[0]=='s' && bufferc[1]=='e')
		 	{

		 		char *searchfile = (char*)malloc(sizeof(char)*40);
				
				
				searchfile = get_relative_path(bufferc);

				char data[1024];char searchinfo[1024];

				snprintf( data, sizeof( data ), "%s###%s", "search", searchfile);



				send(sockr , data , strlen(data) , 0 );

		   		int len = recv(sockr,data,1024,0);
		   		
		    	printf("recv msg node $$$$$$ :%s \n",data);


		    	if(strcmp(data,"FILE NOT FOUND")==0)
		    	{
		    		printf("\n........NO RESULTS FOR GIVEN FILE.....\n\n");
		    	}
		    	else
		    	{	
				    	strcpy(searchinfo,data);
				    	char *token3;
		                char *rest3 = searchinfo;

		                
		                struct  fileinfo fd;
		                char line[1024];
		                char *args[10];
		                int i=0;

		                while ((token3 = strtok_r(rest3, "@@", &rest3)))
		                {
		                    //printf(" %s\n",token3);

							strcpy(line,token3);

		                    char *token4;
		                    char *rest4 = line;
		                    
		                    int j=0;
		                    
		                    while ((token4 = strtok_r(rest4, "##", &rest4)))
		                    {
		                         // printf("%s    |", token4);
		                         args[j]=token4;
		                       
		              			  j++;
		                    }
		                    printf("\n");
		                   

		                   


		                    strcpy(fd.fileName,args[0]);strcpy(fd.filePath,args[1]);strcpy(fd.clientAliasf,args[2]);
		                	strcpy(fd.cip,args[3]);strcpy(fd.dp,args[4]);
		                    output[i]=fd;
		            
		                	bzero(line,1024);
		                   
		                   // 
		                    i++;
		                }

		                printf("\n........FILES FOUND.....\n\n");

		                for(int j=0;j<output.size();j++)
		                {
		                	
			               
			                 		printf("%d  ", (j+1));
			                 		printf("%s  ", output[j].fileName);
			                 		printf("%s  ", output[j].filePath);
			                 		printf("%s  " ,output[j].clientAliasf);
			                 		printf("%s  ", output[j].cip);
			                 		printf("%s  ", output[j].dp);
			                 		printf("\n\n");	                        
			                 		// snprintf( curline, sizeof( curline ), "@@%s  |%s%s  |%s   |%s@@", vt.at(i).fileName, vt.at(i).filePath, vt.at(i).clientAliasf,vt.at(i).cip,vt.at(i).dp);
			                 		// printf("curline %s\n\n",curline);
			                 

		                }

		          }      



		 	}
		 	
		 	

			



 	

    //send(sockr , data , strlen(data) , 0 );

    //int len = recv(sockr,data,strlen(data),0);
   // bufferc[len] = '\0';


    //printf("recv msg node 1 %d $$$$$$ :%s \n",sockr,data);
		 	return ;

}



void  *connect_to_other_server(void *argsfr)
{




			 printf("\nconnect_to_other_servere   \n");


			// printf("in reqfilepath  %s \n",reqfilepath);
			 //printf("in reqipaddress  %s \n",reqipaddress);



			 


			int portnot = *(int *)argsfr;

			int sockr;
	 		 
	 		 if ((sockr = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
					printf("\n Socket creation error \n");
				  return NULL;
			}

	 		 addressc.sin_family = AF_INET;
   			// address.sin_addr.s_addr = inet_addr(argv[1]);
   			// address.sin_port = htons( atoi(argv[2]) );

   			 addressc.sin_addr.s_addr = inet_addr(reqipaddress);
   
	 		 addressc.sin_port = htons( portnot );

   			 //addressc.sin_port = 20004;

	 		 if (connect(sockr, (struct sockaddr *)&addressc, sizeof(sockaddr_in)) < 0)
			 {
				printf("\n Connection Failed 123 \n");
				  return NULL;
			 }

			 printf("\nbefore request in reqfilepath  %s \n",reqfilepath);

			// printf(" before request in strlen(reqfilepath)  %d \n",strlen(reqfilepath));

			// printf(" before request  in reqipaddress  %s \n",reqipaddress);

			 ssize_t sent_bytes;

			 if( (sent_bytes = send(sockr , reqfilepath, strlen(reqfilepath), 0 ))< strlen(reqfilepath) )
			 {
				   perror("error in sending file naem");
				   return NULL;
			 }


  			receieve_file(sockr);

	return NULL;
}

// for receiving file from client
void  receieve_file(int sockc)
{



		
		
				int fp1;
				//strcat(currdir,dwnldfile);

				//printf("%s\n",user_root );

				char temppath[30];

				bzero(temppath,30);
				strcpy(temppath,user_root);

				strcat(temppath,dwnldfile);

				//printf("temppath  %s \n",temppath);
				printf("\ndwnldfile  %s \n",dwnldfile);

				strcpy(dwnldfile,temppath);

				fp1 = open(dwnldfile, O_WRONLY|O_CREAT, 0644);

				ssize_t sent_bytes, rcvd_bytes;

				char downloadbuffer[256];
				bzero(downloadbuffer,256);

				while ( (rcvd_bytes = recv(sockc, downloadbuffer, 256, 0)) > 0 )
	 			{

	 				//printf("  downloadbuffer content  %s \n",downloadbuffer);
			
					if (write(fp1, downloadbuffer, rcvd_bytes) < 0 )
					{
						perror("error writing to file");
						//return NULL;
					}
				}

		bzero(downloadbuffer,256);	
		close(fp1);
	
		close(sockc);
		//pthread_exit(NULL); 
		
}





void *start_node_client(void *arg)
{



	printf("client thread\n");		

  	sleep(1);

	if ((sockc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		  return NULL;
	}

    addressc.sin_family = AF_INET;
   // address.sin_addr.s_addr = inet_addr(argv[1]);
   // address.sin_port = htons( atoi(argv[2]) );

    addressc.sin_addr.s_addr = inet_addr(user_cip);
    addressc.sin_port = htons( portcn1 );


     if (connect(sockc, (struct sockaddr *)&addressc, sizeof(sockaddr_in)) < 0)
	 {
		printf("\nConnection Failed \n");
		  return NULL;
	 }


	ssize_t sent_bytes, rcvd_bytes;
	//int i=0;

	 while(1)
	 {
    	
  
	 	bzero(bufferc, 1024);
  		fgets(bufferc,1024,stdin);

  		//printf("\nin while \n");
			 	

	 	if((bufferc[0]=='g' && bufferc[1]=='e'))
	 	{


				//printf("if block\n");

				

				int v1 = bufferc[4]-'0';v1=v1-1;
				int v2 = bufferc[5]-'0';if(v2>0)v1=v1*10+(v2-1);

				//printf("%d %d\n",v1,v2);
				
				struct fileinfo  si  = output[v1];
				
				bzero(reqipaddress,1024);
				bzero(reqfilepath,1024);
				bzero(currdir,1024);
				bzero(dwnldfile,1024);

				strcpy(reqipaddress,si.cip);
				strcpy(reqfilepath,si.filePath);
				//strcpy(currdir,"dwnld/")
;				
				strcat(currdir,si.fileName);
				strcpy(dwnldfile,currdir);

				
				int portnot = atoi(si.dp);

				//printf("if block %s  \n",reqipaddress);

				//printf("if block  %s  \n",reqfilepath);

				//printf("if block  %s  \n",dwnldfile);


	 		  	int *temp_port = (int *)malloc(1);
	 		  	*temp_port = portnot;

			 	pthread_t threadfr;

			 	int retfr;

			 	retfr = pthread_create(&threadfr,NULL,connect_to_other_server,(void *)temp_port);

			 	//pthread_join(threadfr,NULL);



			
	 	}
	 	else
	 	{

	 		  // printf("else block\n");

	 		  // printf(" cmp  node %c  %c\n",bufferc[0],bufferc[1]);

	 		   send_msg_to_client(sockc);

	 		  // printf("cli i = %d\n",i);
	 		   //if(i==10)
	 		   	//break;
	 		   //i++;


	 	}

	 	 	   bzero(buffer, 1024);
	 	 	 //  bzero(bufferc, 1024);


	 	//i++;









    	//  client to send msg
     	
	 	//receieve_file(sockc);




    	 //  client to receive msg

	 	// 	while ( (rcvd_bytes = recv(sockc, buffer, 256, 0)) > 0 )
	 	// 	{
			
		// 	if (write(fp1, buffer, rcvd_bytes) < 0 )
		// 	{
		// 	perror("error writing to file");
		// 	return NULL;
		// 	}
		// }
		// close(fp1); 




 	 }



 	 return NULL;

}



void init_client_node()
{

	//printf(" init_node v2n1\n");

	portsn1 = atoi(user_dport);
	portcn1 = atoi(user_sport);




	pthread_t threads, threadc;

	int rs,rc;

	rs = pthread_create(&threads,NULL,start_node_server,NULL);
	//start_node_server();

	rc = pthread_create(&threadc,NULL,start_node_client,NULL);
	//start_node_client();	

	pthread_join(threads,NULL);
	pthread_join(threadc,NULL);

	printf("after init_node 3\n");

}


int main(int argc,char *argv[])
{



 	
   strcpy(user_alias,argv[1]);
   strcpy(user_cip,argv[2]);
   strcpy(user_cport,argv[3]);
   strcpy(user_sip,argv[4]);
   strcpy(user_sport,argv[5]);
   strcpy(user_dport,argv[6]);
   strcpy(user_root,argv[7]);
   strcat(user_root,"/");


   // printf(" %s \n",user_alias);
   // printf(" %s \n",user_cip);
   // printf(" %s \n",user_cport);
   // printf(" %s \n",user_sip);
   // printf(" %s \n",user_sport);
   // printf(" %s \n",user_dport);
   // printf(" %s \n",user_root);



  init_client_node();
  return 0;
}