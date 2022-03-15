/*
 * syslog.h
 *
 * Public declarations for a rudimentary log file management API.
 *
 *
 * $Id$
 *
 * Written by Keith Marshall <keith@users.osdn.me>
 * Copyright (C) 2020, MinGW.org Project
 *
 *
 * This is free software.  Permission is granted to copy, modify and
 * redistribute this software, under the provisions of the GNU General
 * Public License, Version 3, (or, at your option, any later version),
 * as published by the Free Software Foundation; see the file COPYING
 * for licensing details.
 *
 * Note, in particular, that this software is provided "as is", in the
 * hope that it may prove useful, but WITHOUT WARRANTY OF ANY KIND; not
 * even an implied WARRANTY OF MERCHANTABILITY, nor of FITNESS FOR ANY
 * PARTICULAR PURPOSE.  Under no circumstances will the author, or the
 * MinGW Project, accept liability for any damages, however caused,
 * arising from the use of this software.
 *
 */
#ifndef _SYSLOG_H
#pragma GCC system_header
#define _SYSLOG_H  1

/* All MinGW applications are required to include this.
 */
#include <_mingw.h>
/* Message severity ratings, in decreasing order of priority, conforming
 * generally to POSIX.1-1990 (SUSv1) specifications.
 */
#define LOG_EMERG        7
#define LOG_ALERT        6
#define LOG_CRIT         5
#define LOG_ERR          4
#define LOG_WARNING      3
#define LOG_NOTICE       2
#define LOG_INFO         1
#define LOG_DEBUG        0

/* POSIX.1-1990 facility identification codes; these are currently unused,
 * in this MinGW implementation, with all messages considered to originate
 * from the default LOG_USER facility.
 */
#define LOG_USER        __syslog_facility( 0)   /* default */
#define LOG_SYSLOG      __syslog_facility( 1)   /* currently unused */
#define LOG_NEWS        __syslog_facility( 2)   /* currently unused */
#define LOG_MAIL        __syslog_facility( 3)   /* currently unused */
#define LOG_LPR         __syslog_facility( 4)   /* currently unused */
#define LOG_LOCAL0      __syslog_facility( 5)   /* currently unused */
#define LOG_LOCAL1      __syslog_facility( 6)   /* currently unused */
#define LOG_LOCAL2      __syslog_facility( 7)   /* currently unused */
#define LOG_LOCAL3      __syslog_facility( 8)   /* currently unused */
#define LOG_LOCAL4      __syslog_facility( 9)   /* currently unused */
#define LOG_LOCAL5      __syslog_facility(10)   /* currently unused */
#define LOG_LOCAL6      __syslog_facility(11)   /* currently unused */
#define LOG_LOCAL7      __syslog_facility(12)   /* currently unused */
#define LOG_KERN        __syslog_facility(13)   /* currently unused */
#define LOG_FTP         __syslog_facility(14)   /* currently unused */
#define LOG_DAEMON      __syslog_facility(15)   /* currently unused */
#define LOG_CRON        __syslog_facility(16)   /* currently unused */
#define LOG_AUTHPRIV    __syslog_facility(17)   /* currently unused */
#define LOG_AUTH        __syslog_facility(18)   /* currently unused */
#define LOG_UUCP        __syslog_facility(19)   /* currently unused */

/* POSIX.1 requires the facility identifier to be passed to syslog(),
 * in logical OR combination with message priority.  Since there are
 * eight contiguously numbered priorities defined, three binary bits
 * are needed to represent them; the following macros make provision
 * for combining any one of the defined facility codes, with any one
 * valid priority, reserving the four low order bits to accommodate
 * the priority.
 */
#define __syslog_facility(id)    ((id) << __syslog_facility_offset)
#define __syslog_facility_offset    4

/* Options which may be set, by calling openlog().  Note that this is not
 * required; if omitted, the log stream will be opened as if LOG_ODELAY is
 * set, and a default file name will be derived from the application name.
 */
#define LOG_CONS        (1 << 0)        /* use stderr as fallback log     */
#define LOG_NDELAY      (1 << 1)        /* open log immediately           */
#define LOG_NOWAIT      (1 << 2)        /* unimplemented -- has no effect */
#define LOG_ODELAY      (1 << 3)        /* default -- open log on demand  */
#define LOG_PERROR      (1 << 4)        /* unimplemented -- has no effect */
#define LOG_PID         (1 << 5)        /* unimplemented -- has no effect */

/* The following LOG_MASK() macro should be used to specify the argument
 * which is to be passed to the setlogmask() function; multiple priority
 * masks may be specified, by logically ORing the effects of invocation
 * of LOG_MASK() for each priority which is to be masked.
 */
#define LOG_MASK( priority )  __logmask( priority )

/* The following implementation-private macros provide the mapping from
 * valid message priority values to their corresponding mask flags.
 */
#define __logmask_bits(id)    (__logmask_enable_bit | (1 << (id)))
#define __logmask_valid(id)   ((LOG_EMERG >= (unsigned int)(id)) ? (id) : 0)
#define __logmask_enable(id)  (__logmask_valid(id) ? __logmask_enable_bit : 0)
#define __logmask(id)         (__logmask_valid(id) ? __logmask_bits(id) : 0)

/* Since an argument of zero, when passed to setlogmask(), operates with
 * a read-only effect, the following pair of macros reserve the use of an
 * additional bit, to support resetting of any active mask bit.
 */
#define __logmask_valid_bits  (__logmask_enable_bit - 1)
#define __logmask_enable_bit  ((1 << (LOG_EMERG + 1)))

/* All of the syslog API functions are implemented with C linkage.
 */
#ifdef __cplusplus
extern "C" {
#endif

/* The syslog() function directs messages to the message logging facility,
 * after implicitly opening the message stream, if necessary.
 */
void syslog (int, const char *, ...);

/* The following pair of functions support explicit opening and closing
 * of the message logging stream.  Their use is optional, since syslog()
 * will implicitly open the stream with default options; however, if any
 * options other than the defaults are desired, they may be established
 * by calling openlog(), before calling syslog() for the first time, or
 * by calling closelog() and then openlog(), to change optios for any
 * subsequent use of syslog(), after defaults have been established.
 */
void closelog (void);
void openlog (const char *, int, int);

/* The setlogmask() function specifies a set of message priorities
 * which are to be excluded from the message stream, as generated by
 * subsequent calls to syslog(); the argument should be specified as
 * the logical OR of LOG_MASK() macro invocations, for each priority
 * which is to be masked out of the stream.
 *
 * Note that calling setlogmask(0) will return the current set of
 * active masks, but will NOT change it; to add further mask bits to
 * the active set, OR the return value of setlogmask(0) with the set
 * of LOG_MASK() values to be added; conversely, to clear any subset
 * of the active set, XOR the return value from setlogmask(0) with
 * the ORed set of LOG_MASK() values for the masks to be cleared.
 */
int setlogmask (int);

#ifdef __cplusplus
}
#endif

#endif  /* !_SYSLOG_H: $RCSfile$: end of file */
