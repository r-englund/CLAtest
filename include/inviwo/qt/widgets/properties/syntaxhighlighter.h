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

	enum Syntax{
		None = 0,
		GLSL = 1
	};

class IVW_QTWIDGETS_API SyntaxHighligther : public QSyntaxHighlighter{
	Q_OBJECT
public:	
	SyntaxHighligther(QTextDocument* parent,Syntax syntax);
	void setSyntax(Syntax syntax);
protected:
	void highlightBlock(const QString& text);

private:
	void loadConfig();
	void loadGLSLConfig();
	Syntax syntax_;	

	QTextCharFormat* formats_;

};	


	


}//namespace

#endif //IVW_SYNTAXHIGHLIGTHER_H