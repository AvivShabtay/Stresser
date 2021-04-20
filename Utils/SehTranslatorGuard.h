#pragma once

#include <eh.h>
#include <Windows.h>

/*
 * Set a per-thread callback function to translate Win32 SEH exception into C++ exceptions.
 * @note You must use /EHa when using _set_se_translator.
 * @note _se_translator_function is the type of the translator function.
 * @note _set_se_translator is the function that used to set the translator function.
 */
class SehTranslatorGuard final
{
public:
	SehTranslatorGuard();

	~SehTranslatorGuard();

	// Delete copy constructor, assignment operator, move constructor, move operator:
	SehTranslatorGuard(const SehTranslatorGuard&) = delete;
	SehTranslatorGuard& operator=(const SehTranslatorGuard&) = delete;
	SehTranslatorGuard(SehTranslatorGuard&&) = delete;
	SehTranslatorGuard& operator=(SehTranslatorGuard&&) = delete;

private:
	static void sehTranslatorFunction(unsigned int errorCode, EXCEPTION_POINTERS*);

	_se_translator_function m_originalSehTranslator;
};