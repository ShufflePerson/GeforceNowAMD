#pragma once


struct CSessionContext
{
	//0
	char padding_1[9];
	//9
	bool m_bIsActitve;

	//10
	char padding_2[6];

	//16
	const char* m_sSessionId;

	//24
	char padding_3[56];
	void* m_cBitfrostClient;
};