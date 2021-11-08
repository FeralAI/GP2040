#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <string.h>
#include "pico/stdlib.h"
#include "httpd/fs.h"
#include "httpd/httpd.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "httpd/fscustom.h"
#include "httpd/fsdata.h"
#include "rndis/rndis.h"
#include "storage.h"
#include "webserver.h"
#include "GamepadStorage.h"

#define PATH_CGI_ACTION "/cgi/action"

#define API_GET_PIN_MAPPINGS "/api/getPinMappings"
#define API_SET_PIN_MAPPINGS "/api/setPinMappings"

#define LWIP_HTTPD_POST_MAX_URI_LEN 128
#define LWIP_HTTPD_POST_MAX_PAYLOAD_LEN 1024

using namespace std;

extern struct fsdata_file file__index_html[];

static vector<string> spaPaths = { "/pin-mapping" };
static vector<string> excludePaths = { "/css", "/images", "/js", "/static" };
static map<string, string> cgiParams;
static Gamepad *gamepad;
static bool is_post = false;
static char *http_post_uri;
static char http_post_payload[LWIP_HTTPD_POST_MAX_PAYLOAD_LEN];
static uint16_t http_post_payload_len = 0;

void webserver(Gamepad *instance)
{
	gamepad = instance;
	rndis_init();
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
	// memset(file, 0, sizeof(struct fs_file));
	file->data = (const char *)data;
	file->len = size;
	file->index = file->len;
	file->http_header_included = 1;
	file->pextension = NULL;
}

string to_json(string method, map<string, string> props)
{
	string json = "{ \"method\": \"" + method + "\"";

	int count = props.size();
	if (count > 0)
	{
		for (auto &nv : props)
			json = json + string(", \"") + nv.first + "\": \"" + nv.second + "\"";
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
	map<string, string> props;

	props.emplace("Up", to_string(gamepad->mapDpadUp->pin));
	props.emplace("Down", to_string(gamepad->mapDpadDown->pin));
	props.emplace("Left", to_string(gamepad->mapDpadLeft->pin));
	props.emplace("Right", to_string(gamepad->mapDpadRight->pin));
	props.emplace("B1", to_string(gamepad->mapButtonB1->pin));
	props.emplace("B2", to_string(gamepad->mapButtonB2->pin));
	props.emplace("B3", to_string(gamepad->mapButtonB3->pin));
	props.emplace("B4", to_string(gamepad->mapButtonB4->pin));
	props.emplace("L1", to_string(gamepad->mapButtonL1->pin));
	props.emplace("R1", to_string(gamepad->mapButtonR1->pin));
	props.emplace("L2", to_string(gamepad->mapButtonL2->pin));
	props.emplace("R2", to_string(gamepad->mapButtonR2->pin));
	props.emplace("S1", to_string(gamepad->mapButtonS1->pin));
	props.emplace("S2", to_string(gamepad->mapButtonS2->pin));
	props.emplace("L3", to_string(gamepad->mapButtonL3->pin));
	props.emplace("R3", to_string(gamepad->mapButtonR3->pin));
	props.emplace("A1", to_string(gamepad->mapButtonA1->pin));
	props.emplace("A2", to_string(gamepad->mapButtonA2->pin));

	return to_json(API_GET_PIN_MAPPINGS, props);
}

string setPinMappings()
{
	// Data format example:
	map<string, string> data;
	return to_json(API_SET_PIN_MAPPINGS, data);
}

/*************************
 * LWIP implementation
 *************************/


// LWIP callback on HTTP POST to validate the URI
err_t httpd_post_begin(void *connection, const char *uri, const char *http_request,
                       uint16_t http_request_len, int content_len, char *response_uri,
                       uint16_t response_uri_len, uint8_t *post_auto_wnd)
{
	LWIP_UNUSED_ARG(http_request);
	LWIP_UNUSED_ARG(http_request_len);
	LWIP_UNUSED_ARG(content_len);
	LWIP_UNUSED_ARG(response_uri);
	LWIP_UNUSED_ARG(response_uri_len);

	struct http_state *hs = (struct http_state *)connection;
	if (!uri || (uri[0] == '\0') || memcmp(uri, "/api", 4))
		return ERR_ARG;

	http_post_uri = (char *)uri;
	*post_auto_wnd = 1;
	is_post = true;
	return ERR_OK;
}

// LWIP callback on HTTP POST to for receiving payload
err_t httpd_post_receive_data(void *connection, struct pbuf *p)
{
	struct http_state *hs = (struct http_state *)connection;
	struct pbuf *q = p;

	int count;
	uint32_t http_post_payload_full_flag = 0;

	// Cache the received data to http_post_payload
	while (q != NULL)
	{
		if (http_post_payload_len + q-> len <= LWIP_HTTPD_POST_MAX_PAYLOAD_LEN)
		{
			MEMCPY(http_post_payload + http_post_payload_len, q-> payload, q-> len);
			http_post_payload_len += q-> len;
		}
		else // Buffer overflow Set overflow flag
		{
			http_post_payload_full_flag = 1;
			break;
		}

		q = q-> next;
	}

	pbuf_free (p);//  release pbuf

	// If the buffer overflows, error out
	if (http_post_payload_full_flag)
		return ERR_BUF;

	return ERR_OK;
}

// LWIP callback to set the HTTP POST response_uri, which can then be looked up via the fs_custom callbacks
void httpd_post_finished(void *connection, char *response_uri, uint16_t response_uri_len)
{
	LWIP_UNUSED_ARG(connection);
	LWIP_UNUSED_ARG(response_uri);
	LWIP_UNUSED_ARG(response_uri_len);

	response_uri = http_post_uri;
}

int fs_open_custom(struct fs_file *file, const char *name)
{
	LWIP_UNUSED_ARG(name);

	if (is_post)
	{
		if (!memcmp(http_post_uri, API_SET_PIN_MAPPINGS, sizeof(API_SET_PIN_MAPPINGS)))
		{
			string json = setPinMappings();
			set_file_data(file, json.data(), json.size());
			return 1;
		}
	}
	else
	{
		if (!memcmp(name, API_GET_PIN_MAPPINGS, sizeof(API_GET_PIN_MAPPINGS)))
		{
			string json = getPinMappings();
			set_file_data(file, json.data(), json.size());
			return 1;
		}
	}

	bool isExclude = false;
	for (auto &excludePath : excludePaths)
		if (!excludePath.compare(name))
			return 0;

	for (auto &spaPath : spaPaths)
	{
		if (!spaPath.compare(name))
		{
			file->data = (const char *)file__index_html[0].data;
			file->len = file__index_html[0].len;
			file->index = file__index_html[0].len;
			file->http_header_included = file__index_html[0].http_header_included;
			file->pextension = NULL;
			file->is_custom_file = 0;
			return 1;
		}
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

	is_post = false;
}
