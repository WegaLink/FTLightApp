//////////////////////////////////////////////////////////////////////////////
//
// CmString.ixx - Module for CmString classes
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

module;

#include "Services/Basic/Modules/CmTypes.h"

export module CmBasic:CmString;

export namespace Cosmos
{
	//Collection of characters that function as delimiters in path strings
#define CM_PATH_DELIMITERS             "\\/"

	// empty string
#define EMPTY_STRING					""

	class CmLString;
	class CmMString;
	template <typename T>
	class CmVector;

	// memory bookkeeping
	int64 ItemsAllocated = 0;
	int64 ItemsReleased = 0;
	int64 ItemsOccupied = 0;
	int64 MemoryAllocated = 0;
	int64 MemoryReleased = 0;
	int64 MemoryOccupied = 0;
	int64 ItemsOfTypeAllocated = 0;
	int64 ItemsOfTypeReleased = 0;
	CRITICAL_SECTION MemoryAccess;
	bool isInitMemory = false;
	bool isFocus = false;		// for analyzing a set of types
	bool isInt8 = false;		// use isInt8 and isUint8 together...
	bool isUint8 = false;		// ...to compensate for type exchange
	bool isInt16 = false;
	bool isUint16 = false;
	bool isDouble = false;
	bool isNaviFTL = false;
	bool isCmString = false;
	bool isCmLString = false;
	bool isCmStringFTL = false;
	bool isCmStringFTLChild = false;
	bool isCmServiceConnection = false;
	bool isCmException = false;
	bool isCmMatrixFTL = false;
	bool isCmMatrix = false;
	bool isCmVector = false;
	bool isCmParallel = false;


	//----------------------------------------------------------------------------
	// CmString: functions for string handling
	//----------------------------------------------------------------------------
	//
	class CmString
	{
	public:
		//Constructors and destructors
		CmString();
		CmString(const int8* _Text);
		CmString(const int8* _Text, uint32 _Length);
		CmString(uint32 _Length, uint8 _Init = 0);
		CmString(const CmString& _String);
		CmString(CmVector<uint8>& _VectorUInt8);
		virtual ~CmString();
		void initWorkspace();

	public:
		// unit test
		static bool testCmString();

	public:
		// set functions for basic types
		void setAt(const uint32 _Pos, int8 _Val8);
		void setAt(const uint32 _Pos, uint8 _Val8);
		void setAt(const uint32 _Pos, bool _ValBool);
		void setAt(const uint32 _Pos, int16 _Val16);
		void setAt(const uint32 _Pos, uint16 _Val16);
		void setAt(const uint32 _Pos, int32 _Val32);
		void setAt(const uint32 _Pos, uint32 _Val32);
		void setAt(const uint32 _Pos, float _ValFloat);
		void setAt(const uint32 _Pos, int64 _Val64);
		void setAt(const uint32 _Pos, uint64 _Val64);
		void setAt(const uint32 _Pos, double _ValDouble);
		void setAt(const uint32 _Pos, void* _ValDouble);
		// get functions for basic types
		void getAt(const uint32 _Pos, int8& _Val8);
		void getAt(const uint32 _Pos, uint8& _Val8);
		void getAt(const uint32 _Pos, bool& _ValBool);
		void getAt(const uint32 _Pos, int16& _Val16);
		void getAt(const uint32 _Pos, uint16& _Val16);
		void getAt(const uint32 _Pos, int32& _Val32);
		void getAt(const uint32 _Pos, uint32& _Val32);
		void getAt(const uint32 _Pos, float& _ValFloat);
		void getAt(const uint32 _Pos, int64& _Val64);
		void getAt(const uint32 _Pos, uint64& _Val64);
		void getAt(const uint32 _Pos, void*& _Val64);
		void getAt(const uint32 _Pos, double& _ValDouble);
		// further set functions
		void clear(); // clear text content
		void reset(); // reset position markers
		void fill(size_t _Length, uint8 _Digit);
		void setText(const CmString& _String);
		void setLength(size_t _Length);
		void adjustLength(size_t _Length);
		int8* setData(const uint8* _Data, size_t _Length);
		int8* setText(const int8* _First, const int8* _Last);
		int8* setText(const int8* _Text);

		// assignment functions resulting in a 64-bit binary value
		void setValue(int8 _Val8);
		void setValue(uint8 _Val8);
		void setValue(bool _ValBool);
		void setValue(int16 _Val16);
		void setValue(uint16 _Val16);
		void setValue(int32 _Val32);
		void setValue(uint32 _Val32);
		void setValue(float _ValFloat);
		void setValue(int64 _Val64);
		void setValue(uint64 _Val64);
		void setValue(double _ValDouble);
		void setValue(void* _ValPointer);

	public:
		// several access functions to the string and its length information
		bool isEmpty();
		uint32 getLength() const;
		int8* getBuffer() const;
		const int8* getText() const;
		CmString getTextDir() const;  // convert forbidden chars to _ (underscore)
		const uint8* getBinary() const;
		const int16* getUnicode();
		uint32 operator[](uint32 _uPos) const;

	public:
		// string assignment and processing operators
		void operator=(const CmString& _String);
		void operator=(const int8* _Text);
		void operator=(const int32 _Val32);
		void operator=(const uint32 _Val32);
		void operator=(const int64 _Val64);
		void operator=(const uint64 _Val64);
		void operator+(const CmString _String);
		void operator+=(const CmString& _String);
		void operator+=(const int8* _Text);
		void operator+=(int32 _Val32);
		void operator+=(uint32 _Val32);
		void operator+=(int64 _Val64);
		void operator+=(uint64 _Val64);
		// compare operators
		bool operator==(const CmString& _String) const;
		bool operator==(const int8* _Text) const;
		bool operator!=(const CmString& _String) const;
		bool operator!=(const int8* _Text) const;
		bool operator<(const CmString& _String) const;
		bool operator<(const int8* _Text) const;
		bool operator<=(const CmString& _String) const;
		bool operator<=(const int8* _Text) const;
		bool operator>(const CmString& _String) const;
		bool operator>(const int8* _Text) const;
		bool operator>=(const CmString& _String) const;
		bool operator>=(const int8* _Text) const;
		// prefix
		bool isPrefix(const CmString& _String) const;
		bool isPrefix(const int8* _Text) const;

	public:
		/** float/double2String.
		*   Conversion of floating point numbers to a string representation
		*/
		bool float2String(float _Value, int32 _Precision = -1, int32 _Digits = -1);
		bool double2String(double _Value, int32 _Precision = -1, int32 _Digits = -1);
		static CmString Float2String(double _Value, int32 _Precision = -1, int32 _Digits = -1);
		static CmString Double2String(double _Value, int32 _Precision = -1, int32 _Digits = -1);

		/** uint2String.
		*   Conversion of unsigned integer numbers to a string representation
		*/
		bool uint2String(uint64 _Value);

		/** int2hex.
		*   Conversion of integers to a hexadecimal string representation
		*/
		static CmString UInt2Hex(uint64 _Value);
		bool uint2hex(uint64 _Value);
		uint64 hex2uint64();
		uint32 hex2uint32();
		uint16 hex2uint16();
		uint8 hex2uint8();

	public:
		/** IP2String.
		*  A network IP address along with a port number will be converted to a string.
		*/
		static CmString IP2String(uint32 _IP, uint16 _Port);

	public:
		// Search functions
		int32 findPosition(char _ch, int32 _StartPos = 0);
		int32 findBackwards(char _ch, int32 _StartPos = 0);
		int32 searchPattern(const char* _Pattern, int32 _StartPos = 0);

	public:
		// allItems. Items between delimiters will be returned
		bool allItems(CmString& _Item, const char* _Delimiters);
		bool allItems(CmString& _Item, uint8 _Delimiter);

	public:
		/** assignSubString.
		*   Copy a substring from a source to this string.
		*/
		int32 assignSubString(int32 _Last, CmString* _Src = (CmString*)0, int32 _First = 0);

	public:
		/** purgeTrailingSpace.
		*  The string will be shortened until trailing spaces disappered.
		*/
		CmString& purgeTrailingSpace();

	public:
		/** set/get/showActive/Highlight
		*   A part or all content of the full string will be marked as 'active/highlight'.
		*   A line number can be specified if LineNum is > 0.
		*/
		bool setActive(uint32 _ActiveStart, uint32 _ActiveLength, int32 _LineNumber = -1);
		bool setHighlight(uint32 _HighlightStart, uint32 _HighlightLength);
		bool getActive(uint32& _ActiveStart, uint32& _ActiveLength, int32& _LineNumber);
		bool getHighlight(uint32& _HighlightStart, uint32& _HighlightLength);
		bool getLineHighlight(int32& _LineNumber, uint32& _LineStart, uint32& _HighlightStart, uint32& _HighlightLength);
		bool showActiveHighlight();

	public:
		/** getNumAsDouble/Uint64/Int32.
		*		A string will be evaluated to a double or uint64 value respectively.
		*   'double' accepts: 123, -123, 12.3, 1.2e3, 1.2E-3
		*   'uint64' accepts: 123, 0x123, 0XaBC
		*   'int32' accepts: 123, -123, 0x123, 0XaBC
		*/
		bool isConversionError;
		double getNumAsDouble();
		double getNumAsDouble(int32 _PosStart, int32 _PosEnd);
		uint64 getNumAsUint64();
		uint64 getNumAsUint64(int32 _PosStart, int32 _PosEnd);
		int32 getNumAsInt32();
		int32 getNumAsInt32(int32 _PosStart, int32 _PosEnd);

		/** getMonth.
		*	The month will be determined from abbreviated names
		*	Jan=1, Feb=2, ... , Dec=12 or 0 will be returned
		*/
		int32 getMonthAsInt32();
		int32 getMonthAsInt32(int32 _PosStart, int32 _PosEnd);

	public:
		//Parse current string and return a list of substrings accordingly to patterns
		//defined by a regular expression. The following expressions are recognized so far:
		// characters:
		//     . - any character
		//    \s - space character (\t,\r,\n)
		//    \d - digit character 0..9
		//    \f - digit character 0..9 and '.' (float numbers)
		//    \w - word character _a..zA..Z
		//    \a - alphanumeric character _a..zA..Z0..9
		//    others - will be treated as the particular character, e.g. a is a and ! is !
		// modifiers:
		//	   * - any number of occurrencies from 0..MAX_UNSIGNED_INT
		//     + - at least one up to any number 1..MAX_UNSIGNED_INT
		//     ? - non-mandatory occurrence (zero or one times)
		//    () - enclose components to be extracted
		// If source characters get exhausted then the match will fail. Otherwise,
		// if the match is complete before all source characters are used then the
		// match succeeds and found substrings will be returned.
		// 
		// examples: 
		//   PROVIDER_QSim::importUVVisData()
		//      ".*UVVIS (P\\d+).+csv"
		//   CmServiceConnection::ip2host()
		//		  "(\\d+).(\\d+).(\\d+).(\\d+)" 
		//   CmRadastro(2001)
		//      "(\\w+ \\w+ \\d+) (\\d+:\\d+:\\d+) (\\d+) +(\\d+:\\d+:\\d+).+Power: +(\\d+\\.\\d+)"
		//
		bool matchPattern(CmString& _Match, int8 const* _Pattern);
		int32 matchPattern(CmLString** _MatchList, int8 const* _Pattern);
		int32 matchPattern(CmLString& _MatchList, int8 const* _Pattern);

	public:
		/** read/write
		*   The binary content of a CmString will be restored from file respectively
		*   it will be written to file.
		*/
		const int8* readBinary(const int8* _Filename);
		bool readBinary(CmString& _Filename);
		bool writeBinary(const int8* _Filename);
		bool appendBinary(const int8* _Filename);
		bool isFileExists(const int8* _Filename = (const int8*)0);
		bool addFilesOnPath(CmLString& _Files, const int8* _Path = (const int8*)0);
		bool allFiles(CmString& _File, const CmString& _Path, const int8* _Pattern = ".*", bool _isLoadContent = false);
		bool allLines(CmString& _Line, uint32 _Offset = 0);

	public:
		/** path & folder operations */
		bool isFolderExists(const int8* _FolderPath = (const int8*)0);
		bool getFoldersOnPath(CmLString& _Folders, const int8* _Path = (const int8*)0);
		bool allFolders(CmString& _Folder, const CmString& _Path);
		bool getParentFolder(CmString& _ParentFolder, const CmString* _Path = (const CmString*)0);
		bool getFolder(CmString& _Folder, const CmString* _Path = (const CmString*)0);

		/** allocate/releaseMemory.
		*  A bookkeeping is done whenever memory is allocated or released.
		*/
		template <typename T>
		static T* allocateMemory(int32 _ItemCount = 1, bool _isType = false)
		{
			// check for zero items
			if (0 == _ItemCount) return NULL;
			// init critical section
			if (false == isInitMemory) {
				InitializeCriticalSection(&MemoryAccess);
				isInitMemory = true;
			}
			// bookkeeping
			EnterCriticalSection(&MemoryAccess);
			ItemsAllocated++;
			MemoryAllocated += sizeof(T) * (_ItemCount > 1 ? _ItemCount : 1);
			_isType ? ItemsOfTypeAllocated += (_ItemCount > 1 ? _ItemCount : 1) : 0;
			// memory allocation
			T* Item = _ItemCount > 1 ? new T[_ItemCount] : new T;
			LeaveCriticalSection(&MemoryAccess);

			return Item;
		}
		template <typename T>
		static void releaseMemory(T*& _Item, int32 _ItemCount = 1, bool _isType = false)
		{
			// check for NULL pointer
			if (NULL == _Item) return;
			// init critical section
			if (false == isInitMemory) {
				InitializeCriticalSection(&MemoryAccess);
				isInitMemory = true;
			}
			// bookkeeping
			EnterCriticalSection(&MemoryAccess);
			ItemsReleased++;
			MemoryReleased += sizeof(T) * (_ItemCount > 1 ? _ItemCount : 1);
			_isType ? ItemsOfTypeReleased += (_ItemCount > 1 ? _ItemCount : 1) : 0;
			// memory deallocation
			// ToDo: fix memory deallocation problem
			// NOTE: This will fail if thread context has changed!
			_ItemCount > 1 ? delete[] _Item : delete _Item;
			_Item = NULL;
			LeaveCriticalSection(&MemoryAccess);
			// release critical section when all memory has been deallocated
			if (isInitMemory && 0 == MemoryAllocated) {
				DeleteCriticalSection(&MemoryAccess);
				isInitMemory = false;
			}
		}
		static CmString& getMemoryState(bool _isClearType = false);
		static bool clearMemoryState();

		friend	class CmLString;
		friend	class CmMString;
		friend	class CmStringFTL;

	protected:
		// full content fields (allocated if not NULL)
		int8* pText;		  //char field
		int16* pUnicode;  // Unicode conversion
		size_t  Length;		//string length

		// substring descriptors for 'active/highlight'
		int32 LineNumber;
		uint32 LineStart;
		uint32 ActiveStart;
		uint32 ActiveLength;
		uint32 HighlightStart;
		uint32 HighlightLength;

		// index for all files and subfolders in a folder
		HANDLE hFile;
		HANDLE hFolder;
	};

	//----------------------------------------------------------------------------
	// CmPath: dealing with strings that represent a path
	//----------------------------------------------------------------------------
	class CmPath : public CmString
	{
	public:
		CmPath();
		CmPath(CmString& _Head, CmString& _Tail);
		virtual ~CmPath();


	};

	//----------------------------------------------------------------------------
	// CmLString: represents a list of CmString
	//----------------------------------------------------------------------------
	//
	// NOTE: Implementation has been done for a LOW number of strings in the list.
	//       In case of a huge amount of strings the implementation should be 
	//       improved by more performant algorithms.
	//
	class CmLString : public CmString
	{
	public:
		// constructor and destructor
		CmLString();
		virtual ~CmLString();

		//-----access-functions-------------------------------------------------------

		/** addString
		*   A new element will be added to the list of strings
		*/
		bool addString(const CmString& _String);

		/** clearList
		*  All subsequent elements of the list will be removed except current element
		*/
		bool clearList();

		/** getSize.
		*  The number of strings in the list will be returned.
		*/
		int32 getSize();

		/** getString.
		*  Return Nth element of the string list, starting with index 0. If the index
		*  exceeds the list size then the last element will be returned.
		*/
		CmString& operator[](int32 _Index);

		/** getLowestString.
		*  The alphabetically lowest string will be returned
		*/
		CmString& getLowestString();

		/** getMatchingString.
		*  A string that matches given pattern will be returned.
		*/
		bool getMatchingString(CmString& _MatchingString, const CmString& _Pattern);

		//-----functions-for-maintaining-a-list-of-strings----------------------------

		friend CmString;

	private:
		CmLString** getAdrNext();
		CmLString* getNext();
		CmString* setNext(CmLString*);

	private:
		CmLString* pNext;
	};


	//----------------------------------------------------------------------------
	// CmMString: performs string matching
	//----------------------------------------------------------------------------
	//
	class CmMString : public CmString
	{
	public:
		// constructor and destructor
		CmMString(const CmString* SourceString, const int8* _Delimiters = NULL);
		CmMString();
		virtual ~CmMString();

	public:
		// Isolate a substring that is delimited by one of a given set 
		//  of delimiter characters starting from an indexed position
		void resetPosition();
		int8* getNextDelimitedString();
		int8* setDelimiters(const int8* _Delimiters);
		const CmString* setSourceString(const CmString* _SourceString);

	private:
		const CmString* pSourceString;
		CmString		mSetOfDelimiters;
		int32			CurrentPosition;
	};


	//----------------------------------------------------------------------------
	// CmVector: maintains a vector of values
	//----------------------------------------------------------------------------
	//
	template <typename T>
	class CmVector
	{
	public:
		/** CmVector.
		*  @_InitialValue: all elements will be initially set to that value
		*  @_Length: the length and size will be set to exactly that value
		*  @_SizeExtension: factor for additional space when extending size
		*                   NOTE: 1.6 has been found optimal for automatically
		*                   extending the vector by incrementing index by 1.
		*/
		CmVector(int32 _Length = 0, double _SizeExtension = 1.6) {
			// initialize workspace
			Vector = NULL;
			// validate length
			_Length < 0 ? _Length = 0 : 0;
			Length = _Length;
			Size = _Length;
			SizeExtension = _SizeExtension;
			NextIndex = 0;
			Scalar = T(0);
			// create and initialize a vector of T values 
			if (_Length > 0) {
				Vector = CmString::allocateMemory<T>(Size, isCmVector);
				for (int32 i = 0; i < _Length; i++) {
					Vector[i] = T(0);
				}
			}
		}
		virtual ~CmVector() {
			if (NULL != Vector) {
				CmString::releaseMemory<T>(Vector, Size, isCmVector);
			}
		}

	public:
		/** set/getLength/getMaxIndex.
		*  Vector's current length will be adjusted/returned.
		*/
		int32 setLength(int32 _NewLength) {
			// validate new length
			_NewLength < 0 ? _NewLength = Length : 0;
			// handle several cases
			if (_NewLength > Length && _NewLength <= Size) {
				// new length fits inside current size
				Length = _NewLength;
			}
			else if (_NewLength > Length && _NewLength > Size) {
				// extend size
				Length = adjustSize(_NewLength);
			}
			else if (_NewLength < Length) {
				// reduce length and force a size adjustment
				Length = adjustSize(_NewLength);
			}

			// reset next index
			NextIndex = 0;

			return Length;
		}
		int32 getLength() {
			return Length;
		}
		int32 getMaxIndex() {
			return Length - 1;
		}

	public:
		/** getData. A pointer to the data field will be returned
		*/
		T* getData() {
			return Vector;
		}

	public:
		/** set/getScalar.
		*  A scalar value will be stored/retrieved.
		*/
		void setScalar(T _Scalar) {
			Scalar = _Scalar;
		}
		const T& getScalar() {
			return Scalar;
		}

	public:
		/** operator[].
		*  A vector element will be returned according to specified index.
		*  If the index exceeds the range then the vector will be extended to
		*  contain that number of elements
		*/
		T& operator[](int32 _Index) {
			// evaluate index
			if (Size < _Index + 1) {
				// extend size
				Length = adjustSize(_Index + 1);
			}
			// adjust length if it is lower than index+1
			if (Length < _Index + 1) {
				Length = _Index + 1;
			}
			// navigation pointer
			NextIndex = _Index + 1;
			// return addressed vector element
			return Vector[_Index];
		}

	public:
		/** operator=. */
		int32 operator=(CmVector& _Vector) {
			// make this vector same size 
			setLength(_Vector.Length);
			// copy all elements
			for (int i = 0; i < Length; i++) {
				operator[](i) = _Vector[i];
			}
			// copy scalar
			setScalar(_Vector.getScalar());
			// return length
			return Length;
		}

	public:
		/** operator==. */
		bool operator==(CmVector& _Vector) {
			// compare Length
			if (Length != _Vector.Length)
				return false;
			// compare all elements
			for (int i = 0; i < Length; i++) {
				if (operator[](i) != _Vector[i])
					return false;
			}
			// compare scalar
			if (getScalar() != _Vector.getScalar()) return false;
			// return length
			return true;
		}
		/** operator!=. */
		bool operator!=(CmVector& _Vector) {
			if (operator==(_Vector)) return false;
			return true;
		}
	public:
		/** setValue.
		*  Convenience functions for setting multiple vector items in one go.
		*/
		CmVector<T>& clear() {
			setLength(0);
			return *this;
		}
		CmVector<T>& operator()(int32 a) { set(a); return *this; }
		CmVector<T>& set(int32 a) {
			setLength(1);
			Vector[0] = a;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b) { set(a, b); return *this; }
		CmVector<T>& set(int32 a, int32 b) {
			setLength(2);
			Vector[0] = a; Vector[1] = b;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c) { set(a, b, c); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c) {
			setLength(3);
			Vector[0] = a; Vector[1] = b; Vector[2] = c;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d) { set(a, b, c, d); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d) {
			setLength(4);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e) { set(a, b, c, d, e); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e) {
			setLength(5);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f) { set(a, b, c, d, e, f); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f) {
			setLength(6);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g) { set(a, b, c, d, e, f, g); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g) {
			setLength(7);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h) { set(a, b, c, d, e, f, g, h); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h) {
			setLength(8);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i) { set(a, b, c, d, e, f, g, h, i); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i) {
			setLength(9);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j) { set(a, b, c, d, e, f, g, h, i, j); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j) {
			setLength(10);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k) { set(a, b, c, d, e, f, g, h, i, j, k); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k) {
			setLength(11);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l) { set(a, b, c, d, e, f, g, h, i, j, k, l); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l) {
			setLength(12);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m) { set(a, b, c, d, e, f, g, h, i, j, k, l, m); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m) {
			setLength(13);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n) {
			setLength(14);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r; Vector[18] = s;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r; Vector[18] = s; Vector[19] = t;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r; Vector[18] = s; Vector[19] = t; Vector[20] = u;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r; Vector[18] = s; Vector[19] = t; Vector[20] = u; Vector[21] = v;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v, int32 w) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v, int32 w) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r; Vector[18] = s; Vector[19] = t; Vector[20] = u; Vector[21] = v; Vector[22] = w;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v, int32 w, int32 x) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v, int32 w, int32 x) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r; Vector[18] = s; Vector[19] = t; Vector[20] = u; Vector[21] = v; Vector[22] = w; Vector[23] = x;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v, int32 w, int32 x, int32 y) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v, int32 w, int32 x, int32 y) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r; Vector[18] = s; Vector[19] = t; Vector[20] = u; Vector[21] = v; Vector[22] = w; Vector[23] = x; Vector[24] = y;
			return *this;
		}
		CmVector<T>& operator()(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v, int32 w, int32 x, int32 y, int32 z) { set(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z); return *this; }
		CmVector<T>& set(int32 a, int32 b, int32 c, int32 d, int32 e, int32 f, int32 g, int32 h, int32 i, int32 j, int32 k, int32 l, int32 m, int32 n, int32 o, int32 p, int32 q, int32 r, int32 s, int32 t, int32 u, int32 v, int32 w, int32 x, int32 y, int32 z) {
			setLength(15);
			Vector[0] = a; Vector[1] = b; Vector[2] = c; Vector[3] = d; Vector[4] = e; Vector[5] = f; Vector[6] = g; Vector[7] = h; Vector[8] = i; Vector[9] = j; Vector[10] = k; Vector[11] = l; Vector[12] = m; Vector[13] = n; Vector[14] = o; Vector[15] = p; Vector[16] = q; Vector[17] = r; Vector[18] = s; Vector[19] = t; Vector[20] = u; Vector[21] = v; Vector[22] = w; Vector[23] = x; Vector[24] = y; Vector[25] = z;
			return *this;
		}

	public:
		/** getNextIndex.
		*  The next position related to most recent access will be returned.
		*/
		int32 getNextIndex() {
			return NextIndex;
		}
		/** setNextIndex/reset.
		*  The next position will be set/reset.
		*/
		void setNextIndex(int32 _NextIndex) {
			// evaluate next index against length
			NextIndex = _NextIndex < Length ? _NextIndex : Length - 1;
		}
		CmVector<T>& reset() {
			NextIndex = 0;
			return *this;
		}

	public:
		/** allItems.
		*  The next item after most recently returned item will be returned.
		*/
		bool allItems(T& _Item) {
			bool isValidIndex = NextIndex < Length;
			isValidIndex ? _Item = operator[](NextIndex) : 0;
			return isValidIndex;
		}

	private:
		int32 adjustSize(int32 _NewLength) {
			// validate new length
			_NewLength < 0 ? _NewLength = (int32)(Size / SizeExtension) : 0;
			// provide for extended/reduced size
			int32 NewSize = (int32)(_NewLength * SizeExtension);
			// extend to next 4k size
			const int32 Size4k = 4096 / sizeof(T);
			NewSize = Size4k * (1 + (NewSize - 1) / Size4k);

			// extend vector
			T* OldVector = Vector;
			NewSize > 0 ? Vector = CmString::allocateMemory<T>(NewSize, isCmVector) : Vector = NULL;

			for (int32 i = 0; i < NewSize; i++) {
				Vector[i] = i < Length && i < _NewLength ? OldVector[i] : (T)0;
			}
			if (NULL != OldVector) {
				CmString::releaseMemory<T>(OldVector, Size, isCmVector);
			}
			Size = NewSize;

			return _NewLength;
		}

		//--------workspace-----------------------------------------------------------

	private:
		// vector state
		T* Vector;
		int32 Length;
		int32 Size;
		double SizeExtension;
		// navigation
		int32 NextIndex;
		// scalar 
		T Scalar;
	};
	typedef CmVector<int32> CmIndex;

	// 2018-09-19/ToDo: fix access problems to member variables
	typedef CmVector<CmPoint2D> CmPolygon;




}  // namespace Cosmos

using namespace Cosmos;


//----------------------------------------------------------------------------
// CmString
//----------------------------------------------------------------------------
CmString::CmString()
{
	initWorkspace();
}

CmString::CmString(const int8* _Text)
{
	initWorkspace();
	setText(_Text);
}

CmString::CmString(const int8* _Text, uint32 _Length)
{
	initWorkspace();

	// Generate a text array of given buffer length
	Length = _Length;
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);

	// Copy text to the array and terminate with a '0'
	memcpy(pText, _Text, _Length);
	pText[Length] = 0;
}

CmString::CmString(uint32 _Length, uint8 _Init)
{
	initWorkspace();

	//Generate text array and initialize it with a token
	Length = _Length;
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);
	memset(pText, _Init, Length);
	pText[Length] = 0;
}

CmString::CmString(const CmString& _String)
{
	initWorkspace();
	setText(_String.getText());
}

CmString::CmString(CmVector<uint8>& _VectorUInt8)
{
	initWorkspace();

	//Generate text array
	Length = _VectorUInt8.getLength();
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);
	memcpy(pText, _VectorUInt8.getData(), Length);
	pText[Length] = 0;
}


CmString::~CmString()
{
	//Remove text/Unicode arrays from heap
	if (NULL != pText) {
		releaseMemory<int8>(pText, int32(Length + 1), isInt8);
	}
	if (NULL != pUnicode) {
		releaseMemory<int16>(pUnicode, int32(Length + 1), isInt16);
	}
}

void CmString::initWorkspace()
{
	//Reset text array and length
	pText = NULL;
	pUnicode = NULL;
	Length = 0;
	// initialize descriptor variables
	LineStart = 0;
	LineNumber = 0;
	ActiveStart = 0;
	ActiveLength = 0;
	HighlightStart = 0;
	HighlightLength = 0;
	// initialize file search
	hFile = INVALID_HANDLE_VALUE;
	hFolder = INVALID_HANDLE_VALUE;
}


// unit test
bool CmString::testCmString()
{
	// set/get functions for basic types
	uint8 Val8 = 0x12;
	uint16 Val16 = 0x1234;
	uint32 Val32 = 0x12345678;
	uint64 Val64 = 0x0123456789ABCDEF;

	CmString Str8(2 * sizeof(uint8));
	CmString Str16(2 * sizeof(uint16));
	CmString Str32(2 * sizeof(uint32));
	CmString Str64(2 * sizeof(uint64));

	uint8 TestVal8;
	uint16 TestVal16;
	uint32 TestVal32;
	uint64 TestVal64;

	Str8.setAt(sizeof(uint8), Val8);
	Str16.setAt(sizeof(uint16), Val16);
	Str32.setAt(sizeof(uint32), Val32);
	Str64.setAt(sizeof(uint64), Val64);

	Str8.getAt(sizeof(uint8), TestVal8);
	Str16.getAt(sizeof(uint16), TestVal16);
	Str32.getAt(sizeof(uint32), TestVal32);
	Str64.getAt(sizeof(uint64), TestVal64);

	if (TestVal8 != Val8) return false;
	if (TestVal16 != Val16) return false;
	if (TestVal32 != Val32) return false;
	if (TestVal64 != Val64) return false;

	// conversion functions
	CmString Num;
	CmString NumNeg;
	uint64 Val;
	uint64 Val5;
	int32 Val5Neg;
	// uint32
	Num = "1234567890";
	NumNeg = "-1234567890";
	Val = 1234567890;
	Val5 = 123456;
	Val5Neg = -12345;
	if (Val != Num.getNumAsUint64()) return false;
	if (Val5 != Num.getNumAsUint64(0, 5)) return false;
	if (Val != Num.getNumAsInt32()) return false;
	if (Val5 != Num.getNumAsInt32(0, 5)) return false;
	if (Val5Neg != NumNeg.getNumAsInt32(0, 5)) return false;
	// uint64
	Num = "12345678901234567890";
	Val = 12345678901234567890;
	if (Val != Num.getNumAsUint64()) return false;
	// hex
	Num = "0x1234567890aBcDeF";
	Val = 0x1234567890aBcDeF;
	if (Val != Num.getNumAsUint64()) return false;
	// test wrong number formats
	Num = "1.23";
	if ((1 != Num.getNumAsUint64()) || (false == Num.isConversionError)) return false;
	Num = "0123A";
	if ((123 != Num.getNumAsUint64()) || (false == Num.isConversionError)) return false;
	Num = "Ca";
	if ((0 != Num.getNumAsUint64()) || (false == Num.isConversionError))	return false;
	Num = " 123";
	if ((123 != Num.getNumAsUint64()) || (true == Num.isConversionError))	return false;
	Num = "123 ";
	if ((123 != Num.getNumAsUint64()) || (true == Num.isConversionError))	return false;
	// double: check against a maximal acceptable error 
	const double fError = 1e-15;
	double fVal;
	// double without exponent
	Num = "-9.0123";
	fVal = -9.0123;
	if (fVal != Num.getNumAsDouble()) return false;
	// double without fraction
	Num = "123456789e+12";
	fVal = 123456789e+12;
	if (fabs((fVal - Num.getNumAsDouble()) / fVal) > fError) return false;
	// double with fraction and exponent
	Num = "1.23456789e-12";
	fVal = 1.23456789e-12;
	if (fabs((fVal - Num.getNumAsDouble()) / fVal) > fError) return false;
	// test wrong number formats
	Num = "1.23456789e--12";
	if (0 != Num.getNumAsDouble())	return false;
	Num = "1.23-456789e-12";
	if (0 != Num.getNumAsDouble()) return false;
	Num = "0-1.23456789e-12";
	if (0 != Num.getNumAsDouble())	return false;
	Num = "1.23456789e1-2";
	if (0 != Num.getNumAsDouble())	return false;
	Num = "1.2345b6789e12";
	if (1.2345 != Num.getNumAsDouble() || false == Num.isConversionError)	return false;
	Num = "12345678901234567";
	if (0 != Num.getNumAsDouble())	return false;
	// double to string
	CmString NumDouble;
	Num = "1.23456789012345";
	fVal = 1.23456789012345;
	NumDouble.double2String(fVal); if (NumDouble != Num) return false;
	Num = "12345678901234567000";
	fVal = 12345678901234567890.0;
	NumDouble.double2String(fVal); if (NumDouble != Num) return false;
	fVal = 1.2345678901234567890e+19;
	NumDouble.double2String(fVal); if (NumDouble != Num) return false;
	Num = "0.0000000000123457";
	fVal = 0.00000000001234567890;
	NumDouble.double2String(fVal); if (NumDouble != Num) return false;
	fVal = 0.1234567890e-10;
	NumDouble.double2String(fVal); if (NumDouble != Num) return false;
	Num = "0";
	fVal = MIN_DOUBLE;
	NumDouble.double2String(fVal); if (NumDouble != Num) return false;
	Num = "MAX_DOUBLE";
	fVal = MAX_DOUBLE;
	NumDouble.double2String(fVal); if (NumDouble != Num) return false;
	// month recognition
	CmString Months("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
	if (1 != Months.getMonthAsInt32(0, 2)) return false;
	if (2 != Months.getMonthAsInt32(4, 6)) return false;
	if (3 != Months.getMonthAsInt32(8, 10)) return false;
	if (4 != Months.getMonthAsInt32(12, 14)) return false;
	if (5 != Months.getMonthAsInt32(16, 18)) return false;
	if (6 != Months.getMonthAsInt32(20, 22)) return false;
	if (7 != Months.getMonthAsInt32(24, 26)) return false;
	if (8 != Months.getMonthAsInt32(28, 30)) return false;
	if (9 != Months.getMonthAsInt32(32, 34)) return false;
	if (10 != Months.getMonthAsInt32(36, 38)) return false;
	if (11 != Months.getMonthAsInt32(40, 42)) return false;
	if (12 != Months.getMonthAsInt32(44, 46)) return false;


	// test match() functionality
	CmString Match("abc_123@...");
	if (false == Match.matchPattern(Match, ".*")) return false;
	if (false == Match.matchPattern(Match, "abc_123@...")) return false;


	// further tests...

	CmLString StringList;

	// PROBLEM: This loop crashes when the item number is ~6350 or higher (release x64) 
	//          respectively ~1000 or higher (debug win32)
	for (int32 i = 0; i < 1000; i++) {
		StringList.addString("item");
	}

	return true;
}

const int8* CmString::getText() const
{
	//Return text array or empty string
	if (NULL != pText) {
		return pText;
	}
	else {
		return (int8*)"";
	}
}

CmString CmString::getTextDir() const
{
	CmString Dir = getText();
	// replace forbidden chars by _ (underscore)
	for (int32 i = 0; i < int32(Dir.getLength()); i++) {
		if (Dir[i] == '/') Dir.setAt(i, '_');
	}

	return Dir;
}

const uint8* CmString::getBinary() const
{
	//Return binary or empty field
	if (NULL != pText) {
		return (const uint8*)pText;
	}
	else {
		return (const uint8*)"";
	}
}

const int16* CmString::getUnicode()
{
	// re-generate a Unicode array
	if (NULL != pUnicode) {
		releaseMemory<int16>(pUnicode, int32(Length + 1), isInt16);
	}
	pUnicode = allocateMemory<int16>(int32(Length + 1), isInt16);
	// convert chars to Unicode
	memset(pUnicode, 0, 2 * (Length + 1));
	for (int i = 0; i < (int32)Length; i++)
		pUnicode[i] = pText[i];

	return pUnicode;
}

int8* CmString::getBuffer() const
{
	// Check length and return text array
	if (Length > 0) {
		return pText;
	}
	else {
		return (int8*)EMPTY_STRING;
	}
}

bool CmString::isEmpty()
{
	return NULL == pText ? true : false;
}
uint32 CmString::getLength() const
{
	//Return text length
	return uint32(Length);
}

int8* CmString::setText(const int8* _Text)
{
	// delete previous text
	if (NULL != pText) {
		releaseMemory<int8>(pText, int32(Length + 1), isInt8);
	}
	pText = NULL;
	Length = 0;

	// check pointer
	if (NULL == _Text) return pText;

	//Replace existing text array with a new string
	// ToDo: this may cause a crash when _Text is invalid (to be solved on a higher level)
	Length = (uint32)strlen(_Text);
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);
	if (Length > 0) {
		memcpy(pText, _Text, Length);
	}
	pText[Length] = 0;
	return pText;
}

int8* CmString::setData(const uint8* _Data, size_t _Length)
{
	//Replace existing text array with a new string
	if (NULL != pText) {
		releaseMemory<int8>(pText, int32(Length + 1), isInt8);
	}
	Length = _Length;
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);
	if (NULL != _Data && Length > 0) {
		memcpy(pText, _Data, Length);
	}
	pText[Length] = 0;
	return pText;
}

void CmString::reset()
{
	LineStart = 0;
	LineNumber = 0;
	ActiveStart = 0;
	ActiveLength = 0;
	HighlightStart = 0;
	HighlightLength = 0;
}
void CmString::clear()
{
	if (NULL != pText) {
		releaseMemory<int8>(pText, int32(Length + 1), isInt8);
	}
	pText = NULL;
	Length = 0;
}
void CmString::fill(size_t _Length, uint8 _Digit)
{
	setLength(_Length);
	// fill
	if (_Length > 0) {
		memset(pText, _Digit, _Length);
	}
}
void CmString::setText(const CmString& _String)
{
	//Replace existing text array with the content of a given CmString object
	setText(_String.getText());
}

int8* CmString::setText(const int8* _First, const int8* _Last)
{
	// delete previous text
	if (NULL != pText) {
		releaseMemory<int8>(pText, int32(Length + 1), isInt8);
	}
	pText = NULL;
	Length = 0;

	// check pointers
	if ((NULL == _First) || (NULL == _Last)) return pText;

	// Replace existing text array with a given substring
	if (_First <= _Last) {
		Length = (uint32)(_Last - _First + 1);
	}
	else {
		Length = 0;
	}
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);
	if (Length > 0) {
		memcpy(pText, _First, Length);
	}
	pText[Length] = 0;
	return pText;
}

void CmString::setLength(size_t _Length)
{
	// clear previous text array
	if (NULL != pText) {
		releaseMemory<int8>(pText, int32(Length + 1), isInt8);
	}
	//Reallocate the text array with a new size
	Length = _Length;
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);
	// clear new data field
	memset(pText, 0, Length + 1);
}

void CmString::adjustLength(size_t _Length)
{
	// preserve old text
	size_t OldLength = Length;
	int8* pOldText = pText;
	// allocate a new text array with new size
	Length = _Length;
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);
	// clear new data field
	memset(pText, 0, Length + 1);
	// copy old data
	if (Length <= (int32)OldLength)
		memcpy(pText, pOldText, Length);
	else
		memcpy(pText, pOldText, OldLength);
	// delete old text array
	releaseMemory<int8>(pOldText, int32(OldLength + 1), isInt8);
}

void CmString::setAt(const uint32 _Pos, int8 _Val8)
{
	// replace a char in the text array
	if (_Pos <= Length - sizeof(int8)) {
		pText[_Pos] = _Val8;
	}
}
void CmString::setAt(const uint32 _Pos, uint8 _Val8)
{
	// replace an unsigned char in the text array
	if (_Pos <= Length - sizeof(uint8)) {
		pText[_Pos] = _Val8;
	}
}
void CmString::setAt(const uint32 _Pos, bool _ValBool)
{
	// replace a bool value in the text array
	if (_Pos <= Length - sizeof(bool)) {
		memcpy(pText + _Pos, &_ValBool, sizeof(bool));
	}
}
void CmString::setAt(const uint32 _Pos, int16 _Val16)
{
	// replace a short value in the text array
	if (_Pos <= Length - sizeof(int16)) {
		memcpy(pText + _Pos, &_Val16, sizeof(int16));
	}
}
void CmString::setAt(const uint32 _Pos, uint16 _Val16)
{
	// replace an unsigned short value in the text array
	if (_Pos <= Length - sizeof(uint16)) {
		memcpy(pText + _Pos, &_Val16, sizeof(uint16));
	}
}
void CmString::setAt(const uint32 _Pos, int32 _Val32)
{
	// replace an integer value in the text array
	if (_Pos <= Length - sizeof(int32)) {
		memcpy(pText + _Pos, &_Val32, sizeof(int32));
	}
}
void CmString::setAt(const uint32 _Pos, uint32 _Val32)
{
	// replace an unsigned integer value in the text array
	if (_Pos <= Length - sizeof(uint32)) {
		memcpy(pText + _Pos, &_Val32, sizeof(uint32));
	}
}
void CmString::setAt(const uint32 _Pos, float _ValFloat)
{
	// replace a float value in the text array
	if (_Pos <= Length - sizeof(float)) {
		memcpy(pText + _Pos, &_ValFloat, sizeof(float));
	}
}
void CmString::setAt(const uint32 _Pos, int64 _Val64)
{
	// replace a long long value in the text array
	if (_Pos <= Length - sizeof(int64)) {
		memcpy(pText + _Pos, &_Val64, sizeof(int64));
	}
}
void CmString::setAt(const uint32 _Pos, uint64 _Val64)
{
	// replace an unsigned long long value in the text array
	if (_Pos <= Length - sizeof(uint64)) {
		memcpy(pText + _Pos, &_Val64, sizeof(uint64));
	}
}
void CmString::setAt(const uint32 _Pos, double _ValDouble)
{
	// replace a double value in the text array
	if (_Pos <= Length - sizeof(double)) {
		memcpy(pText + _Pos, &_ValDouble, sizeof(double));
	}
}
void CmString::setAt(const uint32 _Pos, void* _ValPointer)
{
	// replace a pointer value in the text array
	if (_Pos <= Length - sizeof(void*)) {
		memcpy(pText + _Pos, &_ValPointer, sizeof(void*));
	}
}

void CmString::getAt(const uint32 _Pos, int8& _Val8)
{
	// return a char from text array
	if (_Pos <= Length - sizeof(int8)) {
		_Val8 = pText[_Pos];
	}
}
void CmString::getAt(const uint32 _Pos, uint8& _Val8)
{
	// return an unsigned char from text array
	if (_Pos <= Length - sizeof(uint8)) {
		_Val8 = pText[_Pos];
	}
}
void CmString::getAt(const uint32 _Pos, bool& _ValBool)
{
	// return a bool value from text array
	if (_Pos <= Length - sizeof(bool)) {
		memcpy(&_ValBool, pText + _Pos, sizeof(bool));
	}
}
void CmString::getAt(const uint32 _Pos, int16& _Val16)
{
	// return a short from text array
	if (_Pos <= Length - sizeof(int16)) {
		memcpy(&_Val16, pText + _Pos, sizeof(int16));
	}
}
void CmString::getAt(const uint32 _Pos, uint16& _Val16)
{
	// return an unsigned short from text array
	if (_Pos <= Length - sizeof(uint16)) {
		memcpy(&_Val16, pText + _Pos, sizeof(uint16));
	}
}
void CmString::getAt(const uint32 _Pos, int32& _Val32)
{
	// return an integer from text array
	if (_Pos <= Length - sizeof(int32)) {
		memcpy(&_Val32, pText + _Pos, sizeof(int32));
	}
}
void CmString::getAt(const uint32 _Pos, uint32& _Val32)
{
	// return an unsigned integer from text array
	if (_Pos <= Length - sizeof(uint32)) {
		memcpy(&_Val32, pText + _Pos, sizeof(uint32));
	}
}
void CmString::getAt(const uint32 _Pos, float& _ValFloat)
{
	// return a float from text array
	if (_Pos <= Length - sizeof(float)) {
		memcpy(&_ValFloat, pText + _Pos, sizeof(float));
	}
}
void CmString::getAt(const uint32 _Pos, int64& _Val64)
{
	// return a long long from text array
	if (_Pos <= Length - sizeof(int64)) {
		memcpy(&_Val64, pText + _Pos, sizeof(int64));
	}
}
void CmString::getAt(const uint32 _Pos, uint64& _Val64)
{
	// return an unsigned long long from text array
	if (_Pos <= Length - sizeof(uint64)) {
		memcpy(&_Val64, pText + _Pos, sizeof(uint64));
	}
}
void CmString::getAt(const uint32 _Pos, double& _ValDouble)
{
	// return a double from text array
	if (_Pos <= Length - sizeof(uint64)) {
		memcpy(&_ValDouble, pText + _Pos, sizeof(uint64));
	}
}
void CmString::getAt(const uint32 _Pos, void*& _ValPointer)
{
	// return a double from text array
	if (_Pos <= Length - sizeof(void*)) {
		memcpy(&_ValPointer, pText + _Pos, sizeof(void*));
	}
}

void CmString::setValue(int8 _Val8)
{
	setLength(8);
	setAt(0, _Val8);
}
void CmString::setValue(uint8 _Val8)
{
	setLength(8);
	setAt(0, _Val8);
}
void CmString::setValue(bool _ValBool)
{
	setLength(8);
	setAt(0, _ValBool);
}
void CmString::setValue(int16 _Val16)
{
	setLength(8);
	setAt(0, _Val16);
}
void CmString::setValue(uint16 _Val16)
{
	setLength(8);
	setAt(0, _Val16);
}
void CmString::setValue(int32 _Val32)
{
	setLength(8);
	setAt(0, _Val32);
}
void CmString::setValue(uint32 _Val32)
{
	setLength(8);
	setAt(0, _Val32);
}
void CmString::setValue(float _ValFloat)
{
	setLength(8);
	setAt(0, _ValFloat);
}
void CmString::setValue(int64 _Val64)
{
	setLength(8);
	setAt(0, _Val64);
}
void CmString::setValue(uint64 _Val64)
{
	setLength(8);
	setAt(0, _Val64);
}
void CmString::setValue(double _ValDouble)
{
	setLength(8);
	setAt(0, _ValDouble);
}
void CmString::setValue(void* _ValPointer)
{
	setLength(8);
	setAt(0, _ValPointer);
}

uint32 CmString::operator[](uint32 _uPos) const
{
	//Return single character at a given position in the text array
	if (((int32)_uPos >= Length) || (NULL == pText)) {
		return 0;
	}
	else {
		return pText[_uPos];
	}
}

void CmString::operator+(const CmString _String)
{
	operator += (_String.getBuffer());
}

void CmString::operator += (const CmString& _String)
{
	operator += (_String.getBuffer());
}

void CmString::operator += (const int8* _Text)
{
	// check string to be added
	if (NULL == _Text) return;

	//Preserve existing text array
	int8* pOldText = pText;
	size_t OldLength = Length;

	//Add existing and new string into the new text array
	size_t StringLength = strlen(_Text);
	Length += StringLength;
	pText = allocateMemory<int8>(int32(Length + 1), isInt8);
	pText[Length] = 0;
	if (OldLength > 0) {
		memcpy(pText, pOldText, OldLength);
	}
	if (Length > OldLength) {
		memcpy(pText + OldLength, _Text, StringLength);
	}
	releaseMemory<int8>(pOldText, int32(OldLength + 1), isInt8);
}

void CmString::operator += (uint32 _Val32)
{
	CmString mString;
	mString = _Val32;
	operator += (mString.getBuffer());
}

void CmString::operator += (int32 _Val32)
{
	CmString mString;
	mString = _Val32;
	operator += (mString.getBuffer());
}

void CmString::operator += (uint64 _Val64)
{
	CmString mString;
	mString = _Val64;
	operator += (mString.getBuffer());
}

void CmString::operator += (int64 _Val64)
{
	CmString mString;
	mString = _Val64;
	operator += (mString.getBuffer());
}

void CmString::operator = (const CmString& _String)
{
	//Take over content from a given CmString object
	setText(_String.getText());
}

void CmString::operator = (const int8* _Text)
{
	//Take over content from a given CmString object
	setText(_Text);
}

void CmString::operator=(const int32 _Val32)
{
	char achNum[20];
	SPRINTF4(achNum, sizeof(achNum), "%d", _Val32);
	setText(achNum);
}

void CmString::operator=(const uint32 _Val32)
{
	char achNum[20];
	SPRINTF4(achNum, sizeof(achNum), "%u", _Val32);
	setText(achNum);
}

void CmString::operator=(const int64 _Val64)
{
#define NUM_BILLION 1000000000
	char achNum[40 + 1];
	char* achPos = achNum;
	uint64 Num = (uint64)_Val64;
	if (_Val64 < 0) {
		*achPos++ = '-';
		Num = (uint64)(-_Val64);
	}
	if (Num / NUM_BILLION >= NUM_BILLION) {
		SPRINTF6(achPos, sizeof(achNum) - 1, "%u%09u%09u",
			(uint32)((Num / NUM_BILLION) / NUM_BILLION),
			(uint32)((Num / NUM_BILLION) % NUM_BILLION),
			(uint32)(Num % NUM_BILLION));
	}
	else if (Num >= NUM_BILLION) {
		SPRINTF5(achPos, sizeof(achNum) - 1, "%u%09u",
			(uint32)(Num / NUM_BILLION),
			(uint32)(Num % NUM_BILLION));
	}
	else {
		SPRINTF4(achPos, sizeof(achNum) - 1, "%u", (uint32)Num);
	}
	setText(achNum);
}

void CmString::operator=(const uint64 _Val64)
{
	char achNum[40];
	if (_Val64 / NUM_BILLION >= NUM_BILLION) {
		SPRINTF6(achNum, sizeof(achNum), "%u%09u%09u",
			(uint32)((_Val64 / NUM_BILLION) / NUM_BILLION),
			(uint32)((_Val64 / NUM_BILLION) % NUM_BILLION),
			(uint32)(_Val64 % NUM_BILLION));
	}
	else if (_Val64 >= NUM_BILLION) {
		SPRINTF5(achNum, sizeof(achNum), "%u%09u",
			(uint32)(_Val64 / NUM_BILLION),
			(uint32)(_Val64 % NUM_BILLION));
	}
	else {
		SPRINTF4(achNum, sizeof(achNum), "%u", (uint32)_Val64);
	}
	setText(achNum);
}

bool CmString::operator==(const CmString& _String) const
{
	return operator==(_String.getText());
}
bool CmString::operator==(const int8* _Text) const
{
	// check pointer
	if (NULL == _Text && NULL != pText) return false;
	if (NULL != _Text && NULL == pText) return false;
	if (NULL == _Text && NULL == pText) return true;

	//Compare the text array with a given string
	if (strlen(_Text) != Length) {
		return false;
	}
	else {
		if (0 == strcmp(_Text, pText)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool CmString::operator!=(const CmString& _String) const
{
	return operator!=(_String.getText());
}
bool CmString::operator!=(const int8* _Text) const
{
	// check pointer
	if (NULL == _Text && NULL != pText) return true;
	if (NULL != _Text && NULL == pText) return true;
	if (NULL == _Text && NULL == pText) return false;

	//Compare the text array with a given string
	if (strlen(_Text) != Length) {
		return true;
	}
	else {
		if (0 != strcmp(_Text, pText)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool CmString::operator<(const CmString& _String) const
{
	return operator<(_String.getText());
}
bool CmString::operator<(const int8* _Text) const
{
	// check pointer
	if (NULL == _Text) return false;

	//Compare the text array with a given string upto shortest length
	if ((Length > strlen(_Text)) || (NULL == pText)) {
		return false;
	}
	else {
		if (0 < strncmp(_Text, pText, Length)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool CmString::operator<=(const CmString& _String) const
{
	return operator<=(_String.getText());
}
bool CmString::operator<=(const int8* _Text) const
{
	// check pointer
	if (NULL == _Text) return false;

	//Compare the text array with a given string upto shortest length
	if ((Length > strlen(_Text)) || (NULL == pText)) {
		return false;
	}
	else {
		if (0 <= strncmp(_Text, pText, Length)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool CmString::operator>(const CmString& _String) const
{
	return operator>(_String.getText());
}
bool CmString::operator>(const int8* _Text) const
{
	// check pointer
	if (NULL == _Text) return false;

	//Compare the text array with a given string upto shortest length
	if ((Length < strlen(_Text)) || (NULL == pText)) {
		return false;
	}
	else {
		if (0 > strncmp(_Text, pText, Length)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool CmString::operator>=(const CmString& _String) const
{
	return operator>=(_String.getText());
}
bool CmString::operator>=(const int8* _Text) const
{
	// check pointer
	if (NULL == _Text) return false;

	//Compare the text array with a given string upto shortest length
	if ((Length < strlen(_Text)) || (NULL == pText)) {
		return false;
	}
	else {
		if (0 >= strncmp(_Text, pText, Length)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool CmString::isPrefix(const CmString& _String) const
{
	return isPrefix(_String.getText());
}
bool CmString::isPrefix(const int8* _Text) const
{
	// check pointer
	if (NULL == _Text || NULL == pText) return false;

	//Compare the first characters in the text array with a given header string
	return 0 == strncmp(pText, _Text, strlen(_Text));
}

bool CmString::float2String(float _Value, int32 _Precision, int32 _Digits)
{
	return double2String(_Value, _Precision, _Digits == -1 ? 9 : _Digits);
}
bool CmString::double2String(double _Value, int32 _Precision, int32 _Digits)
{
	char Num[64] = { "0" };
	char Format[64];
	// estimate precision for not loosing information
	if (-1 == _Precision) {
		double ValueRemainder = _Value - nearbyint(_Value);
		int32 DigitsInt = int32(floor(log10(fabs(_Value))) + 1); DigitsInt < 0 ? DigitsInt = 0 : 0;
		const int32 MaxPrecision = _Digits == -1 ? 16 : _Digits - 1 - DigitsInt;
		double MinRemainder = _Digits == -1 ? 3e-16 : 3 * pow(10.0, -_Digits + 1 + DigitsInt);
		for (_Precision = 0; _Precision <= MaxPrecision; _Precision++, ValueRemainder *= 10, MinRemainder *= 10) {
			if (fabs(ValueRemainder - nearbyint(ValueRemainder)) < MinRemainder) break;
		}
		// determine total digits before and after decimal point plus optional decimal separator
		_Digits = DigitsInt + _Precision + (_Precision > 0 ? 1 : 0);
	}
	// limit value to a reasonable range -1e35..1e35 (smallest quantum in our universe or its inverse)
	_Value = _Value < -1e+35 ? -1e+35 : _Value > 1e+35 ? 1e+35 : _Value;
	if (-1 == _Precision && -1 == _Digits) {
		sprintf_s(Num, sizeof(Num), "%.0lf", _Value);
	}
	else if (-1 == _Digits || 9 == _Digits /*for float*/) {
		sprintf_s(Format, sizeof(Format), "%%.%dlf", _Precision);
		sprintf_s(Num, sizeof(Num), Format, _Value);
	}
	else if (_Digits <= 20 && _Precision <= 20) {
		sprintf_s(Format, sizeof(Format), "%%%d.%dlf", _Digits, _Precision);
		sprintf_s(Num, sizeof(Num), Format, _Value);
		/* 2024-01-01: force last digits above accuracy level to zero */
		const int32 AccuracyLevel = 17;
		if (_Digits > AccuracyLevel) {
			memset(Num + AccuracyLevel, '0', _Digits - AccuracyLevel);
		}
	}
	else {
		sprintf_s(Num, "%s", "MAX_DOUBLE");
	}
	setText(Num);

	return true;
}
CmString CmString::Float2String(double _Value, int32 _Precision, int32 _Digits)
{
	CmString Float;
	Float.double2String(_Value, _Precision, _Digits);
	return Float;
}
CmString CmString::Double2String(double _Value, int32 _Precision, int32 _Digits)
{
	CmString Double;
	Double.double2String(_Value, _Precision, _Digits);
	return Double;
}
bool CmString::uint2String(uint64 _Value)
{
	// uint64 has maximal 20 digits
	const int32 MaxLength = 20;
	fill(MaxLength, '0');

	// evaluate all digits
	for (int32 i = MaxLength - 1; i >= 0; i--) {
		getBuffer()[i] = '0' + _Value % 10;
		_Value /= 10;
		if (_Value == 0) break;
	}

	// remove leading zero
	if (getBuffer()[0] == '0') {
		int32 LenZ;
		for (LenZ = 1; LenZ < int32(getLength()); LenZ++) {
			if (getBuffer()[LenZ] != '0') break;
		}
		LenZ >= int32(getLength()) ? LenZ = int32(getLength() - 1) : 0;
		assignSubString(int32(getLength() - 1), 0, LenZ);
	}

	return true;
}

CmString CmString::UInt2Hex(uint64 _Value)
{
	CmString Hex;
	Hex.uint2hex(_Value);
	return Hex;
}

bool CmString::uint2hex(uint64 _Value)
{
	char Hex[64] = { 0 };
	sprintf_s(Hex, sizeof(Hex), "%I64X", _Value);
	setText(Hex);

	return true;
}
uint64 CmString::hex2uint64()
{
	uint64 Value = 0;

	// cumulate all hex digits' binary values
	for (int i = 0; i < (int)getLength(); i++) {
		Value *= 16;
		switch ((*this)[i]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': Value += (*this)[i] - '0'; break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f': Value += (*this)[i] - 'a' + 10; break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F': Value += (*this)[i] - 'A' + 10; break;
		case 'x':
		case 'X': break;
		default: break;
		}
	}

	return Value;
}
uint32 CmString::hex2uint32()
{
	return (uint32)hex2uint64();
}
uint16 CmString::hex2uint16()
{
	return (uint16)hex2uint64();
}
uint8 CmString::hex2uint8()
{
	return (uint8)hex2uint64();
}

bool CmString::matchPattern(CmString& _Match, int8 const* _Pattern)
{
	CmLString* MatchList = NULL;
	if (0 == matchPattern(&MatchList, _Pattern)) { if (NULL != MatchList) MatchList->~CmLString(); return false; }
	// return found match
	_Match = MatchList->operator[](0);
	MatchList->~CmLString();
	// verify found match
	if (0 == _Match.getLength()) return false;

	return true;
}
int32 CmString::matchPattern(CmLString& _MatchList, int8 const* _Pattern)
{
	CmLString* MatchList = NULL;
	int32 FoundItems;
	if (0 == (FoundItems = matchPattern(&MatchList, _Pattern))) { if (NULL != MatchList) MatchList->~CmLString(); return 0; }
	// return found matches
	_MatchList.clearList();
	for (int32 i = 0; i < FoundItems; i++) {
		// verify found match
		if (0 == MatchList->operator[](i).getLength()) return 0;
		_MatchList.addString(MatchList->operator[](i));
	}
	MatchList->~CmLString();

	return FoundItems;
}
int32 CmString::matchPattern(CmLString** _ppLMatchList, int8 const* _pPattern)
{
	int8* pFirst = pText; //first matching character
	int8* pSrcText = pText; //current text position
	int8  chMatch = ' ';	 //single character to be found
	int8* szMatch = NULL;	 //group of alternative characters
	int8* pSkipNextSrc = NULL;				//current text position
	int8 const* pSkipPattern = NULL;	//current text position
	uint32 nSkip = 0;									//skip any character
	uint32 nMatch;		     //current match number
	uint32 nMatchMin;		 //minimal match number
	uint32 nMatchMax;		 //maximal match number
	uint32 nLMatch = 0;	     //number of valid result list members

	// check attern
	if (NULL == _pPattern) return false;

	for (;;) {
		//Evaluate next pattern element
		szMatch = NULL;
		switch (*_pPattern) {
		case 0:
			// check if the complete source string has been found
			if (pSrcText == pText + Length && NULL == *_ppLMatchList) {
				*_ppLMatchList = new CmLString;
				(*_ppLMatchList)->setText(pFirst, pSrcText - 1);
				_ppLMatchList = (*_ppLMatchList)->getAdrNext();
				nLMatch++;
			}
			else {
				if (nSkip > 0) break;
			}
			return nLMatch;
		case '(':
			pFirst = pSrcText;
			_pPattern++;
			continue;
		case ')':
			if (NULL == *_ppLMatchList) {
				*_ppLMatchList = new CmLString;
			}
			(*_ppLMatchList)->setText(pFirst, pSrcText - 1);
			_ppLMatchList = (*_ppLMatchList)->getAdrNext();
			nLMatch++;
			_pPattern++;
			nSkip = 0;
			continue;
		case '.':
			_pPattern++;
			//Determine modifiers
			switch (*_pPattern) {
			case'*':
				nSkip = MAX_UINT32;
				_pPattern++;
				break;
			case '+':
				nSkip = MAX_UINT32;
				pSrcText++;
				_pPattern++;
				break;
			default:
				nSkip = 1;
				break;
			}
			// remember next source and pattern position
			pSkipNextSrc = pSrcText;
			pSkipPattern = _pPattern;
			break;
		case '\\':
			_pPattern++;
			switch (*_pPattern) {
			case 's':	//white spaces
				szMatch = (int8*)" \t\r\n";
				break;
			case 'd':	//digits
				szMatch = (int8*)"0123456789";
				break;
			case 'f':	//digits and '.' (float numbers)
				szMatch = (int8*)"0123456789.";
				break;
			case 'w':	//word characters
				szMatch = (int8*)"abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ";
				break;
			case 'a':	//alphanumeric characters
				szMatch = (int8*)" abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ-+0123456789";
				break;
			default:
				chMatch = *_pPattern;
				break;
			}
			_pPattern++;
			break;
		default:
			chMatch = *_pPattern++;
			break;
		}

		//Determine possible modifiers
		switch (*_pPattern) {
		case '*':
			nMatchMin = 0;
			nMatchMax = MAX_UINT32;
			_pPattern++;
			break;
		case '+':
			nMatchMin = 1;
			nMatchMax = MAX_UINT32;
			_pPattern++;
			break;
		case '?':
			nMatchMin = 0;
			nMatchMax = 1;
			_pPattern++;
			break;
		default:
			nMatchMin = 1;
			nMatchMax = 1;
			break;
		}

		//Step over matching character(s)
		for (nMatch = 0; pSrcText < pText + Length;) {
			if (NULL == szMatch) {
				while (nMatch < nMatchMax && chMatch == *pSrcText) {
					nMatch++;
					pSrcText++;
				}
			}
			else {
				while (nMatch < nMatchMax && NULL != strchr(szMatch, (int32)*pSrcText)) {
					nMatch++;
					pSrcText++;
				}
			}
			if (nMatch < nMatchMin) {
				if (nSkip-- > 0) {
					// adjust next skip position for match failed 
					pSrcText = ++pSkipNextSrc;
					_pPattern = pSkipPattern;
					nMatch = 0;
					break;
				}
				return 0 == *_pPattern ? nLMatch : 0;
			}
			break;
		}
		// check for insufficient source characters
		if (pSrcText >= pText + Length && 0 != *_pPattern) return 0;
	}
}

// convert a network IP address along with a port number to a string.
CmString CmString::IP2String(uint32 _IP, uint16 _Port)
{
	// convert 4 subsequent address digits
	CmString IP;
	for (int i = 3; i >= 0; i--) {
		uint8 Digit = (uint8)(_IP >> (i * 8));
		IP += Digit;
		IP += ".";
	}
	IP.setAt((uint32)IP.getLength() - 1, ':');
	IP += _Port;

	return IP;
}

// Search functions
int32 CmString::findPosition(char _ch, int32 _StartPos)
{
	// handle empty text field
	if (NULL == pText) {
		Length = 0;
	}
	else {
		// Search for a given character starting at a determined position
		for (int32 i = _StartPos; i < (int32)Length; i++)
		{
			if (*(pText + i) == _ch)
				return i;
		}
	}

	// Not found
	return (int32)Length + 1;
}

int32 CmString::findBackwards(char _ch, int32 _StartPos)
{
	// Adjust start position
	if (_StartPos == 0)
		_StartPos = (int32)Length;

	// Search backwards for a given character starting at a determined position
	for (int32 i = _StartPos; i >= 0; i--)
	{
		if (*(pText + i) == _ch)
			return i;
	}

	// Not found
	return (int32)Length + 1;
}

// Search pattern
int32 CmString::searchPattern(const char* _Pattern, int32 _StartPos)
{
	// Check search range
	if (_StartPos >= (int32)Length) return (int32)Length + 1; // outside data

	// search for pattern
	char* Found = strstr(pText + _StartPos, _Pattern);
	if (NULL != Found)
		return (int32)(Found - pText);  // pattern found

	// Try binary search
	int32 Pos = _StartPos;
	while (Pos < (int32)Length) {
		Pos = findPosition(*_Pattern, Pos);
		if ((Pos <= (int32)Length) && 0 == strncmp(pText + Pos, _Pattern, strlen(_Pattern)))
			return Pos;  // pattern found
		Pos++;
	}
	return (int32)Length + 1; // not found
}

bool CmString::allItems(CmString& _Item, const char* _Delimiters)
{
	// check start position
	if (HighlightStart >= Length) return false;

	// find next delimiter
	uint32 PosNext = searchPattern(_Delimiters, HighlightStart);

	// Assign next item
	_Item.clear();
	_Item.reset();
	_Item.assignSubString(PosNext - 1, this, HighlightStart);
	HighlightStart = PosNext + 1;

	return true;
}

bool CmString::allItems(CmString& _Item, uint8 _Delimiter)
{
	// check start position
	if (HighlightStart >= Length) return false;

	// find next delimiter
	uint32 PosNext = findPosition(_Delimiter, HighlightStart);

	// Assign next item
	_Item.assignSubString(PosNext - 1, this, HighlightStart);
	HighlightStart = PosNext + 1;

	return true;
}

// Sub strings
int32 CmString::assignSubString(int32 _Last, CmString* _Src, int32 _First)
{
	size_t DstLength;
	size_t OldLength = Length;

	// Check for source string
	if (_Src == NULL)
		_Src = this;

	// Determine actual target length
	if ((int32)(_Src->getLength()) <= _First)
		DstLength = 0;
	else
		if ((int32)(_Src->getLength()) <= _Last)
			DstLength = _Src->getLength() - _First;
		else
			if (_Last < _First)
				DstLength = 0;
			else
				DstLength = _Last - _First + 1;

	// Check if the target string is empty
	if (DstLength == 0)
		setLength(0);
	else
		// Check if the substring is assigned to current string object
		if (_Src == this)
		{
			// move current buffer away
			int8* pOldText = pText;

			// Allocate new buffer
			Length = DstLength;
			pText = allocateMemory<int8>(int32(Length + 1), isInt8);

			// Transfer substring
			memcpy(pText, pOldText + _First, Length);
			*(pText + Length) = 0;

			// Delete previous string
			releaseMemory<int8>(pOldText, int32(OldLength + 1), isInt8);
		}
		else
		{
			// Adjust current string length
			setLength(DstLength);

			// Transfer content
			_Src->pText != NULL ? memcpy(pText, _Src->pText + _First, DstLength) : 0;
		}

	return (int32)DstLength;
}

CmString& CmString::purgeTrailingSpace()
{
	for (int i = int32(Length) - 1; i >= 0; i--) {
		if (pText[i] != ' ') break;
		pText[i] = 0;
	}

	return *this;
}

bool CmString::setActive(uint32 _ActiveStart, uint32 _ActiveLength, int32 _LineNumber)
{
	// assigne a new active range and line number (if valid)
	ActiveStart = _ActiveStart;
	ActiveLength = _ActiveLength;
	if (_LineNumber >= 0) {
		LineNumber = _LineNumber;
		// a valid line number also causes LineStart be initialized with ActiveStart
		LineStart = ActiveStart;
	}

	return true;
}
bool CmString::setHighlight(uint32 _HighlightStart, uint32 _HighlightLength)
{
	// assigne a new highlight range
	HighlightStart = _HighlightStart;
	HighlightLength = _HighlightLength;

	return true;
}
bool CmString::getActive(uint32& _ActiveStart, uint32& _ActiveLength, int32& _LineNumber)
{
	// return active range and line number
	_ActiveStart = ActiveStart;
	_ActiveLength = ActiveLength;
	_LineNumber = LineNumber;

	return true;
}
bool CmString::getHighlight(uint32& _HighlightStart, uint32& _HighlightLength)
{
	// return highlight range
	_HighlightStart = HighlightStart;
	_HighlightLength = HighlightLength;

	return true;
}
bool CmString::getLineHighlight(int32& _LineNumber, uint32& _LineStart, uint32& _HighlightStart, uint32& _HighlightLength)
{
	// return line and highlight information
	_LineNumber = LineNumber;
	_LineStart = LineStart;
	_HighlightStart = HighlightStart;
	_HighlightLength = HighlightLength;

	return true;
}
bool CmString::showActiveHighlight()
{
	int8 Active[256] = { 0 };
	int8 Highlight[256] = { 0 };
	uint32 Length_l = ActiveLength;

	if (Length_l > sizeof(Active)) {
		Length_l = sizeof(Active);
	}
	memcpy(Active, getText() + ActiveStart, Length_l);

	Length_l = HighlightLength;
	if (Length_l > sizeof(Highlight)) {
		Length_l = sizeof(Highlight);
	}
	memcpy(Highlight, getText() + HighlightStart, Length_l);

	return true;
}

// Conversion function
double CmString::getNumAsDouble()
{
	const double MaxDouble = 1e15;
	const double Error = 0;
	double Value = 0;
	double Fraction = 0;
	double FractionLength = 0;
	double Exponent = 0;
	double ValSign = 1.0;
	double ExpoSign = 1.0;
	double* Part = &Value;
	double* Sign = &ValSign;
	bool isFinished = false;
	bool isUnexpected = false;

	// preset conversion error to true
	isConversionError = true;

	for (int32 i = 0; i < (int32)(getLength()); i++) {
		// check against overflow conditions
		if (Value > MaxDouble) return Error;
		if (Fraction > MaxDouble) return Error;
		if (Exponent > MaxDouble) return Error;
		// assemble values
		switch (*(pText + i)) {
			// decimal digits
		case 0: isFinished = true; break;
		case '0': *Part = *Part * 10 + 0; break;
		case '1': *Part = *Part * 10 + 1; break;
		case '2': *Part = *Part * 10 + 2; break;
		case '3': *Part = *Part * 10 + 3; break;
		case '4': *Part = *Part * 10 + 4; break;
		case '5': *Part = *Part * 10 + 5; break;
		case '6': *Part = *Part * 10 + 6; break;
		case '7': *Part = *Part * 10 + 7; break;
		case '8': *Part = *Part * 10 + 8; break;
		case '9': *Part = *Part * 10 + 9; break;
		case ' ':
			if (0 == *Part) {
				continue; // skip spaces at start
			}
			else {
				isFinished = true;
				break; // space ends conversion properly
			}

			// apply sign
		case '+': break;
		case '-':
			if (*Sign != 1.0) return Error;
			if (Part == &Fraction) return Error;
			if (Part == &Value && i > 0) return Error;
			if (Part == &Exponent && Exponent > 0) return Error;
			*Sign = -*Sign;
			break;
			// start fraction
		case '.':
		case ',': Part = &Fraction; break;
			// start exponent
		case 'e':
		case 'E': Part = &Exponent; Sign = &ExpoSign; break;
			// reject/handle all others
		default:
			if (0 == *Part) {
				isUnexpected = true;
				break; // invalid digit at start 
			}
			else {
				isUnexpected = true;
				isFinished = true;
				break; // invalid digit forces end of conversion
			}
		}
		if (isFinished) break;
		if (Part == &Fraction) FractionLength++;
	}

	// add fraction
	if (FractionLength != 0) {
		Value = ValSign * (Value + Fraction * pow(10.0, -(FractionLength - 1)));
	}
	else {
		Value = ValSign * Value;
	}
	// add exponent
	if (Exponent != 0) {
		Value *= pow(10.0, ExpoSign * Exponent);
	}

	// success: clear conversion error, unexpected: is conversion error 
	isConversionError = isUnexpected ? true : false;

	return Value;
}

double CmString::getNumAsDouble(int32 _PosStart, int32 _PosEnd)
{
	CmString Num;
	Num.assignSubString(_PosEnd, this, _PosStart);
	return Num.getNumAsDouble();
}

uint64 CmString::getNumAsUint64()
{
	const uint64 Error = 0;
	uint64 uNum = 0;
	int Significance = 10;

	// preset conversion error to true
	isConversionError = true;

	for (int32 i = 0; i < (int32)(getLength()); i++) {
		// test end of string
		if (0 == *(pText + i)) return uNum;
		// process next digit
		uNum *= Significance;
		switch (*(pText + i)) {
			// decimal digits
		case '0': uNum += 0; break;
		case '1': uNum += 1; break;
		case '2': uNum += 2; break;
		case '3': uNum += 3; break;
		case '4': uNum += 4; break;
		case '5': uNum += 5; break;
		case '6': uNum += 6; break;
		case '7': uNum += 7; break;
		case '8': uNum += 8; break;
		case '9': uNum += 9; break;
		case ' ':
			if (0 == uNum)
				continue;	// skip leading spaces
			else {
				isConversionError = false;
				return uNum / Significance; // space ends conversion properly
			}

			// start a hex number
		case 'x':
		case 'X':
			if (uNum != 0) return Error;
			Significance = 16;
			break;
			// hex digits
		default:
			if (Significance != 16) {
				if (0 == uNum)
					return Error;	// invalid digit before any value
				else
					return uNum / Significance; // invalid digit forces end of conversion
			}
			switch (*(pText + i)) {
				// hex
			case 'a':
			case 'A':	uNum += 10; break;
			case 'b':
			case 'B': uNum += 11; break;
			case 'c':
			case 'C': uNum += 12; break;
			case 'd':
			case 'D': uNum += 13; break;
			case 'e':
			case 'E': uNum += 14; break;
			case 'f':
			case 'F': uNum += 15; break;
				// reject/handle others
			default:
				if (0 == uNum) {
					return Error; // invalid digit before any value
				}
				else {
					return uNum / Significance; // invalid char forces end of conversion
				}
			}
		}
	}

	// success: clear conversion error
	isConversionError = false;

	return uNum;
}

uint64 CmString::getNumAsUint64(int32 _PosStart, int32 _PosEnd)
{
	CmString Num;
	Num.assignSubString(_PosEnd, this, _PosStart);
	return Num.getNumAsUint64();
}

int32 CmString::getNumAsInt32()
{
	return int32(getNumAsDouble());
}

int32 CmString::getNumAsInt32(int32 _PosStart, int32 _PosEnd)
{
	if (NULL == pText) return 0;
	CmString Num;
	int32 Sign = pText[_PosStart] == '-' ? -1 : 1;
	Num.assignSubString(_PosEnd, this, pText[_PosStart] == '-' ? _PosStart + 1 : _PosStart);
	return Sign * int32(Num.getNumAsUint64());
}

int32 CmString::getMonthAsInt32()
{
	return operator==("Jan") ? 1 : operator==("Feb") ? 2 : operator==("Mar") ? 3 : operator==("Apr") ? 4 : operator==("May") ? 5 : operator==("Jun") ? 6 : operator==("Jul") ? 7 : operator==("Aug") ? 8 : operator==("Sep") ? 9 : operator==("Oct") ? 10 : operator==("Nov") ? 11 : operator==("Dec") ? 12 : 0;
}

int32 CmString::getMonthAsInt32(int32 _PosStart, int32 _PosEnd)
{
	CmString Month;
	Month.assignSubString(_PosEnd, this, _PosStart);
	return Month.getMonthAsInt32();
}

// Binary backup and restore
bool CmString::readBinary(CmString& _Filename)
{
	try {
		readBinary(_Filename.getText());
	}
	catch (...) {
		return false;
	}
	return true;
}
const int8* CmString::readBinary(const int8* _Filename)
{
	FILE* pFile;
	struct  stat buf;
	int32   fh;
	uint32  uFileSize = 0;

	// check pointer
	if (NULL == _Filename) return pText;

	//Determine file size
#ifdef MSVS
	_sopen_s(&fh, _Filename, O_RDONLY, _SH_DENYWR, 0);
	if (-1 != fh) {
		if (0 == fstat(fh, &buf)) {
			uFileSize = buf.st_size;
		}
		else {
			//throw CmException("File status not accessible.");
		}
		close(fh);
	}
	else {
		//throw CmException("Data file not found.");
	}
#else
	if (-1 != (fh = open(_Filename, O_RDONLY))) {
		if (0 == fstat(fh, &buf)) {
			uFileSize = buf.st_size;
		}
		else {
			//throw CmException("File status not accessible.");
		}
		close(fh);
	}
	else {
		//throw CmException("Data file not found.");
	}
#endif

#ifdef MSVS
	fopen_s(&pFile, _Filename, "rb");
#else
	pFile = fopen(_Filename, "rb");
#endif
	if (NULL == pFile) {
		//throw CmException("Data file can not be opened.");
	}
	else {
		try {
			setLength(uFileSize);
			fread(pText, 1, uFileSize, pFile);
			fclose(pFile);
		}
		catch (...) {
			fclose(pFile);
			//throw CmException("Data file not readable.");
		}
	}
	return pText;
}

bool CmString::writeBinary(const int8* _Filename)
{
	FILE* pFile;

	// check pointer
	if (NULL == _Filename) return false;

#ifdef MSVS
	fopen_s(&pFile, _Filename, "wb");
#else
	pFile = fopen(_Filename, "wb");
#endif

	// check if file could be opened
	if (NULL == pFile) return false;

	// write file to disk
	fwrite(pText, Length, 1, pFile);
	fclose(pFile);

	return true;
}

bool CmString::appendBinary(const int8* _Filename)
{
	FILE* pFile;

	// check pointer
	if (NULL == _Filename) return false;

#ifdef MSVS
	fopen_s(&pFile, _Filename, "ab");
#else
	pFile = fopen(_Filename, "ab");
#endif

	// check if file could be opened
	if (NULL == pFile) return false;

	// write file to disk
	fwrite(pText, Length, 1, pFile);
	fclose(pFile);

	return true;
}

bool CmString::isFileExists(const int8* _Filename)
{
	const int8* Filename = (_Filename != NULL ? _Filename : pText);
	if (NULL == Filename) return false;

	// determine whether file exists
	int32   fh;
#ifdef MSVS
	_sopen_s(&fh, Filename, O_RDONLY, _SH_DENYWR, 0);
	if (-1 != fh) {
		close(fh);
	}
	else {
		return false;
	}
#else
	if (-1 == (fh = open(Filename, O_RDONLY))) {
		close(fh);
	}
	else {
		return false;
	}
#endif

	return true;
}
bool CmString::addFilesOnPath(CmLString& _Files, const int8* _Path) {
	// check input parameters
	const int8* Path = (_Path != NULL ? _Path : pText);
	if (NULL == Path) return false;

	// validate folder path
	CmString File;
	CmString Search(Path);
	Search += "*";
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFileA(Search.getText(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return false;
	// get all filenames on path
	for (;;) {
		// check if item is a file
		if (false == (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			// add found file to result list
			File = getText();   // path
			File += FindFileData.cFileName;
			_Files.addString(File);
		}
		// find next item
		if (false == FindNextFileA(hFind, &FindFileData)) break;
	}
	FindClose(hFind);

	if (_Files.getSize() == 0) return false;

	return true;
}

bool CmString::allFiles(CmString& _File, const CmString& _Path, const int8* _Pattern, bool _isLoadContent)
{
	CmString Path(_Path);
	if (Path[Path.getLength() - 1] != '\\') Path += "\\";
	Path += "*";
	WIN32_FIND_DATAA FindFileData;

	// check for start of file search
	if (INVALID_HANDLE_VALUE == hFile) {
		// get first file
		hFile = FindFirstFileA(Path.getText(), &FindFileData);
		if (hFile == INVALID_HANDLE_VALUE) return false;
	}
	else {
		// find next file
		if (false == FindNextFileA(hFile, &FindFileData)) {
			FindClose(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return false;
		}
	}
	// evaluate filename
	CmString Filename(FindFileData.cFileName);
	CmString Match;
	for (;;) {
		// skip entry if not a file
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || Filename == "." || Filename == "..") {
			if (false == FindNextFileA(hFile, &FindFileData)) {
				FindClose(hFile);
				hFile = INVALID_HANDLE_VALUE;
				return false;
			}
			Filename = FindFileData.cFileName;
			continue;
		}
		// check filename against pattern
		Filename = FindFileData.cFileName;
		if (Filename.matchPattern(Match, _Pattern)) break;
		if (false == FindNextFileA(hFile, &FindFileData)) {
			FindClose(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return false;
		}
	}
	// complete path and filename
	Filename = _Path;
	Filename += "\\";
	Filename += FindFileData.cFileName;
	// check if content is to be loaded
	if (_isLoadContent) {
		// return file content
		try {
			_File.readBinary(Filename.getText());
		}
		catch (...) {
			return false;
		}
	}
	else {
		// return just the filename
		_File = Filename;
	}

	return true;
}

bool CmString::allLines(CmString& _Line, uint32 _Offset)
{
	// check start-of-line
	if (LineStart >= Length) return false;

	// find end-of-line
	uint32 LineEnd = findPosition('\x0A', LineStart);
	// evaluate offset
	_Line.clear();
	if (_Offset < LineEnd) {
		// copy line
		_Line.reset();
		_Line.assignSubString(LineEnd, this, LineStart + _Offset);
	}
	// step one line forward
	LineStart = LineEnd + 1;

	return true;
}
bool CmString::isFolderExists(const int8* _FolderPath)
{
	const int8* FolderPath = (_FolderPath != NULL ? _FolderPath : pText);
	if (NULL == FolderPath) return false;

	// strip trailing backslash
	CmString Path(FolderPath);
	if (Path[Path.getLength() - 1] == '\\') Path.setAt(Path.getLength() - 1, uint8(0));

	/* workaround:
	*  Folder exists if it contains at least one file.
	*  ToDo: handle empty case
	*        PathFileExistsA(..) ?
	*/
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFileA(Path.getText(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return false;
	// verify folder
	bool isFolder = FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? true : false;
	FindClose(hFind);
	if (false == isFolder) return false;

	return true;
}
bool CmString::getFoldersOnPath(CmLString& _Folders, const int8* _Path) {
	// check input parameters
	const int8* Path = (_Path != NULL ? _Path : pText);
	if (NULL == Path) return false;

	// initialize returned list of folders
	_Folders.clearList();

	// validate path
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFileA(Path, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return false;
	// get all folders on path
	for (;;) {
		// check if item is a folder
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// add found folder to result list
			CmString Folder(FindFileData.cFileName);
			if (Folder != "." && Folder != "..") {
				_Folders.addString(Folder);
			}
		}
		// find next item
		if (false == FindNextFileA(hFind, &FindFileData)) break;
	}
	FindClose(hFind);

	if (_Folders.getSize() == 0) return false;

	return true;
}
bool CmString::allFolders(CmString& _Folder, const CmString& _Path)
{
	CmString Path(_Path);
	Path += "\\*";
	WIN32_FIND_DATAA FindFolderData;

	// check for start of file search
	if (INVALID_HANDLE_VALUE == hFolder) {
		// get first file
		hFolder = FindFirstFileA(Path.getText(), &FindFolderData);
		if (hFolder == INVALID_HANDLE_VALUE) return false;
	}
	else {
		// find next file
		if (false == FindNextFileA(hFolder, &FindFolderData)) {
			FindClose(hFolder);
			hFolder = INVALID_HANDLE_VALUE;
			return false;
		}
	}
	// evaluate filename
	CmString Foldername(FindFolderData.cFileName);
	for (;;) {
		// skip entry if not a file
		if (0 == (FindFolderData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || Foldername == "." || Foldername == ".." || Foldername == "#recycle") {
			if (false == FindNextFileA(hFolder, &FindFolderData)) {
				FindClose(hFolder);
				hFolder = INVALID_HANDLE_VALUE;
				return false;
			}
			Foldername = FindFolderData.cFileName;
			continue;
		}
		break;
	}
	// return folder
	_Folder = FindFolderData.cFileName;

	return true;
}
bool CmString::getParentFolder(CmString& _ParentFolder, const CmString* _Path)
{
	// get folder
	CmString Folder = NULL != _Path ? *_Path : *this;
	if (false == Folder.getFolder(_ParentFolder)) return false;

	// find delimiter before last folder
	int32 PosDelimiter = _ParentFolder.findBackwards('\\', _ParentFolder.getLength() - 2);
	if (PosDelimiter <= 0) return false;

	// strip last folder
	_ParentFolder.assignSubString(PosDelimiter);

	// check if folder exists
	if (false == isFolderExists(_ParentFolder.getText())) return false;

	return true;
}
bool CmString::getFolder(CmString& _Folder, const CmString* _Path)
{
	// check if path points to a folder
	_Folder = NULL != _Path ? *_Path : *this;
	if (isFolderExists(_Folder.getText())) return true;

	// find last delimiter
	int32 PosDelimiter = _Folder.findBackwards('\\');
	if (PosDelimiter <= 0) return false;

	// strip filename
	_Folder.assignSubString(PosDelimiter);

	// check if folder exists
	if (false == isFolderExists(_Folder.getText())) return false;

	return true;
}

bool CmString::clearMemoryState()
{
	ItemsAllocated = ItemsOfTypeAllocated = ItemsReleased = ItemsOfTypeReleased = MemoryAllocated = MemoryReleased = 0;

	return true;
}

// memory state report
static CmString MemoryState("");

CmString& CmString::getMemoryState(bool _isClearType)
{
	// item state
	int64 ItemsUsed = ItemsAllocated - ItemsReleased;
	int64 ItemsDif = ItemsUsed - ItemsOccupied;
	int64 ItemsOfTypeUsed = ItemsOfTypeAllocated - ItemsOfTypeReleased - (isInt8 ? MemoryState.getLength() : 0);
	ItemsOccupied = ItemsUsed;
	bool isItemsOfType = (ItemsOfTypeAllocated != 0) || (ItemsOfTypeReleased != 0);
	// memory state
	int64 MemoryUsed = MemoryAllocated - MemoryReleased - MemoryState.getLength();
	int64 MemoryDif = MemoryUsed - MemoryOccupied;
	MemoryOccupied = MemoryUsed;

	CmString ItemCount;
	CmString MemorySize;
	CmString ItemsChanged;
	CmString ItemsOfTypeCount;
	CmString MemoryChange;

	// generate a memory report message
	ItemCount.double2String(ItemsUsed / 1000.0, 3);
	ItemsChanged.double2String(double(ItemsDif), 0, 6);
	MemoryState = "Memory items= ";
	MemoryState += ItemCount;
	MemoryState += "k (dif:";
	MemoryState += ItemsChanged;
	MemoryState += ") ";
	if (isItemsOfType) {
		ItemsOfTypeCount.double2String(double(ItemsOfTypeUsed));
		MemoryState += "{";
		if (isFocus) MemoryState += "focus ";
		if (isInt8) MemoryState += "int8 ";
		if (isUint8) MemoryState += "uint8 ";
		if (isInt16) MemoryState += "int16 ";
		if (isUint16) MemoryState += "uint16 ";
		if (isDouble) MemoryState += "double ";
		if (isNaviFTL) MemoryState += "NaviFTL ";
		if (isCmString) MemoryState += "CmString ";
		if (isCmLString) MemoryState += "CmLString ";
		if (isCmStringFTL) MemoryState += "CmStringFTL ";
		if (isCmStringFTLChild) MemoryState += "CmStringFTLChild ";
		if (isCmServiceConnection) MemoryState += "CmServiceConnection ";
		if (isCmException) MemoryState += "CmException ";
		if (isCmMatrixFTL) MemoryState += "CmMatrixFTL ";
		if (isCmMatrix) MemoryState += "CmMatrix ";
		if (isCmVector) MemoryState += "CmVector<T> ";
		if (isCmParallel) MemoryState += "CmParallel ";
		MemoryState += "= ";
		MemoryState += ItemsOfTypeCount;
		MemoryState += "}";
	}
	else {
		CmString NotUsed;
		if (isFocus) NotUsed += "focus ";
		if (isInt8) NotUsed += "int8 ";
		if (isUint8) NotUsed += "uint8 ";
		if (isInt16) NotUsed += "int16 ";
		if (isUint16) NotUsed += "uint16 ";
		if (isDouble) NotUsed += "double ";
		if (isNaviFTL) NotUsed += "NaviFTL ";
		if (isCmString) NotUsed += "CmString ";
		if (isCmLString) NotUsed += "CmLString ";
		if (isCmStringFTL) NotUsed += "CmStringFTL ";
		if (isCmStringFTLChild) NotUsed += "CmStringFTLChild ";
		if (isCmServiceConnection) NotUsed += "CmServiceConnection ";
		if (isCmException) NotUsed += "CmException ";
		if (isCmMatrixFTL) NotUsed += "CmMatrixFTL ";
		if (isCmMatrix) NotUsed += "CmMatrix ";
		if (isCmVector) NotUsed += "CmVector<T> ";
		if (isCmParallel) NotUsed += "CmParallel ";
		if (NotUsed.getLength() > 0) {
			MemoryState += "{";
			MemoryState += NotUsed;
			MemoryState += "not used}";
		}
	}
	const bool isMegaBytes = MemoryUsed > 10000000; // > 10M
	MemorySize.double2String((MemoryUsed) / (isMegaBytes ? 1000000.0 : 1000.0), 1);
	MemoryChange.double2String(double(MemoryDif), 0, 6);
	MemoryState += "  bytes= ";
	MemoryState += MemorySize;
	MemoryState += isMegaBytes ? "M (dif:" : "k (dif:";
	MemoryState += MemoryChange;
	MemoryState += ")";

	// restore state
	ItemCount.clear();
	ItemsChanged.clear();
	MemorySize.clear();
	MemoryChange.clear();
	ItemsOfTypeCount.clear();

	// clear items of type
	if (_isClearType) {
		ItemsOfTypeAllocated = 0;
		ItemsOfTypeReleased = 0;
	}

	return MemoryState;
}

//----------------------------------------------------------------------------
// CmPath
//----------------------------------------------------------------------------
CmPath::CmPath()
{

}
CmPath::CmPath(CmString& _Head, CmString& _Tail)
{
	// take care for a backslash between path components
	setText(_Head);
	if (operator[](int32(Length) - 1) != '\\') operator+=('\\');
	operator+=(_Tail);
}
CmPath::~CmPath()
{

}

//----------------------------------------------------------------------------
// CmLString
//----------------------------------------------------------------------------
CmLString::CmLString()
{
	pNext = NULL;
}
CmLString::~CmLString()
{
	// remove all subsequent elements
	clearList();
}

//-----access-functions-------------------------------------------------------

bool CmLString::addString(const CmString& _String)
{
	// assign string to root element if it is still empty
	if (0 == Length) {
		setText(_String);
		return true;
	}

	// go to last element
	CmLString* pLString = this;
	while (NULL != pLString->pNext) {
		pLString = pLString->pNext;
	}

	// extend list by a new element
	pLString->pNext = allocateMemory<CmLString>(1, isCmLString);
	pLString->pNext->setText(_String);

	return true;
}
bool CmLString::clearList()
{
	// clear next element
	if (NULL != pNext) {
		pNext->~CmLString();
		// free memory
		releaseMemory<CmLString>(pNext, 1, isCmLString);
	}

	// clear content of this string
	clear();

	return true;
}
int32 CmLString::getSize()
{
	int32 Size = Length == 0 ? 0 : 1;

	// count list elements
	CmLString* pString = this->pNext;
	while (NULL != pString) {
		Size++;
		pString = pString->pNext;
	}

	return Size;
}
CmString& CmLString::operator[](int32 _Index)
{
	// iterate list upto index, return last element if the index is too high
	CmLString* pString = this;
	while (_Index-- > 0 && NULL != pString) {
		pString = pString->pNext;
	}

	return *pString;
}
CmString& CmLString::getLowestString()
{
	// return single item
	if (1 == getSize()) return *this;

	int32 IndexLowestString = 0;
	for (int i = 1; i < getSize(); i++) {
		// compare string lengths
		if (this->operator[](i).getLength() < this->operator[](IndexLowestString).getLength()) {
			IndexLowestString = i;
		}
		else if (this->operator[](i).getLength() == this->operator[](IndexLowestString).getLength()) {
			if (strncmp(this->operator[](i).getText(), this->operator[](IndexLowestString).getText(), this->operator[](i).getLength()) < 0) {
				IndexLowestString = i;
			}
		}
	}
	return this->operator[](IndexLowestString);
}
bool CmLString::getMatchingString(CmString& _MatchingString, const CmString& _Pattern)
{
	// clear result
	_MatchingString.clear();

	// match all strings against pattern
	for (int i = 0; i < getSize(); i++) {
		if (this->operator[](i).searchPattern(_Pattern.getText()) < (int32)(this->operator[](i).getLength())) {
			// return matching string
			_MatchingString = this->operator[](i);
			break;
		}
	}
	// check result
	if (_MatchingString.getLength() == 0) return false;

	return true;
}

//-----functions-for-maintaining-a-list-of-strings----------------------------

CmLString* CmLString::getNext()
{
	return pNext;
}
CmString* CmLString::setNext(CmLString* _Next)
{
	return(pNext = _Next);
}
CmLString** CmLString::getAdrNext()
{
	return &pNext;
}



//----------------------------------------------------------------------------
// CmMString
//----------------------------------------------------------------------------
CmMString::CmMString(const CmString* _SourceString, const int8* _Delimiters)
{
	if (NULL == _Delimiters)
		_Delimiters = CM_PATH_DELIMITERS;

	CurrentPosition = 0;
	mSetOfDelimiters.setText(_Delimiters);
	pSourceString = _SourceString;
}

CmMString::CmMString()
{

}

CmMString::~CmMString()
{
}

void CmMString::resetPosition()
{
	CurrentPosition = 0;
}

int8* CmMString::getNextDelimitedString()
{
	int32 SubStringLength = 0;
	bool   fDelimiterFound = false;

	//Current position must not be greater than the string length
	if (NULL == pSourceString || CurrentPosition >= (int32)(pSourceString->Length)) {
		return NULL;
	}
	for (; CurrentPosition < (int32)((*pSourceString).Length); CurrentPosition++) {
		for (int32 k = 0; k < (int32)(mSetOfDelimiters.getLength()); k++) {
			if ((uint32) * (pSourceString->getText() + CurrentPosition) == mSetOfDelimiters[k]) {
				fDelimiterFound = true;
				break;
			}
		}
		if (true == fDelimiterFound) {
			if (SubStringLength > 0) {
				break;
			}
			fDelimiterFound = false;
		}
		else {
			SubStringLength++;
		}
	}
	//Transfer substring
	setLength(SubStringLength);
	for (int32 m = 0; m < SubStringLength; m++) {
		setAt(m, (uint8) * (pSourceString->getText() + CurrentPosition - SubStringLength + m));
	}

	return pText;
}

int8* CmMString::setDelimiters(const int8* _Delimiters)
{
	return mSetOfDelimiters.setText(_Delimiters);
}

const CmString* CmMString::setSourceString(const CmString* _SourceString)
{
	return pSourceString = _SourceString;
}




