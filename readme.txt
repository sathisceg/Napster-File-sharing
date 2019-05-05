
CRS

	Implemented server

		For handling multiple clients to share info regarding files.

	Implemented data structure

		For File sharing and searching.

		File Creation not done to maintain logs.

Client

	Implemented client and server 

		Server module for serving the request made by other clients.

		Client for communicating with CRS to share info regarding file.


Not Impelemented 

	RPC
	Deletion of file
	Repo list and client list




commands 				

./n3 "client c" 10.42.0.1 8000 10.42.0.1 20000 20003 ~/Desktop

./n2 "client b" 10.42.0.1 8000 10.42.0.1 20000 20002 ~/Desktop

./n1 "client a" 10.42.0.1 8000 10.42.0.1 20000 20001 ~/Desktop


./crs 10.42.0.1 20000 repo.txt list.txt ~/Desktop/eval


share "Desktop/temp2/up/1.mp4"

share "up/1.mp4"
search "1.mp4"