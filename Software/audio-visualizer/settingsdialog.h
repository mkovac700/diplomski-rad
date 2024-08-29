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
    bool isLogScale() const { return m_isLogScale; }
    qreal logFactor() const { return m_logFactor; }

    int numLines() const { return m_numLines; }
    float spacingX() const { return m_spacingX; }
    float spacingZ() const { return m_spacingZ; }

    bool applyWindow() const { return m_applyWindow; }
    bool drawGrid() const { return m_drawGrid; }
    int minFreq() const { return m_minFreq; }
    int maxFreq() const { return m_maxFreq; }
    int bandWidth() const { return m_bandWidth; }
    int gridStepHz() const { return m_gridStepHz; }

private slots:
    void on_spinBox_UpdateIntervalMs_editingFinished();

    void on_comboBox_FFTSize_currentIndexChanged(int index);

    void on_comboBox_windowFunction_currentIndexChanged(int index);

    void on_spinBox_UpdateIntervalMs_valueChanged(int arg1);

    void on_checkBox_checkStateChanged(const Qt::CheckState &arg1);

    void on_comboBox_scaleFactor_currentIndexChanged(int index);

    void on_spinBox_numLines_valueChanged(int arg1);

    void on_doubleSpinBox_spacingX_valueChanged(double arg1);

    void on_doubleSpinBox_spacingZ_valueChanged(double arg1);

    void on_checkBox_applyWindow_checkStateChanged(const Qt::CheckState &arg1);

    void on_checkBox_drawGrid_checkStateChanged(const Qt::CheckState &arg1);

    void on_spinBox_minFreq_valueChanged(int arg1);

    void on_spinBox_maxFreq_valueChanged(int arg1);

    void on_spinBox_bandWidth_valueChanged(int arg1);

    void on_spinBox_gridStepHz_valueChanged(int arg1);

private:
    Ui::SettingsDialog *ui;

    //ENGINE
    WindowFunction m_windowFunction = EngineSettings::instance().windowFunction();
    int m_fftSize = EngineSettings::instance().fftSize();
    int m_updateIntervalMs = EngineSettings::instance().updateIntervalMs();

    //GRAPHICS

    //3d spectrum
    bool m_isLogScale = GraphicsSettings::instance().isLogScale();
    qreal m_logFactor = GraphicsSettings::instance().logFactor();

    int m_numLines = GraphicsSettings::instance().numLines();
    float m_spacingX = GraphicsSettings::instance().spacingX();
    float m_spacingZ = GraphicsSettings::instance().spacingZ();

    //waveform
    bool m_applyWindow = GraphicsSettings::instance().applyWindow();

    //bar spectrum
    bool m_drawGrid = GraphicsSettings::instance().drawGrid();
    int m_minFreq = GraphicsSettings::instance().minFreq();
    int m_maxFreq = GraphicsSettings::instance().maxFreq();
    int m_bandWidth = GraphicsSettings::instance().bandWidth();
    int m_gridStepHz = GraphicsSettings::instance().gridStepHz();
};

#endif // SETTINGSDIALOG_H
