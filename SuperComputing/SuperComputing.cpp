#include "SuperComputing.h"
#include "cuda.cuh"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug> 
#include <QProcess>

SuperComputing::SuperComputing(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QObject::connect(ui.btnBubbleSort, &QPushButton::clicked, this, &SuperComputing::onBubbleSortButtonClicked);
}

SuperComputing::~SuperComputing()
{
}

void SuperComputing::loadImage(const QString& imagePath)
{
    if (QFile::exists(imagePath)) {
        QPixmap pixmap(imagePath);

        QLayout* layout = ui.bubbleChart->layout();
        if (!layout) {
            layout = new QVBoxLayout(ui.bubbleChart);
            layout->setContentsMargins(0, 0, 0, 0); 
        }

        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        QLabel* imageLabel = new QLabel(this);
        imageLabel->setPixmap(pixmap.scaled(ui.bubbleChart->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

void SuperComputing::onBubbleSortButtonClicked() {
    int elements = ui.leElementsBubble->text().toInt();
	ui.progressBar->setValue(0);

    QFile csvFile("wyniki_sortowania_bubble.csv");
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
    arguments << "bubbleSortChartGenerator.py";

	process->start("python", arguments);
    process->waitForFinished();

    ui.lblStatusBar->setText("Generuje wykres");
    loadImage("wykres_wydajnosci.png");
    ui.progressBar->setValue(100);
    ui.lblStatusBar->setText("Koniec - Kolejka pusta");
    
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
