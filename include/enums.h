/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef ENUMS_H_
#define ENUMS_H_

typedef enum
{
	BUTTON_LAYOUT_ARCADE,
	BUTTON_LAYOUT_HITBOX,
	BUTTON_LAYOUT_WASD,
	BUTTON_LAYOUT_UDLR,
	BUTTON_LAYOUT_MIXBOX,
	BUTTON_LAYOUT_MAMEA,
} ButtonLayout;

typedef enum
{
	BUTTON_LAYOUT_ARCADEB,
	BUTTON_LAYOUT_HITBOXB,
	BUTTON_LAYOUT_WASDB,
	BUTTON_LAYOUT_VEWLIX,
	BUTTON_LAYOUT_CAPCOM,
	BUTTON_LAYOUT_CAPCOM6,
	BUTTON_LAYOUT_SEGA2P,
	BUTTON_LAYOUT_NOIR8,
	BUTTON_LAYOUT_MAMEB,
} ButtonLayoutRight;

typedef enum
{
    STATICSPLASH,
    CLOSEIN,
    CLOSEINCUSTOM,
    NOSPLASH,
} SplashMode;

#endif
