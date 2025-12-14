//////////////////////////////////////////////////////////////////////////////
//
// CmTypes.h - Declaration of basic type and diagnostics classes
//
//////////////////////////////////////////////////////////////////////////////
//
// author: 	   Eckhard Kantz
// website:    http://wegalink.eu
//
//////////////////////////////////////////////////////////////////////////////
/* 
This is FREE software  

Permission is hereby granted, free of charge,  to any person obtaining  a copy 
of this software and associated documentation files (the "Software"),  to deal 
in the Software without restriction, including without limitation  the  rights 
to use,  copy,  modify,  merge,  publish,  distribute, sublicense, and/or sell 
copies  of  the  Software,   and  to  permit  persons  to  whom  the  Software 
is furnished to do so, subject to the following conditions: 

There are no conditions imposed on the use of this software. 

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT  WARRANTY  OF ANY KIND,  EXPRESS OR 
IMPLIED,  INCLUDING  BUT  NOT  LIMITED  TO  THE  WARRANTIES OF MERCHANTABILITY, 
FITNESS  FOR  A  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS  OR  COPYRIGHT  HOLDERS  BE  LIABLE  FOR  ANY CLAIM,  DAMAGES OR OTHER 
LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,  ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN  THE 
SOFTWARE. 
*/

// NOTE: - macro 'MSVS' has to be defined in MS VisualStudio settings 
//       - shlwapi.lib has to be added to project settings


#ifndef CmTypesH
#define CmTypesH

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <sys/timeb.h>
#include <math.h>
#include <share.h>

#ifdef UNIX
//------------Unix------------
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/param.h>
#define MKDIR(path,mask) mkdir(path,mask)
#define GETCWD(buf,len)  getcwd(buf,len)
#define OPEN(buf,mode,access) open(buf,mode,access)
//------------Unix: 32-bit/64-bit compatibility------------
typedef __int64_t     int64;
typedef __uint64_t    uint64;
typedef __int32_t     int32;
typedef __uint32_t    uint32;
typedef __int16_t     int16;
typedef __uint16_t    uint16;
typedef char          int8;
typedef unsigned char uint8;
#define abs64	llabs
#else

//============================ Windows =========================================
//#include <windows.h>
#include <io.h>
#include <direct.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

//---Common MVC and BVC
#define MAXPATHLEN 1024
#define GETCWD(buf,len) GetCurrentDirectoryA(len,buf)

//--------- Microsoft Visual C++ -------------
#define open  _open
#define read	_read
#define close _close
#define fstat _fstat
#define stat  _stat
#define ftime _ftime
#define abs64	_abs64
#define chdir _chdir

#ifdef O_RDONLY
  #undef O_RDONLY
#endif
#define O_RDONLY   _O_RDONLY
#define timeb	   _timeb
#define MKDIR(path,mask) _mkdir(path)
#define OPEN(buf,mode,access) _open(buf,mode)
#define WRITE(fp,data,length) _write(fp,data,length)

// Define all unix access masks
#define __S_IREAD       0400    /* Read by owner.  */
#define __S_IWRITE      0200    /* Write by owner.  */
#define __S_IEXEC       0100    /* Execute by owner.  */
#if defined __USE_MISC && defined __USE_BSD
# define S_IREAD        S_IRUSR
# define S_IWRITE       S_IWUSR
# define S_IEXEC        S_IXUSR
#endif
#define S_IRGRP (S_IRUSR >> 3)  /* Read by group.  */
#define S_IWGRP (S_IWUSR >> 3)  /* Write by group.  */
#define S_IXGRP (S_IXUSR >> 3)  /* Execute by group.  */
/* Read, write, and execute by group.  */
#define S_IRWXG (S_IRWXU >> 3)
#define S_IROTH (S_IRGRP >> 3)  /* Read by others.  */
#define S_IWOTH (S_IWGRP >> 3)  /* Write by others.  */
#define S_IXOTH (S_IXGRP >> 3)  /* Execute by others.  */
/* Read, write, and execute by others.  */
#define S_IRWXO (S_IRWXG >> 3)
/* file mask */
#define S_MASK  S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH


//------------Win: 32-bit/64-bit compatibility------------
typedef __int64            int64;
typedef unsigned __int64   uint64;
typedef __int32            int32;
typedef unsigned int       uint32;
typedef __int16            int16;
typedef unsigned __int16   uint16;
typedef char               int8;
typedef unsigned char      uint8;

#endif // #ifndef WIN32


//----------MSVS: Avoid safe functions warnings------------
#ifdef MSVS
  #define MEMCPY(a,b,c,d) memcpy_s(a,b,c,d)
  #define STRCPY(a,b,c) strcpy_s(a,b,c)
  #define STRNCPY(a,b,c,d) strncpy_s(a,b,c,d)
  #define SPRINTF3(a,b,c) sprintf_s(a,b,c)
  #define SPRINTF4(a,b,c,d) sprintf_s(a,b,c,d)
  #define SPRINTF5(a,b,c,d,e) sprintf_s(a,b,c,d,e)
  #define SPRINTF6(a,b,c,d,e,f) sprintf_s(a,b,c,d,e,f)
  #define SPRINTF7(a,b,c,d,e,f,g) sprintf_s(a,b,c,d,e,f,g)
  #define SPRINTF8(a,b,c,d,e,f,g,h) sprintf_s(a,b,c,d,e,f,g,h)
  #define SPRINTF9(a,b,c,d,e,f,g,h,i) sprintf_s(a,b,c,d,e,f,g,h,i)
#else
  #define STRCPY(a,b,c)  strcpy(a,c)
  #define STRNCPY(a,b,c,d)  strncpy(a,c,d)
  #define SPRINTF3(a,b,c) sprintf(a,c)
  #define SPRINTF4(a,b,c,d) sprintf(a,c,d)
  #define SPRINTF5(a,b,c,d,e) sprintf(a,c,d,e)
  #define SPRINTF6(a,b,c,d,e,f) sprintf(a,c,d,e,f)
  #define SPRINTF7(a,b,c,d,e,f,g) sprintf(a,c,d,e,f,g)
  #define SPRINTF8(a,b,c,d,e,f,g,h) sprintf(a,c,d,e,f,g,h)
  #define SPRINTF9(a,b,c,d,e,f,g,h,i) sprintf(a,c,d,e,f,g,h,i)
#endif  // #ifdef MSVS


//--------------------DEV C++ compatibility----------------
#ifdef gcc
  #define MIN_INT64         0x8000000000000000ull
  #define MAX_INT64         0x7FFFFFFFFFFFFFFFull
  #define MAX_UINT64        0xFFFFFFFFFFFFFFFFull
#else
  #define MIN_INT64         0x8000000000000000
  #define MAX_INT64         0x7FFFFFFFFFFFFFFF
  #define MAX_UINT64        0xFFFFFFFFFFFFFFFF
#endif // #ifdef gcc


#define MIN_INT16           0x8000
#define MAX_INT16           0x7FFF
#define MAX_UINT16	        0xFFFF

#define MIN_INT32           0x80000000
#define MAX_INT32           0x7FFFFFFF
#define MAX_UINT32	        0xFFFFFFFF

#define LO64(x)		        (uint32)(x&0xFFFFFFFF)
#define HI64(x)		        (uint32)(x>>32)

#define MIN_DOUBLE          1.7E-308
#define MAX_DOUBLE          1.7E+308

#define PI						3.141592653589793238462643383279502884197169399375105820974944592

// Date and time values.
#define DATETIME_FEMTOSECONDS	uint64(1000*1000*1000*1000*1000)
#define DATETIME_PICOSECONDS	uint64(1000*1000*1000*1000)
#define DATETIME_NANOSECONDS	uint64(1000*1000*1000)
#define DATETIME_MICROSECONDS	uint64(1000*1000)
#define DATETIME_MILLISECONDS	uint64(1000)
// Timebase based on NANOSECONDS
#define TIMEBASE_SECONDS		(DATETIME_NANOSECONDS)
#define TIMEBASE_MILLISECONDS	(DATETIME_NANOSECONDS/1000)
#define TIMEBASE_MICROSECONDS	(DATETIME_NANOSECONDS/(1000*1000))
#define TIMEBASE_NANOSECONDS	1
#define TIMEBASE_PICOSECONDS	0.001
#define TIMEBASE_FEMTOSECONDS	0.000001
#define TIMEBASE_DAILY			(86400*DATETIME_NANOSECONDS)
// Seconds
#define SECONDS_PER_MINUTE		60
#define SECONDS_PER_HOUR		3600
#define SECONDS_PER_DAY			86400

#ifdef CMNOLOG
#define LOG14(a,b,c,d,e,f,g,h,i,j,k,l,m,n,msg,LogLevel)
#define LOG13(a,b,c,d,e,f,g,h,i,j,k,l,m,msg,LogLevel)
#define LOG12(a,b,c,d,e,f,g,h,i,j,k,l,msg,LogLevel)
#define LOG11(a,b,c,d,e,f,g,h,i,j,k,msg,LogLevel)
#define LOG10(a,b,c,d,e,f,g,h,i,j,msg,LogLevel)
#define LOG9(a,b,c,d,e,f,g,h,i,msg,LogLevel)
#define LOG8(a,b,c,d,e,f,g,h,msg,LogLevel)
#define LOG7(a,b,c,d,e,f,g,msg,LogLevel)
#define LOG6(a,b,c,d,e,f,msg,LogLevel)
#define LOG5(a,b,c,d,e,msg,LogLevel)
#define LOG4(a,b,c,d,msg,LogLevel)
#define LOG3(a,b,c,msg,LogLevel)
#define LOG2(a,b,msg,LogLevel)
#define LOG1(a,msg,LogLevel)
#else
#define LOG14(a,b,c,d,e,f,g,h,i,j,k,l,m,n,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; msg+=g; msg+=h; msg+=i; msg+=j; msg+=k; msg+=l; msg+=m; msg+=n; log(msg, LogLevel);
#define LOG13(a,b,c,d,e,f,g,h,i,j,k,l,m,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; msg+=g; msg+=h; msg+=i; msg+=j; msg+=k; msg+=l; msg+=m; log(msg, LogLevel);
#define LOG12(a,b,c,d,e,f,g,h,i,j,k,l,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; msg+=g; msg+=h; msg+=i; msg+=j; msg+=k; msg+=l; log(msg, LogLevel);
#define LOG11(a,b,c,d,e,f,g,h,i,j,k,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; msg+=g; msg+=h; msg+=i; msg+=j; msg+=k; log(msg, LogLevel);
#define LOG10(a,b,c,d,e,f,g,h,i,j,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; msg+=g; msg+=h; msg+=i; msg+=j; log(msg, LogLevel);
#define LOG9(a,b,c,d,e,f,g,h,i,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; msg+=g; msg+=h; msg+=i; log(msg, LogLevel);
#define LOG8(a,b,c,d,e,f,g,h,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; msg+=g; msg+=h; log(msg, LogLevel);
#define LOG7(a,b,c,d,e,f,g,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; msg+=g; log(msg, LogLevel);
#define LOG6(a,b,c,d,e,f,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; log(msg, LogLevel);
#define LOG5(a,b,c,d,e,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; log(msg, LogLevel);
#define LOG4(a,b,c,d,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; log(msg, LogLevel);
#define LOG3(a,b,c,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; log(msg, LogLevel);
#define LOG2(a,b,msg,LogLevel) 	CmString msg=a; msg+=b; log(msg, LogLevel);
#define LOG1(a,msg,LogLevel) 	CmString msg=a; log(msg, LogLevel);
#define LOG_6(a,b,c,d,e,f,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; msg+=f; CmStream::log(msg, LogLevel);
#define LOG_5(a,b,c,d,e,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; msg+=e; CmStream::log(msg, LogLevel);
#define LOG_4(a,b,c,d,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; msg+=d; CmStream::log(msg, LogLevel);
#define LOG_3(a,b,c,msg,LogLevel) 	CmString msg=a; msg+=b; msg+=c; CmStream::log(msg, LogLevel);
#define LOG_2(a,b,msg,LogLevel) 	CmString msg=a; msg+=b; CmStream::log(msg, LogLevel);
#define LOG_1(a,msg,LogLevel) 	CmString msg=a; CmStream::log(msg, LogLevel);
#endif

#define CM_IFTL_ROOT_COSMOS     "EKD@JN58nc_Tuerkenfeld.Cosmos"
#define CM_IFTL_ROOT_ANONYMOUS  "anonymous@Earth.FTLI"
#define CM_IFTL_DEFAULT         "Default"

#define WEGALINK_URL            "wegalink.feste-ip.net:50843"

#define CM_NETWORK_IFTL_LAN_TCP		"EKD@JN58nc_Türkenfeld.LAN_TCP"
#define CM_NETWORK_IFTL_LAN_UDP		"EKD@JN58nc_Türkenfeld.LAN_UDP"
#define CM_NETWORK_IFTL_WEGALINK	"EKD@JN58nc_Türkenfeld.WegaLink"
#define CM_NETWORK_PORT_LAN_UDP		4225
#define CM_NETWORK_PORT_LAN_TCP		4226
#define CM_NETWORK_PORT_WEGALINK	CM_NETWORK_PORT_LAN_TCP

// IFTL extensions for a 'CmGatewayTCP/UDP' PROVIDER
#define IFTL_PROVIDER_CmGatewayTCP  "/TCP/built/2017-11-28"
#define IFTL_PROVIDER_CmGatewayUDP  "/UDP/built/2017-11-28"

#define CHECKSUM_DIGIT_1  ((int64)216)
#define CHECKSUM_DIGIT_2  ((int64)216*216)
#define CHECKSUM_DIGIT_3  ((int64)216*216*216)
#define CHECKSUM_DIGIT_4  ((int64)216*216*216*216)
#define CHECKSUM_DIGIT_5  ((int64)216*216*216*216*216)
#define CHECKSUM_DIGIT_6  ((int64)216*216*216*216*216*216)
#define CHECKSUM_DIGIT_7  ((int64)216*216*216*216*216*216*216)

#define FTLight_DTI_MAX	(uint32)0x81BF0FFF	// = 216^4-1
#define FTLight_DTI_MIN	(uint32)0x80000000	// = 2^31


namespace Cosmos
{
	/** CmFlex.
	*   A union that holds all data formats
	*/
	typedef union{
		void* pointerValue;
		uint64 uint64Value;
		int64 int64Value;
		double doubleValue;
		uint32 uint32Value;
		int32 int32Value;
		float floatValue;
		uint16 uint16Value;
		int16 int16Value;
		uint8 uint8Value;
		int8 int8Value;
		bool boolValue;
	} CmFlex;


	/** CmXY
	 *  This class represents a two dimensional size value consisting of a width and
	 *  a height value (or x and y).
	 */
	template <typename T>
	class CmXY
	{
		T _x;
		T _y;

	public:
		CmXY() :_x(0), _y(0){};
		CmXY(T const& x, T const& y) :_x(x), _y(y){}
		CmXY(double const& x, double const& y) :_x((T)x), _y((T)y){}
		CmXY(T const& t) :_x(t.x), _y(t.y){}
		~CmXY(){};

	public:
		// access functions
		T& x(){ return _x; }
		T& y(){ return _y; }
		T x()const{ return _x; }
		T y()const{ return _y; }
		T w()const{ return _x; }
		T h()const{ return _y; }
		T P1()const{ return _x; }
		T P2()const{ return _y; }
		T begin()const{ return _x; }
		T end()const{ return _y; }

		void operator=(CmXY const& t)
		{
			if (this != &t)
			{
				_x = t._x;
				_y = t._y;
			}
		}
		CmXY operator+(CmXY const& t)
		{
			CmXY sum;
			sum.x() = _x + t._x;
			sum.y() = _y + t._y;
			return sum;
		}
		void operator+=(CmXY const& t)
		{
			_x += t._x;
			_y += t._y;
		}
		void operator+=(float s)
		{
			_x += s;
			_y += s;
		}
	};

	typedef CmXY<int32> CmSize;
	typedef CmXY<int32> CmPoint;
	typedef CmXY<int32> CmRange;
	typedef CmXY<float> CmSize2D;
	typedef CmXY<float> CmPoint2D;
	typedef CmXY<float> CmRange2D;
	typedef CmXY<CmPoint2D> CmLine2D;
	typedef CmXY<CmPoint2D> CmRectangle2D;

	/** CmSmoothed.
	*  A series of values will be smoothed according to a smoothing factor.
	*  The values will be updated directly whenever a threshold is exceeded.
	*/
	class CmSmoothed
	{
#define CMSMOOTHED_THRESHOLD_DEFAULT		0.8
#define CMSMOOTHED_FACTOR_DEFAULT				1.0
	public:
		// constructor
		CmSmoothed(double _StartValue =0);
		// desctructor
		~CmSmoothed();
	
	public:
		/** smooth.
		*   A new value will be processed for smoothing
		*/
		double smooth(double _NewValue);
		double smooth(int32 NewValue);
	
	public:
		// control parameters
		double Threshold;
		double Factor;

	public:
		// state
		double Value;
	};

}  // namespace Cosmos

using namespace Cosmos;

#endif // ifndef CmTypesH
