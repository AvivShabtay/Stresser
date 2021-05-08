#pragma once

#include <ntifs.h>

/*
 * Overloading the new operator.
 * @note The first parameter must be size_t size.
 * @note Using the new operator will call the object constructor automatically.
 * @usage:
 *		auto data = new (NonPagedPool, YOUR_DRIVER_TAG) MyDataObject;
 */
void* __cdecl operator new(size_t size, POOL_TYPE type, ULONG tag = 0);

/*
 * Overloading placement new operator.
 * Get allocated memory (not allocating by itself) and construct the object in it.
 * @note JUST constructing the object in that memory.

 * @usage:
 *		auto buffer = (..)ExAllocatePoolWithTag(..);	// allocated in some way
 *		auto data = new (buffer) MyDataObject;			// redundant
 *		new(buffer) MyDataObject;						// the right way
 *
 * @note DO NOT call the delete operator on the buffer, because you cannot know
 * how it got allocated in first place. instead call the object destructor explicitly:
 *		data->~MyDataObject();
 */
void* __cdecl operator new(size_t size, PVOID pointer);

/*
 * Overloading the delete operator.
 */
void operator delete(PVOID pointer, size_t size);