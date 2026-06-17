#include "BlackjackGame.h"
#include "BlackjackWindow.h"

#include <algorithm>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BlackjackWindow window;
    window.show();

    return app.exec();
}
