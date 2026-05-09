#pragma once

#include <QtWidgets/QMainWindow>
#include <QResizeEvent>

#include "ui_SuperComputing.h"
#include "BubbleSortManager.h"

class SuperComputing : public QMainWindow
{
    Q_OBJECT

public:
    SuperComputing(QWidget *parent = nullptr);
    ~SuperComputing();

private:
    Ui::SuperComputingClass ui;
	BubbleSortManager m_bubbleSortManager;

	void loadImage(const QString& imagePath);
	void sortIntBubble(const int& elements, QTextStream& out);
	void sortFloatBubble(const int& elements, QTextStream& out);
    
    template <typename T>
    void fillTable(QTableWidget* table, const std::vector<T>& data);


private slots:
    void onBubbleSortButtonClicked();

};
