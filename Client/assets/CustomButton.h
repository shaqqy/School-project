#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>

class CustomButton : public QPushButton {
    Q_OBJECT

public:
    CustomButton(QPixmap* pixmap, QWidget* parent);

protected:

};

#endif // CUSTOMBUTTON_H
