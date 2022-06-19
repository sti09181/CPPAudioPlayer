////////////////////
///// Main.cpp /////
////////////////////

#include "Libraries.hpp"
#include "AudioPlayer.hpp"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    AudioPlayer audioPlayer;
    audioPlayer.show();
    return QApplication::exec();
}
