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
#define PATH_CGI_ACTION "/cgi/action"

#define ACTION_GET_ECHO_PARAMS  "echoParams"
#define ACTION_GET_PIN_MAPPINGS "getPinMappings"
#define ACTION_SET_PIN_MAPPINGS "setPinMappings"

extern struct fsdata_file file__index_html[];

char *excludePaths[] = { "/css", "/images", "/js", "/static" };
int excludePathCount = 4;

int cgiParamCount;
char **cgiParams;
char **cgiValues;

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

char *get_param(char *name, char **params, char **values, int count)
{
	for (int i = 0; i < count; i++) {
		if (strncmp(name, params[i], sizeof(name)))
			return values[i];
	}

	return NULL;
}

std::string to_json(bool success, char **props, char **values, int count)
{
	std::string json;
	if (success)
		json += "{ \"success\": true";
	else
		json += "{ \"success\": false";

	if (count > 0)
	{
		for (int i = 0; i < count; i++)
			json = json + std::string(", \"") + props[i] + "\": \"" + values[i] + "\"";
	}

	json += " }";

	return json;
}

int fs_open_custom(struct fs_file *file, const char *name)
{
	if (!memcmp(name, PATH_CGI_ACTION, sizeof(PATH_CGI_ACTION)))
	{
		if (!strncmp(name, ACTION_GET_ECHO_PARAMS, sizeof(ACTION_GET_ECHO_PARAMS)))
		{
			std::string json = to_json(true, cgiParams, cgiValues, cgiParamCount);
			set_file_data(file, json.data(), json.length());
		}
		else if (!strncmp(name, ACTION_GET_PIN_MAPPINGS, sizeof(ACTION_GET_PIN_MAPPINGS)))
		{

		}
		else if (!strncmp(name, ACTION_SET_PIN_MAPPINGS, sizeof(ACTION_SET_PIN_MAPPINGS)))
		{

		}

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
