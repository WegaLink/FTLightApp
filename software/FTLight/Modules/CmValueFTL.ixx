//////////////////////////////////////////////////////////////////////////////
//
// CmValueFTL.ixx - Module for CmValueFTL classes
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
#include "Shlwapi.h"

export module CmFTLight:CmValueFTL;

export import :CmStringFTL;

export import CmBasic;
//export import CmNetwork;


export namespace Cosmos
{
	// forward declaration
	class CmMatrix;
	class CmValueINI;

	//------------------------------------------------------------------------------
	// CmMatrixFTL class
	//------------------------------------------------------------------------------
	//
	/** CmMatrixFTL.
	*  This is a polydimensional sparse matrix implementation. It allows for a
	*  dynamic operation of independent dimensional levels. A type information is
	*  maintained in addition to each of the stored values. A matrix element can
	*  be another CmMatrixFTL which provides to the next level  of the dimensional
	*  hierarchy inside the matrix.
	*/
	class CmMatrixFTL : public CmVector<uint64>
	{
	public:
		CmMatrixFTL();
		virtual ~CmMatrixFTL();

	public:
		/** unit test for CmMatrixFTL including CmVector */
		bool testCmMatrixFTL();

	public:
		/** clearMatrix.
		*  All matrix elements will be removed starting from root respectively from
		*  a specified matrix level. The matrix/sub-matrix will become empty.
		*/
		bool clearMatrix();
		bool clearMatrix(CmIndex& _Index);

	public:
		/** getVectorLength.
		*  An index may point to a vector inside the matrix same as the matrix itself
		*  represents a root vector. The function returns the length of that vector.
		*  The return value will be zero whenever there is no item connected at the
		*  indexed position. It will as well be zero when the connected item is not
		*  a vector but any data item like a string, a double or another value.
		*/
		int32 getVectorLength(CmIndex& _Index);

	protected:
		/** set/getScalar.
		*  A scalar value can be set/get to/from a matrix as well as information
		*  about the scalar's format.
		*/
		bool setScalar(uint64 _Scalar, CmDataFormat _ScalarFormat);
		bool getScalar(uint64& _Scalar, CmDataFormat& _ScalarFormat);

	public:
		/** operator[].
		*  This default access function returns the indexed value as a double in case
		*  it was stored as a numeric value and if the last position in the Index has
		*  been reached. Otherwise it will return 0 if the indexed item is an address
		*  or a string or if it was not assigned.
		*/
		double operator[](CmIndex& _Index);
		double operator[](int32 _Index);

	public:
		/** setValue/String.
		*   The binary layout of a data value will be written to a 64-bit field.
		*   Memory will be allocated in case of a string to store the string.
		*   Subsequently, the string's address will be written to the 64-bit data
		*   field. Also, the data format of the data value will be stored.
		*/
		bool setValue(bool _ValBool, CmIndex& _Index);
		bool setValue(int8 _Val8, CmIndex& _Index);
		bool setValue(uint8 _Val8, CmIndex& _Index);
		bool setValue(int16 _Val16, CmIndex& _Index);
		bool setValue(uint16 _Val16, CmIndex& _Index);
		bool setValue(int32 _Val32, CmIndex& _Index);
		bool setValue(uint32 _Val32, CmIndex& _Index);
		bool setValue(float _ValFloat, CmIndex& _Index);
		bool setValue(int64 _Val64, CmIndex& _Index);
		bool setValue(uint64 _Val64, CmIndex& _Index);
		bool setValue(double _ValDouble, CmIndex& _Index);
		bool setValue(void* _ValPointer, CmIndex& _Index); // NOTE: void* is valid only below max double
		bool setString(const CmString& _String, CmIndex& _Index);

	protected:
		/** as<data format>...
		*   These are several conversion functions for particular data formats
		*/
		CmString asString(CmIndex& _Index);
		uint64 asUInt64(CmIndex& _Index);
		int64 asInt64(CmIndex& _Index);
		double asDouble(CmIndex& _Index);
		uint32 asUInt32(CmIndex& _Index);
		int32 asInt32(CmIndex& _Index);
		float asFloat(CmIndex& _Index);
		uint16 asUInt16(CmIndex& _Index);
		int16 asInt16(CmIndex& _Index);
		uint8 asUInt8(CmIndex& _Index);
		int8 asInt8(CmIndex& _Index);
		bool asBool(CmIndex& _Index);

	protected:
		/** getBeforeLastLevel.
		*  The last level of dimension that holds a value will be retrieved. If 
		*  intermediate levels of dimension are still missing then they will be created
		*  if so requested.
		*/
		CmMatrixFTL* getBeforeLastLevel(CmIndex& _Index, bool _isCreateNextLevel = true);

		/** getMatrix. A matrix will be returned if available at selected position.
		*/
		CmMatrixFTL* getMatrixLastLevel(CmIndex& _Index);

		/** set/getValue.
		*  A uint64 value will be set to specified item position in the vector. If the
		*  position is occupied by a CmMatrixFTL object then the scalar value of that
		*  object will be set/get instead. Further, format information will be assigned
		*  and retrieved.
		*/
		bool setValue(uint64 _Value, CmDataFormat _Format, CmIndex& _Index);
		bool getValue(uint64& _Value, CmDataFormat& _Format, CmIndex& _Index);

		/** resetNextIndex/allItems/allValues.
		*   The reset function sets the travel index to start at position 0.
		*	The addressed level will be traveled one by one with each function call.
		*   If a format match was requested and the format does not match then the 
		*   result will be empty or zero.
		*/
		bool resetNextIndex(CmIndex& _Index);
		bool allItems(CmString& _Item, int32& _Position, CmIndex& _Index, bool _isToDouble);
		bool allValues(CmFlex& _Value, int32& _Position, CmIndex& _Index, CmDataFormat _Format, bool _isMatchFormat);

		friend CmMatrix;

		//--------workspace-----------------------------------------------------------

	protected:
		// format information
		CmVector<CmDataFormat> DataFormat;
	};

	/** CmMatrix.
	*   This class extends the CmMatrixFTL class by an index variable. The purpose
	*   is to save index information inside an overloaded 'operator[]' in order to
	*   use that index in overloaded 'operator <type>' and 'operator==' functions.
	*/
	class CmMatrix : public CmMatrixFTL
	{
	public:
		CmMatrix();
		virtual ~CmMatrix();

	public:
		/** clearMatrix.
		*  All matrix elements will be removed starting from root respectively from
		*  a specified matrix level. The matrix/sub-matrix will become empty.
		*/
		bool clearMatrix();

	public:
		/** addressing a matrix element by an index */
		CmMatrix& operator[](CmIndex& _Index);
		CmMatrix& operator()();
		CmMatrix& operator()(int _a);
		CmMatrix& operator()(int _a, int _b);
		CmMatrix& operator()(int _a, int _b, int _c);
		CmMatrix& operator()(int _a, int _b, int _c, int _d);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v, int _w);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v, int _w, int _x);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v, int _w, int _x, int _y);
		CmMatrix& operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v, int _w, int _x, int _y, int _z);

	public:
		/** matrix write access */
		bool operator=(const CmString& _String);
		bool operator=(const int8* _String);
		bool operator=(void* _Value); // NOTE: void* is valid only below max double
		bool operator=(uint64 _Value);
		bool operator=(int64 _Value);
		bool operator=(double _Value);
		bool operator=(uint32 _Value);
		bool operator=(int32 _Value);
		bool operator=(float _Value);
		bool operator=(uint16 _Value);
		bool operator=(int16 _Value);
		bool operator=(uint8 _Value);
		bool operator=(int8 _Value);
		bool operator=(bool _Value);

	public:
		/** matrix read access and subsequent type conversion */
		operator CmString();
		operator const int8* ();
		operator void* (); // NOTE: void* is valid only below max double
		operator uint64();
		operator int64();
		operator double();
		operator uint32();
		operator int32();
		operator float();
		operator uint16();
		operator int16();
		operator uint8();
		operator int8();
		operator bool();

	public:
		/** getSizeLast/VariableIndexLevel. */
		int32 getSizeLastIndexLevel();
		int32 getSizeVariableIndexLevel(CmIndex& _Index);

	public:
		/** addValue. A matrix level is searched for a given int32/int64 value. If
		*   it was not found then it will be added and true is returned. The found
		*   respectively added new position will be returned. If sorted was
		*   requested then a new value will be inserted at appropriate position.
		*/
		bool addValue(int32& _Position, int32 _Value);
		bool addValue(int32& _Position, int64 _Value);
		bool addValueSorted(int32& _Position, int32 _Value);
		bool addValueSorted(int32& _Position, int64 _Value);

	public:
		/** appendItem. A matrix level is searched for a given text item. If it
		*   was not found then it will be appended at the end. The return code
		*   will be true if the item was appended and otherwise it will be false.
		*   The item's position on last index level will be reported back if needed.
		*/
		bool appendItem(const int8* _Item, bool _isForceAppend = true);
		bool appendItem(const CmString& _Item, bool _isForceAppend = true);
		bool appendItem(int32& _Position, const int8* _Item, bool _isForceAppend = true);
		bool appendItem(int32& _Position, const CmString& _Item, bool _isForceAppend = true);

	public:
		/** getItemPos.
		* A matrix level is searched for a given text item on addressed level. If
		* it was not found then a new node will be appended at the end of that
		* level. The item's index on that level will be returned.
		*/
		int32 getItemPos(CmIndex& _Index, const CmString& _Item);

	public:
		/** findItem.
		* A matrix level is searched for a given text item on scalar position. If
		* it was not found then a new node will be appended at the end. The matrix
		* that holds the Item at scalar position will be returned.
		*/
		CmMatrix& findScalarItem(const CmString& _Item);

	public:
		/** findItem.
		* A matrix level is searched for a given text item on addressed level. The
		* item's index on that level will be returned if it exists.
		* @return - whether the item was found
		*/
		bool findItem(int32& _ItemPos, CmIndex& _Index, const CmString& _Item);

	public:
		/** insertItem. All items on a matrix level with an index equal or higher than
		*   a given start index will be moved one index up. A new matrix entry will be
		*   generated on specified index position.
		*/
		bool insertItem(int32 _Position);

	public:
		/** deleteItem. The item at specified position will be deleted.
		*/
		bool deleteItem(int32 _Position);

	public:
		/** resetNextIndex/allItems/allValues.
		*   The reset function sets the travel index to start position 0.
		*   Addressed level will be traveled one by one with each function call.
		*   If a format match was requested and the format does not match then the 
		*  result will be empty or zero.
		*/
		bool resetNextIndex();
		bool allItems(CmString& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(uint64& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(int64& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(double& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(uint32& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(int32& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(float& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(uint16& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(int16& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(uint8& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(int8& _Item, int32& _Position, bool _isMatchFormat = false);
		bool allValues(bool& _Item, int32& _Position, bool _isMatchFormat = false);


		//--------workspace-----------------------------------------------------------

	private:
		// last index
		CmIndex Index;
	};


	// forward declarations (friend)
	class CmChart;

	//------------------------------------------------------------------------------
	// CmValueFTL class
	//------------------------------------------------------------------------------
	//
	/** CmValueFTL.
	*	This class encapsulates a value which is updated from/synchronized with a
	*   FTLight structure. It can be used for delivering a value to modules that
	*   receive FTLight files/streams or when those values are sent.
	*/
	class CmValueFTL : public CmStringFTL
	{
#define IFTL_CmValueFTL		"EKD@JN58nc_Türkenfeld."APP_PROGRAM

		//------Matrix-access-params------------------------------------------------

		// *** CmChart related initialization params ***

		typedef enum {
			IP_Color = 0,
			IP_Alpha = 1,
			IP_MinX = 2,
			IP_MaxX = 3,
			IP_MinY = 4,
			IP_MaxY = 5,

		} InitializationParams;

		// *** script params ***

		typedef enum {
			SP_ScriptAuto = -1,
			SP_ScriptPosition = 0,
			SP_ScriptText = 1,
			SP_ScriptSize = 2,
			SP_ScriptRotation = 3,
			SP_ScriptColor = 4,
			SP_ScriptFont = 5,
			SP_ScriptAlpha = 6,

		} ScriptParams;

		// *** line params ***

		typedef enum {
			LP_LineAuto = -1,
			LP_LineEdges = 0,
			LP_LineWidth = 1,
			LP_LineColor = 2,
			LP_FillColor = 3,
			LP_LineAlpha = 4,
			LP_FillAlpha = 5,

		} LineParams;

		// *** position coordinates ***

		typedef enum {
			PC_X = 0,
			PC_Y = 1,

		} PositionCoordinates;

		friend	CmChart;

	public:
		CmValueFTL(const int8* _Address = NULL);
		CmValueFTL(CmTypeFTL _TypeFTL);
		virtual ~CmValueFTL();

	public:
		/** unit test for CmValueFTL */
		bool testCmValueFTL();

	public:
		// return label (@ CmValueINI)
		virtual CmString& getLabel();

	public:
		/** getAddress.
		*   The address in FTLight hierarchy is returned.
		*/
		const CmString& getAddress();

	public:
		/** getNextConfigValue.	*/
		CmValueFTL& getNextConfigValue(bool& _isEndOfConfigValues);

	public:
		/** getConfigValue. Return indexed config value starting at this. */
		CmValueFTL* getConfigValue(int32& _Index);

	public:
		/** allValuesFTL. Iterate the list of CmValueFTL */
		bool allValuesFTL(CmValueFTL** _ValueFTL);

	public:
		/** update.
		*   The value(s) will be updated from a FTLight structure using stored address.
		*   Alternatively, an index can be specified as a trailing address component.
		*   Further, a start address can be determined that address calculations refers to.
		*/
		bool updateSettings(CmStringFTL& _StringFTL, CmString _StartAddr = CmString());
		bool updateFrom(CmStringFTL& _StringFTL, CmString _StartAddr = CmString());
		bool updateFrom(CmStringFTL& _StringFTL, int32 _Index, CmString _StartAddr = CmString());

	public:
		/** syncSettings.
		*  A FTLight structure will be synchronized with Settings (string) values
		*/
		bool syncSettings(CmStringFTL& _StringFTL);

	public:
		/** updateInfoFTL.
		*  The InfoFTL values will be updated from a CmStringFTL structure.
		*/
		bool updateInfoFTL(CmStringFTL& _InfoFTL, CmValueFTL& _Return, CmString _StartAddr = CmString());

	public:
		/** serializeValueFTL
		*  An array of CmValueFTL items will synchronize its CmStringFTL template.
		*  Subsequently, the updated CmStringFTL will be converted to a string.
		*/
		bool serializeValueFTL(CmString& _StringFTL, CmStringFTL& _TemplateFTL, CmValueINI& _Return);

	public:
		/** writeInfoFTL.
		*  A config's CmStringFTL representation will be updated from a config's CmValueFTL structure.
		*  Subsequently, the config's CmStringFTL representation will be written to disk.
		*  Optionally, the configuration file can be written to a subfolder with same name as the file.
		*/
		bool writeInfoFTL(CmString _ConfigPath, CmStringFTL& _InfoFTL, CmValueINI& _Return, const int8* _NameExtension = NULL);

	public:
		/** readInfoFTL.
		*	A config string will be read from disk. Subsequently, the string will be converted into
		*   a CmStringFTL structure and that will be used to update a config's CmValueFTL structure.
		*   Optionally, the configuration file can read from a subfolder with same name as the file.
		*/
		bool readInfoFTL(CmString _ConfigPath, CmValueINI& _Return, const int8* _NameExtension = NULL);

	public:
		/** determineListOfProfiles.
		*  The profile folder will be searched for available configuration files.
		*  A list of profile name extensions will be returned in a matrix as follows:
		*
		*  [']=update flag, [i]=profile name extension
		*/
		bool determineListOfProfiles(CmString _ConfigPath, CmString _AppProfile, CmString _AppVersion);

	public:
		/** updateInformation.
		*  Local variables will be updated from a FTLight coded string.
		*/
		bool updateInformation(CmString& _Information, CmValueINI& _Return);

	public:
		/** clearConfig. Local Config variables will be cleared */
		bool clearConfig();

	public:
		/** operator=(). Write access to CmValueFTL */
		const CmStringFTL& operator=(const CmStringFTL& _StringFTL);
		const CmString& operator=(const CmString& _String);
		const int8* operator=(const int8* _String);
		void* operator = (void* _ValPointer);
		int8 operator = (int8 _Val8);
		uint8 operator = (uint8 _Val8);
		bool operator = (bool _ValBool);
		int16 operator = (int16 _Val16);
		uint16 operator = (uint16 _Val16);
		int32 operator = (int32 _Val32);
		uint32 operator = (uint32 _Val32);
		float operator = (float _ValFloat);
		int64 operator = (int64 _Val64);
		uint64 operator = (uint64 _Val64);
		double operator = (double _ValDouble);

	public:
		/** as<data format>...
		*   These are several conversion functions for particular data formats
		*/
		CmString asStringFTL(CmString _Prefix, int32 _Precision = -1, int32 _Digits = -1);
		CmString asStringFTLbool(CmString _Prefix);
		CmString& asStringConvert(int32 _Precision = -1, int32 _Digits = -1);

	public:
		/** explicit type conversion */
		operator CmString& ();
		operator const int8* ();
		operator void* ();
		operator uint64();
		operator int64();
		operator double();
		operator uint32();
		operator int32();
		operator float();
		operator uint16();
		operator int16();
		operator uint8();
		operator int8();
		operator bool();

	public:
		/** compare operators. Content will be evaluated dependent on type */
		// string
		bool operator == (const int8* _String);
		bool operator != (const int8* _String);
		bool operator == (CmString& _String);
		bool operator != (CmString& _String);
		// double
		bool operator == (double _Value);
		bool operator != (double _Value);
		bool operator > (double _Value);
		bool operator < (double _Value);
		// int
		bool operator == (int64 _Value);
		bool operator != (int64 _Value);
		bool operator > (int64 _Value);
		bool operator < (int64 _Value);
		// uint
		bool operator == (uint64 _Value);
		bool operator != (uint64 _Value);
		bool operator > (uint64 _Value);
		bool operator < (uint64 _Value);
		// bool
		bool operator == (bool _Value);
		bool operator != (bool _Value);

	public:
		// large numbers operators
		CmString& setNumber(const CmString& _Num);
		// comparison
		bool operator > (const CmValueFTL& _Value);
		bool operator < (const CmValueFTL& _Value);
		bool operator >= (const CmValueFTL& _Value);
		bool operator <= (const CmValueFTL& _Value);
		// arithmetic operations
		CmString operator+ (const CmValueFTL& _B);
		CmString& operator+= (const CmValueFTL& _B);
		CmString operator- (const CmValueFTL& _B);
		CmString& operator-= (const CmValueFTL& _B);
		CmString operator* (const CmValueFTL& _B);
		CmString& operator*= (const CmValueFTL& _B);
		CmString operator/ (const CmValueFTL& _B);
		CmString& operator/= (const CmValueFTL& _B);
		CmString operator% (const CmValueFTL& _B);
		CmString& getRemainder();

	public:
		// string operations
		void operator+=(const CmString& _String);
		void operator+=(const int8* _String);
		void operator+=(int32 _Value);
		void operator+=(uint32 _Value);
		void operator+=(int64 _Value);
		void operator+=(uint64 _Value);

	private:
		/** asDataFormat.
		*   A string/numeric content will be converted into requested data format
		*/
		bool asDataFormat(CmDataFormat _DataFormat, int32 _Precision = -1, int32 _Digits = -1);
		int32 limitPrecision(double _Value, int32 _ValidDigits = 2);

		//--------CmMatrixFTL-integration---------------------------------------------
	public:
		/** clearMatrix.
		*  All matrix elements will be removed. The matrix will become empty.
		*/
		bool clearMatrix();

	public:
		/** M.
		* Retrieve respectively create the root of a polydimensional matrix.
		* Lock (implicitely) and unlock (explicitely) matrix access. The
		* leave function will return the critical section's recursion counter.
		*/
		CmMatrix& getMatrix(int32 _LockID = 0);
		int32 leaveMatrix();
		int32 MatrixRecursionCounter;

		//--------CmMatrixFTL-application---------------------------------------------

	public:
		/** set/getItemUpdated.
		*  Item updates are indicated by a flag in the scalar position of a matrix.
		*
		*  [`]=item updated
		*/
		bool setItemUpdated();
		bool clearItemUpdated();

	public:
		/** set/getDrawingEnabled/ItemEnabled.
		*  Drawing on screen can be activated/deactivated by a flag in the scalar
		*  position of a matrix respectively in the scalar position of an item.
		*
		*  [`]=enabled
		*/
		bool setDrawingEnabled(bool _isDrawingEnabled);
		bool getDrawingEnabled();
		bool setItemEnabled(int32 _Index, bool _isItemEnabled);
		bool getItemEnabled(int32 _Index);
		int32 getItemCount();

	public:
		/** setChartBackground/Axis.
		*  The background parameters as well as the axis parameters are set/get.
		*
		* [0]=background, [1]=alpha, [2]=Xmin, [3]=Xmax, [4]=Xmin, [5]=Xmax
		*/
		bool setChartBackground(const CmString& _Color, double _Alpha);
		bool getChartBackground(CmString& _Color, double& _Alpha);
		bool setRangeX(double _Xmin, double _Xmax);
		bool getRangeX(double& _Xmin, double& _Xmax);
		bool setRangeY(double _Ymin, double _Ymax);
		bool getRangeY(double& _Ymin, double& _Ymax);


	public:
		/** setScript
		*  A chart text definition along with all parameters will be written to
		*  the matrix on a given index position, respectively it will be retrieved.
		*  The set of display parameters can also be defined as default parameters.
		*  The text output can be disabled for particular items or in general.
		*
		*  [`]=enabled, [i]=default, [0,l]=disabled, [0,l,0/1]=position x/y, [1,l]=text, [2,l]=size, [3,l]=rotation, [4,l]=color, [5,l]=font, [6,l]=alpha
		*/
		bool setScript(int64 _Index, const CmPoint2D& _Position, const CmString& _Text, double _FontSize = -1, double _Rotation = -1, const CmString& _Color = "", double _Alpha = -1, const CmString& _Font = "");
		bool setScript(int32& _Index, const CmPoint2D& _Position, const CmString& _Text, double _FontSize = -1, double _Rotation = -1, const CmString& _Color = "", double _Alpha = -1, const CmString& _Font = "");
		bool setScriptWrap(int32& _Index, const CmPoint2D& _Position, const int32 _PosWrap, const CmString& _Text, double _FontSize = -1, double _Rotation = -1, const CmString& _Color = "", double _Alpha = -1, const CmString& _Font = "");
		bool setScriptDefaults(double _FontSize = 10, double _Rotation = 0, const CmString& _Color = "Black", double _Alpha = 1, const CmString& _Font = "Arial");
		bool getScript(int32 _Index, CmPoint2D& _Position, CmString& _Text, double& _FontSize, double& _Rotation, CmString& _Color, double& _Alpha, CmString& _Font);

	public:
		/** setDrawLine
		*  A chart line definition along with all parameters will be written to
		*  the matrix on a given index position, respectively it will be retrieved.
		*  The set of display parameters can also be defined as default parameters.
		*  The output can be disabled for particular lines or in general.
		*
		*  [`]=enabled, [i]=default, [0,p]=disabled, [0,p,0,0../1,0..]=points, [1,p]=width, [2,p]=line color, [3,p]=fill color, [4,p]=line alpha, [5,p]=fill alpha
		*/

		bool setDrawLine(int32 _Index, CmVector<float>& _PointX, CmVector<float>& _PointY, double _LineWidth = -1, const CmString& _LineColor = "", const CmString& _FillColor = "", double _LineAlpha = -1, double _FillAlpha = -1);
		bool setDrawRectangle(int32 _Index, const CmRectangle2D& _Rectangle, double _LineWidth = -1, const CmString& _LineColor = "", const CmString& _FillColor = "", double _LineAlpha = -1, double _FillAlpha = -1);
		bool setDrawLineDefaults(double _LineWidth = 1, const CmString& _LineColor = "Black", const CmString& _FillColor = "Gray", double _LineAlpha = 1, double _FillAlpha = 0);
		bool getDrawLine(int32 _Index, CmVector<float>& _PointX, CmVector<float>& _PointY, double& _LineWidth, CmString& _LineColor, CmString& _FillColor, double& _LineAlpha, double& _FillAlpha);

	public:
		// return access to LogLevel, Message and Context of a Dynamics structure,
		// assuming that those are subsequent config values
		CmValueFTL& getLogLevel();
		CmValueFTL& getMessage();
		CmValueFTL& getContext();



		//--------workspace-----------------------------------------------------------

	public:
		CmValueFTL* Next;

	protected:
		// address in FTLight hierarchy
		CmString Address;

	protected:
		// chain of CmValueFTL
		static CmValueFTL* ValuesFTL;

	protected:
		// linear array of CmValueFTL elements
		CmValueFTL* Array;

		// polydimensional matrix inside this CmValueFTL 
		CmMatrix* Matrix;

	private:
		// remainder of a devision operation
		CmString Remainder;
	};

	//------------------------------------------------------------------------------
	// CmValueINI class
	//------------------------------------------------------------------------------
	//
	/** CmValueINI.
	*	This class generates addresses for a FTLight structure automatically and
	*   initializes the structure's nodes.
	*   The position inside the structure is controlled as follows:
	*   NULL or "" (empty string) or a valid IFTL -> root node for the IFTL
	*   "/name:value"    -> name:value pair on first level below root
	*   "name:value"     -> a subsequent name:value on current (hierarchy) level
	*   ">name:value"    -> name:value pair on a level below last 'name' level
	*   ">>name:value"   -> name:value pair on a level below last 'value' level
	*   "<name:value"    -> name:value pair on a level above last 'name' level
	*   "<..<name:value" -> name:value two (or more) levels above last 'name' level
	*
	*   Interface to FTLight repository:
	*
	*   ---------------------------
	*	IFTL:Timestamp:Period:86400
	*	/Header
	*    ...
	*   /Data
	*   >DataTimestamp:timebase s
	*   Name1:base unit
	*   ...
	*   NameN:base unit
	*   ---------------------------
	*
	*   The Period determines the time from start to end of a file. The Timestamp
	*   will be generated as a multiple of the Period.
	*
	*   All lines from Header to Data will be copied to each new file.
	*
	*   After the Data entry line, the column headers will be written to the next line
	*   followed by an '@' as indicator for parallel writing. The column headers contain
	*   information about the data values' base and unit as specified in the Interface.
	*
	*   Subsequently, all data sets with a DataTimestamp equal or higher to  Timestamp
	*   and less than Timestamp + Period will be added after column headers.
	*   Each particular data value will be a FTL coded integer. The absolute measure
	*   value can be retrieved by multiplying the integer value by the factor in the
	*   column header. It will be in the unit contained as well in the column header.
	*/
	class CmValueINI : public CmValueFTL
	{
	public:
		CmValueINI(const int8* _Init = NULL);
		virtual ~CmValueINI();

	public:
		/** unit test for CmValueINI */
		bool testCmValueINI();

	public:
		// retrieve name from a name:value pair
		static CmString getName(CmString _NameValue, int* _PosName = NULL);

	public:
		// retrieve value from a name:value pair
		static CmString getValue(CmString _NameValue);

	public:
		// return label
		CmString& getLabel();

	public:
		// An array of CmValueFTL items will be initialized by a CmStringFTL content
		static bool setDefaultInfoFTL(CmValueINI& _ValueINI);
		static bool setDefaultInfoFTL(CmValueINI& _ValueINI, CmValueFTL& _Return);

	public:
		/** getInfoFTL.
		*   A InfoFTL object will be generated from initialization string at IFTL.
		*   Subsequently, it will be synchronized with the CmValueFTL items array.
		*   Furthermore, the InfoFTL can be serialized to a StringFTL.
		*/
		bool getInfoFTL(CmStringFTL& _InfoFTL, CmValueINI& _Return);
		bool getStringFTL(CmString& _StringFTL, CmValueINI& _Return);

	public:
		/** operator=(). Write access to CmValueFTL */
		const CmStringFTL& operator=(const CmStringFTL& _StringFTL) { return CmValueFTL::operator=(_StringFTL); }
		const CmString& operator=(const CmString& _String) { return CmValueFTL::operator=(_String); }
		const int8* operator=(const int8* _String) { return CmValueFTL::operator=(_String); }
		void* operator = (void* _ValPointer) { return CmValueFTL::operator=(_ValPointer); }
		int8 operator = (int8 _Val8) { return CmValueFTL::operator=(_Val8); }
		uint8 operator = (uint8 _Val8) { return CmValueFTL::operator=(_Val8); }
		bool operator = (bool _ValBool) { return CmValueFTL::operator=(_ValBool); }
		int16 operator = (int16 _Val16) { return CmValueFTL::operator=(_Val16); }
		uint16 operator = (uint16 _Val16) { return CmValueFTL::operator=(_Val16); }
		int32 operator = (int32 _Val32) { return CmValueFTL::operator=(_Val32); }
		uint32 operator = (uint32 _Val32) { return CmValueFTL::operator=(_Val32); }
		float operator = (float _ValFloat) { return CmValueFTL::operator=(_ValFloat); }
		int64 operator = (int64 _Val64) { return CmValueFTL::operator=(_Val64); }
		uint64 operator = (uint64 _Val64) { return CmValueFTL::operator=(_Val64); }
		double operator = (double _ValDouble) { return CmValueFTL::operator=(_ValDouble); }

	public:
		/** drawDIF. DIF data will be projected on a canvas.*/
		template <typename T>
		bool drawDIF(CmVector<T>& _X, CmVector<T>& _Y, float _Left, float _Right, float _Top, float _Bottom, float _Baseline, double& _AmplitudeMin, double& _AmplitudeMax, double& _AmplitudePP, double& _RangeMin, double& _Travel, float _Reduction = 1.05)
		{
			// decode DIF data
			CmVector<int64> ValDIF;
			asVector(ValDIF);

			// determine min and max amplitude
			int64 AmplitudeMin = MAX_INT64;
			int64 AmplitudeMax = MIN_INT64;
			for (int i = 0; i < ValDIF.getLength(); i++) {
				if (AmplitudeMin > ValDIF[i]) {
					AmplitudeMin = ValDIF[i];
				}
				if (AmplitudeMax < ValDIF[i]) {
					AmplitudeMax = ValDIF[i];
				}
			}
			_AmplitudePP = double(AmplitudeMax - AmplitudeMin);

			// estimate signal travel
			const double MinRMS = 0.5;
			const double ScaleTravel = 20;  // = 50�V/V
			double Center = double(AmplitudeMin + AmplitudeMax) / 2;
			double RMS = pow(ValDIF[0] - Center, 2);
			double Travel = 0;
			for (int i = 1; i < ValDIF.getLength(); i++) {
				RMS += pow(ValDIF[i] - Center, 2);
				Travel += abs(ValDIF[i] - ValDIF[i - 1]);
			}
			RMS = sqrt(RMS / ValDIF.getLength());
			Travel /= ValDIF.getLength();
			_Travel = RMS > MinRMS ? ScaleTravel * Travel / RMS : 0;

			// adjust for even range values
			AmplitudeMin = int64(10 * ceil(_Reduction * AmplitudeMin / 10));
			AmplitudeMax = int64(10 * ceil(_Reduction * AmplitudeMax / 10));
			// determine scaling
			double ScaleMax = AmplitudeMax != 0 ? (_Top - _Baseline) / AmplitudeMax : 0;
			double ScaleMin = AmplitudeMin != 0 ? (_Bottom - _Baseline) / AmplitudeMin : 0;
			double Scale = AmplitudeMin >= 0 ? ScaleMax : AmplitudeMax <= 0 ? ScaleMin : fmin(ScaleMin, ScaleMax);
			// minimal range constraint
			if (_RangeMin * Scale > _Top - _Baseline) {
				Scale = (_Top - _Baseline) / _RangeMin;
			}
			else {
				_RangeMin = double(AmplitudeMax > 0 ? AmplitudeMax : -AmplitudeMin);
			}

			_AmplitudeMin = double(AmplitudeMin);
			_AmplitudeMax = double(AmplitudeMax);

			// beginning left
			_X[0] = _Left;	_Y[0] = _Bottom;

			// generate chart data
			int32 DataLength = ValDIF.getLength();
			float RangeX = _Right - _Left + 1;
			int i = 0;
			for (; i < DataLength; i++) {
				_X[i + 1] = T(RangeX * i / DataLength + _Left);
				_Y[i + 1] = T(Scale * ValDIF[i] + _Baseline);
			}

			// finishing right
			_X[i] = T(_Right);	_Y[i] = T(_Bottom);

			return true;
		}

	public:
		// access initialization string
		bool setStringINI(CmString& _StringINI);
		CmString* getStringINI();

	public:
		// clear workspace
		bool clearWorkspace();

		//--------workspace-----------------------------------------------------------

	private:
		// label
		CmString Label;

	private:
		// automatic address and FTL hierarchy generation
		CmMatrix* MatrixINI;
		CmString* StringINI;
	};

	//--------------------------------------------------------------------------------
	// template for deriving a new config from a default config
	//--------------------------------------------------------------------------------
	//
	// ToDo: DataFormat changes, e.g. from DATAFORMAT_FTLight to DATAFORMAT_STRING
	//
	template <typename T>
	T& newConfig(T** T1, T& T0) {
		if (NULL == *T1) {
			// create a new config from default config
			*T1 = new T(T0);
			// fallback to default config if 'new' failed
			NULL == *T1 ? *T1 = &T0 : 0;
			// obtain member list
			int32 Count = sizeof(T) / sizeof(CmValueINI);
			CmValueINI* INI = (CmValueINI*)*T1;
			// adjust member list
			for (int32 i = 0; i < Count - 1; i++) {
				(INI + i)->Next = INI + i + 1;
			}
			// clear workspace (which has to be disconnected from template, otherwise desctructor will fail) 
			for (int32 i = 0; i < Count; i++) {
				(INI + i)->clearWorkspace();
			}
			// copy StringINI 
			CmValueINI* IFTL = (CmValueINI*)&T0;
			NULL != IFTL->getStringINI() ? INI->setStringINI(*IFTL->getStringINI()) : 0;
		}

		return **T1;
	}

} // namespace Cosmos

using namespace Cosmos;

//----------------------------------------------------------------------------
// CmMatrixFTL class
//----------------------------------------------------------------------------
//
CmMatrixFTL::CmMatrixFTL()
{

}
CmMatrixFTL::~CmMatrixFTL()
{
	clearMatrix();
}

bool CmMatrixFTL::testCmMatrixFTL()
{
	bool Result = true;

	// test values of different formats
	CmString TestString("67890.12345");
	CmString TestStringFloat("67890.125");
	double TestValue;
	sscanf_s(TestString.getText(), "%lf", &TestValue);
	const void* TestPointer = (void*)&TestValue;
	const uint64 TestUInt64 = uint64(TestValue);
	const int64 TestInt64 = -int64(TestValue);
	const double TestDouble = double(TestValue);
	const uint32 TestUInt32 = uint32(TestValue);
	const int32 TestInt32 = -int32(TestValue);
	const float TestFloat = float(TestValue);
	const uint16 TestUInt16 = uint16(TestValue);
	const int16 TestInt16 = -int16(TestValue);
	const uint8 TestUInt8 = uint8(TestValue);
	const int8 TestInt8 = -int8(TestValue);
	const bool TestBool = true;

	// test CmVector
	double Value;
	CmVector<double> Vector;
	Vector[0] = 1;
	Vector[2] = 1;
	if (1 != Vector[2]) Result = false;
	Vector[1] = TestInt32;
	if (TestInt32 != Vector[1]) Result = false;
	// vector extension
	if (Vector.getLength() != 3) Result = false;
	Vector[3] = TestInt32;
	if (Vector.getLength() != 4) Result = false;
	if (4 != Vector.getLength()) Result = false;
	if (TestInt32 != Vector[3]) Result = false;
	if (TestInt32 != Vector[1]) Result = false;
	// further size extension
	const int32 Index = 1500;
	Vector[Index] = TestInt32;
	if (Vector.getLength() != Index + 1) Result = false;
	if (1 != Vector[0]) Result = false;
	if (TestInt32 != Vector[1]) Result = false;
	if (TestInt32 != Vector[3]) Result = false;
	if (TestInt32 != Vector[Index]) Result = false;
	// size reduction
	const int32 Length = 3;
	Vector.setLength(Length);
	if (Vector.getLength() != Length) Result = false;
	if (1 != Vector[0]) Result = false;
	if (TestInt32 != Vector[1]) Result = false;
	if (1 != Vector[2]) Result = false;
	// length extension, size unchanged
	const int32 ExtendedLength = 5;
	Vector.setLength(ExtendedLength);
	if (Vector.getLength() != ExtendedLength) Result = false;
	if (1 != Vector[0]) Result = false;
	if (0 != Vector[ExtendedLength - 1]) Result = false;
	// length extension, size extension
	const int32 ExtendedLengthSize = 1500;
	Vector.setLength(ExtendedLengthSize);
	if (Vector.getLength() != ExtendedLengthSize) Result = false;
	if (1 != Vector[0]) Result = false;
	if (0 != Vector[ExtendedLengthSize - 1]) Result = false;

	// vector copy & compare
	Vector(1, 2, 3);
	Vector.setScalar(5);
	CmVector<double> Vector1;
	Vector1 = Vector;
	if (false == (Vector1 == Vector)) Result = false;

	// runtime 2000
	CmTimestamp Time;
	const char* MsgInit = "\n runtime double[%d] %s           %5.1f ns";
	int32 MaxIndex = 2000;
	// read
	Time.startRuntime();
	for (int32 i = 0; i < MaxIndex; i++) {
		Value = Vector[i];
	}
	printf(MsgInit, MaxIndex, "read         ", (double)Time.getRuntime_ns() / MaxIndex);
	// write
	Value = 12345678.87654321;
	Time.startRuntime();
	for (int32 i = 0; i < MaxIndex; i++) {
		Vector[i] = Value;
	}
	printf(MsgInit, MaxIndex, "write        ", (double)Time.getRuntime_ns() / MaxIndex);
	// write/read
	Time.startRuntime();
	for (int32 i = 0; i < MaxIndex; i++) {
		Vector[i] = Value;
		Value = Vector[i];
	}
	printf(MsgInit, MaxIndex, "write/read   ", (double)Time.getRuntime_ns() / MaxIndex);
	// write/read 1000000
	MaxIndex = 1000000;
	Time.startRuntime();
	for (int32 i = 0; i < MaxIndex; i++) {
		Vector[i] = Value;
		Value = Vector[i];
	}
	printf(MsgInit, MaxIndex, "write/read", (double)Time.getRuntime_ns() / MaxIndex);

	// test CmMatrixFTL constructor/destructor
	CmIndex I;
	const int32 Count = 10000;
	CmMatrixFTL* Array[Count];
	const int32 Levels = 3;
	I(1, 2, 3);
	const char* MsgMatrix = "\n runtime matrix new/delete(%d)          %8.1f ns";
	Time.startRuntime();
	for (int i = 0; i < Count; i++) {
		Array[i] = CmString::allocateMemory<CmMatrixFTL>(1, isCmMatrixFTL);
		Array[i]->setString(TestString, I);
	}
	CmString Delete;
	for (int i = 0; i < Count; i++) {
		Delete.releaseMemory<CmMatrixFTL>(Array[i], 1, isCmMatrixFTL);
	}
	printf(MsgMatrix, Count, (double)Time.getRuntime_ns() / (Levels * Count));

	//---test skalar
	CmMatrixFTL Matrix;
	Matrix.setValue(TestUInt16, I(0, 0));
	Matrix.setValue(TestUInt32, I(0, 0, 0));
	Matrix.setValue(TestUInt8, I(0));
	if (TestUInt16 != uint16(Matrix[I(0, 0)])) Result = false;
	if (TestUInt32 != uint32(Matrix[I(0, 0, 0)])) Result = false;
	if (TestUInt8 != uint8(Matrix[I(0)])) Result = false;

	//---test CmMatrixFTL 2x3
	// write item 1,2,3
	Matrix.setValue(TestDouble, I(1, 2));
	Matrix.setValue(TestInt64, I(0, 1));
	Matrix.setValue(TestUInt8, I(1, 0));
	// read item 1,2,3
	if (TestDouble != double(Matrix[I(1, 2)])) Result = false;
	if (TestInt64 != int64(Matrix[I(0, 1)])) Result = false;
	if (TestUInt8 != uint8(Matrix[I(1, 0)])) Result = false;

	//---test getVectorLength() 
	if (2 != Matrix.getVectorLength(I.clear())) Result = false;
	if (3 != Matrix.getVectorLength(I(1))) Result = false;
	if (0 != Matrix.getVectorLength(I(1, 2))) Result = false;
	if (0 != Matrix.getVectorLength(I(1, 2, 3))) Result = false;

	//---test partial clearMatrix()
	if (false == Matrix.clearMatrix(I(1))) return false;
	if (TestInt64 != int64(Matrix[I(0, 1)])) Result = false;
	//---intended test of a removed item in the deleted sub-matrix
	if (TestDouble == double(Matrix[I(1, 2)])) Result = false;

	//---test clearMatrix()
	if (false == Matrix.clearMatrix()) return false;

	//---test formats
	// string
	Matrix.setString(TestString, I);
	if (Matrix.asString(I) != TestString) Result = false;
	// pointer
	Matrix.setValue((void*)TestPointer, I);
	if ((void*)uint64(Matrix[I]) != TestPointer) Result = false;
	// uint64
	Matrix.setValue(TestUInt64, I);
	if (uint64(Matrix[I]) != TestUInt64) Result = false;
	// int64
	Matrix.setValue(TestInt64, I);
	if (int64(Matrix[I]) != TestInt64) Result = false;
	// double
	Matrix.setValue(TestDouble, I);
	if (double(Matrix[I]) != TestDouble) Result = false;
	// uint32
	Matrix.setValue(TestUInt32, I);
	if (uint32(Matrix[I]) != TestUInt32) Result = false;
	// int32
	Matrix.setValue(TestInt32, I);
	if (int32(Matrix[I]) != TestInt32) Result = false;
	// float
	Matrix.setValue(TestFloat, I);
	if (float(Matrix[I]) != TestFloat) Result = false;
	// uint16
	Matrix.setValue(TestUInt16, I);
	if (uint16(Matrix[I]) != TestUInt16) Result = false;
	// int16
	Matrix.setValue(TestInt16, I);
	if (int16(Matrix[I]) != TestInt16) Result = false;
	// uint8
	Matrix.setValue(TestUInt8, I);
	if (uint8(Matrix[I]) != TestUInt8) Result = false;
	// int8
	Matrix.setValue(TestInt8, I);
	if (int8(Matrix[I]) != TestInt8) Result = false;
	// bool
	Matrix.setValue(TestBool, I);
	if (Matrix.asBool(I) != TestBool) Result = false;

	//---test full CmMatrixFTL 3x4x5
	const char* MsgInitMatrix = "\n runtime matrix %s           %8.1f ns";
	double Length3D;
	uint32 ItemCount = 0;
	Time.startRuntime();
	// write
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 5; k++) {
				// length of the space diagonal
				I(i, j, k);
				Length3D = sqrt((double)(i * i + j * j + k * k));
				Length3D = k % 2 == 0 ? Length3D : -Length3D;
				// write 
				Matrix.setValue(Length3D, I);
				ItemCount++;
			}
		}
	}
	// read
	for (int k = 0; k < 5; k++) {
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 3; i++) {
				// length of the space diagonal
				I(i, j, k);
				Length3D = sqrt((double)(i * i + j * j + k * k));
				Length3D = k % 2 == 0 ? Length3D : -Length3D;
				// read and compare
				if (double(Matrix[I]) != Length3D) Result = false;
			}
		}
	}
	printf(MsgInitMatrix, "3x4x5 write/read", (double)Time.getRuntime_ns() / ItemCount);

	//---test sparse CmMatrixFTL 26D
	CmIndex I1;
	CmIndex I2;
	CmIndex I3;
	CmIndex I4;
	const int Loops = 5000;
	Time.startRuntime();
	for (int i = 0; i < Loops; i++) {
		// modify indexes
		I1(1, 1, i, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6);
		I2(1, 1, 1, 1, 1, 2, 2, i, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6);
		I3(1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, i, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6);
		I4(1, 1, 1, i, 1, 2, 2, i, 2, 2, 3, 3, i, 3, 3, 4, 4, i, 4, 4, 5, 5, i, 5, 5, 6);
		// write 4 entries
		Matrix.setValue(1 * TestDouble, I1);
		Matrix.setValue(2 * TestDouble, I2);
		Matrix.setValue(3 * TestDouble, I3);
		Matrix.setString(TestString, I4);
		// read and compare 4 entries
		if (double(Matrix[I1]) != 1 * TestDouble) Result = false;
		if (double(Matrix[I2]) != 2 * TestDouble) Result = false;
		if (double(Matrix[I3]) != 3 * TestDouble) Result = false;
		if (Matrix.asString(I4) != TestString) Result = false;
	}
	printf(MsgInitMatrix, "26D write/read  ", (double)Time.getRuntime_ns() / (4 * Loops));

	//---test CmMatrix
	CmMatrix M;
	// string
	M[I(0)] = TestString;
	if (CmString(M(0)) != TestString) Result = false;
	// void*
	M[I(0, 1)] = (void*)TestPointer;
	if ((void*)uint64(M(0, 1)) != TestPointer) Result = false;
	// uint64
	M[I(0, 1, 2)] = TestUInt64;
	if (uint64(M(0, 1, 2)) != TestUInt64) Result = false;
	// int64
	M[I(0, 1, 2, 3)] = TestInt64;
	if (int64(M(0, 1, 2, 3)) != TestInt64) Result = false;
	// uint32
	M[I(0, 1, 2, 3, 4)] = TestUInt32;
	if (uint32(M(0, 1, 2, 3, 4)) != TestUInt32) Result = false;
	// int32
	M[I(0, 1, 2, 3, 4, 5)] = TestInt32;
	if (int32(M(0, 1, 2, 3, 4, 5)) != TestInt32) Result = false;
	// float
	M[I(0, 1, 2, 3, 4, 5, 6)] = TestFloat;
	if (float(M(0, 1, 2, 3, 4, 5, 6)) != TestFloat) Result = false;
	// uint16
	M[I(0, 1, 2, 3, 4, 5, 6, 7)] = TestUInt16;
	if (uint16(M(0, 1, 2, 3, 4, 5, 6, 7)) != TestUInt16) Result = false;
	// int16
	M[I(0, 1, 2, 3, 4, 5, 6, 7, 8)] = TestInt16;
	if (int16(M(0, 1, 2, 3, 4, 5, 6, 7, 8)) != TestInt16) Result = false;
	// uint8
	M[I(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)] = TestUInt8;
	if (uint8(M(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)) != TestUInt8) Result = false;
	// int8
	M[I(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)] = TestInt8;
	if (int8(M(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)) != TestInt8) Result = false;
	// bool
	M[I(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11)] = TestBool;
	if (bool(M(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11)) != TestBool) Result = false;
	// re-test all entries
	if (CmString(M(0)) != TestString) Result = false;
	if ((void*)uint64(M(0, 1)) != TestPointer) Result = false;
	if (uint64(M(0, 1, 2)) != TestUInt64) Result = false;
	if (int64(M(0, 1, 2, 3)) != TestInt64) Result = false;
	if (uint32(M(0, 1, 2, 3, 4)) != TestUInt32) Result = false;
	if (int32(M(0, 1, 2, 3, 4, 5)) != TestInt32) Result = false;
	if (float(M(0, 1, 2, 3, 4, 5, 6)) != TestFloat) Result = false;
	if (uint16(M(0, 1, 2, 3, 4, 5, 6, 7)) != TestUInt16) Result = false;
	if (int16(M(0, 1, 2, 3, 4, 5, 6, 7, 8)) != TestInt16) Result = false;
	if (uint8(M(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)) != TestUInt8) Result = false;
	if (int8(M(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)) != TestInt8) Result = false;
	if (bool(M(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11)) != TestBool) Result = false;
	// intended mismatch
	if (int32(M(0, 1, 2, 3, 4, 6)) == TestInt32) Result = false;

	//---test partial clearMatrix()
	M(1) = TestString;
	if (false == M(0).clearMatrix()) return false;
	if (CmString(M(1)) != TestString) Result = false;
	//---intended test of a removed item in the deleted sub-matrix
	if (uint64(M(0, 1, 2)) == TestUInt64) Result = false;

	// test allItems() function
	M.clearMatrix();
	M(0, 0) = TestString;
	M(0, 1) = TestUInt64;
	M(0, 2) = TestInt64;
	M(0, 3) = TestDouble;
	M(0, 4) = TestUInt32;
	M(0, 5) = TestInt32;
	M(0, 6) = TestFloat;
	M(0, 7) = TestUInt16;
	M(0, 8) = TestInt16;
	M(0, 9) = TestUInt8;
	M(0, 10) = TestInt8;
	M(0, 11) = TestBool;
	M(0).resetNextIndex();
	bool isMatchFormat = true;
	int32 Position;
	CmString AllString; if (false == M(0).allItems(AllString, Position, isMatchFormat)) Result = false; if (TestString != AllString) Result = false;
	uint64 AllUInt64; if (false == M(0).allValues(AllUInt64, Position, isMatchFormat)) Result = false; if (TestUInt64 != AllUInt64) Result = false;
	int64 AllInt64; if (false == M(0).allValues(AllInt64, Position, isMatchFormat)) Result = false; if (TestInt64 != AllInt64) Result = false;
	double AllDouble; if (false == M(0).allValues(AllDouble, Position, isMatchFormat)) Result = false; if (TestDouble != AllDouble) Result = false;
	uint32 AllUInt32; if (false == M(0).allValues(AllUInt32, Position, isMatchFormat)) Result = false; if (TestUInt32 != AllUInt32) Result = false;
	int32 AllInt32; if (false == M(0).allValues(AllInt32, Position, isMatchFormat)) Result = false; if (TestInt32 != AllInt32) Result = false;
	float AllFloat; if (false == M(0).allValues(AllFloat, Position, isMatchFormat)) Result = false; if (TestFloat != AllFloat) Result = false;
	uint16 AllUInt16; if (false == M(0).allValues(AllUInt16, Position, isMatchFormat)) Result = false; if (TestUInt16 != AllUInt16) Result = false;
	int16 AllInt16; if (false == M(0).allValues(AllInt16, Position, isMatchFormat)) Result = false; if (TestInt16 != AllInt16) Result = false;
	uint8 AllUInt8; if (false == M(0).allValues(AllUInt8, Position, isMatchFormat)) Result = false; if (TestUInt8 != AllUInt8) Result = false;
	int8 AllInt8; if (false == M(0).allValues(AllInt8, Position, isMatchFormat)) Result = false; if (TestInt8 != AllInt8) Result = false;
	bool AllBool; if (false == M(0).allValues(AllBool, Position, isMatchFormat)) Result = false; if (TestBool != AllBool) Result = false;
	// intended format mismatch
	M(0).resetNextIndex();
	if (true == M(0).allValues(AllUInt64, Position, isMatchFormat)) Result = false; if (AllUInt64 != 0) Result = false;
	if (true == M(0).allItems(AllString, Position, isMatchFormat)) Result = false; if (AllString.getLength() != 0) Result = false;
	if (true == M(0).allValues(AllInt32, Position, isMatchFormat)) Result = false; if (AllInt32 != 0) Result = false;
	// implicit format conversion
	M(0).resetNextIndex();
	/* CmString -> double */ if (false == M(0).allValues(AllDouble, Position)) Result = false; if (TestDouble != AllDouble) Result = false;
	/* uint64 -> int32 */ if (false == M(0).allValues(AllInt32, Position)) Result = false; if (abs(TestInt32) != AllInt32) Result = false;
	/* int64 -> uint32 */ if (false == M(0).allValues(AllUInt32, Position)) Result = false; if (TestInt32 != int32(AllUInt32)) Result = false;
	/* double -> float */ if (false == M(0).allValues(AllFloat, Position)) Result = false; if (TestFloat != AllFloat) Result = false;
	/* uint32 -> uint64 */ if (false == M(0).allValues(AllUInt64, Position)) Result = false; if (TestUInt64 != AllUInt64) Result = false;
	/* int32 -> int64 */ if (false == M(0).allValues(AllInt64, Position)) Result = false; if (TestInt64 != AllInt64) Result = false;
	/* float -> CmString */ if (false == M(0).allItems(AllString, Position)) Result = false; if (TestStringFloat != AllString) Result = false;
	/* uint16 -> bool */ if (false == M(0).allValues(AllBool, Position)) Result = false; if (TestBool != AllBool) Result = false;
	/* int16 -> int8 */ if (false == M(0).allValues(AllInt8, Position)) Result = false; if (TestInt8 != AllInt8) Result = false;
	/* uint8 -> uint16 */ if (false == M(0).allValues(AllUInt16, Position)) Result = false; if (TestUInt8 != AllUInt16) Result = false;
	/* int8 -> int16 */ if (false == M(0).allValues(AllInt16, Position)) Result = false; if (TestInt8 != AllInt16) Result = false;
	/* bool -> uint8 */ if (false == M(0).allValues(AllUInt8, Position)) Result = false; if (TestBool != (AllUInt8 == 0 ? false : true)) Result = false;

	return Result;
}

bool CmMatrixFTL::clearMatrix()
{
	CmIndex I;
	return clearMatrix(I);
}
bool CmMatrixFTL::clearMatrix(CmIndex& _Index)
{
	// retrieve before last level of dimension
	bool isCreateNextLevel = false;
	CmMatrixFTL* MatrixBeforeLastLevel = getBeforeLastLevel(_Index.reset(), isCreateNextLevel);
	if (NULL == MatrixBeforeLastLevel) return false;

	// get last level value and format
	CmMatrixFTL* Vector = MatrixBeforeLastLevel;
	if (_Index.getLength() > 0) {
		int32 Index = _Index[_Index.getMaxIndex()];
		Vector = (CmMatrixFTL*)MatrixBeforeLastLevel->CmVector<uint64>::operator[](Index);
		CmDataFormat Format = MatrixBeforeLastLevel->DataFormat[Index];
		// check format for valid matrix vector
		if (DATAFORMAT_MATRIX != Format || NULL == Vector) return false;
	}


	// travel vectors and delete all created objects
	CmMatrixFTL& M = *Vector;
	for (int32 i = 0; i < M.getLength(); i++) {
		if (DATAFORMAT_STRING == M.DataFormat[i]) {
			CmString* String = (CmString*)M.CmVector<uint64>::operator[](i);
			if (NULL != String) {
				CmString::releaseMemory<CmString>(String, 1, isCmString);
			}
			M.DataFormat[i] = DATAFORMAT_NONE;
		}
		if (DATAFORMAT_MATRIX == M.DataFormat[i]) {
			CmMatrixFTL* Vector = (CmMatrixFTL*)M.CmVector<uint64>::operator[](i);
			if (NULL != Vector) {
				CmString::releaseMemory<CmMatrixFTL>(Vector, 1, isCmMatrixFTL);
			}
			M.DataFormat[i] = DATAFORMAT_NONE;
		}
	}
	M.setLength(0);
	M.DataFormat.clear();

	// delete objects that are stored in the scalar
	uint64 Scalar;
	CmDataFormat ScalarFormat;
	M.getScalar(Scalar, ScalarFormat);
	if (DATAFORMAT_STRING == ScalarFormat) {
		CmString* String = (CmString*)Scalar;
		if (NULL != String) {
			CmString::releaseMemory<CmString>(String, 1, isCmString);
		}
		M.setScalar(NULL, DATAFORMAT_NONE);
	}
	if (DATAFORMAT_MATRIX == ScalarFormat) {
		CmMatrixFTL* Vector = (CmMatrixFTL*)Scalar;
		if (NULL != Vector) {
			CmString::releaseMemory<CmMatrixFTL>(Vector, 1, isCmMatrixFTL);
		}
		M.setScalar(NULL, DATAFORMAT_NONE);
	}



	return true;
}

int32 CmMatrixFTL::getVectorLength(CmIndex& _Index)
{
	// check for root vector
	if (0 == _Index.getLength()) {
		return getLength();
	}

	// retrieve before last level of dimension
	bool isCreateNextLevel = false;
	CmMatrixFTL* MatrixBeforeLastLevel = getBeforeLastLevel(_Index.reset(), isCreateNextLevel);
	if (NULL == MatrixBeforeLastLevel) return 0;

	// get current value and format
	int32 Index = _Index[_Index.getMaxIndex()];
	CmMatrixFTL* Vector = (CmMatrixFTL*)MatrixBeforeLastLevel->CmVector<uint64>::operator[](Index);
	CmDataFormat Format = MatrixBeforeLastLevel->DataFormat[Index];

	// check format for valid matrix vector
	if (DATAFORMAT_MATRIX != Format || NULL == Vector) return 0;

	return Vector->getLength();
}

bool CmMatrixFTL::setScalar(uint64 _Scalar, CmDataFormat _ScalarFormat)
{
	CmVector<uint64>::setScalar(_Scalar);
	DataFormat.setScalar(_ScalarFormat);

	return true;
}
bool CmMatrixFTL::getScalar(uint64& _Scalar, CmDataFormat& _ScalarFormat)
{
	_Scalar = CmVector<uint64>::getScalar();
	_ScalarFormat = DataFormat.getScalar();

	return true;
}

double CmMatrixFTL::operator[](CmIndex& _Index)
{
	// retrieve last level of dimension
	bool isCreateNextLevel = false;
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset(), isCreateNextLevel);
	if (NULL == MatrixFTL) return 0;

	// get current value and format
	uint64 Value = 0;
	CmDataFormat Format = DATAFORMAT_NONE;
	MatrixFTL->getValue(Value, Format, _Index);

	// check data format for numeric value
	double Double = 0;
	switch (Format) {
		// non-numeric formats
	case DATAFORMAT_NONE: break;
	case DATAFORMAT_FTLight: break;
	case DATAFORMAT_STRING: break;
		// numeric formats
	case DATAFORMAT_BINARY: Double = (double)Value; break;
	case DATAFORMAT_UINT64: Double = (double)(uint64)Value; break;
	case DATAFORMAT_INT64: Double = (double)(int64)Value; break;
	case DATAFORMAT_DOUBLE: Double = *((double*)&Value); break;
	case DATAFORMAT_UINT32: Double = (double)(uint32)Value; break;
	case DATAFORMAT_INT32: Double = (double)(int32)Value; break;
	case DATAFORMAT_FLOAT: Double = (double)(*((float*)&Value)); break;
	case DATAFORMAT_UINT16: Double = (double)(uint16)Value; break;
	case DATAFORMAT_INT16: Double = (double)(int16)Value; break;
	case DATAFORMAT_UINT8: Double = (double)(uint8)Value; break;
	case DATAFORMAT_INT8: Double = (double)(int8)Value; break;
	case DATAFORMAT_BOOL: Double = (double)Value; break;
		// vector
	case DATAFORMAT_MATRIX: break;
	default: break;
	}

	return Double;
}
double CmMatrixFTL::operator[](int32 _Index)
{
	// get current value and format
	CmIndex I(_Index);
	uint64 Value = 0;
	CmDataFormat Format = DATAFORMAT_NONE;
	getValue(Value, Format, I);

	// check data format for numeric value
	double Double = 0;
	switch (Format) {
		// non-numeric formats
	case DATAFORMAT_NONE: break;
	case DATAFORMAT_FTLight: break;
	case DATAFORMAT_STRING: break;
		// numeric formats
	case DATAFORMAT_BINARY: Double = (double)Value; break;
	case DATAFORMAT_UINT64: Double = (double)(uint64)Value; break;
	case DATAFORMAT_INT64: Double = (double)(int64)Value; break;
	case DATAFORMAT_DOUBLE: Double = *((double*)&Value); break;
	case DATAFORMAT_UINT32: Double = (double)(uint32)Value; break;
	case DATAFORMAT_INT32: Double = (double)(int32)Value; break;
	case DATAFORMAT_FLOAT: Double = (double)(*((float*)&Value)); break;
	case DATAFORMAT_UINT16: Double = (double)(uint16)Value; break;
	case DATAFORMAT_INT16: Double = (double)(int16)Value; break;
	case DATAFORMAT_UINT8: Double = (double)(uint8)Value; break;
	case DATAFORMAT_INT8: Double = (double)(int8)Value; break;
	case DATAFORMAT_BOOL: Double = (double)Value; break;
		// vector
	case DATAFORMAT_MATRIX: break;
	default: break;
	}

	return Double;
}
bool CmMatrixFTL::setValue(int8 _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value, DATAFORMAT_INT8, _Index);
}
bool CmMatrixFTL::setValue(uint8 _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value, DATAFORMAT_UINT8, _Index);
}
bool CmMatrixFTL::setValue(bool _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value ? 1 : 0, DATAFORMAT_BOOL, _Index);
}
bool CmMatrixFTL::setValue(int16 _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value, DATAFORMAT_INT16, _Index);
}
bool CmMatrixFTL::setValue(uint16 _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value, DATAFORMAT_UINT16, _Index);
}
bool CmMatrixFTL::setValue(int32 _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value, DATAFORMAT_INT32, _Index);
}
bool CmMatrixFTL::setValue(uint32 _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value, DATAFORMAT_UINT32, _Index);
}
bool CmMatrixFTL::setValue(float _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue(*((uint32*)&_Value), DATAFORMAT_FLOAT, _Index);
}
bool CmMatrixFTL::setValue(int64 _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value, DATAFORMAT_INT64, _Index);
}
bool CmMatrixFTL::setValue(uint64 _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)_Value, DATAFORMAT_UINT64, _Index);
}
bool CmMatrixFTL::setValue(double _Value, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue(*((uint64*)&_Value), DATAFORMAT_DOUBLE, _Index);
}
bool CmMatrixFTL::setValue(void* _Value, CmIndex& _Index)
{
	uint64 Pointer = uint64(_Value);
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());
	return NULL == MatrixFTL ? false : MatrixFTL->setValue(*((uint64*)&Pointer), DATAFORMAT_BINARY, _Index);
}
bool CmMatrixFTL::setString(const CmString& _String, CmIndex& _Index)
{
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset());

	CmString* String = CmString::allocateMemory<CmString>(1, isCmString);
	(*String) = _String;
	return NULL == MatrixFTL ? false : MatrixFTL->setValue((uint64)String, DATAFORMAT_STRING, _Index);
}

CmString CmMatrixFTL::asString(CmIndex& _Index)
{
	// retrieve last level of dimension
	bool isCreateNextLevel = false;
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset(), isCreateNextLevel);
	if (NULL == MatrixFTL) return CmString();

	// get current value and format
	uint64 Value = 0;
	CmDataFormat Format = DATAFORMAT_NONE;
	MatrixFTL->getValue(Value, Format, _Index);

	// check for string
	if (DATAFORMAT_STRING != Format || NULL == Value) return CmString();

	return *(CmString*)Value;
}

uint64 CmMatrixFTL::asUInt64(CmIndex& _Index)
{
	return (uint64)this->operator[](_Index);
}
int64 CmMatrixFTL::asInt64(CmIndex& _Index)
{
	return (int64)this->operator[](_Index);
}
double CmMatrixFTL::asDouble(CmIndex& _Index)
{
	return this->operator[](_Index);
}
uint32 CmMatrixFTL::asUInt32(CmIndex& _Index)
{
	return (uint32)this->operator[](_Index);
}
int32 CmMatrixFTL::asInt32(CmIndex& _Index)
{
	return (int32)this->operator[](_Index);
}
float CmMatrixFTL::asFloat(CmIndex& _Index)
{
	return (float)this->operator[](_Index);
}
uint16 CmMatrixFTL::asUInt16(CmIndex& _Index)
{
	return (uint16)this->operator[](_Index);
}
int16 CmMatrixFTL::asInt16(CmIndex& _Index)
{
	return (int16)this->operator[](_Index);
}
uint8 CmMatrixFTL::asUInt8(CmIndex& _Index)
{
	return (uint8)this->operator[](_Index);
}
int8 CmMatrixFTL::asInt8(CmIndex& _Index)
{
	return (int8)this->operator[](_Index);
}

bool CmMatrixFTL::asBool(CmIndex& _Index)
{
	return this->operator[](_Index) == 0 ? false : true;
}

CmMatrixFTL* CmMatrixFTL::getBeforeLastLevel(CmIndex& _IndexVector, bool _isCreateNextLevel)
{
	// initialize return value
	CmMatrixFTL* NextMatrixLevel = this;

	// check for empty _IndexVector
	if (0 == _IndexVector.getLength()) return NextMatrixLevel;

	// check for next level of dimension
	int32 Level = _IndexVector.getNextIndex();
	if (Level < _IndexVector.getMaxIndex()) {
		// get current entries
		int32 Index = _IndexVector[Level];
		// reject negative indices
		if (Index < 0) return NULL;
		NextMatrixLevel = (CmMatrixFTL*)CmVector<uint64>::operator[](Index);
		CmDataFormat Format = DataFormat[Index];
		// check if a next level of dimension has to be created
		if (DATAFORMAT_MATRIX != Format || NULL == NextMatrixLevel) {
			if (false == _isCreateNextLevel) return NULL;
			NextMatrixLevel = CmString::allocateMemory<CmMatrixFTL>(1, isCmMatrixFTL);
			// put previous value to scalar of the new vector
			NextMatrixLevel->setScalar(CmVector<uint64>::operator[](Index), DataFormat[Index]);
			// assign new vector to indexed position
			CmVector<uint64>::operator[](Index) = (uint64)NextMatrixLevel;
			DataFormat[Index] = DATAFORMAT_MATRIX;
			if (NULL == NextMatrixLevel) return NULL;
		}
		// retrieve last level of dimension
		NextMatrixLevel = NextMatrixLevel->getBeforeLastLevel(_IndexVector);
	}
	else {
		//if (false == _isCreateNextLevel){
		//	return NULL;
		//}
	}

	return NextMatrixLevel;
}
bool CmMatrixFTL::setValue(uint64 _Value, CmDataFormat _Format, CmIndex& _Index)
{
	// check for empty vector
	if (0 == _Index.getLength()) {
		// write value and format to own scalar
		setScalar(_Value, _Format);
	}
	else {
		uint32 Index = _Index[_Index.getMaxIndex()];
		// delete string if one is attached
		if (DATAFORMAT_STRING == DataFormat[Index]) {
			CmString* String = (CmString*)CmVector<uint64>::operator[](Index);
			if (NULL != String) {
				CmString::releaseMemory<CmString>(String, 1, isCmString);
			}
			DataFormat[Index] = DATAFORMAT_NONE;
		}
		// check if a matrix is attached
		if (DATAFORMAT_MATRIX == DataFormat[Index] && NULL != CmVector<uint64>::operator[](Index)) {
			// write value and format to attached matrix's scalar
			CmMatrixFTL* MatrixFTL = (CmMatrixFTL*)CmVector<uint64>::operator[](Index);
			MatrixFTL->setScalar(_Value, _Format);
		}
		else {
			// set new value and format on indexed position
			CmVector<uint64>::operator[](Index) = _Value;
			DataFormat[Index] = _Format;
		}
	}

	return true;
}
bool CmMatrixFTL::getValue(uint64& _Value, CmDataFormat& _Format, CmIndex& _Index)
{
	// check for empty vector
	if (0 == _Index.getLength()) {
		// get value/format from own scalar
		getScalar(_Value, _Format);
	}
	else {
		int32 Index = _Index[_Index.getMaxIndex()];
		// validate index
		if (Index < 0) {
			_Value = 0;
			_Format = DATAFORMAT_NONE;
		}
		else
			// check if a matrix is attached
			if (DATAFORMAT_MATRIX == DataFormat[Index] && NULL != CmVector<uint64>::operator[](Index)) {
				// get value/format from matrix's scalar
				CmMatrixFTL* MatrixFTL = (CmMatrixFTL*)CmVector<uint64>::operator[](Index);
				MatrixFTL->getScalar(_Value, _Format);
			}
			else {
				// get value/format from indexed position
				_Value = CmVector<uint64>::operator[](Index);
				_Format = DataFormat[Index];
			}
	}

	return true;
}
CmMatrixFTL* CmMatrixFTL::getMatrixLastLevel(CmIndex& _Index)
{
	// retrieve before last level of dimension
	bool isCreateNextLevel = false;
	CmMatrixFTL* MatrixFTL = getBeforeLastLevel(_Index.reset(), isCreateNextLevel);
	if (NULL == MatrixFTL) return NULL;

	// get last index component
	int32 Index = _Index[_Index.getMaxIndex()];
	if (Index < 0) return NULL;
	// check if a matrix is attached
	if (DATAFORMAT_MATRIX != MatrixFTL->DataFormat[Index] || NULL == MatrixFTL->CmVector<uint64>::operator[](Index)) return NULL;

	// return matrix
	return (CmMatrixFTL*)MatrixFTL->CmVector<uint64>::operator[](Index);
}
bool CmMatrixFTL::resetNextIndex(CmIndex& _Index)
{
	// retrieve selected matrix
	CmMatrixFTL* MatrixFTL = getMatrixLastLevel(_Index);
	NULL == MatrixFTL ? MatrixFTL = this : 0;

	// reset CmVector's Next index
	MatrixFTL->reset();

	return true;
}
bool CmMatrixFTL::allItems(CmString& _Item, int32& _Position, CmIndex& _Index, bool _isMatchFormat)
{
	// initialize item
	_Item.clear();

	// retrieve selected matrix
	CmMatrixFTL* MatrixFTL = getMatrixLastLevel(_Index);
	NULL == MatrixFTL ? MatrixFTL = this : 0;

	// get selected item's value and format
	CmFlex Value; Value.uint64Value = 0;
	CmDataFormat Format = DATAFORMAT_NONE;
	CmIndex NextIndex(1);
	NextIndex[0] = _Position = MatrixFTL->getNextIndex();
	MatrixFTL->getValue(Value.uint64Value, Format, NextIndex);

	// check for string
	if (DATAFORMAT_NONE == Format || (DATAFORMAT_STRING != Format && _isMatchFormat)) return false;

	// return string
	if (DATAFORMAT_STRING == Format) {
		_Item = *(CmString*)Value.pointerValue;
	}
	else {
		// convert CmFlex value to a double string
		CmStringFTL Converter;
		Converter.flex2Double(Value, Format, _Item);
	}

	return true;
}
bool CmMatrixFTL::allValues(CmFlex& _Value, int32& _Position, CmIndex& _Index, CmDataFormat _Format, bool _isMatchFormat)
{
	// initialize return value
	_Value.uint64Value = 0;

	// retrieve selected matrix
	CmMatrixFTL* MatrixFTL = getMatrixLastLevel(_Index);
	NULL == MatrixFTL ? MatrixFTL = this : 0;

	// get selected item's value and format
	CmFlex Value;	Value.uint64Value = 0;
	CmDataFormat Format = DATAFORMAT_NONE;
	CmIndex NextIndex(1);
	NextIndex[0] = _Position = MatrixFTL->getNextIndex();
	MatrixFTL->getValue(Value.uint64Value, Format, NextIndex);

	// check format
	if (DATAFORMAT_NONE == Format || (_Format != Format && _isMatchFormat)) return false;

	if (_Format != Format) {
		// convert any format to double
		CmStringFTL Converter;
		if (DATAFORMAT_STRING == Format) {
			// convert CmString to double
			Value.doubleValue = Converter.flex2Double(Value, Format);
		}
		else {
			// convert CmFlex value to double
			Value.doubleValue = Converter.flex2Double(Value, Format);
		}
		// convert double to target format
		if (false == Converter.double2Flex(Value, _Format)) return false;
	}

	// return value
	_Value.uint64Value = Value.uint64Value;

	return true;
}



//----------------------------------------------------------------------------
// CmMatrix class
//----------------------------------------------------------------------------
//
CmMatrix::CmMatrix()
{

}
CmMatrix::~CmMatrix()
{

}

bool CmMatrix::clearMatrix()
{
	return CmMatrixFTL::clearMatrix(Index);
}

/** addressing matrix element by an index */
CmMatrix& CmMatrix::operator[](CmIndex& _Index)
{
	Index = _Index;
	return *this;
}
CmMatrix& CmMatrix::operator()() {
	Index.clear();
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a) {
	Index(_a);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b) {
	Index(_a, _b);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c) {
	Index(_a, _b, _c);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d) {
	Index(_a, _b, _c, _d);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e) {
	Index(_a, _b, _c, _d, _e);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f) {
	Index(_a, _b, _c, _d, _e, _f);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g) {
	Index(_a, _b, _c, _d, _e, _f, _g);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s, _t);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s, _t, _u);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s, _t, _u, _v);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v, int _w) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s, _t, _u, _v, _w);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v, int _w, int _x) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s, _t, _u, _v, _w, _x);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v, int _w, int _x, int _y) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s, _t, _u, _v, _w, _x, _y);
	return *this;
}
CmMatrix& CmMatrix::operator()(int _a, int _b, int _c, int _d, int _e, int _f, int _g, int _h, int _i, int _j, int _k, int _l, int _m, int _n, int _o, int _p, int _q, int _r, int _s, int _t, int _u, int _v, int _w, int _x, int _y, int _z) {
	Index(_a, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k, _l, _m, _n, _o, _p, _q, _r, _s, _t, _u, _v, _w, _x, _y, _z);
	return *this;
}

/** matrix write access */
bool CmMatrix::operator=(const CmString& _String) {
	return setString(_String, Index);
}
bool CmMatrix::operator=(const int8* _String) {
	return setString(_String, Index);
}
bool CmMatrix::operator=(void* _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(uint64 _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(int64 _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(double _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(uint32 _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(int32 _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(float _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(uint16 _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(int16 _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(uint8 _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(int8 _Value) {
	return setValue(_Value, Index);
}
bool CmMatrix::operator=(bool _Value) {
	return setValue(_Value, Index);
}

/** matrix read acces and subsequent type conversion */
CmMatrix::operator CmString() {
	return asString(Index);
}
CmMatrix::operator const int8* () {
	return asString(Index).getText();
}
CmMatrix::operator void* () {
	return (void*)uint64(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator uint64() {
	return uint64(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator int64() {
	return int64(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator double() {
	return CmMatrixFTL::operator[](Index);
}
CmMatrix::operator uint32() {
	return uint32(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator int32() {
	return int32(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator float() {
	return float(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator uint16() {
	return uint16(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator int16() {
	return int16(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator uint8() {
	return uint8(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator int8() {
	return int8(CmMatrixFTL::operator[](Index));
}
CmMatrix::operator bool() {
	return CmMatrixFTL::operator[](Index) == 0 ? false : true;
}

/** matrix functions */
int32 CmMatrix::getSizeLastIndexLevel()
{
	return CmMatrixFTL::getVectorLength(Index);
}
int32 CmMatrix::getSizeVariableIndexLevel(CmIndex& _Index)
{
	int32 VarItemPos = _Index.getScalar();
	Index.setLength(VarItemPos);
	for (int32 i = 0; i < VarItemPos; i++) {
		Index[i] = _Index[i];
	}
	return getSizeLastIndexLevel();
}
bool CmMatrix::addValue(int32& _Position, int32 _Value)
{
	int32 SizeLL = getSizeLastIndexLevel();
	int32 Levels = Index.getLength();
	int32 i;
	bool isAdded = false;
	for (i = 0; i < SizeLL; i++) {
		Index[Levels] = i;
		if (_Value == int32(operator[](Index))) {
			break;
		}
	}
	// insert new P1
	if (i >= SizeLL) {
		Index[Levels] = i;
		operator=(_Value);
		isAdded = true;
	}
	_Position = i;

	return isAdded;
}
bool CmMatrix::addValue(int32& _Position, int64 _Value)
{
	int32 SizeLL = getSizeLastIndexLevel();
	int32 Levels = Index.getLength();
	int32 i;
	bool isAdded = false;
	for (i = 0; i < SizeLL; i++) {
		Index[Levels] = i;
		if (_Value == int64(operator[](Index))) {
			break;
		}
	}
	// insert new P1
	if (i >= SizeLL) {
		Index[Levels] = i;
		operator=(_Value);
		isAdded = true;
	}
	_Position = i;

	return isAdded;
}
bool CmMatrix::addValueSorted(int32& _Position, int32 _Value)
{
	int32 SizeLL = getSizeLastIndexLevel();
	int32 Levels = Index.getLength();
	int32 i;
	int32 Val;
	bool isFound = false;
	bool isInserted = false;
	for (i = 0; i < SizeLL; i++) {
		Index[Levels] = i;
		// determine position for a properly sorted vector
		if (_Value <= (Val = int32(operator[](Index)))) {
			_Value == (Val = int32(operator[](Index))) ? isFound = true : 0;
			break;
		}
	}
	if ((i >= SizeLL) || (false == isFound)) {
		// insert new P1 at appropriate position for getting a properly sorted vector
		int32 j = SizeLL;
		while (j > i) {
			Index[Levels] = j - 1;
			Val = int32(operator[](Index));
			Index[Levels] = j;
			operator=(Val);
			j--;
		}
		Index[Levels] = i;
		operator=(_Value);
		isInserted = true;
	}
	_Position = i;

	return isInserted;
}
bool CmMatrix::addValueSorted(int32& _Position, int64 _Value)
{
	int32 SizeLL = getSizeLastIndexLevel();
	int32 Levels = Index.getLength();
	int32 i;
	int64 Val;
	bool isFound = false;
	bool isInserted = false;
	for (i = 0; i < SizeLL; i++) {
		Index[Levels] = i;
		// determine position for a properly sorted vector
		if (_Value <= (Val = int64(operator[](Index)))) {
			_Value == (Val = int64(operator[](Index))) ? isFound = true : 0;
			break;
		}
	}
	if ((i >= SizeLL) || (false == isFound)) {
		// insert new P1 at appropriate position for getting a properly sorted vector
		int32 j = SizeLL;
		while (j > i) {
			Index[Levels] = j - 1;
			Val = int64(operator[](Index));
			Index[Levels] = j;
			operator=(Val);
			j--;
		}
		Index[Levels] = i;
		operator=(_Value);
		isInserted = true;
	}
	_Position = i;

	return isInserted;
}
bool CmMatrix::appendItem(const int8* _Item, bool _isForceAppend)
{
	int32 Position;
	CmString Item(_Item);
	return appendItem(Position, Item, _isForceAppend);
}
bool CmMatrix::appendItem(const CmString& _Item, bool _isForceAppend)
{
	int32 Position;
	return appendItem(Position, _Item, _isForceAppend);
}
bool CmMatrix::appendItem(int32& _Position, const int8* _Item, bool _isForceAppend)
{
	CmString Item(_Item);
	return appendItem(_Position, Item, _isForceAppend);
}
bool CmMatrix::appendItem(int32& _Position, const CmString& _Item, bool _isForceAppend)
{
	int32 SizeLL = getSizeLastIndexLevel();
	int32 Levels = Index.getLength();
	int32 i = SizeLL;
	CmString Item;
	bool isInserted = false;
	if (false == _isForceAppend) {
		for (i = 0; i < SizeLL; i++) {
			Index[Levels] = i;
			Item = CmString(operator[](Index));
			// check if the item exists already
			if (_Item == Item) {
				_Position = i;
				break;
			}
		}
	}

	// insert new Item
	if (i >= SizeLL) {
		Index[Levels] = i;
		operator=(_Item);
		isInserted = true;
		_Position = i;
	}

	return isInserted;
}
int32 CmMatrix::getItemPos(CmIndex& _Index, const CmString& _Item)
{
	// iterate existing items
	int32 i;
	CmIndex I;
	I = _Index;
	int32 IndexLength = I.getLength();
	int32 VectorLength = getVectorLength(I);
	for (i = 0; i < VectorLength; i++) {
		I[IndexLength] = i;
		CmString Item = operator[](I);
		if (Item == _Item) return i;
	}
	// append new item
	I[IndexLength] = i;
	operator[](I) = _Item;

	return i;
}
CmMatrix& CmMatrix::findScalarItem(const CmString& _Item)
{
	// iterate existing items
	CmIndex I;
	int32 i;
	int32 VectorLength = getVectorLength(I(0));
	for (i = 0; i < VectorLength; i++) {
		CmString Item = operator()(i);
		if (Item == _Item) return operator()(i);
	}
	// insert new item
	operator()(i) = _Item;

	return operator()(i);
}
bool CmMatrix::findItem(int32& _ItemPos, CmIndex& _Index, const CmString& _Item)
{
	// initialize ItemPos
	_ItemPos = 0;
	// iterate existing items
	int32 i;
	CmIndex I;
	I = _Index;
	int32 IndexLength = I.getLength();
	int32 VectorLength = getVectorLength(I);
	for (i = 0; i < VectorLength; i++) {
		I[IndexLength] = i;
		CmString Item = operator[](I);
		if (Item == _Item) {
			_ItemPos = i;
			break;
		}
	}
	// check if item was found
	if (i >= VectorLength) return false;

	return true;
}
bool CmMatrix::insertItem(int32 /*_Position*/)
{
	// ToDo.

	return false;
}
bool CmMatrix::deleteItem(int32 /*_Position*/)
{
	// ToDo.

	return false;
}
bool CmMatrix::resetNextIndex()
{
	return CmMatrixFTL::resetNextIndex(Index);
}
bool CmMatrix::allItems(CmString& _Item, int32& _Position, bool _isMatchFormat)
{
	return CmMatrixFTL::allItems(_Item, _Position, Index, _isMatchFormat);
}
bool CmMatrix::allValues(uint64& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_UINT64, _isMatchFormat);
	_Value = Value.uint64Value;
	return Ret;
}
bool CmMatrix::allValues(int64& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_INT64, _isMatchFormat);
	_Value = Value.int64Value;
	return Ret;
}
bool CmMatrix::allValues(double& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_DOUBLE, _isMatchFormat);
	_Value = Value.doubleValue;
	return Ret;
}
bool CmMatrix::allValues(uint32& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_UINT32, _isMatchFormat);
	_Value = Value.uint32Value;
	return Ret;
}
bool CmMatrix::allValues(int32& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_INT32, _isMatchFormat);
	_Value = Value.int32Value;
	return Ret;
}
bool CmMatrix::allValues(float& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_FLOAT, _isMatchFormat);
	_Value = Value.floatValue;
	return Ret;
}
bool CmMatrix::allValues(uint16& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_UINT16, _isMatchFormat);
	_Value = Value.uint16Value;
	return Ret;
}
bool CmMatrix::allValues(int16& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_INT16, _isMatchFormat);
	_Value = Value.int16Value;
	return Ret;
}
bool CmMatrix::allValues(uint8& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_UINT8, _isMatchFormat);
	_Value = Value.uint8Value;
	return Ret;
}
bool CmMatrix::allValues(int8& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_INT8, _isMatchFormat);
	_Value = Value.int8Value;
	return Ret;
}
bool CmMatrix::allValues(bool& _Value, int32& _Position, bool _isMatchFormat)
{
	CmFlex Value;
	bool Ret = CmMatrixFTL::allValues(Value, _Position, Index, DATAFORMAT_BOOL, _isMatchFormat);
	_Value = Value.boolValue;
	return Ret;
}

//----------------------------------------------------------------------------
// CmValueFTL class
//----------------------------------------------------------------------------
//
CmValueFTL* CmValueFTL::ValuesFTL = NULL;

CmValueFTL::CmValueFTL(const int8* _Address)
{
	if (NULL == _Address) {
		Address = "0";
	}
	else if (strlen(_Address) == 0) {
		Address = "0";
	}
	else {
		Address = _Address;
	}

	char Addr[256];
	strcpy_s(Addr, sizeof(Addr), Address.getBuffer());

	// initialize list of values
	Next = NULL;
	if (Address.getLength() == 1) {
		// the IFTL will be the container for subsequent CmValueFTL
		ValuesFTL = this;
	}
	else if (NULL != ValuesFTL) {
		// insert into list of CmValueFTL
		CmValueFTL* ValueFTL = ValuesFTL;
		while (NULL != ValueFTL->Next) {
			ValueFTL = ValueFTL->Next;
		}
		ValueFTL->Next = this;
	}

	// reset root of linear CmValueFTL array
	Array = NULL;

	// reset root of a multidimensional CmMatrixFTL matrix
	Matrix = NULL;
}
CmValueFTL::CmValueFTL(CmTypeFTL _TypeFTL)
{
	// set type
	TypeFTL = _TypeFTL;

	// initialize workspace
	Address = "0";
	Next = NULL;
	Array = NULL;
	Matrix = NULL;
	Remainder = "0";
}
CmValueFTL::~CmValueFTL()
{
	// cleanup workspace
	releaseMemory<CmMatrix>(Matrix, 1, isCmMatrix);
}

// action test function
const int32 ActionTestVal = 12345;
typedef int32(*Action)(void);
int32 action(void) { return ActionTestVal; }

bool CmValueFTL::testCmValueFTL()
{
	bool Result = true;
	printf("\n");

	// test format conversion
	CmString TestString("67890.12345");
	double TestValue;
	sscanf_s(TestString.getText(), "%lf", &TestValue);
	const uint64 TestUInt64 = (uint64)TestValue;
	const int64 TestInt64 = (int64)TestValue;
	const double TestDouble = (double)TestValue;
	const uint32 TestUInt32 = (uint32)TestValue;
	const int32 TestInt32 = (int32)TestValue;
	const float TestFloat = (float)TestValue;
	const uint16 TestUInt16 = (uint16)TestValue;
	const int16 TestInt16 = (int16)TestValue;
	const uint8 TestUInt8 = (uint8)TestValue;
	const int8 TestInt8 = (int8)TestValue;
	const bool TestBool = true;

	// check particular formats
	CmValueFTL Val;
	Val = TestDouble;
	if (TestDouble != double(Val)) { printf("\n 'double' failed: %.5f != %.5f", TestDouble, double(Val)); Result = false; }
	if (TestString != Val.asStringConvert(5)) { printf("\n 'CmString' failed: %s != %s", TestString.getText(), Val.asStringConvert().getText()); Result = false; }
	// NOTE: conversion from any format to 'uint64' is not supported (cannot be done exactly using 'double' as an intermediate format, CPU/compiler problems?)
	if (TestUInt64 == uint64(Val)) { printf("\n 'uint64' failed: %llu != %llu", TestUInt64, uint64(Val)); Result = false; }
	if (TestInt64 != int64(Val)) { printf("\n 'int64' failed: %lld != %lld", TestInt64, int64(Val)); Result = false; }
	if (TestUInt32 != uint32(Val)) { printf("\n 'uint32' failed: %u != %u", TestUInt32, uint32(Val)); Result = false; }
	if (TestInt32 != int32(Val)) { printf("\n 'int32' failed: %d != %d", TestInt32, int32(Val)); Result = false; }
	if (TestFloat != float(Val)) { printf("\n 'float' failed: %f != %f", TestFloat, float(Val)); Result = false; }
	if (TestUInt16 != uint16(Val)) { printf("\n 'uint16' failed: %u != %u", TestUInt16, uint16(Val)); Result = false; }
	if (TestInt16 != int16(Val)) { printf("\n 'int16' failed: %d != %d", TestInt16, int16(Val)); Result = false; }
	if (TestUInt8 != uint8(Val)) { printf("\n 'uint8' failed: %u != %u", TestUInt8, uint8(Val)); Result = false; }
	if (TestInt8 != int8(Val)) { printf("\n 'int8' failed: %d != %d", TestInt8, int8(Val)); Result = false; }
	if (TestBool != bool(Val)) { printf("\n 'bool' failed: %s != %s", TestBool ? "true" : "false", bool(Val) ? "true" : "false"); Result = false; }
	// re-check initial 'double' value
	if (TestDouble != double(Val)) { printf("\n 'double' failed: %.5f != %.5f", TestDouble, double(Val)); Result = false; }
	// check string with length in format specification
	CmString TestLength(" ");
	TestLength += TestString;
	if (TestLength != Val.asStringConvert(5, 12)) { printf("\n 'CmString' failed: %s != %s", TestLength.getText(), Val.asStringConvert().getText()); Result = false; }
	// (1) check 'uint64' conversion
	uint64 TestUInt64_1 = 0xFFFFFFFFFFFFFFFF;
	const double TestDouble_1 = (double)TestUInt64_1;
	Val = TestUInt64_1;
	if (TestUInt64_1 != uint64(Val)) { printf("\n 'uint64(1)' failed: %llu != %llu", TestUInt64_1, uint64(Val)); Result = false; }
	if (TestDouble_1 != double(Val)) { printf("\n 'double(1)' failed: %.5f != %.5f", TestDouble_1, double(Val)); Result = false; }
	// (2) check 'uint32' conversion
	uint32 TestUInt32_2 = 0xFFFFFFFF;
	CmString TestString_2("4294967295");
	const double TestDouble_2 = (double)TestUInt32_2;
	Val = TestUInt32_2;
	if (TestUInt32_2 != uint32(Val)) { printf("\n 'uint32(2)' failed: %lu != %lu", TestUInt32_2, uint32(Val)); Result = false; }
	if (TestDouble_2 != double(Val)) { printf("\n 'double(2)' failed: %.0f != %.0f", TestDouble_2, double(Val)); Result = false; }
	if (TestString_2 != Val.asStringConvert()) { printf("\n 'CmString(2)' failed: %s != %s", TestString_2.getText(), Val.asStringConvert().getText()); Result = false; }
	if (TestUInt32_2 != uint32(Val)) { printf("\n 'uint32(2)' failed: %lu != %lu", TestUInt32_2, uint32(Val)); Result = false; }
	// (3) check 'float' conversion
	float TestFloat_3 = TestFloat;
	const double TestDouble_3 = (double)TestFloat_3;
	Val = TestFloat_3;
	if (TestFloat_3 != float(Val)) { printf("\n 'float(3)' failed: %f != %f", TestFloat_3, float(Val)); Result = false; }
	if (TestDouble_3 != double(Val)) { printf("\n 'double(3)' failed: %f != %f", TestDouble_3, double(Val)); Result = false; }
	if (TestFloat_3 != float(Val)) { printf("\n 'float(3)' failed: %f != %f", TestFloat_3, float(Val)); Result = false; }

	// test mixed conversions
	Val.setText(TestString);
	if (TestInt64 != int64(Val)) { printf("\n 'int64' failed: %d != %d", int32(TestInt64), int32(Val)); Result = false; }
	if (TestInt32 != int32(Val)) { printf("\n 'int32' failed: %d != %d", TestInt32, int32(Val)); Result = false; }
	if (TestInt16 != int16(Val)) { printf("\n 'int16' failed: %d != %d", TestInt16, int16(Val)); Result = false; }
	if (TestInt8 != int8(Val)) { printf("\n 'int8'  failed: %d != %d", TestInt8, int8(Val)); Result = false; }
	if (TestUInt32 != uint32(Val)) { printf("\n 'uint32' failed: %d != %d", TestUInt32, uint32(Val)); Result = false; }
	if (TestUInt16 != uint16(Val)) { printf("\n 'uint16' failed: %d != %d", TestUInt16, uint32(Val)); Result = false; }
	if (TestUInt8 != uint8(Val)) { printf("\n 'uint8'  failed: %d != %d", TestUInt8, uint32(Val)); Result = false; }
	if (TestFloat != float(Val)) { printf("\n 'float' failed: %f != %f", TestFloat, float(Val)); Result = false; }
	if (TestDouble_3 != double(Val)) { printf("\n 'double(3)' failed: %f != %f", TestDouble_3, double(Val)); Result = false; }

	//
	// test data formats retrieved from a FTLight structure
	// 
#define TEST_DATA_FORMAT "EKD@JN58nc_Türkenfeld.CmStringFTL,0\n\
,UInt64:0x1234567890abCDef\n\
,Int64:-1234567890123456\n\
,Double:0.1234567890123456\n\
,Double0:0.0000000000000003\n\
,Double8:0.12345678\n\
,Double10:10.12345678\n\
,Double15:12345678901234.5\n\
,UInt32:0x1234abCD\n\
,Int32:-1234567890\n\
,Float:0.1234567\n\
,Float0:0.0000001\n\
,Float2:0.53\n\
,Float4:0.1234\n\
,Float7:123.4567\n\
,UInt16:0x12ab\n\
,Int16:-32768\n\
,UInt8:0xFF\n\
,Int8:-128\n\
,False:00\n\
,True:11\n\
"
	CmString StringFTL;
	CmStringFTL FormatFTL;
	FormatFTL.processStringFTL(TEST_DATA_FORMAT);
	// IFTL
	CmValueFTL FTL_IFTL("0");
	if (false == FTL_IFTL.updateFrom(FormatFTL)) return false;
	if (CmString(FTL_IFTL) != CmString("EKD@JN58nc_Türkenfeld.CmStringFTL")) return false;
	// uint64
	CmValueFTL FTL_uint64("0-1-0");
	if (false == FTL_uint64.updateFrom(FormatFTL)) return false;
	if (0x1234567890abcdef != uint64(FTL_uint64)) return false;
	// int64
	CmValueFTL FTL_int64("0-2-0");
	if (false == FTL_int64.updateFrom(FormatFTL)) return false;
	if (-1234567890123456 != int64(FTL_int64)) return false;
	StringFTL = FTL_int64.asStringFTL(""); if (StringFTL != "-1234567890123456") return false;
	// double
	CmValueFTL FTL_double("0-3-0");
	if (false == FTL_double.updateFrom(FormatFTL)) return false;
	if (0.1234567890123456 != double(FTL_double)) return false;
	StringFTL = FTL_double.asStringFTL(""); if (StringFTL != "0.1234567890123456") return false;
	// double 0
	CmValueFTL FTL_double0("0-4-0");
	if (false == FTL_double0.updateFrom(FormatFTL)) return false;
	if (0.0000000000000003 != double(FTL_double0)) return false;
	StringFTL = FTL_double0.asStringFTL(""); if (StringFTL != "0.0000000000000003") return false;
	// double 8
	CmValueFTL FTL_double8("0-5-0");
	if (false == FTL_double8.updateFrom(FormatFTL)) return false;
	if (0.12345678 != double(FTL_double8)) return false;
	StringFTL = FTL_double8.asStringFTL(""); if (StringFTL != "0.12345678") return false;
	// double 10
	CmValueFTL FTL_double10("0-6-0");
	if (false == FTL_double10.updateFrom(FormatFTL)) return false;
	if (10.12345678 != double(FTL_double10)) return false;
	StringFTL = FTL_double10.asStringFTL(""); if (StringFTL != "10.12345678") return false;
	// double 15
	CmValueFTL FTL_double15("0-7-0");
	if (false == FTL_double15.updateFrom(FormatFTL)) return false;
	if (12345678901234.5 != double(FTL_double15)) return false;
	StringFTL = FTL_double15.asStringFTL(""); if (StringFTL != "12345678901234.5") return false;
	// uint32
	CmValueFTL FTL_uint32("0-8-0");
	if (false == FTL_uint32.updateFrom(FormatFTL)) return false;
	if (0x1234abcd != uint32(FTL_uint32)) return false;
	// int32
	CmValueFTL FTL_int32("0-9-0");
	if (false == FTL_int32.updateFrom(FormatFTL)) return false;
	if (-1234567890 != int32(FTL_int32)) return false;
	StringFTL = FTL_int32.asStringFTL(""); if (StringFTL != "-1234567890") return false;
	// float
	CmValueFTL FTL_float("0-10-0");
	if (false == FTL_float.updateFrom(FormatFTL)) return false;
	if (fabs(0.1234567 - float(FTL_float)) > 0.00000005) return false;
	StringFTL = FTL_float.asStringFTL(""); if (StringFTL != "0.1234567") return false;
	// float 0
	CmValueFTL FTL_float0("0-11-0");
	if (false == FTL_float0.updateFrom(FormatFTL)) return false;
	if (fabs(0.0000001 - float(FTL_float0)) > 0.00000005) return false;
	StringFTL = FTL_float0.asStringFTL(""); if (StringFTL != "0.0000001") return false;
	// float 2
	CmValueFTL FTL_float2("0-12-0");
	if (false == FTL_float2.updateFrom(FormatFTL)) return false;
	if (fabs(0.53 - float(FTL_float2)) > 0.005) return false;
	StringFTL = FTL_float2.asStringFTL(""); if (StringFTL != "0.53") return false;
	// float 4
	CmValueFTL FTL_float4("0-13-0");
	if (false == FTL_float4.updateFrom(FormatFTL)) return false;
	if (fabs(0.1234 - float(FTL_float4)) > 0.00005) return false;
	StringFTL = FTL_float4.asStringFTL(""); if (StringFTL != "0.1234") return false;
	// float 7
	CmValueFTL FTL_float7("0-14-0");
	if (false == FTL_float7.updateFrom(FormatFTL)) return false;
	if (fabs(123.4567 - float(FTL_float7)) > 0.00005) return false;
	StringFTL = FTL_float7.asStringFTL(""); if (StringFTL != "123.4567") return false;
	// uint16
	CmValueFTL FTL_uint16("0-15-0");
	if (false == FTL_uint16.updateFrom(FormatFTL)) return false;
	if (0x12ab != uint16(FTL_uint16)) return false;
	// int16
	CmValueFTL FTL_int16("0-16-0");
	if (false == FTL_int16.updateFrom(FormatFTL)) return false;
	if (-32768 != int16(FTL_int16)) return false;
	// uint8
	CmValueFTL FTL_uint8("0-17-0");
	if (false == FTL_uint8.updateFrom(FormatFTL)) return false;
	if (0xFF != uint8(FTL_uint8)) return false;
	// int8
	CmValueFTL FTL_int8("0-18-0");
	if (false == FTL_int8.updateFrom(FormatFTL)) return false;
	if (-128 != int8(FTL_int8)) return false;
	// bool: false
	CmValueFTL FTL_false("0-19-0");
	if (false == FTL_false.updateFrom(FormatFTL)) return false;
	if (false != bool(FTL_false)) return false;
	// bool: true
	CmValueFTL FTL_true("0-20-0");
	if (false == FTL_true.updateFrom(FormatFTL)) return false;
	if (true != bool(FTL_true)) return false;

	//---test matrix formats
	CmIndex I;
	CmMatrix M = Val.getMatrix();
	I(1, 2, 3);
	// string
	M[I] = TestString;
	if (CmString(M[I]) != TestString) Result = false;
	// uint64
	M[I] = TestUInt64;
	if (uint64(M[I]) != TestUInt64) Result = false;
	// int64
	M[I] = TestInt64;
	if ((int64)M[I] != TestInt64) Result = false;
	// double
	M[I] = TestDouble;
	if ((double)M[I] != TestDouble) Result = false;
	// uint32
	M[I] = TestUInt32;
	if ((uint32)M[I] != TestUInt32) Result = false;
	// int32
	M[I] = TestInt32;
	if ((int32)M[I] != TestInt32) Result = false;
	// float
	M[I] = TestFloat;
	if ((float)M[I] != TestFloat) Result = false;
	// uint16
	M[I] = TestUInt16;
	if ((uint16)M[I] != TestUInt16) Result = false;
	// int16
	M[I] = TestInt16;
	if ((int16)M[I] != TestInt16) Result = false;
	// uint8
	M[I] = TestUInt8;
	if ((uint8)M[I] != TestUInt8) Result = false;
	// int8
	M[I] = TestInt8;
	if ((int8)M[I] != TestInt8) Result = false;
	// bool
	M[I] = TestBool;
	if ((bool)M[I] != (TestBool ? 1 : 0)) Result = false;

	//---test getVectorLength() 
	if (2 != M().getSizeLastIndexLevel()) Result = false;
	if (3 != M(1).getSizeLastIndexLevel()) Result = false;
	if (4 != M(1, 2).getSizeLastIndexLevel()) Result = false;
	if (0 != M(1, 2, 3).getSizeLastIndexLevel()) Result = false;
	if (0 != M(1, 2, 3, 4).getSizeLastIndexLevel()) Result = false;

	// test add
	int32 Position;
	M().clearMatrix();
	M().addValue(Position, 1); if (Position != 0) Result = false;
	M().addValue(Position, 3); if (Position != 1) Result = false;
	M().addValue(Position, 2); if (Position != 2) Result = false;
	M().addValue(Position, 4); if (Position != 3) Result = false;
	M().addValue(Position, 3); if (Position != 1) Result = false;

	// test add sorted
	M().clearMatrix();
	M().addValueSorted(Position, 1); if (Position != 0) Result = false;
	M().addValueSorted(Position, 3); if (Position != 1) Result = false;
	M().addValueSorted(Position, 2); if (Position != 1) Result = false;
	M().addValueSorted(Position, 4); if (Position != 3) Result = false;
	M().addValueSorted(Position, 3); if (Position != 2) Result = false;

	// test action function pointer on matrix scalar position
	M() = &action;
	Action TestAction = (Action)(void*)M();
	int32 TestVal = (*TestAction)(); if (TestVal != ActionTestVal) Result = false;

	// done with matrix
	Val.leaveMatrix();

	//--test bignum operations
	const uint64 ValA = 4785604381;
	const uint64 ValB = 2175489273;
	const uint64 ValD = 14856043819707253914;
	uint64 ValC;
	uint64 ValR;
	uint64 SumR = ValA + ValB;
	uint64 DifR = ValA - ValB;
	uint64 MulR = ValA * ValB;
	uint64 DivR = ValD / ValB;
	uint64 RemR = ValD % ValB;
	CmValueFTL A(TYPEFTL_NUMBER);
	CmValueFTL B(TYPEFTL_NUMBER);
	CmValueFTL C(TYPEFTL_NUMBER);
	CmValueFTL D(TYPEFTL_NUMBER);
	CmString R;
	A.uint2String(ValA);
	B.uint2String(ValB);
	D.uint2String(ValD);

	// runtime
	CmTimestamp Time;
	Time.startRuntime();
	const char* MsgInit = "\n runtime[%d] %s %s %10.1f us";
	const char* Info = "64-bit                     ";
	int32 Loops = 1000;

	// sum
	for (int32 i = 0; i < Loops; i++) { R = A + B; }
	ValR = R.getNumAsUint64();
	if (ValR != SumR) Result = false;
	printf(MsgInit, Loops, "A + B", Info, (double)Time.getRuntime_us() / Loops);
	if (ValR != SumR) printf(" failed");
	// sum=
	for (int32 i = 0; i < Loops; i++) { C.setNumber(A); C += B; }
	ValC = C.getNumAsUint64();
	if (ValC != SumR) Result = false;
	printf(MsgInit, Loops, "C +=A", Info, (double)Time.getRuntime_us() / Loops);
	if (ValC != SumR) printf(" failed");
	// dif
	for (int32 i = 0; i < Loops; i++) { R = A - B; }
	ValR = R.getNumAsUint64();
	if (ValR != DifR) Result = false;
	printf(MsgInit, Loops, "A - B", Info, (double)Time.getRuntime_us() / Loops);
	if (ValR != DifR) printf(" failed");
	for (int32 i = 0; i < Loops; i++) { R = B - A; }
	R.assignSubString(int32(R.getLength()) - 1, 0, 1);
	ValR = R.getNumAsUint64();
	if (ValR != DifR) Result = false;
	printf(MsgInit, Loops, "B - A", Info, (double)Time.getRuntime_us() / Loops);
	if (ValR != DifR) printf(" failed");
	// dif=
	for (int32 i = 0; i < Loops; i++) { C.setNumber(A); C -= B; }
	ValC = C.getNumAsUint64();
	if (ValC != DifR) Result = false;
	printf(MsgInit, Loops, "C -=A", Info, (double)Time.getRuntime_us() / Loops);
	if (ValC != DifR) printf(" failed");
	// mul
	for (int32 i = 0; i < Loops; i++) { R = A * B; }
	ValR = R.getNumAsUint64();
	if (ValR != MulR) Result = false;
	printf(MsgInit, Loops, "A * B", Info, (double)Time.getRuntime_us() / Loops);
	if (ValR != MulR) printf(" failed");
	// mul=
	for (int32 i = 0; i < Loops; i++) { C.setNumber(A); C *= B; }
	ValC = C.getNumAsUint64();
	if (ValC != MulR) Result = false;
	printf(MsgInit, Loops, "C *=A", Info, (double)Time.getRuntime_us() / Loops);
	if (ValC != MulR) printf(" failed");
	// div
	for (int32 i = 0; i < Loops; i++) { R = D / B; }
	ValR = R.getNumAsUint64();
	if (ValR != DivR) Result = false;
	printf(MsgInit, Loops, "D / B", Info, (double)Time.getRuntime_us() / Loops);
	if (ValR != DivR) printf(" failed");
	// div
	for (int32 i = 0; i < Loops; i++) { C.setNumber(D); C /= B; }
	ValC = C.getNumAsUint64();
	if (ValC != DivR) Result = false;
	printf(MsgInit, Loops, "D /=B", Info, (double)Time.getRuntime_us() / Loops);
	if (ValC != DivR) printf(" failed");
	// rem
	for (int32 i = 0; i < Loops; i++) { R = D % B; }
	ValR = R.getNumAsUint64();
	if (ValR != RemR) Result = false;
	printf(MsgInit, Loops, "D % B", Info, (double)Time.getRuntime_us() / Loops);
	if (ValR != RemR) printf(" failed");
	printf("\n");

	// generate bignum
	int32 Chunks = 10;
	CmString BigA;
	CmString BigB;
	CmString BigD;
	for (int32 i = 0; i < Chunks; i++) {
		BigA += CmString(A);
		BigB += CmString(B);
		BigD += CmString(A);
		BigD += CmString(B);
	}
	A.setNumber(BigA);
	B.setNumber(BigB);
	D.setNumber(BigD);
	int32 Digits = int32(A.getLength() + B.getLength());
	Info = "bignum 100 digits          ";

	Loops = 100;
	// sum
	for (int32 i = 0; i < Loops; i++) { R = A + B; }
	printf(MsgInit, Loops, " A + B", Info, (double)Time.getRuntime_us() / Loops);
	// dif
	for (int32 i = 0; i < Loops; i++) { R = A - B; }
	printf(MsgInit, Loops, " A - B", Info, (double)Time.getRuntime_us() / Loops);
	for (int32 i = 0; i < Loops; i++) { R = B - A; }
	printf(MsgInit, Loops, " B - A", Info, (double)Time.getRuntime_us() / Loops);
	// mul
	Loops = 10;
	for (int32 i = 0; i < Loops; i++) { R = A * B; }
	printf(MsgInit, Loops, "  A * B", Info, (double)Time.getRuntime_us() / Loops);
	// div
	Loops = 10;
	for (int32 i = 0; i < Loops; i++) { R = D / B; }
	printf(MsgInit, Loops, "  D / B", Info, (double)Time.getRuntime_us() / Loops);
	printf("\n");

	Chunks = 24;
	A.uint2String(ValA);
	B.uint2String(ValB);
	BigA.setLength(0);
	BigB.setLength(0);
	BigD.setLength(0);
	for (int32 i = 0; i < Chunks; i++) {
		BigA += CmString(A);
		BigB += CmString(B);
		BigD += CmString(A);
		BigD += CmString(B);
	}
	A.setNumber(BigA);
	B.setNumber(BigB);
	D.setNumber(BigD);
	Digits = int32(A.getLength() + B.getLength());
	Info = "bignum 240 digits          ";

	Loops = 100;
	// sum
	for (int32 i = 0; i < Loops; i++) { R = A + B; }
	printf(MsgInit, Loops, " A + B", Info, double(Time.getRuntime_us()) / Loops);
	// dif
	for (int32 i = 0; i < Loops; i++) { R = A - B; }
	printf(MsgInit, Loops, " A - B", Info, double(Time.getRuntime_us()) / Loops);
	for (int32 i = 0; i < Loops; i++) { R = B - A; }
	printf(MsgInit, Loops, " B - A", Info, double(Time.getRuntime_us()) / Loops);
	// mul
	Loops = 10;
	for (int32 i = 0; i < Loops; i++) { R = A * B; }
	printf(MsgInit, Loops, "  A * B", Info, double(Time.getRuntime_us()) / Loops);
	// div
	Loops = 10;
	for (int32 i = 0; i < Loops; i++) { R = D / B; }
	printf(MsgInit, Loops, "  D / B", Info, double(Time.getRuntime_us()) / Loops);
	printf("\n");

	// RSA numbers
	int32 Count = 7;
	CmValueFTL RSA;
	CmValueFTL P;
	CmValueFTL Q;

	for (int32 i = 0; i < Count; i++) {
		switch (i) {
		case 0:	Info = "  RSA-100  330_bits  ";
			RSA.setNumber("1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139");
			P.setNumber("37975227936943673922808872755445627854565536638199");
			Q.setNumber("40094690950920881030683735292761468389214899724061");
			break;
		case 1:	Info = "  RSA-130  430_bits  ";
			RSA.setNumber("1807082088687404805951656164405905566278102516769401349170127021450056662540244048387341127590812303371781887966563182013214880557");
			P.setNumber("39685999459597454290161126162883786067576449112810064832555157243");
			Q.setNumber("45534498646735972188403686897274408864356301263205069600999044599");
			break;
		case 2:	Info = "  RSA-150  496_bits  ";
			RSA.setNumber("155089812478348440509606754370011861770654545830995430655466945774312632703463465954363335027577729025391453996787414027003501631772186840890795964683");
			P.setNumber("348009867102283695483970451047593424831012817350385456889559637548278410717");
			Q.setNumber("445647744903640741533241125787086176005442536297766153493419724532460296199");
			break;
		case 3:	Info = "  RSA-174  576_bits  ";
			RSA.setNumber("188198812920607963838697239461650439807163563379417382700763356422988859715234665485319060606504743045317388011303396716199692321205734031879550656996221305168759307650257059");
			P.setNumber("398075086424064937397125500550386491199064362342526708406385189575946388957261768583317");
			Q.setNumber("472772146107435302536223071973048224632914695302097116459852171130520711256363590397527");
			break;
		case 4:	Info = "  RSA-200  663_bits  ";
			RSA.setNumber("27997833911221327870829467638722601621070446786955428537560009929326128400107609345671052955360856061822351910951365788637105954482006576775098580557613579098734950144178863178946295187237869221823983");
			P.setNumber("3532461934402770121272604978198464368671197400197625023649303468776121253679423200058547956528088349");
			Q.setNumber("7925869954478333033347085841480059687737975857364219960734330341455767872818152135381409304740185467");
			break;
		case 5:	Info = "  RSA-230  762_bits  ";
			RSA.setNumber("17969491597941066732916128449573246156367561808012600070888918835531726460341490933493372247868650755230855864199929221814436684722874052065257937495694348389263171152522525654410980819170611742509702440718010364831638288518852689");
			P.setNumber("3968132623150957588532394439049887341769533966621957829426966084093049516953598120833228447171744337427374763106901");
			Q.setNumber("4528450358010492026612439739120166758911246047493700040073956759261590397250033699357694507193523000343088601688589");
			break;
		case 6:	Info = "  RSA-232  768_bits  ";
			RSA.setNumber("1230186684530117755130494958384962720772853569595334792197322452151726400507263657518745202199786469389956474942774063845925192557326303453731548268507917026122142913461670429214311602221240479274737794080665351419597459856902143413");
			P.setNumber("33478071698956898786044169848212690817704794983713768568912431388982883793878002287614711652531743087737814467999489");
			Q.setNumber("36746043666799590428244633799627952632279158164343087642676032283815739666511279233373417143396810270092798736308917");
			break;
		case 7:	Info = "  RSA-470 1559_bits  ";
			RSA.setNumber("17051473784681185209081599238887028025183255852149159683588918369809675398036897711442383602526314519192366612270595815510311970886116763177669964411814095748660238871306469830461919135901638237924444074122866545522954536883748558744552128950445218096208188788876324395049362376806579941053305386217595984047709603954312447692725276887594590658792939924609261264788572032212334726855302571883565912645432522077138010357669555555071044090857089539320564963576770285413369");
			P.setNumber("3847538028593018472389908148350764205843210893478047089347180907511475367987843270908432737486765382147094348974217195367100435478567510397658349192715687404381714314578910938001438475193740863923847081748561931007453847100368970425127");
			Q.setNumber("4756842193475028547381937805637418507649354718093471513790852479097284019785319708523419734364718529043250897946518734805247385195493874049510890384576810894713854903815147089384708917138475910189580381197082438147053740853420198038475");
			break;

		default:
			break;
		}

		for (int32 i = 0; i < Loops; i++) { R = P * Q; }
		printf(MsgInit, Loops, Info, "P * Q        ", double(Time.getRuntime_us()) / Loops);
		if (R != RSA) { printf(" failed"); Result = false; }
		for (int32 i = 0; i < Loops; i++) { R = RSA / P; }
		printf(MsgInit, Loops, Info, "RSA/P        ", double(Time.getRuntime_us()) / Loops);
		if (R != Q) { printf(" failed"); Result = false; }
		printf("\n");
	}

	// remove conversion tables
	clearConversionTable();

	return Result;
}

CmString& CmValueFTL::getLabel()
{
	// return Address as label (CmValueINI will return an actual Label)
	return Address;
}

const CmString& CmValueFTL::getAddress()
{
	return Address;
}

CmValueFTL& CmValueFTL::getNextConfigValue(bool& _isEndOfConfigValues)
{
	_isEndOfConfigValues = NULL == Next ? true : NULL == Next->Next ? true : false;
	return NULL == Next ? *this : *Next;
}

CmValueFTL* CmValueFTL::getConfigValue(int32& _Index)
{
	CmValueFTL* ValueFTL = this;
	while (NULL != ValueFTL->Next && _Index > 0) {
		ValueFTL = ValueFTL->Next;
		_Index--;
	}

	return ValueFTL;
}

bool CmValueFTL::allValuesFTL(CmValueFTL** _ValueFTL)
{
	*_ValueFTL = Next;

	return NULL == *_ValueFTL ? false : true;
}

bool CmValueFTL::updateSettings(CmStringFTL& _StringFTL, CmString _StartAddr)
{
	// travel the list of contained values
	CmValueFTL* ValueFTL = this;
	do {
		// update value from StringFTL
		if (false == ValueFTL->updateFrom(_StringFTL, _StartAddr)) {
			// address missing in StringFTL
			CmString Message("Settings failed with address '");
			Message += ValueFTL->getAddress();
			Message += "'.";
			throw CmException((void*)CMLOG_Error, Message.getText(), getText());
		}
		// next value
		ValueFTL = ValueFTL->Next;
	} while (NULL != ValueFTL);

	return true;
}
bool CmValueFTL::updateFrom(CmStringFTL& _StringFTL, CmString _StartAddr)
{
	// update value from specified address
	return _StringFTL.getValueFTL(Address, *this, _StartAddr);
}
bool CmValueFTL::updateFrom(CmStringFTL& _StringFTL, int32 _Index, CmString _StartAddr)
{
	// update value from specified address plus index
	CmString _Address = Address;
	_Address += "-";
	_Address += _Index;
	return _StringFTL.getValueFTL(_Address, *this, _StartAddr);
}

bool CmValueFTL::syncSettings(CmStringFTL& _StringFTL)
{
	// travel the list of contained values
	CmValueFTL* pValueFTL = this;
	do {
		// synchronize StringFTL with current value
		CmString ValueFTL(pValueFTL->asStringFTL(""));
		if (false == _StringFTL.setValueFTL(pValueFTL->Address, ValueFTL)) {
			// address missing in StringFTL
			CmString Message("Settings failed with address '");
			Message += pValueFTL->getAddress();
			Message += "'.";
			throw CmException((void*)CMLOG_Error, Message.getText(), getText());
		}
		// next value
		pValueFTL = pValueFTL->Next;
	} while (NULL != pValueFTL);

	return true;
}

bool CmValueFTL::updateInfoFTL(CmStringFTL& _InfoFTL, CmValueFTL& _Return, CmString _StartAddr)
{
	// get return values
	CmValueFTL& _LogLevel = _Return.getLogLevel();
	CmValueFTL& _Message = _Return.getMessage();
	CmValueFTL& _Context = _Return.getContext();

	// check for pending error
	if (int32(_LogLevel) == CMLOG_Error) return false;
	_LogLevel = CMLOG_None;

	try {
		updateSettings(_InfoFTL, _StartAddr);
	}
	catch (CmException e) {
		_LogLevel = uint64(e.getData());
		_Message.setText(e.getMessage());
		_Context.setText(e.getContext());
		return false;
	}

	return true;
}

bool CmValueFTL::serializeValueFTL(CmString& _StringFTL, CmStringFTL& _TemplateFTL, CmValueINI& _Return)
{
	// get return values
	CmValueFTL& _LogLevel = _Return.getLogLevel();
	CmValueFTL& _Message = _Return.getMessage();
	CmValueFTL& _Context = _Return.getContext();

	// synchronize _TemplateFTL with the control ValueFTL array
	try {
		syncSettings(_TemplateFTL);
	}
	catch (CmException e) {
		_LogLevel = uint64(e.getData());
		_Message.setText(e.getMessage());
		_Context.setText(e.getContext());
		return false;
	}

	// generate StringFTL
	if (false == _TemplateFTL.serializeStringFTL(_StringFTL)) return false;

	return true;
}


bool CmValueFTL::writeInfoFTL(CmString _ConfigPath, CmStringFTL& _InfoFTL, CmValueINI& _Return, const int8* _NameExtension)
{
	// get return values
	CmValueFTL& _LogLevel = _Return.getLogLevel();
	CmValueFTL& _Message = _Return.getMessage();
	CmValueFTL& _Context = _Return.getContext();

	// check for pending error
	if (int32(_LogLevel) == CMLOG_Error) return false;
	_LogLevel = CMLOG_None;

	// check if config path exists
	if (false == PathFileExistsA(_ConfigPath.getText())) {
		_LogLevel = CMLOG_Error;
		_Message.setText(_ConfigPath);
		_Context.setText("Config path not available (could not be created?):");
		return false;
	}

	// complete config filename
	CmString ConfigFile(_ConfigPath);
	CmString ConfigFileExtended;
	CmIFTL IFTL;
	IFTL = getText();
	ConfigFile += IFTL.getConfig();
	if (_NameExtension != NULL) {
		ConfigFileExtended = ConfigFile;
		ConfigFileExtended += "_";
		ConfigFileExtended += _NameExtension;
		ConfigFileExtended += ".ftl";
	}
	ConfigFile += ".ftl";

	// synchronize InfoFTL with Profile values
	try {
		syncSettings(_InfoFTL);
	}
	catch (CmException e) {
		_LogLevel = uint64(e.getData());
		_Message.setText(e.getMessage());
		_Context.setText(e.getContext());
		return false;
	}

	// generate config string from profile
	CmString ConfigString;
	_InfoFTL.serializeStringFTL(ConfigString);

	// write application profile to disk
	if (false == ConfigString.writeBinary(ConfigFile.getText())) {
		_LogLevel = CMLOG_Error;
		_Message.setText(_ConfigPath);
		_Context.setText("Config file could not be written:");
	}
	if (ConfigFileExtended.getLength() > 0) {
		if (false == ConfigString.writeBinary(ConfigFileExtended.getText())) {
			_LogLevel = CMLOG_Error;
			_Message.setText(_ConfigPath);
			_Context.setText("Config file (extended) could not be written:");
		}
	}

	// wait for folder synchronisation
	Sleep(100);
	return true;
}
bool CmValueFTL::readInfoFTL(CmString _ConfigPath, CmValueINI& _Return, const int8* _NameExtension)
{
	// get return values
	CmValueFTL& _LogLevel = _Return.getLogLevel();
	CmValueFTL& _Message = _Return.getMessage();
	CmValueFTL& _Context = _Return.getContext();

	// check for pending error
	if (int32(_LogLevel) == CMLOG_Error) return false;
	_LogLevel = CMLOG_None;

	// complete config filename
	CmString ConfigFile(_ConfigPath);
	CmIFTL IFTL;
	IFTL = getText();
	ConfigFile += IFTL.getConfig();
	if (_NameExtension != NULL) {
		ConfigFile += "_";
		ConfigFile += _NameExtension;
	}
	ConfigFile += ".ftl";

	// check file availability
	_Message.setText("- new version -");
	if (false == isFileExists(ConfigFile.getText())) return false;
	// read profile from disk
	try {
		CmString ConfigString;
		CmStringFTL StringFTL;
		ConfigString.readBinary(ConfigFile.getText());
		StringFTL.processStringFTL(ConfigString);

		// update application profile 
		_Message.setText("message");
		if (false == updateInfoFTL(StringFTL, _Return)) {
			// DIAG: write failed StringFTL to disk
			ConfigFile += "_failed.txt";
			CmString FailedStringFTL;
			StringFTL.serializeStringFTL(FailedStringFTL);
			FailedStringFTL.writeBinary(ConfigFile.getBuffer());
			return false;
		}
	}
	catch (...) {
		CmString Message("Config file '");
		Message += ConfigFile;
		Message += "' failed";
		_LogLevel = CMLOG_Error;
		_Message.setText(Message.getText());
		_Context.setText(getText());
		return false;
	}

	return true;
}
bool CmValueFTL::determineListOfProfiles(CmString _ConfigPath, CmString _AppProfile, CmString _AppVersion)
{
	// get list of available profiles
	CmLString ProfileList;

	// profile name
	CmString ProfileName(_AppProfile);
	ProfileName += "_";
	ProfileName += _AppVersion;
	// path to configuration profiles
	CmString ProfilePath(_ConfigPath);
	ProfilePath += "\\*";

	// get all files on current path
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	CmString Filename;
	CmString Extension;
	hFind = FindFirstFileA(ProfilePath.getText(), &FindFileData);
	while (hFind != INVALID_HANDLE_VALUE) {
		// check if item is a file
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			// add the extension of a found configuration file to the result list
			Filename = FindFileData.cFileName;
			if (Filename >= ProfileName) {
				Extension.assignSubString(int32(Filename.getLength()), &Filename, 1 + int32(ProfileName.getLength()));
			}
			Extension.getLength() > 0 ? ProfileList.addString(Extension) : 0;
		}
		// find next item
		if (false == FindNextFileA(hFind, &FindFileData)) break;
	}
	hFind != INVALID_HANDLE_VALUE ? FindClose(hFind) : 0;


	CmMatrix& M = getMatrix();

	// [']=update, [i]=profile

	for (int32 i = 0; i < ProfileList.getSize(); i++) {
		// add profile to matrix
		M(i) = ProfileList[i];
	}
	// set update flag true
	M() = true;
	leaveMatrix();

	return true;
}

//------Information-----------------------------------------------------------
bool CmValueFTL::updateInformation(CmString& _Information, CmValueINI& _Return)
{
	// get return values
	CmValueFTL& _LogLevel = _Return.getLogLevel();
	//CmValueFTL& _Message = _Return.getMessage();
	//CmValueFTL& _Context = _Return.getContext();

	// generate a CmStringFTL hierarchy
	CmStringFTL InfoFTL;
	InfoFTL.processStringFTL(_Information);

	_LogLevel = CMLOG_None;
	return updateInfoFTL(InfoFTL, _Return);
}

bool CmValueFTL::clearConfig()
{
	// travel the list of contained values
	CmValueFTL* ValueFTL = this;
	do {
		// clear matrix
		releaseMemory<CmMatrix>(ValueFTL->Matrix, 1, isCmMatrix);

		// next value
		ValueFTL = ValueFTL->Next;
	} while (NULL != ValueFTL);

	return true;
}

/** operator=(). Write access to CmValueFTL */
const CmStringFTL& CmValueFTL::operator=(const CmStringFTL& /*_StringFTL*/)
{
	// ToDo

	return *this;
}
const CmString& CmValueFTL::operator=(const CmString& _String)
{
	CmString::setText(_String);
	DataFormat = DATAFORMAT_STRING;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _String;
}
const int8* CmValueFTL::operator=(const int8* _String)
{
	CmString::setText(_String);
	DataFormat = DATAFORMAT_STRING;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _String;
}
int8 CmValueFTL::operator = (int8 _Val8)
{
	CmString::setValue(_Val8);
	DataFormat = DATAFORMAT_INT8;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _Val8;
}
uint8 CmValueFTL::operator = (uint8 _Val8)
{
	CmString::setValue(_Val8);
	DataFormat = DATAFORMAT_UINT8;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _Val8;
}
bool CmValueFTL::operator = (bool _ValBool)
{
	CmString::setValue(_ValBool);
	DataFormat = DATAFORMAT_BOOL;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _ValBool;
}
int16 CmValueFTL::operator = (int16 _Val16)
{
	CmString::setValue(_Val16);
	DataFormat = DATAFORMAT_INT16;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _Val16;
}
uint16 CmValueFTL::operator = (uint16 _Val16)
{
	CmString::setValue(_Val16);
	DataFormat = DATAFORMAT_UINT16;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _Val16;
}
int32 CmValueFTL::operator = (int32 _Val32)
{
	CmString::setValue(_Val32);
	DataFormat = DATAFORMAT_INT32;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _Val32;
}
uint32 CmValueFTL::operator = (uint32 _Val32)
{
	CmString::setValue(_Val32);
	DataFormat = DATAFORMAT_UINT32;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _Val32;
}
float CmValueFTL::operator = (float _ValFloat)
{
	CmString::setValue(_ValFloat);
	DataFormat = DATAFORMAT_FLOAT;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _ValFloat;
}
int64 CmValueFTL::operator = (int64 _Val64)
{
	CmString::setValue(_Val64);
	DataFormat = DATAFORMAT_INT64;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _Val64;
}
uint64 CmValueFTL::operator = (uint64 _Val64)
{
	CmString::setValue(_Val64);
	DataFormat = DATAFORMAT_UINT64;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _Val64;
}
double CmValueFTL::operator = (double _ValDouble)
{
	CmString::setValue(_ValDouble);
	DataFormat = DATAFORMAT_DOUBLE;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _ValDouble;
}
void* CmValueFTL::operator = (void* _ValPointer)
{
	CmString::setValue(_ValPointer);
	DataFormat = DATAFORMAT_BINARY;
	TypeFTL = TYPEFTL_NONE;
	// delete TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	return _ValPointer;
}

CmString CmValueFTL::asStringFTL(CmString _Prefix, int32 _Precision, int32 _Digits)
{
	// generate FTL string
	double ValDouble;
	CmString Val;
	switch (DataFormat) {
	case DATAFORMAT_NONE:					Val = getText(); break;
	case DATAFORMAT_FTLight:
		switch (TypeFTL) {
		case TYPEFTL_OPERATOR:				Val = getText(); break;
		case TYPEFTL_LOCATION:				Val = getText(); break;
		case TYPEFTL_IDENTIFIER:			Val = getText(); break;
		case TYPEFTL_QUERY:					Val = getText(); break;
		case TYPEFTL_TXL:					Val = getText(); break;
		case TYPEFTL_TXL_BINFTL:			Val = getText(); break;
		case TYPEFTL_NUMBER:				ValDouble = getNumAsDouble(); if (isConversionError) { Val = getText(); } else { Val.double2String(ValDouble, _Precision > -1 ? _Precision : limitPrecision(ValDouble, _Digits), _Digits); }; break;
		case TYPEFTL_NUMBER_BINFTL:			ValDouble = getNumAsDouble(); if (isConversionError) { Val = getText();	} else { Val.double2String(ValDouble, _Precision > -1 ? _Precision : limitPrecision(ValDouble, _Digits), _Digits); } break;
		case TYPEFTL_Local:					Val = ""; break;
		case TYPEFTL_Local_BINFTL:			Val = ""; break;
		case TYPEFTL_FIXED_PARENTS:			Val = ""; break;
		case TYPEFTL_CHECKSUM:				Val = ""; break;
		case TYPEFTL_OPTION:				Val = getText(); break;
		case TYPEFTL_DEFAULT:				Val = ""; break;
		case TYPEFTL_COMMENT:				Val = getText(); break;
		case TYPEFTL_BINFTL:				Val = ""; break;
		case TYPEFTL_CONTROLFTL:				Val = ""; break;
		case TYPEFTL_FTLightOPEN:			Val = ""; break;
		case TYPEFTL_FTLightWRAP:			Val = ""; break;
		case TYPEFTL_DTI_MCL:				Val = ""; break;
		case TYPEFTL_BINFTLBINARY:			Val = ""; break;
		case TYPEFTL_BINFTLSTRING:			Val = ""; break;
		case TYPEFTL_BINFTLVALUE:			Val = ""; break;
		case TYPEFTL_BINFTLTIME:			Val = ""; break;
		case TYPEFTL_CMFTLTOKEN:				Val = ""; break;
		case TYPEFTL_CMFTLLINK:				Val = ""; break;
		case TYPEFTL_BINDIF:				Val = ""; break;
		default: 							Val = ""; break;
		}
		break;
	case DATAFORMAT_STRING:				Val = getText(); break;
	case DATAFORMAT_BINARY:				Val.uint2hex(uint64(*this)); break;
	case DATAFORMAT_UINT64:				Val.uint2hex(uint64(*this)); break;
	case DATAFORMAT_INT64:				Val.double2String((double)int64(*this)); break;
	case DATAFORMAT_DOUBLE:				Val.double2String(double(*this), _Precision > -1 ? _Precision : limitPrecision(double(*this), _Digits), _Digits); break;
	case DATAFORMAT_UINT32:				Val.double2String(int32(*this)); break;
	case DATAFORMAT_INT32:				Val.double2String(int32(*this)); break;
	case DATAFORMAT_FLOAT:				Val.float2String(float(*this), _Precision > -1 ? _Precision : limitPrecision(double(float(*this)), -1), _Digits); break;
	case DATAFORMAT_UINT16:				Val.double2String(uint16(*this)); break;
	case DATAFORMAT_INT16:				Val.double2String(int16(*this)); break;
	case DATAFORMAT_UINT8:				Val.double2String(uint8(*this)); break;
	case DATAFORMAT_INT8:					Val.double2String(int8(*this)); break;
	case DATAFORMAT_BOOL:					Val = bool(*this) ? "1" : "0"; break;
	}
	_Prefix += Val;

	return _Prefix;
}

CmString CmValueFTL::asStringFTLbool(CmString _Prefix)
{
	// generate FTL string
	CmString Val(bool(*this) ? "1" : "0");
	_Prefix += Val;

	return _Prefix;
}

/** explicit type conversion */
CmString& CmValueFTL::asStringConvert(int32 _Precision, int32 _Digits)
{
	if (false == asDataFormat(DATAFORMAT_STRING, _Precision, _Digits)) this->setText("<unknown>");
	return *this;
}
CmValueFTL::operator CmString& () {
	return asStringConvert();
}
CmValueFTL::operator const int8* () {
	return asStringConvert().getText();
}
CmValueFTL::operator void* () {
	if (false == asDataFormat(DATAFORMAT_UINT64)) return NULL;
	return *(void**)getBinary();
}
CmValueFTL::operator uint64() {
	if (false == asDataFormat(DATAFORMAT_UINT64)) return 0;
	return *(uint64*)getBinary();
}
CmValueFTL::operator int64() {
	if (false == asDataFormat(DATAFORMAT_INT64)) return 0;
	return *(int64*)getBinary();
}
CmValueFTL::operator double() {
	if (false == asDataFormat(DATAFORMAT_DOUBLE)) return 0;
	return *(double*)getBinary();
}
CmValueFTL::operator uint32() {
	if (false == asDataFormat(DATAFORMAT_UINT32)) return 0;
	return *(uint32*)getBinary();
}
CmValueFTL::operator int32() {
	if (false == asDataFormat(DATAFORMAT_INT32)) return 0;
	return *(int32*)getBinary();
}
CmValueFTL::operator float() {
	if (false == asDataFormat(DATAFORMAT_FLOAT)) return 0;
	return *(float*)getBinary();
}
CmValueFTL::operator uint16() {
	if (false == asDataFormat(DATAFORMAT_UINT16)) return 0;
	return *(uint16*)getBinary();
}
CmValueFTL::operator int16() {
	if (false == asDataFormat(DATAFORMAT_INT16)) return 0;
	return *(int16*)getBinary();
}
CmValueFTL::operator uint8() {
	if (false == asDataFormat(DATAFORMAT_UINT8)) return 0;
	return *(uint8*)getBinary();
}
CmValueFTL::operator int8() {
	if (false == asDataFormat(DATAFORMAT_INT8)) return 0;
	return *(int8*)getBinary();
}
CmValueFTL::operator bool() {
	if (false == asDataFormat(DATAFORMAT_BOOL)) return false;
	return *(bool*)getBinary();
}

/** compare operators */
// string
bool CmValueFTL::operator == (const int8* _String) {
	if (NULL == _String) {
		return operator const int8 * () == NULL ? true : false;
	}
	return operator CmString & () == _String;
}
bool CmValueFTL::operator != (const int8* _String) {
	if (NULL == _String) {
		return operator const int8 * () != NULL ? true : false;
	}
	return operator CmString & () != _String;
}
bool CmValueFTL::operator == (CmString& _String) {
	return operator CmString & () == _String;
}
bool CmValueFTL::operator != (CmString& _String) {
	return operator CmString & () != _String;
}
// double
bool CmValueFTL::operator == (double _Value) {
	return operator double() == _Value;
}
bool CmValueFTL::operator != (double _Value) {
	return operator double() != _Value;
}
bool CmValueFTL::operator > (double _Value) {
	return operator double() > _Value;
}
bool CmValueFTL::operator < (double _Value) {
	return operator double() < _Value;
}
// int
bool CmValueFTL::operator == (int64 _Value) {
	return operator int64() == _Value;
}
bool CmValueFTL::operator != (int64 _Value) {
	return operator int64() != _Value;
}
bool CmValueFTL::operator > (int64 _Value) {
	return operator int64() > _Value;
}
bool CmValueFTL::operator < (int64 _Value) {
	return operator int64() < _Value;
}
// uint
bool CmValueFTL::operator == (uint64 _Value) {
	return operator uint64() == _Value;
}
bool CmValueFTL::operator != (uint64 _Value) {
	return operator uint64() != _Value;
}
bool CmValueFTL::operator > (uint64 _Value) {
	return operator uint64() > _Value;
}
bool CmValueFTL::operator < (uint64 _Value) {
	return operator uint64() < _Value;
}
// bool
bool CmValueFTL::operator == (bool _Value) {
	return operator bool() == _Value;
}
bool CmValueFTL::operator != (bool _Value) {
	return operator bool() != _Value;
}

// large numbers operators
CmString& CmValueFTL::setNumber(const CmString& _Num)
{
	*this = _Num;
	this->TypeFTL = TYPEFTL_NUMBER;

	return *this;
}
// comparison
bool CmValueFTL::operator > (const CmValueFTL& _Value)
{
	// only numbers (strings) are evaluated
	if ((TypeFTL != TYPEFTL_NUMBER) || (_Value.TypeFTL != TYPEFTL_NUMBER)) return false;

	if (getLength() < _Value.getLength()) return false;
	if (getLength() > _Value.getLength()) return true;

	return strcmp(getBuffer(), _Value.getBuffer()) > 0 ? true : false;
}
bool CmValueFTL::operator < (const CmValueFTL& _Value)
{
	// only numbers (strings) are evaluated
	if ((TypeFTL != TYPEFTL_NUMBER) || (_Value.TypeFTL != TYPEFTL_NUMBER)) return false;

	if (getLength() < _Value.getLength()) return true;
	if (getLength() > _Value.getLength()) return false;

	return strcmp(getBuffer(), _Value.getBuffer()) < 0 ? true : false;
}
bool CmValueFTL::operator >= (const CmValueFTL& _Value)
{
	// only numbers (strings) are evaluated
	if ((TypeFTL != TYPEFTL_NUMBER) || (_Value.TypeFTL != TYPEFTL_NUMBER)) return false;

	if (getLength() < _Value.getLength()) return false;
	if (getLength() > _Value.getLength()) return true;

	return strcmp(getBuffer(), _Value.getBuffer()) >= 0 ? true : false;
}
bool CmValueFTL::operator <= (const CmValueFTL& _Value)
{
	// only numbers (strings) are evaluated
	if ((TypeFTL != TYPEFTL_NUMBER) || (_Value.TypeFTL != TYPEFTL_NUMBER)) return false;

	if (getLength() < _Value.getLength()) return true;
	if (getLength() > _Value.getLength()) return false;

	return strcmp(getBuffer(), _Value.getBuffer()) <= 0 ? true : false;
}
// arithmetic operations
CmString CmValueFTL::operator + (const CmValueFTL& _B)
{
	// only numbers (strings) are accepted
	CmString Result("sum");
	if ((TypeFTL != TYPEFTL_NUMBER) || (_B.TypeFTL != TYPEFTL_NUMBER)) return Result;

	// determine size
	int32 LenA = int32(getLength());
	int32 LenB = int32(_B.getLength());
	int32 LenResult = max(LenA, LenB) + 1;
	Result.setLength(LenResult);

	// Result = A
	for (int32 r = LenResult, a = LenA; r >= 0; r--, a--) {
		memset(Result.getBuffer() + r, a >= 0 ? CmString(*this)[a] : '0', 1);
	}
	// add B
	for (int32 b = LenB - 1; b >= 0; b--) {
		int32 B = CmString(_B)[b] - '0';
		for (int32 r = LenResult - (LenB - b); r >= 0; r--) {
			memset(Result.getBuffer() + r, '0' + (B += (Result[r] - '0')) % 10, 1);
			if ((B /= 10) == 0) break;
		}
	}

	// remove leading zero if needed
	if (Result[0] == '0') {
		Result.assignSubString(int32(Result.getLength() - 1), 0, 1);
	}

	return Result;
}
CmString& CmValueFTL::operator+= (const CmValueFTL& _B)
{
	setNumber(operator+(_B));
	return *this;
}
CmString CmValueFTL::operator - (const CmValueFTL& _B)
{
	// only number (strings) are accepted
	CmString Result("dif");
	if ((TypeFTL != TYPEFTL_NUMBER) || (_B.TypeFTL != TYPEFTL_NUMBER)) return Result;

	// handle sign
	bool isNegative = this->operator>=(_B) ? false : true;
	const CmValueFTL& A = isNegative ? _B : *this;
	const CmValueFTL& B = isNegative ? *this : _B;

	// determine size
	int32 LenA = int32(A.getLength());
	int32 LenB = int32(B.getLength());
	int32 LenResult = max(LenA, LenB) + 1;
	Result.setLength(LenResult);

	// Result = A
	for (int32 r = LenResult, a = LenA; r >= 0; r--, a--) {
		memset(Result.getBuffer() + r, a >= 0 ? CmString(A)[a] : '0', 1);
	}
	// sub B
	for (int32 b = LenB - 1; b >= 0; b--) {
		int32 ValB = CmString(B)[b] - '0';
		int32 Carry = 0;
		for (int32 r = LenResult - (LenB - b); r > 0; r--) {
			int32 ValR = Result[r] - '0' - Carry;
			Carry = ValR < ValB ? 1 : 0;
			int32 Digit = ValR - ValB + Carry * 10;
			memset(Result.getBuffer() + r, '0' + Digit, 1);
			if (Carry == 0) {
				break;
			}
			else { ValB = 0; }
		}
	}

	// remove leading zero if needed
	if (Result[0] == '0') {
		int32 LenZ;
		for (LenZ = 1; LenZ < int32(Result.getLength()); LenZ++) {
			if (Result[LenZ] != '0') break;
		}
		LenZ >= int32(Result.getLength()) ? LenZ = int32(Result.getLength() - 1) : 0;
		Result.assignSubString(int32(Result.getLength() - 1), 0, LenZ);
	}
	if (isNegative) {
		CmString Value = Result;
		Result = "-";
		Result += Value;
	}

	return Result;
}
CmString& CmValueFTL::operator-= (const CmValueFTL& _B)
{
	setNumber(operator-(_B));
	return *this;
}
CmString CmValueFTL::operator * (const CmValueFTL& _B)
{
	// only number (strings) are accepted
	CmString Result("0");
	if ((TypeFTL != TYPEFTL_NUMBER) || (_B.TypeFTL != TYPEFTL_NUMBER)) return Result;

	// determine size
	int32 LenA = int32(getLength());
	int32 LenB = int32(_B.getLength());
	int32 LenResult = LenA + LenB;
	Result.setLength(LenResult);
	memset(Result.getBuffer(), '0', LenResult);

	/** long multiplication:
	*   - every digit of operand A will be multiplied by each digit of operand B
	*   - the sum of particular products yields the result
	*/
	for (int32 a = LenA - 1; a >= 0; a--) {
		for (int32 b = LenB - 1; b >= 0; b--) {
			int32 A = CmString(*this)[a] - '0';
			int32 B = CmString(_B)[b] - '0';
			int32 P = A * B;
			for (int32 r = a + b + 1; r >= 0; r--) {
				memset(Result.getBuffer() + r, '0' + (P += (Result[r] - '0')) % 10, 1);
				if ((P /= 10) == 0) break;
			}
		}
	}

	return Result;
}
CmString& CmValueFTL::operator*= (const CmValueFTL& _B)
{
	setNumber(operator*(_B));
	return *this;
}
CmString CmValueFTL::operator / (const CmValueFTL& _B)
{
	// only number (strings) are accepted
	CmString Result("0");
	if ((TypeFTL != TYPEFTL_NUMBER) || (_B.TypeFTL != TYPEFTL_NUMBER)) return Result;

	// initialize result
	int32 LenR = int32(getLength() - _B.getLength() + 1);
	if (LenR > 0) { Result.fill(LenR, '0'); }

	// return remainder immediately if A < B
	if (*this < _B) {
		Remainder = getText();
		return Result;
	}

	// operands
	CmString Val;
	CmValueFTL A;
	A.setNumber(*this);
	CmVector<CmValueFTL*> B(10);
	for (int32 i = 0; i < 10; i++) {
		B[i] = new CmValueFTL;
		if (i == 0) {
			Val.fill(LenR, '0');
			B[i]->setNumber(Val);
		}
		if (i == 1) {
			Val = _B;
			Val += CmString(*B[0]);
			B[i]->setNumber(Val);
		}
		if (i > 1) B[i]->setNumber(*B[i - 1] + *B[1]);
	}

	// subtract divisor
	for (int32 i = 0; i < LenR; i++) {
		// decrement exponent
		for (int32 e = 0; e < 10; e++) {
			B[e]->adjustLength(B[e]->getLength() - 1);
			// TEST
			Val.setText(*B[e]);
		}
		if (B[0]->getLength() == 0) { B[0]->setNumber("0"); }

		// btree compare
		int32 Digit = 0;
		if (*B[5] > A) {
			if (*B[2] > A) {
				if (*B[1] > A) {
					Digit = 0;
				}
				else { Digit = 1; }
			}
			else if (*B[4] > A) {
				if (*B[3] > A) {
					Digit = 2;
				}
				else { Digit = 3; }
			}
			else { Digit = 4; }
		}
		else if (*B[8] > A) {
			if (*B[7] > A) {
				if (*B[6] > A) {
					Digit = 5;
				}
				else { Digit = 6; }
			}
			else { Digit = 7; }
		}
		else	if (*B[9] > A) {
			Digit = 8;
		}
		else { Digit = 9; }

		// subtract and set result
		A -= *B[Digit];
		memset(Result.getBuffer() + i, '0' + Digit, 1);
	}

	// set remainder
	Remainder = A;

	// purge operand B
	for (int32 i = 0; i < 10; i++) {
		delete B[i];
	}

	// remove leading zero if needed
	if (Result[0] == '0') {
		Result.assignSubString(int32(Result.getLength() - 1), 0, 1);
	}

	return Result;
}
CmString& CmValueFTL::operator/= (const CmValueFTL& _B)
{
	setNumber(operator/(_B));
	return *this;
}
CmString CmValueFTL::operator % (const CmValueFTL& _B)
{
	// only number (strings) are accepted
	CmString Result("rem");
	if ((TypeFTL != TYPEFTL_NUMBER) || (_B.TypeFTL != TYPEFTL_NUMBER)) return Result;

	// get remainder after division
	operator/(_B);
	Result = Remainder;

	return Result;
}
CmString& CmValueFTL::getRemainder()
{
	return Remainder;
}
// string operations
void CmValueFTL::operator+=(const CmString& _String)
{
	CmString::operator+=(_String);
}
void CmValueFTL::operator+=(const int8* _String)
{
	CmString::operator+=(_String);
}
void CmValueFTL::operator+=(int32 _Value)
{
	CmString::operator+=(_Value);
}
void CmValueFTL::operator+=(uint32 _Value)
{
	CmString::operator+=(_Value);
}
void CmValueFTL::operator+=(int64 _Value)
{
	CmString::operator+=(_Value);
}
void CmValueFTL::operator+=(uint64 _Value)
{
	CmString::operator+=(_Value);
}

bool CmValueFTL::asDataFormat(CmDataFormat _DataFormat, int32 _Precision, int32 _Digits)
{
	// reject conversion if data format is unknown
	if (DATAFORMAT_NONE == DataFormat) return false;

	// check if requested format matches current format
	if (_DataFormat == DataFormat) return true;

	// provide a string representation
	if (DATAFORMAT_STRING == _DataFormat && TYPEFTL_NUMBER == TypeFTL && NULL != TXL && 0 != LengthTXL) {
		// restore string representaion of a numeric value
		setLength(LengthTXL);
		memcpy(getBuffer(), TXL, LengthTXL);
	}
	else if (DATAFORMAT_STRING == _DataFormat && DATAFORMAT_FTLight == DataFormat) {
		// leave string content unchanged
	}
	// convert FTLight/numeric format
	else if (DATAFORMAT_FTLight == DataFormat && TYPEFTL_NUMBER == TypeFTL) {
		// number string goes to TXL, binary format is written to Text
		if (false == decodeNum(_DataFormat)) return false;
	}
	// convert FTLight/binary format
	else if (TYPEFTL_NONE == TypeFTL || (0 == LengthTXL && NULL == TXL)) {
		// check for binary data
		if (DATAFORMAT_BINARY != DataFormat) {
			// conversion to 'uint64' from any other format is not supported (can not be done exactly using an intermediate 'double')
			if (DATAFORMAT_UINT64 == _DataFormat) return false;
			// put current value into a 'double' bit field and save it as FTL in TXL
			if (NULL == TXL && 0 == LengthTXL) {
				switch (DataFormat) {
				case DATAFORMAT_DOUBLE: encodeNUM(double(*this));  break;
				case DATAFORMAT_UINT64: encodeNUM((double)uint64(*this));  break;
				case DATAFORMAT_INT64: encodeNUM((double)int64(*this));  break;
				case DATAFORMAT_UINT32: encodeNUM((double)uint32(*this));  break;
				case DATAFORMAT_INT32: encodeNUM((double)int32(*this));  break;
				case DATAFORMAT_FLOAT: encodeNUM((double)float(*this));  break;
				case DATAFORMAT_UINT16: encodeNUM((double)uint16(*this));  break;
				case DATAFORMAT_INT16: encodeNUM((double)int16(*this));  break;
				case DATAFORMAT_UINT8: encodeNUM((double)uint8(*this));  break;
				case DATAFORMAT_INT8: encodeNUM((double)int8(*this));  break;
				case DATAFORMAT_BOOL: encodeNUM((double)bool(*this));  break;
				default: break;
				}
				// TypeFTL is now TYPEFTL_NONE
				0 != LengthTXL ? TypeFTL = TYPEFTL_NONE : 0;
			}
			// restore a 'double' bit field in Text from FTL, preserve FTL representation in TXL
			decodeFTL(TXL, LengthTXL);
			// special case 'uint64': convert 'double'
			if (DATAFORMAT_UINT64 == _DataFormat) {
				*(uint64*)getBuffer() = (uint64)(*(double*)getBuffer());
			}
		}
		// check length
		if (getLength() < 8) return false;
		// leave data for uint64 format as it is, all other formats: 
		// assume that the binary data represents a double value
		double Value = *(double*)getBuffer();
		// convert double to requested format
		switch (_DataFormat) {
		case DATAFORMAT_STRING: double2String(Value, _Precision > -1 ? _Precision : limitPrecision(Value, _Digits), _Digits); break;
		case DATAFORMAT_INT64: *(int64*)getBuffer() = (int64)Value; break;
		case DATAFORMAT_DOUBLE: *(double*)getBuffer() = (double)Value; break;
		case DATAFORMAT_UINT32: *(uint32*)getBuffer() = (uint32)Value; break;
		case DATAFORMAT_INT32: *(int32*)getBuffer() = (int32)Value; break;
		case DATAFORMAT_FLOAT: *(float*)getBuffer() = (float)Value; break;
		case DATAFORMAT_UINT16: *(uint16*)getBuffer() = (uint16)Value; break;
		case DATAFORMAT_INT16: *(int16*)getBuffer() = (int16)Value; break;
		case DATAFORMAT_UINT8: *(uint8*)getBuffer() = (uint8)Value; break;
		case DATAFORMAT_INT8: *(int8*)getBuffer() = (int8)Value; break;
		case DATAFORMAT_BOOL: *(bool*)getBuffer() = fabs(Value) > MIN_DOUBLE; break;
			break;
		default: break;
		}
	}

	// remember new data format
	DataFormat = _DataFormat;

	return true;
}
int32 CmValueFTL::limitPrecision(double _Value, int32 _ValidDigits)
{
	// unlimited precision if valid digits are unlimited
	if (-1 == _ValidDigits) return -1;
	// determine digits before decimal separator
	int32 Exponent = _Value < 1.7e-308 ? 0 : (int32)floor(log10(_Value)) + 1;
	// limit precision for not exceeding valid digits
	int32 Precision = Exponent >= _ValidDigits ? 0 : _ValidDigits - Exponent - 1;
	// set precision to 0 for smallest double value 
	Precision = _Value > MIN_DOUBLE ? Precision : 0;

	return Precision;
}

//--------CmMatrixFTL-integration---------------------------------------------
bool CmValueFTL::clearMatrix()
{
	if (NULL == Matrix) return true;
	// purge matrix
	bool isSuccess = getMatrix().clearMatrix();
	leaveMatrix();
	// purge root element
	getMatrix()() = 0;
	leaveMatrix();
	return isSuccess;
}

CmMatrix& CmValueFTL::getMatrix(int32 _LockID)
{
	// lock matrix
	MatrixRecursionCounter = enterSerialize(_LockID);

	// Instantiate a CmMatrixFTL class if it does not exist yet
	if (NULL == Matrix) {
		Matrix = CmString::allocateMemory<CmMatrix>(1, isCmMatrix);
	}

	// Throw exception in case of memory problems
	if (NULL == Matrix)	throw(0);

	// reset index to root element
	return (*Matrix)();
}
int32 CmValueFTL::leaveMatrix()
{
	// unlock matrix
	MatrixRecursionCounter = leaveSerialize();

	return MatrixRecursionCounter;
}

//--------CmMatrixFTL-application---------------------------------------------

/** set/clearItemUpdated.
*  Item updates are indicated by a flag in the scalar position of a matrix.
*
*  [`]=item updated
*/
bool CmValueFTL::setItemUpdated()
{
	CmMatrix& M = getMatrix();
	M() = true;
	leaveMatrix();

	return true;
}
bool CmValueFTL::clearItemUpdated()
{
	// get ItemUpdated flag
	CmMatrix& M = getMatrix();
	bool isItemUpdated = M();

	// clear ItemUpdated flag
	M() = false;
	leaveMatrix();

	return isItemUpdated;
}

/** set/getCanvasDrawingEnabled/ItemEnabled.
*  Drawing on screen can be activated/deactivated by a flag in the scalar
*  position of a matrix respectively in the scalar position of an item.
*
*  [`]=drawing enabled, [0,i]=item i disabled
*/
bool CmValueFTL::setDrawingEnabled(bool _isDrawingEnabled)
{
	CmMatrix& M = getMatrix();
	M() = _isDrawingEnabled;
	leaveMatrix();
	return _isDrawingEnabled;
}
bool CmValueFTL::getDrawingEnabled()
{
	CmMatrix& M = getMatrix();
	bool isDrawingEnabled = M();
	leaveMatrix();
	return isDrawingEnabled;
}
bool CmValueFTL::setItemEnabled(int32 _Index, bool _isItemEnabled)
{
	CmMatrix& M = getMatrix();
	bool isItemEnabled = M(0, _Index) = _isItemEnabled ? false : true; // disable item drawing
	leaveMatrix();
	return isItemEnabled;
}
bool CmValueFTL::getItemEnabled(int32 _Index)
{
	CmMatrix& M = getMatrix();
	bool isItemEnabled = (M(0, _Index)) ? false : true;
	leaveMatrix();
	return isItemEnabled;
}
int32 CmValueFTL::getItemCount()
{
	CmMatrix& M = getMatrix();
	CmIndex I;
	int32 ItemCount = M(0).getSizeLastIndexLevel();
	leaveMatrix();
	return ItemCount;
}

/** setChartBackground/Axis.
*  The background parameters as well as the axis parameters are set/get.
*
* [0]=color, [1]=alpha, [2]=Xmin, [3]=Xmax, [4]=Ymin, [5]=Ymax
*/
bool CmValueFTL::setChartBackground(const CmString& _Color, double _Alpha)
{
	CmMatrix& M = getMatrix();
	M(IP_Color) = _Color;
	M(IP_Alpha) = _Alpha;
	leaveMatrix();
	return true;
}
bool CmValueFTL::getChartBackground(CmString& _Color, double& _Alpha)
{
	CmMatrix& M = getMatrix();
	_Color = CmString(M(IP_Color));
	_Alpha = double(M(IP_Alpha));
	leaveMatrix();
	return true;
}
bool CmValueFTL::setRangeX(double _Xmin, double _Xmax)
{
	CmMatrix& M = getMatrix();
	M(IP_MinX) = _Xmin;
	M(IP_MaxX) = _Xmax;
	leaveMatrix();
	return true;
}
bool CmValueFTL::getRangeX(double& _Xmin, double& _Xmax)
{
	CmMatrix& M = getMatrix();
	_Xmin = double(M(IP_MinX));
	_Xmax = double(M(IP_MaxX));
	leaveMatrix();
	return true;
}
bool CmValueFTL::setRangeY(double _Ymin, double _Ymax)
{
	CmMatrix& M = getMatrix();
	M(IP_MinY) = _Ymin;
	M(IP_MaxY) = _Ymax;
	leaveMatrix();
	return true;
}
bool CmValueFTL::getRangeY(double& _Ymin, double& _Ymax)
{
	CmMatrix& M = getMatrix();
	_Ymin = double(M(IP_MinY));
	_Ymax = double(M(IP_MaxY));
	leaveMatrix();
	return true;
}


/** script
*  A collection of scripts will be written into the matrix by an application module
*  and they will be read out for displaying them on screen by a visualization module.
*
* [`]=enabled, [0,t]=disabled, [0,t,0/1]=position x/y, [1,t]=text, [i]=defaults, [2,t]=size, [3,t]=rotation, [4,t]=color, [5,t]=font, [6,t]=alpha
*/
bool CmValueFTL::setScript(int64 _Index, const CmPoint2D& _Position, const CmString& _Text, double _FontSize, double _Rotation, const CmString& _Color, double _Alpha, const CmString& _Font)
{
	int32 Index = int32(_Index);
	return(setScript(Index, _Position, _Text, _FontSize, _Rotation, _Color, _Alpha, _Font));
}
bool CmValueFTL::setScript(int32& _Index, const CmPoint2D& _Position, const CmString& _Text, double _FontSize, double _Rotation, const CmString& _Color, double _Alpha, const CmString& _Font)
{
	CmMatrix& M = getMatrix();
	// provide for having default values always available 
	M().getSizeLastIndexLevel() == 0 ? setScriptDefaults() : 0;

	// auto index
	int32 Items = M(SP_ScriptPosition).getSizeLastIndexLevel();
	LP_LineAuto == _Index ? _Index = Items : 0;

	M(SP_ScriptPosition, _Index) = false; // enable script
	M(SP_ScriptPosition, _Index, PC_X) = _Position.x();
	M(SP_ScriptPosition, _Index, PC_Y) = _Position.y();
	M(SP_ScriptText, _Index) = _Text;
	M(SP_ScriptSize, _Index) = _FontSize;
	M(SP_ScriptRotation, _Index) = _Rotation;
	M(SP_ScriptColor, _Index) = _Color;
	M(SP_ScriptFont, _Index) = _Font;
	M(SP_ScriptAlpha, _Index) = _Alpha;
	_Index++;
	leaveMatrix();
	return true;
}
bool CmValueFTL::setScriptWrap(int32& _Index, const CmPoint2D& _Position, const int32 _PosWrap, const CmString& _Text, double _FontSize, double _Rotation, const CmString& _Color, double _Alpha, const CmString& _Font)
{
	CmMatrix& M = getMatrix();
	// provide for having default values always available 
	M().getSizeLastIndexLevel() == 0 ? setScriptDefaults() : 0;

	CmString Text = _Text;
	CmString Line;
	int32 TextLength = int32(_Text.getLength());
	double RatioFontSize2Width = 1.28;
	int32 LineLength = _FontSize > 0 ? int32((_PosWrap - _Position.x()) * RatioFontSize2Width / _FontSize) : TextLength;
	int32 PosY = int32(_Position.y());
	const double LineDistance = 1.3;
	// write all lines
	for (int32 i = 0; i < TextLength; i += LineLength) {
		// generate a line of text
		Line.assignSubString(LineLength, &Text);
		Text.assignSubString(int32(Text.getLength()), 0, LineLength);

		M(SP_ScriptPosition, _Index) = false; // enable script
		M(SP_ScriptPosition, _Index, PC_X) = _Position.x();
		M(SP_ScriptPosition, _Index, PC_Y) = PosY;
		M(SP_ScriptText, _Index) = Line;
		M(SP_ScriptSize, _Index) = _FontSize;
		M(SP_ScriptRotation, _Index) = _Rotation;
		M(SP_ScriptColor, _Index) = _Color;
		M(SP_ScriptFont, _Index) = _Font;
		M(SP_ScriptAlpha, _Index) = _Alpha;
		_Index++;
		PosY -= int32(LineDistance * _FontSize);
	}
	leaveMatrix();

	return true;
}
bool CmValueFTL::setScriptDefaults(double _FontSize, double _Rotation, const CmString& _Color, double _Alpha, const CmString& _Font)
{
	CmMatrix& M = getMatrix();
	M() = true; // enable drawing
	M(SP_ScriptPosition) = false; // set default disable script: off
	M(SP_ScriptSize) = _FontSize;
	M(SP_ScriptRotation) = _Rotation;
	M(SP_ScriptColor) = _Color;
	M(SP_ScriptFont) = _Font;
	M(SP_ScriptAlpha) = _Alpha;
	leaveMatrix();

	return true;
}
bool CmValueFTL::getScript(int32 _Index, CmPoint2D& _Position, CmString& _Text, double& _FontSize, double& _Rotation, CmString& _Color, double& _Alpha, CmString& _Font)
{
	CmMatrix& M = getMatrix();
	_Position.x() = float(M(SP_ScriptPosition, _Index, PC_X));
	_Position.y() = float(M(SP_ScriptPosition, _Index, PC_Y));
	_Text = CmString(M(SP_ScriptText, _Index));
	_FontSize = double(M(SP_ScriptSize, _Index));
	_Rotation = double(M(SP_ScriptRotation, _Index));
	_Color = CmString(M(SP_ScriptColor, _Index));
	_Font = CmString(M(SP_ScriptFont, _Index));
	_Alpha = double(M(SP_ScriptAlpha, _Index));
	// check for default values
	_FontSize == -1 ? _FontSize = double(M(SP_ScriptSize)) : 0;
	_Rotation == -1 ? _Rotation = double(M(SP_ScriptRotation)) : 0;
	if (_Color == "") { _Color = CmString(M(SP_ScriptColor)); }
	if (_Font == "") { _Font = CmString(M(SP_ScriptFont)); }
	_Alpha == -1 ? _Alpha = double(M(SP_ScriptAlpha)) : 0;
	leaveMatrix();

	return true;
}

/** draw lines
*  A collection of lines will be written into the matrix by an application module and
*  they will be read out for putting them on screen by a visualization module.
*
*	 @PointX/Y: a (-1,-1)-terminated array of points that represent a line respectively a polyline
*
*  [`]=enabled, [0,p]=disabled, [0,p,0,0../1,0..]=points x/y, [1,p]=width, [i]=defaults, [2,p]=line color, [3,p]=fill color, [4,p]=line alpha, [5,p]=fill alpha
*/

bool CmValueFTL::setDrawLine(int32 _Index, CmVector<float>& PointX, CmVector<float>& PointY, double _LineWidth, const CmString& _LineColor, const CmString& _FillColor, double _LineAlpha, double _FillAlpha)
{
	CmMatrix& M = getMatrix();
	// provide for always having default values available 
	M().getSizeLastIndexLevel() == 0 ? setDrawLineDefaults() : 0;

	// auto index
	int32 Lines = M(LP_LineEdges).getSizeLastIndexLevel();
	LP_LineAuto == _Index ? _Index = Lines : 0;

	// write points to array
	int32 PointCount = PointX.getLength();
	for (int i = 0; i < PointCount; i++) {
		M(LP_LineEdges, _Index, PC_X, i) = PointX[i];
		M(LP_LineEdges, _Index, PC_Y, i) = PointY[i];
	}
	M(LP_LineWidth, _Index) = _LineWidth;
	M(LP_LineColor, _Index) = _LineColor;
	M(LP_FillColor, _Index) = _FillColor;
	M(LP_LineAlpha, _Index) = _LineAlpha;
	M(LP_FillAlpha, _Index) = _FillAlpha;
	M(LP_LineEdges, _Index) = false; // enable line
	leaveMatrix();

	return true;
}
bool CmValueFTL::setDrawRectangle(int32 _Index, const CmRectangle2D& _Rectangle, double _LineWidth, const CmString& _LineColor, const CmString& _FillColor, double _LineAlpha, double _FillAlpha)
{
	// generate 4 points of a rectangle
	CmVector<float> PointX;
	CmVector<float> PointY;
	PointX[0] = _Rectangle.P1().x(); PointY[0] = _Rectangle.P1().y();
	PointX[1] = _Rectangle.P2().x(); PointY[1] = _Rectangle.P1().y();
	PointX[2] = _Rectangle.P2().x(); PointY[2] = _Rectangle.P2().y();
	PointX[3] = _Rectangle.P1().x(); PointY[3] = _Rectangle.P2().y();
	bool isSuccess = setDrawLine(_Index, PointX, PointY, _LineWidth, _LineColor, _FillColor, _LineAlpha, _FillAlpha);

	return isSuccess;
}
bool CmValueFTL::setDrawLineDefaults(double _LineWidth, const CmString& _LineColor, const CmString& _FillColor, double _LineAlpha, double _FillAlpha)
{
	CmMatrix& M = getMatrix();
	M() = true; // enable drawing
	M(LP_LineEdges) = false; // set default disable line drawing: off
	M(LP_LineWidth) = _LineWidth;
	M(LP_LineColor) = _LineColor;
	M(LP_FillColor) = _FillColor;
	M(LP_LineAlpha) = _LineAlpha;
	M(LP_FillAlpha) = _FillAlpha;
	leaveMatrix();

	return true;
}
bool CmValueFTL::getDrawLine(int32 _Index, CmVector<float>& PointX, CmVector<float>& PointY, double& _LineWidth, CmString& _LineColor, CmString& _FillColor, double& _LineAlpha, double& _FillAlpha)
{
	CmMatrix& M = getMatrix();
	// generate point array
	int32 PointCount = M(LP_LineEdges, _Index, PC_X).getSizeLastIndexLevel();
	PointX.setLength(PointCount);
	PointY.setLength(PointCount);

	// retrieve points from matrix and forward them to caller
	for (int i = 0; i < PointCount; i++) {
		PointX[i] = float(M(LP_LineEdges, _Index, PC_X, i));
		PointY[i] = float(M(LP_LineEdges, _Index, PC_Y, i));
	}
	_LineWidth = float(M(LP_LineWidth, _Index));
	_LineColor = CmString(M(LP_LineColor, _Index));
	_FillColor = CmString(M(LP_FillColor, _Index));
	_LineAlpha = float(M(LP_LineAlpha, _Index));
	_FillAlpha = float(M(LP_FillAlpha, _Index));
	// check for default values
	_LineWidth == -1 ? _LineWidth = float(M(LP_LineWidth)) : 0;
	if (_LineColor == "") { _LineColor = CmString(M(LP_LineColor)); }
	if (_FillColor == "") { _FillColor = CmString(M(LP_FillColor)); }
	_LineAlpha == -1 ? _LineAlpha = float(M(LP_LineAlpha)) : 0;
	_FillAlpha == -1 ? _FillAlpha = float(M(LP_FillAlpha)) : 0;
	leaveMatrix();

	return true;
}

CmValueFTL& CmValueFTL::getLogLevel()
{
	return *this;
}
CmValueFTL& CmValueFTL::getMessage()
{
	bool isEndOfConfigValues;
	return getNextConfigValue(isEndOfConfigValues);
}
CmValueFTL& CmValueFTL::getContext()
{
	bool isEndOfConfigValues;
	return getNextConfigValue(isEndOfConfigValues).getNextConfigValue(isEndOfConfigValues);
}


//------------------------------------------------------------------------------
// CmValueINI class
//------------------------------------------------------------------------------
//

static CmMatrix* MatrixINI_ = NULL;
static CmString* StringINI_ = NULL;

bool CmValueINI::testCmValueINI()
{
	bool Result = true;
	printf("\n");

	// test CmValueINI
	struct _ConfigSettings {
		// identification
		CmValueINI IFTL;
		CmValueINI Timestamp;
		CmValueINI LogLevel;
		CmValueINI Message;
		CmValueINI Context;
		// items
		CmValueINI Item00;
		CmValueINI Item0000;
		CmValueINI Item01;
		CmValueINI Item02;
		CmValueINI Item03;
		CmValueINI Item031;
		CmValueINI Item04;
		CmValueINI Item0400;
		CmValueINI Item04001;
		CmValueINI Item05;

	};
	typedef struct _ConfigSettings ConfigSettings;

#define CfgIFTL				"EKD@JN58nc_Türkenfeld.CmValueINI"
#define CfgTimestamp		"/timestamp"
#define CfgLogLevel			">loglevel"
#define CfgMessage			">message"
#define CfgContext			">context"
#define CfgItem00			"/Item00:Value000"
#define CfgItem0000			">>Item0000:Value00000"
#define CfgItem01			"<<Item01:Value010"
#define CfgItem02			"Item02"
#define CfgItem03			":Value030"
#define CfgItem031			">Item031"
#define CfgItem04			"<Item04:Value040"
#define CfgItem0400			">>Item0400:Value04000"
#define CfgItem04001		">Item04001"
#define CfgItem05			"/Item05:Value050"
	ConfigSettings Cfg =
	{
		// identification
		CfgIFTL,
		CfgTimestamp,
		CfgLogLevel,
		CfgMessage,
		CfgContext,
		// name:value or name or value
		CfgItem00,
		CfgItem0000,
		CfgItem01,
		CfgItem02,
		CfgItem03,
		CfgItem031,
		CfgItem04,
		CfgItem0400,
		CfgItem04001,
		CfgItem05,

	};

	// verify values and structure
	// IFTL, Timestamp, LogLevel, Message, Context
	if (CmValueINI::getName(CfgIFTL) != Cfg.IFTL || Cfg.IFTL.getAddress() != "0") Result = false;
	if (CmValueINI::getName(CfgTimestamp) != Cfg.Timestamp || Cfg.Timestamp.getAddress() != "0-0") Result = false;
	if (CmValueINI::getName(CfgLogLevel) != Cfg.LogLevel || Cfg.LogLevel.getAddress() != "0-0-0") Result = false;
	if (CmValueINI::getName(CfgMessage) != Cfg.Message || Cfg.Message.getAddress() != "0-0-0-0") Result = false;
	if (CmValueINI::getName(CfgContext) != Cfg.Context || Cfg.Context.getAddress() != "0-0-0-0-0") Result = false;
	// name (value) below root level
	if ((CmValueINI::getValue(CfgItem00) != Cfg.Item00) || (Cfg.Item00.getAddress() != "0-1-0")) Result = false;
	// name:value pair two levels lower as before
	if (CmValueINI::getValue(CfgItem0000) != (Cfg.Item0000) || (Cfg.Item0000.getAddress() != "0-1-0-0-0")) Result = false;
	// name:value pair on same level as before
	if (CmValueINI::getValue(CfgItem01) != (Cfg.Item01) || (Cfg.Item01.getAddress() != "0-2-0")) Result = false;
	// only a name on same level as before
	if (CmValueINI::getName(CfgItem02) != (Cfg.Item02) || (Cfg.Item02.getAddress() != "0-3")) Result = false;
	// only a value on same level as before
	if (CmValueINI::getValue(CfgItem03) != (Cfg.Item03) || (Cfg.Item03.getAddress() != "0-4-0")) Result = false;
	// only a name one level lower as before
	if ((CmValueINI::getName(CfgItem031) != Cfg.Item031) || (Cfg.Item031.getAddress() != "0-4-1")) Result = false;
	// name:value pair one level higher as before
	if (CmValueINI::getValue(CfgItem04) != (Cfg.Item04) || (Cfg.Item04.getAddress() != "0-5-0")) Result = false;
	// name:value pair two levels lower as before
	if ((CmValueINI::getValue(CfgItem0400) != Cfg.Item0400) || (Cfg.Item0400.getAddress() != "0-5-0-0-0")) Result = false;
	// item three levels lower as before with a path gap
	if ((CmValueINI::getName(CfgItem04001) != Cfg.Item04001) || (Cfg.Item04001.getAddress() != "0-5-0-0-1")) Result = false;
	// name (value) below root level
	if ((CmValueINI::getValue(CfgItem05) != Cfg.Item05) || (Cfg.Item05.getAddress() != "0-6-0")) Result = false;

	// synchronize and write resulting StringFTL to disk
	NULL != Cfg.IFTL.StringINI ? Cfg.IFTL.StringINI->writeBinary("./Data/CmValueINI_v1.0.ini") : 0;

	// generate FTLight structure
	CmStringFTL StructureFTL;
	CmString StringFTL;
	CmIFTL IFTL(CfgIFTL);
	StructureFTL.processStringFTL(Cfg.IFTL.StringINI->getText());

	StructureFTL.serializeStringFTL(StringFTL);
	StringFTL.writeBinary("./Data/CmValueINI_v1.0.ftl");

	if (false == Cfg.IFTL.writeInfoFTL("./Data/", StructureFTL, Cfg.LogLevel)) {
		printf("\nwriteInfoFTL failed:\n");
		printf(Cfg.Message.getText());
		printf(Cfg.Context.getText());
		printf("\n");
	}

	return Result;
}
CmValueINI::CmValueINI(const int8* _Init)
	:CmValueFTL(CmIFTL::isIFTL(_Init) ? "0" : "0-0")
{
	// initialize workspace
	MatrixINI = NULL;
	StringINI = NULL;

	// split init string into a name/value/... components
	int PosName = 0; // name's starting position in the init string
	CmString Name = CmValueINI::getName(_Init, &PosName);
	CmString Value = CmValueINI::getValue(_Init);
	CmString Separator(",");

	// is root element?
	if ((_Init == NULL) || (strlen(_Init) == 0) || CmIFTL::isIFTL(_Init)) {
		// start new FTLight hierarchy generation
		MatrixINI = CmString::allocateMemory<CmMatrix>(1, isCmMatrix);
		MatrixINI_ = MatrixINI;
		StringINI = CmString::allocateMemory<CmString>(1, isCmString);
		StringINI_ = StringINI;
		// verify that an INI string exists
		if (NULL == StringINI_) return;
		// IFTL will be the top element
		Address = "0";
		CmValueFTL::operator= (Name);
		if (NULL != _Init) {
			// insert root element (IFTL) in the FTLight string
			*StringINI_ = _Init;
			*StringINI_ += "\n";
		}
	}
	else {
		CmString AddressName;
		CmString AddressValue;

		// verify that an INI item exists
		if (NULL == MatrixINI_) return;
		if (NULL == StringINI_) return;
		if (NULL == _Init) return;

		/** *MatrixINI_
		* The matrix stores the number of elements on each level from current level upwards.
		* It allows to return and to continue on each level above current level.
		* Furthermore, the FTLight address will be generated from information in the matrix.
		*/
		CmMatrix& M = *MatrixINI_;

		// retrieve current level in the FTLight hierarchy
		int Level = M.getLength();

		// navigation
		int32 ExtendedLevels;
		switch (*_Init) {
		case '/':
			// extend subordinate level below root
			Level > 0 ? M(0) = int(M(0)) + 1 : 0;
			M.setLength(Level = 1);
			break;
		case '>':
			// shift level downwards
			M.setLength(Level += PosName);
			ExtendedLevels = M(0, 0);
			PosName <= ExtendedLevels ? M(Level - 1) = int(M(Level - 1)) + 1 : 0;
			break;
		case '<':
			Level > PosName ? M.setLength(Level -= PosName) : 0;
			M(Level - 1) = int(M(Level - 1)) + 1;
			M.setLength(Level);
			break;
		default:
			// continue on current level
			Level > 0 ? M(Level - 1) = int(M(Level - 1)) + 1 : M(0) = 0;
			break;
		}
		// address/StringFTL generation
		AddressName = "0";
		for (int i = 0; i < Level; i++) {
			AddressName += "-";
			AddressName += int(M(i));
			*StringINI_ += i <= PosName ? "," : Separator;
		}
		AddressValue = AddressName;
		AddressValue += "-0";

		// insert name and value to StringINI_
		*StringINI_ += Name.getLength() > 0 ? Name : " ";
		if (Value.getLength() > 0) { *StringINI_ += ":"; }
		if (Value.getLength() > 0) { *StringINI_ += Value; }
		*StringINI_ += "\n";

		// assign address of name or value to config item
		if (Value.getLength() > 0) {
			Address = AddressValue;
			CmValueFTL::operator= (Value);
			M(0, 0) = 1; // level is extended by 1 (value below name)
		}
		else {
			Address = AddressName;
			CmValueFTL::operator= (Name);
			M(0, 0) = 0; // level is not extended (name without any item below)
		}

		// assign label
		Label = Name;
	}
}
CmValueINI::~CmValueINI()
{
	// cleanup resources
	releaseMemory<CmMatrix>(MatrixINI, 1, isCmMatrix);
	releaseMemory<CmString>(StringINI, 1, isCmString);
}
CmString CmValueINI::getName(CmString _NameValue, int* _PosName)
{
	// find ':' position
	int PosColon = _NameValue.findPosition(':');

	// skip control chars
	int PosName = 0;
	while ((_NameValue[PosName] == '/') || (_NameValue[PosName] == '>') || (_NameValue[PosName] == '<')) {
		PosName++;
	}
	NULL != _PosName ? *_PosName = PosName : 0;

	// return name
	_NameValue.assignSubString(PosColon - 1, 0, PosName);

	return _NameValue;
}
CmString CmValueINI::getValue(CmString _NameValue)
{
	// find ':' position
	int PosColon = _NameValue.findPosition(':');

	// return value
	_NameValue.assignSubString(_NameValue.getLength() - 1, 0, PosColon + 1);

	return _NameValue;
}
CmString& CmValueINI::getLabel()
{
	return Label;
}
bool CmValueINI::setDefaultInfoFTL(CmValueINI& _ValueINI)
{
	CmValueFTL DummyReturn;
	return setDefaultInfoFTL(_ValueINI, DummyReturn);
}
bool CmValueINI::setDefaultInfoFTL(CmValueINI& _ValueINI, CmValueFTL& _Return)
{
	// check initialization string availability
	if (NULL == _ValueINI.StringINI) return false;

	CmStringFTL InfoFTL;
	InfoFTL.processStringFTL(*_ValueINI.StringINI);
	return _ValueINI.updateInfoFTL(InfoFTL, _Return);
}
bool CmValueINI::getInfoFTL(CmStringFTL& _InfoFTL, CmValueINI& _Return)
{
	// get return values
	CmValueFTL& _LogLevel = _Return.getLogLevel();
	CmValueFTL& _Message = _Return.getMessage();
	CmValueFTL& _Context = _Return.getContext();

	// check initialization string availability
	if (NULL == StringINI) return false;
	if (false == _InfoFTL.processStringFTL(*StringINI)) return false;

	// synchronize _InfoFTL with current ValueINI values
	try {
		syncSettings(_InfoFTL);
	}
	catch (CmException e) {
		_LogLevel = uint64(e.getData());
		_Message.setText(e.getMessage());
		_Context.setText(e.getContext());
		return false;
	}

	return true;
}
bool CmValueINI::getStringFTL(CmString& _StringFTL, CmValueINI& _Return)
{
	CmStringFTL InfoFTL;
	if (false == getInfoFTL(InfoFTL, _Return)) return false;

	if (false == InfoFTL.serializeStringFTL(_StringFTL)) return false;

	return true;
}
bool CmValueINI::setStringINI(CmString& _StringINI)
{
	// create StringINI 
	if (NULL == StringINI) {
		StringINI = CmString::allocateMemory<CmString>(1, isCmString);
	}
	// set StringINI 
	*StringINI = _StringINI;

	return true;
}
CmString* CmValueINI::getStringINI()
{
	return StringINI;
}
bool CmValueINI::clearWorkspace()
{
	MatrixINI = NULL;
	StringINI = NULL;
	return true;
}
