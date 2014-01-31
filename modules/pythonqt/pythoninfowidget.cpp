#include "pythoninfowidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>

#include <modules/python/pyinviwo.h>

namespace inviwo {
    PythonInfoWidget::PythonInfoWidget(QWidget *parent) 
        :  InviwoDockWidget(tr("Python API Documentation"),parent){
        setObjectName("PythonInfoWidget");
        setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        setFloating(true);
        setVisible(false);

        buildWidget();
        resize(500,700);
    }

    PythonInfoWidget::~PythonInfoWidget(){

    }


    void PythonInfoWidget::buildWidget(){

        QWidget* content = new QWidget(this);
        QVBoxLayout* layout_ = new QVBoxLayout();

        tabWidget_ = new QTabWidget(content);
        layout_->addWidget(tabWidget_);

        std::vector<PyModule*> modules = PyInviwo::getPtr()->getAllPythonModules();
        for (size_t i = 0;i<modules.size();++i){
            onModuleRegistered(modules[i]);
        }

        content->setLayout(layout_);
        setWidget(content);
    }

    void PythonInfoWidget::onModuleRegistered( PyModule* module ) {
        QScrollArea* tab = new QScrollArea(tabWidget_);
        tab->setWidgetResizable(true);
        QWidget* content = new QWidget(tab);

        std::vector<PyMethod*> methods =  module->getPyMethods();
        
        QGridLayout* layout = new QGridLayout();
        layout->setColumnStretch(2,1);

        QLabel* funcLabel = new QLabel("Function");
        QLabel* paramLabel = new QLabel("Parameters");
        QLabel* descLabel = new QLabel("Description");
        QFont font = funcLabel->font();
        font.setPointSize(font.pointSize()+1);
        font.setBold(true);
        funcLabel->setFont(font);
        paramLabel->setFont(font);
        descLabel->setFont(font);

        layout->addWidget(funcLabel,0,0);
        layout->addWidget(paramLabel,0,1);
        layout->addWidget(descLabel,0,2);


        layout->addWidget(new QLabel("<hr />"),1,0,1,3);

        for(int i = 0;i<static_cast<int>(methods.size());++i){
            int row = i*2 + 2;
            QLabel* functionNameLabel = new QLabel(methods[i]->getName().c_str());
            functionNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
            layout->addWidget(functionNameLabel,row,0);

            std::string params = methods[i]->getParamDesc();
            replaceInString(params," , ","<br />");
            layout->addWidget(new QLabel(params.c_str()),row,1);

            QLabel* desc = new QLabel(methods[i]->getDesc().c_str());
            desc->setWordWrap(true);
            layout->addWidget(desc,row,2);

            layout->addWidget(new QLabel("<hr />"),row+1,0,1,3);
        }

        
        content->setLayout(layout);
        tab->setWidget(content);
        tabWidget_->addTab(tab,module->getModuleName());

    }

} // namespace

