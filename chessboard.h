#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QMessageBox>
#include <QDialog>
#include <QShowEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include "plateau.h"

class MenuDialog:public QDialog
{
    Q_OBJECT

public:
    explicit MenuDialog(QWidget *parent=nullptr);
    int choix() const {return m_choix;}

protected:
    void showEvent(QShowEvent *event) override;

private:
    int m_choix=1;
};

class ChessBoard:public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(int gameMode,QWidget *parent=nullptr);

protected:
    void paintEvent(QPaintEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    plateau  m_plateau;
    int m_offsetX = 0;
    int m_offsetY = 0;
    int      m_mode;
    bool     m_selected;
    int      m_selRow, m_selCol;
    bool     m_gameOver;
    QString  m_statusMsg;
    int m_cell = 70;     // sensible default, gets recalculated on resize
    int m_margin = 30;   // sensible default, gets recalculated on resize
    QPoint cellToPixel(int row,int col)const;
    void pixelToCell(QPoint pt,int &row,int &col)const;
    void drawBoard(QPainter &p);
    void drawPieces(QPainter &p);
    void drawHighlight(QPainter &p);
    void drawStatus(QPainter &p);
    void drawAxisLabels(QPainter &p);
    void recomputeGeometry();
    QString pieceText(int val) const;   // Unicode chess symbol
    QColor  pieceColor(int val) const;
    void afterMove();
    void checkFinPartie();
};

#endif
