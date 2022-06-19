///////////////////////////
///// AudioPlayer.cpp /////
///////////////////////////

#include "AudioPlayer.hpp"

AudioPlayer::AudioPlayer()
{
    this->initializeVariables();
    this->initializeLayouts();
}

void AudioPlayer::initializeVariables()
{
    auto QActionInitialization = [this](QAction* target, const QString& actionName, const QString& actionTip, void (*actionCallback)(AudioPlayer*)) -> void
    {
        target->setText(actionName);
        target->setStatusTip(actionTip);
        QAction::connect(target, &QAction::triggered, std::bind_front(actionCallback, this));
    };

    auto QPushButtonInitialization = [this](QPushButton* target, const QString& buttonName, const QString& buttonTip, void (*buttonCallback)(AudioPlayer*)) -> void
    {
        target->setText(buttonName);
        target->setStatusTip(buttonTip);
        QPushButton::connect(target, &QPushButton::clicked, std::bind_front(buttonCallback, this));
    };

    this->setCentralWidget(this->mainCentralWidget.get());

    this->mainMenuBar = this->menuBar();
    this->mainStatusBar = this->statusBar();

    this->mediaPlayer->setVolume(60);
    this->mediaPlayer->setPlaylist(this->mediaPlaylist.get());

    QMediaPlayer::connect(this->mediaPlayer.get(), &QMediaPlayer::mediaStatusChanged, std::bind_front(EventHandler::MediaPlayer::mediaStatusChanged, this));
    QMediaPlayer::connect(this->mediaPlayer.get(), &QMediaPlayer::stateChanged, std::bind_front(EventHandler::MediaPlayer::stateChanged, this));
    QMediaPlayer::connect(this->mediaPlayer.get(), &QMediaPlayer::volumeChanged, std::bind_front(EventHandler::MediaPlayer::volumeChanged, this));
    QMediaPlayer::connect(this->mediaPlayer.get(), &QMediaPlayer::durationChanged, std::bind_front(EventHandler::MediaPlayer::durationChanged, this));
    QMediaPlayer::connect(this->mediaPlayer.get(), &QMediaPlayer::positionChanged, std::bind_front(EventHandler::MediaPlayer::positionChanged, this));

    this->volumeStatusLabel->setText(QString::asprintf("  Volume : %d%%  ", this->mediaPlayer->volume()));
    this->playStatusLabel->setText("  Play Status : Stopped  ");
    this->currentFileLabel->setText("  Current : No Media  ");

    QActionInitialization(this->openFilesAction.get(), "&Open Files", "Clear playlist and open files", EventHandler::FileOperation::openFiles);
    QActionInitialization(this->openFolderAction.get(), "&Open Folder", "Clear playlist and open folder", EventHandler::FileOperation::openFolder);
    QActionInitialization(this->addFilesAction.get(), "&Add Files", "Add multiple files to the current playlist", EventHandler::FileOperation::addFiles);
    QActionInitialization(this->removeFileAction.get(), "&Remove File", "Remove current file from the playlist", EventHandler::FileOperation::removeFile);
    QActionInitialization(this->exitFileAction.get(), "&Exit", "Terminate this program", EventHandler::FileOperation::exit);

    this->repeatSingleFileAction->setCheckable(true);
    this->repeatAllFilesAction->setCheckable(true);

    QActionInitialization(this->playAction.get(), "&Play", "Play current file", EventHandler::PlaybackOperation::play);
    QActionInitialization(this->pauseAction.get(), "&Pause", "Pause current file", EventHandler::PlaybackOperation::pause);
    QActionInitialization(this->stopAction.get(), "&Stop", "Stop current file", EventHandler::PlaybackOperation::stop);
    QActionInitialization(this->previousFileAction.get(), "&Previous File", "Play previous file", EventHandler::PlaybackOperation::previousFile);
    QActionInitialization(this->nextFileAction.get(), "&Next File", "Play next file", EventHandler::PlaybackOperation::nextFile);
    QActionInitialization(this->priorityUpAction.get(), "&Priority Up", "Increase current file priority", EventHandler::PlaybackOperation::priorityUp);
    QActionInitialization(this->priorityDownAction.get(), "&Priority Down", "Decrease current file priority", EventHandler::PlaybackOperation::priorityDown);
    QActionInitialization(this->volumeUpAction.get(), "&Volume Up", "Add 5% volume rate", EventHandler::PlaybackOperation::volumeUp);
    QActionInitialization(this->volumeDownAction.get(), "&Volume Down", "Down 5% volume rate", EventHandler::PlaybackOperation::volumeDown);
    QActionInitialization(this->repeatSingleFileAction.get(), "&Repeat Current File", "Repeat current file", EventHandler::PlaybackOperation::repeatSingleFile);
    QActionInitialization(this->repeatAllFilesAction.get(), "&Repeat All Files", "Repeat all files", EventHandler::PlaybackOperation::repeatAllFiles);
    QActionInitialization(this->clearPlaylistAction.get(), "&Clear Playlist", "Stop current file and clear playlist", EventHandler::PlaybackOperation::clearPlaylist);

    this->playlistTextEdit->setReadOnly(true);
    this->playlistTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    this->playlistTextEdit->viewport()->setCursor(Qt::ArrowCursor);
    this->playlistTextEdit->setFocusPolicy(Qt::NoFocus);

    this->durationBar->setRange(0, 100);
    this->durationBar->setTracking(false);
    this->durationBar->setOrientation(Qt::Horizontal);

    QSlider::connect(this->durationBar.get(), &QSlider::sliderMoved, std::bind_front(EventHandler::DurationBar::sliderMoved, this));

    this->currentDurationLabel->setText("  0:00:00  ");
    this->totalDurationLabel->setText("  0:00:00  ");

    QPushButtonInitialization(this->playButton.get(), "&Play", "Play current file", EventHandler::PlaybackOperation::play);
    QPushButtonInitialization(this->pauseButton.get(), "&Pause", "Pause current file", EventHandler::PlaybackOperation::pause);
    QPushButtonInitialization(this->stopButton.get(), "&Stop", "Stop current file", EventHandler::PlaybackOperation::stop);
    QPushButtonInitialization(this->previousFileButton.get(), "&Previous File", "Play previous file", EventHandler::PlaybackOperation::previousFile);
    QPushButtonInitialization(this->nextFileButton.get(), "&Next File", "Play next file", EventHandler::PlaybackOperation::nextFile);
    QPushButtonInitialization(this->priorityUpButton.get(), "&Priority Up", "Increase current file priority", EventHandler::PlaybackOperation::priorityUp);
    QPushButtonInitialization(this->priorityDownButton.get(), "&Priority Down", "Decrease current file priority", EventHandler::PlaybackOperation::priorityDown);
    QPushButtonInitialization(this->volumeUpButton.get(), "&Volume Up", "Add 5% volume rate", EventHandler::PlaybackOperation::volumeUp);
    QPushButtonInitialization(this->volumeDownButton.get(), "&Volume Down", "Down 5% volume rate", EventHandler::PlaybackOperation::volumeDown);
}

void AudioPlayer::initializeLayouts()
{
    this->setGeometry(250, 250, 1000, 500);
    this->setWindowTitle("Basic Audio Player");

    this->mainStatusBar->addPermanentWidget(this->volumeStatusLabel.get());
    this->mainStatusBar->addPermanentWidget(this->playStatusLabel.get());
    this->mainStatusBar->addPermanentWidget(this->currentFileLabel.get());

    this->fileMenu = this->mainMenuBar->addMenu("File");
    this->fileMenu->addAction(this->openFilesAction.get());
    this->fileMenu->addAction(this->openFolderAction.get());
    this->fileMenu->addSeparator();
    this->fileMenu->addAction(this->addFilesAction.get());
    this->fileMenu->addAction(this->removeFileAction.get());
    this->fileMenu->addSeparator();
    this->fileMenu->addAction(this->exitFileAction.get());

    this->playbackMenu = this->mainMenuBar->addMenu("Playback");
    this->playbackMenu->addAction(this->playAction.get());
    this->playbackMenu->addAction(this->pauseAction.get());
    this->playbackMenu->addAction(this->stopAction.get());
    this->playbackMenu->addSeparator();
    this->playbackMenu->addAction(this->previousFileAction.get());
    this->playbackMenu->addAction(this->nextFileAction.get());
    this->playbackMenu->addSeparator();
    this->playbackMenu->addAction(this->priorityUpAction.get());
    this->playbackMenu->addAction(this->priorityDownAction.get());
    this->playbackMenu->addSeparator();
    this->playbackMenu->addAction(this->volumeUpAction.get());
    this->playbackMenu->addAction(this->volumeDownAction.get());
    this->playbackMenu->addSeparator();
    this->playbackMenu->addAction(this->repeatSingleFileAction.get());
    this->playbackMenu->addAction(this->repeatAllFilesAction.get());
    this->playbackMenu->addSeparator();
    this->playbackMenu->addAction(this->clearPlaylistAction.get());

    this->playlistLayout->addWidget(this->playlistTextEdit.get());

    this->durationBarLayout->addWidget(this->currentDurationLabel.get());
    this->durationBarLayout->addWidget(this->durationBar.get());
    this->durationBarLayout->addWidget(this->totalDurationLabel.get());

    this->mainControlsLayout->addWidget(this->playButton.get());
    this->mainControlsLayout->addWidget(this->pauseButton.get());
    this->mainControlsLayout->addWidget(this->stopButton.get());
    this->mainControlsLayout->addWidget(this->volumeUpButton.get());
    this->mainControlsLayout->addWidget(this->volumeDownButton.get());

    this->playlistControlsLayout->addWidget(this->previousFileButton.get());
    this->playlistControlsLayout->addWidget(this->nextFileButton.get());
    this->playlistControlsLayout->addWidget(this->priorityUpButton.get());
    this->playlistControlsLayout->addWidget(this->priorityDownButton.get());

    this->mainLayout->addLayout(this->playlistLayout.get());
    this->mainLayout->addLayout(this->durationBarLayout.get());
    this->mainLayout->addLayout(this->mainControlsLayout.get());
    this->mainLayout->addLayout(this->playlistControlsLayout.get());

    this->mainCentralWidget->setLayout(this->mainLayout.get());
}

void AudioPlayer::closeEvent(QCloseEvent* event)
{
    const QString confirmTitle = "Confirm";
    const QString confirmMessage = "If you really want to terminate this program, then press \"Yes\"\t";

    const int confirmMessageBox = QMessageBox::question(this, confirmTitle, confirmMessage, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (confirmMessageBox == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

AudioPlayer::~AudioPlayer()
{
    this->playlistLayout.reset();
    this->durationBarLayout.reset();
    this->mainControlsLayout.reset();
    this->playlistControlsLayout.reset();
    this->mainLayout.reset();
}
