///////////////////////////
///// AudioPlayer.hpp /////
///////////////////////////

#pragma once
#include "Libraries.hpp"
#include "EventHandler.hpp"

class AudioPlayer : public QMainWindow
{
public:
    std::unique_ptr<QWidget> mainCentralWidget = std::make_unique<QWidget>();

    QMenuBar* mainMenuBar = nullptr;
    QStatusBar* mainStatusBar = nullptr;

    std::unique_ptr<QMediaPlaylist> mediaPlaylist = std::make_unique<QMediaPlaylist>();
    std::unique_ptr<QMediaPlayer> mediaPlayer = std::make_unique<QMediaPlayer>();

    std::unique_ptr<QLabel> volumeStatusLabel = std::make_unique<QLabel>();
    std::unique_ptr<QLabel> playStatusLabel = std::make_unique<QLabel>();
    std::unique_ptr<QLabel> currentFileLabel = std::make_unique<QLabel>();

    std::unique_ptr<QAction> openFilesAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> openFolderAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> addFilesAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> removeFileAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> exitFileAction = std::make_unique<QAction>();

    std::unique_ptr<QAction> playAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> pauseAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> stopAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> previousFileAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> nextFileAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> priorityUpAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> priorityDownAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> volumeUpAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> volumeDownAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> repeatSingleFileAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> repeatAllFilesAction = std::make_unique<QAction>();
    std::unique_ptr<QAction> clearPlaylistAction = std::make_unique<QAction>();

    std::unique_ptr<QTextEdit> playlistTextEdit = std::make_unique<QTextEdit>();

    std::unique_ptr<QSlider> durationBar = std::make_unique<QSlider>();

    std::unique_ptr<QLabel> currentDurationLabel = std::make_unique<QLabel>();
    std::unique_ptr<QLabel> totalDurationLabel = std::make_unique<QLabel>();

    std::unique_ptr<QPushButton> playButton = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> pauseButton = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> stopButton = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> previousFileButton = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> nextFileButton = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> priorityUpButton = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> priorityDownButton = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> volumeUpButton = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> volumeDownButton = std::make_unique<QPushButton>();

    QMenu* fileMenu = nullptr;
    QMenu* playbackMenu = nullptr;

    std::unique_ptr<QHBoxLayout> playlistLayout = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> durationBarLayout = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> mainControlsLayout = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QHBoxLayout> playlistControlsLayout = std::make_unique<QHBoxLayout>();
    std::unique_ptr<QVBoxLayout> mainLayout = std::make_unique<QVBoxLayout>();

public:
    AudioPlayer();

public:
    void initializeVariables();
    void initializeLayouts();

public:
    void closeEvent(QCloseEvent* event) final;

public:
    ~AudioPlayer() override;

    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;
    AudioPlayer(AudioPlayer&&) = delete;
    AudioPlayer& operator=(AudioPlayer&&) = delete;
};
