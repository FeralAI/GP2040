/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "xinput_driver.h"

uint8_t endpoint_in = 0;
uint8_t endpoint_out = 0;

bool send_xinput_report(void *report, uint8_t report_size)
{
	bool sent = false;

	if (
		(tud_ready()) &&                                         // Is the device ready?
		(endpoint_in != 0) && (!usbd_edpt_busy(0, endpoint_in))  // Is the IN endpoint available?
	) {
		usbd_edpt_claim(0, endpoint_in);                         // Take control of IN endpoint
		usbd_edpt_xfer(0, endpoint_in, report, report_size);     // Send report buffer
		usbd_edpt_release(0, endpoint_in);                       // Release control of IN endpoint
		sent = true;
	}

	return sent;
}

static void xinput_init(void)
{

}

static void xinput_reset(uint8_t __unused rhport)
{

}

static uint16_t xinput_open(uint8_t __unused rhport, tusb_desc_interface_t const *itf_descriptor, uint16_t max_length)
{
	uint16_t driver_length = sizeof(tusb_desc_interface_t) + (itf_descriptor->bNumEndpoints * sizeof(tusb_desc_endpoint_t)) + 16;

	TU_VERIFY(max_length >= driver_length, 0);

	uint8_t const *current_descriptor = tu_desc_next(itf_descriptor);
	uint8_t found_endpoints = 0;
	while ((found_endpoints < itf_descriptor->bNumEndpoints) && (driver_length <= max_length))
	{
		tusb_desc_endpoint_t const *endpoint_descriptor = (tusb_desc_endpoint_t const *)current_descriptor;
		if (TUSB_DESC_ENDPOINT == tu_desc_type(endpoint_descriptor))
		{
			TU_ASSERT(usbd_edpt_open(rhport, endpoint_descriptor));

			if (tu_edpt_dir(endpoint_descriptor->bEndpointAddress) == TUSB_DIR_IN)
				endpoint_in = endpoint_descriptor->bEndpointAddress;
			else
				endpoint_out = endpoint_descriptor->bEndpointAddress;

			++found_endpoints;
		}

		current_descriptor = tu_desc_next(current_descriptor);
	}
	return driver_length;
}

static bool xinput_device_control_request(uint8_t __unused rhport, tusb_control_request_t __unused const *request)
{
	return true;
}

static bool xinput_control_complete(uint8_t __unused rhport, tusb_control_request_t __unused const *request)
{
	return true;
}

static bool xinput_xfer_callback(uint8_t __unused rhport, uint8_t __unused ep_addr, xfer_result_t __unused result, uint32_t __unused xferred_bytes)
{
	return true;
}

const usbd_class_driver_t xinput_driver =
{
#if CFG_TUSB_DEBUG >= 2
	.name = "XINPUT",
#endif
	.init = xinput_init,
	.reset = xinput_reset,
	.open = xinput_open,
	.control_request = xinput_device_control_request,
	.control_complete = xinput_control_complete,
	.xfer_cb = xinput_xfer_callback,
	.sof = NULL
};
