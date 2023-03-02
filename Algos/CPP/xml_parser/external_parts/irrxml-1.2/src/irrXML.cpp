// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine" and the "irrXML" project.
// For conditions of distribution and use, see copyright notice in irrlicht.h and/or irrXML.h

#include "irrXML.h"
#include "irrString.h"
#include "irrArray.h"
#include "fast_atof.h"
#include "CXMLReaderImpl.h"
#include "defs.hpp"
#include "io.hpp"

namespace irr
{
namespace io
{

//! Implementation of the file read callback for ordinary files
class CFileReadCallBack : public IFileReadCallBack
{
public:

	//! construct from filename
	CFileReadCallBack(const char* filename)
	{
	    m_pointer=0;
		m_data.clear();
		io_load_data_from_file(m_data,std::string(filename));
	}

	//! destructor
	virtual ~CFileReadCallBack()
	{
	    m_data.clear();
	}

	//! Reads an amount of bytes from the file.
	virtual int read(void* buffer, int sizeToRead)
	{
	    int data_to_copy=0;
	    if ((m_pointer+sizeToRead)<=this->getSize()){data_to_copy=sizeToRead;}else{data_to_copy=(this->getSize()-m_pointer);}
        _MEMCPY_(buffer,&m_data[m_pointer],data_to_copy);
        m_pointer+=data_to_copy;
		return data_to_copy;
	}

	//! Returns size of file in bytes
	virtual int getSize(){return m_data.size();}
private:
    int m_pointer;
    std::vector<U8> m_data;

}; // end class CFileReadCallBack

// FACTORY FUNCTIONS:

//! Creates an instance of an UFT-8 or ASCII character xml parser.
IrrXMLReader* createIrrXMLReader(const char* filename)
{
	return new CXMLReaderImpl<char, IXMLBase>(new CFileReadCallBack(filename));
}

//! Creates an instance of an UFT-8 or ASCII character xml parser.
IrrXMLReader* createIrrXMLReader(IFileReadCallBack* callback)
{
	return new CXMLReaderImpl<char, IXMLBase>(callback, false);
}

//! Creates an instance of an UTF-16 xml parser.
IrrXMLReaderUTF16* createIrrXMLReaderUTF16(const char* filename)
{
	return new CXMLReaderImpl<char16, IXMLBase>(new CFileReadCallBack(filename));
}

//! Creates an instance of an UTF-16 xml parser.
IrrXMLReaderUTF16* createIrrXMLReaderUTF16(IFileReadCallBack* callback)
{
	return new CXMLReaderImpl<char16, IXMLBase>(callback, false);
}

//! Creates an instance of an UTF-32 xml parser.
IrrXMLReaderUTF32* createIrrXMLReaderUTF32(const char* filename)
{
	return new CXMLReaderImpl<char32, IXMLBase>(new CFileReadCallBack(filename));
}

//! Creates an instance of an UTF-32 xml parser.
IrrXMLReaderUTF32* createIrrXMLReaderUTF32(IFileReadCallBack* callback)
{
	return new CXMLReaderImpl<char32, IXMLBase>(callback, false);
}

} // end namespace io
} // end namespace irr
