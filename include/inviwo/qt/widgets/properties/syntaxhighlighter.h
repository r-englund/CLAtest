/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Rickard Englund
 *
 **********************************************************************/

#ifndef IVW_SYNTAXHIGHLIGTHER_H
#define IVW_SYNTAXHIGHLIGTHER_H


#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

//QT includes
#include <QSyntaxHighlighter>


class QTextDocument;

namespace inviwo{

	enum SyntaxType{
		None = 0,
		GLSL = 1,
        Python = 2,
	};


class IVW_QTWIDGETS_API SyntaxFormater{
public:
    struct Result{
        std::vector<unsigned int> start;
        std::vector<unsigned int> length;
        QTextCharFormat* format;
        int outgoingState;

        Result():format(0),outgoingState(-1){}
    };

    virtual Result eval(const QString &text,const int &previousBlockState) = 0;
    SyntaxFormater(){}
    virtual ~SyntaxFormater(){}
};

class IVW_QTWIDGETS_API SyntaxHighligther : public QSyntaxHighlighter{
	Q_OBJECT
public:	
    template<SyntaxType T> void setSyntax();
    template<SyntaxType T> static SyntaxHighligther* createSyntaxHighligther(QTextDocument* parent);
    virtual ~SyntaxHighligther();
protected:
    void clearFormaters();
    SyntaxHighligther(QTextDocument* parent);
	void highlightBlock(const QString& text);

private:
    template<SyntaxType T> void loadConfig();
	QTextCharFormat defaultFormat_;
    std::vector<SyntaxFormater*> formaters_;
};	

template<SyntaxType T> void SyntaxHighligther::setSyntax(){
    clearFormaters();
    
    loadConfig<T>();
}


template<SyntaxType T> SyntaxHighligther* SyntaxHighligther::createSyntaxHighligther(QTextDocument* parent){
    SyntaxHighligther* s = new SyntaxHighligther(parent);
    s->loadConfig<T>();
    return s;
}

template<> IVW_QTWIDGETS_API void SyntaxHighligther::loadConfig<None>();
template<> IVW_QTWIDGETS_API void SyntaxHighligther::loadConfig<GLSL>();
template<> IVW_QTWIDGETS_API void SyntaxHighligther::loadConfig<Python>();
	


}//namespace

#endif //IVW_SYNTAXHIGHLIGTHER_H