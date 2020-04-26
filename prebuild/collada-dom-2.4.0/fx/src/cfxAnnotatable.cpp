/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#include <cfxAnnotatable.h>
#include <cfxAnnotate.h>


// cfxAnnotatable
cfxAnnotatable::cfxAnnotatable() 
{
}
 
cfxAnnotatable::~cfxAnnotatable() 
{
	for (size_t i=0; i<annotateArray.size(); i++)
	{
		delete  (cfxAnnotate*)(annotateArray[i])->getData();
		delete annotateArray[i];
	}
	annotateArray.clear();
}
 
void cfxAnnotatable::pushAnnotate(cfxAnnotate* annotate)
{
  annotateArray.push_back(annotate);
}

const std::vector<cfxAnnotate*> &cfxAnnotatable::getAnnotateArray() const
{
	return annotateArray;
}
