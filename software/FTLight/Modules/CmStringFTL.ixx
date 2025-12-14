//////////////////////////////////////////////////////////////////////////////
//
// CmStringFTL.ixx - Module for CmStringFTL classes
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

/*------CmStringFTL--------------------ToDo-----------------------------------
*
*  - process IP as text, e.g. 192.168.42.30
*  - configuration of "0.02" failed
*
*/

module;

#include "Services/Basic/Modules/CmTypes.h"

#include "Shlwapi.h"

export module CmFTLight:CmStringFTL;

export import CmBasic;

//export import CmNetwork;

export namespace Cosmos
{

// radix for FTL conversion
#define RADIX_216 216

//----------------------------------------------------------------------------
// CmParallel class
//----------------------------------------------------------------------------
//

/** CmParallel.
*  A function will run in parallel to other functionality in a program by
*  letting it run in a separate thread. Additionally, an identical second
*  function can be started in another thread in order to have watchdog and
*  other timer functionality available for assisting parallel computing.
*
* see: https://stackoverflow.com/questions/1372967/how-do-you-use-createthread-for-functions-which-are-class-members
*/

// NOTE: The declaration of CmParallel went to CmDateTime in order
//       to avoid a circle dependency.
class CmParallel;

//=======================CmStringFTL=declarations=========================

/** FTLight data type identifiers (little endian)
*	The data type identifiers have been defined as symbols
*   from the 216-radix based FTL conding. The identifiers
*   are unused symbols and start from highest 216^4-1 value.
*
*   NOTE: FTLight data types will be represented little endian!
*/
enum FTLight_DATA_TYPE_IDENTIFIER {
	DTI_FTLight_OPEN = FTLight_DTI_MAX - 0,
	DTI_FTLight_WRAP = FTLight_DTI_MAX - 1,
	DTI_MCL = FTLight_DTI_MAX - 2,
	DTI_FTL = FTLight_DTI_MAX - 3,
	DTI_TXL = FTLight_DTI_MAX - 4,
	DTI_UNIT = FTLight_DTI_MAX - 5,
	DTI_TIME = FTLight_DTI_MAX - 6,
	DTI_TOKEN = FTLight_DTI_MAX - 7,
	DTI_LINK = FTLight_DTI_MAX - 8,
	DTI_DIF = FTLight_DTI_MAX - 9,

	// unknown type
	FTLight_NONE = FTLight_DTI_MIN
};

/** CmTypeFTL.
*   The FTLight specification uses special characters, data type identifiers
*   along with rules to conclude about the type of data that comes as items
*   in a FTLight line. The following definitions refer to those data types.
*/
enum CmTypeFTL {
	TYPEFTL_NONE = 0,
	TYPEFTL_OPERATOR = 1,
	TYPEFTL_LOCATION = 2,
	TYPEFTL_IDENTIFIER = 3,
	TYPEFTL_QUERY = 4,
	TYPEFTL_TXL = 5,
	TYPEFTL_TXL_BINFTL = 6,
	TYPEFTL_NUMBER = 7,
	TYPEFTL_NUMBER_BINFTL = 8,
	TYPEFTL_Local = 9,
	TYPEFTL_Local_BINFTL = 10,
	TYPEFTL_FIXED_PARENTS = 11,
	TYPEFTL_CHECKSUM = 12,
	TYPEFTL_OPTION = 13,
	TYPEFTL_DEFAULT = 14,
	TYPEFTL_COMMENT = 15,
	TYPEFTL_BINFTL = 16,
	TYPEFTL_CONTROLFTL = 17,
	TYPEFTL_FTLightOPEN = 18,
	TYPEFTL_FTLightWRAP = 19,
	TYPEFTL_DTI_MCL = 20,
	TYPEFTL_BINFTLBINARY = 21,
	TYPEFTL_BINFTLSTRING = 22,
	TYPEFTL_BINFTLVALUE = 23,
	TYPEFTL_BINFTLTIME = 24,
	TYPEFTL_CMFTLTOKEN = 25,
	TYPEFTL_CMFTLLINK = 26,
	TYPEFTL_BINDIF = 27,

};

/** CmDataFormat
*   When converting to and from FTLight, the following data formats apply.
*/
enum CmDataFormat {
	DATAFORMAT_NONE = 0,
	DATAFORMAT_FTLight = 1,
	DATAFORMAT_STRING = 2,
	DATAFORMAT_BINARY = 3,
	DATAFORMAT_UINT64 = 4,
	DATAFORMAT_INT64 = 5,
	DATAFORMAT_DOUBLE = 6,
	DATAFORMAT_UINT32 = 7,
	DATAFORMAT_INT32 = 8,
	DATAFORMAT_FLOAT = 9,
	DATAFORMAT_UINT16 = 10,
	DATAFORMAT_INT16 = 11,
	DATAFORMAT_UINT8 = 12,
	DATAFORMAT_INT8 = 13,
	DATAFORMAT_BOOL = 14,
	// vector elements
	DATAFORMAT_MATRIX = 15,

};

/** CmUnit.
*  Physical units can be involved according to the following enumeration.
*/
enum CmPhysicalUnit {
	UNIT_NONE = 0,						// without unit
	UNIT_TIME = 1,						// second, [s]
	UNIT_LENGTH = 2,					// meter, [m]
	UNIT_MASS = 3,						// kilogram, [kg]
	UNIT_CURRENT = 4,					// ampere, [A]
	UNIT_TEMPERATURE = 5,				// kelvin, [K]
	UNIT_QUANTITY_OF_MATERIAL = 6,		// mol, [mol]
	UNIT_LIGHT_INTENSITY = 7,			// candela, [cd]
	UNIT_PLANE_ANGLE = 8,				// radiant, [rad]
	UNIT_SOLID_ANGLE = 9,				// steradiant, [sr]
	UNIT_FREQUENCY = 10,				// hertz, [Hz]
	UNIT_FORCE = 11,					// newton, [N]
	UNIT_PRESSURE = 12,					// pascal, [Pa]
	UNIT_ENERGY_WORK_HEAT = 13,			// joule, [J]
	UNIT_POWER = 14,					// watt, [W]
	UNIT_ELECTRICAL_CHARGE = 15,		// coulomb, [C]
	UNIT_VOLTAGE = 16,					// volt, [V]
	UNIT_CAPACITANCE = 17,				// farad, [F]
	UNIT_ELECTRICAL_RESISTANCE = 18,	// ohm, [ohm]
	UNIT_SUSCEPTANCE = 19,				// siemens, [S]
	UNIT_MAGNETIC_FLUX = 20,			// weber, [Wb]
	UNIT_MAGNETIC_FLUX_DENSITY = 21,	// tesla, [T]
	UNIT_INDUCTANCE = 22,				// henry, [H]
	UNIT_TEMPERATURE_CELSIUS = 23,		// degree celsius, [°C]
	UNIT_LUMINOUS_FLUX = 24,			// lumen, [lm]
	UNIT_LUMINANCE_INTENSITY = 25,		// lux, [lx]
	UNIT_RADIOACTIVITY = 26,			// becquerel, [Bq]
	UNIT_ABSORBED_DOSE = 27,			// gray, [Gy]
	UNIT_DOSE_EQUIVALENT = 28,			// sievert, [Sv]
	UNIT_CATALYTIC_ACTIVITY = 29,		// katal, [kat]

};

/** CmLockID.
*  The following LockIDs will be used when protecting parallel program execution paths.
*/
#define LOCKID_RANGE	1000
enum CmLockID {
	// CmLockID
	CMLOCKID_NONE = 0,

	// CmStringFTL (0)
	CMLOCKID_StringFTL_LogAccess = CMLOCKID_NONE,

	// CmServiceAccess (1000)
	CMLOCKID_CmServiceAccess = CMLOCKID_StringFTL_LogAccess + LOCKID_RANGE,
	CMLOCKID_CmServiceAccess_switchServiceAccess = 1 + CMLOCKID_CmServiceAccess,
	CMLOCKID_CmServiceAccess_shutdownConnections = 2 + CMLOCKID_CmServiceAccess,
	CMLOCKID_CmServiceAccess_deleteIdleConnections = 3 + CMLOCKID_CmServiceAccess,
	CMLOCKID_CmServiceAccess_deleteConnections = 4 + CMLOCKID_CmServiceAccess,

	// SERVICE_CmGateway (2000)
	CMLOCKID_SERVICE_CmGateway = CMLOCKID_CmServiceAccess + LOCKID_RANGE,
	CMLOCKID_SERVICE_CmGateway_Provider = 1 + CMLOCKID_SERVICE_CmGateway,
	CMLOCKID_SERVICE_CmGateway_addToGatewayConnectionList = 2 + CMLOCKID_SERVICE_CmGateway,
	CMLOCKID_SERVICE_CmGateway_cleanupGatewayConnections = 3 + CMLOCKID_SERVICE_CmGateway,
	CMLOCKID_SERVICE_CmGateway_sendInfoToGateway = 4 + CMLOCKID_SERVICE_CmGateway,

	// CmApplication (3000)
	CMLOCKID_CmApplication = CMLOCKID_SERVICE_CmGateway + LOCKID_RANGE,
	// CmModule (4000)
	CMLOCKID_CmModule = CMLOCKID_CmApplication + LOCKID_RANGE,
	CMLOCKID_CmModule2 = CMLOCKID_CmModule + LOCKID_RANGE,
	CMLOCKID_CmModule3 = CMLOCKID_CmModule2 + LOCKID_RANGE,
	// CmDevice (7000)
	CMLOCKID_CmDevice = CMLOCKID_CmModule3 + LOCKID_RANGE,
	CMLOCKID_CmDevice2 = CMLOCKID_CmDevice + LOCKID_RANGE,
	CMLOCKID_CmDevice3 = CMLOCKID_CmDevice2 + LOCKID_RANGE,

};



//----------------------------------------------------------------------------
// CmStringFTL class
//----------------------------------------------------------------------------
//
// NOTE: The declaration of CmStringFTL went to CmServiceConnection in order
//       to avoid a circle dependency.

	/** CmStringFTL.
	*   The class CmStringFTL represents a single node in a FTLight hierarchy.
	*   On the one hand it provides for converting all data types to and from
	*   FTLight data formats. On the other hand, it forms the structure of a
	*   FTLight hierarchy by connecting 'child' and 'sibling' nodes.
	*/
class CmStringFTL : public CmString, public CmParallel
{
public:
	CmStringFTL(CmStringFTL* _Parent = NULL);
	CmStringFTL(const int8* _Text, CmStringFTL* _Parent = NULL);
	virtual ~CmStringFTL();

public:
	/** unit test for CmStringFTL */
	bool testCmStringFTL();

	//===================Public-functions=================================

	//-------------------TXL-functions------------------------------------

public:
	/** setTXL.
	*  A string will be stored in TXL and the data format will be adjusted to DATAFORMAT_NONE
	*/
	bool setTXL(const int8* _Text);
	bool setTXL(CmString& _Text);

public:
	/** getTXL/LengthTXL
	*   A FTLight encoded string / its length will be returned.
	*/
	const uint8* getTXL();
	int64 getLengthTXL();

public:
	/** encodeTXL.
	*   A text string will be converted into a FTLight compliant string.
	*   Basically, some defined special characters are escaped by '\'.
	*/
	CmStringFTL& encodeTXL(const int8* _Text = NULL);
	CmStringFTL& encodeTXL(CmString& _Text);

public:
	/** decodeTXL.
	*   A FTLight encoded string will be returned to original string.
	*   Basically, escaping of some defined special characters by a
	*   backslash will be removed.
	*   If TXL==NULL then the content of the CmString will be decoded.
	*/
	void decodeTXL(const int8* _TXL = NULL, size_t _LengthTXL = 0);
	void decodeTXL(CmString& _TXL);

	//-------------------FTL-functions------------------------------------

public:
	/** encodeFTL.
	*   A binary field will be converted into a FTLight compliant string.
	*   Basically, it will be encoded by symbols of radix 216.
	*   If Data==NULL then the content of the CmString will be encoded.
	*   The binary input data will be preserved in the base CmString class.
	*   The encoded FTL string will be available in TXL.
	*/
	void encodeFTL(const uint8* _Data = NULL, size_t _Length = 0);
	void encodeFTL(CmString& _Data);

public:
	/** decodeFTL.
	*   A FTLight encoded string will be returned to original binary field.
	*   Basically, it will be restored from symbols of radix 216.
	*   If Data==NULL then the content of the CmString will be decoded.
	*   The FTL input string will be preserved in TXL.
	*   The decoded binary data will be in the base CmString class.
	*/
	void decodeFTL(const uint8* _Data = NULL, size_t _Length = 0);
	void decodeFTL(CmString& _Data);

public:
	/** build/clear conversion table */
	bool isConversionTable();
	bool clearConversionTable();

	//-------------------NUM-functions------------------------------------

public:
	/** encodeNUM.
	*	A 64-bit binary field will be encoded into a FTL string. Usually,
	*   any data type from a single bit logical value or a byte upto 64-bit
	*   data types will first be casted/copied into an unsigned 64-bit field.
	*   Unused bits should be filled with 0. The resulting FTL string will
	*   encompass the minimal byte number that is needed to represent the
	*   valid bits without leading zeros. A negative value will be converted
	*   into a positive value and will be preceded by a back-apostroph (`).
	*/
	void encodeNUM(uint64 _Value);
	void encodeNUM(int64 _Value);
	void encodeNUM(double _Value);
	void encodeNUM(uint32 _Value);
	void encodeNUM(int32 _Value);
	void encodeNUM(float _Value);
	void encodeNUM(uint16 _Value);
	void encodeNUM(int16 _Value);
	void encodeNUM(uint8 _Value);
	void encodeNUM(int8 _Value);
	void encodeNUM(bool _Value);

public:
	/** decodeNUM.
	*   A FTL string will be converted back into a 64-bit binary field.
	*   Dependend on the meaning of the value, the 64-bit field will usually
	*   be casted/converted into the target data type which may contain less
	*   then 64 bits.
	*/
	uint64 decodeNUM2uint64(uint64* _Value = NULL);
	int64 decodeNUM2int64(int64* _Value = NULL);
	double decodeNUM2double(double* _Value = NULL);
	uint32 decodeNUM2uint32(uint32* _Value = NULL);
	int32 decodeNUM2int32(int32* _Value = NULL);
	float decodeNUM2float(float* _Value = NULL);
	uint16 decodeNUM2uint16(uint16* _Value = NULL);
	int16 decodeNUM2int16(int16* _Value = NULL);
	uint8 decodeNUM2uint8(uint8* _Value = NULL);
	int8 decodeNUM2int8(int8* _Value = NULL);
	bool decodeNUM2bool(bool* _Value = NULL);

public:
	/** decodeNum.
	*   A Num (number) string will be converted into a specified data format.
	*/
	bool decodeNum(CmDataFormat _DataFormat);

public:
	/**
	*   Convert 64-bit values to and from a FTL string
	*/
	CmString* num2FTL(CmString* _pFTL, uint64 _u64Num);
	CmString* num2FTL(CmString* _pFTL, uint64* _u64Num);
	CmString* sigNum2FTL(CmString* _pFTL, int64 _64Num);
	static uint64 FTL2num(CmString& _mFTL);
	static int64 FTL2sigNum(CmString& _mFTL);


	//-------------------UNIT-functions------------------------------------

public:
	/** encode/decodeUNIT.
	*  A physical unit will be FTLight encoded.
	*/
	bool encodeUNIT(CmString& _UNIT, CmPhysicalUnit _Unit, int64 _Factor = 1, int64 _Divisor = 1, int64 _Base = 1, int64 _Exponent = 0, bool _isValueNegative = false);
	bool decodeUNIT(const CmString& _UNIT, CmPhysicalUnit& _Unit, double& _Value);

public:
	/** isInteger. A string will be evaluated for being an integer (+-0..9) */
	bool isInteger();

	//-------------------TIME-functions-----------------------------------

public:
	/** encode/decodeTIME.
	* A FTLight format string for a time unit will be encoded.
	*/
	bool encodeTIME(CmString& _TIME, int64 _Factor = 1, int64 _Divisor = 1, int64 _Base = 1, int64 _Exponent = 0, bool _isTimeNegative = false);
	bool decodeTIME(const CmString& _TIME, double& _Time);


	//-------------------Flex-functions-----------------------------------

public:
	/** flex2Double. A CmFlex value will be converted to double */
	double flex2Double(CmFlex& _Flex, CmDataFormat _DataFormat);
	double flex2Double(CmFlex& _Flex, CmDataFormat _DataFormat, CmString& _Item);

public:
	/** double2Flex. A double value will be converted to CmFlex of a dataformat */
	bool double2Flex(CmFlex& _Flex, CmDataFormat _DataFormat);

	//-------------------XML-functions------------------------------------

public:
	/** convertFTLight2XML.
	*   A FTLight encoded file will be analyzed and converted to a XML file.
	*/
	bool convertFTL2XML(const CmString& _FileFTL, const CmString& _FileXML);
	bool convertFTL2XML(const int8* _FileFTL, const int8* _FileXML);


	//-------------------FTLight structure-functionality------------------

public:
	/** clear FTLight hierarchy */
	bool clearFTLightHierarchy();

public:
	/** getIdentifier.
	*  First ItemFTL will be read from an Info StringFTL and its type will
	*  be checked against the identifier type (=IFTL).
	*/
	static bool getIdentifier(CmString& _Info, CmIFTL& _IFTL);

public:
	/** setParent. */
	void setParent(CmStringFTL* _Parent);

public:
	/** getParent/Sibling/Child/TypeFTL
	*	A pointer to requested node/type will be returned.
	*/
	CmStringFTL* getParent();
	CmStringFTL* getSibling();
	CmStringFTL* getChild();
	bool isBinary(bool _isParentBinary = false);

public:
	/** set/getTypeFTL. The TypeFTL value will be set/get
	*/
	CmTypeFTL setTypeFTL(CmTypeFTL _TypeFTL);
	CmTypeFTL getTypeFTL();

public:
	/** addChild.
	*   A new CmStringFTL will be appended to child position. If the child position
	*   is already occupied then a new child will be inserted into a sibling list.
	*/
	bool addChild(CmStringFTL** _Child);

public:
	/** findChild.
	*   A child with a specified content will be returned if one could be found.
	*/
	bool findChild(CmStringFTL** _Child, CmString& _Content, CmTypeFTL _TypeFTL = TYPEFTL_TXL);

public:
	/** receiveLineFTL.
	*   A subsequent FTLight line will be inserted into current hierarchy of CmStringFTL nodes.
	*/
	bool receiveLineFTL(CmString& _LineFTL);

public:
	/** processStringFTL.
	*   A string with multiple LineFTL items will be inserted into hierarchy of CmStringFTL nodes.
	*/
	bool processStringFTL(const int8* _StringFTL);
	bool processStringFTL(CmString& _StringFTL);

public:
	/** readFileFTL.
	*   A file will be read and converted into a hierarchy of CmStringFTL nodes
	*/
	bool readFileFTL(const int8* _FileFTL);

public:
	/** serializeStringFTL
	*  A hierarchy of CmStringFTL nodes will be converted to a string.
	*/
	bool serializeStringFTL(CmString& _StringFTL);

public:
	/** getNode.
	*   The node at a specified position will be returned. If there is no node at that position
	*   then one will be created. In that case there is no content available (empty).
	*/
	const CmString& getNode(const CmString& _Address);

public:
	/** addItemFTL.
	*   A next binary item will be added to a FTLight line collection
	*/
	bool addItemFTL(const CmString& _Val);
	bool addItemFTL(void* _Val);
	bool addItemFTL(uint64 _Val);
	bool addItemFTL(int64 _Val);
	bool addItemFTL(double _Val);
	bool addItemFTL(uint32 _Val);
	bool addItemFTL(int32 _Val);
	bool addItemFTL(float _Val);
	bool addItemFTL(uint16 _Val);
	bool addItemFTL(int16 _Val);
	bool addItemFTL(uint8 _Val);
	bool addItemFTL(int8 _Val);
	bool addItemFTL(bool _Val);

public:
	/** set/getDataFormat.
	*   Current format of the data in the base CmString buffer will be returned
	*/
	CmDataFormat setDataFormat(CmDataFormat _DataFormat);
	CmDataFormat getDataFormat();
	CmDataFormat getDataFormat(CmString& _DataFormat);

public:
	/** insert/set/getValueFTL.
	*   A CmStringFTL will be inserted/copied to/from specified address in a FTLight hierarchy.
	*/
	bool insertValueFTL(const CmString& _Address, const CmString& _ValueFTL);
	bool setValueFTL(const CmString& _Address, const CmString& _ValueFTL);
	bool getValueFTL(const CmString& _Address, CmStringFTL& _ValueFTL, CmString _StartAddr = CmString());

public:
	/** getValue.
	*   A value will be restored from ItemFTL at specified address in a FTLight hierarchy.
	*/
	bool getValue(const char* _Address, CmDataFormat _DataFormat, CmStringFTL** _Val);
	bool getValue(const char* _Address, CmString& _Val);
	bool getValue(const char* _Address, void*& _Val);
	bool getValue(const char* _Address, uint64& _Val);
	bool getValue(const char* _Address, int64& _Val);
	bool getValue(const char* _Address, double& _Val);
	bool getValue(const char* _Address, uint32& _Val);
	bool getValue(const char* _Address, int32& _Val);
	bool getValue(const char* _Address, float& _Val);
	bool getValue(const char* _Address, uint16& _Val);
	bool getValue(const char* _Address, int16& _Val);
	bool getValue(const char* _Address, uint8& _Val);
	bool getValue(const char* _Address, int8& _Val);
	bool getValue(const char* _Address, bool& _Val);

	bool getValue(const CmString& _Addr, CmDataFormat _DataFormat, CmStringFTL** _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, CmString& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, void*& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, uint64& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, int64& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, double& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, uint32& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, int32& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, float& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, uint16& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, int16& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, uint8& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, int8& _Val, CmString _StartAddr = CmString());
	bool getValue(const CmString& _Addr, bool& _Val, CmString _StartAddr = CmString());

public:
	/** getDataItem/Value.
	*   Return data item at specified position inside a collection on specified path
	*/
	//bool getDataItem(CmFlex _Flex, CmDataFormat _Format, const CmVector<const int8*>& _Path, int32 _Index);
	bool getDataItem(CmString& _DataItem, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(void*& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(uint64& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(int64& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(double& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(uint32& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(int32& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	bool getDataValue(float& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(uint16& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(int16& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(uint8& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(int8& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);
	//bool getDataValue(bool& _DataValue, CmVector<CmString*>& _Path, int32 _Index = 0);

public:
	/** incrementSequenceNumber()
	*	The sequence number for volatile synchronous writing will be incremented and returned.
	*/
	int64 incrementSequenceNumber();

public:
	/** getConfigPath.
	*   A configuration path will be determined and created if it does not exist yet.
	*/
	bool getConfigPath(CmString _AppData, CmString _AppCompany, CmString _AppProgram, CmString _AppVersion);

public:
	/** for CmFileFTL usage */
	static bool isDataTypeIdentifier(CmString& _FTLight, FTLight_DATA_TYPE_IDENTIFIER _eDataType);

public:
	/** convert type identifiers to and from FTL format */
	bool setFTLightDataType(CmString& _FTL, FTLight_DATA_TYPE_IDENTIFIER _FTLightDataType);
	bool getFTLightDataType(CmString& _FTL, FTLight_DATA_TYPE_IDENTIFIER& _FTLightDataType);

	//===================Private-functions================================

	//-------------------FTL----------------------------------------------
private:
	// Convert symbols 0..215 into legal FTL characters 
	uint8 Symbol2FTL(uint8 _Symbol);
	uint8 FTL2Symbol(uint8 _FTL);

private:
	/** getLengthTXL2bin.
	*   Resulting binary length from a FTL2bin conversion will be reported. If
	*   fTail==true then incomplete 4-byte quadrupel will be counted as well.
	*/
	static size_t getLengthTXL2bin(size_t _LengthTXL, bool _fTail = false);

private:
	/** bin2FTL.
	*   A binary field will be converted into a FTL string. If nDigits==0 then the
	*   FTL length will be determined from the length of the binary field.
	*/
	CmString* bin2FTL(CmString* _pFTL, const uint8* _pBuf, int32 _nBinLen, int32 _nDigits = 0);

private:
	/** FTL2bin.
	*   A FTL string will be converted back into original binary field. If fTail==true
	*   then incomplete 4-byte quadrupel will be converted as well.
	*/
	static int32 FTL2bin(uint8* _pBuf, int32 _nBufSize, CmString& _mFTL, bool _fTail = false);

	//-------------------MCL----------------------------------------------

private:
	/** Convert binary data to and from a MCL string */
	bool bin2MCL(int _n, uint16* _inp, uint16* _out);
	bool MCL2bin(int _n, uint16* _inp, uint16* _out);

	//-------------------DIF----------------------------------------------

private:
	/** Convert binary data to and from DIF format */
	bool val2DIF(CmVector<uint8>& _DIF, CmVector<int64>& _Values, int32 _insertAbsoluteValue = 62);
	bool val2DIF(CmVector<uint8>& _DIF, CmVector<uint8>& _Values, bool _isEntangled = false, int32 _insertAbsoluteValue = 65536);
	bool DIF2val(CmVector<uint8>& _DIF, CmVector<int64>& _Values);
	bool DIF2val(CmVector<uint8>& _DIF, CmVector<uint8>& _Values, bool _isEntangled);
	bool asVector(CmVector<int64>& _Data);
	int64 getDigits216(int64& _Value);

	//-------------------FTLight-structure-functionality------------------

private:
	/** createRoot.
	*   A standard node hierarchy consisting of an operator node, a location node
	*   and an identifier node will be generated if it does not exist yet. The
	*   identifier node will become the root of further items to be inserted.
	*   Alternatively, an empty root node can be created.
	*/
	bool createRoot(CmString& _Identifier);

protected:
	bool createRoot();

private:
	/** getParent.
	*   The parent node will be returned
	*/
	bool getParent(CmStringFTL** _Parent);

private:
	/** getChild.
	*   Find or create a child node at specified position
	*/
	bool getChild(int32 _ChildPosition);

private:
	/** isChild.
	*   Find a child node at specified position
	*/
	bool isChild(int32 _ChildPosition);

private:
	/** getLastChild.
	*   The child that was most recently inserted will be returned.
	*/
	bool getLastChild(CmStringFTL** _LastChild);

private:
	/** getNextItemFTL.
	*	The next item in a FTLight line as well as its type will be determined.
	*   The active range in LineFTL will be shortened by found ItemFTL and TypeFTL
	*   information. The ItemFTL position and length are returned as 'highlight'.
	*   When starting a new line, 'highlight' needs to be set to 'active' range.
	*/
	bool getNextItemFTL(CmString& _LineFTL, CmString& _ItemFTL, CmTypeFTL& _TypeFTL, bool& _isStartOfCollection, bool _isParentBinary);

private:
	/** getChecksum.
	*   Current line's checksum will be evaluated.
	*/
	bool getChecksum(CmString& _Line, CmString& _Checksum);

private:
	/** verifyChecksum.
	*   Current line's checksum will be evaluated and be checked against the
	*   checksum that was attached as last item at the end of line. The range
	*   depends on checksum's length
	*/
	bool verifyChecksum(CmString& _Line, CmString& _Checksum);

private:
	/** getPosition.
	*   A position according to specified address will be adjusted with PosFTL.
	*/
	bool getPosition(const CmString& _Address);

private:
	/** findPosition.
	*   A position according to specified address will be searched for. If it
	*   exists then its address will be returned in PosFTL.
	*   Additionally, a start address can be specified which will replace the
	*   first part of the main Address.
	*/
	bool findPosition(const CmString& _Address, CmString _StartAddr = CmString());

private:
	/** getLocal.
	*   A Local will be returned according to received address string
	*/
	bool getLocal(const CmString& _Address, CmStringFTL*& _LocalFTL);

private:
	/** initLineFTL.
	*   The LineFTL reader will be prepared for reading next line.
	*/
	bool initLineFTL(CmString& _LineFTL, uint32& _LineStart, uint32& _ActiveStart, uint32& _ActiveLength, uint32& _HighlightStart, uint32& _HighlightLength);

private:
	/** checkSyncronousWriting.
	*   The first item in a line will be checked whether syncronous writing takes place.
	*/
	bool checkSyncronousWriting(CmTypeFTL _TypeFTL);

private:
	/** runSynchronousWriting.
	*   An item will be handled in the context of a synchronous writing operation.
	*   If an item was consumed then the return is true or otherwise false.
	*/
	bool runSynchronousWriting(CmString& _ItemFTL, CmTypeFTL _TypeFTL);

private:
	/** evaluateRepeatedPath.
	*   In case of a zero-length item, a repeated path will be evaluated.
	*   The return value will be true for a repeated path item.
	*/
	bool evaluateRepeatedPath();

private:
	/** putChildOnPath.
	*   The final path will be evaluated and a child be generated on it.
	*/
	bool putChildOnPath(CmString& _ItemFTL, CmTypeFTL& _TypeFTL, CmStringFTL* _LocalFTL = NULL);

	//--------workspace---------------------------------------------------

private:
	// FTLight structure and type
	CmStringFTL* Parent;
	CmStringFTL* Sibling;
	CmStringFTL* Child;

protected:
	CmTypeFTL TypeFTL;			// TypeFTL according to FTLight specification
	CmDataFormat DataFormat;	// content type in CmString, e.g DATAFORMAT_FTLight,
	// for all other formats the FTLight content may be in TXL/LengthTXL

protected:
	// FTLight content
	size_t LengthTXL;
	uint8* TXL;

private:
	typedef struct _NaviFTL {
		// positions in FTLight hierarchy
		CmStringFTL* Root;						// current FTLight hierarchy's root node
		CmStringFTL* PosFTL;					// current position in FTLight hierarchy
		CmStringFTL* PathFTL;					// current path in FTLight hierarchy

		// FTLight state/positions for synchronous writing
		struct {
			bool isSynchronousWriting;
			bool isFixedParentCollection;
			// hierarchy positions/parameters
			int64 FirstParentPos;
			int64 ParentPos;
			int64 Levels;
			int64 SequenceNumber;
			int64 BufferLength;
			int64 BufferPos;
		} sync;

		// parser state
		bool isPath;
		bool isRepeatedPath;
		bool isFirstItem;
		bool isOption;
		bool isStartOfCollection;
		bool isWaitForBufferStart;
		bool isWaitForBufferLength;
		bool isSingleAddressBeforeAt;
		bool isRestoreSynchronousWriting;
		bool isParentBinary;

	} NaviFTL;

private:
	// FTLight tree navigation
	NaviFTL* Navigation;
	NaviFTL* FTL();

};

//----------------------------------------------------------------------------
// CmStringFTLroot class
//----------------------------------------------------------------------------
//
/** CmStringFTLroot.
*   During initialization of this class, members will be adjusted such that it
*   can function as an (empty) root in a CmStringFTL hierarchy (without identifier).
*/
class CmStringFTLroot : public CmStringFTL
{
public:
	CmStringFTLroot();
	~CmStringFTLroot();
};

} // namespace Cosmos

using namespace Cosmos;

/** FTL conversion */
uint8* BINFTL = NULL;
uint8* SYMBOL = NULL;
const int32 FTLconvertSize = 256;		    // number of entries in the FTL conversion table

/** MCL conversion */
uint16* MCLconvert = NULL;				// MCL conversion table
const int32 MCLconvertSize = 32768;		// number of entries in the MCL conversion table

/** CmStringFTL bookkeeping */
int32 CountStringFTL = 0;

//----------------------------------------------------------------------------
// CmStringFTL class.
//----------------------------------------------------------------------------
CmStringFTL::CmStringFTL(CmStringFTL* _Parent)
{
	// initialize workspace
	Parent = _Parent;
	Sibling = this;
	Child = NULL;
	TXL = NULL;
	Navigation = NULL;
	LineNumber = 0;
	LengthTXL = 0;
	DataFormat = DATAFORMAT_NONE;

	// without a parent this will be a top operator node
	if (NULL == Parent)
		TypeFTL = TYPEFTL_OPERATOR;
	else
		TypeFTL = TYPEFTL_TXL;

	// counter for MCLconvert bookkeeping
	CountStringFTL++;
}

CmStringFTL::CmStringFTL(const int8* _Text, CmStringFTL* _Parent)
	:CmString(_Text)
{
	// initialize workspace
	Parent = _Parent;
	Sibling = this;
	Child = NULL;
	TXL = NULL;
	Navigation = NULL;
	LengthTXL = 0;
	DataFormat = DATAFORMAT_NONE;

	// convert input string to a FTLight compliant string
	encodeTXL(_Text);

	// counter for MCLconvert bookkeeping
	CountStringFTL++;
}

CmStringFTL::~CmStringFTL()
{
	clearFTLightHierarchy();

	// check if last CmStringFTL
	CountStringFTL--;
	if (0 == CountStringFTL) {

		clearConversionTable();
	}
}

// unit test for CmStringFTL
bool CmStringFTL::testCmStringFTL()
{
	// define test parameters
#define BIN_LEN		1421507
#define BIN_LEN_MCL (16 * BIN_LEN / 15)
#define STR_LEN		30
	CmString Memory;
	uint8* pField = Memory.allocateMemory<uint8>(BIN_LEN + 1, isUint8);
	uint8* pField1 = Memory.allocateMemory<uint8>(BIN_LEN + 1, isUint8);
	uint16* pMCL = Memory.allocateMemory<uint16>(BIN_LEN_MCL / 2, isUint16);
	CmString mFTL;
	CmString mFTL1(STR_LEN);
	CmString mFTL2(STR_LEN);
	CmVector<int64> ValDIF;
	CmVector<int64> ValDIF1;
	CmVector<int64> ValDIF16;
	CmVector<uint8> DIF;

	// generate test input 
	for (int i = 0; i < BIN_LEN; i++) {
		pField[i] = (uint8)(i * 43);
		ValDIF16[i] = int64(32767 * sin(double(i / (BIN_LEN / (2 * PI)))));
		ValDIF[i] = int64(1234567890123456789 * sin(double(i / (BIN_LEN / (2 * PI)))));
	}

	//========================Private-functions=(data-conversion)=============

	//-----------------------test-FTL-----------------------------------------

	// Convert binary to a FTL string
	bin2FTL(&mFTL, pField, BIN_LEN);

	// Convert a FTL string to binary
	FTL2bin(pField1, BIN_LEN, mFTL);

	// Compare
	if (0 != memcmp(pField, pField1, BIN_LEN)) {
		// show first and last positions of the FTL field
		MEMCPY(mFTL1.getBuffer(), mFTL1.getLength(), mFTL.getBuffer(), STR_LEN);
		MEMCPY(mFTL2.getBuffer(), mFTL2.getLength(), mFTL.getBuffer() + mFTL.getLength() - STR_LEN, STR_LEN);
		printf("\n FTL (%3d): \"%s.....%s\"", (int32)mFTL.getLength(), mFTL1.getBuffer(), mFTL2.getBuffer());
		printf("\n FTL conversion ---FAILED---\n");
		return false;
	}

	//-----------------------test-MCL-----------------------------------------

	// Convert binary to a MCL string
	bin2MCL(BIN_LEN / 30, (uint16*)pField, pMCL);

	// Convert a MCL string to binary
	MCL2bin(BIN_LEN / 30, pMCL, (uint16*)pField1);

	// Compare
	if (0 != memcmp(pField, pField1, BIN_LEN - BIN_LEN % 2)) {
		// show first and last positions of the MCL field
		MEMCPY(mFTL1.getBuffer(), mFTL1.getLength(), (char*)pMCL, STR_LEN);
		MEMCPY(mFTL2.getBuffer(), mFTL2.getLength(), (char*)pMCL + BIN_LEN_MCL - STR_LEN, STR_LEN);
		printf("\n MCL (%3d): \"%s.....%s\"", (int32)mFTL.getLength(), mFTL1.getBuffer(), mFTL2.getBuffer());
		printf("\n MCL conversion ---FAILED---");
		return false;
	}

	//-----------------------test-DIF-----------------------------------------

	// Convert values to DIF format
	val2DIF(DIF, ValDIF);

	// Convert from DIF format to values
	DIF2val(DIF, ValDIF1);

	if (ValDIF != ValDIF1) {
		printf("\n DIF conversion ---FAILED---");
		return false;
	}

	//========================Public-functions================================

	//------------------------test-TXL-functions-----------------------------

	// test strings containing all special characters
	//const int8* SpecialCharacters = "Test\n\r,-:;=@`\x7F\\end";
	//const int8* SpecialCharactersEscaped = "Test\\\n\\\r\\,\\-\\:\\;\\=\\@\\`\\\x7F\\end";
	// ToDo: escape '-' only in case of an address
	const int8* SpecialCharacters = "Test\n\r,:;=@`\x7F\\end";
	const int8* SpecialCharactersEscaped = "Test\\\n\\\r\\,\\:\\;\\=\\@\\`\\\x7F\\end";
	// binary field containing all byte values 255..0
	uint8 BinField[256];
	for (int i = sizeof(BinField) - 1; i >= 0; i--) {
		BinField[i] = (uint8)(255 - i);
	}

	// set/getTXL
	setTXL(SpecialCharacters);
	if (0 != strncmp((char*)TXL, SpecialCharacters, strlen(SpecialCharacters))) return false;

	// test encodeTestFTL()
	encodeTXL(SpecialCharacters);
	if (0 != strncmp((char*)TXL, SpecialCharactersEscaped, strlen(SpecialCharactersEscaped))) return false;

	// test in-place decodeTXL() 
	decodeTXL();
	if (*this != SpecialCharacters) return false;

	// test decodeTXL()
	decodeTXL(SpecialCharactersEscaped, strlen(SpecialCharactersEscaped));
	if (*this != SpecialCharacters) return false;

	// test in-place encodeTXL
	CmStringFTL Special(SpecialCharacters);
	CmString Escaped = (int8*)Special.encodeTXL().getTXL();
	if (Escaped != SpecialCharactersEscaped) return false;

	//-------------------------test-FTL-functions-----------------------------

	// test encode/decodeFTL()
	encodeFTL(BinField, sizeof(BinField));
	decodeFTL(TXL, LengthTXL);
	for (int i = 0; i < sizeof(BinField); i++) {
		if (BinField[i] != (uint8) * (pText + i)) 
			return false;
	}

	// in-place encode/decode
	CmStringFTL TestFTL;
	TestFTL.setData(BinField, sizeof(BinField));
	TestFTL.encodeFTL();
	setData(TestFTL.getTXL(), TestFTL.getLengthTXL());
	decodeFTL();
	for (int i = 0; i < sizeof(BinField); i++) {
		if (BinField[i] != (uint8) * (pText + i))
			return false;
	}

	//-------------------------test-NUM-functions-----------------------------

	// test encode/decodeNUM()
	uint64 TestVal64 = 0xFEDCBA9876543210;
	uint64 uint64Val, uint64Ret;
	int64 int64Val, int64Ret;
	double doubleVal, doubleRet;
	uint32 uint32Val, uint32Ret;
	int32 int32Val, int32Ret;
	float floatVal, floatRet;
	uint16 uint16Val, uint16Ret;
	int16 int16Val, int16Ret;
	uint8 uint8Val, uint8Ret;
	int8 int8Val, int8Ret;
	bool boolVal, boolRet;
	// uint64
	encodeNUM(TestVal64);
	setData(TXL, LengthTXL);
	uint64Ret = decodeNUM2uint64(&uint64Val);
	if ((uint64Ret != TestVal64) || (uint64Val != TestVal64)) return false;
	// int64
	encodeNUM((int64)TestVal64);
	setData(TXL, LengthTXL);
	int64Ret = decodeNUM2int64(&int64Val);
	if ((int64Ret != (int64)TestVal64) || (int64Val != (int64)TestVal64)) return false;
	// double
	encodeNUM(*((double*)&TestVal64));
	setData(TXL, LengthTXL);
	doubleRet = decodeNUM2double(&doubleVal);
	if ((doubleRet != *((double*)&TestVal64)) || (doubleVal != *((double*)&TestVal64))) return false;
	// uint32
	encodeNUM((uint32)TestVal64);
	setData(TXL, LengthTXL);
	uint32Ret = decodeNUM2uint32(&uint32Val);
	if ((uint32Ret != (uint32)TestVal64) || (uint32Val != (uint32)TestVal64)) return false;
	// int32
	encodeNUM((int32)TestVal64);
	setData(TXL, LengthTXL);
	int32Ret = decodeNUM2int32(&int32Val);
	if ((int32Ret != (int32)TestVal64) || (int32Val != (int32)TestVal64)) return false;
	// float
	encodeNUM(*((float*)&TestVal64));
	setData(TXL, LengthTXL);
	floatRet = decodeNUM2float(&floatVal);
	if ((floatRet != *((float*)&TestVal64)) || (floatVal != *((float*)&TestVal64))) return false;
	// uint16
	encodeNUM((uint16)TestVal64);
	setData(TXL, LengthTXL);
	uint16Ret = decodeNUM2uint16(&uint16Val);
	if ((uint16Ret != (uint16)TestVal64) || (uint16Val != (uint16)TestVal64)) return false;
	// int16
	encodeNUM((int16)TestVal64);
	setData(TXL, LengthTXL);
	int16Ret = decodeNUM2int16(&int16Val);
	if ((int16Ret != (int16)TestVal64) || (int16Val != (int16)TestVal64)) return false;
	// uint8
	encodeNUM((uint8)TestVal64);
	setData(TXL, LengthTXL);
	uint8Ret = decodeNUM2uint8(&uint8Val);
	if ((uint8Ret != (uint8)TestVal64) || (uint8Val != (uint8)TestVal64)) return false;
	// int8
	encodeNUM((int8)TestVal64);
	setData(TXL, LengthTXL);
	int8Ret = decodeNUM2int8(&int8Val);
	if ((int8Ret != (int8)TestVal64) || (int8Val != (int8)TestVal64)) return false;
	// bool
	encodeNUM(TestVal64 > 0);
	setData(TXL, LengthTXL);
	boolRet = decodeNUM2bool(&boolVal);
	if ((boolRet != (TestVal64 > 0)) || (boolVal != (TestVal64 > 0))) return false;

	// Convert 64-bit values to and from a FTL string
	uint64 u64Num = 85;
	uint64 u64Num1;
	uint64 u64Factor = 1;
	srand((unsigned)time(NULL));
	for (int i = 0; i < 9; i++) {
		if (i < 8) {
			u64Num = u64Factor + (rand() * rand()) % (u64Factor * RADIX_216);
		}
		else {
			u64Num += 0xC000000000000000;
		}
		num2FTL(&mFTL, &u64Num);
		u64Num1 = FTL2num(mFTL);
		if (i == 7) {
			// Extended search for failure
			for (int k = 0; k < 100000 && u64Num == u64Num1; k++) {
				u64Num = u64Factor + (rand() * rand()) % (u64Factor * RADIX_216);
				num2FTL(&mFTL, &u64Num);
				u64Num1 = FTL2num(mFTL);
			}
		}
		u64Num != u64Num1 ? printf("Numeric conversion (%20I64u): %9s len=%u (%20I64u) %s\n", u64Num, mFTL.getBuffer(), (int32)mFTL.getLength(), u64Num1, "failed") : 0;
		u64Factor *= RADIX_216;
	}

	// test signed 64-bit to FTL
	int64 NumPos = 123456789012345;
	int64 NumNeg = -123456789012345;
	int64 SigNum;
	CmString FTL;
	// positive number
	sigNum2FTL(&FTL, NumPos); SigNum = FTL2sigNum(FTL);
	SigNum != NumPos ? printf("Numeric conversion (%20I64d): %9s len=%d (%20I64d) failed\n", NumPos, FTL.getBuffer(), (int32)FTL.getLength(), SigNum) : 0;
	// negative number
	sigNum2FTL(&FTL, NumNeg); SigNum = FTL2sigNum(FTL);
	SigNum != NumNeg ? printf("Numeric conversion (%20I64d): %9s len=%d (%20I64d) failed\n", NumNeg, FTL.getBuffer(), (int32)FTL.getLength(), SigNum) : 0;


	//-----------------------test-VAL-functions-------------------------------
	
	bool Success = true;
	double Value;
	CmString Format;
	CmPhysicalUnit Unit = UNIT_NONE, Unit1;
	bool isValueNegative = false;
	// default
	int64 Factor = 1, Divisor = 1, Base = 10, Exponent = 0; isValueNegative = false;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// negative value
	Factor = 1, Divisor = 1, Base = 10, Exponent = 0; isValueNegative = true;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// unit
	Unit = UNIT_VOLTAGE; Factor = 1; Divisor = 1; Base = 10; Exponent = 0; isValueNegative = false;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// unit & negative value
	Unit = UNIT_ENERGY_WORK_HEAT; Factor = 1; Divisor = 1; Base = 10; Exponent = 0; isValueNegative = true;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// factor
	Unit = UNIT_CURRENT; Factor = 23; Divisor = 1; Base = 10; Exponent = 0; isValueNegative = false;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// factor & negative value
	Unit = UNIT_CURRENT; Factor = 23; Divisor = 1; Base = 10; Exponent = 0; isValueNegative = true;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// divisor
	Unit = UNIT_FREQUENCY; Factor = 23; Divisor = 1000; Base = 10; Exponent = 0; isValueNegative = false;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// divisor & negative value
	Unit = UNIT_FREQUENCY; Factor = 23; Divisor = 1000; Base = 10; Exponent = 0; isValueNegative = true;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// exponent
	Unit = UNIT_MASS; Factor = 23; Divisor = 1000; Base = 10; Exponent = 5; isValueNegative = false;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// exponent & negative value
	Unit = UNIT_POWER; Factor = 23; Divisor = 1000; Base = 10; Exponent = 5; isValueNegative = true;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// negative exponent
	Unit = UNIT_FORCE; Factor = 23; Divisor = 1000; Base = 10; Exponent = -5; isValueNegative = false;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// negative exponent & negative value
	Unit = UNIT_LENGTH; Factor = 23; Divisor = 1000; Base = 10; Exponent = -5; isValueNegative = true;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// base
	Unit = UNIT_PRESSURE; Factor = 23; Divisor = 1000; Base = 4; Exponent = 5; isValueNegative = false;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// base & negative value
	Unit = UNIT_CAPACITANCE; Factor = 23; Divisor = 1000; Base = 4; Exponent = 5; isValueNegative = true;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// base & negative exponent
	Unit = UNIT_ELECTRICAL_RESISTANCE; Factor = 23; Divisor = 1000; Base = 4; Exponent = -5; isValueNegative = false;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// base & negative exponent & negative value
	Unit = UNIT_ELECTRICAL_CHARGE; Factor = 23; Divisor = 1000; Base = 4; Exponent = -5; isValueNegative = true;
	encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative); decodeUNIT(Format, Unit1, Value);
	if (Unit != Unit1 || Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;

	//-----------------------test-encode/decodeTIME---------------------------
	// default
	Factor = 1, Divisor = 1, Base = 10, Exponent = 0; isValueNegative = false;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// negative value
	Factor = 1, Divisor = 1, Base = 10, Exponent = 0; isValueNegative = true;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// factor
	Factor = 23, Divisor = 1, Base = 10, Exponent = 0; isValueNegative = false;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// factor & negative value
	Factor = 23, Divisor = 1, Base = 10, Exponent = 0; isValueNegative = true;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// divisor
	Factor = 23, Divisor = 1000, Base = 10, Exponent = 0; isValueNegative = false;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// divisor & negative value
	Factor = 23, Divisor = 1000, Base = 10, Exponent = 0; isValueNegative = true;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// exponent
	Factor = 23, Divisor = 1000, Base = 10, Exponent = 5; isValueNegative = false;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// exponent & negative value
	Factor = 23, Divisor = 1000, Base = 10, Exponent = 5; isValueNegative = true;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// negative exponent
	Factor = 23, Divisor = 1000, Base = 10, Exponent = -5; isValueNegative = false;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// negative exponent & negative value
	Factor = 23, Divisor = 1000, Base = 10, Exponent = -5; isValueNegative = true;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// base
	Factor = 23, Divisor = 1000, Base = 4, Exponent = 5; isValueNegative = false;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// base & negative value
	Factor = 23, Divisor = 1000, Base = 4, Exponent = 5; isValueNegative = true;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// base & negative exponent
	Factor = 23, Divisor = 1000, Base = 4, Exponent = -5; isValueNegative = false;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;
	// base & negative exponent & negative value
	Factor = 23, Divisor = 1000, Base = 4, Exponent = -5; isValueNegative = true;
	encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative); decodeTIME(Format, Value);
	if (Value != -Factor / double(Divisor) * pow(double(Base), double(Exponent))) Success = false;

	// evaluate success
	if (false == Success) return false;

	//-----------------------conversion-benchmarks----------------------------

	MBenchmark   mBenchmark(3);

	// benchmark FTL
	try {
		while (mBenchmark.run()) {
			bin2FTL(&mFTL, pField, BIN_LEN);
		}
		printf("\n bin2FTL          / Byte: %s", mBenchmark.info(14, BIN_LEN).getText());
	}
	catch (...) {
		return false;
	}
	// benchmark MCL
	try {
		while (mBenchmark.run()) {
			bin2MCL(BIN_LEN / 30, (uint16*)pField, pMCL);
		}
		printf("\n bin2MCL          / Byte: %s", mBenchmark.info(9, BIN_LEN).getText());
	}
	catch (...) {
		return false;
	}
	// benchmark DIF(16)
	try {
		while (mBenchmark.run()) {
			val2DIF(DIF, ValDIF16);
		}
		printf("\n bin2DIF(16)      / Byte: %s", mBenchmark.info(15, BIN_LEN * sizeof(int16)).getText());
	}
	catch (...) {
		return false;
	}
	// benchmark DIF(64)
	try {
		while (mBenchmark.run()) {
			val2DIF(DIF, ValDIF);
		}
		printf("\n bin2DIF(64)      / Byte: %s", mBenchmark.info(28, BIN_LEN * sizeof(int64)).getText());
	}
	catch (...) {
		return false;
	}

	// benchmark encode/decodeVAL
	try {
		while (mBenchmark.run()) {
			encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative);
		}
		printf("\n encodeVAL         / Run: %s", mBenchmark.info(10920, 1).getText());
	}
	catch (...) {
		return false;
	}
	try {
		while (mBenchmark.run()) {
			decodeUNIT(Format, Unit1, Value);
		}
		printf("\n decodeVAL         / Run: %s", mBenchmark.info(464, 1).getText());
	}
	catch (...) {
		return false;
	}

	// without exponent
	Factor = 23, Divisor = 1000, Base = 10, Exponent = 0; isValueNegative = false;

	// benchmark encode/decodeVAL
	try {
		while (mBenchmark.run()) {
			encodeUNIT(Format, Unit, Factor, Divisor, Base, Exponent, isValueNegative);
		}
		printf("\n encode no exponent/ Run: %s", mBenchmark.info(7020, 1).getText());
	}
	catch (...) {
		return false;
	}
	try {
		while (mBenchmark.run()) {
			decodeUNIT(Format, Unit1, Value);
		}
		printf("\n decode no exponent/ Run: %s", mBenchmark.info(200, 1).getText());
	}
	catch (...) {
		return false;
	}

	// benchmark encode/decodeTIME
	try {
		while (mBenchmark.run()) {
			encodeTIME(Format, Factor, Divisor, Base, Exponent, isValueNegative);
		}
		printf("\n encodeTIME        / Run: %s", mBenchmark.info(5560, 1).getText());
	}
	catch (...) {
		return false;
	}
	try {
		while (mBenchmark.run()) {
			decodeTIME(Format, Value);
		}
		printf("\n decodeTIME        / Run: %s", mBenchmark.info(168, 1).getText());
	}
	catch (...) {
		return false;
	}

	// cleanup resources
	Memory.releaseMemory<uint8>(pField, BIN_LEN + 1, isUint8);
	Memory.releaseMemory<uint8>(pField1, BIN_LEN + 1, isUint8);
	Memory.releaseMemory<uint16>(pMCL, BIN_LEN_MCL / 2, isUint16);

	//-----------------------test-FTLight-structure-functionality-------------
	
	setText("EKD@JN58nc_Türkenfeld.CmStringFTL");		// identification
	// add some childs
	CmStringFTL* Child = NULL;
	CmStringFTL* Child1 = NULL;
	const int NumChilds = 8;
	const int NumChilds3 = 3;
	const int NumChilds32 = 1;
	const int NumChilds320 = 5;
	const int NumChilds6 = 2;
	for (int i = 0; i < NumChilds; i++) {
		if (true == addChild(&Child)) {
			CmString Content = "Child-";
			Content += i;
			Child->setText(Content);
		}
	}
	// find a  child
	CmString Content3("Child-3");
	CmString Content32("Child-3-2");
	if (true == findChild(&Child1, Content3)) {
		// add some childs to selected child
		for (int i = 0; i < NumChilds3; i++) {
			if (true == Child1->addChild(&Child)) {
				CmString Content = "Child-3-";
				Content += i;
				Child->setText(Content);
			}
		}
		// find a child
		if (true == Child1->findChild(&Child1, Content32)) {
			// add some childs to selected child
			for (int i = 0; i < NumChilds32; i++) {
				if (true == Child1->addChild(&Child)) {
					CmString Content = "Child-3-2-";
					Content += i;
					Child->setText(Content);
				}
			}
			// add some childs to last child
			Child1 = Child;
			if (NULL != Child1) {
				for (int i = 0; i < NumChilds320; i++) {
					if (true == Child1->addChild(&Child)) {
						CmString Content = "Child-3-2-0-";
						Content += i;
						Child->setText(Content);
					}
				}
			}
		}
	}
	// find a  child
	CmString Content6("Child-6");
	if (true == findChild(&Child1, Content6)) {
		// add some childs to selected child
		for (int i = 0; i < NumChilds6; i++) {
			if (true == Child1->addChild(&Child)) {
				CmString Content = "Child-6-";
				Content += i;
				Child->setText(Content);
			}
		}
	}

	// test serializing a FTL hierarchy
#define TEST_SERIALIZE_STRINGFTL_HIERARCHY "e.kantz@JN58nc_Türkenfeld.Test_serialize_CmStringFTL:1522000000:v1.0\n\
,Elements:Ca\n\
,Group1:::comment1\n\
,,Item1:Value1\n\
,,Item2:Value2\n\
,Group2:::comment2\n\
,,Item3:Value3\n\
,,Item4:Value4\n\
"

	CmString SourceFTL(TEST_SERIALIZE_STRINGFTL_HIERARCHY);
	CmString StringFTL;
	CmStringFTL ProFTL;
	// NOTE: process/serializeStringFTL has to run from own CmStringFTL (not from *this)
	ProFTL.processStringFTL(SourceFTL);
	ProFTL.serializeStringFTL(StringFTL);
	// update timestamp
	int32 Start = SourceFTL.findPosition(':');
	int32 Length = SourceFTL.findPosition(':', Start + 1) - Start + 1;
	memcpy_s(SourceFTL.getBuffer() + Start, Length, StringFTL.getBuffer() + Start, Length);
	// compare
	if (StringFTL != SourceFTL) return false;

	// test hex number recognition
	CmString ItemFTL;
	CmString Hex("0xCa"); Hex.setActive(0, Hex.getLength(), 0);
	CmString NotHex("Ca"); NotHex.setActive(0, NotHex.getLength(), 0);
	CmString NotHex1("10xCa"); NotHex1.setActive(0, NotHex1.getLength(), 0);
	CmString NotHex2("0xC+a"); NotHex2.setActive(0, NotHex2.getLength(), 0);
	bool isStartOfCollection = true;
	bool isParentBinary = false;
	getNextItemFTL(Hex, ItemFTL, TypeFTL, isStartOfCollection, isParentBinary); if (TypeFTL != TYPEFTL_NUMBER) return false;
	getNextItemFTL(NotHex, ItemFTL, TypeFTL, isStartOfCollection, isParentBinary); if (TypeFTL != TYPEFTL_TXL) return false;
	getNextItemFTL(NotHex1, ItemFTL, TypeFTL, isStartOfCollection, isParentBinary); if (TypeFTL != TYPEFTL_TXL) return false;
	getNextItemFTL(NotHex2, ItemFTL, TypeFTL, isStartOfCollection, isParentBinary); if (TypeFTL != TYPEFTL_TXL) return false;


	// test synchronous reading/writing
#define TEST_SYNCHRONOUS_WRITING "e.kantz@JN58nc_Türkenfeld.Test_synchronous_reading:1524000000:v1.0\n\
,Data:Timestamp,Item1,Item2\n\
ñ÷÷÷`!`¨$;ò÷÷÷`*;ò÷÷÷`$;@\n\
8·b¡.Hn\\; ý¼©\\; Ð„zŒ\n\
heÊ°.Hnf; ý¼©–; Ù&YŒ\n\
àpoÀ.Hnf; ý¼©–; m{ #Œ\n\
"
	CmString DataIn = TEST_SYNCHRONOUS_WRITING;
	CmString DataOut;
	CmString DataOut2;
	CmStringFTL DataFTL;
	// convert data forth and back
	DataFTL.processStringFTL(DataIn);
	DataFTL.serializeStringFTL(DataOut);
	// compare
	//int8* Input = DataIn.getBuffer();
	//int8* Output = DataOut.getBuffer();
	// convert data forth and back
	DataFTL.clearFTLightHierarchy();
	DataFTL.processStringFTL(DataOut);
	DataFTL.serializeStringFTL(DataOut2);
	// TEST
	//DataOut.writeBinary("DataOut.txt");
	//DataOut2.writeBinary("DataOut2.txt");
	if (DataOut != DataOut2) return false;


	// test writing table data as formatted arrays
#define TEST_TABLE_ARRAYS "\
Tabellendaten mit formatierten Arrays\n\
f[Hz], DC[mV], Z, ICTC[ms], Uload[mV], Utrig[mV], Ucyl[mV], P[0.1uW], Q[0.1uW], RngP[0.1mV], RngQ[0.1mV], MinP[0.1mV], MinQ[0.1mV], MaxP[0.1mV], MaxQ[0.1mV], T_box[0.1°C], T_load[0.1°C], T_cyl[0.1°C], T_ambient[0.1°C],\n\
38008, 130, 0, 13, -377, 38516, -45299, -18542, 0, 100, 0, -4496, 0, 3184, 0, 29305, 29880, 0, 0\n\
38008, 131, 0, 27, -487, 45057, -37139, -23904, 0, 101, 0, -4451, 0, 3185, 0, 29304, 29935, 0, 0\n\
38008, 122, 0, 41, -484, 45057, -37136, -23743, 0, 102, 0, -4448, 0, 3188, 0, 29324, 29903, 0, 0\n\
38008, 126, 0, 56, -486, 45057, -37143, -23830, 0, 99, 0, -4406, 0, 3097, 0, 29304, 29893, 0, 0\n\
38008, 127, 0, 70, -480, 44951, -37140, -23542, 0, 99, 0, -4354, 0, 3193, 0, 29312, 29897, 0, 0\n\
"









	// remove conversion tables
	clearConversionTable();

	return true;
}

//===========================Public-functions=================================

//---------------------------TXL-functions-----------------------------------

//
// TXL access functions
//
bool CmStringFTL::setTXL(const int8* _Text)
{
	DataFormat = DATAFORMAT_NONE;

	// delete previous TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}

	// check for new content
	if (NULL == _Text) return true;

	// allocate TXL memory
	LengthTXL = strlen(_Text);
	TXL = allocateMemory<uint8>((int32)LengthTXL + 1, isUint8);
	if (NULL == TXL) return true;
	// save text in TXL
	strncpy_s((int8*)TXL, LengthTXL + 1, _Text, strlen(_Text));

	return true;
}

bool CmStringFTL::setTXL(CmString& _Text)
{
	return setTXL(_Text.getBuffer());
}

const uint8* CmStringFTL::getTXL()
{
	return TXL;
}

int64 CmStringFTL::getLengthTXL()
{
	return LengthTXL;
}

//
// convert to and from TXL format
//
CmStringFTL& CmStringFTL::encodeTXL(const int8* _Text)
{
	// store new text or take existing text
	_Text != NULL ? CmString::setText(_Text) : 0;
	DataFormat = DATAFORMAT_NONE;

	// delete previous TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}

	// check for valid content
	if (NULL == pText) return *this;

	// look for special characters and determine target string length
	const int8* pSrc = pText;
	while (0 != *pSrc) {
		// special FTLight characters
		switch (*pSrc) {
		case 10: LengthTXL++; break;	// line feed
		case 13: LengthTXL++; break;	// carriage return
		case 44: LengthTXL++; break;	// comma
			//case 45: LengthTXL++; break;	// minus: special in addresses only
		case 58: LengthTXL++; break;	// colon
		case 59: LengthTXL++; break;	// semicolon
		case 61: LengthTXL++; break;	// equal sign
		case 64: LengthTXL++; break;	// "at" sign
		case 96: LengthTXL++; break;	// backtick
		case 127: LengthTXL++; break;	// delete
		default: break;
		}
		LengthTXL++;
		pSrc++;
	}

	// allocate TXL memory
	TXL = allocateMemory<uint8>((int32)LengthTXL + 1, isUint8);
	if (NULL == TXL) return *this;

	// generate TXL compliant to FTLight specification
	pSrc = pText;
	uint8* pDst = TXL;
	while (0 != *pSrc) {
		// escape special FTLight characters by a preceding backslash
		switch (*pSrc) {
		case 10: *pDst++ = '\\'; break;	// line feed
		case 13: *pDst++ = '\\'; break;	// carriage return
		case 44: *pDst++ = '\\'; break;	// comma
			//case 45: *pDst++ = '\\'; break;	// minus: special in addresses only
		case 58: *pDst++ = '\\'; break;	// colon
		case 59: *pDst++ = '\\'; break;	// semicolon
		case 61: *pDst++ = '\\'; break;	// equal sign
		case 64: *pDst++ = '\\'; break;	// "at" sign
		case 96: *pDst++ = '\\'; break;	// backtick
		case 127: *pDst++ = '\\'; break;	// delete
		default: break;
		}
		*pDst++ = *pSrc++;
	}
	// add a string terminating zero
	*pDst = 0;

	return *this;
}

CmStringFTL& CmStringFTL::encodeTXL(CmString& _Text)
{
	encodeTXL(_Text.getText());
	return *this;
}

void CmStringFTL::decodeTXL(const int8* _TXL, size_t _LengthTXL)
{
	// delete previous TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}

	// transfer intern Data to _TXL if no _TXL was provided
	if (NULL == _TXL || _LengthTXL == 0) {
		_TXL = pText;
		_LengthTXL = Length;
	}

	// store new FTLight coded string
	TXL = allocateMemory<uint8>((int32)_LengthTXL + 1, isUint8);
	if (NULL == TXL) return;
	// string terminating zero
	*(TXL + _LengthTXL) = 0;
	LengthTXL = _LengthTXL;
	memcpy(TXL, _TXL, LengthTXL);

	// count escaped characters and determine resulting decoded length
	size_t Length_l;
	Length_l = LengthTXL;
	const uint8* pSrc = TXL + 1;  // first character can not be escaped
	bool isSpecial;
	while (1 < _LengthTXL--) {
		// special FTLight characters
		isSpecial = false;
		switch (*pSrc) {
		case 10: isSpecial = true; break;	// line feed
		case 13: isSpecial = true; break;	// carriage return
		case 44: isSpecial = true; break;	// comma
			//case 45: isSpecial = true; break;	// minus
		case 58: isSpecial = true; break;	// colon
		case 59: isSpecial = true; break;	// semicolon
		case 61: isSpecial = true; break;	// equal sign
		case 64: isSpecial = true; break;	// "at" sign
		case 96: isSpecial = true; break;	// backtick
		case 127: isSpecial = true; break;	// delete
		default: break;
		}
		// check if a special character was escaped 
		if (isSpecial) {
			if (*(pSrc - 1) == '\\')
				Length_l--;
		}
		pSrc++;
	}

	// set resultig text length
	setLength(Length_l);

	// decode FTLight coded string by removing backslash from 
	// all escaped special characters
	int8 TestCh;
	pSrc = TXL;
	int8* pDst = pText;
	*pDst++ = *pSrc++;		// first character can not be escaped
	_LengthTXL = LengthTXL;
	while (1 < _LengthTXL--) {
		// special FTLight characters
		isSpecial = false;
		TestCh = *pSrc;
		switch (*pSrc) {
		case 10: isSpecial = true; break;	// line feed
		case 13: isSpecial = true; break;	// carriage return
		case 44: isSpecial = true; break;	// comma
			//case 45: isSpecial = true; break;	// minus
		case 58: isSpecial = true; break;	// colon
		case 59: isSpecial = true; break;	// semicolon
		case 61: isSpecial = true; break;	// equal sign
		case 64: isSpecial = true; break;	// "at" sign
		case 96: isSpecial = true; break;	// backtick
		case 127: isSpecial = true; break;	// delete
		default: break;
		}
		// check if a special character was escaped 
		if (isSpecial) {
			if (*(pSrc - 1) == '\\') {
				*--pDst = *pSrc++;
				pDst++;
			}
			else {
				*pDst++ = *pSrc++;
			}
		}
		else {
			*pDst++ = *pSrc++;
		}
	}
	DataFormat = DATAFORMAT_STRING;
}

void CmStringFTL::decodeTXL(CmString& _Text)
{
	decodeTXL(_Text.getText(), _Text.getLength());
}

//---------------------------FTL-functions------------------------------------

//
// convert to and from FTL format
//
void CmStringFTL::encodeFTL(const uint8* _Data, size_t _Length)
{
	// delete previous TXL
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}

	// use TXL as intermediate memory for input data
	if (NULL != _Data && 0 != _Length) {
		TXL = (uint8*)_Data;
		LengthTXL = _Length;
	}else {
		TXL = (uint8*)pText;
		LengthTXL = Length;
		pText = NULL;
		Length = 0;
	}

	// convert to FTL
	bin2FTL(this, TXL, (int32)LengthTXL);

	// put FTL to TXL 
	uint8* TXL1 = TXL;
	TXL = (uint8*)pText;
	pText = NULL;
	size_t LengthTXL1 = LengthTXL;
	LengthTXL = Length;
	Length = 0;

	// save the binary field to pText (CmString base class)
	if (NULL != _Data && 0 != _Length) {
		setData(_Data, (int32)_Length);
	}
	else {
		// return binary field from intermediate place to pText (CmString base class)
		pText = (int8*)TXL1;
		Length = LengthTXL1;
	}

	DataFormat = DATAFORMAT_BINARY;
}

void CmStringFTL::encodeFTL(CmString& _Data)
{
	encodeFTL(_Data.getBinary(), _Data.getLength());
}

void CmStringFTL::decodeFTL(const uint8* _Data, size_t _Length)
{
	// free TXL
	if (NULL != TXL && _Data != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}

	// Store FTL data in CmString
	if (NULL != _Data && 0 != _Length) {
		setData(_Data, (int32)_Length);
	}

	// release TXL
	releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
	// Determine resulting binary length, assume fTail = true if _Length == 8 
	// in order to properly restore an uint64 bit field
	LengthTXL = getLengthTXL2bin(int32(Length), Length == 8 ? true : false);
	TXL = allocateMemory<uint8>(int32(LengthTXL + 1), isUint8);
	memset(TXL, 0, LengthTXL + 1);

	// Run conversion from FTL to a binary field
	FTL2bin(TXL, (int32)LengthTXL, *this);

	// put FTL to TXL 
	uint8* TXL1 = TXL;
	TXL = (uint8*)pText;
	pText = NULL;
	size_t LengthTXL1 = LengthTXL;
	LengthTXL = Length;
	Length = 0;

	// return binary field from intermediate place to pText (CmString base class)
	pText = (int8*)TXL1;
	Length = LengthTXL1;
	DataFormat = DATAFORMAT_BINARY;
}

void CmStringFTL::decodeFTL(CmString& _Data)
{
	decodeFTL(_Data.getBinary(), _Data.getLength());
}

bool CmStringFTL::isConversionTable()
{
	// check if conversion tables have been created before
	if (NULL == MCLconvert) {

		CmString Memory;
		// get memory for BINFTL/SYMBOL
		if (NULL == (BINFTL = Memory.allocateMemory<uint8>(FTLconvertSize, isUint8))) return false;
		if (NULL == (SYMBOL = Memory.allocateMemory<uint8>(FTLconvertSize, isUint8))) return false;
		// fill conversion tables
		for (int32 i = 0; i < FTLconvertSize; i++) {
			*(BINFTL + i) = Symbol2FTL(uint8(i));
			*(SYMBOL + i) = FTL2Symbol(uint8(i));
		}
		// get memory for MCL
		if (NULL == (MCLconvert = Memory.allocateMemory<uint16>(MCLconvertSize, isUint16))) return false;
		// fill conversion table
		for (int32 i = 0; i < MCLconvertSize; i++) {
			*(MCLconvert + i) = 256 * Symbol2FTL((uint8)(i / 216)) + Symbol2FTL((uint8)(i % 216));
		}
	}
	return true;
}
bool CmStringFTL::clearConversionTable()
{
	// remove FTL conversion tables
	releaseMemory<uint8>(BINFTL, FTLconvertSize, isUint8);
	releaseMemory<uint8>(SYMBOL, FTLconvertSize, isUint8);
	// remove MCL conversion table
	releaseMemory<uint16>(MCLconvert, MCLconvertSize, isUint16);

	return true;
}

//---------------------------NUM-functions------------------------------------

//
// encode
//
void CmStringFTL::encodeNUM(uint64 _Value)
{
	// convert 64-bit field to a FTL string
	num2FTL(this, _Value);

	// put FTL string to TXL
	if (NULL != TXL)
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
	LengthTXL = Length;
	TXL = allocateMemory<uint8>((int32)LengthTXL + 1, isUint8);
	*(TXL + LengthTXL) = 0;
	memcpy(TXL, pText, Length);

	// put 64-bit value to CmString
	setData((uint8*)&_Value, 8);
	DataFormat = DATAFORMAT_UINT64;
}
void CmStringFTL::encodeNUM(int64 _Value)
{
	uint64 Value = 0;
	Value = (uint64)_Value;
	encodeNUM(Value);
	DataFormat = DATAFORMAT_INT64;
}
void CmStringFTL::encodeNUM(double _Value)
{
	uint64 Value = 0;
	Value = *((uint64*)(&_Value));
	encodeNUM(Value);
	DataFormat = DATAFORMAT_DOUBLE;
}
void CmStringFTL::encodeNUM(uint32 _Value)
{
	uint64 Value = 0;
	Value = (uint64)_Value;
	encodeNUM(Value);
	DataFormat = DATAFORMAT_UINT32;
}
void CmStringFTL::encodeNUM(int32 _Value)
{
	uint64 Value = 0;
	Value = (uint64)_Value;
	encodeNUM(Value);
	DataFormat = DATAFORMAT_INT32;
}
void CmStringFTL::encodeNUM(float _Value)
{
	uint64 Value = 0;
	Value = *((uint32*)(&_Value));
	encodeNUM(Value);
	DataFormat = DATAFORMAT_FLOAT;
}
void CmStringFTL::encodeNUM(uint16 _Value)
{
	uint64 Value = 0;
	Value = (uint64)_Value;
	encodeNUM(Value);
	DataFormat = DATAFORMAT_UINT16;
}
void CmStringFTL::encodeNUM(int16 _Value)
{
	uint64 Value = 0;
	Value = (uint64)_Value;
	encodeNUM(Value);
	DataFormat = DATAFORMAT_INT16;
}
void CmStringFTL::encodeNUM(uint8 _Value)
{
	uint64 Value = 0;
	Value = (uint64)_Value;
	encodeNUM(Value);
	DataFormat = DATAFORMAT_UINT8;
}
void CmStringFTL::encodeNUM(int8 _Value)
{
	uint64 Value = 0;
	Value = (uint64)_Value;
	encodeNUM(Value);
	DataFormat = DATAFORMAT_INT8;
}
void CmStringFTL::encodeNUM(bool _Value)
{
	uint64 Value = 0;
	Value = (uint64)(_Value ? 1 : 0);
	encodeNUM(Value);
	DataFormat = DATAFORMAT_BOOL;
}
//
// decode
//
uint64 CmStringFTL::decodeNUM2uint64(uint64* _Value)
{
	// convert FTL to uint64
	uint64 Value = FTL2num(*this);

	// put FTL to TXL
	if (NULL != TXL)
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
	LengthTXL = Length;
	TXL = allocateMemory<uint8>((int32)LengthTXL + 1, isUint8);
	*(TXL + LengthTXL) = 0;
	memcpy(TXL, pText, Length);

	// put 64-bit value to CmString
	setData((uint8*)&Value, 8);

	// return result
	if (NULL != _Value)
		*_Value = Value;
	DataFormat = DATAFORMAT_UINT64;

	return Value;
}
int64 CmStringFTL::decodeNUM2int64(int64* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = (int64) * ((uint64*)pText);
	DataFormat = DATAFORMAT_INT64;

	return (int64) * ((uint64*)pText);
}
double CmStringFTL::decodeNUM2double(double* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = *((double*)pText);
	DataFormat = DATAFORMAT_DOUBLE;

	return *((double*)pText);
}
uint32 CmStringFTL::decodeNUM2uint32(uint32* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = (uint32) * ((uint64*)pText);
	DataFormat = DATAFORMAT_UINT32;

	return (uint32) * ((uint64*)pText);
}
int32 CmStringFTL::decodeNUM2int32(int32* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = (int32) * ((uint64*)pText);
	DataFormat = DATAFORMAT_INT32;

	return (int32) * ((uint64*)pText);
}
float CmStringFTL::decodeNUM2float(float* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = *((float*)pText);
	DataFormat = DATAFORMAT_FLOAT;

	return *((float*)pText);
}
uint16 CmStringFTL::decodeNUM2uint16(uint16* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = (uint16) * ((uint64*)pText);
	DataFormat = DATAFORMAT_UINT16;

	return (uint16) * ((uint64*)pText);
}
int16 CmStringFTL::decodeNUM2int16(int16* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = (int16) * ((uint64*)pText);
	DataFormat = DATAFORMAT_INT16;

	return (int16) * ((uint64*)pText);
}
uint8 CmStringFTL::decodeNUM2uint8(uint8* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = (uint8) * ((uint64*)pText);
	DataFormat = DATAFORMAT_UINT8;

	return (uint8) * ((uint64*)pText);
}
int8 CmStringFTL::decodeNUM2int8(int8* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = (int8) * ((uint64*)pText);
	DataFormat = DATAFORMAT_INT8;

	return (int8) * ((uint64*)pText);
}
bool CmStringFTL::decodeNUM2bool(bool* _Value)
{
	// convert FTL to uint64
	decodeNUM2uint64();

	// return result
	if (NULL != _Value)
		*_Value = *((uint64*)pText) > 0 ? true : false;
	DataFormat = DATAFORMAT_BOOL;

	return *((uint64*)pText) > 0 ? true : false;
}

bool CmStringFTL::decodeNum(CmDataFormat _DataFormat)
{
	// check for FTLight Num format
	if (TypeFTL != TYPEFTL_NUMBER && TypeFTL != TYPEFTL_NUMBER_BINFTL) return false;

	// try appropriate conversion
	uint64 uVal = 0;
	double fVal = 0;
	if (DATAFORMAT_UINT64 == _DataFormat || DATAFORMAT_BOOL == _DataFormat) {
		// for uint64 format, a separate conversion is used in order to get full length
		uVal = getNumAsUint64();
		if (isConversionError) {
			// if UINT64 does not work then try double
			fVal = (double)getNumAsDouble();
			if (isConversionError) {
				// failed, no chance
				return false;
			}
			uVal = (uint64)fVal;
		}
	}
	else {
		// all other formats will first be converted into double
		fVal = getNumAsDouble();
		// if double conversion does not work then try UINT64
		if (isConversionError) {
			fVal = (double)getNumAsUint64();
			if (isConversionError) {
				// failed, no chance
				return false;
			}
		}
	}
	// transfer Num string to TXL
	if (getLength() == 0) return false;
	if (NULL != TXL) {
		releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);
		TXL = NULL;
		LengthTXL = 0;
	}
	// ToDo: make memory deallocation thread-safe
	//LengthTXL = getLength();
	//TXL = allocateMemory<uint8>((int32)LengthTXL+1, isUint8);
	//*(TXL + LengthTXL) = 0;
	//memcpy(TXL, getBuffer(), LengthTXL);

	// write appropriate data format into CmString base class, 
	CmFlex Val;
	setLength(sizeof(Val));
	DataFormat = _DataFormat;
	switch (DataFormat)
	{
	case DATAFORMAT_UINT64: Val.uint64Value = uVal; break;
	case DATAFORMAT_DOUBLE: Val.doubleValue = fVal; break;
	case DATAFORMAT_INT64: Val.int64Value = (int64)fVal; break;
	case DATAFORMAT_UINT32: Val.uint32Value = (uint32)fVal; break;
	case DATAFORMAT_INT32: Val.int32Value = (int32)fVal; break;
	case DATAFORMAT_FLOAT: Val.floatValue = (float)fVal; break;
	case DATAFORMAT_UINT16: Val.uint16Value = (uint16)fVal; break;
	case DATAFORMAT_INT16: Val.int16Value = (int16)fVal; break;
	case DATAFORMAT_UINT8: Val.uint8Value = (uint8)fVal; break;
	case DATAFORMAT_INT8: Val.int8Value = (int8)fVal; break;
	case DATAFORMAT_BOOL: Val.boolValue = uVal > 0 ? true : false; break;
	default: return false;
	}
	memcpy(getBuffer(), &Val.uint8Value, sizeof(Val));

	return true;
}

CmString* CmStringFTL::num2FTL(CmString* _pFTL, uint64 _u64Num)
{
	return num2FTL(_pFTL, &_u64Num);
}
CmString* CmStringFTL::num2FTL(CmString* _pFTL, uint64* _p64Num)
{
	uint64 u64Num = *_p64Num;
	int32  nDigits;
	// determine number of FTL digits
//#ifdef gcc
//	if (0 == (u64Num & 0xFFFFFFFF80000000ull)) {
//		for (nDigits = 1; u64Num >= RADIX_216; nDigits++) {
//			u64Num /= RADIX_216;
//		}
//	}
//	else if (0 == (u64Num & 0xC000000000000000ull)) {
//#else
	if (0 == (u64Num & 0xFFFFFFFF80000000)) {
		for (nDigits = 1; u64Num >= RADIX_216; nDigits++) {
			u64Num /= RADIX_216;
		}
	}
	else if (0 == (u64Num & 0xC000000000000000)) {
//#endif
		for (nDigits = 1; u64Num >= RADIX_216; nDigits++) {
			u64Num /= RADIX_216;
		}
	}
	else {
		nDigits = 9;
	}
	// convert uint64 to FTL
	return bin2FTL(_pFTL, (uint8*)_p64Num, 8, nDigits);
}
CmString* CmStringFTL::sigNum2FTL(CmString * _pFTL, int64 _64Num)
{
	CmString FTL;
	// negative numbers will become positive numbers
	uint64 u64Num = _64Num < 0 ? -_64Num : _64Num;
	// convert uint64 to FTL
	num2FTL(&FTL, &u64Num);
	// insert sign if num was negative
	if (_64Num < 0) {
		*_pFTL = "`";
		*_pFTL += FTL;
	}
	else {
		*_pFTL = FTL;
	}
	return _pFTL;
}
uint64 CmStringFTL::FTL2num(CmString & _mFTL)
{
	uint64 u64Num;
	FTL2bin((uint8*)&u64Num, 8, _mFTL, true);
	return u64Num;
}
int64 CmStringFTL::FTL2sigNum(CmString & _mFTL)
{
	// determine sign
	int64 Sign = _mFTL[0] == '`' ? -1 : 1;
	// convert FTL to uint64
	uint64 u64Int;
	CmString FTL;
	if (Sign < 0) {
		FTL.assignSubString(_mFTL.getLength(), &_mFTL, 1);
	}
	else {
		FTL = _mFTL;
	}
	u64Int = FTL2num(FTL);
	// take sign into account
	return Sign * int64(u64Int);
}

//---------------------------VAL-functions------------------------------------

bool CmStringFTL::encodeUNIT(CmString& _UNIT, CmPhysicalUnit _Unit, int64 _Factor, int64 _Divisor, int64 _Base, int64 _Exponent, bool _isValueNegative)
{
	CmString Num;
	CmString Format;
	int32 Components = 0;
	// physical unit
	if ((_Unit != 0) || (_Factor != 1) || (_Divisor != 1) || (_Base != 10) || (_Exponent != 0)) {
		Format += "`";
		num2FTL(&Num, _Unit);
		Format += Num;
		Components++;
	}
	// factor
	if ((_Factor != 1) || (_Divisor != 1) || (_Base != 10) || (_Exponent != 0)) {
		Format += "`";
		num2FTL(&Num, _Factor);
		Format += Num;
		Components++;
	}
	// devisor
	if ((_Divisor != 1) || (_Base != 10) || (_Exponent != 0)) {
		Format += "`";
		num2FTL(&Num, _Divisor);
		Format += Num;
		Components++;
	}
	// base
	if (_Base != 10) {
		Format += "`";
		num2FTL(&Num, _Base);
		Format += Num;
		Components++;
	}
	// exponent
	if (_Exponent != 0) {
		Format += "`";
		_Exponent < 0 ? Components++ : 0;
		if (_Exponent < 0) { Format += "`"; }
		num2FTL(&Num, abs(_Exponent));
		Format += Num;
		Components++;
	}
	// negative value
	if (_isValueNegative) {
		Format += "`";
		Components++;
	}
	// format identifier
	setFTLightDataType(_UNIT, DTI_UNIT);
	// number of components
	if (Components > 0) {
		_UNIT += "`";
		num2FTL(&Num, Components + 1);
		_UNIT += Num;
	}
	// encoded format components
	_UNIT += Format;

	return true;
}
bool CmStringFTL::decodeUNIT(const CmString& _UNIT, CmPhysicalUnit& _Unit, double& _Value)
{
	// initialize return values
	_Unit = UNIT_NONE;
	_Value = 0;

	// test for CmValueFTL (F2 F7 F7 F7)
	const uint32 TypeVAL = 0xF7F7F7F2;
	uint32* Type = (uint32*)_UNIT.getText();
	if (Type[0] != TypeVAL) return false;

	// evaluate fields
	uint8* Format = (uint8*)_UNIT.getText() + 4;
	int32 Length = _UNIT.getLength() - 4;
	int32 Index = 0;
	int32 Count = 0;
	int64 Components = 0;
	int64 Factor = 1;
	int64 Divisor = 1;
	int64 Base = 10;
	int64 Exponent = 0;
	int64 Exponent1 = 0;
	int64 Value = 0;
	int64 Weight = 1;
	bool isValueNegative[9] = { false };
	bool isDelimiter = false;
	for (int32 i = 0; i < Length; i++) {
		if (Format[i] == '`') {
			Index == 1 ? Components = Value : Index == 2 ? _Unit = CmPhysicalUnit(Value) : Index == 3 ? Factor = Value : Index == 4 ? Divisor = Value : Index == 5 ? Exponent = Value : Index == 6 ? Exponent1 = Value : 0;
			// negative 
			isDelimiter ? Index < 9 ? isValueNegative[Index] = true : 0 : 0;
			false == isDelimiter ? Index++ : 0;
			Count++;
			isDelimiter = true;
			Value = 0;
			Weight = 1;
			continue;
		}
		isDelimiter = false;
		Value += FTL2Symbol(Format[i]) * Weight;
		Weight *= 216;
	}
	// assign last component
	false == isDelimiter ? Index == 1 ? Components = Value : Index == 2 ? _Unit = CmPhysicalUnit(Value) : Index == 3 ? Factor = Value : Index == 4 ? Divisor = Value : Index == 5 ? Exponent = Value : 0 : 0;
	// base
	isValueNegative[5] || isValueNegative[6] ? Count-- : 0;
	isDelimiter ? Count-- : 0;
	if (Count == 6) {
		Base = Exponent;
		Exponent = isDelimiter ? Exponent1 : Value;
	}
	// negative exponent
	isValueNegative[5] || isValueNegative[6] ? Exponent = -Exponent : 0;
	// value
	_Value = Factor / double(Divisor != 0 ? Divisor : 1) * pow(double(Base), double(Exponent));
	// negative value
	isDelimiter ? _Value = -_Value : 0;

	return true;
}
bool CmStringFTL::isInteger()
{
	// evaluated string content for being an integer (+-0..9)
	bool isInteger = true;
	for (int32 i = 0; i < int32(Length); i++) {
		uint8 Char = *(pText + i);
		if ((Char < '0' || Char > '9') && (Char != '+') && (Char != '-')) {
			isInteger = false;
			break;
		}
	}

	return isInteger;
}

//---------------------------TIME-functions-----------------------------------

bool CmStringFTL::encodeTIME(CmString& _TIME, int64 _Factor, int64 _Divisor, int64 _Base, int64 _Exponent, bool _isTimeNegative)
{
	CmString Num;
	CmString Format;
	int32 Components = 0;
	// validate conversion table
	if (false == isConversionTable()) return false;
	// factor
	if ((_Factor != 1) || (_Divisor != 1) || (_Base != 10) || (_Exponent != 0)) {
		Format += "`";
		num2FTL(&Num, _Factor);
		Format += Num;
		Components++;
	}
	// devisor
	if ((_Divisor != 1) || (_Base != 10) || (_Exponent != 0)) {
		Format += "`";
		num2FTL(&Num, _Divisor);
		Format += Num;
		Components++;
	}
	// base
	if (_Base != 10) {
		Format += "`";
		num2FTL(&Num, _Base);
		Format += Num;
		Components++;
	}
	// exponent
	if (_Exponent != 0) {
		Format += "`";
		_Exponent < 0 ? Components++ : 0;
		if (_Exponent < 0) { Format += "`"; }
		num2FTL(&Num, abs(_Exponent));
		Format += Num;
		Components++;
	}
	// negative time (before 1970)
	if (_isTimeNegative) {
		Format += "`";
		Components++;
	}
	// format identifier
	setFTLightDataType(_TIME, DTI_TIME);
	// number of components
	if (Components > 0) {
		_TIME += "`";
		num2FTL(&Num, Components + 1);
		_TIME += Num;
	}
	// encoded format components
	_TIME += Format;

	return true;
}
bool CmStringFTL::decodeTIME(const CmString& _TIME, double& _Time)
{
	// initialize return value
	_Time = 0;

	// test for CmValueFTL (F1 F7 F7 F7)
	const uint32 TypeTIME = 0xF7F7F7F1;
	uint32* Type = (uint32*)_TIME.getText();
	if (Type[0] != TypeTIME) return false;

	// evaluate fields
	uint8* Format = (uint8*)_TIME.getText() + 4;
	int32 Length = _TIME.getLength() - 4;
	int32 Index = 0;
	int32 Count = 0;
	int64 Components = 0;
	int64 Factor = 1;
	int64 Divisor = 1;
	int64 Base = 10;
	int64 Exponent = 0;
	int64 Exponent1 = 0;
	int64 Value = 0;
	int64 Weight = 1;
	bool isValueNegative[9] = { false };
	bool isDelimiter = false;
	for (int32 i = 0; i < Length; i++) {
		if (Format[i] == '`') {
			Index == 1 ? Components = Value : Index == 2 ? Factor = Value : Index == 3 ? Divisor = Value : Index == 4 ? Exponent = Value : Index == 5 ? Exponent1 = Value : 0;
			// negative 
			isDelimiter ? Index < 9 ? isValueNegative[Index] = true : 0 : 0;
			false == isDelimiter ? Index++ : 0;
			Count++;
			isDelimiter = true;
			Value = 0;
			Weight = 1;
			continue;
		}
		isDelimiter = false;
		Value += FTL2Symbol(Format[i]) * Weight;
		Weight *= 216;
	}
	// assign last component
	false == isDelimiter ? Index == 1 ? Components = Value : Index == 2 ? Factor = Value : Index == 3 ? Divisor = Value : Index == 4 ? Exponent = Value : 0 : 0;
	// base
	isValueNegative[4] || isValueNegative[5] ? Count-- : 0;
	isDelimiter ? Count-- : 0;
	if (Count == 5) {
		Base = Exponent;
		Exponent = isDelimiter ? Exponent1 : Value;
	}
	// negative exponent
	isValueNegative[4] || isValueNegative[5] ? Exponent = -Exponent : 0;
	// value
	_Time = Factor / double(Divisor != 0 ? Divisor : 1) * pow(double(Base), double(Exponent));
	// negative value
	isDelimiter ? _Time = -_Time : 0;

	return true;
}

//---------------------------Flex-functions-----------------------------------

double CmStringFTL::flex2Double(CmFlex& _Flex, CmDataFormat _DataFormat)
{
	double ValueDouble = 0;
	if (DATAFORMAT_STRING == _DataFormat) {
		// convert string to double
		CmString* String = (CmString*)_Flex.pointerValue;
		NULL != String ? ValueDouble = String->getNumAsDouble() : 0;
	}
	else {
		// convert any format to double
		switch (_DataFormat) {
		case DATAFORMAT_UINT64: ValueDouble = double(_Flex.uint64Value); break;
		case DATAFORMAT_INT64: ValueDouble = double(_Flex.int64Value); break;
		case DATAFORMAT_DOUBLE: ValueDouble = double(_Flex.doubleValue); break;
		case DATAFORMAT_UINT32: ValueDouble = double(_Flex.uint32Value); break;
		case DATAFORMAT_INT32: ValueDouble = double(_Flex.int32Value); break;
		case DATAFORMAT_FLOAT: ValueDouble = double(_Flex.floatValue); break;
		case DATAFORMAT_UINT16: ValueDouble = double(_Flex.uint16Value); break;
		case DATAFORMAT_INT16: ValueDouble = double(_Flex.int16Value); break;
		case DATAFORMAT_UINT8: ValueDouble = double(_Flex.uint8Value); break;
		case DATAFORMAT_INT8: ValueDouble = double(_Flex.int8Value); break;
		case DATAFORMAT_BOOL: ValueDouble = double(_Flex.boolValue); break;
		default: break;
		}
	}

	return ValueDouble;
}
double CmStringFTL::flex2Double(CmFlex& _Flex, CmDataFormat _DataFormat, CmString& _Item)
{
	// convert to double
	double ValueDouble = flex2Double(_Flex, _DataFormat);
	// convert double to string
	_Item.double2String(ValueDouble);

	return ValueDouble;
}
bool CmStringFTL::double2Flex(CmFlex& _Flex, CmDataFormat _DataFormat)
{
	// convert a double value to requested target CmFlex format
	switch (_DataFormat) {
	case DATAFORMAT_UINT64: _Flex.uint64Value = uint64(_Flex.doubleValue); break;
	case DATAFORMAT_INT64: _Flex.int64Value = int64(_Flex.doubleValue); break;
	case DATAFORMAT_DOUBLE: break;
	case DATAFORMAT_UINT32: _Flex.uint32Value = uint32(_Flex.doubleValue); break;
	case DATAFORMAT_INT32: _Flex.int32Value = int32(_Flex.doubleValue); break;
	case DATAFORMAT_FLOAT: _Flex.floatValue = float(_Flex.doubleValue); break;
	case DATAFORMAT_UINT16: _Flex.uint16Value = uint16(_Flex.doubleValue); break;
	case DATAFORMAT_INT16: _Flex.int16Value = int16(_Flex.doubleValue); break;
	case DATAFORMAT_UINT8: _Flex.uint8Value = uint8(_Flex.doubleValue); break;
	case DATAFORMAT_INT8: _Flex.int8Value = int8(_Flex.doubleValue); break;
	case DATAFORMAT_BOOL: _Flex.boolValue = _Flex.doubleValue == 0 ? false : true; break;
	default: break;
	}

	return true;
}

//---------------------------XML-functions------------------------------------

bool CmStringFTL::convertFTL2XML(const CmString& FileFTL, const CmString& FileXML)
{
	return convertFTL2XML(FileFTL.getText(), FileXML.getText());
}

bool CmStringFTL::convertFTL2XML(const int8* FileFTL, const int8* FileFTLML)
{
	// read and analyze FTLight encoded file
	if (false == readFileFTL(FileFTL)) return false;

	// convert FTLight to XML
	CmString XML;
	CmString Element;
	CmString Value;
	CmStringFTL* NodeChild = this;
	CmStringFTL* NodeSibling = this;
	CmStringFTL* NodeLocal = NULL;
	int LevelChild = 0;
	int OrderSibling = 0;
	int NodeIndex = 0;
	XML = "<FTLight>";
	// travel FTLight hierarchy
	while (NULL != NodeSibling) {
		// first travel all childs 
		if (NULL != NodeSibling->getChild()) {
			LevelChild++;
			NodeChild = NodeSibling->getChild();
			NodeSibling = NodeChild;
			Element = "\n<level";
			Element += LevelChild;
			Element += ">";
			XML += Element;
		}
		else {
			// second travel siblings
			if (NodeSibling->getSibling() != NodeChild) {
				OrderSibling++;
				NodeSibling = NodeSibling->getSibling();
			}
			else {
				// no more siblings, travel levels backwards
				while (NULL != NodeSibling && NodeSibling->getSibling() == NodeChild) {
					if (LevelChild > 0) {
						Element = "\n</level";
						Element += LevelChild;
						Element += ">";
						XML += Element;
					}
					LevelChild--;
					OrderSibling = 0;
					NodeSibling = NodeSibling->getParent();
					NodeChild = NodeSibling;
					if (NULL != NodeChild && NULL != NodeChild->getParent()) {
						NodeChild = NodeChild->getParent()->getChild();
					}
				}
				if (NULL != NodeSibling) {
					NodeSibling = NodeSibling->getSibling();
				}
			}
		}
		// add Node
		if (NULL != NodeSibling) {
			if (NodeSibling->getLength() == 0) {
				continue;
			}
			if (CmString("-") == NodeSibling->getText()) {
				continue;
			}
			Element = "\n<node";
			Element += LevelChild;
			Element += "_";
			Element += ++NodeIndex;
			Element += ">";
			switch (NodeSibling->getTypeFTL()) {
			case	TYPEFTL_OPERATOR:
				Element += "OPERATOR: ";
				Element += NodeSibling->getText();
				break;
			case	TYPEFTL_LOCATION:
				Element += "LOCATION: ";
				Element += NodeSibling->getText();
				break;
			case	TYPEFTL_IDENTIFIER:
				Element += "IDENTIFIER: ";
				Element += NodeSibling->getText();
				break;
			case	TYPEFTL_QUERY:
				Element += "TYPEFTL_QUERY";
				break;
			case	TYPEFTL_TXL:
				Element += NodeSibling->getText();
				break;
			case	TYPEFTL_TXL_BINFTL:
				Element += NodeSibling->decodeNUM2uint64();
				break;
			case	TYPEFTL_NUMBER:
				Element += NodeSibling->getText();
				break;
			case	TYPEFTL_NUMBER_BINFTL:
				try {
					Element += NodeSibling->decodeNUM2uint64();
				}
				catch (...) {
					Element += 0;
				}
				break;
				break;
			case TYPEFTL_Local:
				// recover Locald node's address
				if (sizeof(NodeLocal) != NodeSibling->getLength()) {
					Element += "--address length mismatch";
				}
				else {
					memcpy(&NodeLocal, NodeSibling->getBuffer(), sizeof(NodeLocal));
					Element += "--";
					if (NULL != NodeLocal) {
						Element += NodeLocal->getText();
					}
					else {
						Element += "invalid Local";
					}
				}
				break;
			case TYPEFTL_Local_BINFTL:
				Element += NodeSibling->getText();
				Element += " (invalid Local)";
				break;
			case	TYPEFTL_FIXED_PARENTS:
				Element += "TYPEFTL_FIXED_PARENTS";
				break;
			case	TYPEFTL_CHECKSUM:
				Element += "TYPEFTL_CHECKSUM";
				break;
			case TYPEFTL_OPTION:
				Element += "::";
				Element += NodeSibling->getText();
				break;
			case	TYPEFTL_DEFAULT:
				Element += "TYPEFTL_DEFAULT";
				break;
			case TYPEFTL_COMMENT:
				Element += ":::";
				Element += NodeSibling->getText();
				break;
			case	TYPEFTL_BINFTL:
				try {
					Element += NodeSibling->decodeNUM2uint16();
				}
				catch (...) {
					Element += 0;
				}
				break;
			case	TYPEFTL_CONTROLFTL:
				Element += "TYPEFTL_CONTROLFTL";
				break;
			case	TYPEFTL_FTLightOPEN:
				Element += "TYPEFTL_FTLightOPEN";
				break;
			case	TYPEFTL_FTLightWRAP:
				Element += "TYPEFTL_FTLightWRAP";
				break;
			case	TYPEFTL_DTI_MCL:
				Element += "TYPEFTL_DTI_MCL";
				break;
			case	TYPEFTL_BINFTLBINARY:
				Element += "TYPEFTL_BINFTLBINARY";
				break;
			case	TYPEFTL_BINFTLSTRING:
				Element += "TYPEFTL_BINFTLSTRING";
				break;
			case	TYPEFTL_BINFTLVALUE:
				Element += "TYPEFTL_BINFTLVALUE";
				break;
			case	TYPEFTL_BINFTLTIME:
				Element += "TYPEFTL_BINFTLTIME";
				break;
			case	TYPEFTL_CMFTLTOKEN:
				Element += "TYPEFTL_CMFTLTOKEN";
				break;
			case	TYPEFTL_CMFTLLINK:
				Element += "TYPEFTL_CMFTLLINK";
				break;
			case TYPEFTL_BINDIF:
				Element += "TYPEFTL_BINDIF";
				break;
			default:
				Element += "TYPEFTL_NONE";
			}
			Element += "</node";
			Element += LevelChild;
			Element += "_";
			Element += NodeIndex;
			Element += ">";
			XML += Element;
		}
	}
	XML += "\n</FTLight>";

	XML.writeBinary(FileFTLML);

	return true;
}


//---------------------------FTLight structure-functionality------------------

bool CmStringFTL::clearFTLightHierarchy()
{
	// delete all childs (the list of siblings)
	CmStringFTL* Sibling = Child;
	CmStringFTL* Delete = Sibling;
	for (; NULL != Child; Delete = Sibling) {
		// next sibling
		Sibling = Sibling->Sibling;
		// delete child
		releaseMemory<CmStringFTL>(Delete, 1, isCmStringFTLChild);
		if (Sibling == Child) break;
	}

	// delete navigation
	releaseMemory<NaviFTL>(Navigation, 1, isNaviFTL);

	// delete TXL
	releaseMemory<uint8>(TXL, (int32)LengthTXL + 1, isUint8);

	// re-initialize workspace
	Child = NULL;
	LineNumber = 0;
	LengthTXL = 0;
	DataFormat = DATAFORMAT_NONE;

	return true;
}

bool CmStringFTL::getIdentifier(CmString& _Info, CmIFTL& _IFTL)
{
	// find @ before delimiters ,:;=
	bool isAt = false;
	int32 i;
	for (i = 0; i < int32(_Info.getLength()); i++) {
		_Info[i] == '@' ? i > 0 ? _Info[i - 1] != '\\' ? isAt = true : 0 : 0 : 0;
		if (_Info[i] == ',' || _Info[i] == ':' || _Info[i] == ';' || _Info[i] == '=') {
			break; // delimiter found
		}
	}
	// without @ it is not a IFTL
	if (false == isAt) return false;

	// return found IFTL
	_IFTL.setIFTL(_Info, i - 1);

	return true;
}

void CmStringFTL::setParent(CmStringFTL* _Parent)
{
	Parent = _Parent;
}
CmStringFTL* CmStringFTL::getParent()
{
	return Parent;
}
CmStringFTL* CmStringFTL::getSibling()
{
	return Sibling;
}
CmStringFTL* CmStringFTL::getChild()
{
	return Child;
}
bool CmStringFTL::isBinary(bool _isParentBinary)
{
	bool isBinary = false;
	switch (TypeFTL) {
	case TYPEFTL_BINFTL:
	case TYPEFTL_CONTROLFTL:
	case TYPEFTL_FTLightOPEN:
	case TYPEFTL_FTLightWRAP:
	case TYPEFTL_DTI_MCL:
	case TYPEFTL_BINFTLBINARY:
	case TYPEFTL_BINFTLSTRING:
	case TYPEFTL_BINFTLVALUE:
	case TYPEFTL_BINFTLTIME:
	case TYPEFTL_CMFTLTOKEN:
	case TYPEFTL_CMFTLLINK:
	case TYPEFTL_BINDIF:
		isBinary = true;
		break;
	case TYPEFTL_TXL_BINFTL:
		// will be binary when parent was binary
		isBinary = _isParentBinary ? true : false;
		break;
	default: break;
	}

	return isBinary;
}

CmTypeFTL CmStringFTL::setTypeFTL(CmTypeFTL _TypeFTL)
{
	return TypeFTL = _TypeFTL;
}
CmTypeFTL CmStringFTL::getTypeFTL()
{
	return TypeFTL;
}

bool CmStringFTL::addChild(CmStringFTL** _Child)
{
	// create a new child
	CmStringFTL* NewChild = allocateMemory<CmStringFTL>(1, isCmStringFTLChild);
	if (NULL == NewChild) return false;

	// check if a child existed already
	if (NULL == Child) {
		Child = NewChild;
	}
	else {
		// insert new child into chain of siblings
		CmStringFTL* LastChild;
		if (false == Child->getLastChild(&LastChild)) {
			releaseMemory<CmStringFTL>(NewChild, 1, isCmStringFTLChild);
			return false;
		}
		LastChild->Sibling = NewChild;
		NewChild->Sibling = Child;
	}

	// establish a child-parent relationship
	NewChild->Parent = this;

	// return new child
	if (NULL == _Child) return false;
	*_Child = NewChild;

	return true;
}

bool CmStringFTL::findChild(CmStringFTL** _Child, CmString& _Content, CmTypeFTL _TypeFTL)
{
	// initialize return value
	if (NULL != _Child)
		*_Child = NULL;

	// check parameter and if any child exists
	if ((NULL == _Child) || (NULL == Child)) return false;

	// step through list of siblings
	*_Child = Child;
	do {
		// compare content
		if ((*_Child)->TypeFTL == _TypeFTL && (*_Child)->getLength() == _Content.getLength()) {
			if (0 == memcmp((*_Child)->getText(), _Content.getText(), _Content.getLength()))
				return true;
		}
		*_Child = (*_Child)->Sibling;
	} while (*_Child != Child);

	// no child matches
	*_Child = NULL;

	return false;
}

bool CmStringFTL::receiveLineFTL(CmString& _LineFTL)
{
	int32 LineNumber_l;
	uint32 LineStart_l;
	uint32 ActiveStart_l;
	uint32 ActiveLength_l;
	uint32 HighlightStart_l;
	uint32 HighlightLength_l;
	CmTypeFTL TypeFTL_l;
	CmStringFTL ItemFTL;
	CmStringFTL* LocalFTL = NULL;

	// initialize LineFTL reader for reading next line
	if (false == initLineFTL(_LineFTL, LineStart_l, ActiveStart_l, ActiveLength_l, HighlightStart_l, HighlightLength_l)) return false;

	// obtain all ItemFTL along with their types
	while (getNextItemFTL(_LineFTL, ItemFTL, TypeFTL_l, FTL()->isStartOfCollection, FTL()->isParentBinary)) {

		// determine active and highlighted text fragments
		_LineFTL.getActive(ActiveStart_l, ActiveLength_l, LineNumber_l);
		_LineFTL.getHighlight(HighlightStart_l, HighlightLength_l);

		// process item dependent on TypeFTL_l
		switch (TypeFTL_l) {
		case TYPEFTL_IDENTIFIER:
			FTL()->isSingleAddressBeforeAt = false;
			// at line 1, goto or create operator, location and identifier nodes
			if (1 == LineNumber_l && 0 == HighlightStart_l) {
				if (false == createRoot(ItemFTL)) return false;
				FTL()->isRepeatedPath = false;
				FTL()->isFirstItem = false;
				continue;
			}
			break;
		case TYPEFTL_Local_BINFTL:
			FTL()->isSingleAddressBeforeAt = false;
			// adjust PosFTL to addressed/Locald position
			if (false == getPosition(ItemFTL)) return false;
			FTL()->PathFTL = FTL()->PosFTL;
			FTL()->isFirstItem = false;
			// mark address if it is the first item in a line
			if (LineStart_l == HighlightStart_l) {
				FTL()->isSingleAddressBeforeAt = true;
			}
			continue;
		case TYPEFTL_Local:
			FTL()->isSingleAddressBeforeAt = false;
			// handle Local (for further items beyond line start)
			if (false == getLocal(ItemFTL, LocalFTL)) return false;
			break;
		case TYPEFTL_CHECKSUM:
			FTL()->isSingleAddressBeforeAt = false;
			// verify checksum
			if (false == verifyChecksum(_LineFTL, ItemFTL)) return false;
			continue;
		case TYPEFTL_TXL_BINFTL:
		case TYPEFTL_NUMBER_BINFTL:
			FTL()->isSingleAddressBeforeAt = false;
			if (FTL()->isFirstItem && false == FTL()->sync.isSynchronousWriting) {
				// remove escape ('\') character at first position if NOT synchronous writing
				ItemFTL.decodeTXL();
				ItemFTL.TypeFTL = TypeFTL_l; // restore type
			}
			break;
		case TYPEFTL_TXL:
		case TYPEFTL_NUMBER:
			FTL()->isSingleAddressBeforeAt = false;
			// remove escape ('\') character for type TXL
			ItemFTL.decodeTXL();
			ItemFTL.TypeFTL = TypeFTL_l; // restore type
			break;
		case TYPEFTL_FIXED_PARENTS:
			// restore synchronous writing if only a single address occured before this '@' item
			if (FTL()->isSingleAddressBeforeAt) {
				FTL()->isRestoreSynchronousWriting = true;
				FTL()->isSingleAddressBeforeAt = false;
			}
			break;
		case TYPEFTL_BINFTLTIME:
			FTL()->isSingleAddressBeforeAt = false;
			if (FTL()->isFirstItem) {
				FTL()->isParentBinary = true;
			}
			break;
		default:
			FTL()->isSingleAddressBeforeAt = false;
			break;
		}

		// analyse first item for synchronous writing 
		if (FTL()->isFirstItem || FTL()->isRestoreSynchronousWriting) {
			if (false == checkSyncronousWriting(TypeFTL_l)) return false;
		}

		// run synchronous writing
		if (FTL()->sync.isSynchronousWriting) {
			if (false == runSynchronousWriting(ItemFTL, TypeFTL_l)) return false;
			continue;
		}

		// evaluate repeated (empty) path items
		if (0 == HighlightLength_l) {
			if (true == evaluateRepeatedPath()) continue;
		}

		// generate child
		putChildOnPath(ItemFTL, TypeFTL_l, LocalFTL);

		// check whether path turns into a collection
		if (FTL()->isStartOfCollection) {
			FTL()->isPath = false;
		}

		// switch path to new child
		if (FTL()->isPath) {
			FTL()->PathFTL = FTL()->PosFTL;
		}
	}

	return true;
}

bool CmStringFTL::processStringFTL(const int8* _StringFTL)
{
	CmString StringFTL(_StringFTL);

	return processStringFTL(StringFTL);
}

bool CmStringFTL::processStringFTL(CmString& _StringFTL)
{
	// split string into lines
	uint32 StartPos = 0;
	uint32 FoundPos = 0;
	FTL()->isParentBinary = false;
	while (StartPos < _StringFTL.getLength()) {
		// find end of line
		FoundPos = _StringFTL.searchPattern("\x0A", StartPos);
		// convert line content and insert it into CmStringFTL hierarchy
		_StringFTL.setActive(StartPos, FoundPos - StartPos + 1, ++LineNumber);
		receiveLineFTL(_StringFTL);
		// next line
		StartPos = FoundPos + 1;
	}

	return true;
}

bool CmStringFTL::readFileFTL(const int8* _FileFTL)
{
	//read file from disk
	CmString FileFTL;
	try {
		FileFTL.readBinary(_FileFTL);
	}
	catch (...) {
		return false;
	}

	return processStringFTL(FileFTL);
}

bool CmStringFTL::serializeStringFTL(CmString& _StringFTL)
{
	// ToDo: complete all variants

	// add timestamp
	CmString Timestamp;
	bool isOption = false;
	bool isComment = false;
	bool isParentBinary = false;
	const bool isPerformanceCounter = false;
	uint64 uTimestamp = CmDateTime::getSysClockNanoSec(isPerformanceCounter) / DATETIME_NANOSECONDS;
	Timestamp.double2String((double)uTimestamp);
	setValueFTL("0-0", Timestamp);

	// convert CmStringFTL hierarchy to a string
	CmString LineFTL;
	CmStringFTL* NodeChild = this;
	CmStringFTL* NodeSibling = this;
	CmStringFTL* NodeLocal = NULL;
	int LevelChild = 0;
	// travel StringFTL hierarchy
	while (NULL != NodeSibling) {
		// first travel all childs 
		if (NULL != NodeSibling->getChild()) {
			if (LevelChild > 1) { LineFTL += (NodeSibling->getChild()->isBinary(isParentBinary) ? "=" : ":"); }
			LevelChild++;
			NodeChild = NodeSibling->getChild();
			NodeSibling = NodeChild;
			isOption = false;
			isComment = false;
		}
		else {
			// second travel siblings
			if (false == isOption && false == isComment && NodeSibling->getSibling() != NodeChild) {
				// ToDo: remove 'LevelChild < 6' limitation
				if (LevelChild >= 3 && LevelChild < 6) {
					LineFTL += "\n";
					for (int i = 2; i < LevelChild; i++) {
						LineFTL += NodeSibling->isBinary(isParentBinary) ? ";" : ",";
					}
				}
				else {
					LineFTL += NodeSibling->isBinary(isParentBinary) ? ";" : ",";
				}
				NodeSibling = NodeSibling->getSibling();
			}
			else {
				// no more siblings, travel levels backwards
				while (NULL != NodeSibling && NodeSibling->getSibling() == NodeChild) {
					LevelChild--;
					NodeSibling = NodeSibling->getParent();
					NodeChild = NodeSibling;
					if (NULL != NodeChild && NULL != NodeChild->getParent()) {
						NodeChild = NodeChild->getParent()->getChild();
					}
				}
				if (NULL != NodeSibling) {
					LineFTL += "\n";
					for (int i = LevelChild; i > 2; i--) LineFTL += NodeSibling->isBinary(isParentBinary) ? ";" : ",";
					NodeSibling = NodeSibling->getSibling();
				}
			}
		}
		// add Node value
		if (NULL != NodeSibling) {
			switch (NodeSibling->getTypeFTL()) {
			case TYPEFTL_OPTION:
				isOption = true;
				LineFTL += ":";
				LineFTL += (int8*)NodeSibling->encodeTXL().getTXL();
				break;
			case TYPEFTL_COMMENT:
				isComment = true;
				LineFTL += "::";
				LineFTL += (int8*)NodeSibling->encodeTXL().getTXL();
				break;
			case TYPEFTL_Local:
				// recover Locald node's address
				if (sizeof(NodeLocal) != NodeSibling->getLength()) {
					LineFTL += "--address length mismatch";
				}
				else {
					memcpy(&NodeLocal, NodeSibling->getBuffer(), sizeof(NodeLocal));
					LineFTL += "--";
					if (NULL != NodeLocal) {
						LineFTL += NodeLocal->getText();
					}
					else {
						LineFTL += "invalid Local";
					}
				}
				break;
			case TYPEFTL_Local_BINFTL:
				LineFTL += (int8*)NodeSibling->encodeTXL().getTXL();
				LineFTL += " (invalid Local)";
				break;
			default:
				LineFTL += LevelChild > 2 && false == NodeSibling->isBinary(isParentBinary) ? (int8*)NodeSibling->encodeTXL().getTXL() : NodeSibling->getText();
			}
			if (LevelChild > 1) { _StringFTL += LineFTL; }
			LineFTL = "";
			isParentBinary = NodeSibling->isBinary(isParentBinary) ? true : false;
		}
		else {
			_StringFTL += "\n";
		}

	}

	return true;
}

const CmString& CmStringFTL::getNode(const CmString& _Address)
{
	// get a node at specified position (create one if it does not existed yet)
	if (false == getPosition(_Address)) return _Address;

	// PosFTL points to specified node
	return *Navigation->PosFTL;
}

bool CmStringFTL::addItemFTL(const CmString& _Val)
{
	// get a FTL coded item
	CmString FTL;
	bin2FTL(&FTL, _Val.getBinary(), (int32)_Val.getLength());

	// increase string length and add a separator and the Item
	size_t PrevLength = Length;
	this->adjustLength(Length + 1 + FTL.getLength());
	setAt((int32)PrevLength, (uint8)';');
	memcpy(pText + PrevLength + 1, FTL.getBuffer(), FTL.getLength());
	DataFormat = DATAFORMAT_FTLight;

	return true;
}
bool CmStringFTL::addItemFTL(void* _Val)
{
	CmString Item(sizeof(void*));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(uint64 _Val)
{
	CmString Item(sizeof(uint64));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(int64 _Val)
{
	CmString Item(sizeof(int64));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(double _Val)
{
	CmString Item(sizeof(double));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(uint32 _Val)
{
	CmString Item(sizeof(uint32));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(int32 _Val)
{
	CmString Item(sizeof(int32));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(float _Val)
{
	CmString Item(sizeof(float));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(uint16 _Val)
{
	CmString Item(sizeof(uint16));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(int16 _Val)
{
	CmString Item(sizeof(int16));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(uint8 _Val)
{
	CmString Item(sizeof(uint8));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(int8 _Val)
{
	CmString Item(sizeof(int8));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}
bool CmStringFTL::addItemFTL(bool _Val)
{
	CmString Item(sizeof(bool));
	Item.setAt(0, _Val);
	return addItemFTL(Item);
}

CmDataFormat CmStringFTL::setDataFormat(CmDataFormat _DataFormat)
{
	return DataFormat = _DataFormat;
}
CmDataFormat CmStringFTL::getDataFormat()
{
	return DataFormat;
}
CmDataFormat CmStringFTL::getDataFormat(CmString& _DataFormat)
{
	switch (DataFormat) {
	case DATAFORMAT_NONE: _DataFormat = "DATAFORMAT_NONE"; break;
	case DATAFORMAT_FTLight: _DataFormat = "DATAFORMAT_FTLight"; break;
	case DATAFORMAT_STRING: _DataFormat = "DATAFORMAT_STRING"; break;
	case DATAFORMAT_BINARY: _DataFormat = "DATAFORMAT_BINARY"; break;
	case DATAFORMAT_UINT64: _DataFormat = "DATAFORMAT_UINT64"; break;
	case DATAFORMAT_INT64: _DataFormat = "DATAFORMAT_INT64"; break;
	case DATAFORMAT_DOUBLE: _DataFormat = "DATAFORMAT_DOUBLE"; break;
	case DATAFORMAT_UINT32: _DataFormat = "DATAFORMAT_UINT32"; break;
	case DATAFORMAT_INT32: _DataFormat = "DATAFORMAT_INT32"; break;
	case DATAFORMAT_FLOAT: _DataFormat = "DATAFORMAT_FLOAT"; break;
	case DATAFORMAT_UINT16: _DataFormat = "DATAFORMAT_UINT16"; break;
	case DATAFORMAT_INT16: _DataFormat = "DATAFORMAT_INT16"; break;
	case DATAFORMAT_UINT8: _DataFormat = "DATAFORMAT_UINT8"; break;
	case DATAFORMAT_INT8: _DataFormat = "DATAFORMAT_INT8"; break;
	case DATAFORMAT_BOOL: _DataFormat = "DATAFORMAT_BOOL"; break;
	}

	return DataFormat;
}

bool CmStringFTL::insertValueFTL(const CmString& _Address, const CmString& _ValueFTL)
{
	// obtain existing/create new node
	CmString ValueFTL = getNode(_Address);

	// set value
	ValueFTL.setText(_ValueFTL);

	return true;
}
bool CmStringFTL::setValueFTL(const CmString& _Address, const CmString& _ValueFTL)
{
	// adjust addressed FTLight node in PosFTL
	if (false == findPosition(_Address)) return false;

	// set new value
	FTL()->PosFTL->setText(_ValueFTL);
	FTL()->PosFTL->DataFormat = DATAFORMAT_FTLight;

	return true;
}
bool CmStringFTL::getValueFTL(const CmString& _Address, CmStringFTL& _ValueFTL, CmString _StartAddr)
{
	// adjust addressed FTLight node in PosFTL
	if (false == findPosition(_Address, _StartAddr)) return false;

	// convert from FTL to binary data if necessary
	if (DATAFORMAT_FTLight == FTL()->PosFTL->DataFormat) {
		switch (FTL()->PosFTL->TypeFTL) {
		case TYPEFTL_BINFTL: FTL()->PosFTL->decodeFTL(); break;
		default: break;
		}
	}

	// copy data and data format information to ValueFTL
	_ValueFTL.setLength(FTL()->PosFTL->getLength());
	memcpy(_ValueFTL.getBuffer(), FTL()->PosFTL->getBinary(), FTL()->PosFTL->getLength());
	_ValueFTL.DataFormat = FTL()->PosFTL->DataFormat;
	_ValueFTL.TypeFTL = FTL()->PosFTL->TypeFTL;

	return true;
}

bool CmStringFTL::getValue(const char* _Address, CmDataFormat _DataFormat, CmStringFTL** _Value)
{
	// adjust addressed FTLight node in PosFTL
	CmString Address_l(_Address);
	if (false == findPosition(Address_l)) return false;

	// convert from FTLight to appropriate data format if necessary
	if (DATAFORMAT_FTLight == FTL()->PosFTL->DataFormat) {
		switch (FTL()->PosFTL->TypeFTL) {
		case TYPEFTL_BINFTL: FTL()->PosFTL->decodeFTL(); break;
		case TYPEFTL_NUMBER:	FTL()->PosFTL->decodeNum(_DataFormat); break;
		default: break;
		}
	}

	// return found value
	if (NULL != _Value) {
		*_Value = FTL()->PosFTL;
	}

	return true;
}
bool CmStringFTL::getValue(const char* _Address, CmString& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_STRING, &Value)) return false;
	_Val = *Value;

	return true;
}
bool CmStringFTL::getValue(const char* _Address, void*& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_BINARY, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, uint64& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_UINT64, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, int64& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_INT64, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, double& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_DOUBLE, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, uint32& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_UINT32, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, int32& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_INT32, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, float& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_FLOAT, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, uint16& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_UINT16, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, int16& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_INT16, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, uint8& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_UINT8, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, int8& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_INT8, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const char* _Address, bool& _Val)
{
	CmStringFTL* Value;
	if (false == getValue(_Address, DATAFORMAT_BOOL, &Value)) return false;
	Value->getAt(0, _Val);

	return true;
}
// CmString Addr
bool CmStringFTL::getValue(const CmString& _Addr, CmDataFormat _DataFormat, CmStringFTL** _Val, CmString _StartAddr)
{
	// adjust addressed FTLight node in PosFTL
	if (false == findPosition(_Addr, _StartAddr)) return false;

	// convert from FTLight to appropriate data format if necessary
	if (DATAFORMAT_FTLight == FTL()->PosFTL->DataFormat && _DataFormat != DATAFORMAT_STRING) {
		switch (FTL()->PosFTL->TypeFTL) {
		case TYPEFTL_BINFTL: FTL()->PosFTL->decodeFTL(); break;
		case TYPEFTL_NUMBER:	FTL()->PosFTL->decodeNum(_DataFormat); break;
		default: break;
		}
	}

	// return found value
	if (NULL != _Val) {
		*_Val = FTL()->PosFTL;
	}

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, CmString& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_STRING, &Value, _StartAddr)) return false;
	_Val = *Value;

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, void*& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_BINARY, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, uint64& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_UINT64, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, int64& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_INT64, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, double& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_DOUBLE, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, uint32& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_UINT32, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, int32& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_INT32, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, float& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_FLOAT, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, uint16& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_UINT16, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, int16& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_INT16, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, uint8& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_UINT8, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, int8& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_INT8, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}
bool CmStringFTL::getValue(const CmString& _Addr, bool& _Val, CmString _StartAddr)
{
	CmStringFTL* Value;
	if (false == getValue(_Addr, DATAFORMAT_BOOL, &Value, _StartAddr)) return false;
	Value->getAt(0, _Val);

	return true;
}

//bool CmStringFTL::getDataItem(CmFlex _Flex, CmDataFormat _Format, const CmVector<const int8*>& _Path, int32 _Index)
//{
//
//	return true;
//}
bool CmStringFTL::getDataItem(CmString& _DataItem, CmVector<CmString*>& _Path, int32 _Index)
{
	CmStringFTL* pNode = this;
	// goto root
	while (NULL != pNode && NULL != pNode->Parent) {
		pNode = pNode->Parent;
	}
	// find identifier node
	while (NULL != pNode && TYPEFTL_IDENTIFIER != pNode->TypeFTL) {
		pNode = pNode->Child;
	}
	// find items on all requested levels
	CmStringFTL* pEntry = NULL;
	CmString* Item;
	_Path.reset();
	for (; NULL != pNode && _Path.allItems(Item);) {
		if (false == pNode->findChild(&pEntry, *Item)) break;
		pNode = pEntry;
	}
	// navigate to child with requested index
	NULL != pEntry ? pEntry = pEntry->Child : 0;
	for (int32 i = 0; NULL != pEntry && i < _Index; i++) {
		pEntry = pEntry->Sibling;
	}

	NULL != pEntry ? _DataItem = *pEntry : _DataItem.clear();

	return true;
}
//bool CmStringFTL::getDataValue(void*& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(uint64& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(int64& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(double& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(uint32& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(int32& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
bool CmStringFTL::getDataValue(float& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
{
	CmStringFTL* pNode = this;
	// goto root
	while (NULL != pNode && NULL != pNode->Parent) {
		pNode = pNode->Parent;
	}
	// find identifier node
	while (NULL != pNode && TYPEFTL_IDENTIFIER != pNode->TypeFTL) {
		pNode = pNode->Child;
	}
	// find items on all requested levels
	CmStringFTL* pEntry = NULL;
	CmString* Item;
	_Path.reset();
	for (; NULL != pNode && _Path.allItems(Item);) {
		if (false == pNode->findChild(&pEntry, *Item)) break;
		pNode = pEntry;
	}
	// navigate to child with requested index
	NULL != pEntry ? pEntry = pEntry->Child : 0;
	for (int32 i = 0; NULL != pEntry && i < _Index; i++) {
		pEntry = pEntry->Sibling;
	}

	_DataValue = NULL != pEntry ? float(pEntry->getNumAsDouble()) : 0;

	return true;
}
//bool CmStringFTL::getDataValue(uint16& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(int16& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(uint8& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(int8& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}
//bool CmStringFTL::getDataValue(bool& _DataValue, CmVector<CmString*>& _Path, int32 _Index)
//{
//
//	return true;
//}


int64 CmStringFTL::incrementSequenceNumber()
{
	return ++FTL()->sync.SequenceNumber;
}
bool CmStringFTL::getConfigPath(CmString _AppData, CmString _AppCompany, CmString _AppProgram, CmString _AppVersion)
{
	// check if AppData path is available
	setText(_AppData);
	if (false == PathFileExistsA(getText())) return false;

	// get company path
	*this += "\\";
	*this += _AppCompany;
	if (false == PathFileExistsA(getText())) {
		// create company path
		if (-1 == _mkdir(getText())) return false;
	}

	// get program path
	*this += "\\";
	*this += _AppProgram;
	if (false == PathFileExistsA(getText())) {
		// create program path
		if (-1 == _mkdir(getText())) return false;
	}

	// get profile_version path
	if (_AppProgram.getLength() > 0 && _AppVersion.getLength() > 0) {
		*this += "\\";
		*this += _AppProgram;
		*this += "_";
		*this += _AppVersion;
		if (false == PathFileExistsA(getText())) {
			// create program path
			if (-1 == _mkdir(getText())) return false;
		}
	}

	// trailing backslash
	*this += "\\";

	return true;
}

bool CmStringFTL::isDataTypeIdentifier(CmString& _FTLight, FTLight_DATA_TYPE_IDENTIFIER _eDataType)
{
	uint32 uNum = 0;
	// Evaluate first 4 bytes
	if (_FTLight.getLength() >= 4) {
		for (int i = 3; i >= 0; i--) {
			uNum *= 216;
			uNum += *((uint8*)(_FTLight.getBuffer() + i)) - 32;
		}
		if ((uint32)_eDataType == uNum) {
			return true;
		}
	}
	return false;
}
bool CmStringFTL::setFTLightDataType(CmString& _FTL, FTLight_DATA_TYPE_IDENTIFIER _FTLightType)
{
	// set result length
	_FTL.setLength(4);
	// decompose identifier value
	uint32 TypeFTL = _FTLightType;
	for (int i = 0; i < 4; i++) {
		_FTL.setAt(i, BINFTL[(uint8)(TypeFTL % 216)]);
		TypeFTL -= TypeFTL % 216;
		TypeFTL /= 216;
	}

	return true;
}

bool CmStringFTL::getFTLightDataType(CmString& _FTL, FTLight_DATA_TYPE_IDENTIFIER& _FTLightType)
{
	// initialize return value
	_FTLightType = FTLight_NONE;

	// check data availabílity
	uint32 ActiveStart;
	uint32 ActiveLength;
	int32 LineNumber;
	if (_FTL.getLength() < 4) return false;
	_FTL.getActive(ActiveStart, ActiveLength, LineNumber);
	if (ActiveLength < 4) return false;

	// decode field
	uint32 FTLightType = 0;
	for (int i = 3; i >= 0; i--)
	{
		FTLightType *= 216;
		FTLightType += SYMBOL[(uint8)(*(_FTL.getBuffer() + ActiveStart + i))];
	}

	// check for valid range
	if ((FTLightType < FTLight_DTI_MIN) || (FTLightType > FTLight_DTI_MAX)) return false;

	_FTLightType = (FTLight_DATA_TYPE_IDENTIFIER)FTLightType;

	return true;
}


//===========================Private-functions================================

//---------------------------FTL----------------------------------------------
 
// FTLight bin encode, byt<=215
uint8 CmStringFTL::Symbol2FTL(uint8 _Symbol)
{
	uint8 b;
	switch (_Symbol)
	{
	case 12: b = 248; break;      // avoid ,
	case 13: b = 249; break;      // avoid -
	case 26: b = 250; break;      // avoid :
	case 27: b = 251; break;      // avoid ;
	case 29: b = 252; break;      // avoid =
	case 32: b = 253; break;      // avoid @
	case 64: b = 254; break;      // avoid `
	case 95: b = 255; break;      // avoid del
	default: b = _Symbol + 32;
	}
	return b;
}

// FTLight bin decode
uint8 CmStringFTL::FTL2Symbol(uint8 _FTL)
{
	uint8 b;
	switch (_FTL)
	{
	case 248: b = 12; break;      // avoid ,
	case 249: b = 13; break;      // avoid -
	case 250: b = 26; break;      // avoid :
	case 251: b = 27; break;      // avoid ;
	case 252: b = 29; break;      // avoid =
	case 253: b = 32; break;      // avoid @
	case 254: b = 64; break;      // avoid `
	case 255: b = 95; break;      // avoid del
	default: b = _FTL - 32;
	}
	return b;
}

size_t CmStringFTL::getLengthTXL2bin(size_t _LengthTXL, bool _fTail)
{
	int32 nFTLbits = 31 * ((int32)_LengthTXL / 4) + (((int32)_LengthTXL % 4) == 0 ? 0 : 8 * ((int32)_LengthTXL % 4) - 1);
	size_t nBinLen = nFTLbits / 8 + ((_fTail == true) && (nFTLbits < 64) && (nFTLbits % 8 > 0) ? 1 : 0);

	return nBinLen;
}

CmString* CmStringFTL::bin2FTL(CmString* _pFTL, const uint8* _pBuf, int32 _nBinLen, int32 _nDigits)
{
	int64 n64Bits = 8 * _nBinLen;
	int32 nFTLLen = _nDigits > 0 ? _nDigits : (int32)(4 * ((n64Bits) / 31) + ((n64Bits) % 31) / 8 + ((n64Bits) % 31 > 0 ? 1 : 0));
	int64 n64Used;
	int32 nShift;
	int32 nPos = 0;
	int64 n64Factor;
	uint8 uchCode;
	uint32 uQuadrupel;
	uint64 u64Register;

	// check conversion table
	if (false == isConversionTable()) return NULL;

	// Determine length of the resulting FTL string
	_pFTL->setLength(nFTLLen);

	// Split input bit field into 31-bit chunks
	for (n64Used = 0; n64Used < n64Bits; n64Used += 31) {
		//Load register
		if (n64Used / 8 + 8 <= _nBinLen) {
			u64Register = *((uint64*)(_pBuf + (int32)(n64Used / 8)));
		}
		else {
			u64Register = 0;
			n64Factor = 1;
			for (int i = (int32)(n64Used / 8); i < _nBinLen; i++) {
				u64Register += *(_pBuf + i) * n64Factor;
				n64Factor *= 256;
			}
		}
		// Bit shift
		nShift = (int32)(n64Used % 8);
		if (0 == nShift) {
			uQuadrupel = (uint32)u64Register;
		}
		else {
			u64Register >>= nShift;
			uQuadrupel = (uint32)u64Register;
		}
		uQuadrupel &= 0x7FFFFFFF;
		// Code generation
		// ToDo: accellerate by using 2 quadrupels in one conversion
		for (int i = 0; i < 4 && nPos < nFTLLen; i++) {
			uchCode = uQuadrupel % RADIX_216;
			uQuadrupel /= RADIX_216;
			uchCode = *(uint8*)(MCLconvert + uchCode);
			_pFTL->setAt(nPos++, uchCode);
		}
	}
	return _pFTL;
}

int32 CmStringFTL::FTL2bin(uint8* _pBuf, int32 _nBufSize, CmString& _mFTL, bool _fTail)
{
	size_t nFTLLen = _mFTL.getLength();
	size_t nBinLen = getLengthTXL2bin(nFTLLen, _fTail);
	int32 nPos;
	uint8 uchMerge;
	int64 n64Used = 0;
	int32 nShift;
	int32 nFactor;
	uint8 uchCode;
	bool  fMerge;
	uint64 uQuadrupel;

	// Check if buffer size is sufficient
	memset(_pBuf, 0, _nBufSize);
	if (_nBufSize < (int32)nBinLen) {
		throw CmException("Unsufficient binary buffer size", (uint32)_nBufSize);
		return 0;
	}
	for (nPos = 0; nPos < (int32)nFTLLen; nPos += 4) {
		//Decode FTL characters
		uQuadrupel = 0;
		nFactor = 1;
		for (int i = nPos; i < nPos + 4 && i < (int32)nFTLLen; i++) {
			uchCode = *(_mFTL.getBuffer() + i);
			switch (uchCode) {
			case 248: uchCode = 12; break;		//avoid 44 - comma
			case 249: uchCode = 13; break;		//avoid 45 - minus sign
			case 250: uchCode = 26; break;		//avoid 58 - colon
			case 251: uchCode = 27; break;		//avoid 59 - semicolon
			case 252: uchCode = 29; break;		//avoid 61 - equal sign
			case 253: uchCode = 32; break;		//avoid	64 - @ sign
			case 254: uchCode = 64; break;		//avoid 96 - back apostroph
			case 255: uchCode = 95; break;		//avoid 127- del
			default:  uchCode -= 32;
			}
			uQuadrupel += uchCode * nFactor;
			nFactor *= RADIX_216;
		}
		// Generate bit sequence
		nShift = (int32)(n64Used % 8);
		fMerge = true;
		if (0 == nShift) {
			fMerge = false;
		}
		else {
			uQuadrupel <<= nShift;
		}
		if (true == fMerge) {
			uchMerge = *(_pBuf + (int32)(n64Used / 8));
			uQuadrupel += uchMerge;
		}
		// Fill buffer
		if (n64Used / 8 + 4 < (int32)nBinLen) {
			*(int32*)(_pBuf + (int32)(n64Used / 8)) = (int32)uQuadrupel;
			if (nShift > 1) {
				*(_pBuf + (int32)(n64Used / 8) + 4) = (int8)(uQuadrupel >> 32);
			}
		}
		else {
			for (int32 i = (int32)(n64Used / 8); i < (int32)nBinLen; i++) {
				*(_pBuf + i) = (int8)(uQuadrupel);
				uQuadrupel >>= 8;
			}
		}
		n64Used += 31;
	}

	return (int32)nBinLen;
}

//---------------------------MCL----------------------------------------------

bool CmStringFTL::bin2MCL(int _n, uint16* _inp, uint16* _out)
{
	int i;
	union shifter {
		uint16 wrd[2];			//[0]=lsw [1]=msw
		unsigned long lwr;
	} shf;

	// check conversion table
	if (false == isConversionTable()) return false;

	// convert n groups of 15 words into 32n FTLight legal chars
	for (i = 0; i < _n; i++) {									//30270 = 32768*15/16
		shf.lwr = 0;

		shf.wrd[0] = *_inp++;									//get input word
		shf.lwr = shf.lwr << 1;									//save msb
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));	//table lookup and store two chars

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		shf.wrd[0] = *_inp++;
		shf.lwr = shf.lwr << 1;
		*_out++ = *(uint16*)(MCLconvert + (shf.wrd[0] >> 1));

		*_out++ = *(uint16*)(MCLconvert + shf.wrd[1]);                   //store the word made of msb's
	}
	return true;
}

bool CmStringFTL::MCL2bin(int _n, uint16* _inp, uint16* _out)
{
	//unpacks n 32 byte groups into n 15 word groups
	uint16 w, wb[15];
	for (int i = 0; i < _n; i++) {
		for (int j = 0; j < 15; j++) {
			w = *_inp++;
			wb[j] = 216 * SYMBOL[(uint8)(w / 256)] + SYMBOL[(uint8)(w % 256)];
		}
		w = *_inp++;
		w = 216 * SYMBOL[(uint8)(w / 256)] + SYMBOL[(uint8)(w % 256)];
		w = w << 1;
		for (int j = 0; j < 15; j++)
		{
			*_out++ = (wb[j]) + (w & 32768);
			w = w << 1;
		}
	}
	return true;
}

//---------------------------DIF----------------------------------------------

bool CmStringFTL::val2DIF(CmVector<uint8>& _DIF, CmVector<int64>& _Values, int32 _insertAbsoluteValue)
{
	CmVector<uint8>& D = _DIF;
	CmVector<int64>& V = _Values;

	// D : [i] = DIF
	// V : [i] = Value

	// check conversion tables
	if (false == isConversionTable()) return false;

	// convert all values to DIF symbols
	int32 I = 0;
	int64 Value, PrevVal = V[0];
	uint8 Repetition = 210;
	uint8 PrevFTL = BINFTL[210];
	uint8 PrevPrevFTL = PrevFTL;
	int32 ValDIFcount = _insertAbsoluteValue;
	D.clear();
	for (int i = 0; i < V.getLength(); i++) {
		// get dif
		int64 ValDIF = (Value = V[i]) - PrevVal;
		PrevVal = Value;
		// check for absolute value
		if (abs(ValDIF) > 100 || ValDIFcount >= _insertAbsoluteValue) {
			// finalize eventually still running repetition
			if (Repetition > 210) {
				Repetition = 210;
				I++;
			}
			// insert absolute value
			int64 Digits216 = getDigits216(Value);	// negative Value will be transformed to positive range
			D[I++] = BINFTL[uint8(200 + Digits216)];
			for (int d = 0; d < Digits216; d++) {
				PrevPrevFTL = PrevFTL;
				D[I++] = PrevFTL = BINFTL[Value % 216];
				Value = Value / 216;
			}
			ValDIFcount = 0;
		}
		else {
			// insert DIF value
			uint8 FTL = BINFTL[uint8(ValDIF + 100)];
			// insert FTL and take care for repeated FTL values
			if (0 == ValDIFcount) {
				// no repeated FTL after absolute value (potential risk of misleading symbols)
				PrevPrevFTL = PrevFTL;
				D[I++] = PrevFTL = FTL;
				ValDIFcount++;
			}
			else if (Repetition > 210) {
				// previous FTL repetition
				if ((FTL == PrevPrevFTL) && (Repetition != 215)) {
					// repetion further continues
					D[I] = BINFTL[++Repetition];
				}
				else {
					// max repetitions exceeded, insert new DIF
					Repetition = 210;
					PrevPrevFTL = PrevFTL;
					D[++I] = PrevFTL = FTL;
					ValDIFcount++;
					I++;
				}
			}
			else if (FTL != PrevFTL) {
				// no repeated FTL
				PrevPrevFTL = PrevFTL;
				D[I++] = PrevFTL = FTL;
				ValDIFcount++;
			}
			else if ((FTL == PrevFTL) && (FTL == PrevPrevFTL) && (ValDIFcount > 1)) {
				// start new repetition
				D[--I] = PrevFTL = BINFTL[Repetition = 211];
			}
			else {
				// no repeated FTL
				PrevPrevFTL = PrevFTL;
				D[I++] = PrevFTL = FTL;
				ValDIFcount++;
			}
		}
	}

	return true;
}

bool CmStringFTL::val2DIF(CmVector<uint8>& _DIF, CmVector<uint8>& _Values, bool _isEntangled, int32 _insertAbsoluteValue)
{
	// generate int64 vector
	const int32 DataLength = _Values.getLength();
	const int32 Bits = sizeof(_Values[0]) * 8;
	CmVector<int64> Values;
	if (_isEntangled) {
		// DIF entangled (astro image): _insertAbsoluteValue=62 -> ~ +3% data, _insertAbsoluteValue=65536 -> ~ -0.1% data
		Values.setLength(DataLength / 2);
		// entangle two values
		int64 Value0, Value1;
		for (int32 i = 0; i < DataLength / 2; i++) {
			Value0 = _Values[2 * i + 0];
			Value1 = _Values[2 * i + 1];
			// generate bit gaps
			uint64 Mask = 0x7F;
			for (int j = 0; j < Bits - 1; j++, Mask >>= 1) {
				Value0 = 2 * (Value0 & ~Mask) + (Value0 & Mask);
				Value1 = 2 * (Value1 & ~Mask) + (Value1 & Mask);
			}
			// entangled int64 value
			Values[i] = Value0 | 2 * Value1;
		}
	}
	else {
		// convert uint8 input values to int64
		Values.setLength(DataLength);
		for (int32 i = 0; i < DataLength; i++) {
			Values[i] = int64(_Values[i]);
		}
	}
	// convert int64 vector to DIF
	_DIF.clear();
	return val2DIF(_DIF, Values, _insertAbsoluteValue);
}

bool CmStringFTL::DIF2val(CmVector<uint8>& _DIF, CmVector<int64>& _Values)
{
	CmVector<uint8>& D = _DIF;
	CmVector<int64>& V = _Values;

	// D : [i] = FTL
	// V : [i] = Value

	// check conversion tables
	if (false == isConversionTable()) return false;

	// convert a DIF data stream to int64 values
	int32 I = 0;
	int32 Repetitions = 210;
	int64 Value = 0;
	uint8 PrevSymbol = 210;
	uint8 AbsoluteSymbols = 200;
	int64 AbsoluteRange = 1;
	V.clear();
	for (int32 i = 0; i < D.getLength(); i++) {
		uint8 Symbol = SYMBOL[D[i]];
		if (PrevSymbol > 200 && PrevSymbol < 210) {
			if (AbsoluteSymbols++ < PrevSymbol) {
				// assemble an absolute value
				Value = Value + Symbol * AbsoluteRange;
				AbsoluteRange *= 216;
			}
			if (AbsoluteSymbols >= PrevSymbol) {
				// absolute value finished, adjust sign
				Value >= AbsoluteRange / 2 ? Value -= AbsoluteRange : 0;
				V[I++] = Value;
				PrevSymbol = 210;
			}
		}
		else if (Symbol > 200 && Symbol < 210) {
			// start of absolute value
			PrevSymbol = Symbol;
			AbsoluteSymbols = 200;
			AbsoluteRange = 1;
			Value = 0;
		}
		else if (Symbol > 210) {
			// repetition
			while (Repetitions++ <= Symbol) {
				V[I++] = Value = Value + PrevSymbol - 100;
			}
			Repetitions = 210;
			PrevSymbol = Symbol;
		}
		else if (Symbol == 210) {
			// empty value: insert minimal int64 value
			V[I++] = MIN_INT64;
			PrevSymbol = Symbol;
		}
		else {
			// DIF value
			V[I++] = Value = Value + Symbol - 100;
			PrevSymbol = Symbol;
		}
	}

	return true;
}

bool CmStringFTL::DIF2val(CmVector<uint8>& _DIF, CmVector<uint8>& _Values, bool _isEntangled)
{
	// convert DIF to value array
	CmVector<int64> Values;
	if (false == DIF2val(_DIF, Values)) return false;

	// return decoded values
	const int32 Bits = sizeof(_Values[0]) * 8;
	const int32 DataLength = Values.getLength();
	if (_isEntangled) {
		_Values.setLength(DataLength * 2);
		for (int32 i = 0; i < DataLength; i++) {
			// separate two values
			int64 Value0, Value1;
			Value0 = Values[i] & 0x5555;
			Value1 = (Values[i] / 2) & 0x5555;
			uint64 Mask = 0x3FFF;
			for (int j = 0; j < Bits - 1; j++, Mask >>= 2) {
				Value0 = (Value0 & ~Mask) / 2 + (Value0 & Mask);
				Value1 = (Value1 & ~Mask) / 2 + (Value1 & Mask);
			}
			// return original values
			_Values[2 * i + 0] = uint8(Value0);
			_Values[2 * i + 1] = uint8(Value1);
		}
	}
	else {
		_Values.setLength(DataLength);
		for (int32 i = 0; i < DataLength; i++) {
			// return original value
			_Values[i] = uint8(Values[i]);
		}
	}

	return true;
}

bool CmStringFTL::asVector(CmVector<int64>& _Data)
{
	// align vector lengths with string length
	CmVector<uint8> DIF;
	DIF.setLength(int32(Length));
	// copy data to vector 
	memcpy(DIF.getData(), pText, Length);
	// decode DIF data to 64bit values
	return DIF2val(DIF, _Data);
}

int64 CmStringFTL::getDigits216(int64& _Value)
{
#define RANGE_1		216
#define RANGE_2		46656
#define RANGE_3		10077696
#define RANGE_4		2176782336
#define RANGE_5		470184984576
#define RANGE_6		101559956668416
#define RANGE_7		21936950640377856
#define RANGE_8		4738381338321616896

	// determine the minimal amount of digits for representing Value base 216 in DIF format
	int64 Value = abs(_Value);
	if (Value < RANGE_1 / 2) {
		_Value < 0 ? _Value += RANGE_1 : 0;
		return 1;
	}
	else if (Value < RANGE_2 / 2) {
		_Value < 0 ? _Value += RANGE_2 : 0;
		return 2;
	}
	else if (Value < RANGE_3 / 2) {
		_Value < 0 ? _Value += RANGE_3 : 0;
		return 3;
	}
	else if (Value < RANGE_4 / 2) {
		_Value < 0 ? _Value += RANGE_4 : 0;
		return 4;
	}
	else if (Value < RANGE_5 / 2) {
		_Value < 0 ? _Value += RANGE_5 : 0;
		return 5;
	}
	else if (Value < RANGE_6 / 2) {
		_Value < 0 ? _Value += RANGE_6 : 0;
		return 6;
	}
	else if (Value < RANGE_7 / 2) {
		_Value < 0 ? _Value += RANGE_7 : 0;
		return 7;
	}
	else if (Value < RANGE_8 / 2) {
		_Value < 0 ? _Value += RANGE_8 : 0;
		return 8;
	}
	else {
		// NOTE: huge negative numbers where abs(_Value) >= RANGE_8 are not suppported
		return 9;
	}
}

//---------------------------FTLight-structure-functionality------------------

bool CmStringFTL::createRoot(CmString& _Identifier)
{
	// extract operator and location from identifier
	CmString Operator;
	CmString Location;
	CmIFTL IFTL("", _Identifier.getText());
	Operator = IFTL.getOperator();
	Location = IFTL.getLocation();

	// add an operator node if this node was different
	FTL()->PathFTL = this;
	if ((TYPEFTL_OPERATOR != FTL()->PathFTL->TypeFTL) || (0 != FTL()->PathFTL->getLength() && *FTL()->PathFTL != Operator)) {
		FTL()->PathFTL->addChild(&FTL()->PosFTL);
		if (NULL == FTL()->PosFTL) return false;
		FTL()->PathFTL = FTL()->PosFTL;
		FTL()->PathFTL->TypeFTL = TYPEFTL_OPERATOR;
		FTL()->PathFTL->DataFormat = DATAFORMAT_FTLight;
	}
	if (0 == FTL()->PathFTL->getLength()) {
		FTL()->PathFTL->setText(Operator);
		FTL()->PathFTL->DataFormat = DATAFORMAT_FTLight;
	}

	// search for a location node
	FTL()->PathFTL->findChild(&FTL()->PosFTL, Location);
	if (NULL != FTL()->PosFTL) {
		FTL()->PathFTL = FTL()->PosFTL;
	}
	else {
		// add a location node since it does not exist yet
		FTL()->PathFTL->addChild(&FTL()->PosFTL);
		if (NULL == FTL()->PosFTL) return false;
		FTL()->PathFTL = FTL()->PosFTL;
		FTL()->PathFTL->TypeFTL = TYPEFTL_LOCATION;
		FTL()->PathFTL->setText(Location);
		FTL()->PathFTL->DataFormat = DATAFORMAT_FTLight;
	}

	// search for an identifier node
	FTL()->PathFTL->findChild(&FTL()->PosFTL, _Identifier);
	if (NULL != FTL()->PosFTL) {
		FTL()->PathFTL = FTL()->PosFTL;
	}
	else {
		// add an identifier node since it does not exist yet
		FTL()->PathFTL->addChild(&FTL()->PosFTL);
		if (NULL == FTL()->PosFTL) return false;
		FTL()->PathFTL = FTL()->PosFTL;
		FTL()->PathFTL->TypeFTL = TYPEFTL_IDENTIFIER;
		FTL()->PathFTL->setText(_Identifier);
		FTL()->PathFTL->DataFormat = DATAFORMAT_FTLight;
	}

	// set Root to found respectively created identifier node
	if (NULL != FTL()->Root && *FTL()->Root != _Identifier) return false;
	FTL()->Root = FTL()->PathFTL;
	FTL()->PosFTL = FTL()->Root;

	return true;
}

bool CmStringFTL::createRoot()
{
	// make this a root node above "0" in a CmStringFTL hierarchy
	Parent = NULL;
	Sibling = this;
	Child = NULL;
	FTL()->Root = this;
	FTL()->PathFTL = this;
	FTL()->PosFTL = this;

	return true;
}

bool CmStringFTL::getParent(CmStringFTL** _Parent)
{
	// check parameter
	if ((NULL == _Parent) || (NULL == Parent)) return false;

	// return parent node
	*_Parent = Parent;

	return true;
}

bool CmStringFTL::getChild(int32 _ChildPosition)
{
	// goto first child
	if (NULL != FTL()->PosFTL/* && NULL != FTL()->PosFTL*/->Child) {
		FTL()->PosFTL = FTL()->PosFTL->Child;
	}
	else {
		CmStringFTL* FirstChild = NULL;
		if (false == FTL()->PosFTL->addChild(&FirstChild)) return false;
		FTL()->PosFTL = FirstChild;
		*FTL()->PosFTL += "-";
	}

	// find/create appropriate sibling
	while (_ChildPosition-- > 0) {
		// check if a next sibling exists (it must not be the first one)
		if (NULL != FTL()->PosFTL->Sibling && FTL()->PosFTL->Sibling != FTL()->PosFTL->Sibling->Parent->Child) {
			FTL()->PosFTL = FTL()->PosFTL->Sibling;
		}
		else {
			// create a new sibling since PosFTL points already to the last one
			CmStringFTL* NewChild = allocateMemory<CmStringFTL>(1, isCmStringFTLChild);
			if (NULL == NewChild) return false;
			NewChild->setParent(FTL()->PosFTL->Parent);
			NewChild->Sibling = FTL()->PosFTL->Sibling;
			FTL()->PosFTL->Sibling = NewChild;
			FTL()->PosFTL = NewChild;
			*FTL()->PosFTL += "-";
		}
	}

	return true;
}

bool CmStringFTL::isChild(int32 _ChildPosition)
{
	// goto first child
	if ((NULL == FTL()->PosFTL) || (NULL == FTL()->PosFTL->Child)) return false;
	FTL()->PosFTL = FTL()->PosFTL->Child;

	// find appropriate sibling
	while (_ChildPosition-- > 0) {
		// check if a next sibling exists (its not the first one)
		if (FTL()->PosFTL->Sibling == FTL()->PosFTL->Sibling->Parent->Child) return false;
		FTL()->PosFTL = FTL()->PosFTL->Sibling;
	}

	return true;
}

bool CmStringFTL::getLastChild(CmStringFTL** _LastChild)
{
	// check parameter
	if (NULL == _LastChild) return false;

	// search for last child
	*_LastChild = this->Sibling;
	while ((*_LastChild)->Sibling != this) {
		*_LastChild = (*_LastChild)->Sibling;
	}

	return true;
}

bool CmStringFTL::getNextItemFTL(CmString& _LineFTL, CmString& ItemFTL, CmTypeFTL& _TypeFTL, bool& _isStartOfCollection, bool _isParentBinary)
{
	// initialize control variables
	bool isStartOfLine = false;
	int32 LineNumber_l;
	uint32 ActiveStart_l;
	uint32 ActiveLength_l;
	uint32 HighlightStart_l;
	uint32 HighlightLength_l;
	_LineFTL.getActive(ActiveStart_l, ActiveLength_l, LineNumber_l);
	_LineFTL.getHighlight(HighlightStart_l, HighlightLength_l);
	if (ActiveStart_l == HighlightStart_l && ActiveLength_l == HighlightLength_l) {
		isStartOfLine = true;
	}
	HighlightStart_l = ActiveStart_l;
	HighlightLength_l = 0;
	_LineFTL.setHighlight(HighlightStart_l, HighlightLength_l);
	_isStartOfCollection = false;
	_TypeFTL = TYPEFTL_TXL;

	// initialize ItemFTL
	ItemFTL.setLength(0);

	// check data availability
	if (0 == ActiveLength_l) return false;

	// flags to run a state machine
	bool isDetermined = true;
	bool isIdentifier = false;
	bool isNumber = true;
	bool isHexZero = false;
	bool isHexNumber = false;
	bool isHexDigit = false;
	bool isFirstDigit = false;
	bool isAddress = true;
	bool isAddressDelimiter = false;
	bool isChecksum = false;
	bool isText = true;
	bool isLineEnd = false;
	bool isQuery = false;
	bool isDelete = false;
	bool isEscape = false;
	bool isDelimiter = true;
	bool isNextDelimiter = false;
	bool isFramework = false;
	bool isOption = false;
	bool isComment = false;
	bool isProcessed = true;		// processing flag for option/comment

	// process first byte (usually a delimiter, however exceptions exist)
	ActiveLength_l--;
	HighlightLength_l++;
	const uint8* src = _LineFTL.getBinary() + ActiveStart_l++;
	switch (*src) {
	case 0:  isLineEnd = true; break;	// end of string
	case 10: isLineEnd = true; break;	// line feed
	case 13: isLineEnd = true; break;	// carriage return
	case 44: break;	// comma
	case 58: _isStartOfCollection = true; isFramework = true; break;	// colon
	case 59: isText = false; isNumber = false; break;	// semicolon
	case 61: isText = false; isNumber = false; isChecksum = true; _isStartOfCollection = true; break;	// equal sign
	case 64: return false;	// "at" sign, not allowed as first data in a field
	case 96: isQuery = true; isAddress = false; break;	// backtick: query, however, it has to be a single char
	case 127: isDelete = true; break;	// delete
	case '0': isDelimiter = false; isHexZero = true; break;
	case '1': isDelimiter = false; break;
	case '2': isDelimiter = false; break;
	case '3': isDelimiter = false; break;
	case '4': isDelimiter = false; break;
	case '5': isDelimiter = false; break;
	case '6': isDelimiter = false; break;
	case '7': isDelimiter = false; break;
	case '8': isDelimiter = false; break;
	case '9': isDelimiter = false; break;
	case '-': isDelimiter = false; isAddress = false; break;
	case '+': isDelimiter = false; isAddress = false; break;
	case '.': isDelimiter = false; isAddress = false; break;
	case 'E': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'e': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'X': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'x': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'A': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'a': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'B': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'b': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'C': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'c': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'D': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'd': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'F': isDelimiter = false; isAddress = false; isHexDigit = true; break;
	case 'f': isDelimiter = false; isAddress = false; isHexDigit = true; break;

	default: isDelimiter = false; isNumber = false; isAddress = false; break;
	}

	// implicite (left out) identifier if a line starts with a delimiter
	if (isStartOfLine && isDelimiter) {
		ActiveStart_l--;
		ActiveLength_l++;
		HighlightLength_l--;
		_LineFTL.setActive(ActiveStart_l, ActiveLength_l);
		_LineFTL.setHighlight(HighlightStart_l, HighlightLength_l);
		// a (left out) identifier at start of line is common in FTLight
		_TypeFTL = TYPEFTL_IDENTIFIER;
		return true;
	}

	// finish when line end has been reached
	if (isLineEnd) {
		_LineFTL.setActive(ActiveStart_l, ActiveLength_l);
		_LineFTL.setHighlight(HighlightStart_l, HighlightLength_l);
		// line end at the beginning should never happen
		return false;
	}

	// check if the data type could be uniquely determined
	if (isStartOfLine && false == isDelimiter)
		isDetermined = false;

	// skip delimiter in the beginning
	if (isDelimiter) {
		HighlightStart_l++;
		HighlightLength_l--;
		// next byte is the first digit
		isFirstDigit = true;
	}

	// find next delimiter
	while (ActiveLength_l > 0) {
		src = _LineFTL.getBinary() + ActiveStart_l;
		switch (*src) {
		case 0:  isLineEnd = true; break;	// end of string
		case 10: isLineEnd = true; break;	// line feed
		case 13: isLineEnd = true; break;	// carriage return
		case 44: // comma
			if (isEscape == false) {
				isNextDelimiter = true;
				isChecksum = false;
				break;
			}
			isEscape = false;
			break;
		case 58: // colon
			if (isEscape == false) {
				if (isFramework && 0 == HighlightLength_l) {
					// option detected
					isFramework = false;
					isOption = true;
					isProcessed = false;
				}
				else
					if (isOption && 0 == HighlightLength_l) {
						// comment detected
						isOption = false;
						isComment = true;
						isProcessed = false;
					}
					else {
						isNextDelimiter = true;
					}
				isChecksum = false;
				break;
			}
			isEscape = false;
			break;
		case 59: // semicolon
			if (isEscape == false) {
				isNextDelimiter = true;
				isChecksum = false;
				break;
			}
			isEscape = false;
			break;
		case 61: // equal sign
			if (isEscape == false) {
				isNextDelimiter = true;
				isChecksum = true;
				break;
			}
			isEscape = false;
			break;
		case 64: // "at" sign
			if (isEscape == false) {
				isIdentifier = true;
				break;
			}
			isEscape = false;
			isAddress = false;
			break;
		case 92: // backslash
			if (isText /*text field*/ && (isDetermined || _isParentBinary == false) /*no synchronous binary parent*/) {
				isEscape = true;
			}
			break;
		case 96: // backtick
			if (isEscape == false) {
				isQuery = true;
				isAddress = false;
				break;
			}
			isEscape = false;
			break;
		case 127: // delete
			if (isEscape == false) {
				isDelete = true;
				isAddress = false;
				break;
			}
			isEscape = false;
			break;
		case '0': isEscape = false; isDelimiter = false; isFirstDigit ? isHexZero = true : 0; break;
		case '1': isEscape = false; isDelimiter = false; break;
		case '2': isEscape = false; isDelimiter = false; break;
		case '3': isEscape = false; isDelimiter = false; break;
		case '4': isEscape = false; isDelimiter = false; break;
		case '5': isEscape = false; isDelimiter = false; break;
		case '6': isEscape = false; isDelimiter = false; break;
		case '7': isEscape = false; isDelimiter = false; break;
		case '8': isEscape = false; isDelimiter = false; break;
		case '9': isEscape = false; isDelimiter = false; break;
		case '-': isEscape = false; isDelimiter = false; if (HighlightLength_l > 0) isAddressDelimiter = true; isHexNumber = false; break;
		case '+': isEscape = false; isDelimiter = false; isAddress = false; isHexNumber = false; break;
		case '.': isEscape = false; isDelimiter = false; isAddress = false; isHexNumber = false; break;
		case 'E': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'e': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'X': isEscape = false; isDelimiter = false; isAddress = false; isHexZero ? isHexNumber = true : 0; break;
		case 'x': isEscape = false; isDelimiter = false; isAddress = false; isHexZero ? isHexNumber = true : 0; break;
		case 'A': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'a': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'B': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'b': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'C': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'c': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'D': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'd': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'F': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		case 'f': isEscape = false; isDelimiter = false; isAddress = false; isHexDigit = true; break;
		default: isEscape = false; isNumber = false; isAddress = false; break;
		}
		// reset FirstDigit flag
		isFirstDigit = false;

		// finish when item end has been reached
		if (isNextDelimiter || isLineEnd) {
			break;
		}
		// step forward
		ActiveStart_l++;
		ActiveLength_l--;
		HighlightLength_l++;

		// adjust for option/comment
		if ((isOption || isComment) && false == isProcessed) {
			isProcessed = true;
			HighlightStart_l++;
			HighlightLength_l--;
		}
	}

	// check for misplaced query operator
	if (isQuery && 1 == HighlightLength_l && false == isNextDelimiter) {
		_TypeFTL = TYPEFTL_QUERY;
		_LineFTL.setActive(ActiveStart_l, ActiveLength_l);
		_LineFTL.setHighlight(HighlightStart_l, HighlightLength_l);
		return false;		// stand-alone query operator is not allowed
	}

	// determine appropriate data type
	if (isIdentifier && 1 == HighlightLength_l) {
		// the string contains a single '@' sign which indicates the start of a synchronous 
		// write operation with a fixed parent collection
		_TypeFTL = TYPEFTL_FIXED_PARENTS;
	}
	else
	// determine appropriate data type
	if (isIdentifier) {
		// the string contains a non-escaped '@' sign which makes it a FTLight identifier
		_TypeFTL = TYPEFTL_IDENTIFIER;
	}
	else
	if (isChecksum && isLineEnd) {
		// a binary field at the end of line preceded by an "=" sign represents the line's checksum
		_TypeFTL = TYPEFTL_CHECKSUM;
	}
	else
	if (isQuery && 1 == HighlightLength_l) {
		// a single backtick (no other characters) represents the query operator
		_TypeFTL = TYPEFTL_QUERY;
	}
	else
	if (isOption) {
		// two colons in the beginning make this item an option
		_TypeFTL = TYPEFTL_OPTION;
	}
	else
	if (isComment) {
		// three colons in the begining make this item a comment
		_TypeFTL = TYPEFTL_COMMENT;
	}
	else
	if (false == isDetermined && isText && false == isNumber) {
		// characters other than (0123456789+-.eExXaAbBcCdDfF) at start of line 
		// without a delimiter sugggest this item to be text or binary (and not a number)
		_TypeFTL = TYPEFTL_TXL_BINFTL;

		// evaluate parent node for binary type



	}
	else
	if (isText && (false == isNumber || (isNumber && isHexDigit && (false == isHexNumber)) || 0 == HighlightLength_l)) {
		// characters other than (0123456789+-.eExXaAbBcCdDfF) after a 
		// text delimiter (,:) make this item a text (and not a number)
		_TypeFTL = TYPEFTL_TXL;
	}
	else
	// 2015-04-25/EKantz: a single number will no longer be a Local, however
	//                    a Local has to have a delimiter, e.g. '0-' or 0-0'
	if (false == isDetermined && isAddress && isAddressDelimiter) {
		// an address may contain only numbers (0123456789) and it has to have
		// address delimiter(s) if the length is bigger than 1, however, at the
		// start of a line without a delimiter it could also be binary.
		_TypeFTL = TYPEFTL_Local_BINFTL;
	}
	else
	if (isAddress && isAddressDelimiter) {
		// an address may contain only numbers (0123456789) and it has to have
		// address delimiter(s) if the length is bigger than 1
		_TypeFTL = TYPEFTL_Local;
	}
	else
	if (false == isDetermined && isNumber) {
		// the set of characters (0123456789+-.eExXaAbBcCdDfF) after a 
		// text delimiter (,:) suggest that this item could be a number, however, 
		// at the start of a line without a delimiter it could also be binary.
		_TypeFTL = TYPEFTL_NUMBER_BINFTL;
	}
	else
	if (isNumber) {
		// the set of characters (0123456789+-.eExXaAbBcCdDfF) after a 
		// text delimiter (,:) suggest that this item could be a number
		_TypeFTL = TYPEFTL_NUMBER;
	}
	else {
		// after sorting out all other data types this item is TYPEFTL_BINFTL
		_TypeFTL = TYPEFTL_BINFTL;
	}

	// check for special binary types
	if ((TYPEFTL_BINFTL == _TypeFTL) || (TYPEFTL_TXL_BINFTL == _TypeFTL)) {
		FTLight_DATA_TYPE_IDENTIFIER FTLightType;
		getFTLightDataType(_LineFTL, FTLightType);

		if (DTI_FTLight_OPEN == FTLightType) {
			_TypeFTL = TYPEFTL_FTLightOPEN;
		}
		else
		if (DTI_FTLight_WRAP == FTLightType) {
			_TypeFTL = TYPEFTL_FTLightWRAP;
		}
		else
		if (DTI_MCL == FTLightType) {
			_TypeFTL = TYPEFTL_DTI_MCL;
		}
		else
		if (DTI_FTL == FTLightType) {
			_TypeFTL = TYPEFTL_BINFTLBINARY;
		}
		else
		if (DTI_TXL == FTLightType) {
			_TypeFTL = TYPEFTL_BINFTLSTRING;
		}
		else
		if (DTI_UNIT == FTLightType) {
			_TypeFTL = TYPEFTL_BINFTLVALUE;
		}
		else
		if (DTI_TIME == FTLightType) {
			_TypeFTL = TYPEFTL_BINFTLTIME;
		}
		else
		if (DTI_TOKEN == FTLightType) {
			_TypeFTL = TYPEFTL_CMFTLTOKEN;
		}
		else
		if (DTI_LINK == FTLightType) {
			_TypeFTL = TYPEFTL_CMFTLLINK;
		}
		else
		if (TYPEFTL_BINDIF == FTLightType) {
			_TypeFTL = TYPEFTL_BINDIF;
		}
	}

	// return descriptor values
	_LineFTL.setActive(ActiveStart_l, ActiveLength_l);
	_LineFTL.setHighlight(HighlightStart_l, HighlightLength_l);

	// generate ItemFTL
	ItemFTL.assignSubString(HighlightStart_l + HighlightLength_l - 1, &_LineFTL, HighlightStart_l);

	return true;
}

bool CmStringFTL::getChecksum(CmString& _LineFTL, CmString& _Checksum)
{
	// get line and highlight information
	int32 LineNumber_l;
	uint32 LineStart_l;
	uint32 HighlightStart_l;
	uint32 HighlightLength_l;
	if (false == _LineFTL.getLineHighlight(LineNumber_l, LineStart_l, HighlightStart_l, HighlightLength_l)) return false;

	// determine range (dependent on checksum length)
	int64 Range;
	switch (HighlightLength_l) {
	case 1: Range = CHECKSUM_DIGIT_1; break;
	case 2: Range = CHECKSUM_DIGIT_2; break;
	case 3: Range = CHECKSUM_DIGIT_3; break;
	case 4: Range = CHECKSUM_DIGIT_4; break;
	case 5: Range = CHECKSUM_DIGIT_5; break;
	case 6: Range = CHECKSUM_DIGIT_6; break;
	case 7: Range = CHECKSUM_DIGIT_7; break;
	default: Range = CHECKSUM_DIGIT_1; break;
	}

	// calculate checksum without line number
	uint64 Checksum = 0;
	for (uint32 n = 0; n < HighlightStart_l - LineStart_l; n++) {
		Checksum *= 256;
		Checksum += _LineFTL[LineStart_l + n];
		Checksum %= Range;
	}

	// include line number
	CmString Num;
	Num += 10;
	for (uint32 n = 0; n < Num.getLength(); n++) {
		Checksum *= 256;
		Checksum += Num[n];
		Checksum %= Range;
	}

	// convert checksum to FTL format
	num2FTL(&_Checksum, Checksum);

	return true;
}

bool CmStringFTL::verifyChecksum(CmString& _LineFTL, CmString& _Checksum)
{
	// calculate checksum
	CmString Checksum;
	if (false == getChecksum(_LineFTL, Checksum)) return false;

	// verify received checksum against calculated checksum
	if (Checksum != _Checksum) return false;

	return true;
}

bool CmStringFTL::getPosition(const CmString& _Address)
{
	// adjust PosFTL to Root position's parent
	if (NULL != FTL()->Root && NULL != FTL()->Root->Parent) {
		FTL()->PosFTL = FTL()->Root->Parent;
	}

	// find/create childs according to address string's components
	int32 ChildPosition = 0;
	for (uint32 n = 0; n < _Address.getLength(); n++)
	{
		if ('-' == _Address[n]) {
			// find/create a child at ChildPosition
			if (false == getChild(ChildPosition)) return false;
			ChildPosition = 0;
			continue;
		}
		ChildPosition *= 10;
		ChildPosition += _Address[n] - '0';
	}

	// find/create a child according to last address component
	if (false == getChild(ChildPosition)) return false;

	return true;
}

bool CmStringFTL::findPosition(const CmString& _Address, CmString _StartAddr)
{
	// adjust PosFTL to Root position's parent
	if (NULL != FTL()->Root && NULL != FTL()->Root->Parent) {
		FTL()->PosFTL = FTL()->Root->Parent;
	}

	// find childs according to address string's components
	int32 ChildPosition = 0;
	int32 PositionStartAddr = -1;
	for (uint32 n = 0; n < _Address.getLength(); n++)
	{
		if ('-' == _Address[n]) {
			// replace with matching address component if it exists
			int32 StartPosition = 0;
			while (++PositionStartAddr < (int32)_StartAddr.getLength()) {
				if ((_StartAddr[PositionStartAddr] < '0') || (_StartAddr[PositionStartAddr] > '9')) break;
				StartPosition *= 10;
				StartPosition += _StartAddr[PositionStartAddr] - '0';
				ChildPosition = StartPosition;
			}
			// find a child at ChildPosition
			if (false == isChild(ChildPosition)) return false;
			ChildPosition = 0;
			continue;
		}
		ChildPosition *= 10;
		ChildPosition += _Address[n] - '0';
	}

	// find/create a child according to last address component
	if (false == isChild(ChildPosition)) return false;

	return true;
}

bool CmStringFTL::getLocal(const CmString& Address, CmStringFTL*& LocalFTL)
{
	CmStringFTL* PosFTL1 = FTL()->PosFTL;

	// check if Local item exists
	if (true == findPosition(Address)) {
		// preserve Local information
		LocalFTL = FTL()->PosFTL;
	}
	else {
		// Local item does not exist
		LocalFTL = NULL;
		TypeFTL = TYPEFTL_Local_BINFTL;
	}

	// Local: recover position
	FTL()->PosFTL = PosFTL1;

	return true;
}

bool CmStringFTL::initLineFTL(CmString& _LineFTL, uint32& _LineStart, uint32& _ActiveStart, uint32& _ActiveLength, uint32& _HighlightStart, uint32& _HighlightLength)
{
	// 'highlight' the 'active' range to indicate start of a new line
	_LineFTL.getActive(_ActiveStart, _ActiveLength, LineNumber);
	_LineStart = _ActiveStart;
	_HighlightStart = _ActiveStart;
	_HighlightLength = _ActiveLength;
	_LineFTL.setHighlight(_HighlightStart, _HighlightLength);

	// PosFTL will be adjusted step-by-step to root position
	FTL()->PosFTL = FTL()->Root;
	if (NULL != FTL()->Root && NULL != FTL()->Root->Parent) {
		FTL()->PosFTL = FTL()->Root->Parent;
	}

	FTL()->isPath = true;
	FTL()->isOption = false;
	FTL()->isFirstItem = true;
	FTL()->isRepeatedPath = true;
	FTL()->isStartOfCollection = false;
	FTL()->isWaitForBufferStart = false;
	FTL()->isWaitForBufferLength = false;
	FTL()->isSingleAddressBeforeAt = false;
	FTL()->isRestoreSynchronousWriting = false;

	return true;
}

bool CmStringFTL::checkSyncronousWriting(CmTypeFTL _TypeFTL)
{
	// check for valid navigation
	if ((NULL == FTL()->PosFTL) || (NULL == FTL()->PathFTL)) return false;

	if (TYPEFTL_IDENTIFIER == _TypeFTL) {
		// switch synchronous writing off
		FTL()->sync.isSynchronousWriting = false;
		FTL()->sync.isFixedParentCollection = false;
	}
	else {
		// check for start of synchronous writing 
		if (false == FTL()->sync.isSynchronousWriting) {
			// initialize parent/level/child positions for synchronous writing
			FTL()->sync.isSynchronousWriting = true;
			FTL()->sync.FirstParentPos = -1;
			FTL()->sync.Levels = 0;
			FTL()->sync.SequenceNumber = 0;
			FTL()->sync.BufferLength = 0;
			FTL()->sync.BufferPos = -1;

			// adjust state
			FTL()->isRepeatedPath = false;
			FTL()->isPath = false;

			// check for synchronous writing on a new vs. existing position
			if ((NULL != FTL()->PosFTL->Parent) && (FTL()->isRestoreSynchronousWriting || (NULL == FTL()->PosFTL->Child))) {
				// existing position: adjust PathFTL to current position's parent 
				FTL()->PathFTL = FTL()->PosFTL->Parent;
				FTL()->sync.FirstParentPos = -1;
				// search for addressed first item (on parent level)
				CmStringFTL* ParentPos = FTL()->PathFTL->Child;
				while (ParentPos != NULL && ParentPos != FTL()->PosFTL && ParentPos->Sibling != ParentPos->Sibling->Parent->Child) {
					FTL()->sync.FirstParentPos++;
					ParentPos = ParentPos->Sibling;
				}
			}
			else {
				// preserve existing items (on parent level) when starting a new synchronous writing 
				FTL()->PosFTL = FTL()->PathFTL;
				if (NULL != FTL()->PosFTL && NULL != FTL()->PosFTL->Child) {
					FTL()->sync.Levels = 1;
				}
			}
			FTL()->sync.ParentPos = FTL()->sync.FirstParentPos;
		}
		else {
			// initialize parent position
			FTL()->sync.ParentPos = FTL()->sync.FirstParentPos;
			// increment level and buffer position
			if (false == FTL()->sync.isFixedParentCollection) {
				FTL()->sync.Levels++;
			}
			else {
				FTL()->sync.SequenceNumber++;
				FTL()->sync.BufferPos++;
				if (FTL()->sync.BufferLength > 0) {
					FTL()->sync.BufferPos %= FTL()->sync.BufferLength;
				}
			}
		}
	}
	FTL()->isFirstItem = false;
	FTL()->isRestoreSynchronousWriting = false;

	return true;
}

bool CmStringFTL::runSynchronousWriting(CmString& _ItemFTL, CmTypeFTL _TypeFTL)
{
	// next parent position
	FTL()->sync.ParentPos++;

	// detect fixed parent collections and buffer length
	if (TYPEFTL_FIXED_PARENTS == _TypeFTL) {
		FTL()->sync.isFixedParentCollection = true;
		FTL()->isWaitForBufferLength = true;
		FTL()->isWaitForBufferStart = false;
		return true;
	}
	else {
		if (FTL()->isWaitForBufferLength) {
			FTL()->sync.BufferLength = _ItemFTL.getNumAsUint64();
			FTL()->isWaitForBufferLength = false;
			FTL()->isWaitForBufferStart = true;
			return true;
		}
		else {
			if (FTL()->isWaitForBufferStart) {
				FTL()->sync.BufferPos = _ItemFTL.getNumAsUint64() - 1;
				FTL()->isWaitForBufferStart = false;
				return true;
			}
		}
	}

	// generate a relative address 
	CmString SyncAddr;
	SyncAddr += FTL()->sync.ParentPos;
	int64 Levels = FTL()->sync.Levels;
	while (Levels-- > 0) {
		SyncAddr += "-0";
	}
	if (FTL()->sync.isFixedParentCollection) {
		SyncAddr += "-";
		SyncAddr += FTL()->sync.BufferPos;
	}

	// adjust PosFTL to addressed position and write _ItemFTL content
	FTL()->PathFTL->FTL()->Root = NULL; // relative start instead of start at root position
	FTL()->PathFTL->FTL()->PosFTL = FTL()->PathFTL; // PathFTL is the relative start position
	if (false == FTL()->PathFTL->getPosition(SyncAddr)) return false;
	FTL()->PathFTL->FTL()->PosFTL->TypeFTL = _TypeFTL;
	FTL()->PathFTL->FTL()->PosFTL->setText(_ItemFTL);
	FTL()->PathFTL->FTL()->PosFTL->DataFormat = DATAFORMAT_FTLight;

	return true;
}

bool CmStringFTL::evaluateRepeatedPath()
{
	if (false == FTL()->isStartOfCollection) {
		if (FTL()->isRepeatedPath) {
			// start at current path
			CmStringFTL* NewPathFTL = FTL()->PathFTL;
			// check if path was exhausted
			if (FTL()->PosFTL == FTL()->PathFTL) {
				// involve child in path
				NULL != NewPathFTL->Child ? NewPathFTL = NewPathFTL->Child : 0;
			}
			// involve one more of previous path items
			while (NULL != NewPathFTL->Parent && NewPathFTL->Parent != FTL()->PosFTL && NewPathFTL != FTL()->PosFTL) {
				NewPathFTL = NewPathFTL->Parent;
			}
			FTL()->PosFTL = NewPathFTL;
			return true;
		}
	}
	else {
		FTL()->isRepeatedPath = false;
	}

	return false;
}

bool CmStringFTL::putChildOnPath(CmString& _ItemFTL, CmTypeFTL& _TypeFTL, CmStringFTL* _LocalFTL)
{
	// recover effective path from PosFTL
	FTL()->PathFTL = FTL()->PosFTL;

	// determine parent position in order to maintain childs or siblings
	if (false == FTL()->isPath && NULL != FTL()->PathFTL->Parent && false == FTL()->sync.isSynchronousWriting && false == FTL()->isStartOfCollection) {
		FTL()->PathFTL = FTL()->PathFTL->Parent;
	}

	// evaluation for option
	if (TYPEFTL_OPTION == _TypeFTL) {
		FTL()->isOption = true;
	}
	else if (FTL()->isStartOfCollection) {
		FTL()->isOption = false;
	}
	if (FTL()->isOption) {
		_TypeFTL = TYPEFTL_OPTION;
	}

	// add next item
	FTL()->PathFTL->addChild(&FTL()->PosFTL);
	if (NULL == FTL()->PosFTL) return false;
	FTL()->PosFTL->TypeFTL = _TypeFTL;
	FTL()->PosFTL->DataFormat = DATAFORMAT_FTLight;
	// set _ItemFTL content respectively a Local (address)
	if (TYPEFTL_Local == _TypeFTL) {
		FTL()->PosFTL->setLength(sizeof(_LocalFTL));
		memcpy(FTL()->PosFTL->getBuffer(), &_LocalFTL, sizeof(_LocalFTL));
	}
	else {
		FTL()->PosFTL->setText(_ItemFTL);
	}
	FTL()->PosFTL->DataFormat = DATAFORMAT_FTLight;

	return true;
}

CmStringFTL::NaviFTL* CmStringFTL::FTL()
{
	// create navigation if it does not exist yet
	if (NULL == Navigation) {
		Navigation = allocateMemory<NaviFTL>(1, isNaviFTL);
		memset(Navigation, 0, sizeof(NaviFTL));
	}

	return Navigation;
}


//----------------------------------------------------------------------------
// CmStringFTLroot class.
//----------------------------------------------------------------------------
//
CmStringFTLroot::CmStringFTLroot()
{
	// make this an empty root node
	createRoot();
}
CmStringFTLroot::~CmStringFTLroot()
{

}
