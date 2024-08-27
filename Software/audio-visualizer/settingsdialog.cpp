#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
// , m_windowFunction(DefaultWindowFunction)
// , m_fftSize(1024)
// , m_updateIntervalMs(10)
{
    ui->setupUi(this);

    for (int i = 8; i <= 16; i++) {
        int pwr = qPow(2, i);
        ui->comboBox_FFTSize->addItem(QString::number(pwr), QVariant::fromValue(pwr));
    }
    ui->comboBox_FFTSize->setCurrentIndex(
        ui->comboBox_FFTSize->findData(QVariant::fromValue(EngineSettings::instance().fftSize())));

    ui->comboBox_windowFunction->addItem(tr("None"),
                                         QVariant::fromValue(int(WindowFunction::NoWindow)));
    ui->comboBox_windowFunction->addItem("Hann",
                                         QVariant::fromValue(int(WindowFunction::HannWindow)));
    ui->comboBox_windowFunction->addItem("Blackman",
                                         QVariant::fromValue(int(WindowFunction::BlackmanWindow)));
    ui->comboBox_windowFunction->setCurrentIndex(EngineSettings::instance().windowFunction());

    //3D GRAPHICS
    for (int i = 0; i <= 8; i++) {
        int pwr = qPow(2, i);
        ui->comboBox_scaleFactor->addItem("/" + QString::number(pwr),
                                          QVariant::fromValue(1 / static_cast<qreal>(pwr)));
    }
    ui->comboBox_scaleFactor->setCurrentIndex(ui->comboBox_scaleFactor->findData(
        QVariant::fromValue(GraphicsSettings::instance().logFactor())));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_spinBox_UpdateIntervalMs_editingFinished()
{
    int currentValue = ui->spinBox_UpdateIntervalMs->value();

    // Provjera je li currentValue množitelj 10
    if (currentValue % 10 != 0) {
        int validValue = (currentValue / 10) * 10;
        ui->spinBox_UpdateIntervalMs->setValue(validValue);
    }
}

void SettingsDialog::on_comboBox_FFTSize_currentIndexChanged(int index)
{
    m_fftSize = ui->comboBox_FFTSize->itemData(index).value<int>();
}

void SettingsDialog::on_comboBox_windowFunction_currentIndexChanged(int index)
{
    m_windowFunction = static_cast<WindowFunction>(
        ui->comboBox_windowFunction->itemData(index).value<int>());
}

void SettingsDialog::on_spinBox_UpdateIntervalMs_valueChanged(int arg1)
{
    m_updateIntervalMs = arg1 == 0 ? 1 : arg1;
}

void SettingsDialog::on_checkBox_checkStateChanged(const Qt::CheckState &arg1)
{
    if (arg1 == Qt::CheckState::Checked)
        m_isLogScale = true;
    else
        m_isLogScale = false;
}

void SettingsDialog::on_comboBox_scaleFactor_currentIndexChanged(int index)
{
    m_logFactor = ui->comboBox_scaleFactor->itemData(index).value<qreal>();
}

void SettingsDialog::on_spinBox_numLines_valueChanged(int arg1)
{
    m_numLines = arg1;
}

void SettingsDialog::on_doubleSpinBox_spacingX_valueChanged(double arg1)
{
    m_spacingX = arg1;
}

void SettingsDialog::on_doubleSpinBox_spacingZ_valueChanged(double arg1)
{
    m_spacingZ = arg1;
}
