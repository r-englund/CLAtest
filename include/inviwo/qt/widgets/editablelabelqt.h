#ifndef EDITABLELABELQT_H
#define EDITABLELABELQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMenu>
namespace inviwo {

    class IVW_QTWIDGETS_API EditableLabelQt: public QWidget{
    Q_OBJECT
public:
    EditableLabelQt(std::string text);
    EditableLabelQt(std::string text, QMenu* contextMenu);
    void edit();
    std::string getText(){return text_;};
    void setText(std::string txt);
    void setContextMenu(QMenu* menu){contextMenu_ = menu;};
public slots:
    void editingOff();
    void showContextMenu(const QPoint& pos);

private:
    QLabel* label_;
    QLineEdit* lineEdit_;
    std::string text_;
    void generateWidget();
    QMenu* contextMenu_;
    void mouseDoubleClickEvent( QMouseEvent * event );
    std::string shortenText();

    signals:
        void textChanged();
};
}//namespace

#endif //EDITABLELABELQT_H