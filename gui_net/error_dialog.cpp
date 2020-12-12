#include "error_dialog.h"

//error_dialog::error_dialog(QDialog *parent) : QDialog(parent)
//{

//}

error_dialog::error_dialog()
{
    this->setModal(true);
    this->resize(300,200);
    this->setWindowTitle("通讯出错");
    this->show();
}
