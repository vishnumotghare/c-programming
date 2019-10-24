#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/rtc.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
       #include <sys/timerfd.h>


void fun(struct rtc_time *rtc_tm)
{
	struct tm ti;

	memcpy(&ti,rtc_tm,sizeof(struct rtc_time));

        fprintf(stderr, "RTC - %02d:%02d:%02d\n", ti.tm_hour, ti.tm_min, ti.tm_sec);
	
}
int main()
{
	int fd,retval;
        struct rtc_time *rtc_tm;
	

        fd = open ("/dev/rtc", O_RDONLY);

        if (fd ==  -1) {
                perror("/dev/rtc");
                exit(errno);
        }


	rtc_tm = (struct rtc_time*)malloc(sizeof(struct rtc_time));
	
	if (rtc_tm == NULL) {
		printf("mem allocation failed\n");
		exit(1);
	}
        /* Read the RTC time/date */
        retval = ioctl(fd, RTC_RD_TIME, rtc_tm);
        if (retval == -1) {
                perror("ioctl RTC_RD_TIME");
                exit(errno);
        }
        fprintf(stderr, "RTC - %02d:%02d:%02d; ", rtc_tm->tm_hour, rtc_tm->tm_min, rtc_tm->tm_sec);

        close(fd);

	fun(rtc_tm);
}
