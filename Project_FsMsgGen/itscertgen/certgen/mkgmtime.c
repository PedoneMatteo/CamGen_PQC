// Found in DeSmuME
// Alexis modified it a tad bit so it would compile as C (opposed to C++)

/*  lib/mkgmtime.c

    Copyright (C) 2010 DeSmuME team

    This file is part of DeSmuME

    DeSmuME is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    DeSmuME is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DeSmuME; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

//Taken from newlib 1.18.0 which is licensed under GPL 2 and modified for desmume

/*
 * mktime.c
 * Original Author:  G. Haley
 *
 * Converts the broken-down time, expressed as local time, in the structure
 * pointed to by tim_p into a calendar time value. The original values of the
 * tm_wday and tm_yday fields of the structure are ignored, and the original
 * values of the other fields have no restrictions. On successful completion
 * the fields of the structure are set to represent the specified calendar
 * time. Returns the specified calendar time. If the calendar time can not be
 * represented, returns the value (time_t) -1.
 *
 * Modifications:  Fixed tm_isdst usage - 27 August 2008 Craig Howland.
 */

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS) 
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "mkgmtime.h"

#define _SEC_IN_MINUTE 60L
#define _SEC_IN_HOUR 3600L
#define _SEC_IN_DAY 86400L

static const int DAYS_IN_MONTH[12] =
{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define _DAYS_IN_MONTH(x) ((x == 1) ? days_in_feb : DAYS_IN_MONTH[x])

static const int _DAYS_BEFORE_MONTH[12] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

#define _ISLEAP(y) (((y) % 4) == 0 && (((y) % 100) != 0 || (((y)+1900) % 400) == 0))
#define _DAYS_IN_YEAR(year) (_ISLEAP(year) ? 366 : 365)

static void validate_structure(struct tm *tim_p)
{
  div_t res;
  int days_in_feb = 28;

  /* calculate time & date to account for out of range values */
  if (tim_p->tm_sec < 0 || tim_p->tm_sec > 59)
    {
      res = div (tim_p->tm_sec, 60);
      tim_p->tm_min += res.quot;
      if ((tim_p->tm_sec = res.rem) < 0)
        {
          tim_p->tm_sec += 60;
          --tim_p->tm_min;
        }
    }

  if (tim_p->tm_min < 0 || tim_p->tm_min > 59)
    {
      res = div (tim_p->tm_min, 60);
      tim_p->tm_hour += res.quot;
      if ((tim_p->tm_min = res.rem) < 0)
        {
          tim_p->tm_min += 60;
          --tim_p->tm_hour;
        }
    }

  if (tim_p->tm_hour < 0 || tim_p->tm_hour > 23)
    {
      res = div (tim_p->tm_hour, 24);
      tim_p->tm_mday += res.quot;
      if ((tim_p->tm_hour = res.rem) < 0)
        {
          tim_p->tm_hour += 24;
          --tim_p->tm_mday;
        }
    }

  if (tim_p->tm_mon > 11)
    {
      res = div (tim_p->tm_mon, 12);
      tim_p->tm_year += res.quot;
      if ((tim_p->tm_mon = res.rem) < 0)
        {
          tim_p->tm_mon += 12;
          --tim_p->tm_year;
        }
    }

  if (_DAYS_IN_YEAR (tim_p->tm_year) == 366)
    days_in_feb = 29;

  if (tim_p->tm_mday <= 0)
    {
      while (tim_p->tm_mday <= 0)
        {
          if (--tim_p->tm_mon == -1)
            {
              tim_p->tm_year--;
              tim_p->tm_mon = 11;
              days_in_feb =
                ((_DAYS_IN_YEAR (tim_p->tm_year) == 366) ?
                 29 : 28);
            }
          tim_p->tm_mday += _DAYS_IN_MONTH (tim_p->tm_mon);
        }
    }
  else
    {
      while (tim_p->tm_mday > _DAYS_IN_MONTH (tim_p->tm_mon))
        {
          tim_p->tm_mday -= _DAYS_IN_MONTH (tim_p->tm_mon);
          if (++tim_p->tm_mon == 12)
            {
              tim_p->tm_year++;
              tim_p->tm_mon = 0;
              days_in_feb =
                ((_DAYS_IN_YEAR (tim_p->tm_year) == 366) ?
                 29 : 28);
            }
        }
    }
}


static const unsigned long _leap_moments[] = {
	1136073600,
	1230768000,
	1341100800,
	1435708800,
};

time_t addleapseconds(time_t t)
{
	int i;
	for (i = 0; i < sizeof(_leap_moments) / sizeof(_leap_moments[0]); i++){
		if (t < _leap_moments[i]) break;
	}
	return t + i;
}

time_t removeleapseconds(time_t t)
{
	int i;
	for (i = 0; i < sizeof(_leap_moments) / sizeof(_leap_moments[0]); i++){
		if (t < _leap_moments[i]) break;
		t--;
	}
	return t;
}

#define ITS_UTC_EPOCH 1072915200

unsigned long mkitstime32(struct tm *tim_p)
{
	time_t ret = mktaitime(tim_p);
	if (ret > 0){
		ret -= ITS_UTC_EPOCH;
	}
	return (unsigned long)ret;
}

unsigned long long mkitstime64(struct tm *tim_p)
{
	unsigned long long ret = mktaitime64(tim_p);
	if (ret > 0){
		ret -= ((unsigned long long)ITS_UTC_EPOCH) * 1000;
	}
	return ret;
}

time_t mktaitime(struct tm *tim_p)
{
	time_t t = mkgmtime(tim_p);
	if (t >= 0){
		t = addleapseconds(t);
	}
	return t;
}

unsigned long long mktaitime64(struct tm *tim_p)
{
	time_t t = mkgmtime(tim_p);
	if (t >= 0){
		t = addleapseconds(t);
	}
	return ((unsigned long long)t)*1000;
}

unsigned long unix2itstime32(time_t t)
{
	return ((unsigned long) addleapseconds(t)) - ITS_UTC_EPOCH;
}

unsigned long long unix2itstime64(time_t t)
{
	return (((unsigned long long) addleapseconds(t)) - ITS_UTC_EPOCH) * 1000;
}

static char _datebuf[8][16];
static int _datebufidx = 0;
const char * strgmtdate(time_t t)
{
	struct tm * tm;
	char * b = _datebuf[_datebufidx];
	_datebufidx = (_datebufidx + 1) & 7;

	tm = gmtime(&t);
	sprintf(b, "%u-%02u-%02u", 1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday);
	return b;
}

const char * strtaidate(time_t t)
{
	return strgmtdate(removeleapseconds(t));
}

const char * stritsdate32(time_t t)
{
	return strtaidate(t + ITS_UTC_EPOCH);
}

const char * stritsdate64(unsigned long long t)
{
	/* this is microseconds */
	return stritsdate32(t / 1000000);
}

time_t mkgmtime(struct tm *tim_p)
{
  time_t tim = 0;
  long days = 0;
  int year, isdst, tm_isdst;

  /* validate structure */
  validate_structure (tim_p);

  /* compute hours, minutes, seconds */
  tim += tim_p->tm_sec + (tim_p->tm_min * _SEC_IN_MINUTE) +
    (tim_p->tm_hour * _SEC_IN_HOUR);

  /* compute days in year */
  days += tim_p->tm_mday - 1;
  days += _DAYS_BEFORE_MONTH[tim_p->tm_mon];
  if (tim_p->tm_mon > 1 && _DAYS_IN_YEAR (tim_p->tm_year) == 366)
    days++;

  /* compute day of the year */
  tim_p->tm_yday = days;

  if (tim_p->tm_year > 10000
      || tim_p->tm_year < -10000)
    {
      return (time_t) -1;
    }

  /* compute days in other years */
  if (tim_p->tm_year > 70)
    {
      for (year = 70; year < tim_p->tm_year; year++)
        days += _DAYS_IN_YEAR (year);
    }
  else if (tim_p->tm_year < 70)
    {
      for (year = 69; year > tim_p->tm_year; year--)
        days -= _DAYS_IN_YEAR (year);
      days -= _DAYS_IN_YEAR (year);
    }

  /* compute day of the week */
  if ((tim_p->tm_wday = (days + 4) % 7) < 0)
    tim_p->tm_wday += 7;

  /* compute total seconds */
  tim += (days * _SEC_IN_DAY);

  /* Convert user positive into 1 */
  tm_isdst = tim_p->tm_isdst > 0  ?  1 : tim_p->tm_isdst;
  isdst = tm_isdst;

  //screw this!

 // if (_daylight)
 //   {
 //     int y = tim_p->tm_year + YEAR_BASE;
 //     if (y == tz->__tzyear || __tzcalc_limits (y))
        //{
        //  /* calculate start of dst in dst local time and 
        //     start of std in both std local time and dst local time */
 //         time_t startdst_dst = tz->__tzrule[0].change
        //    - (time_t) tz->__tzrule[1].offset;
        //  time_t startstd_dst = tz->__tzrule[1].change
        //    - (time_t) tz->__tzrule[1].offset;
        //  time_t startstd_std = tz->__tzrule[1].change
        //    - (time_t) tz->__tzrule[0].offset;
        //  /* if the time is in the overlap between dst and std local times */
        //  if (tim >= startstd_std && tim < startstd_dst)
        //    ; /* we let user decide or leave as -1 */
 //         else
        //    {
        //      isdst = (tz->__tznorth
        //               ? (tim >= startdst_dst && tim < startstd_std)
        //               : (tim >= startdst_dst || tim < startstd_std));
 //              /* if user committed and was wrong, perform correction, but not
 //               * if the user has given a negative value (which
 //               * asks mktime() to determine if DST is in effect or not) */
 //              if (tm_isdst >= 0  &&  (isdst ^ tm_isdst) == 1)
        //        {
        //          /* we either subtract or add the difference between
        //             time zone offsets, depending on which way the user got it
        //             wrong. The diff is typically one hour, or 3600 seconds,
        //             and should fit in a 16-bit int, even though offset
        //             is a long to accomodate 12 hours. */
        //          int diff = (int) (tz->__tzrule[0].offset
        //                            - tz->__tzrule[1].offset);
        //          if (!isdst)
        //            diff = -diff;
        //          tim_p->tm_sec += diff;
        //          validate_structure (tim_p);
        //          tim += diff;  /* we also need to correct our current time calculation */
        //        }
        //    }
        //}
 //   }

  //screw this also 
  /* add appropriate offset to put time in gmt format */
  //if (isdst == 1)
  //  tim += (time_t) tz->__tzrule[1].offset;
  //else /* otherwise assume std time */
  //  tim += (time_t) tz->__tzrule[0].offset;

  //and screw this too
  /* reset isdst flag to what we have calculated */
  tim_p->tm_isdst = isdst;

  return tim;
}

// vim: ts=2 sw=2 et
