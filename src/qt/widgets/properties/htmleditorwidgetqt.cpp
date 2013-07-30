#include <inviwo/qt/widgets/properties/htmleditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/htmllistwidgetqt.h>
#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>

#include <inviwo/core/util/logdistributor.h>

#include <QCommandLinkButton>
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolBar>
#include <QString>

namespace inviwo{

HtmlEditorWidgetQt::HtmlEditorWidgetQt(){
    generateWidget();
    resize(700,500);
}

void HtmlEditorWidgetQt::closeEvent(QCloseEvent *event)
{
    if (mainParentWidget_->saveDialog()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void HtmlEditorWidgetQt::generateWidget() {
    setWindowIcon(QIcon(":/icons/html.png")); 
    setWindowTitle(QString("inviwo-html-editor"));

    setAcceptDrops(true);

    QVBoxLayout* textEditorLayout = new QVBoxLayout();
    textEditorLayout->setSpacing(0);
    textEditorLayout->setMargin(0);
    toolBar_ = new QToolBar();

    runButton_ = new QToolButton();
    runButton_->setIcon(QIcon(":/icons/html.png"));
    runButton_->setToolTip("Generate");
    saveButton_ = new QToolButton();
    saveButton_->setIcon(QIcon(":/icons/save.png")); // Temporary icon
    saveButton_->setToolTip("Save file");    
    reLoadButton_ = new QToolButton();
    reLoadButton_->setIcon(QIcon(":/icons/inviwo_tmp.png")); // Temporary icon
    reLoadButton_->setToolTip("Reload");

    toolBar_->addWidget(runButton_);
    toolBar_->addSeparator();   
    toolBar_->addWidget(saveButton_);
    toolBar_->addSeparator();   
    toolBar_->addWidget(reLoadButton_);
    toolBar_->addSeparator();

    mainWidget_ = new QWidget();
    htmlEditor_ = new QTextEdit();
    htmlEditor_->createStandardContextMenu();
    htmlEditor_->setObjectName("htmlEditor");    

    htmlOutput_ = new QTextEdit();
    htmlOutput_->setObjectName("htmlEditorOutput");
    htmlOutput_->setReadOnly(true);
    htmlOutput_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    htmlOutput_->createStandardContextMenu();

    textEditorLayout->addWidget(toolBar_);

    htmlTreeWidgetQt_ = new HtmlTreeWidget(this);

    QSplitter* horizontal_splitter = new  QSplitter();
    horizontal_splitter->setOrientation(Qt::Horizontal);  
    horizontal_splitter->setOpaqueResize(false);
    horizontal_splitter->addWidget(htmlTreeWidgetQt_);
    horizontal_splitter->addWidget(htmlEditor_);
    horizontal_splitter->setStretchFactor(0, 5);
    horizontal_splitter->setStretchFactor(1, 20);

    QSplitter* vertical_splitter = new  QSplitter();
    vertical_splitter->setOrientation(Qt::Vertical);  
    vertical_splitter->setOpaqueResize(false);
    vertical_splitter->addWidget(horizontal_splitter);
    vertical_splitter->addWidget(htmlOutput_);
    vertical_splitter->setStretchFactor(0, 20);
    vertical_splitter->setStretchFactor(1, 7);
    textEditorLayout->addWidget(vertical_splitter);

    setLayout(textEditorLayout);    

    connect(runButton_,SIGNAL(clicked()),this,SLOT(run()));
}


void HtmlEditorWidgetQt::setParent(TextEditorWidgetQt* tmp){
    mainParentWidget_ = tmp;
}

 void HtmlEditorWidgetQt::run(){     
     QString htmlsource = htmlEditor_->toHtml();
     htmlOutput_->clear();
     htmlOutput_->append(htmlsource);
 }

} // namespace