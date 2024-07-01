#pragma once
#ifndef satobs_H
#define satobs_H

struct GPST
{
	short week;
	double sow;//second of GPS week
	GPST()
	{
		week = 0;
		sow = 0.0;
	}

};

struct UTC
{
	short year;
	short day;//day of year
	double sod;//second of day
	UTC()
	{
		year = 0;
		day = 0;
		sod = 0.0;
	}
};

struct satobs
{
	int obsType;//02101标识地基测距，02401标识星间测距
	int stationID;
	int satlliteID;
	//char obsType[6]; 
	//char stationID[10] ;
	//char satlliteID[10];
	char obsSign[8];
	UTC time;
	double distance;//单位：KM，D向下取整为第一个整数，将D的小数部分乘1e9（10^9）取整为第二个整数
	satobs()
	{
		//memset(obsType, '\0', sizeof(obsType));
		//memset(stationID, '\0', sizeof(stationID));
		//memset(satlliteID, '\0', sizeof(satlliteID));
		obsType = stationID = satlliteID = 0;
		memset(obsSign, '\0', sizeof(obsSign));
		distance = 0.0;
	}
};

//时间比较
inline bool& operator <(const GPST& gt1, const GPST& gt2)
{
	bool ok = false;
	if ((gt1.week * 604800 + gt1.sow) < (gt2.week * 604800 + gt2.sow))
		ok = true;
	else ok = false;
	return ok;
}

inline bool& operator >(const GPST& gt1, const GPST& gt2)
{
	bool ok = false;
	if ((gt1.week * 604800 + gt1.sow)  > (gt2.week * 604800 + gt2.sow))
		ok = true;
	else ok = false;
	return ok;
}

inline bool& operator ==(const GPST& gt1, const GPST& gt2)
{
	bool ok = false;
	if ((gt1.week * 604800 + gt1.sow) == (gt2.week * 604800 + gt2.sow))
		ok = true;
	else ok = false;
	return ok;
}

//UTC
inline bool& operator <(const UTC& t1, const UTC& t2)
{
	bool ok = false;
	if (t1.year < t2.year) ok = true;
	else if(t1.year == t2.year)
	{
		if (t1.day * 86400 + t1.sod < t2.day * 86400 + t2.sod)
			ok = true;
		else ok = false;
	}
	else ok = false;
	return ok;
}

inline bool& operator  > (const UTC& t1, const UTC& t2)
{
	bool ok = false;
	if (t1.year > t2.year) ok = true;
	else if (t1.year == t2.year)
	{
		if (t1.day * 86400 + t1.sod > t2.day * 86400 + t2.sod)
			ok = true;
		else ok = false;
	}
	else ok = false;
	return ok;
}

inline bool& operator  == (const UTC& t1, const UTC& t2)
{
	bool ok = false;
	if (t1.year == t2.year)
	{
		if (t1.day * 86400 + t1.sod == t2.day * 86400 + t2.sod)
			ok = true;
		else ok = false;
	}
	else ok = false;
	return ok;
}
#endif // !satobs_H
