#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "webserver.h"

extern int rndis_init();
extern void rndis_task();

char *html[2]={"/index.html","/Index.html"};

static const char *cgi_reset_usb_boot(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	reset_usb_boot(0, 0);
	return html[0];
}

static const tCGI cgi_handlers[] =
{
	{
		"/reset_usb_boot",
		cgi_reset_usb_boot
	},
};

void webserver()
{
	rndis_init();
	http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));
	while (1)
	{
		rndis_task();
	}
}
