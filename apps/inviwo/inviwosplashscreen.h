#ifndef IVW_INVIWOSPLASHSCREEN_H
#define IVW_INVIWOSPLASHSCREEN_H

#include <QSplashScreen>

namespace inviwo {

class InviwoSplashScreen : public QSplashScreen {
Q_OBJECT;
public:

	InviwoSplashScreen();
    ~InviwoSplashScreen();

	virtual void show();
	virtual void showMessage(std::string message);
	virtual void finish(QWidget* mainWindow);

private:
	bool showSplashScreen_;
};

} // namespace

#endif // IVW_INVIWOSPLASHSCREEN_H
