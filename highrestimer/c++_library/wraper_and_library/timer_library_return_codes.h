/*
The Timer Library Constants
Copyright (C) 2005 Madhur Kumar Tanwani

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

//error and success codes
#define TIMER_LIB_OK 0
#define TIMER_LIB_FAILED -1
#define TIMER_LIB_ERROR_INCORRECT_TIMER 1
#define TIMER_LIB_ERROR_BUFFER_TOO_SMALL 2
#define TIMER_LIB_GETTIMEOFDAY_RETURNED_ERROR 3
#define TIMER_LIB_CLOCK_GETTTIME_REALTIME_RETURNED_ERROR 4
#define TIMER_LIB_CLOCK_GETTTIME_HIGHRES_RETURNED_ERROR 5
#define TIMER_LIB_ERROR_NO_TIMER_COULD_BE_RESOLVED 6
#define TIMER_LIB_ERROR_UNSUPPORTED_HIGHRES_TIMESTAMP_REQUEST 7
