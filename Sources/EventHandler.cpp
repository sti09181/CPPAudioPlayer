////////////////////////////
///// EventHandler.cpp /////
////////////////////////////

#include "EventHandler.hpp"

void EventHandler::MediaPlayer::mediaStatusChanged(AudioPlayer* target)
{
    if (!target->mediaPlaylist->isEmpty() and target->mediaPlayer->mediaStatus() != QMediaPlayer::NoMedia)
    {
        target->currentFileLabel->setText("  Current : " + target->mediaPlaylist->media(target->mediaPlaylist->currentIndex()).canonicalUrl().fileName() + "  ");
    }
    else
    {
        target->currentFileLabel->setText("  Current : No Media  ");
        target->totalDurationLabel->setText("  0:00:00  ");
        target->currentDurationLabel->setText("  0:00:00  ");
    }
}

void EventHandler::MediaPlayer::stateChanged(AudioPlayer* target)
{
    if (target->mediaPlayer->state() == QMediaPlayer::PlayingState)
    {
        target->playStatusLabel->setText("  Play Status : Playing  ");
    }
    else if (target->mediaPlayer->state() == QMediaPlayer::PausedState)
    {
        target->playStatusLabel->setText("  Play Status : Paused  ");
    }
    else if (target->mediaPlayer->state() == QMediaPlayer::StoppedState)
    {
        target->playStatusLabel->setText("  Play Status : Stopped  ");
        target->currentDurationLabel->setText("  0:00:00  ");
    }
}

void EventHandler::MediaPlayer::volumeChanged(AudioPlayer* target)
{
    target->volumeStatusLabel->setText(QString::asprintf("  Volume : %d%%  ", target->mediaPlayer->volume()));
}

void EventHandler::MediaPlayer::durationChanged(AudioPlayer* target, int duration)
{
    target->durationBar->setRange(0, duration);
    target->totalDurationLabel->setText(QString::asprintf("  %d:%02d:%02d  ", (duration / 3600000), ((duration / 60000) % 60), ((duration / 1000) % 60)));
}

void EventHandler::MediaPlayer::positionChanged(AudioPlayer* target, int position)
{
    target->durationBar->setValue(position);
    target->currentDurationLabel->setText(QString::asprintf("  %d:%02d:%02d  ", (position / 3600000), ((position / 60000) % 60), ((position / 1000) % 60)));
}

void EventHandler::DurationBar::sliderMoved(AudioPlayer* target, int position)
{
    target->mediaPlayer->setPosition(position);
}

void EventHandler::FileOperation::openFiles(AudioPlayer* target)
{
    EventHandler::PlaybackOperation::clearPlaylist(target);
    EventHandler::FileOperation::addFiles(target);
}

void EventHandler::FileOperation::openFolder(AudioPlayer* target)
{
    EventHandler::PlaybackOperation::clearPlaylist(target);

    const QString directoryPath = QFileDialog::getExistingDirectory(nullptr, "Exploring...", QString());

    if (!directoryPath.isEmpty())
    {
        QDirIterator dirIterator(directoryPath);

        while (dirIterator.hasNext())
        {
            dirIterator.next();

            if (!dirIterator.fileInfo().isDir() and dirIterator.fileInfo().suffix() == "wav")
            {
                target->mediaPlaylist->addMedia(QMediaContent(QUrl(dirIterator.filePath())));
                target->playlistTextEdit->insertPlainText(dirIterator.fileInfo().fileName() + "\n");
            }
        }
    }
}

void EventHandler::FileOperation::addFiles(AudioPlayer* target)
{
    const QList<QUrl> files = QFileDialog::getOpenFileUrls(nullptr, "Exploring...", QUrl(), "Audio Files (*.wav)");

    if (!files.isEmpty())
    {
        for (auto& file : files)
        {
            target->mediaPlaylist->addMedia(QMediaContent(file));
            target->playlistTextEdit->insertPlainText(file.fileName() + "\n");
        }
    }
}

void EventHandler::FileOperation::removeFile(AudioPlayer* target)
{
    if (!target->mediaPlaylist->isEmpty())
    {
        target->mediaPlaylist->removeMedia(target->mediaPlaylist->currentIndex());

        target->playlistTextEdit->clear();

        for (int i = 0; i < target->mediaPlaylist->mediaCount(); i++)
            target->playlistTextEdit->insertPlainText(target->mediaPlaylist->media(i).canonicalUrl().fileName() + "\n");
    }
    else
    {
        EventHandler::PlaybackOperation::clearPlaylist(target);
    }
}

void EventHandler::FileOperation::exit(AudioPlayer* target)
{
    target->close();
}

void EventHandler::PlaybackOperation::play(AudioPlayer* target)
{
    if (target->mediaPlaylist->isEmpty())
        EventHandler::FileOperation::openFiles(target);

    target->mediaPlayer->play();
}

void EventHandler::PlaybackOperation::pause(AudioPlayer* target)
{
    target->mediaPlayer->pause();
}

void EventHandler::PlaybackOperation::stop(AudioPlayer* target)
{
    target->mediaPlayer->stop();
}

void EventHandler::PlaybackOperation::volumeUp(AudioPlayer* target)
{
    target->mediaPlayer->setVolume(std::min(target->mediaPlayer->volume() + 5, 100));
}

void EventHandler::PlaybackOperation::volumeDown(AudioPlayer* target)
{
    target->mediaPlayer->setVolume(std::max(target->mediaPlayer->volume() - 5, 0));
}

void EventHandler::PlaybackOperation::previousFile(AudioPlayer* target)
{
    if (!target->mediaPlaylist->isEmpty() and target->mediaPlaylist->currentIndex() != 0)
    {
        if (target->repeatSingleFileAction->isChecked() or target->repeatAllFilesAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);

        target->mediaPlaylist->previous();

        if (target->repeatSingleFileAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        else if (target->repeatAllFilesAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void EventHandler::PlaybackOperation::nextFile(AudioPlayer* target)
{
    if (!target->mediaPlaylist->isEmpty() and target->mediaPlaylist->currentIndex() != target->mediaPlaylist->mediaCount() - 1)
    {
        if (target->repeatSingleFileAction->isChecked() or target->repeatAllFilesAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);

        target->mediaPlaylist->next();

        if (target->repeatSingleFileAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        else if (target->repeatAllFilesAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void EventHandler::PlaybackOperation::priorityUp(AudioPlayer* target)
{
    if (!target->mediaPlaylist->isEmpty() and target->mediaPlaylist->currentIndex() != 0)
    {
        if (target->repeatSingleFileAction->isChecked() or target->repeatAllFilesAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);

        target->mediaPlaylist->setCurrentIndex(target->mediaPlaylist->previousIndex());
        target->mediaPlaylist->moveMedia(target->mediaPlaylist->currentIndex(), target->mediaPlaylist->nextIndex());

        target->playlistTextEdit->clear();

        for (int i = 0; i < target->mediaPlaylist->mediaCount(); i++)
            target->playlistTextEdit->insertPlainText(target->mediaPlaylist->media(i).canonicalUrl().fileName() + "\n");

        if (target->repeatSingleFileAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        else if (target->repeatAllFilesAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void EventHandler::PlaybackOperation::priorityDown(AudioPlayer* target)
{
    if (!target->mediaPlaylist->isEmpty() and target->mediaPlaylist->currentIndex() != target->mediaPlaylist->mediaCount() - 1)
    {
        if (target->repeatSingleFileAction->isChecked() or target->repeatAllFilesAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);

        target->mediaPlaylist->moveMedia(target->mediaPlaylist->currentIndex(), target->mediaPlaylist->nextIndex());
        target->mediaPlaylist->setCurrentIndex(target->mediaPlaylist->nextIndex());

        target->playlistTextEdit->clear();

        for (int i = 0; i < target->mediaPlaylist->mediaCount(); i++)
            target->playlistTextEdit->insertPlainText(target->mediaPlaylist->media(i).canonicalUrl().fileName() + "\n");

        if (target->repeatSingleFileAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        else if (target->repeatAllFilesAction->isChecked())
            target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void EventHandler::PlaybackOperation::repeatSingleFile(AudioPlayer* target)
{
    if (target->repeatSingleFileAction->isChecked())
    {
        if (target->repeatAllFilesAction->isChecked())
            target->repeatAllFilesAction->setChecked(false);

        target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else
    {
        target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);
    }
}

void EventHandler::PlaybackOperation::repeatAllFiles(AudioPlayer* target)
{
    if (target->repeatAllFilesAction->isChecked())
    {
        if (target->repeatSingleFileAction->isChecked())
            target->repeatSingleFileAction->setChecked(false);

        target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    }
    else
    {
        target->mediaPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);
    }
}

void EventHandler::PlaybackOperation::clearPlaylist(AudioPlayer* target)
{
    target->mediaPlaylist->clear();
    target->playlistTextEdit->clear();
}
