#pragma once
#include "include/gameideas.h"
#include "ui/ui_gameideas.h"

#include <QDialog>

namespace Ui {
class Genres;
}

class Genres : public QDialog
{
    Q_OBJECT
public:
    explicit Genres( Ui::GameIdeas *gameIdeasUi, GameIdeas *gameIdeasObject, QWidget *parent = nullptr);
    ~Genres();
private slots:
    void slotAddSelectedGenre();
    void slotDeleteSelectedGenres();
    void slotAddCustomGenre();
    void slotSearchCustomGenre(const QString &changedText);
    void slotConfirm();
    void slotCancel();
private:
    void addGenresFromGameIdeasUi();
    Ui::Genres *ui;
    Ui::GameIdeas *gameIdeasUi;
    GameIdeas *gameIdeasObject;
};

