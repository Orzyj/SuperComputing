
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug> 
#include <QProcess>
#include <QWindowStateChangeEvent>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <qstandardpaths.h>

#include "SuperComputing.h"
#include "cuda.cuh"
#include "Constants.h"

SuperComputing::SuperComputing(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QMenu* reportsMenu = menuBar()->addMenu("Raporty");
    QAction* generateReportAction = reportsMenu->addAction("Generuj raport PDF");
    
    QObject::connect(generateReportAction, &QAction::triggered, this, &SuperComputing::onGenerateReportButtonClicked);
    QObject::connect(ui.btnBubbleSort, &QPushButton::clicked, this, &SuperComputing::onBubbleSortButtonClicked);
    QObject::connect(ui.btnMergeSort, &QPushButton::clicked, this, &SuperComputing::onMergeSortButtonClicked);
    QObject::connect(ui.btnQuickSort, &QPushButton::clicked, this, &SuperComputing::onQuickSortButtonClicked);
	QObject::connect(ui.btnRefreshCharts, &QPushButton::clicked, this, &SuperComputing::onRefreshChartsButtonClicked);   
}

SuperComputing::~SuperComputing()
{
}

void SuperComputing::loadImage(const QString& imagePath, QFrame* frame)
{
    if (QFile::exists(imagePath)) {
        QPixmap pixmap(imagePath);

        QLayout* layout = frame->layout();
        if (!layout) {
            layout = new QVBoxLayout(frame);
            layout->setContentsMargins(0, 0, 0, 0); 
        }

        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        QLabel* imageLabel = new QLabel(this);
        imageLabel->setPixmap(pixmap.scaled(frame->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(imageLabel);

    }
    else {
        qDebug() << "Nie znaleziono pliku z wykresem: " << imagePath;
    }
}

void SuperComputing::onBubbleSortButtonClicked() {
    int elements = ui.leElementsBubble->text().toInt();
    ui.progressBar->setValue(0);

    QFile csvFile(Config::Files::BUBBLE_SORT_CSV);
    bool isNewFile = !csvFile.exists();

    if (!csvFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Nie udalo sie otworzyc pliku CSV do zapisu!";
        return;
    }

    QTextStream out(&csvFile);

    if (isNewFile) {
        out << "Ilosc_elementow,Typ_danych,Sekwencyjnie(ms),Wielowatkowo(ms),CUDA(ms),OpenMP(ms)\n";
    }

    if (ui.chbIntBubble->isChecked()) {
        BubbleSortManager<int> managerInt;
        executeBenchmark<int>(managerInt, elements, out, "Integer",
            ui.lblintSeq, ui.lblIntMulti, ui.lblIntCuda, ui.lblIntOMP,
            ui.bubbleTableIntSeq, ui.bubbleTableIntMulti, ui.bubbleTableIntCuda, ui.bubbleTableIntOMP);
    }

    if (ui.chklFloatBubble->isChecked()) {
        BubbleSortManager<float> managerFloat;
        executeBenchmark<float>(managerFloat, elements, out, "Float",
            ui.lblFloatSeq, ui.lblFloatMulti, ui.lblFloatCuda, ui.lblFloatOmp,
            ui.bubbleTableFloatSeq, ui.bubbleTableFloatMulti, ui.bubbleTableFloatCuda, ui.bubbleTableFloatOMP);
    }

    ui.progressBar->setValue(98);
    ui.lblStatusBar->setText("Zakonczono sortowania");

    csvFile.close();

    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << "SortChartGenerator.py"
        << Config::Files::BUBBLE_SORT_CSV
        << Config::Images::BUBBLE_CHART
        << Config::SortingAlgorithms::BUBBLE;

    process->start("python", arguments);
    process->waitForFinished();

    ui.lblStatusBar->setText("Generuje wykres");
    loadImage(Config::Images::BUBBLE_CHART, ui.bubbleChart);
    ui.progressBar->setValue(100);
    ui.lblStatusBar->setText("Koniec - Kolejka pusta");
}

void SuperComputing::onMergeSortButtonClicked() {
    int elements = ui.leElementsMergeSort->text().toInt();
    ui.progressBar->setValue(0);

    QFile csvFile(Config::Files::MERGE_SORT_CSV);
    bool isNewFile = !csvFile.exists();

    if (!csvFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Nie udalo sie otworzyc pliku CSV do zapisu!";
        return;
    }

    QTextStream out(&csvFile);

    if (isNewFile) {
        out << "Ilosc_elementow,Typ_danych,Sekwencyjnie(ms),Wielowatkowo(ms),CUDA(ms),OpenMP(ms)\n";
    }

    if (ui.chbIntMergeSort->isChecked()) {
        MergeSortManager<int> managerInt;
        executeBenchmark<int>(managerInt, elements, out, "Integer",
            ui.lblIntSeqMerge, ui.lblIntMultiMerge, ui.lblIntCUDAMerge, ui.lblIntOMPMerge,
            ui.mergeTableIntSeq, ui.mergeTableIntMulti, ui.mergeTableIntCUDA, ui.mergeTableIntOMP);
    }

    if (ui.chkFloatMergeSort->isChecked()) {
        MergeSortManager<float> managerFloat;
        executeBenchmark<float>(managerFloat, elements, out, "Float",
            ui.lblFloatSeqMerge, ui.lblFloatMultiMerge, ui.lblFloatCUDAMerge, ui.lblFloatOMPMerge,
            ui.mergeTableFloatSeq, ui.mergeTableFloatMulti, ui.mergeTableFloatCUDA, ui.mergeTableFloatOMP);
    }

    ui.progressBar->setValue(98);
    ui.lblStatusBar->setText("Zakonczono sortowania");

    csvFile.close();

    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << "SortChartGenerator.py"
        << Config::Files::MERGE_SORT_CSV
        << Config::Images::MERGE_CHART
        << Config::SortingAlgorithms::MERGE;

    process->start("python", arguments);
    process->waitForFinished();

    ui.lblStatusBar->setText("Generuje wykres");
    loadImage(Config::Images::MERGE_CHART, ui.mergeSortChart);
    ui.progressBar->setValue(100);
    ui.lblStatusBar->setText("Koniec - Kolejka pusta");
}

void SuperComputing::onQuickSortButtonClicked() {
    int elements = ui.leElementsQuickSort_4->text().toInt();
    ui.progressBar->setValue(0);

    QFile csvFile(Config::Files::QUICK_SORT_CSV);
    bool isNewFile = !csvFile.exists();

    if (!csvFile.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Nie udalo sie otworzyc pliku CSV do zapisu!";
        return;
    }

    QTextStream out(&csvFile);

    if (isNewFile) {
        out << "Ilosc_elementow,Typ_danych,Sekwencyjnie(ms),Wielowatkowo(ms),CUDA(ms),OpenMP(ms)\n";
    }

    if (ui.chbIntQuickSort->isChecked()) {
        QuickSortManager<int> managerInt;
        executeBenchmark<int>(managerInt, elements, out, "Integer",
            ui.lblIntSeqQuick, ui.lblIntMultiQuick, ui.lblIntCUDAQuick, ui.lblIntOMPQuick,
            ui.quickTableIntSeq, ui.quickTableIntMulti, ui.quickTableIntCUDA, ui.quickTableIntOMP);
    }

    if (ui.chkFloatQuickSort->isChecked()) {
        QuickSortManager<float> managerFloat;
        executeBenchmark<float>(managerFloat, elements, out, "Float",
            ui.lblFloatSeqQuick, ui.lblFloatMultiQuick, ui.lblFloatCUDAQuick, ui.lblFloatOMPQuick,
            ui.quickTableFloatSeq, ui.quickTableFloatMulti, ui.quickTableFloatCUDA, ui.quickTableFloatOMP);
    }

    ui.progressBar->setValue(98);
    ui.lblStatusBar->setText("Zakonczono sortowania");

    csvFile.close();

    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << "SortChartGenerator.py"
        << Config::Files::QUICK_SORT_CSV
        << Config::Images::QUICK_CHART
        << Config::SortingAlgorithms::QUICK;

    process->start("python", arguments);
    process->waitForFinished();

    ui.lblStatusBar->setText("Generuje wykres");
    loadImage(Config::Images::QUICK_CHART, ui.quickChart);
    ui.progressBar->setValue(100);
    ui.lblStatusBar->setText("Koniec - Kolejka pusta");
}

void SuperComputing::onRefreshChartsButtonClicked()
{
    loadImage(Config::Images::BUBBLE_CHART, ui.bubbleChart);
    loadImage(Config::Images::MERGE_CHART, ui.mergeSortChart);
	loadImage(Config::Images::QUICK_CHART, ui.quickChart);
}

void SuperComputing::onGenerateReportButtonClicked()
{
	QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << "SortReportGenerator.py"
        << Config::Files::BUBBLE_SORT_CSV
        << Config::Files::MERGE_SORT_CSV
        << Config::Files::QUICK_SORT_CSV
        << Config::Files::REPORT_PDF;
    process->start("python", arguments);

    if (process->waitForFinished(30000)) {
        ui.lblStatusBar->setText("Pomyœlnie wygenerowano Raport_Wydajnosci.pdf!");
		QMessageBox::information(this, "Raport wygenerowany", "Raport_Wydajnosci.pdf zosta³ pomyœlnie wygenerowany.");

        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString fullPath = desktopPath + "/" + Config::Files::REPORT_PDF;
        QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
    }
    else {
        ui.lblStatusBar->setText("B³¹d generowania raportu: Przekroczono czas.");
		QMessageBox::critical(this, "B³¹d", "Wyst¹pi³ b³¹d podczas generowania raportu. Proszê spróbowaæ ponownie.");
    }
}

template<typename T>
std::vector<T> SuperComputing::generateRandomData(int elements)
{
    std::vector<T> arr(elements);
    for (int i = 0; i < elements; ++i) {
        if constexpr (std::is_same_v<T, int>) {
            arr[i] = rand() % 1000;
        }
        else if constexpr (std::is_same_v<T, float>) {
            arr[i] = static_cast<float>(rand()) / RAND_MAX * 1000.0f;
        }
    }
    return arr;
}

template<typename T>
void SuperComputing::executeBenchmark(ISortManager<T>& manager, int elements, QTextStream& out, const QString& typeName, QLabel* lblSeq, QLabel* lblMulti, QLabel* lblCuda, QLabel* lblOmp, QTableWidget* tblSeq, QTableWidget* tblMulti, QTableWidget* tblCuda, QTableWidget* tblOmp)
{
    std::vector<T> original_arr = generateRandomData<T>(elements);
    double t_seq = 0.0, t_multi = 0.0, t_cuda = 0.0, t_omp = 0.0;

    // SEQ
    std::vector<T> arr_seq = original_arr;
    manager.sort(arr_seq, t_seq);
    lblSeq->setText(QString::number(t_seq) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(15);
    ui.lblStatusBar->setText("Zakonczono sortowanie sekwencyjne dla " + typeName);

    // MULTITHREAD
    std::vector<T> arr_multi = original_arr;
    manager.sortParallel(arr_multi, t_multi);
    lblMulti->setText(QString::number(t_multi) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(30);
    ui.lblStatusBar->setText("Zakonczono sortowanie wielow¹tkowe dla " + typeName);

    // CUDA
    std::vector<T> arr_cuda = original_arr;
    manager.cudaSort(arr_cuda, t_cuda);
    lblCuda->setText(QString::number(t_cuda) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(40);
    ui.lblStatusBar->setText("Zakonczono sortowanie CUDA dla " + typeName);

    // OpenMP
    std::vector<T> arr_omp = original_arr;
    manager.sortWithOpenMP(arr_omp, t_omp);
    lblOmp->setText(QString::number(t_omp) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(55);
    ui.lblStatusBar->setText("Zakonczono sortowanie OpenMP dla " + typeName);

    out << elements << "," << typeName << ","
        << QString::number(t_seq, 'f', 4) << ","
        << QString::number(t_multi, 'f', 4) << ","
        << QString::number(t_cuda, 'f', 4) << ","
        << QString::number(t_omp, 'f', 4) << "\n";

    fillTable(tblSeq, arr_seq);
    fillTable(tblMulti, arr_multi);
    fillTable(tblCuda, arr_cuda);
    fillTable(tblOmp, arr_omp);
}

template<typename T>
void SuperComputing::fillTable(QTableWidget* table, const std::vector<T>& data)
{
    if (!table) return; 

    int rowsToShow = std::min(100, (int)data.size());
    table->setRowCount(rowsToShow);
    table->setColumnCount(1); 
    table->setHorizontalHeaderLabels({ "Posortowane elementy" });

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < rowsToShow; ++i) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(data[i])));
        table->item(i, 0)->setTextAlignment(Qt::AlignCenter);
    }
}

void SuperComputing::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent* stateEvent = static_cast<QWindowStateChangeEvent*>(event);
        if (this->isMaximized() && !(stateEvent->oldState() & Qt::WindowMaximized)) {
            loadImage(Config::Images::BUBBLE_CHART, ui.bubbleChart);
            loadImage(Config::Images::MERGE_CHART, ui.mergeSortChart);
            loadImage(Config::Images::QUICK_CHART, ui.quickChart);
        }
        else if (!this->isMaximized() && (stateEvent->oldState() & Qt::WindowMaximized)) {
            loadImage(Config::Images::BUBBLE_CHART, ui.bubbleChart);
            loadImage(Config::Images::MERGE_CHART, ui.mergeSortChart);
            loadImage(Config::Images::QUICK_CHART, ui.quickChart);
        }
        QMainWindow::changeEvent(event);
    }
}

template void SuperComputing::executeBenchmark<int>(ISortManager<int>&, int, QTextStream&, const QString&, QLabel*, QLabel*, QLabel*, QLabel*, QTableWidget*, QTableWidget*, QTableWidget*, QTableWidget*);
template void SuperComputing::executeBenchmark<float>(ISortManager<float>&, int, QTextStream&, const QString&, QLabel*, QLabel*, QLabel*, QLabel*, QTableWidget*, QTableWidget*, QTableWidget*, QTableWidget*);