/*
 * lozsrame - Legend of Zelda SRAM Editor
 * Copyright (C) 2007-2008 emuWorks
 * http://games.technoplaza.net/
 *
 * This file is part of lozsrame.
 *
 * lozsrame is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * lozsrame is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * lozsrame; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <QActionGroup>
#include <QApplication>
#include <QButtonGroup>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QRegExpValidator>
#include <QScreen>
#include <QSignalMapper>
#include <QUrl>

#include "view/mainwindow.hh"

using namespace lozsrame;

MainWindow::MainWindow() : QMainWindow(), ignoreSignals(false), open(false) {
    // create widgets
    ui.setupUi(this);

    // setup button groups
    auto *gameActionGroup = new QActionGroup(this);
    gameActionGroup->addAction(ui.gameGame1);
    gameActionGroup->addAction(ui.gameGame2);
    gameActionGroup->addAction(ui.gameGame3);
    gameActionGroup->setExclusive(true);

    auto *questButtonGroup = new QButtonGroup(this);
    questButtonGroup->addButton(ui.radioQuestFirst);
    questButtonGroup->addButton(ui.radioQuestSecond);
    questButtonGroup->setExclusive(true);

    auto *swordButtonGroup = new QButtonGroup(this);
    swordButtonGroup->addButton(ui.radioSwordNone);
    swordButtonGroup->addButton(ui.radioSwordWooden);
    swordButtonGroup->addButton(ui.radioSwordWhite);
    swordButtonGroup->addButton(ui.radioSwordMaster);
    swordButtonGroup->setExclusive(true);

    auto *arrowsButtonGroup = new QButtonGroup(this);
    arrowsButtonGroup->addButton(ui.radioArrowsNone);
    arrowsButtonGroup->addButton(ui.radioArrowsWooden);
    arrowsButtonGroup->addButton(ui.radioArrowsSilver);
    arrowsButtonGroup->setExclusive(true);

    auto *candleButtonGroup = new QButtonGroup(this);
    candleButtonGroup->addButton(ui.radioCandleNone);
    candleButtonGroup->addButton(ui.radioCandleBlue);
    candleButtonGroup->addButton(ui.radioCandleRed);
    candleButtonGroup->setExclusive(true);

    auto *potionButtonGroup = new QButtonGroup(this);
    potionButtonGroup->addButton(ui.radioPotionNone);
    potionButtonGroup->addButton(ui.radioPotionBlue);
    potionButtonGroup->addButton(ui.radioPotionRed);
    potionButtonGroup->setExclusive(true);

    auto *ringButtonGroup = new QButtonGroup(this);
    ringButtonGroup->addButton(ui.radioRingNone);
    ringButtonGroup->addButton(ui.radioRingBlue);
    ringButtonGroup->addButton(ui.radioRingRed);
    ringButtonGroup->setExclusive(true);

    auto *noteButtonGroup = new QButtonGroup(this);
    noteButtonGroup->addButton(ui.radioNoteOldMan);
    noteButtonGroup->addButton(ui.radioNoteLink);
    noteButtonGroup->addButton(ui.radioNoteOldWoman);
    noteButtonGroup->setExclusive(true);

    // create button group arrays
    compassChecks[0] = ui.checkCompass1;
    compassChecks[1] = ui.checkCompass2;
    compassChecks[2] = ui.checkCompass3;
    compassChecks[3] = ui.checkCompass4;
    compassChecks[4] = ui.checkCompass5;
    compassChecks[5] = ui.checkCompass6;
    compassChecks[6] = ui.checkCompass7;
    compassChecks[7] = ui.checkCompass8;
    compassChecks[8] = ui.checkCompass9;

    mapChecks[0] = ui.checkMap1;
    mapChecks[1] = ui.checkMap2;
    mapChecks[2] = ui.checkMap3;
    mapChecks[3] = ui.checkMap4;
    mapChecks[4] = ui.checkMap5;
    mapChecks[5] = ui.checkMap6;
    mapChecks[6] = ui.checkMap7;
    mapChecks[7] = ui.checkMap8;
    mapChecks[8] = ui.checkMap9;

    triforceChecks[0] = ui.checkTriforce1;
    triforceChecks[1] = ui.checkTriforce2;
    triforceChecks[2] = ui.checkTriforce3;
    triforceChecks[3] = ui.checkTriforce4;
    triforceChecks[4] = ui.checkTriforce5;
    triforceChecks[5] = ui.checkTriforce6;
    triforceChecks[6] = ui.checkTriforce7;
    triforceChecks[7] = ui.checkTriforce8;

    // setup name validator
    auto *nameValidator =
        new QRegExpValidator(QRegExp("[A-Z0-9 ,!'&\\.\"\\?_]{0,8}"), this);
    ui.lineHerosName->setValidator(nameValidator);

    // setup connections for the game menu
    auto *gameMapper = new QSignalMapper(this);

    gameMapper->setMapping(ui.gameGame1, 0);
    gameMapper->setMapping(ui.gameGame2, 1);
    gameMapper->setMapping(ui.gameGame3, 2);

    connect(gameMapper, SIGNAL(mapped(int)), this, SLOT(selectGame(int)));

    connect(ui.gameGame1, SIGNAL(triggered(bool)), gameMapper, SLOT(map()));
    connect(ui.gameGame2, SIGNAL(triggered(bool)), gameMapper, SLOT(map()));
    connect(ui.gameGame3, SIGNAL(triggered(bool)), gameMapper, SLOT(map()));

    updateUI();
}

auto MainWindow::closeSRAM() -> bool {
    Q_ASSERT(open);

    if (sram->isModified()) {
        int choice = QMessageBox::question(
            this, tr("Warning: Unsaved Changes"), tr("Save changes?"),
            QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);

        if (choice == QMessageBox::Yes) {
            ui.fileSave->trigger();

            // if the save failed, abort
            if (sram->isModified()) {
                return false;
            }
        } else if (choice == QMessageBox::Cancel) {
            return false;
        }
    }

    delete sram;
    open = false;

    return true;
}

void MainWindow::loadSRAMData() {
    Q_ASSERT(open);

    ignoreSignals = true;

    // load the hero's name
    ui.lineHerosName->setText(sram->getName());

    // load the play count
    ui.spinPlayCount->setValue(sram->getPlayCount());

    // load the quest number
    switch (sram->getQuest()) {
        case QUEST_FIRST:
            ui.radioQuestFirst->setChecked(true);
            break;
        case QUEST_SECOND:
            ui.radioQuestSecond->setChecked(true);
            break;
    }

    // load the sword type
    switch (sram->getSword()) {
        case SWORD_NONE:
            ui.radioSwordNone->setChecked(true);
            break;
        case SWORD_WOODEN:
            ui.radioSwordWooden->setChecked(true);
            break;
        case SWORD_WHITE:
            ui.radioSwordWhite->setChecked(true);
            break;
        case SWORD_MASTER:
            ui.radioSwordMaster->setChecked(true);
            break;
    }

    // load the arrow type
    switch (sram->getArrows()) {
        case ARROW_NONE:
            ui.radioArrowsNone->setChecked(true);
            break;
        case ARROW_WOODEN:
            ui.radioArrowsWooden->setChecked(true);
            break;
        case ARROW_SILVER:
            ui.radioArrowsSilver->setChecked(true);
            break;
    }

    // load the candle type
    switch (sram->getCandle()) {
        case CANDLE_NONE:
            ui.radioCandleNone->setChecked(true);
            break;
        case CANDLE_BLUE:
            ui.radioCandleBlue->setChecked(true);
            break;
        case CANDLE_RED:
            ui.radioCandleRed->setChecked(true);
            break;
    }

    // load the potion type
    switch (sram->getPotion()) {
        case POTION_NONE:
            ui.radioPotionNone->setChecked(true);
            break;
        case POTION_BLUE:
            ui.radioPotionBlue->setChecked(true);
            break;
        case POTION_RED:
            ui.radioPotionRed->setChecked(true);
            break;
    }

    // load the ring type
    switch (sram->getRing()) {
        case RING_NONE:
            ui.radioRingNone->setChecked(true);
            break;
        case RING_BLUE:
            ui.radioRingBlue->setChecked(true);
            break;
        case RING_RED:
            ui.radioRingRed->setChecked(true);
            break;
    }

    // load inventory data
    ui.checkBoomerang->setChecked(sram->hasItem(ITEM_BOOMERANG));
    ui.checkBow->setChecked(sram->hasItem(ITEM_BOW));
    ui.checkMagicBoomerang->setChecked(sram->hasItem(ITEM_MAGICBOOMERANG));
    ui.checkRaft->setChecked(sram->hasItem(ITEM_RAFT));
    ui.checkLadder->setChecked(sram->hasItem(ITEM_LADDER));
    ui.checkWhistle->setChecked(sram->hasItem(ITEM_WHISTLE));
    ui.checkWand->setChecked(sram->hasItem(ITEM_WAND));
    ui.checkBook->setChecked(sram->hasItem(ITEM_BOOK));
    ui.checkMagicKey->setChecked(sram->hasItem(ITEM_MAGICKEY));
    ui.checkMagicShield->setChecked(sram->hasItem(ITEM_MAGICSHIELD));
    ui.checkPowerBracelet->setChecked(sram->hasItem(ITEM_POWERBRACELET));
    ui.checkBait->setChecked(sram->hasItem(ITEM_BAIT));

    // load dungeon data
    for (int i = 1; i < 10; ++i) {
        compassChecks[i - 1]->setChecked(sram->hasCompass(i));
        mapChecks[i - 1]->setChecked(sram->hasMap(i));

        if (i < 9) {
            triforceChecks[i - 1]->setChecked(sram->hasTriforce(i));
        }
    }

    // load potion note data
    switch (sram->getNote()) {
        case NOTE_OLDMAN:
            ui.radioNoteOldMan->setChecked(true);
            break;
        case NOTE_LINK:
            ui.radioNoteLink->setChecked(true);
            break;
        case NOTE_OLDWOMAN:
            ui.radioNoteOldWoman->setChecked(true);
            break;
    }

    // load treasure and bomb data
    ui.spinRupees->setValue(sram->getRupees());
    ui.spinKeys->setValue(sram->getKeys());
    ui.spinHeartContainers->setValue(sram->getHeartContainers());
    ui.spinBombsCarrying->setValue(sram->getBombs());
    ui.spinBombsCapacity->setValue(sram->getBombCapacity());

    ignoreSignals = false;
}

void MainWindow::openSRAM(const QString &filename) {
    Q_ASSERT(!open);

    try {
        sram     = new SRAMFile(filename);
        sramFile = filename;
        open     = true;

        loadSRAMData();
        updateUI();
    } catch (InvalidSRAMFileException &e) {
        QString temp;

        if (e.getError() == ISFE_INVALIDSIZE) {
            temp = tr("Invalid SRAM File Size");
        } else if (e.getError() == ISFE_NOVALIDGAMES) {
            temp = tr("No Save Games Found");
        }

        QMessageBox::warning(this, tr("Unable to Open SRAM File"), temp,
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void MainWindow::selectArrows(enum sf_arrow arrows) {
    Q_ASSERT(open);

    sram->setArrows(arrows);
    updateUI();
}

void MainWindow::selectCandle(enum sf_candle candle) {
    Q_ASSERT(open);

    sram->setCandle(candle);
    updateUI();
}

void MainWindow::selectCompass(int level, bool give) {
    Q_ASSERT(open);
    Q_ASSERT((level >= 1) && (level <= 9));

    sram->setCompass(level, give);
    updateUI();
}

void MainWindow::selectItem(enum sf_item item, bool give) {
    Q_ASSERT(open);

    sram->setItem(item, give);
    updateUI();
}

void MainWindow::selectMap(int level, bool give) {
    Q_ASSERT(open);
    Q_ASSERT((level >= 1) && (level <= 9));

    sram->setMap(level, give);
    updateUI();
}

void MainWindow::selectNote(enum sf_note note) {
    Q_ASSERT(open);

    sram->setNote(note);
    updateUI();
}

void MainWindow::selectQuest(enum sf_quest quest) {
    Q_ASSERT(open);

    sram->setQuest(quest);
    updateUI();
}

void MainWindow::selectPotion(enum sf_potion potion) {
    Q_ASSERT(open);

    sram->setPotion(potion);
    updateUI();
}

void MainWindow::selectRing(enum sf_ring ring) {
    Q_ASSERT(open);

    sram->setRing(ring);
    updateUI();
}

void MainWindow::selectSword(enum sf_sword sword) {
    Q_ASSERT(open);

    sram->setSword(sword);
    updateUI();
}

void MainWindow::selectTriforce(int level, bool give) {
    Q_ASSERT(open);
    Q_ASSERT((level >= 1) && (level <= 8));

    sram->setTriforce(level, give);
    updateUI();
}

void MainWindow::updateUI() {
    ui.fileClose->setEnabled(open);
    ui.fileSave->setEnabled(open && sram->isModified());
    ui.fileSaveAs->setEnabled(open);

    ui.gameGame1->setEnabled(open && sram->isValid(0));
    ui.gameGame2->setEnabled(open && sram->isValid(1));
    ui.gameGame3->setEnabled(open && sram->isValid(2));

    ui.centralwidget->setVisible(open);

    if (open) {
        switch (sram->getGame()) {
            case 0:
                ui.gameGame1->setChecked(true);
                break;
            case 1:
                ui.gameGame2->setChecked(true);
                break;
            case 2:
                ui.gameGame3->setChecked(true);
                break;

            default:
                // we must never get here!
                Q_ASSERT(false);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (open) {
        event->setAccepted(closeSRAM());
    } else {
        event->accept();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    QList<QUrl> urls     = event->mimeData()->urls();
    QString     filename = urls.first().toLocalFile();

    // uri could be something else
    if (filename.isEmpty()) {
        return;
    }

    if (open && !closeSRAM()) {
        return;
    }

    openSRAM(filename);
}

void MainWindow::on_fileClose_triggered(bool) {
    Q_ASSERT(open);

    if (closeSRAM()) {
        updateUI();
    }
}

void MainWindow::on_fileOpen_triggered(bool) {
    if (open && !closeSRAM()) {
        return;
    }

    QString filename =
        QFileDialog::getOpenFileName(this, tr("Open Legend of Zelda SRAM File"),
                                     "", tr("SRAM Files (*.sav)"));

    if (filename.isNull()) {
        return;
    }

    openSRAM(filename);
}

void MainWindow::on_fileSave_triggered(bool) {
    Q_ASSERT(open);

    if (!sram->save(sramFile)) {
        QMessageBox::warning(this, tr("Unable to Save SRAM File"),
                             tr("An I/O error occurred while trying to save."),
                             QMessageBox::Ok, QMessageBox::NoButton);
    }

    updateUI();
}

void MainWindow::on_fileSaveAs_triggered(bool) {
    Q_ASSERT(open);

    QString temp = QFileDialog::getSaveFileName(this, tr("Save SRAM File As"),
                                                "", tr("SRAM Files (*.sav)"));

    if (temp.isNull()) {
        return;
    }

    sramFile = temp;

    ui.fileSave->trigger();
}

void MainWindow::on_helpAbout_triggered(bool) {
    QMessageBox::about(this, tr("About lozsrame"),
                       tr("Legend of Zelda SRAM Editor Version 0.90a\n"
                          "Copyright (C) 2007-2008 emuWorks\n"
                          "http://games.technoplaza.net/"));
}

void MainWindow::on_lineHerosName_textEdited(const QString &text) {
    Q_ASSERT(open);

    sram->setName(text);
    updateUI();
}

void MainWindow::on_spinBombsCarrying_valueChanged(int value) {
    Q_ASSERT(open);

    if (!ignoreSignals) {
        sram->setBombs(value);
        updateUI();
    }
}

void MainWindow::on_spinBombsCapacity_valueChanged(int value) {
    Q_ASSERT(open);

    if (!ignoreSignals) {
        sram->setBombCapacity(value);
        updateUI();
    }
}

void MainWindow::on_spinHeartContainers_valueChanged(int value) {
    Q_ASSERT(open);

    if (!ignoreSignals) {
        sram->setHeartContainers(value);
        updateUI();
    }
}

void MainWindow::on_spinKeys_valueChanged(int value) {
    Q_ASSERT(open);

    if (!ignoreSignals) {
        sram->setKeys(value);
        updateUI();
    }
}

void MainWindow::on_spinPlayCount_valueChanged(int value) {
    Q_ASSERT(open);

    if (!ignoreSignals) {
        sram->setPlayCount(value);
        updateUI();
    }
}

void MainWindow::on_spinRupees_valueChanged(int value) {
    Q_ASSERT(open);

    if (!ignoreSignals) {
        sram->setRupees(value);
        updateUI();
    }
}

void MainWindow::selectGame(int game) {
    Q_ASSERT(open);

    sram->setGame(game);
    loadSRAMData();
    updateUI();
}

void MainWindow::showEvent(QShowEvent *) {
    static bool centered = false;

    // center on screen the first time we're displayed
    if (!centered) {
        QRect screenRect = QApplication::primaryScreen()->availableGeometry();
        QRect windowRect = frameGeometry();

        move(screenRect.width() / 2 - windowRect.width() / 2,
             screenRect.height() / 2 - windowRect.height() / 2);

        centered = true;
    }
}
