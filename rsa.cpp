#include <iostream>
#include <cmath>
#include <cstring>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <getopt.h>
#include <stdlib.h>
#include <stdexcept>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>

using namespace std;

#define PORT 19022
string ipaddress="127.0.0.1";

string command="id";
string command_dec;
//string command="ls -l";


int Plaintext [100]; // Открытый текст
long long Ciphertext [100]; // Зашифрованный текст
int n, e = 0, d;


string rsaexec(const char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}
 // Двоичное преобразование
int BianaryTransform(int num, int bin_num[])
{
	int i = 0,  mod = 0;
	// Преобразуется в двоичный, обратный временно сохраняется в массиве temp []
	while(num != 0)
	{
		mod = num%2;
		bin_num[i] = mod;
		num = num/2;
		i++;
	}
	// Возвращает количество цифр в двоичных числах
	return i;
}

 // Повторное возведение в квадрат в степень
long long Modular_Exonentiation(long long a, int b, int n)
{
	int c = 0, bin_num[1000];
	long long d = 1;
	int k = BianaryTransform(b, bin_num)-1;

	for(int i = k; i >= 0; i--)
	{
		c = 2*c;
		d = (d*d)%n;
		if(bin_num[i] == 1)
		{
			c = c + 1;
			d = (d*a)%n;
		}
	}
	return d;
}

 // Генерация простых чисел в пределах 1000
int ProducePrimeNumber(int prime[])
{
	int c = 0, vis[1001];
	memset(vis, 0, sizeof(vis));
	for(int i = 2; i <= 1000; i++)if(!vis[i])
	{
		prime[c++] = i;
		for(int j = i*i; j <= 1000; j+=i)
			vis[j] = 1;
	}
	return c;
}
 // Расширенный алгоритм Евклида
int Exgcd(int m,int n,int &x)
{
	int x1,y1,x0,y0, y;
	x0=1; y0=0;
	x1=0; y1=1;
	x=0; y=1;
	int r=m%n;
	int q=(m-r)/n;
	while(r)
	{
		x=x0-q*x1; y=y0-q*y1;
		x0=x1; y0=y1;
		x1=x; y1=y;
		m=n; n=r; r=m%n;
		q=(m-r)/n;
	}
	return n;
}
 // Инициализация RSA
void RSA_Initialize()
{
	// Вынимаем простые числа в пределах 1000 и сохраняем их в массиве prime []
	int prime[5000];
	int count_Prime = ProducePrimeNumber(prime);
	// Случайно возьмем два простых числа p, q
	srand((unsigned)time(NULL));
	int ranNum1 = rand()%count_Prime;
	int ranNum2 = rand()%count_Prime;
	int p = prime[ranNum1], q = prime[ranNum2];
	n = p*q;
	int On = (p-1)*(q-1);
	// Используем расширенный алгоритм Евклида, чтобы найти e, d
	for(int j = 3; j < On; j+=1331)
	{
		int gcd = Exgcd(j, On, d);
		if( gcd == 1 && d > 0)
		{
			e = j;
			break;
		}
	}
}

void RSA_Encrypt()
{
	//cout<<"Public Key (e, n) : e = "<<e<<" n = "<<n<<'\n';
	//cout<<"Private Key (d, n) : d = "<<d<<" n = "<<n<<'\n'<<'\n';
	int i = 0;
	for(i = 0; i < command.length(); i++)
	{
		Ciphertext[i] = Modular_Exonentiation(Plaintext[i], e, n);
		//cout<<Ciphertext[i]<<" ";
	}
	cout<<endl;
}

void RSA_Decrypt()
{
	int i = 0, j=0;
	command_dec="";
	for(j = 100-1; j > 0; j--){
		if (Ciphertext[j] != 0)
			break;
	}
	for(i=0;i<j+1;i++)
	{
		Ciphertext[i] = Modular_Exonentiation(Ciphertext[i], d, n);
		//cout<<char(Ciphertext[i]+'0');
		command_dec=command_dec+char(Ciphertext[i]+'0');
	}
	cout<<endl;
	//cout<<"Use private key (d, n) to decrypt:"<<'\n';
}

void Initialize()
{
	int i;
	//srand((unsigned)time(NULL));
	//string text="Secret text for 313";

	//uint16_t        A;
	for (i = 0; i < command.length(); i++ )
	{
		Plaintext[i] = (int)command[i]-'0';
	}
}

int main(int ac, char *av[])
{
	//for (int i=1;i<ac;i++)
	//	cout<<"'"<<av[i]<<"'";
	//cout<<endl;
	 int rez = 0;
	 bool s,c = false;

	 if (ac == 1)return -1;

	 for (int i=1;i<ac;i++)
	 {
		// cout<<av[i]<<endl;
		 if (strcmp(av[i], "-k") ==0)
		 {
			 cout<<"Generate keys..."<<endl;
			//Initialize();
			while(!e)
			RSA_Initialize();
			cout<<"Public Key (e, n) : e = "<<e<<" n = "<<n<<'\n';
			cout<<"Private Key (d, n) : d = "<<d<<" n = "<<n<<'\n'<<'\n';
			return 0;
		 }
		 if (strcmp(av[i], "-e") ==0)
		 //if (av[i] == "-e")
		 {
			 e = atoi(av[i+1]);
			 i+=1;
		 }
		 if (strcmp(av[i], "-n") ==0)
		//if (av[i] == "-n")
		 {
			 n = atoi(av[i+1]);
			 i+=1;
		 }
		 if (strcmp(av[i], "-d") ==0)
		 //if (av[i] == "-d")
		 {
			 d = atoi(av[i+1]);
			 i+=1;
		 }
		 if (strcmp(av[i], "-s") ==0)
		 //if (av[i] == "-s")
		 {
			 s=true;
		 }
		 if (strcmp(av[i], "-c") ==0)
		 //if (av[i] == "-c")
		 {
			 c=true;
		 }
		 if (strcmp(av[i], "-h") ==0)
		// if (av[i] == "-h")
		 {
			 ipaddress = av[i+1];
			 i+=1;
		 }
		 if (strcmp(av[i], "-r")==0)
		 //if (av[i] == "-r")
		 {
			 command="";
			 i+=1;
			 for( ;i < ac; i++)
				 command=command+av[i];
			 //cout<<"command="<<command<<endl;
			 break;
		 }
	 }
	  /*while ((rez = getopt(ac, av, "scd:n:ke:r:h:")) != -1) {
	    switch (rez) {
	    case 's':
	      //puts("Server starting...");
	      s=true;
	      break;
	    case 'c':
	      //puts("Client starting...");
	      c=true;
	      break;
	    case 'd':
	      //cout<<"d="<<optarg<<endl;
	      d = atoi(optarg);
	      break;
	    case 'e':
		  //cout<<"e="<<optarg<<endl;
		  e = atoi(optarg);
		  break;
	    case 'n':
	      //cout<<"n="<<optarg<<endl;
	      n = atoi(optarg);
	      break;
	    case 'r':
		  //command = optarg;
	    	command="";
		  optind--;
		         // for( ;optind < ac && *av[optind] != '-'; optind++)
		  	  	  for( ;optind < ac ; optind++)
		                command=command+av[optind];
		  cout<<"command="<<command<<endl;
		  break;
	    case 'h':
		  ipaddress = optarg;
		  break;
	    case 'k':
		  cout<<"Generate keys..."<<endl;
		  //Initialize();
		  while(!e)
			RSA_Initialize();
		  cout<<"Public Key (e, n) : e = "<<e<<" n = "<<n<<'\n';
		  cout<<"Private Key (d, n) : d = "<<d<<" n = "<<n<<'\n'<<'\n';
		  return 0;
	    }
	  }
	  int index;
	  for (index = optind; index < ac; index++)
	  {
	      printf ("Non-option argument %s\n", av[index]);
	    return 0;
	  }*/
	if(s == true && d>0 && n>0 )
	{
		//Initialize();
		cout<<"Server starting..."<<endl;
		int server_fd, new_socket;
		ssize_t valread;
		struct sockaddr_in address;
		int opt = 1;
		socklen_t addrlen = sizeof(address);
		long long rev_command[100] = { 0 };


		// Creating socket file descriptor
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("socket failed");
			exit(EXIT_FAILURE);
		}

		// Forcefully attaching socket to the port 8080
		if (setsockopt(server_fd, SOL_SOCKET,
					   SO_REUSEADDR | SO_REUSEPORT, &opt,
					   sizeof(opt))) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		address.sin_family = AF_INET;
		//address.sin_addr.s_addr = INADDR_ANY;
		address.sin_addr.s_addr =htonl(INADDR_ANY);
		//address.sin_addr.s_addr = inet_addr(ipaddress.c_str());
		address.sin_port = htons(PORT);

		if (bind(server_fd, (struct sockaddr*)&address,sizeof(address))< 0)
		{
			perror("bind failed");
			exit(EXIT_FAILURE);
		}
		while(1)
		{
			if (listen(server_fd, 3) < 0)
			{
				perror("listen");
				exit(EXIT_FAILURE);
			}
			if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen))< 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			memset(Ciphertext,0,100);
			if (read(new_socket, Ciphertext,   sizeof(Ciphertext)+1) >0 )
			{
				//cout<<Ciphertext[0];
				//cout<<"d="<<d<<" n="<<n<<endl;
				RSA_Decrypt();
				string out=rsaexec(command_dec.c_str());
				cout<<out<<endl;
				write(new_socket, out.c_str(), sizeof(out.c_str())*1024);
			}
			close(new_socket);
			}
		close(server_fd);

	}else if(c==true && e>0 && n>0 )
	{
		cout<<"Client starting..."<<endl;
		Initialize();
		RSA_Encrypt();

		int status, valread, client_fd;
		struct sockaddr_in serv_addr;
		//char* hello = "get";
		char buffer[1024] = { 0 };
		if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			return -1;
		}
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(ipaddress.c_str());
		serv_addr.sin_port = htons(PORT);

		// Convert IPv4 and IPv6 addresses from text to binary
		// form
		if (inet_pton(AF_INET, (char *)ipaddress.c_str(), &serv_addr.sin_addr)<= 0)
		{
			printf("\nInvalid address/ Address not supported \n");
			return -1;
		}
		if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))< 0)
		{
			printf("\nConnection Failed \n");
			return -1;
		}

		//while(1)
		//{
			if (write(client_fd, Ciphertext, sizeof(Ciphertext)+1) >0)
			{
				read(client_fd, buffer,1024);
				cout<<buffer<<endl;
				cout<<"Done."<<endl;
			}

		//}
		close(client_fd);
	}else return 1;

	/*Initialize();
	while(!e)
		RSA_Initialize();
	RSA_Encrypt();
	RSA_Decrypt();
*/
	return 0;
}
