#pragma once
#include <cstdint>


namespace NVB {
	class CBifrostClient
	{
	public:
		char pad_0000[88]; //0x0000
		class CNetwork* m_cNetwork; //0x0058
		class CClientCallbacks* m_cClientCallbacks; //0x0060
		class CScheduler* m_cScheduler; //0x0068
		class CGridServer* m_cGridServer; //0x0070
		char pad_0078[8]; //0x0078
		bool m_bIsConnected; //0x0080
		char pad_0081[559]; //0x0081
	}; //Size: 0x02B0

	class CNetwork
	{
	public:
		char pad_0000[1288]; //0x0000
	}; //Size: 0x0508

	class CClientCallbacks
	{
	public:
		char pad_0000[8]; //0x0000
	}; //Size: 0x0008

	class CScheduler
	{
	public:
		char pad_0000[24]; //0x0000
	}; //Size: 0x0018

	class CGridServer
	{
	public:
		char pad_0000[264]; //0x0000
	}; //Size: 0x0108

	class CObserver
	{
	public:
		char pad_0000[8]; //0x0000
	}; //Size: 0x0008

	class CObservable
	{
	public:
		char pad_0000[8]; //0x0000
	}; //Size: 0x0008
}