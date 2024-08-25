#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <enginesettings.h>
#include <graphicssettings.h>
#include <spectrum.h>
#include <utils.h>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    //ENGINE
    WindowFunction windowFunction() const { return m_windowFunction; }
    int fftSize() const { return m_fftSize; }
    int updateIntervalMs() const { return m_updateIntervalMs; }

    //GRAPHICS
    bool logScale() const { return m_logScale; }
    qreal logFactor() const { return m_logFactor; }

private slots:
    void on_spinBox_UpdateIntervalMs_editingFinished();

    void on_comboBox_FFTSize_currentIndexChanged(int index);

    void on_comboBox_windowFunction_currentIndexChanged(int index);

    void on_spinBox_UpdateIntervalMs_valueChanged(int arg1);

private:
    Ui::SettingsDialog *ui;

    //ENGINE
    WindowFunction m_windowFunction = EngineSettings::instance().windowFunction();
    int m_fftSize = EngineSettings::instance().fftSize();
    int m_updateIntervalMs = EngineSettings::instance().updateIntervalMs();

    //GRAPHICS
    bool m_logScale = GraphicsSettings::instance().logScale();
    qreal m_logFactor = GraphicsSettings::instance().logFactor();
};

#endif // SETTINGSDIALOG_H
