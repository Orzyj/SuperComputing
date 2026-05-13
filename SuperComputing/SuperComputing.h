#pragma once

#include <QtWidgets/QMainWindow>
#include <QResizeEvent>

#include "ui_SuperComputing.h"
#include "BubbleSortManager.h"
#include "MergeSortManager.h"
#include "QuickSortManager.h"

class SuperComputing : public QMainWindow
{
    Q_OBJECT

public:
    SuperComputing(QWidget *parent = nullptr);
    ~SuperComputing();

private:
    Ui::SuperComputingClass ui;

	BubbleSortManager m_bubbleSortManager;
	MergeSortManager m_mergeSortManager;
	QuickSortManager m_quickSortManager;


	void loadImage(const QString& imagePath, QFrame* frame);

	void sortIntBubble(const int& elements, QTextStream& out);
	void sortFloatBubble(const int& elements, QTextStream& out);

    void sortIntMerge(const int& elements, QTextStream& out);
    void sortFloatMerg(const int& elements, QTextStream& out);

    void sortIntQuick(const int& elements, QTextStream& out);
    void sortFloatQuick(const int& elements, QTextStream& out);
    
    template <typename T>
    void fillTable(QTableWidget* table, const std::vector<T>& data);

private slots:
    void onBubbleSortButtonClicked();
	void onMergeSortButtonClicked();
    void onQuickSortButtonClicked();
	void onRefreshChartsButtonClicked();
    void onGenerateReportButtonClicked();

protected:
	void changeEvent(QEvent* event) override;


};
