#include <stdio.h>
#include <string>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "httpd/fs.h"
#include "httpd/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "httpd/fscustom.h"
#include "rndis/rndis.h"
#include "webserver.h"

#define CGI_ACTION "/cgi/action"

char *html[]={ "/index.html", CGI_ACTION };

int cgiParamCount;
char **cgiParams;
char **cgiValues;

static const char *cgi_action(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	cgiParamCount = iNumParams;
	cgiParams = pcParam;
	cgiValues = pcValue;
	return CGI_ACTION;
}

static const tCGI cgi_handlers[] =
{
	{
		CGI_ACTION,
		cgi_action
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

void set_file_data(struct fs_file *file, const char *data, size_t size)
{
	memset(file, 0, sizeof(struct fs_file));
	file->pextension = mem_malloc(size);
	memcpy(file->pextension, data, size);
	file->data = (const char *)file->pextension;
	file->len = size;
	file->index = file->len;
	file->is_custom_file = 1;
	file->http_header_included = 0;
}

int fs_open_custom(struct fs_file *file, const char *name)
{
	if (!memcmp(name, CGI_ACTION, sizeof(CGI_ACTION)))
	{
		std::string message("{ \"success\": true");
		if (cgiParamCount > 0)
		{
			for (int i = 0; i < cgiParamCount; i++)
				message = message + std::string(", \"") + cgiParams[i] + "\": \"" + cgiValues[i] + "\"";
		}
		message += " }";

		set_file_data(file, message.data(), message.length());
		return 1;
	}

	return 0;
}

void fs_close_custom(struct fs_file *file)
{
	if (file && file->pextension)
	{
		mem_free(file->pextension);
		file->pextension = NULL;
	}
}
