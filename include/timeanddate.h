///@file A set of functions for dealing with FAT dates.

#ifndef _TIME_AND_DATE_H
#define _TIME_AND_DATE_H

#include <time.h>

#pragma pack(push)
#pragma pack(1)

///@brief A struct to hold a file time.
typedef struct FILE_TIME
{
	///@brief Five bits to hold number of seconds (divided by 2).
	unsigned int doubleseconds	:	5;
	///@brief Six bits to hold number of minutes.
	unsigned int minutes		:	6;
	///@brief Five bits to hold number of hours.
	unsigned int hours			:	5;
} FILE_TIME;

///@brief A struct to hold a file date.
typedef struct FILE_DATE
{
	///@brief Four bits to hold month.
	unsigned int month		:	4;
	///@brief Seven bits to hold years (since 1980).
	unsigned int year		:	7;
	///@brief Five bits to hold day.
	unsigned int day		:	5;
} FILE_DATE;

#pragma pack(pop)

///@brief Populates a FILE_TIME and a FILE_DATE from a time_t provided.  Both the FILE_TIME and FILE_DATE pointers can be NULL.
///@param[in]	in		A time_t object.
///@param[out]	time	A FILE_TIME object to fill. (Can be NULL to ignore.)
///@param[out]	date	A FILE_DATE object to fill. (Can be NULL to ignore.)
void createFileDateTime(time_t in, FILE_DATE *date, FILE_TIME *time);

///@brief Populates a tm struct given a FILE_DATE and a FILE_TIME.  It is possible to simply put NULL in for either field if unavailable.
///@param[in]	date	A FILE_DATE object.
///@param[in]	time	A FILE_TIME object.
///@param[out]	out		A pointer to an allocated tm struct. Can be NULL.
///@return Returns a time_t of the time inputted.
time_t timeDateToCTime(const FILE_DATE *date, const FILE_TIME *time, struct tm *out);


///@brief Populates a pre-allocated string buffer with the date and/or time provided.
///@param[in]	date	An optional FILE_DATE object. (Use NULL to negate.)
///@param[in]	time	An optional FILE_TIME object. (Use NULL to negate.)
///@param[out]	out		A pre-allocated string buffer large enough to contain the date and/or time string produced.
void getHumanReadableDateTimeString(const FILE_DATE *date, const FILE_TIME *time, char *out);

///@brief Provides a human-readable date string given a FILE_DATE object.
///@param[in]	date	A FILE_DATE struct.
///@return	Returns a char pointer to an internal static buffer.  (Not threadsafe)
//char* getHumanReadableDate(FILE_DATE date);

///@brief Provides a human-readable timee string given a FILE_TIME object.
///@param[in]	date	A FILE_TIME struct.
///@return	Returns a char pointer to an internal static buffer.  (Not threadsafe)
//char* getHumanReadableTime(FILE_TIME time);

#endif