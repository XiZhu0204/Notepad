#include "notepad.h"
#include "ui_notepad.h"

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
    currentFile.clear();
    ui->textBox->setText(QString());
}

void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString readText = in.readAll();
    ui->textBox->setText(readText);
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
        QMessageBox unsavedMsg;
        unsavedMsg.setText("You have unsaved changes.");
        unsavedMsg.setInformativeText("Do you want to save your changes?");
        unsavedMsg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (unsavedMsg.exec()) {
        case QMessageBox::Save:
            Notepad::on_actionSave_triggered();
            break;
        case QMessageBox::Discard:
            QApplication::quit();
            break;
        case QMessageBox::Cancel:
            return;
        default:
            // should never be reached
            break;
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
    Notepad::on_actionExit_triggered();
    // below might not do anything
    // TODO: Read up on the documentation to find difference, if any, between QApplication::quit() and QWidget::closeEvent(event)
    QWidget::closeEvent(event);
}
