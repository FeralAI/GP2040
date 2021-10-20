#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "lwip/apps/fs.h"
#include "lwip/apps/httpd.h"
#include "lwip/def.h"
#include "httpd/fscustom.h"
#include "rndis/rndis.h"
#include "webserver.h"

const char * API__BOARD__PIN_MAPPING = "/api/board/pin-mapping";

char *html[] = { "/index.html" };
char currentUri[1024];

static const char *action_reset_usb_boot(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	reset_usb_boot(0, 0);
	return html[0];
}

void webserver()
{
	rndis_init();

	while (1)
	{
		rndis_task();
	}
}

err_t httpd_post_begin(void *connection, const char *uri, const char *http_request,
                       u16_t http_request_len, int content_len, char *response_uri,
                       u16_t response_uri_len, u8_t *post_auto_wnd)
{
	LWIP_UNUSED_ARG(connection);
	LWIP_UNUSED_ARG(http_request);
	LWIP_UNUSED_ARG(http_request_len);
	LWIP_UNUSED_ARG(content_len);
	LWIP_UNUSED_ARG(post_auto_wnd);

	if (!memcmp(uri, API__BOARD__PIN_MAPPING, 22))
	{
		memcpy(currentUri, API__BOARD__PIN_MAPPING, sizeof(API__BOARD__PIN_MAPPING));

		// TODO: Assign pin values, or just debug, you know whatever

		/* e.g. for large uploads to slow flash over a fast connection, you should
			manually update the rx window. That way, a sender can only send a full
			tcp window at a time. If this is required, set 'post_aut_wnd' to 0.
			We do not need to throttle upload speed here, so: */
		*post_auto_wnd = 1;
		return ERR_OK;
	}

	return ERR_VAL;
}

err_t httpd_post_receive_data(void *connection, struct pbuf *p)
{
	err_t ret;

	LWIP_ASSERT("NULL pbuf", p != NULL);

	static uint8_t properyNameLength = 5;

	u16_t dataOffset = pbuf_memfind(p, "data=", properyNameLength, 0);
	if (dataOffset != 0xFFFF)
	{
		if (memcmp(currentUri, API__BOARD__PIN_MAPPING, sizeof(API__BOARD__PIN_MAPPING)))
		{
			// TODO: Pin mapping return data
		}

		char dataBuffer[4096]; // Limit post data to 4 kilobytes
		char *data = (char *)pbuf_get_contiguous(p, dataBuffer, sizeof(dataBuffer), p->tot_len - properyNameLength, dataOffset + properyNameLength);
		ret = ERR_OK;
	}
	else
	{
		ret = ERR_VAL;
	}

	/* this function must ALWAYS free the pbuf it is passed or it will leak memory */
	pbuf_free(p);

	return ret;
}

void httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len)
{

}

int fs_open_custom(struct fs_file *file, const char *name)
{
	// /* this example only provides one file */
	// if (!strcmp(name, "/generated.html")) {
	// 	/* initialize fs_file correctly */
	// 	memset(file, 0, sizeof(struct fs_file));
	// 	file->pextension = mem_malloc(sizeof(generated_html));
	// 	if (file->pextension != NULL) {
	// 		/* instead of doing memcpy, you would generate e.g. a JSON here */
	// 		memcpy(file->pextension, generated_html, sizeof(generated_html));
	// 		file->data = (const char *)file->pextension;
	// 		file->len = sizeof(generated_html) - 1; /* don't send the trailing 0 */
	// 		file->index = file->len;
	// 		/* allow persisteng connections */
	// 		file->flags = FS_FILE_FLAGS_HEADER_PERSISTENT;
	// 		return 1;
	// 	}
	// }
	return 0;
}

void fs_close_custom(struct fs_file *file)
{
	// if (file && file->pextension)
	// {
	// 	mem_free(file->pextension);
	// 	file->pextension = NULL;
	// }
}
