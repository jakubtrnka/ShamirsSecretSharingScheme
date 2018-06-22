#include "mainwindow.h"
#include <ui_mainwindow.h>

#include <slip39_wrapper.h>
#include <get_insecure_randomness.h>

#include <QMessageBox>
#include <QString>
#include <QRegExp>
#include <string>
#include <vector>
#include <cstdint>

#include <iostream>

namespace {
    std::vector<uint8_t> readhex(const std::string & inputfield) {
        if (inputfield.size() % 2) throw "Not multiple of 8 bits.";
        std::vector<uint8_t> output;
        bool carried(false);
        for (unsigned i = 0; i < inputfield.size(); ++i) {
                unsigned tmp;
                if (sscanf( inputfield.c_str() + i, "%1x", &tmp) == 1) {
                    if (carried) {
                        output.back() += tmp;
                        carried = false;
                    }
                    else {
                        output.push_back((uint8_t) (tmp*16));
                        carried = true;
                    }
                }
                else throw "Invalid hex string";
        }
        return output;
    }
} // anonymous namespace

void MainWindow::mergeSecret() const {
    auto qinput = ui->InputField->toPlainText();
    if (qinput.size() == 0) return;
    auto qlines = qinput.split("\n",QString::SkipEmptyParts);
    std::vector<std::vector<std::string>> share_coll;
    for (auto &&line: qlines) {
        std::vector<std::string> sentence;
        auto qwords = line.split(QRegExp("\\s"), QString::SkipEmptyParts);
        for (auto &&word: qwords) {
            sentence.push_back(word.toStdString());
        }
        share_coll.push_back(sentence);
    }
    try {
        std::vector<uint8_t> result;
        if (ui->PBKDF2->isChecked()) {
            result = Shamir::toSeed(share_coll, ui->PassphraseField->toPlainText().toStdString());
        } else
            result = Shamir::toEnthropy(share_coll);
        QString outstring;
        for (auto it: result) {
            outstring.append(QString::asprintf("%02x", it));
        }
        ui->OutputField->setText(outstring);

    } catch (const char * s) {
        QMessageBox qm;
        qm.setIcon(QMessageBox::Critical);
        qm.setText(QString(s));
        qm.exec();
        ui->OutputField->clear();
    }
}

std::vector<uint8_t> MainWindow::readHexEnthropy() const {
    auto qinput = ui->InputField->toPlainText().split("\n", QString::SkipEmptyParts);
    if (qinput.size() == 0) return std::vector<uint8_t>();
    if (qinput.size() > 1) {
        QMessageBox qbx;
        qbx.setIcon(QMessageBox::Question);
        qbx.setText("Really want to distribute? You probably wanted to merge shares");
        qbx.exec();
        return std::vector<uint8_t>();
    }
    std::vector<uint8_t> rawenthropy;
    try {
        auto tmpenthropy = readhex(qinput.first().toStdString());
        rawenthropy = tmpenthropy;
    } catch (const char * s) {
        QMessageBox qbx;
        qbx.setIcon(QMessageBox::Critical);
        qbx.setText(s);
        qbx.exec();
        return std::vector<uint8_t>();
    }
    return rawenthropy;
}

void MainWindow::distributeSecret(const std::vector<uint8_t> & rawenthropy) const {
    if (rawenthropy.size() == 0) return;
    std::vector<std::vector<std::string>> result;
    try {
	    result = Shamir::fromEnthropy(rawenthropy, (unsigned) ui->CountField->value(), (unsigned) ui->ThresholdField->value());
    } catch (const char * s) {
	    QMessageBox qbx;
	    qbx.setIcon(QMessageBox::Critical);
	    qbx.setText(s);
	    qbx.exec();
	    return;
    }
    QString qresult;
    for (auto &&it: result) {
        for (auto &&jt: it) {
            qresult.append(QString::fromStdString(jt));
            qresult.append(' ');
        }
        qresult.resize(qresult.size() - 1);
        qresult.append('\n');
    }
    qresult.resize(qresult.size() - 1);
    ui->OutputField->setText(qresult);
}

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap img(":/shamirlogo");

    ui->shamirlogo->setPixmap(img.scaled(5*(ui->shamirlogo->size()),Qt::KeepAspectRatio));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_GoBtn_clicked(bool) {
    if (ui->DistributeBtn->isChecked()) {
        distributeSecret(readHexEnthropy());
    } else {
        mergeSecret();
    }
}

void MainWindow::on_ReconstructBtn_toggled(bool checked) {
    ui->PBKDF2->setEnabled(checked);
    if (ui->PBKDF2->isChecked()) ui->PassphraseField->setEnabled(true);
    if (!checked) ui->PassphraseField->setEnabled(false);
}

void MainWindow::on_PBKDF2_toggled(bool checked) {
    ui->PassphraseField->setEnabled(checked);
}

void MainWindow::on_Rand128_clicked() {
    std::vector<uint8_t> rnd(16);
    pseudo_random_fill(rnd);
    QString hexrnd;
    for (auto it: rnd)
        hexrnd += QString::asprintf("%02x", it);
    ui->InputField->setPlainText(hexrnd);
    distributeSecret(rnd);
}
