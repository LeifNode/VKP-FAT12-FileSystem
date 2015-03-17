#include "timeanddate.h"

//An array holding month names.
const char *MONTHS_STR[] =
{
	"January",
	"February",
	"March",
	"April",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

void createFileDateTime(time_t in, FILE_DATE *date, FILE_TIME *time)
{
	struct tm * timeinfo;
	
	//Get the local time from the time_t object passed in.
	timeinfo = localtime(&in);
	
	if(date)
	{
		//Convert from days since 1900 to days since 1980.
		date->year	=	timeinfo->tm_year - 80;
		
		date->month	=	timeinfo->tm_mon;
		date->day	=	timeinfo->tm_mday;
	}
	
	if(time)
	{
		time->hours		=	timeinfo->tm_hour;
		time->minutes	=	timeinfo->tm_min;
		
		//Need to convert seconds to doubleseconds.
		time->doubleseconds	=	timeinfo->tm_sec / 2;
	}
}

time_t timeDateToCTime(const FILE_DATE *date, const FILE_TIME *time, struct tm *out)
{
	//Allocate a temp just in case.
	struct tm temp = {0};
	
	//A pointer to whatever tm struct we are working with.
	struct tm *t;
	
	//Use the temp if we weren't given a tm struct to play with.
	if(out)
	{
		t = out;
	}
	else
	{
		t = &temp;
	}
	
	if(time)
	{
		//Need to convert doubleseconds to seconds.
		t->tm_sec	=	time->doubleseconds / 2;
		
		t->tm_hour	=	time->hours;
		t->tm_min	=	time->minutes;
	}
	
	if(date)
	{
		t->tm_mday	=	date->day;
		t->tm_mon	=	date->month;
		
		//Need to convert from years since 1980 to years since 1900.
		out->tm_year	=	date->year + 80;
	}
	
	//This simultaneously returns a valid time_t object and also fixes up our tm struct wday and yday fields.
	return mktime(t);
}

//char* getHumanReadableDate(FILE_DATE date)

//char* getHumanReadableTime(FILE_TIME time)