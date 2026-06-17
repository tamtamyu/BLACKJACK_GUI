#ifndef BLACKJACKWINDOW_H
#define BLACKJACKWINDOW_H

#include "BlackjackGame.h"

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QString>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QUrl>


class BlackjackWindow : public QWidget {
private:
    BlackjackGame game;

    QLabel* titleLabel;
    QLabel* roundLabel;
    QLabel* chipsLabel;
    QLabel* statsLabel;
    QLabel* betInfoLabel;
    QLabel* dealerTotalLabel;
    QLabel* playerTotalLabel;
    QLabel* messageLabel;

    QSpinBox* betBox;

    QPushButton* dealButton;
    QPushButton* hitButton;
    QPushButton* standButton;
    QPushButton* doubleButton;

    QHBoxLayout* dealerCardLayout;
    QHBoxLayout* playerCardLayout;

    QTimer* dealerTimer;

public:
    BlackjackWindow(QWidget* parent = nullptr);


    void setupSounds();

    void playCardSound();

    void playResultSound();

    QString cardImagePath(const Card& card);

    void clearCardLayout(QHBoxLayout* layout);

    void addCardImage(QHBoxLayout* layout, const QString& imagePath, bool animate);

    void updateCardImages(bool animate);

    void updateScreen(bool animateCards);
};

#endif // BLACKJACKWINDOW_H
