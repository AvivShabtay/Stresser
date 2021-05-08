#pragma once

#include "String.h"
#include "AutoBuffer.h"
#include "List.h"

#define DRIVER_PREFIX "Stresser Engine: "
#define DRIVER_TAG 'Strs'
#define DEVICE_NAME L"\\Device\\StresserEngine"
#define SYMBOLIC_LINK_PATH L"\\??\\StresserEngine"
#define DRIVER_ALTITUDE L"7657.124"

using StresserString = String<DRIVER_TAG, PagedPool>;
using StresserBuffer = AutoBuffer<DRIVER_TAG, PagedPool>;