#ifndef _STDLISTDEVICESSORT_H_
#define _STDLISTDEVICESSORT_H_
#include "AvGuiSystem.h"



class CmpareIpAddressFromLargeToSmall
{
public:
	bool operator()(const C_DevSearch s1, const C_DevSearch s2) const{
		unsigned int nAddr1, nAddr2;
		nAddr1 = inet_addr(s1.NetWorkProfile.Ipv4);
		nAddr2 = inet_addr(s2.NetWorkProfile.Ipv4);
		return nAddr1 > nAddr2;
	}
};

class CmpareIpAddressFromSmallToLarge
{
public:
	bool operator()(const C_DevSearch s1, const C_DevSearch s2) const{
		unsigned int nAddr1, nAddr2;
		nAddr1 = inet_addr(s1.NetWorkProfile.Ipv4);
		nAddr2 = inet_addr(s2.NetWorkProfile.Ipv4);
		return nAddr1 < nAddr2;
	}
};

class CmpareGateAddressFromLargeToSmall
{
public:
	bool operator()(const C_DevSearch s1, const C_DevSearch s2) const{
		unsigned int nAddr1, nAddr2;
		nAddr1 = inet_addr(s1.NetWorkProfile.GateWay);
		nAddr2 = inet_addr(s2.NetWorkProfile.GateWay);
		return nAddr1 > nAddr2;
	}

};
class CmpareGateAddressFromSmallToLarge
{
public:
	bool operator()(const C_DevSearch s1, const C_DevSearch s2) const{
		unsigned int nAddr1, nAddr2;
		nAddr1 = inet_addr(s1.NetWorkProfile.GateWay);
		nAddr2 = inet_addr(s2.NetWorkProfile.GateWay);
		return nAddr1 < nAddr2;
	}

};

class CmpareStartUpFromLargeToSmall
{
public:
	bool operator()(const C_DevSearch s1, const C_DevSearch s2) const {
		return s1.DevStatusInfo.uptime > s2.DevStatusInfo.uptime;
	}

};

class CmpareStartUpFromSmallToLarge
{
public:
	bool operator()(const C_DevSearch s1, const C_DevSearch s2) const {
		return s1.DevStatusInfo.uptime < s2.DevStatusInfo.uptime;
	}

};











#endif