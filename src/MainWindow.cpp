#include "include/mainwindow.hpp"
#include <include/bmp2png.hpp>

MainWindow::MainWindow()
          : QMainWindow()
{
    
    this->setWindowTitle(WindowTitle);
    createWidgets();
}
MainWindow::~MainWindow(){ }

void MainWindow::closeEvent( QCloseEvent *event )
{
    /*if( !m_editor->close() )      { event->ignore(); return; }
    if( !m_circuit->newCircuit()) { event->ignore(); return; }*/
    
    event->accept();
}


void MainWindow::setTitle( QString title )
{
    setWindowTitle(WindowTitle + "  -  " + title);
}


void MainWindow::createWidgets()
{
    centralWidget = new QWidget( this );
    centralWidget->setObjectName("centralWidget");

    QGridLayout *baseWidgetLayout = new QGridLayout( centralWidget );
    baseWidgetLayout->setSpacing(0);
    baseWidgetLayout->setContentsMargins(0, 0, 0, 0);

    SourceLbl = new QLabel("Source:",centralWidget);
    DestLbl = new QLabel("Dest:",centralWidget);
    IncompleteLbl = new QLabel("In Progress:",centralWidget);
    CompletedLbl = new QLabel("Completed:",centralWidget);

    SourceTxt = new QLineEdit(centralWidget);
    DestTxt = new QLineEdit(centralWidget);

    SourceBtn = new QPushButton("...", centralWidget);
    DestBtn = new QPushButton("...", centralWidget);

    StartBtn = new QPushButton("Start", centralWidget);
    StopBtn = new QPushButton("X", centralWidget);

    // IncompleteProgList = new QListWidget(centralWidget);
    IncompleteTxtList = new QListWidget(centralWidget);
    // CompletedProgList = new QListWidget(centralWidget);
    CompletedTxtList = new QListWidget(centralWidget);

    ProgressBar = new QProgressBar(centralWidget);
    ProgressBar->setFormat("%p%");
    // ProgressBar->setTextVisible(true);

    baseWidgetLayout->addWidget(SourceLbl, 0, 0);
    baseWidgetLayout->addWidget(SourceTxt, 0, 1);
    baseWidgetLayout->addWidget(SourceBtn, 0, 2);

    baseWidgetLayout->addWidget(DestLbl, 1, 0);
    baseWidgetLayout->addWidget(DestTxt, 1, 1);
    baseWidgetLayout->addWidget(DestBtn, 1, 2);

    baseWidgetLayout->addWidget(StartBtn, 2, 0);
    baseWidgetLayout->addWidget(StopBtn, 2, 2);

    baseWidgetLayout->addWidget(IncompleteLbl, 3, 0);
    baseWidgetLayout->addWidget(IncompleteTxtList, 3, 1);

    baseWidgetLayout->addWidget(CompletedLbl, 4, 0);
    baseWidgetLayout->addWidget(CompletedTxtList, 4, 1);

    baseWidgetLayout->addWidget(ProgressBar, 5, 1);

    ProgressStatus = new QFutureWatcher<QString>();
    QObject::connect(ProgressStatus, SIGNAL(progressRangeChanged(int,int)),
        ProgressBar, SLOT(setRange(int,int)));
    QObject::connect(ProgressStatus, SIGNAL(progressValueChanged(int)),
        ProgressBar, SLOT(setValue(int)));
    // QObject::connect(ProgressStatus, SIGNAL(finished()),
    //     ProgressBar, SLOT(reset()));
    QObject::connect(ProgressStatus, SIGNAL(resultReadyAt(int)),
        this, SLOT(ConversionComplete(int)));
    
    pool = new QThreadPool();
    future = new QFuture<QString>();

    QObject::connect(SourceBtn, SIGNAL(clicked()), this, SLOT(SourceClicked()));
    QObject::connect(DestBtn, SIGNAL(clicked()), this, SLOT(DestClicked()));
    QObject::connect(StartBtn, SIGNAL(clicked()), this, SLOT(StartClicked()));
    QObject::connect(StopBtn, SIGNAL(clicked()), this, SLOT(StopClicked()));

    setCentralWidget(centralWidget);
}

void MainWindow::SourceClicked()
{
    QString SrcFolder = QFileDialog::getExistingDirectory(this, "Select Directory",
            QStandardPaths::locate(QStandardPaths::HomeLocation,"",
                                   QStandardPaths::LocateDirectory),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!SrcFolder.isEmpty())
        SourceTxt->setText(SrcFolder);
}

void MainWindow::DestClicked()
{
    QString SrcFolder = QFileDialog::getExistingDirectory(this, "Select Directory",
            QStandardPaths::locate(QStandardPaths::HomeLocation,"",
                                   QStandardPaths::LocateDirectory),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!SrcFolder.isEmpty())
        DestTxt->setText(SrcFolder);
}

QString MainWindow::SrcPath;
QString MainWindow::DestPath;

void MainWindow::StartClicked()
{
    vector.clear();
    SrcPath = SourceTxt->text();
    DestPath = DestTxt->text();
    if((SrcPath == "") || (DestPath == ""))
        return;

    QDirIterator FileList(SrcPath, QStringList() << "*.bmp",
                          QDir::Files);
    while(FileList.hasNext())
    {
        QString FileName = FileList.next();
        vector.append(FileName);
        QListWidgetItem* ListText = new QListWidgetItem(FileName,
                                                        IncompleteTxtList);
        IncompleteTxtList->addItem(ListText);
    }

    *future = QtConcurrent::mapped(vector, ConvertFile);
    ProgressStatus->setFuture(*future);
    StartBtn->setEnabled(false);
    StopBtn->setEnabled(true);
}

void MainWindow::ConversionComplete(int index)
{
    QListWidgetItem* ListText = new QListWidgetItem(
        ProgressStatus->resultAt(index), CompletedTxtList);
    CompletedTxtList->addItem(ListText);
    StartBtn->setEnabled(true);
    StopBtn->setEnabled(false);
    QMessageBox msg(QMessageBox::Information, "Success", 
        "Images Converted Successfully", QMessageBox::Ok, centralWidget);
    msg.exec();
}

QString MainWindow::getDestPath()
{
    return DestPath;
}

QString ConvertFile(const QString FileName)
{
   QFileInfo FileInfo(FileName);
   QString DestPath = MainWindow::getDestPath();
   QString OutFile = DestPath + "/" + 
                     FileInfo.fileName().section('.',0,-2) + ".png";
   QImage ImageFile(FileName,"bmp");
   ImageFile.save(OutFile, "png", -1);
   return OutFile;
}

void MainWindow::StopClicked()
{
    future->cancel();
    StartBtn->setEnabled(true);
    StopBtn->setEnabled(false);
    QMessageBox msg(QMessageBox::Critical, "Aborted", 
        "Images Conversion Aborted", QMessageBox::Ok, centralWidget);
}
