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
