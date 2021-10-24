#include <stdio.h>
#include <vector>
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

#define METHOD_GET_ECHO_PARAMS  "echoParams"
#define METHOD_GET_PIN_MAPPINGS "getPinMappings"
#define METHOD_SET_PIN_MAPPINGS "setPinMappings"

using namespace std;

extern struct fsdata_file file__index_html[];

static char *excludePaths[] = { "/css", "/images", "/js", "/static" };
static int excludePathCount = 4;

static int cgiParamCount;
static vector<string> cgiParams;
static vector<string> cgiValues;
static Gamepad *gamepad;

// Generic CGI method for capturing query params
static const char *cgi_action(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	cgiParams.clear();
	cgiValues.clear();
	cgiParamCount = iNumParams;

	for (int i = 0; i < cgiParamCount; i++)
	{
		cgiParams.push_back(pcParam[i]);
		cgiValues.push_back(pcValue[i]);
	}

	return PATH_CGI_ACTION;
}

static const tCGI cgi_handlers[] =
{
	{
		PATH_CGI_ACTION,
		cgi_action
	},
};

void webserver(Gamepad *instance)
{
	gamepad = instance;
	rndis_init();
	http_set_cgi_handlers(cgi_handlers, LWIP_ARRAYSIZE(cgi_handlers));
	while (1)
	{
		rndis_task();
	}
}

/*************************
 * Helper methods
 *************************/

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

string get_param_value(string name)
{
	for (int i = 0; i < cgiParamCount; i++) {
		if (!name.compare(cgiParams[i]))
			return cgiValues[i];
	}

	return NULL;
}

string to_json(bool success, vector<string> props, vector<string> values, int count)
{
	string json;
	if (success)
		json += "{ \"success\": true";
	else
		json += "{ \"success\": false";

	if (count > 0)
	{
		for (int i = 0; i < count; i++)
			json = json + string(", \"") + props[i] + "\": \"" + values[i] + "\"";
	}

	json += " }";

	return json;
}

/*************************
 * API methods
 *************************/

string getPinMappings()
{
	string result;
	vector<string> props;
	vector<string> values;

	props.push_back("Up");    values.push_back(to_string(gamepad->mapDpadUp->pin));
	props.push_back("Down");  values.push_back(to_string(gamepad->mapDpadDown->pin));
	props.push_back("Left");  values.push_back(to_string(gamepad->mapDpadLeft->pin));
	props.push_back("Right"); values.push_back(to_string(gamepad->mapDpadRight->pin));
	props.push_back("B1");    values.push_back(to_string(gamepad->mapButtonB1->pin));
	props.push_back("B2");    values.push_back(to_string(gamepad->mapButtonB2->pin));
	props.push_back("B3");    values.push_back(to_string(gamepad->mapButtonB3->pin));
	props.push_back("B4");    values.push_back(to_string(gamepad->mapButtonB4->pin));
	props.push_back("L1");    values.push_back(to_string(gamepad->mapButtonL1->pin));
	props.push_back("R1");    values.push_back(to_string(gamepad->mapButtonR1->pin));
	props.push_back("L2");    values.push_back(to_string(gamepad->mapButtonL2->pin));
	props.push_back("R2");    values.push_back(to_string(gamepad->mapButtonR2->pin));
	props.push_back("S1");    values.push_back(to_string(gamepad->mapButtonS1->pin));
	props.push_back("S2");    values.push_back(to_string(gamepad->mapButtonS2->pin));
	props.push_back("L3");    values.push_back(to_string(gamepad->mapButtonL3->pin));
	props.push_back("R3");    values.push_back(to_string(gamepad->mapButtonR3->pin));
	props.push_back("A1");    values.push_back(to_string(gamepad->mapButtonA1->pin));
	props.push_back("A2");    values.push_back(to_string(gamepad->mapButtonA2->pin));

	return to_json(true, props, values, props.size());
}

/*************************
 * LWIP implementation
 *************************/

int fs_open_custom(struct fs_file *file, const char *name)
{
	if (!memcmp(name, PATH_CGI_ACTION, sizeof(PATH_CGI_ACTION)))
	{
		string method = get_param_value("method");

		if (!method.compare(METHOD_GET_ECHO_PARAMS))
		{
			string json = to_json(true, cgiParams, cgiValues, cgiParamCount);
			set_file_data(file, json.data(), json.length());
			return 1;
		}
		else if (!method.compare(METHOD_GET_PIN_MAPPINGS))
		{
			string json = getPinMappings();
			set_file_data(file, json.data(), json.length());
			return 1;
		}
		else if (!method.compare(METHOD_SET_PIN_MAPPINGS))
		{

			return 1;
		}
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
