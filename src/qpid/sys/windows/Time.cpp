/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

#include "qpid/sys/Time.h"
<<<<<<< HEAD
=======
#include <cmath>
>>>>>>> 3bbfc42... Imported Upstream version 0.32
#include <ostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread_time.hpp>
#include <windows.h>
<<<<<<< HEAD
=======
#include <time.h>
>>>>>>> 3bbfc42... Imported Upstream version 0.32

using namespace boost::posix_time;

namespace {

// High-res timing support. This will display times since program start,
// more or less. Keep track of the start value and the conversion factor to
// seconds.
bool timeInitialized = false;
<<<<<<< HEAD
LARGE_INTEGER start;
double freq = 1.0;
=======
LARGE_INTEGER start_hpc;
double hpc_freq = 1.0;

double start_time;

/// Static constant to remove time skew between FILETIME and POSIX
/// time.  POSIX and Win32 use different epochs (Jan. 1, 1970 v.s.
/// Jan. 1, 1601).  The following constant defines the difference
/// in 100ns ticks.
const DWORDLONG FILETIME_to_timval_skew = 0x19db1ded53e8000;
>>>>>>> 3bbfc42... Imported Upstream version 0.32

}

namespace qpid {
namespace sys {

AbsTime::AbsTime(const AbsTime& t, const Duration& d) {
    if (d == Duration::max()) {
        timepoint = ptime(max_date_time);
    }
    else {
        time_duration td = microseconds(d.nanosecs / 1000);
        timepoint = t.timepoint + td;
    }
}

AbsTime AbsTime::FarFuture() {
    AbsTime ff;
    ptime maxd(max_date_time);
    ff.timepoint = maxd;
    return ff;
}

<<<<<<< HEAD
AbsTime AbsTime::Epoch() {
=======
AbsTime AbsTime::Zero() {
    AbsTime time_epoch;
    time_epoch.timepoint = boost::posix_time::from_time_t(0);
    return time_epoch;
}

AbsTime AbsTime::epoch() {
>>>>>>> 3bbfc42... Imported Upstream version 0.32
    AbsTime time_epoch;
    time_epoch.timepoint = boost::posix_time::from_time_t(0);
    return time_epoch;
}

AbsTime AbsTime::now() {
    AbsTime time_now;
    time_now.timepoint = boost::get_system_time();
    return time_now;
}

<<<<<<< HEAD
=======
Duration Duration::FromEpoch() {
    time_duration d = boost::get_system_time() - boost::posix_time::from_time_t(0);
    return d.total_nanoseconds();
}

>>>>>>> 3bbfc42... Imported Upstream version 0.32
Duration::Duration(const AbsTime& start, const AbsTime& finish) {
    time_duration d = finish.timepoint - start.timepoint;
    nanosecs = d.total_nanoseconds();
}

std::ostream& operator<<(std::ostream& o, const Duration& d) {
<<<<<<< HEAD
    return o << int64_t(d) << "ns";   
=======
    if (d >= TIME_SEC) return o << (double(d)/TIME_SEC) << "s";
    if (d >= TIME_MSEC) return o << (double(d)/TIME_MSEC) << "ms";
    if (d >= TIME_USEC) return o << (double(d)/TIME_USEC) << "us";
    return o << int64_t(d) << "ns";
}

std::istream& operator>>(std::istream& i, Duration& d) {
    // Don't throw, let the istream throw if it's configured to do so.
    double number;
    i >> number;
    if (i.fail()) return i;

    if (i.eof() || std::isspace(i.peek())) // No suffix
        d = number*TIME_SEC;
    else {
        std::string suffix;
        i >> suffix;
        if (i.fail()) return i;
        if (suffix.compare("s") == 0) d = number*TIME_SEC;
        else if (suffix.compare("ms") == 0) d = number*TIME_MSEC;
        else if (suffix.compare("us") == 0) d = number*TIME_USEC;
        else if (suffix.compare("ns") == 0) d = number*TIME_NSEC;
        else i.setstate(std::ios::failbit);
    }
    return i;
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}

std::ostream& operator<<(std::ostream& o, const AbsTime& t) {
    std::string time_string = to_simple_string(t.timepoint);
    return o << time_string;
}


void sleep(int secs) {
    ::Sleep(secs * 1000);
}

void usleep(uint64_t usecs) {
    DWORD msecs = usecs / 1000;
    if (msecs == 0)
        msecs = 1;
    ::Sleep(msecs);
}

void outputFormattedNow(std::ostream& o) {
    ::time_t rawtime;
    ::tm timeinfo;
    char time_string[100];

    ::time( &rawtime );
#ifdef _MSC_VER
    ::localtime_s(&timeinfo, &rawtime);
#else
    timeinfo = *(::localtime(&rawtime));
#endif
    ::strftime(time_string, 100,
               "%Y-%m-%d %H:%M:%S",
               &timeinfo);
    o << time_string << " ";
}

void outputHiresNow(std::ostream& o) {
<<<<<<< HEAD
    if (!timeInitialized) {
        start.QuadPart = 0;
        LARGE_INTEGER iFreq;
        iFreq.QuadPart = 1;
        QueryPerformanceCounter(&start);
        QueryPerformanceFrequency(&iFreq);
        freq = static_cast<double>(iFreq.QuadPart);
        timeInitialized = true;
    }
    LARGE_INTEGER iNow;
    iNow.QuadPart = 0;
    QueryPerformanceCounter(&iNow);
    iNow.QuadPart -= start.QuadPart;
    if (iNow.QuadPart < 0)
        iNow.QuadPart = 0;
    double now = static_cast<double>(iNow.QuadPart);
    now /= freq;                 // now is seconds after this
    o << std::fixed << std::setprecision(8) << std::setw(16) << std::setfill('0') << now << "s ";
=======
    ::time_t tv_sec;
    ::tm timeinfo;
    char time_string[100];

    if (!timeInitialized) {
        // To start, get the current time from FILETIME which includes
        // sub-second resolution. However, since FILETIME is updated a bit
        // "bumpy" every 15 msec or so, future time displays will be the
        // starting FILETIME plus a delta based on the high-resolution
        // performance counter.
        FILETIME file_time;
        ULARGE_INTEGER start_usec;
        ::GetSystemTimeAsFileTime(&file_time);   // This is in 100ns units
        start_usec.LowPart = file_time.dwLowDateTime;
        start_usec.HighPart = file_time.dwHighDateTime;
        start_usec.QuadPart -= FILETIME_to_timval_skew;
        start_usec.QuadPart /= 10;   // Convert 100ns to usec
        tv_sec = (time_t)(start_usec.QuadPart / (1000 * 1000));
        long tv_usec = (long)(start_usec.QuadPart % (1000 * 1000));
        start_time = static_cast<double>(tv_sec);
        start_time += tv_usec / 1000000.0;

        start_hpc.QuadPart = 0;
        LARGE_INTEGER iFreq;
        iFreq.QuadPart = 1;
        QueryPerformanceCounter(&start_hpc);
        QueryPerformanceFrequency(&iFreq);
        hpc_freq = static_cast<double>(iFreq.QuadPart);
        timeInitialized = true;
    }
    LARGE_INTEGER hpc_now;
    hpc_now.QuadPart = 0;
    QueryPerformanceCounter(&hpc_now);
    hpc_now.QuadPart -= start_hpc.QuadPart;
    if (hpc_now.QuadPart < 0)
        hpc_now.QuadPart = 0;
    double now = static_cast<double>(hpc_now.QuadPart);
    now /= hpc_freq;                 // now is seconds after this
    double fnow = start_time + now;
    double usec, sec;
    usec = modf(fnow, &sec);
    tv_sec = static_cast<time_t>(sec);
#ifdef _MSC_VER
    ::localtime_s(&timeinfo, &tv_sec);
#else
    timeinfo = *(::localtime(&tv_sec));
#endif
    ::strftime(time_string, 100,
               "%Y-%m-%d %H:%M:%S",
               &timeinfo);
    // No way to set "max field width" to cleanly output the double usec so
    // convert it back to integral number of usecs and print that.
    unsigned long i_usec = usec * 1000 * 1000;
    o << time_string << "." << std::setw(6) << std::setfill('0') << i_usec << " ";
>>>>>>> 3bbfc42... Imported Upstream version 0.32
}
}}
