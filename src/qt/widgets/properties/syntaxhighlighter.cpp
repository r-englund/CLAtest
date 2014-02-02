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

#include <inviwo/qt/widgets/properties/syntaxhighlighter.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/util/urlparser.h>

#include <QTextDocument>
#include <QTextBlock>



namespace inviwo {

SyntaxHighligther::SyntaxHighligther(QTextDocument* parent):QSyntaxHighlighter(parent) {
}

SyntaxHighligther::~SyntaxHighligther() {
    clearFormaters();
}

void SyntaxHighligther::clearFormaters() {
    while (!formaters_.empty()) {
        delete formaters_.back();
        formaters_.pop_back();
    }
}

void SyntaxHighligther::highlightBlock(const QString& text) {
    setFormat(0,text.size(),defaultFormat_);
    std::vector<SyntaxFormater*>::iterator it;

    for (it = formaters_.begin(); it != formaters_.end(); ++it) {
        SyntaxFormater::Result res = (*it)->eval(text,previousBlockState());

        for (size_t i = 0; i<res.start.size(); i++) {
            setFormat(res.start[i],res.length[i],*res.format);
            setCurrentBlockState(res.outgoingState);
        }
    }
}

template<>
void SyntaxHighligther::loadConfig<None>() {
    QColor textColor;
    QColor bgColor;
    textColor.setNamedColor("#aaaaaa");
    bgColor.setNamedColor("#4d4d4d");
    defaultFormat_.setBackground(bgColor);
    defaultFormat_.setForeground(textColor);
}



} // namespace


