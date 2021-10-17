/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef WEBSERVER_DESCRIPTORS_H_
#define WEBSERVER_DESCRIPTORS_H_

#include <stdint.h>

static const uint8_t webserver_string_language[]     = { 0x09, 0x04 };
static const uint8_t webserver_string_manufacturer[] = "TinyUSB";
static const uint8_t webserver_string_product[]      = "USB Webserver";
static const uint8_t webserver_string_version[]      = "1.0";

static const uint8_t *webserver_string_descriptors[] =
{
	webserver_string_language,
	webserver_string_manufacturer,
	webserver_string_product,
	webserver_string_version
};

static const uint8_t webserver_device_descriptor[] =
{
	0x12,        // bLength
	0x01,        // bDescriptorType (Device)
	0x00, 0x02,  // bcdUSB 2.00
	0xEF,        // bDeviceClass
	0x02,        // bDeviceSubClass
	0x01,        // bDeviceProtocol
	0x40,        // bMaxPacketSize0 64
	0xFE, 0xCA,  // idVendor 0xCAFE
	0x21, 0x40,  // idProduct 0x4021
	0x01, 0x01,  // bcdDevice 2.01
	0x01,        // iManufacturer (String Index)
	0x02,        // iProduct (String Index)
	0x03,        // iSerialNumber (String Index)
	0x02,        // bNumConfigurations 2
};

static const uint8_t webserver_configuration_descriptor[] =
{
	0x09,        // bLength
	0x02,        // bDescriptorType (Configuration)
	0x4B, 0x00,  // wTotalLength 75
	0x02,        // bNumInterfaces 2
	0x01,        // bConfigurationValue
	0x00,        // iConfiguration (String Index)
	0x80,        // bmAttributes
	0x32,        // bMaxPower 100mA

	0x08,        // bLength
	0x0B,        // bDescriptorType (Unknown)
	0x00, 0x02, 0xE0, 0x01, 0x03, 0x00,
	0x09,        // bLength
	0x04,        // bDescriptorType (Interface)
	0x00,        // bInterfaceNumber 0
	0x00,        // bAlternateSetting
	0x01,        // bNumEndpoints 1
	0xE0,        // bInterfaceClass
	0x01,        // bInterfaceSubClass
	0x03,        // bInterfaceProtocol
	0x04,        // iInterface (String Index)

	0x05,        // bLength
	0x24,        // bDescriptorType (Dependant on Type)
	0x00, 0x10, 0x01,
	0x05,        // bLength
	0x24,        // bDescriptorType (Dependant on Type)
	0x01, 0x00, 0x01,
	0x04,        // bLength
	0x24,        // bDescriptorType (Dependant on Type)
	0x02, 0x00,
	0x05,        // bLength
	0x24,        // bDescriptorType (Dependant on Type)
	0x06, 0x00, 0x01,
	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x81,        // bEndpointAddress (IN/D2H)
	0x03,        // bmAttributes (Interrupt)
	0x08, 0x00,  // wMaxPacketSize 8
	0x01,        // bInterval 1 (unit depends on device speed)

	0x09,        // bLength
	0x04,        // bDescriptorType (Interface)
	0x01,        // bInterfaceNumber 1
	0x00,        // bAlternateSetting
	0x02,        // bNumEndpoints 2
	0x0A,        // bInterfaceClass
	0x00,        // bInterfaceSubClass
	0x00,        // bInterfaceProtocol
	0x00,        // iInterface (String Index)

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x82,        // bEndpointAddress (IN/D2H)
	0x02,        // bmAttributes (Bulk)
	0x40, 0x00,  // wMaxPacketSize 64
	0x00,        // bInterval 0 (unit depends on device speed)

	0x07,        // bLength
	0x05,        // bDescriptorType (Endpoint)
	0x02,        // bEndpointAddress (OUT/H2D)
	0x02,        // bmAttributes (Bulk)
	0x40, 0x00,  // wMaxPacketSize 64
	0x00,        // bInterval 0 (unit depends on device speed)
};

#endif
