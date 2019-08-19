#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

QString ConvertFile(const QString);

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

 static MainWindow* self() { return m_pSelf; }
        
        void setTitle( QString title );
        static QString getSourcePath();
        static QString getDestPath();

    protected:
        QWidget *centralWidget;
        QGridLayout *baseWidgetLayout;
        QLabel *SourceLbl, *DestLbl, *IncompleteLbl, *CompletedLbl;
        QLineEdit *SourceTxt, *DestTxt;
        QListWidget *IncompleteProgList, *IncompleteTxtList,
                  *CompletedProgList, *CompletedTxtList;
        QPushButton *SourceBtn, *DestBtn;
        QPushButton *StartBtn, *StopBtn;
        QProgressBar *ProgressBar;
        QFutureWatcher<QString> *ProgressStatus;
        QThreadPool *pool;
        QFuture<QString> *future;
        QList<QString> vector;

        void closeEvent(QCloseEvent* event);

    public slots:
        void SourceClicked();
        void DestClicked();
        void StartClicked();
        void StopClicked();
        void ConversionComplete(int);

    private:
        int Progress;
        static QString SrcPath;
        static QString DestPath;

 static MainWindow* m_pSelf;

        void createWidgets();
        void createMenus();
        void createToolBars();
        void writeSettings();
        void applyStile();
        
};

#endif
