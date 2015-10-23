/*
 * HtmlRemoval.h
 *
 *  Created on: 12/mar/2013
 *      Author: nemo
 */

#ifndef HTMLREMOVAL_H_
#define HTMLREMOVAL_H_

#include <string>
#include <stdio.h>
namespace cesare {

class HtmlRemoval {
public:
	HtmlRemoval();
	virtual ~HtmlRemoval();
	void elaborate(std::string fileIn);
private:
    void skipTagOnly(FILE *f,std::string tag);
    void skipTagAndContent(FILE *f,std::string tag);
    int openOrCloseTag(std::string tagRead,std::string toCompare);
    std::string readTagName(FILE *f);
};

} /* namespace cesare */
#endif /* HTMLREMOVAL_H_ */
