/*
 * HtmlRemoval.cpp
 *
 *  Created on: 12/mar/2013
 *      Author: nemo
 */

#include "HtmlRemoval.h"

namespace cesare {

HtmlRemoval::HtmlRemoval() {}

HtmlRemoval::~HtmlRemoval() {}

void HtmlRemoval::skipTagOnly(FILE *f,std::string tag){
    char c;
    while(!feof(f)){
        fscanf(f,"%c",&c);
        if(c=='>')
            return;
    }
}

void HtmlRemoval::elaborate(std::string fileIn) {
    FILE *f,*out;
    f = fopen(fileIn.c_str(),"r+");
    //std::string tagToSkip[] = { "html","b","i","p","div","span","img","br","table","tr","td","a","!DOCTYPE","body","ul","li","\0"};
    std::string tagToSkip[] = {"img","br","table","tr","td","a","!DOCTYPE","body","span","center","div","ul","li","\0"};
    std::string tagAndContentToSkip[] = {"script","head","form","style","iframe","canvas","cufon","\0"};
    if(f==NULL) {
    	//qDebug() << "failed elaborating file...";
    	return;
    }
    std::string buf;
    char c;
    while(!feof(f)) {
        fscanf(f,"%c",&c);
        if(c=='<') { //start tag found.
            std::string tag = readTagName(f);
            int i=0;
            bool found = false;
            while(tagAndContentToSkip[i].compare("\0")) {
                if(tag.compare(tagAndContentToSkip[i]) == 0) {
                    skipTagAndContent(f, tag);
                    found = true;
                    break;
                }
                i++;
            }
            if(tag.compare("head") == 0) { //Inject code!
            	buf+="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\"></meta></head>\n";
            }
            i=0;
            if(!found){
                while(tagToSkip[i].compare("\0")){
                    if(openOrCloseTag(tag, tagToSkip[i])){
                        skipTagOnly(f, tag);
                        found = true;
                        break;
                    }
                    i++;
                }
            }

            if(found) {
                if(tag.compare("div")==0)
                    buf += "\n";
                if(tag.compare("tr")==0)
                    buf += "<br>";
                if(tag.compare("table")==0)
                    buf += "<br>";
                continue;
            }
            fscanf(f,"%c",&c);

        }
        if(c!='\n')
            buf += c;
    }
    fclose(f);
    out = fopen(fileIn.c_str(),"w+");
    fprintf(out,"%s",buf.c_str());
    fclose(out);
}

void HtmlRemoval::skipTagAndContent(FILE *f,std::string tag) {
    char c;
    std::string endTag = "</" + tag+">";
    std::string buf;

    while(!feof(f)){
        fscanf(f,"%c",&c);
        buf+=c;
        if(buf.find(endTag)!= std::string::npos) { //trovato!
            return;
        }
    }
}

std::string HtmlRemoval::readTagName(FILE *f) {
    char c;
    std::string tag;
    long count = 1; // il minore di inizio tag è giaà considerato
    while (!feof(f)) {
        fscanf(f,"%c",&c);
        count ++;
        if(c!=' ' && c!='>') {
            tag += c;
        } else {
            break;
        }
    }

    fseek(f,-count,SEEK_CUR); //torno indietro.
    return tag;
}

int HtmlRemoval::openOrCloseTag(std::string tagRead,std::string toCompare) {
    if(tagRead.compare(toCompare) == 0 || tagRead.compare("/"+toCompare) == 0)
        return 1;
    return 0;
}

} /* namespace cesare */
