///@file A set of functions for dealing with FAT dates.

#ifndef _TIME_AND_DATE_H
#define _TIME_AND_DATE_H

#include <time.h>

#pragma pack(push)
#pragma pack(1)

///@brief A struct to hold a file time.
typedef struct FILE_TIME
{
	unsigned int hours			:	5;
	unsigned int minutes		:	6;
	unsigned int doubleseconds	:	5;
} FILE_TIME;

///@brief A struct to hold a file date.
typedef struct FILE_DATE
{
	unsigned int year		:	7;
	unsigned int month		:	4;
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

///@brief Provides a human-readable date string given a FILE_DATE object.
///@param[in]	date	A FILE_DATE struct.
///@return	Returns a char pointer to an internal static buffer.  (Not threadsafe)
//char* getHumanReadableDate(FILE_DATE date);

///@brief Provides a human-readable timee string given a FILE_TIME object.
///@param[in]	date	A FILE_TIME struct.
///@return	Returns a char pointer to an internal static buffer.  (Not threadsafe)
//char* getHumanReadableTime(FILE_TIME time);

#endif