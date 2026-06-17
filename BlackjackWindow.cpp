#include "BlackjackWindow.h"

BlackjackWindow::BlackjackWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Blackjack");
    resize(1000, 700);
    setMinimumSize(900, 650);

    setStyleSheet(
        "QWidget {"
        " background-color: #0b5d1e;"
        " color: white;"
        " font-size: 15px;"
        "}"
        "QLabel {"
        " color: white;"
        "}"
        "QPushButton {"
        " background-color: white;"
        " color: black;"
        " border-radius: 8px;"
        " padding: 5px;"
        " font-size: 15px;"
        " font-weight: bold;"
        "}"
        "QPushButton:hover {"
        " background-color: #dddddd;"
        "}"
        "QPushButton:disabled {"
        " background-color: #777777;"
        " color: #bbbbbb;"
        "}"
        "QSpinBox {"
        " background-color: white;"
        " color: black;"
        " font-size: 15px;"
        " min-width: 70px;"
        " max-width: 90px;"
        "}"
        );

    titleLabel = new QLabel("BLACKJACK");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: bold;"
        "color: gold;"
        );

    roundLabel = new QLabel();
    chipsLabel = new QLabel();
    statsLabel = new QLabel();
    betInfoLabel = new QLabel();
    dealerTotalLabel = new QLabel();
    playerTotalLabel = new QLabel();

    messageLabel = new QLabel();
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setWordWrap(true);
    messageLabel->setMinimumHeight(45);
    messageLabel->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #fff2a8;"
        );

    betBox = new QSpinBox();
    betBox->setMinimum(1);

    dealButton = new QPushButton("Deal");
    hitButton = new QPushButton("Hit");
    standButton = new QPushButton("Stand");
    doubleButton = new QPushButton("Double Down");

    dealerCardLayout = new QHBoxLayout();
    playerCardLayout = new QHBoxLayout();

    dealerCardLayout->setSpacing(8);
    playerCardLayout->setSpacing(8);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(3);
    mainLayout->setContentsMargins(15, 10, 15, 10);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(roundLabel);
    mainLayout->addWidget(chipsLabel);
    mainLayout->addWidget(statsLabel);
    mainLayout->addWidget(betInfoLabel);

    QHBoxLayout* betLayout = new QHBoxLayout();
    betLayout->setSpacing(10);
    betLayout->addWidget(new QLabel("Bet:"));
    betLayout->addWidget(betBox);
    betLayout->addWidget(dealButton);
    betLayout->addStretch();

    mainLayout->addLayout(betLayout);

    QLabel* dealerTitle = new QLabel("Dealer's Hand:");
    dealerTitle->setStyleSheet("font-weight: bold; font-size: 18px;");
    mainLayout->addWidget(dealerTitle);
    mainLayout->addLayout(dealerCardLayout);
    mainLayout->addWidget(dealerTotalLabel);

    QLabel* playerTitle = new QLabel("Your Hand:");
    playerTitle->setStyleSheet("font-weight: bold; font-size: 18px;");
    mainLayout->addWidget(playerTitle);
    mainLayout->addLayout(playerCardLayout);
    mainLayout->addWidget(playerTotalLabel);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(hitButton);
    buttonLayout->addWidget(standButton);
    buttonLayout->addWidget(doubleButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(messageLabel);

    setLayout(mainLayout);

    dealerTimer = new QTimer(this);
    dealerTimer->setInterval(800);

    connect(dealButton, &QPushButton::clicked, this, [this]() {
        game.startRound(betBox->value());
        updateScreen(true);
    });

    connect(hitButton, &QPushButton::clicked, this, [this]() {
        game.hit();
        updateScreen(true);
    });

    connect(standButton, &QPushButton::clicked, this, [this]() {
        game.beginDealerTurn();
        updateScreen(false);
        dealerTimer->start();
    });

    connect(doubleButton, &QPushButton::clicked, this, [this]() {
        game.doubleDown();
        updateScreen(true);

        if (game.isDealerTurn()) {
            dealerTimer->start();
        }
    });

    connect(dealerTimer, &QTimer::timeout, this, [this]() {
        if (game.dealerNeedsCard()) {
            game.dealerDrawOne();
            updateScreen(true);
        } else {
            dealerTimer->stop();
            game.settleNormalRound();
            updateScreen(false);
        }
    });

    updateScreen(false);
}

QString BlackjackWindow::cardImagePath(const Card& card) {
    return QString::fromStdString(
        ":/cards/" + card.rank + "_" + card.suit + ".png"
        );
}
void BlackjackWindow::clearCardLayout(QHBoxLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}
void BlackjackWindow::addCardImage(QHBoxLayout* layout, const QString& imagePath, bool animate) {
    QLabel* cardLabel = new QLabel();

    QPixmap pixmap(imagePath);

    if (pixmap.isNull()) {
        cardLabel->setText("Image not found:\n" + imagePath);
        cardLabel->setAlignment(Qt::AlignCenter);
        cardLabel->setFixedSize(90, 125);
        cardLabel->setStyleSheet(
            "border: 2px solid white;"
            "background-color: #333333;"
            "color: red;"
            "font-size: 9px;"
            );
    } else {
        cardLabel->setPixmap(
            pixmap.scaled(80, 112, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
        cardLabel->setAlignment(Qt::AlignCenter);
        cardLabel->setFixedSize(90, 125);
    }

    layout->addWidget(cardLabel);

    if (animate) {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(cardLabel);
        cardLabel->setGraphicsEffect(effect);

        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(250);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
void BlackjackWindow::updateCardImages(bool animate) {
    clearCardLayout(dealerCardLayout);
    clearCardLayout(playerCardLayout);

    const vector<Card>& dealerHand = game.getDealerHand();
    const vector<Card>& playerHand = game.getPlayerHand();

    for (int i = 0; i < dealerHand.size(); i++) {
        if (game.isDealerHidden() && i == 1) {
            addCardImage(dealerCardLayout, ":/cards/back.png", animate);
        } else {
            addCardImage(dealerCardLayout, cardImagePath(dealerHand[i]), animate);
        }
    }

    for (const Card& card : playerHand) {
        addCardImage(playerCardLayout, cardImagePath(card), animate);
    }

    dealerCardLayout->addStretch();
    playerCardLayout->addStretch();
}
void BlackjackWindow::updateScreen(bool animateCards) {
    roundLabel->setText("Round: " + QString::number(game.getRound()));
    chipsLabel->setText("Chips: $" + QString::number(game.getChips()));

    statsLabel->setText(
        "Wins: " + QString::number(game.getWins()) +
        "    Losses: " + QString::number(game.getLosses()) +
        "    Ties: " + QString::number(game.getTies())
        );

    betInfoLabel->setText("Current Bet: $" + QString::number(game.getBet()));

    dealerTotalLabel->setText("Dealer total: " + QString::number(game.getDealerTotal()));
    playerTotalLabel->setText("Your total: " + QString::number(game.getPlayerTotal()));

    messageLabel->setText(QString::fromStdString(game.getMessage()));

    betBox->setMaximum(game.getChips());

    bool active = game.isRoundActive();
    bool dealerTurn = game.isDealerTurn();
    bool gameOver = game.isGameOver();

    dealButton->setEnabled(!active && !gameOver);
    hitButton->setEnabled(active && !dealerTurn);
    standButton->setEnabled(active && !dealerTurn);
    doubleButton->setEnabled(game.canDoubleDown());

    betBox->setEnabled(!active && !gameOver);

    updateCardImages(animateCards);
}
