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
#include "httpd/fsdata.h"
#include "rndis/rndis.h"
#include "webserver.h"

#define PATH_INDEX      "/index.html"
#define PATH_GENERATED  "/generated.html"
#define PATH_CGI_ACTION "/cgi/action"

extern struct fsdata_file file__index_html[];

char *html[] = { PATH_INDEX, PATH_CGI_ACTION };
char *excludePaths[] = { "/css", "/images", "/js", "/static" };
int excludePathCount = 4;

int cgiParamCount;
char **cgiParams;
char **cgiValues;

const char generated_html[] =
"<html>\n"
"<head><title>lwIP - A Lightweight TCP/IP Stack</title></head>\n"
" <body bgcolor=\"white\" text=\"black\">\n"
"  <table width=\"100%\">\n"
"   <tr valign=\"top\">\n"
"    <td width=\"80\">\n"
"     <a href=\"http://www.sics.se/\"><img src=\"/img/sics.gif\"\n"
"      border=\"0\" alt=\"SICS logo\" title=\"SICS logo\"></a>\n"
"    </td>\n"
"    <td width=\"500\">\n"
"     <h1>lwIP - A Lightweight TCP/IP Stack</h1>\n"
"     <h2>Generated page</h2>\n"
"     <p>This page might be generated in-memory at runtime</p>\n"
"    </td>\n"
"    <td>\n"
"    &nbsp;\n"
"    </td>\n"
"   </tr>\n"
"  </table>\n"
" </body>\n"
"</html>";

// Generic CGI method for capturing query params
static const char *cgi_action(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	cgiParamCount = iNumParams;
	cgiParams = pcParam;
	cgiValues = pcValue;
	return PATH_CGI_ACTION;
}

static const tCGI cgi_handlers[] =
{
	{
		PATH_CGI_ACTION,
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
	file->http_header_included = 0;
}

int fs_open_custom(struct fs_file *file, const char *name)
{
	if (!strcmp(name, PATH_GENERATED))
	{
		/* initialize fs_file correctly */
		memset(file, 0, sizeof(struct fs_file));
		file->pextension = mem_malloc(sizeof(generated_html));
		if (file->pextension != NULL)
		{
			/* instead of doing memcpy, you would generate e.g. a JSON here */
			memcpy(file->pextension, generated_html, sizeof(generated_html));
			file->data = (const char *)file->pextension;
			file->len = sizeof(generated_html) - 1; /* don't send the trailing 0 */
			file->index = file->len;
			return 1;
		}
	}

	if (!memcmp(name, PATH_CGI_ACTION, sizeof(PATH_CGI_ACTION)))
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

	bool isExclude = false;
	for (int i = 0; i < excludePathCount; i++)
		isExclude = isExclude || strstr(name, excludePaths[i]) != NULL;

	if (!isExclude)
	{
		// Redirect everything else to index page
		file->data = (const char *)file__index_html[0].data;
		file->len = file__index_html[0].len;
		file->index = file__index_html[0].len;
		file->http_header_included = file__index_html[0].http_header_included;
		file->pextension = NULL;
		file->is_custom_file = 0;
		return 1;
	}

	return 0;
}

void fs_close_custom(struct fs_file *file)
{
	if (file && file->is_custom_file && file->pextension)
	{
		mem_free(file->pextension);
		file->pextension = NULL;
	}
}
