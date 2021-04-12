#include "pch.h"
#include "SehTranslatorGuard.h"
#include "SehException.h"

SehTranslatorGuard::SehTranslatorGuard()
	: m_originalSehTranslator(nullptr)
{
	this->m_originalSehTranslator = _set_se_translator(SehTranslatorGuard::sehTranslatorFunction);
}

SehTranslatorGuard::~SehTranslatorGuard()
{
	try
	{
		if (nullptr != this->m_originalSehTranslator)
		{
			_set_se_translator(this->m_originalSehTranslator);
		}
	}
	catch (...)
	{
		// Internationally left blank
	}
}

void SehTranslatorGuard::sehTranslatorFunction(unsigned errorCode, EXCEPTION_POINTERS*)
{
	throw SehException(errorCode);
}