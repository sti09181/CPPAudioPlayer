////////////////////////////
///// EventHandler.hpp /////
////////////////////////////

#pragma once
#include "Libraries.hpp"
#include "AudioPlayer.hpp"

class AudioPlayer;

namespace EventHandler
{
    namespace MediaPlayer
    {
        void mediaStatusChanged(AudioPlayer* target);
        void stateChanged(AudioPlayer* target);
        void volumeChanged(AudioPlayer* target);
        void durationChanged(AudioPlayer* target, int duration);
        void positionChanged(AudioPlayer* target, int position);
    }

    namespace DurationBar
    {
        void sliderMoved(AudioPlayer* target, int position);
    }

    namespace FileOperation
    {
        void openFiles(AudioPlayer* target);
        void openFolder(AudioPlayer* target);
        void addFiles(AudioPlayer* target);
        void removeFile(AudioPlayer* target);
        void exit(AudioPlayer* target);
    }

    namespace PlaybackOperation
    {
        void play(AudioPlayer* target);
        void pause(AudioPlayer* target);
        void stop(AudioPlayer* target);
        void volumeUp(AudioPlayer* target);
        void volumeDown(AudioPlayer* target);
        void previousFile(AudioPlayer* target);
        void nextFile(AudioPlayer* target);
        void priorityUp(AudioPlayer* target);
        void priorityDown(AudioPlayer* target);
        void repeatSingleFile(AudioPlayer* target);
        void repeatAllFiles(AudioPlayer* target);
        void clearPlaylist(AudioPlayer* target);
    }
}
