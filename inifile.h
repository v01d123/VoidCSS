/*******************************************************************************************************************************
	Programmer: Ludvik Jerabek
	Date: June 15th, 2009

    Defined Classed: CIniFileW CIniFileA

	Purpose: C++ Inifile Reader\Writer. Uses std::map to implement an efficient ini object.

	Summary: This is a total re-write of the original CIniFile class written in 2006. Originally the data structures
             underlying the CIniFile object were std::list which was extreamly inefficient when dealing with huge ini files.


		   Note: The class currently supports std::wstring and std::string. The typedef CIniFile is based on the whether of no
                 _UNICODE is defined. If _UNICODE is define in your project CIniFile is a CIniFileW if _UNICODE is not defined
                 then CIniFile is a CIniFileA object.

		   Defines:

		           _TRACE_CINIFILE - If defined enables call tracing to standard output
		                  _UNICODE - If defined the CIniFile will be defined as CIniFileW instead of CIniFileA
	          _FORCE_UNIX_LINEFEED - If defined when _WIN32 is defined (WINDOWS) the default linefeed CRLF is overridden to CR
		   _FORCE_WINDOWS_LINEFEED - If defined when _WIN32 is not defined (*NIX) the default linefeed CR is overridden to CRLF

		   Updates:

            12\01\2005 - Initial MFC Release.
            01\12\2006 - Ported to Ansi C++ Non-MFC.
            06\16\2009 - Added support for different linefeed types, resolved issues around reading different types of linefeeds.
            06\17\2009 - Added support for wide characters.
            06\21\2009 - Re-written to use std::map.
            07\02\2009 - Removed MFC version. Since Ansi version works in MFC ( Examples provided for download ).
            07\03\2009 - Added support for VS6.
            07\03\2009 - Fixed issue with SecMapA \ SecMapW. Were not named specific to the encoding may have caused issues.
            07\03\2009 - Fixed GetKeys and GetSections functions to return const ref v.s. copy of data.
            07\14\2009 - Fixed Load() whitespace preservation on key value.
            09\21\2009 - Fixed removing all the sections and keys, replaced empty() with clear()
            09\22\2009 - Added overloaded Load() and Save() to read\write streams
            09\23\2009 - Added operators for << and >> to be used with streams
            09\24\2009 - Added merge option to Load()
            09\25\2009 - Added CIniMerge for use with << and >>
            09\27\2009 - Moved CIniMerge into CIniFile, fixed issue with VC6 CIniFile::CR

*******************************************************************************************************************************/
#ifndef __CINIFILE_H_
#define __CINIFILE_H_

#ifdef _WIN32
	// VC6 "identifier was truncated to '255' characters in the debug information"
	#if defined(_MSC_VER) && (_MSC_VER >= 1200) && (_MSC_VER < 1300)
		#pragma warning(disable: 4786)
	#endif
	// Prevent compile time warnings for deprecation
	#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <map>
#include <string>
#include <string.h>
#include <wchar.h>

#define INI_TOKEN_A_ANSI "\a"	// I.E. Item1;Item2;Item3 - '\a' used in place of ';'
#define INI_TOKEN_B_ANSI "\b"	// I.E. Item1,Item1b;Item2,Item2b;Item3,Item3b - '\b' used in place of ','
#define INI_EMPTY_ANSI "*"		// Used to indicate empty value in token string. I.E. *;Item2;*;Item3;

struct ci_less_a
{
  bool operator() (const std::string & s1, const std::string & s2) const
  {
      #ifndef _WIN32
            return strcasecmp(s1.c_str(), s2.c_str()) < 0;
      #else
            return _stricmp(s1.c_str(), s2.c_str()) < 0;
      #endif
  }
};

class CIniFileA
{
    public:
        static const char* const LF;
	public:
		CIniFileA();
		~CIniFileA();

        // Used to save the data back to the file or your choice
		bool Save( const std::string& fileName );

		// Save data to an output stream
		void Save( std::ostream& output );

		// Loads the Reads the data in the ini file into the IniFile object
		bool Load( const std::string& fileName , bool bMerge = false );

		// Load data from an input stream
		void Load( std::istream& input , bool bMerge = false );

    public:
            class CIniMergeA
            {
                public:
                    explicit CIniMergeA(CIniFileA& ini):_ini(ini){}
                    std::istream &operator()(std::istream& input) const
                    {
                        _ini.Load( input , true );
                        return input;
                    }
                private:
                CIniFileA& _ini;
            };
	public:
            class CIniSectionA
			{
				friend class CIniFileA; // Allow CIniFileA to create sections
				public:
					class CIniKeyA
					{
						friend class CIniSectionA; // Allow CIniSectionA to create keys
						private: // CIniFileA acts as a class factory for CIniSectionA Objects
							CIniKeyA( CIniSectionA* pSection , std::string sKeyName );
							CIniKeyA( const CIniKeyA& ); // No Copy
							CIniKeyA& operator=(const CIniKeyA&); // No Copy
							~CIniKeyA( );
						public:
							// Sets the value of the key
							void SetValue( std::string sValue );
							// Returns the value of the Key
							std::string GetValue() const;
							// Sets the key name, returns true on success, fails if the section
							// name sKeyName already exists
							bool SetKeyName( std::string sKeyName );
							// Returns the name of the Key
							std::string GetKeyName() const;
						private:
							// Name of the Key
							std::string m_sKeyName;
							// Value associated
							std::string m_sValue;
							// Pointer to the parent CIniSectionA
							CIniSectionA* m_pSection;
					}; // End of CIniKeyA
					typedef std::map<std::string,CIniKeyA*,ci_less_a> KeyMapA;
					#ifdef _WIN32
                        // Added for VC6 Support
                        #if defined(_MSC_VER) && (_MSC_VER >= 1200) && (_MSC_VER < 1300)
                            friend class CIniKeyA;
                        #endif
                    #endif
				private: // CIniSectionA acts as a class factory for CIniKeyA Objects
					CIniSectionA( CIniFileA* pIniFile , std::string sSectionName );
					CIniSectionA( const CIniSectionA& ); // No Copy
					CIniSectionA& operator=(const CIniSectionA&); // No Copy
					~CIniSectionA( );
				public:
					// Adds a key to the CIniSectionA object, returns a CIniKeyA pointer to the new or existing object
					CIniKeyA* AddKey( std::string sKeyName );
					// Removes a single key by pointer
					void RemoveKey( CIniKeyA* pKey );
					// Removes a single key by string
					void RemoveKey( std::string sKey );
                    // Removes all the keys in the section
					void RemoveAllKeys( );
					// Returns a CIniKeyA pointer to the key by name, NULL if it was not found
					CIniKeyA* GetKey( std::string sKeyName ) const;
					// Returns all keys in the section by KeyList ref
					const KeyMapA& GetKeys() const;
					// Returns a KeyValue at a certain section
					std::string GetKeyValue( std::string sKey ) const;
					// Sets a KeyValuePair at a certain section
					void SetKeyValue( std::string sKey, std::string sValue );
					// Sets the section name, returns true on success, fails if the section
					// name sSectionName already exists
					bool SetSectionName( std::string sSectionName );
					// Returns the section name
					std::string GetSectionName() const;

				private:
					// CIniFileA pointer back to the object that instanciated the section
					CIniFileA* m_pIniFile;
					// Name of the section
					std::string m_sSectionName;
					// List of CIniKeyA pointers ( Keys in the section )
					KeyMapA m_keys;
			}; // End of CIniSectionA
		// Typedef of a List of CIniSectionA pointers
		typedef std::map<std::string,CIniSectionA*,ci_less_a> SecMapA;
        #ifdef _WIN32
            // Added for VC6 Support
            #if defined(_MSC_VER) && (_MSC_VER >= 1200) && (_MSC_VER < 1300)
                friend class CIniSectionA;
            #endif
        #endif
	public:
		// Adds a section to the CIniFileA object, returns a CIniFileA pointer to the new or existing object
		CIniSectionA* AddSection( std::string sSection );
		// Removes section by pointer
		void RemoveSection( CIniSectionA* pSection );
		// Removes a section by its name sSection
		void RemoveSection( std::string sSection );
		// Removes all existing sections
		void RemoveAllSections( );
		// Returns a CIniSectionA* to the section by name, NULL if it was not found
		CIniSectionA* GetSection( std::string sSection ) const;
		// Gets all the section in the ini file
		const SecMapA& GetSections() const;
		// Returns a KeyValue at a certain section
		std::string GetKeyValue( std::string sSection, std::string sKey ) const;
		// Sets a KeyValuePair at a certain section
		void SetKeyValue( std::string sSection, std::string sKey, std::string sValue );
		// Renames an existing section returns true on success, false if the section didn't exist or there was another section with the same sNewSectionName
		bool RenameSection( std::string sSectionName  , std::string sNewSectionName );
		// Renames an existing key returns true on success, false if the key didn't exist or there was another section with the same sNewSectionName
		bool RenameKey( std::string sSectionName  , std::string sKeyName , std::string sNewKeyName);
	private:
		CIniFileA( const CIniFileA&); // No Copy
		CIniFileA& operator=(const CIniFileA&); // No Copy
		// List of CIniSectionA pointers ( List of sections in the class )
		SecMapA m_sections;
}; // End of CIniFileA

// Basic typedefs for ease of use
typedef CIniFileA::CIniMergeA CIniMergeA;
typedef CIniFileA::CIniSectionA CIniSectionA;
typedef CIniSectionA::CIniKeyA CIniKeyA;

// Pointers
typedef CIniFileA* PCINIA;
typedef CIniKeyA* PCINIKEYA;
typedef CIniSectionA* PCINISECA;

// Map Types
typedef CIniSectionA::KeyMapA KeyMapA;
typedef CIniFileA::SecMapA SecMapA;


std::ostream& operator<<(std::ostream& output, CIniFileA& obj);
std::istream& operator>>(std::istream& input, CIniFileA& obj);
std::istream& operator>>(std::istream& input, CIniMergeA merger);

// Unicode Class Definition

#define INI_TOKEN_A_UNICODE L"\a"	// I.E. Item1;Item2;Item3 - '\a' used in place of ';'
#define INI_TOKEN_B_UNICODE L"\b"	    // I.E. Item1,Item1b;Item2,Item2b;Item3,Item3b - '\b' used in place of ','
#define INI_EMPTY_UNICODE L"*"		// Used to indicate empty value in token string. I.E. *;Item2;*;Item3;

struct ci_less_w
{
  bool operator() (const std::wstring & s1, const std::wstring & s2) const
  {
      #ifndef _WIN32
            return wcscasecmp(s1.c_str(), s2.c_str()) < 0;
      #else
            return _wcsicmp(s1.c_str(), s2.c_str()) < 0;
      #endif
  }
};


class CIniFileW
{
    public:
        static const wchar_t* const LF;
	public:
		CIniFileW();
		~CIniFileW();

        // Used to save the data back to the file or your choice
		bool Save( const std::wstring& fileName );

        // Save data to an out stream
		void Save( std::wostream& output );

		// Loads the Reads the data in the ini file into the IniFile object
		bool Load( const std::wstring& fileName , bool bMerge = false );

		// Load data from an input stream
		void Load( std::wistream& input , bool bMerge = false );
    public:
            class CIniMergeW
            {
                public:
                    explicit CIniMergeW(CIniFileW& ini):_ini(ini){}
                    std::wistream& operator()(std::wistream& input) const
                    {
                    _ini.Load( input , true );
                    return input;
                    }
                private:
                    CIniFileW& _ini;
            };
	public:
			class CIniSectionW
			{
				friend class CIniFileW; // Allow CIniFileW to create sections
				public:
					class CIniKeyW
					{
						friend class CIniSectionW; // Allow CIniSectionW to create keys
						private: // CIniFileW acts as a class factory for CIniSectionW Objects
							CIniKeyW( CIniSectionW* pSection , std::wstring sKeyName );
							CIniKeyW( const CIniKeyW& ); // No Copy
							CIniKeyW& operator=(const CIniKeyW&); // No Copy
							~CIniKeyW( );
						public:
							// Sets the value of the key
							void SetValue( std::wstring sValue );
							// Returns the value of the Key
							std::wstring GetValue() const;
							// Sets the key name, returns true on success, fails if the section
							// name sKeyName already exists
							bool SetKeyName( std::wstring sKeyName );
							// Returns the name of the Key
							std::wstring GetKeyName() const;
						private:
							// Name of the Key
							std::wstring m_sKeyName;
							// Value associated
							std::wstring m_sValue;
							// Pointer to the parent CIniSectionW
							CIniSectionW* m_pSection;
					}; // End of CIniKeyW
					typedef std::map<std::wstring,CIniKeyW*,ci_less_w> KeyMapW;
					#ifdef _WIN32
                        // Added for VC6 Support
                        #if defined(_MSC_VER) && (_MSC_VER >= 1200) && (_MSC_VER < 1300)
                            friend class CIniKeyW;
                        #endif
                    #endif
				private: // CIniSectionW acts as a class factory for CIniKeyW Objects
					CIniSectionW( CIniFileW* pIniFile , std::wstring sSectionName );
					CIniSectionW( const CIniSectionW& ); // No Copy
					CIniSectionW& operator=(const CIniSectionW&); // No Copy
					~CIniSectionW( );
				public:
					// Adds a key to the CIniSectionW object, returns a CIniKeyW pointer to the new or existing object
					CIniKeyW* AddKey( std::wstring sKeyName );
					// Removes a single key by pointer
					void RemoveKey( CIniKeyW* pKey );
					// Removes a single key by string
					void RemoveKey( std::wstring sKey );
                    // Removes all the keys in the section
					void RemoveAllKeys( );
					// Returns a CIniKeyW pointer to the key by name, NULL if it was not found
					CIniKeyW* GetKey( std::wstring sKeyName ) const;
					// Returns all keys in the section by KeyList ref
					const KeyMapW& GetKeys() const;
					// Returns a KeyValue at a certain section
					std::wstring GetKeyValue( std::wstring sKey ) const;
					// Sets a KeyValuePair at a certain section
					void SetKeyValue( std::wstring sKey, std::wstring sValue );
					// Sets the section name, returns true on success, fails if the section
					// name sSectionName already exists
					bool SetSectionName( std::wstring sSectionName );
					// Returns the section name
					std::wstring GetSectionName() const;

				private:
					// CIniFileW pointer back to the object that instanciated the section
					CIniFileW* m_pIniFile;
					// Name of the section
					std::wstring m_sSectionName;
					// List of CIniKeyW pointers ( Keys in the section )
					KeyMapW m_keys;
			}; // End of CIniSectionW
		// Typedef of a List of CIniSectionW pointers
		typedef std::map<std::wstring,CIniSectionW*,ci_less_w> SecMapW;
        #ifdef _WIN32
            // Added for VC6 Support
            #if defined(_MSC_VER) && (_MSC_VER >= 1200) && (_MSC_VER < 1300)
                friend class CIniSectionW;
            #endif
        #endif
	public:
		// Adds a section to the CIniFileW object, returns a CIniFileW pointer to the new or existing object
		CIniSectionW* AddSection( std::wstring sSection );
		// Removes section by pointer
		void RemoveSection( CIniSectionW* pSection );
		// Removes a section by its name sSection
		void RemoveSection( std::wstring sSection );
		// Removes all existing sections
		void RemoveAllSections( );
		// Returns a CIniSectionW* to the section by name, NULL if it was not found
		CIniSectionW* GetSection( std::wstring sSection ) const;
		// Gets all the section in the ini file
		const SecMapW& GetSections() const;
		// Returns a KeyValue at a certain section
		std::wstring GetKeyValue( std::wstring sSection, std::wstring sKey ) const;
		// Sets a KeyValuePair at a certain section
		void SetKeyValue( std::wstring sSection, std::wstring sKey, std::wstring sValue );
		// Renames an existing section returns true on success, false if the section didn't exist or there was another section with the same sNewSectionName
		bool RenameSection( std::wstring sSectionName  , std::wstring sNewSectionName );
		// Renames an existing key returns true on success, false if the key didn't exist or there was another section with the same sNewSectionName
		bool RenameKey( std::wstring sSectionName  , std::wstring sKeyName , std::wstring sNewKeyName);
	private:
		CIniFileW( const CIniFileW&); // No Copy
		CIniFileW& operator=(const CIniFileW&); // No Copy
		// List of CIniSectionW pointers ( List of sections in the class )
		SecMapW m_sections;
}; // End of CIniFileW

// Basic typedefs for ease of use
typedef CIniFileW::CIniMergeW CIniMergeW;
typedef CIniFileW::CIniSectionW CIniSectionW;
typedef CIniSectionW::CIniKeyW CIniKeyW;

// Pointers
typedef CIniFileW* PCINIW;
typedef CIniKeyW* PCINIKEYW;
typedef CIniSectionW* PCINISECW;

// Map Types
typedef CIniSectionW::KeyMapW KeyMapW;
typedef CIniFileW::SecMapW SecMapW;

std::wostream& operator<<(std::wostream& output, CIniFileW& obj);
std::wistream& operator>>(std::wistream& input, CIniFileW& obj);
std::wistream& operator>>(std::wistream& input, CIniMergeW merger);

// Additional defines
#ifdef _UNICODE
	#define INI_TOKEN_A INI_TOKEN_UNICODE
	#define INI_TOKEN_B INI_TOKEN_UNICODE
	#define INI_EMPTY INI_EMPTY_UNICODE
	typedef CIniMergeW CIniMerge;
    typedef CIniFileW CIniFile;
    typedef CIniSectionW CIniSection;
    typedef CIniKeyW CIniKey;
    typedef PCINIW PCINI;
    typedef PCINIKEYW PCINIKEY;
    typedef PCINISECW PCINISEC;
    typedef KeyMapW KeyMap;
    typedef SecMapW SecMap;
#else
	#define INI_TOKEN_A INI_TOKEN_ANSI
	#define INI_TOKEN_B INI_TOKEN_ANSI
	#define INI_EMPTY INI_EMPTY_ANSI
	typedef CIniMergeA CIniMerge;
    typedef CIniFileA CIniFile;
    typedef CIniSectionA CIniSection;
    typedef CIniKeyA CIniKey;
    typedef PCINIA PCINI;
    typedef PCINIKEYA PCINIKEY;
    typedef PCINISECA PCINISEC;
    typedef KeyMapA KeyMap;
    typedef SecMapA SecMap;
#endif

#endif

