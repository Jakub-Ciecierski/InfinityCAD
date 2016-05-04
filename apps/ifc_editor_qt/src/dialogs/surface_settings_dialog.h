#ifndef SURFACE_SETTINGS_DIALOG_H
#define SURFACE_SETTINGS_DIALOG_H

#include <QDialog>

namespace Ui {
class SurfaceSettingsDialog;
}

class SurfaceSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SurfaceSettingsDialog(QWidget *parent = 0);
    ~SurfaceSettingsDialog();

public slots:

    void cancelButtonClicked();
    void okButtonClicked();

private:
    Ui::SurfaceSettingsDialog *ui;

    void initDialog();
};

#endif // SURFACE_SETTINGS_DIALOG_H
