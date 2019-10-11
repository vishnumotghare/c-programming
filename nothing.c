#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "ntpclient.h"
#include <stdbool.h>
#include <ctype.h> 
#include <linux/rtc.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <signal.h>
#include <sys/time.h>

//ntp_packet packet;


void error( char* msg )
{
	perror( msg ); // Print the error message to stderr.

	exit( 0 ); // Quit the process.
}


struct NTPServer {
	int port;
	char *ServerName;
	int poll;
	int pollrate_min;
	bool synced;
	int status;
}srv;

void setsynced(bool status)
{
	srv.synced = status;
}


void setup_default_config(char *server,int poll, int poll_min,int port)
{
	srv.port = port;
	srv.ServerName = server;
	srv.poll = poll;
	srv.pollrate_min = poll_min;
}

void setServerName(char *ServerName)
{
	srv.ServerName = ServerName;
}

char *getServername()
{
	return (srv.ServerName);
}

void setPoll(int poll,int pollrate_min)
{
	srv.poll = poll;
	srv.pollrate_min = pollrate_min;
}


void setPort(int port)
{
	srv.port = port;
}

int getPort()
{
        return (srv.port);
}

int getpoll()
{
	return srv.poll;
}


void get_local_time(ntp_packet packet)
{	
	/* Read local time */
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

	time_t txTm = ( time_t ) ( packet.txTm_s - NTP_TIMESTAMP_DELTA );
        struct tm * ti;
        ti = localtime ( &txTm );
	fprintf(stderr, "LOC - %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void get_rtc_time()
{
	int fd,retval;
	struct rtc_time rtc_tm;
	
	fd = open ("/dev/rtc", O_RDONLY);

        if (fd ==  -1) {
                perror("/dev/rtc");
                exit(errno);
        }

	/* Read the RTC time/date */
        retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
        if (retval == -1) {
                perror("ioctl RTC_RD_TIME");
                exit(errno);
        }
	fprintf(stderr, "RTC - %02d:%02d:%02d; ", rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	close(fd);
}

void send_packet(int sockfd,ntp_packet packet)
{
	int n;
	// Send it the NTP packet it wants. If n == -1, it failed.

	printf("sending packet to server\n");
	n = write( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );

	if ( n < 0 )
		error( "ERROR in sending packet to socket" );
}

void receive_packet(int sockfd,ntp_packet packet)
{
	int n;

	// Wait and receive the packet back from the server. If n == -1, it failed.

	printf("reading from  server\n");

	n = read( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );

	if ( n < 0 )
	error( "ERROR reading from socket" );
}

void get_ntp_time(ntp_packet packet)
{
	
	// These two fields contain the time-stamp seconds as the packet left the NTP server.
	// The number of seconds correspond to the seconds passed since 1900.
	// ntohl() converts the bit/byte order from the network's to host's "endianness".

	packet.txTm_s = ntohl( packet.txTm_s ); // Time-stamp seconds.
	packet.txTm_f = ntohl( packet.txTm_f ); // Time-stamp fraction of a second.

	// Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
	// Subtract 70 years worth of seconds from the seconds since 1900.
	// This leaves the seconds since the UNIX epoch of 1970.
	// (1900)------------------(1970)**************************************(Time Packet Left the Server)

	time_t txTm = ( time_t ) ( packet.txTm_s - NTP_TIMESTAMP_DELTA );

	fflush(stderr);

        // Print the time we got from the server, accounting for local timezone and conversion from UTC time.

        printf( "Time: %s", ctime( ( const time_t* ) &txTm ) );	
}

void  setup_connect(int sockfd,char *hostname,int port)
{
	
	printf("In start\n");
        struct sockaddr_in serv_addr; // Server address data structure.
        struct hostent *server;      // Server data structure

	ntp_packet packet = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	memset( &packet, 0, sizeof( ntp_packet ) );

        // Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.

        *( ( char * ) &packet + 0 ) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.
        *( ( char * ) &packet + 2 ) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.

	server = gethostbyname(srv.ServerName ); // Convert URL to IP.

	printf("server name = %s  port =%d\n",srv.ServerName,srv.port);

        if ( server == NULL )
                error( "ERROR, no such host" );

        // Zero out the server address structure.

        bzero( ( char* ) &serv_addr, sizeof( serv_addr ) );

        serv_addr.sin_family = AF_INET;

        // Copy the server's IP address to the server address structure.

        bcopy( ( char* )server->h_addr, ( char* ) &serv_addr.sin_addr.s_addr, server->h_length );

        // Convert the port number integer to network big-endian style and save it to the server address structure.

        serv_addr.sin_port = htons( srv.port );

        // Call up the server using its IP address and port number.

        printf("connecting to server\n");

        if ( connect( sockfd, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr) ) < 0 )
                error( "ERROR connecting" );
	
}

static void usage(char *argv0)
{
        fprintf(stderr,
        "Usage: %s [-h hostname]"
        " [-p port] [-r pollrate] -s [start] -d [stop]\n"
        ,
        argv0);
}

int get_socket()
{
	int sockfd;
        sockfd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.

        if ( sockfd < 0 )
                error( "ERROR opening socket" );

	return sockfd;
}

void timer_handler (int signum)
{
	int sockfd;
	char * hostname;
	int port;

	sockfd = get_socket();
	hostname = getServername();
	port = getPort();
	ntp_packet packet;
	setup_connect(sockfd,hostname,port);
	send_packet(sockfd,packet);
	receive_packet(sockfd,packet);
}


void start_ntp()
{
	struct sigaction sa;
        struct itimerval timer;
	printf("In start\n");

        /* Install timer_handler as the signal handler for SIGVTALRM. */
        memset (&sa, 0, sizeof (sa));
        sa.sa_handler = &timer_handler;
        sigaction (SIGVTALRM, &sa, NULL);

        /* Configure the timer to expire after 1 sec... */
        timer.it_value.tv_sec = 1;
        timer.it_value.tv_usec = 0;
        /* ... and every 1000 msec after that. */
        timer.it_interval.tv_sec = 1;
        timer.it_interval.tv_usec = 0;
        /* Start a virtual timer. It counts down whenever this process is
        *    executing. */
        setitimer (ITIMER_VIRTUAL, &timer, NULL);

	srv.status = 1;
	while (1);
		sleep (1);
}

void stop_ntp()
{
	printf("Stop handler\n");
	struct itimerval value;
	int which = ITIMER_VIRTUAL;
	getitimer( which, &value );
	value.it_value.tv_sec = 0;
        value.it_value.tv_usec = 0;

        setitimer( which, &value, NULL );
	srv.status = 0;
}

void get_status()
{
	printf("status = %d\n",srv.status);
	if (srv.status == 1)
		printf("running");	
}

int main( int argc, char* argv[ ] )
{
	int c;
	int sockfd, n;  /* socket */
	ntp_packet packet;
	srv.port = 123; // NTP UDP port number
        srv.ServerName = "10.235.8.241"; // NTP server";
        srv.poll = 600;
        srv.pollrate_min = 0;
	char *p;
	int a;

	for (;;) {
		c = getopt( argc, argv, "h:p:r:g:s:d:f:k:");
		if (c == EOF) break;
		switch (c) {
			case 'h':
				srv.ServerName = optarg;
				printf("%s\n",srv.ServerName);
				break;
			case 'p':
				srv.port = atoi(optarg);
				printf("%d\n",srv.port);
				break;
			case 'r':
				srv.poll = atoi(optarg);
				printf("%d\n",srv.poll);
				break;
			case 's':
				start_ntp();
				break;
			case 'd':
				get_ntp_time(packet);
				break;
			case 'f':
				stop_ntp();
				break;
			case 'k':
				get_status();
				break;
			default:
                                usage(argv[0]);
				exit(1);
		}
		
	}
	daemon(1,1);
	/* Startup sequence */

//	start_ntp();

//	setup_connect(sockfd,srv.ServerName,srv.poll);
//	send_packet(sockfd,packet);
//	receive_packet(sockfd,packet);
//	get_ntp_time(packet);
	

//	primary_loop(sockfd);
	
	close(sockfd);

	return 0;
}	
