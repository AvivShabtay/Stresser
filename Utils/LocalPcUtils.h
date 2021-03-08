#pragma once

/*
 * Include helper methods to query and preform actions on the local PC.
 */
class LocalPcUtils final
{
public:
	/* Return the local computer name in string (ASCII) format. */
	static std::wstring getLocalComputerName();
};

