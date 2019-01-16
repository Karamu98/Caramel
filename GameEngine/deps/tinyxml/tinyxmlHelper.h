//
//  tinyxmlHelper.h
//  
//
//  Created by AIE Melbourne on 1/05/12.
//  
//

#ifndef Revolve_tinyxmlHelper_h
#define Revolve_tinyxmlHelper_h

#include "tinyxml.h"
#include "StrHandler.h"

bool GetAttributeString( TiXmlElement *element, char const * const toFind, char * const toFillIn );

template<typename T>
inline bool GetAttributeInt( TiXmlElement *element, char const * const toFind, T& toFillIn )
{
	const char * found = element->Attribute( toFind );
	if ( !found )
	{
		return false;
	}
	toFillIn = atoi( found );
    
	return true;
}

inline bool GetAttributeFloat( TiXmlElement *element, char const * const toFind, float& toFillIn )
{
	const char * found = element->Attribute( toFind );
	if ( !found )
	{
		return false;
	}
	toFillIn = (float)atof( found );
    
	return true;
}

bool GetAttributeIntArray2( TiXmlElement *element, char const * const toFind, int * const toFillIn );

bool GetAttributeIntArray4( TiXmlElement *element, char const * const toFind, int * const toFillIn );


#endif
