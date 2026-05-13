
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

void SuperComputing::sortIntBubble(const int& elements, QTextStream& out)
{
    std::vector<int> original_arr(elements);
    for (int i = 0; i < elements; ++i)
        original_arr[i] = rand() % 1000;

    double time_Seq_Int = 0.0, time_Multi_Int = 0.0, time_Cuda_Int = 0.0, time_omp = 0.0;

    std::vector<int> arr_seq = original_arr;
    m_bubbleSortManager.sort(arr_seq, time_Seq_Int);
    ui.lblintSeq->setText(QString::number(time_Seq_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(15);
    ui.lblStatusBar->setText("Zakonczono sortowanie sekwencyjne dla Integer");

    std::vector<int> arr_multi = original_arr;
    m_bubbleSortManager.sortParallel(arr_multi, time_Multi_Int);
    ui.lblIntMulti->setText(QString::number(time_Multi_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(30);
    ui.lblStatusBar->setText("Zakonczono sortowanie wielow¹tkowe dla Integer");

    std::vector<int> arr_cuda = original_arr;
    m_bubbleSortManager.cudaSort(arr_cuda, time_Cuda_Int);
    ui.lblIntCuda->setText(QString::number(time_Cuda_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(40);
    ui.lblStatusBar->setText("Zakonczono sortowanie CUDA dla Integer");

    std::vector<int> arr_omp = original_arr;
    m_bubbleSortManager.sortWithOpenMP(arr_omp, time_omp);
    ui.lblIntOMP->setText(QString::number(time_omp) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(55);
    ui.lblStatusBar->setText("Zakonczono sortowanie OpenMP dla Integer");

    out << elements << ",Integer,"
        << QString::number(time_Seq_Int, 'f', 4) << ","
        << QString::number(time_Multi_Int, 'f', 4) << ","
        << QString::number(time_Cuda_Int, 'f', 4) << ","
        << QString::number(time_omp, 'f', 4) << "\n";

    fillTable(ui.bubbleTableIntSeq, arr_seq);
    fillTable(ui.bubbleTableIntMulti, arr_multi);
    fillTable(ui.bubbleTableIntCuda, arr_cuda);
    fillTable(ui.bubbleTableIntOMP, arr_omp);
}

void SuperComputing::sortFloatBubble(const int& elements, QTextStream& out)
{
    std::vector<float> original_arr(elements);
    for (int i = 0; i < elements; ++i)
        original_arr[i] = static_cast<float>(rand()) / RAND_MAX * 1000.0f;

    double time_Seq_Float = 0.0, time_Multi_Float = 0.0, time_Cuda_Float = 0.0, time_omp = 0.0;

    std::vector<float> arr_seq = original_arr;
    m_bubbleSortManager.sort(arr_seq, time_Seq_Float);
    ui.lblFloatSeq->setText(QString::number(time_Seq_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(65);
    ui.lblStatusBar->setText("Zakonczono sortowanie sekwencyjne dla Float");

    std::vector<float> arr_multi = original_arr;
    m_bubbleSortManager.sortParallel(arr_multi, time_Multi_Float);
    ui.lblFloatMulti->setText(QString::number(time_Multi_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(75);
    ui.lblStatusBar->setText("Zakonczono sortowanie wielow¹tkowe dla Float");

    std::vector<float> arr_cuda = original_arr;
    m_bubbleSortManager.cudaSort(arr_cuda, time_Cuda_Float);
    ui.lblFloatCuda->setText(QString::number(time_Cuda_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(85);
    ui.lblStatusBar->setText("Zakonczono sortowanie CUDA dla Float");

    std::vector<float> arr_omp = original_arr;
    m_bubbleSortManager.sortWithOpenMP(arr_omp, time_omp);
    ui.lblFloatOmp->setText(QString::number(time_omp) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(90);
    ui.lblStatusBar->setText("Zakonczono sortowanie OpenMP dla Float");

    out << elements << ",Float,"
        << QString::number(time_Seq_Float, 'f', 4) << ","
        << QString::number(time_Multi_Float, 'f', 4) << ","
        << QString::number(time_Cuda_Float, 'f', 4) << ","
        << QString::number(time_omp, 'f', 4) << "\n";

    fillTable(ui.bubbleTableFloatSeq, arr_seq);
    fillTable(ui.bubbleTableFloatMulti, arr_multi);
    fillTable(ui.bubbleTableFloatCuda, arr_cuda);
    fillTable(ui.bubbleTableFloatOMP, arr_omp);
}

void SuperComputing::sortIntMerge(const int& elements, QTextStream& out)
{
    std::vector<int> original_arr(elements);
    for (int i = 0; i < elements; ++i)
        original_arr[i] = rand() % 1000;

    double time_Seq_Int = 0.0, time_Multi_Int = 0.0, time_Cuda_Int = 0.0, time_omp = 0.0;

    std::vector<int> arr_seq = original_arr;
    m_mergeSortManager.sort(arr_seq, time_Seq_Int);
    ui.lblIntSeqMerge->setText(QString::number(time_Seq_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(15);
    ui.lblStatusBar->setText("Zakonczono sortowanie sekwencyjne dla Integer");

    std::vector<int> arr_multi = original_arr;
    m_mergeSortManager.sortParallel(arr_multi, time_Multi_Int);
    ui.lblIntMultiMerge->setText(QString::number(time_Multi_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(30);
    ui.lblStatusBar->setText("Zakonczono sortowanie wielow¹tkowe dla Integer");

    std::vector<int> arr_cuda = original_arr;
    m_mergeSortManager.cudaSort(arr_cuda, time_Cuda_Int);
    ui.lblIntCUDAMerge->setText(QString::number(time_Cuda_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(40);
    ui.lblStatusBar->setText("Zakonczono sortowanie CUDA dla Integer");

    std::vector<int> arr_omp = original_arr;
    m_mergeSortManager.sortWithOpenMP(arr_omp, time_omp);
    ui.lblIntOMPMerge->setText(QString::number(time_omp) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(55);
    ui.lblStatusBar->setText("Zakonczono sortowanie OpenMP dla Integer");

    out << elements << ",Integer,"
        << QString::number(time_Seq_Int, 'f', 4) << ","
        << QString::number(time_Multi_Int, 'f', 4) << ","
        << QString::number(time_Cuda_Int, 'f', 4) << ","
        << QString::number(time_omp, 'f', 4) << "\n";

    fillTable(ui.mergeTableIntSeq, arr_seq);
    fillTable(ui.mergeTableIntMulti, arr_multi);
    fillTable(ui.mergeTableIntCUDA, arr_cuda);
    fillTable(ui.mergeTableIntOMP, arr_omp);
}

void SuperComputing::sortFloatMerg(const int& elements, QTextStream& out)
{
    std::vector<float> original_arr(elements);
    for (int i = 0; i < elements; ++i)
        original_arr[i] = static_cast<float>(rand()) / RAND_MAX * 1000.0f;

    double time_Seq_Float = 0.0, time_Multi_Float = 0.0, time_Cuda_Float = 0.0, time_omp = 0.0;

    std::vector<float> arr_seq = original_arr;
    m_mergeSortManager.sort(arr_seq, time_Seq_Float);
    ui.lblFloatSeqMerge->setText(QString::number(time_Seq_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(65);
    ui.lblStatusBar->setText("Zakonczono sortowanie sekwencyjne dla Float");

    std::vector<float> arr_multi = original_arr;
    m_mergeSortManager.sortParallel(arr_multi, time_Multi_Float);
    ui.lblFloatMultiMerge->setText(QString::number(time_Multi_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(75);
    ui.lblStatusBar->setText("Zakonczono sortowanie wielow¹tkowe dla Float");

    std::vector<float> arr_cuda = original_arr;
    m_mergeSortManager.cudaSort(arr_cuda, time_Cuda_Float);
    ui.lblFloatCUDAMerge->setText(QString::number(time_Cuda_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(85);
    ui.lblStatusBar->setText("Zakonczono sortowanie CUDA dla Float");

    std::vector<float> arr_omp = original_arr;
    m_mergeSortManager.sortWithOpenMP(arr_omp, time_omp);
    ui.lblFloatOMPMerge->setText(QString::number(time_omp) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(90);
    ui.lblStatusBar->setText("Zakonczono sortowanie OpenMP dla Float");

    out << elements << ",Float,"
        << QString::number(time_Seq_Float, 'f', 4) << ","
        << QString::number(time_Multi_Float, 'f', 4) << ","
        << QString::number(time_Cuda_Float, 'f', 4) << ","
        << QString::number(time_omp, 'f', 4) << "\n";

    fillTable(ui.mergeTableFloatSeq, arr_seq);
    fillTable(ui.mergeTableFloatMulti, arr_multi);
    fillTable(ui.mergeTableFloatCUDA, arr_cuda);
    fillTable(ui.mergeTableFloatOMP, arr_omp);
}

void SuperComputing::sortIntQuick(const int& elements, QTextStream& out)
{
    std::vector<int> original_arr(elements);
    for (int i = 0; i < elements; ++i)
        original_arr[i] = rand() % 1000;

    double time_Seq_Int = 0.0, time_Multi_Int = 0.0, time_Cuda_Int = 0.0, time_omp = 0.0;

    std::vector<int> arr_seq = original_arr;
    m_quickSortManager.sort(arr_seq, time_Seq_Int);
    ui.lblIntSeqQuick->setText(QString::number(time_Seq_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(15);
    ui.lblStatusBar->setText("Zakonczono sortowanie sekwencyjne dla Integer");

    std::vector<int> arr_multi = original_arr;
    m_quickSortManager.sortParallel(arr_multi, time_Multi_Int);
    ui.lblIntMultiQuick->setText(QString::number(time_Multi_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(30);
    ui.lblStatusBar->setText("Zakonczono sortowanie wielow¹tkowe dla Integer");

    std::vector<int> arr_cuda = original_arr;
    m_quickSortManager.cudaSort(arr_cuda, time_Cuda_Int);
    ui.lblIntCUDAQuick->setText(QString::number(time_Cuda_Int) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(40);
    ui.lblStatusBar->setText("Zakonczono sortowanie CUDA dla Integer");

    std::vector<int> arr_omp = original_arr;
    m_quickSortManager.sortWithOpenMP(arr_omp, time_omp);
    ui.lblIntOMPQuick->setText(QString::number(time_omp) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(55);
    ui.lblStatusBar->setText("Zakonczono sortowanie OpenMP dla Integer");

    out << elements << ",Integer,"
        << QString::number(time_Seq_Int, 'f', 4) << ","
        << QString::number(time_Multi_Int, 'f', 4) << ","
        << QString::number(time_Cuda_Int, 'f', 4) << ","
        << QString::number(time_omp, 'f', 4) << "\n";

    fillTable(ui.quickTableIntSeq, arr_seq);
    fillTable(ui.quickTableIntMulti, arr_multi);
    fillTable(ui.quickTableIntCUDA, arr_cuda);
    fillTable(ui.quickTableIntOMP, arr_omp);
}

void SuperComputing::sortFloatQuick(const int& elements, QTextStream& out)
{
    std::vector<float> original_arr(elements);
    for (int i = 0; i < elements; ++i)
        original_arr[i] = static_cast<float>(rand()) / RAND_MAX * 1000.0f;

    double time_Seq_Float = 0.0, time_Multi_Float = 0.0, time_Cuda_Float = 0.0, time_omp = 0.0;

    std::vector<float> arr_seq = original_arr;
    m_quickSortManager.sort(arr_seq, time_Seq_Float);
    ui.lblFloatSeqQuick->setText(QString::number(time_Seq_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(65);
    ui.lblStatusBar->setText("Zakonczono sortowanie sekwencyjne dla Float");

    std::vector<float> arr_multi = original_arr;
    m_quickSortManager.sortParallel(arr_multi, time_Multi_Float);
    ui.lblFloatMultiQuick->setText(QString::number(time_Multi_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(75);
    ui.lblStatusBar->setText("Zakonczono sortowanie wielow¹tkowe dla Float");

    std::vector<float> arr_cuda = original_arr;
    m_quickSortManager.cudaSort(arr_cuda, time_Cuda_Float);
    ui.lblFloatCUDAQuick->setText(QString::number(time_Cuda_Float) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(85);
    ui.lblStatusBar->setText("Zakonczono sortowanie CUDA dla Float");

    std::vector<float> arr_omp = original_arr;
    m_quickSortManager.sortWithOpenMP(arr_omp, time_omp);
    ui.lblFloatOMPQuick->setText(QString::number(time_omp) + " ms");
    QCoreApplication::processEvents();
    ui.progressBar->setValue(90);
    ui.lblStatusBar->setText("Zakonczono sortowanie OpenMP dla Float");

    out << elements << ",Float,"
        << QString::number(time_Seq_Float, 'f', 4) << ","
        << QString::number(time_Multi_Float, 'f', 4) << ","
        << QString::number(time_Cuda_Float, 'f', 4) << ","
        << QString::number(time_omp, 'f', 4) << "\n";

    fillTable(ui.quickTableFloatSeq, arr_seq);
    fillTable(ui.quickTableFloatMulti, arr_multi);
    fillTable(ui.quickTableFloatCUDA, arr_cuda);
    fillTable(ui.quickTableFloatOMP, arr_omp);
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
        sortIntBubble(elements, out);
    }

    if (ui.chklFloatBubble->isChecked()) {
        sortFloatBubble(elements, out);
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
        sortIntMerge(elements, out);
    }

    if (ui.chkFloatMergeSort->isChecked()) {
        sortFloatMerg(elements, out);
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
        sortIntQuick(elements, out);
    }

    if (ui.chkFloatQuickSort->isChecked()) {
        sortFloatQuick(elements, out);
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
