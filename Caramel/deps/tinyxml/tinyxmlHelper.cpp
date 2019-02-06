//
//  tinyxmlHelper.cpp
//  AIE
//
//  Created by AIE Melbourne on 1/05/12.
//  
//
#include "tinyxmlHelper.h"

// Helper functions for passing XML.
bool GetAttributeString( TiXmlElement *element, char const * const toFind, char * const toFillIn )
{
	const char * found = element->Attribute( toFind );
	if ( !found )
	{
		return false;
	}
    StrHandler::Copy( toFillIn, 256, found );
    
	return true;
}

bool GetAttributeIntArray2( TiXmlElement *element, char const * const toFind, int * const toFillIn )
{
	const char * found = element->Attribute( toFind );
	if ( !found )
	{
		return false;
	}
#	ifdef _WIN32
	sscanf_s( found, "%d,%d", &toFillIn[0], &toFillIn[1] );
#	else
	sscanf( found, "%d,%d", &toFillIn[0], &toFillIn[1] );
#	endif
    
	return true;
}

bool GetAttributeIntArray4( TiXmlElement *element, char const * const toFind, int * const toFillIn )
{
	const char * found = element->Attribute( toFind );
	if ( !found )
	{
		return false;
	}
#	ifdef _WIN32
	sscanf_s( found, "%d,%d,%d,%d", &toFillIn[0], &toFillIn[1], &toFillIn[2], &toFillIn[3] );
#	else
	sscanf( found, "%d,%d,%d,%d", &toFillIn[0], &toFillIn[1], &toFillIn[2], &toFillIn[3] );
#	endif
    
	return true;
}