
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
#include <map>
#include <iterator>
#include <vector>
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

char crs_ip[30];
char crs_port[30];
char crs_rep[30];
char crs_cli[30];
char crs_root[30];




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

struct clientinfo
{

   
};


// vector<struct fileinfo> filevector;
// vector<struct clientinfo> clientvector;

map<string,vector < struct fileinfo > > filemap;
map<string,vector < struct clientinfo > > clientmap;



//#define PORTCSR 10001
int portcrst;





void *send_msg(void *sockt)
{



    int sock = *(int*)sockt;
    int datalen;
    int i=0;


   while(1)
   {
          i++;

            if(i==10)
            {
              break;
            }

            printf("\nin crs while loop\n");

            datalen = recv( sock , buffer, 1024,0);

            if(buffer[0]=='s' && buffer[1]=='e')
            {

                char data[ 1024 ];
                strcpy(data,buffer);
                char *token2;
                char *rest2 = data;
                //to get file name
                // struct  fileinfo fd;
                // struct  clientinfo cd;
                 char *args[10];
                 
                //char searchfile[30];


                    printf("\n\n\n");
                    int i=0;
                    while ((token2 = strtok_r(rest2, "###", &rest2)))
                    {
                      // printf("%s\n", token2);
                      //strcpy(file_name,token1);
                      args[i]=token2;
                      i++;
                    }
                    args[i]=NULL;

                    if(filemap.count(args[1])>0)
                    {

                        char clientdata[1024];
                        char curline[1024];
                        
                        vector < struct fileinfo  > vt  = filemap[args[1]];
                       // vector < struct clientinfo > vc;

                        
                        for (int i = 0  ; i < vt.size() ; i++)
                        {
                            
                              snprintf( curline, sizeof( curline ), "@@%s##%s##%s##%s##%s@@", vt.at(i).fileName, vt.at(i).filePath, vt.at(i).clientAliasf,vt.at(i).cip,vt.at(i).dp);
                              strcat(clientdata,curline);  

                            // for (int j = 0  ; j < vc.size() ; j++)
                            // {
                            //     if(vc.at(j).dp && vc.at(j).cip)
                            //     {
                            //       snprintf( curline, sizeof( curline ), "@@%s##%s##%s##%s##%s@@", vt.at(i).fileName, vt.at(i).filePath, vt.at(i).clientAliasf,vc.at(j).cip,vc.at(j).dp);
                            //       strcat(clientdata,curline);                  
                            //     }
                            //     bzero(curline, 1024);
                            // }  
                        }

                        printf("\nclientdata  !!!!!! %s \n",clientdata); 
                       // strcpy(buffer,clientdata);

                        send(sock , clientdata , strlen(clientdata) , 0 );
                        bzero(clientdata, 1024);    
                        bzero(buffer, 1024);          
                    }
                    else
                    {
                       //error message 
                        char clientdata[1024];

                        strcpy(clientdata,"FILE NOT FOUND");
                        
                       // printf("clientdata  !!!!!! %s \n",clientdata); 
                       // strcpy(buffer,clientdata);

                        send(sock , clientdata , strlen(clientdata) , 0 );
                    }



            }
            else
            {

                    char data[ 1024 ];
                    strcpy(data,buffer);




                    char *token2;
                    char *rest2 = data;
                    //to get file name

                      struct  fileinfo fd;
                      struct  clientinfo cd;
                      char *args[10];
               

                     printf("\n\n\n");
                    int i=0;
                    while ((token2 = strtok_r(rest2, "###", &rest2)))
                    {
                       // printf("%s\n", token2);
                      //strcpy(file_name,token1);
                      args[i]=token2;
                      i++;
                    }
                     args[i]=NULL;



                  if(strcmp(args[6],"share")==0)
                  {
                      printf("\nshare \n");



                      strcpy(fd.filePath,args[0]);strcpy(fd.fileName,args[1]);strcpy(fd.clientAliasf,args[2]);
                      strcpy(fd.cip,args[3]);strcpy(fd.cp,args[4]);strcpy(fd.dp,args[5]);
                      //  cd.clientAliasc = args[2];cd.cip=args[3];cd.cp=args[4];cd.dp=args[5]
                      filemap[fd.fileName].push_back(fd); 

                       bzero(buffer, 1024);

                      strcpy(buffer,"SUCCESS:FILE RECEIVED");   

                      send(sock , buffer , strlen(buffer) , 0 );

                      if(datalen>0)
                      {
                           printf("crs recv msg:  !!!!!! %s \n",buffer);           
                      }
                      else
                      {
                        return NULL ;
                      }

                     bzero(buffer, 1024);

                     bzero(data, 1024);
                      
                }
                else
                {
                  
                }
            }
            
            // else
            // {
            //     return NULL; //to stop infinite running of in crs while loop
            // }
              

            bzero(buffer, 1024);
          
             

            

            // send(sock , buffer , strlen(buffer) , 0 );

            // if(datalen>0)
            // {
            //      printf("crs thread recv msg:  !!!!!! %s \n",buffer);           
            // }
            // else
            // {
            //   return NULL ;
            // }
            //  bzero(buffer, 1024);
            // printf("in crs while loop\n");
            // datalen = recv( sock , buffer, 1024,0);
            // send(sock , buffer , strlen(buffer) , 0 );
            // if(datalen>0)
            // {
            //      printf("crs thread recv msg:  !!!!!! %s \n",buffer);           
            //  }
            // else
            // {
            //   return NULL ;
            // }
            //  bzero(buffer, 1024);
         //close(fp);
      }  


    free(sockt);
    close(sock);
    pthread_exit(NULL);      
    return NULL;
}

void start_csr()
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

   // uint32_t ipAddress = parseIPV4string(crs_ip);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(crs_ip);
    address.sin_port = htons( portcrst );
    

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
                           
                printf("\nafter accepting\n");

                new_socket = (int *)malloc(1);
                *new_socket = temp;
  
                pthread_t thread1;
              

                ret1 = pthread_create( &thread1 , NULL ,  send_msg , (void *)new_socket) ;    

        
                //pthread_join
                printf("\nclient connected crs thread \n");


      }



    printf("out of while");

   if (new_socket < 0)
    {
        perror("accept failed");
        return ;
    }

}




void init_csr()
{

	  printf("\nStarting CRS......\n");
  	start_csr();	

}


int main(int argc,char *argv[])
{

   //portcrst = 30000;  
  

   strcpy(crs_ip,argv[1]);
   strcpy(crs_port,argv[2]);
   strcpy(crs_rep,argv[3]);
   strcpy(crs_cli,argv[4]);
   strcpy(crs_root,argv[5]);
 
   // printf(" %s \n",crs_ip);
   // printf(" %s \n",crs_port);
   // printf(" %s \n",crs_rep);
   // printf(" %s \n",crs_cli);
   // printf(" %s \n",crs_root);
   portcrst=atoi(crs_port);

   init_csr();
   return 0;
}