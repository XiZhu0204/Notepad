#include "notepad.h"
#include "ui_notepad.h"

#include <QCloseEvent>

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textBox);
}

Notepad::~Notepad()
{
    delete ui;
}


void Notepad::on_actionNew_triggered()
{
    if(!saved && !unsaved_dialog()) {
        return;
    }
    currentFile.clear();
    setWindowTitle("Notepad");
    ui->textBox->setText(QString());
    saved = true;

}

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file: " + file.errorString());
        return;
    }
    int lastSlash = fileName.lastIndexOf('/');
    fileName = fileName.remove(0, lastSlash + 1);
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString readText = in.readAll();
    ui->textBox->setText(readText);
    saved = true;
    file.close();
}

void Notepad::on_actionSave_triggered()
{
    if(QFile::exists(currentFile)) {
        QFile file(currentFile);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Error", "Could not save: " + file.errorString());
            return;
        }
        QTextStream out(&file);
        QString readText = ui->textBox->toPlainText();
        out << readText;
        saved = true;
        file.close();
    } else {
        Notepad::on_actionSave_as_triggered();
    }

}

void Notepad::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as") + ".txt";
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Could not save: " + file.errorString());
        return;
    }
    int lastSlash = fileName.lastIndexOf('/');
    fileName = fileName.remove(0, lastSlash + 1);
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString readText = ui->textBox->toPlainText();
    out << readText;
    saved = true;
    file.close();
}

void Notepad::on_actionExit_triggered()
{
    if (!saved) {
        if (unsaved_dialog()) {
            QApplication::quit();
        } else {
            return;
        }
    } else {
        QApplication::quit();
    }
}

void Notepad::on_textBox_textChanged()
{
    saved = false;
}

void Notepad::on_actionCut_triggered()
{
    ui->textBox->cut();
}

void Notepad::on_actionCopy_triggered()
{
    ui->textBox->copy();
}

void Notepad::on_actionPaste_triggered()
{
    ui->textBox->paste();
}

void Notepad::on_actionUndo_triggered()
{
    ui->textBox->undo();
}

void Notepad::on_actionRedo_triggered()
{
    ui->textBox->redo();
}

void Notepad::closeEvent(QCloseEvent *event) {
    if (!saved) {
        if (unsaved_dialog()) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        QWidget::closeEvent(event);
    }
}

bool Notepad::unsaved_dialog() {
    QMessageBox unsavedMsg;
    unsavedMsg.setText("You have unsaved changes.");
    unsavedMsg.setInformativeText("Do you want to save your changes?");
    unsavedMsg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (unsavedMsg.exec()) {
    case QMessageBox::Save:
        Notepad::on_actionSave_triggered();
        return true;
    case QMessageBox::Discard:
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        // should never be reached
        return false;
    }
}
