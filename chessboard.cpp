#include "chessboard.h"
#include <QDialog>
#include <QShowEvent>
#include <QResizeEvent>
#include <QTimer>
#include <QMessageBox>
#include <cmath>

// =====================================================================
//  ChessBoard
// =====================================================================

ChessBoard::ChessBoard(int gameMode,QWidget *parent)
    : QWidget(parent),
    m_mode(gameMode),
    m_selected(false),
    m_selRow(-1),m_selCol(-1),
    m_gameOver(false)
{
    setWindowTitle("Jeu d'Échecs");
    setStyleSheet("background:#1a1a2e;");
    m_statusMsg="Tour des Blancs";
    recomputeGeometry();
}

void ChessBoard::recomputeGeometry()
{
    // Reserve space at the bottom for the status text + axis labels
    int reservedBottom = 60;
    int reservedForLabels = 40; // top/left labels + a/h bottom labels

    int available = qMin(width(), height()) - reservedBottom - reservedForLabels;
    m_cell = qMax(20, available / 8);          // never shrink below 20px
    m_margin = reservedForLabels / 2 + 10;

    // Total footprint of the board including margin and reserved bottom space
    int boardWidth  = m_margin*2 + m_cell*8;
    int boardHeight = m_margin*2 + m_cell*8 + reservedBottom;

    // Center that whole footprint within the actual widget size
    m_offsetX = qMax(0, (width()  - boardWidth)  / 2);
    m_offsetY = qMax(0, (height() - boardHeight) / 2);
}

void ChessBoard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    recomputeGeometry();
    update();
}

void ChessBoard::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    showFullScreen();
    recomputeGeometry();   // make sure geometry is correct once fullscreen size is known
    update();
}

QPoint ChessBoard::cellToPixel(int row,int col)const
{
    return {m_offsetX+m_margin+col*m_cell, m_offsetY+m_margin+row*m_cell};
}

void ChessBoard::pixelToCell(QPoint pt,int &row,int &col)const
{
    col=(pt.x()-m_offsetX-m_margin)/m_cell;
    row=(pt.y()-m_offsetY-m_margin)/m_cell;
}

QString ChessBoard::pieceText(int val) const
{
    switch(val){
    case  1: return "♙";
    case  2: return "♖";
    case  3: return "♘";
    case  4: return "♗";
    case  5: return "♕";
    case  6: return "♔";
    case -1: return "♟";
    case -2: return "♜";
    case -3: return "♞";
    case -4: return "♝";
    case -5: return "♛";
    case -6: return "♚";
    default: return "";
    }
}

QColor ChessBoard::pieceColor(int val)const
{
    return (val>0)?QColor(255,255,240):QColor(30,30,30);
}

void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    drawBoard(p);
    drawHighlight(p);
    drawPieces(p);
    drawAxisLabels(p);
    drawStatus(p);
}

void ChessBoard::drawBoard(QPainter &p)
{
    for(int r=0;r<8;r++){
        for(int c=0;c<8;c++){
            QColor light("#c8a97e");
            QColor dark("#6b3f1f");
            QColor fill=((r+c)%2==0)?light:dark;
            QPoint tl=cellToPixel(r,c);
            p.fillRect(tl.x(),tl.y(),m_cell,m_cell,fill);
        }
    }
}

void ChessBoard::drawHighlight(QPainter &p)
{
    if(!m_selected) return;
    bool stillInCheck;
    QPoint dl;
    QPoint tl=cellToPixel(m_selRow, m_selCol);

    p.fillRect(tl.x(),tl.y(),m_cell,m_cell,QColor(255,220,50,160));
    int couleur=(m_plateau.get_Tour()%2==0)?1:-1;

    p.setBrush(QColor(80,200,120,255));
    p.setPen(Qt::NoPen);
    for(int r=0;r<8;r++){
        for(int c=0;c<8;c++){
            plateau &ref=const_cast<plateau&>(m_plateau);
            if(ref.evaluation(m_selRow,m_selCol,r,c)==1){
                int saved=m_plateau.ech[r][c];
                int piece=m_plateau.ech[m_selRow][m_selCol];
                m_plateau.ech[r][c]=piece;
                m_plateau.ech[m_selRow][m_selCol]=0;

                stillInCheck=(m_plateau.roi_en_echec(couleur)==1);

                m_plateau.ech[m_selRow][m_selCol]=piece;
                m_plateau.ech[r][c]=saved;

                if(stillInCheck) continue;

                dl=cellToPixel(r,c);

                if(saved!=0)
                {
                    p.fillRect(dl.x(),dl.y(),m_cell,m_cell,QColor(200,40,40,180));
                }
                else
                {
                    int radius = qMax(8, m_cell/6);
                    p.drawEllipse(dl.x()+m_cell/2-radius,dl.y()+m_cell/2-radius,radius*2,radius*2);
                }
            }
        }
    }
}

void ChessBoard::drawPieces(QPainter &p)
{
    QFont f("Segoe UI Symbol", qMax(10, m_cell * 6 / 10));
    QPoint tl;
    p.setFont(f);

    for (int r=0; r<8;r++){
        for (int c=0;c<8;c++){
            int val=m_plateau.get_Piece(r,c);
            if(val==0) continue;

            tl=cellToPixel(r,c);
            QRect cell(tl.x(),tl.y(),m_cell,m_cell);

            p.setPen(QColor(0,0,0,100));
            p.drawText(cell.adjusted(2,2,2,2),Qt::AlignCenter,pieceText(val));

            p.setPen(pieceColor(val));
            p.drawText(cell,Qt::AlignCenter,pieceText(val));
        }
    }
}

void ChessBoard::drawAxisLabels(QPainter &p)
{
    const char cols[]="abcdefgh";

    QFont f("Consolas", qMax(10, m_cell/6), QFont::Bold);
    p.setFont(f);
    p.setPen(QColor(180,150,100));

    for(int c=0;c<8;c++){
        int x=m_offsetX+m_margin+c*m_cell+m_cell/2-6;
        p.drawText(x,m_offsetY+m_margin-6,QString(cols[c]));
        p.drawText(x,m_offsetY+m_margin+8*m_cell+18,QString(cols[c]));
    }
    for(int r=0;r<8;r++){
        int y=m_offsetY+m_margin+r*m_cell+m_cell/2+5;
        p.drawText(m_offsetX+6,y,QString::number(8-r));
        p.drawText(m_offsetX+m_margin+8*m_cell+6,y,QString::number(8-r));
    }
}

void ChessBoard::drawStatus(QPainter &p)
{
    int y=m_offsetY+m_margin+8*m_cell+30;
    QFont f("Georgia",13,QFont::Bold);
    p.setFont(f);
    p.setPen(QColor(240,200,60));
    p.drawText(QRect(0,y,width(),36),Qt::AlignCenter,m_statusMsg);
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    int row,col,piece;
    if(m_gameOver) return;
    if(m_mode==2 && m_plateau.get_Tour()%2!=0) return;


    pixelToCell(event->pos(),row,col);
    if(row<0 || row>7 || col<0 || col>7) return;

    piece=m_plateau.get_Piece(row, col);

    if(!m_selected){
        if(piece==0) return;
        if(m_plateau.get_Tour()%2==0 && piece<0) return;
        if(m_plateau.get_Tour()%2==1 && piece>0) return;
        m_selected=true;
        m_selRow=row;
        m_selCol=col;
    }

    else{
        if(row==m_selRow && col==m_selCol){
            m_selected=false;
            update();
            return;
        }

        if(piece!=0){
            bool sameColor=(m_plateau.get_Tour()%2==0 && piece>0) || (m_plateau.get_Tour()%2==1 && piece<0);
            if(sameColor){
                m_selRow=row;
                m_selCol=col;
                update();
                return;
            }
        }

        m_plateau.mov_Piece(m_selRow,m_selCol,row,col);
        m_selected=false;
        afterMove();
    }
    update();
}

void ChessBoard::afterMove()
{
    checkFinPartie();

    if(!m_gameOver && m_mode==2 && m_plateau.get_Tour()%2!=0){
        m_statusMsg="L'IA réfléchit…";
        update();
        QTimer::singleShot(1000,this,[this](){
            m_plateau.deplacer_IA();
            checkFinPartie();
            update();
        });
    }
}

void ChessBoard::checkFinPartie()
{
    int couleur=(m_plateau.get_Tour()%2==0)?1:-1;
    bool enEchec=(m_plateau.roi_en_echec(couleur)==1);
    bool aLegal=(m_plateau.a_mouvement_legal(couleur)==1);

    if(enEchec && !aLegal){
        QString winner=(couleur==1)?"Noirs":"Blancs";
        m_statusMsg=QString("Échec et mat! Les %1 gagnent!").arg(winner);
        m_gameOver=true;
        update();
        QMessageBox msg(this);
        msg.setWindowTitle("Fin de partie");
        msg.setText(QString("Échec et mat!\nLes %1 gagnent!").arg(winner));
        msg.setStyleSheet(
            "QMessageBox {background:#1a1a2e;}"
            "QLabel      {color:white; font-size:16px; font-family:Georgia;}"
            "QPushButton {background:#3a7bd5; color:white; border-radius:4px;"
            "padding:6px 20px; font-size:13px;}"
            );
        msg.setFixedSize(600,300);
        msg.exec();
        return;
    }

    if(!enEchec && !aLegal){
        m_statusMsg="Pat! Match nul!";
        m_gameOver=true;
        update();
        QMessageBox msg(this);
        msg.setWindowTitle("Fin de partie");
        msg.setText("Pat! Match nul!");
        msg.setStyleSheet(
            "QMessageBox {background:#1a1a2e;}"
            "QLabel      {color:white; font-size:16px; font-family:Georgia;}"
            "QPushButton {background:#3a7bd5; color:white; border-radius:4px;"
            "padding:6px 20px;font-size:13px;}"
            );
        msg.setFixedSize(600, 300);
        msg.exec();
        return;
    }

    if(enEchec){
        m_statusMsg=QString("Échec au Roi %1! À vous de jouer.").arg((m_plateau.get_Tour()%2==0)?"Blanc":"Noir");
    }
    else{
        m_statusMsg=QString("Tour des %1").arg((m_plateau.get_Tour()%2==0)?"Blancs":"Noirs");
    }
    update();
}

// =====================================================================
//  MenuDialog
// =====================================================================

MenuDialog::MenuDialog(QWidget *parent):QDialog(parent)
{
    setWindowTitle("Jeu d'Échecs");
    setWindowFlags(Qt::Window);
    setStyleSheet(
        "QDialog {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "    stop:0 #1a1a2e, stop:0.5 #16213e, stop:1 #1a1a2e);"
        "  color:#e0e0e0;"
        "}"
        );

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);
    auto *centerWrap = new QWidget(this);
    auto *centerLayout = new QVBoxLayout(centerWrap);
    centerLayout->setAlignment(Qt::AlignCenter);
    centerLayout->setSpacing(28);
    auto *emblem = new QLabel("♛", centerWrap);
    emblem->setAlignment(Qt::AlignCenter);
    QFont emblemFont("Georgia", 64);
    emblem->setFont(emblemFont);
    emblem->setStyleSheet("color:#f0c040;");
    centerLayout->addWidget(emblem);
    auto *title = new QLabel("JEU D'ÉCHECS", centerWrap);
    title->setAlignment(Qt::AlignCenter);
    QFont tf("Georgia", 42, QFont::Bold);
    title->setFont(tf);
    title->setStyleSheet("color:#f0c040; letter-spacing:6px;");
    centerLayout->addWidget(title);
    auto *sub = new QLabel("Choisissez un mode de jeu", centerWrap);
    sub->setAlignment(Qt::AlignCenter);
    QFont sf("Georgia", 15);
    sub->setFont(sf);
    sub->setStyleSheet("color:#9aa0b4; letter-spacing:1px;");
    centerLayout->addWidget(sub);
    auto *spacer = new QWidget(centerWrap);
    spacer->setFixedHeight(20);
    centerLayout->addWidget(spacer);
    auto *hl = new QHBoxLayout();
    hl->setSpacing(40);
    auto makeCardButton = [&](const QString &icon, const QString &mainText,
                              const QString &subText, const QString &accent,
                              const QString &accentHover) -> QPushButton* {
        auto *btn = new QPushButton(centerWrap);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedSize(300, 220);
        btn->setText(QString("%1\n\n%2\n\n%3").arg(icon, mainText, subText));
        btn->setStyleSheet(QString(
                               "QPushButton {"
                               "  background: rgba(255,255,255,12);"
                               "  border: 2px solid %1;"
                               "  border-radius: 16px;"
                               "  color:#fff;"
                               "  font-size:16px;"
                               "  font-weight:bold;"
                               "  padding:20px;"
                               "}"
                               "QPushButton:hover {"
                               "  background: %2;"
                               "  border: 2px solid %2;"
                               "}"
                               "QPushButton:pressed {"
                               "  background: %1;"
                               "}"
                               ).arg(accent, accentHover));
        return btn;
    };
    auto *btn1 = makeCardButton("♙", "Joueur vs Joueur",
                                "Affrontez un ami\nsur le même écran",
                                "#3a7bd5", "#5a9bf5");
    auto *btn2 = makeCardButton("♞", "Joueur vs IA",
                                "Défiez l'intelligence\nartificielle",
                                "#2d9c5e", "#3fc878");
    hl->addStretch();
    hl->addWidget(btn1);
    hl->addWidget(btn2);
    hl->addStretch();
    centerLayout->addLayout(hl);
    root->addStretch();
    root->addWidget(centerWrap, 0, Qt::AlignCenter);
    root->addStretch();
    auto *footer = new QLabel("Appuyez sur Échap pour quitter", this);
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("color:#5a5f73; font-size:11px; margin-bottom:24px;");
    root->addWidget(footer);
    connect(btn1, &QPushButton::clicked, this, [this]{ m_choix = 1; accept(); });
    connect(btn2, &QPushButton::clicked, this, [this]{ m_choix = 2; accept(); });
}

void MenuDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    showFullScreen();
}