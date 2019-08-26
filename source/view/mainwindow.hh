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

// $Id: mainwindow.hh,v 1.10 2008/02/01 13:21:44 technoplaza Exp $

#ifndef LOZSRAME_MAINWINDOW_HH_
#define LOZSRAME_MAINWINDOW_HH_

#include <QDragEnterEvent>
#include <QDropEvent>

#include "ui_mainwindow.h"

#include "model/sramfile.hh"

/// namespace used by all the classes and members of lozsrame
namespace lozsrame {
    /**
     * The main application window class.
     */
    class MainWindow : public QMainWindow {
        Q_OBJECT
        
    private:
        QString sramFile;
        QCheckBox *compassChecks[9], *mapChecks[9], *triforceChecks[8];
        Ui::MainWindow ui;
        SRAMFile *sram;
        bool ignoreSignals, open;
        
        /**
         * Closes the current SRAM file.
         *
         * @return true if closed; false otherwise.
         */
        bool closeSRAM();
        
        /**
         * Loads the SRAM data into the controls.
         */
        void loadSRAMData();
        
        /**
         * Opens an SRAM file for editing.
         *
         * @param filename The SRAM filename.
         */
        void openSRAM(const QString &filename);
        
        /**
         * Updates the UI based on the status of the program.
         */
        void updateUI();
        
        /**
         * Selects one of the arrow types.
         *
         * @param arrows The new arrow type.
         */
        void selectArrows(enum sf_arrow arrows);
        
        /**
         * Selects one of the candle types.
         *
         * @param candle The new candle type.
         */
        void selectCandle(enum sf_candle candle);
        
        /**
         * (De)selects one of the level compasses.
         *
         * @param level The level.
         * @param give true to give; false to take away.
         */
        void selectCompass(int level, bool give);
        
        /**
         * (De)selects one of the items.
         *
         * @param item The item.
         * @param give true to give; false to take away.
         */
        void selectItem(enum sf_item item, bool give);
        
        /**
         * (De)selects one of the level maps.
         *
         * @param level The level.
         * @param give true to give; false to take away.
         */
        void selectMap(int level, bool give);
        
        /**
         * Selects one of the note locations.
         *
         * @param note The new note position.
         */
        void selectNote(enum sf_note note);
        
        /**
         * Selects one of the potions.
         *
         * @param potion The new potion.
         */
        void selectPotion(enum sf_potion potion);
        
        /**
         * Selects one of the quests.
         *
         * @param quest The new quest.
         */
        void selectQuest(enum sf_quest quest);
        
        /**
         * Selects one of the rings.
         *
         * @param ring The new ring.
         */
        void selectRing(enum sf_ring ring);
        
        /**
         * Selects one of the swords.
         *
         * @param sword The new sword.
         */
        void selectSword(enum sf_sword sword);
        
        /**
         * (De)selects one of the triforce pieces.
         *
         * @param level The level.
         * @param give true to give; false to take away.
         */
        void selectTriforce(int level, bool give);
        
    private slots:
        /**
         * Called when the bait is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkBait_clicked(bool checked);
        
        /**
         * Called when the book is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkBook_clicked(bool checked);
        
        /**
         * Called when the boomerang is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkBoomerang_clicked(bool checked);
        
        /**
         * Called when the bow is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkBow_clicked(bool checked);
        
        /**
         * Called when the level 1 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass1_clicked(bool checked);
        
        /**
         * Called when the level 2 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass2_clicked(bool checked);
        
        /**
         * Called when the level 3 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass3_clicked(bool checked);
        
        /**
         * Called when the level 4 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass4_clicked(bool checked);
        
        /**
         * Called when the level 5 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass5_clicked(bool checked);
        
        /**
         * Called when the level 6 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass6_clicked(bool checked);
        
        /**
         * Called when the level 7 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass7_clicked(bool checked);
        
        /**
         * Called when the level 8 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass8_clicked(bool checked);
        
        /**
         * Called when the level 9 compass is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkCompass9_clicked(bool checked);
        
        /**
         * Called when the ladder is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkLadder_clicked(bool checked);
        
        /**
         * Called when the level 1 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap1_clicked(bool checked);
        
        /**
         * Called when the level 2 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap2_clicked(bool checked);
        
        /**
         * Called when the level 3 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap3_clicked(bool checked);
        
        /**
         * Called when the level 4 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap4_clicked(bool checked);
        
        /**
         * Called when the level 5 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap5_clicked(bool checked);
        
        /**
         * Called when the level 6 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap6_clicked(bool checked);
        
        /**
         * Called when the level 7 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap7_clicked(bool checked);
        
        /**
         * Called when the level 8 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap8_clicked(bool checked);
        
        /**
         * Called when the level 9 map is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMap9_clicked(bool checked);
        
        /**
         * Called when the magic boomerang is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMagicBoomerang_clicked(bool checked);
        
        /**
         * Called when the magic key is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMagicKey_clicked(bool checked);
        
        /**
         * Called when the magic shield is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkMagicShield_clicked(bool checked);
        
        /**
         * Called when the power bracelet is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkPowerBracelet_clicked(bool checked);
        
        /**
         * Called when the raft is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkRaft_clicked(bool checked);
        
        /**
         * Called when triforce piece 1 is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkTriforce1_clicked(bool checked);
        
        /**
         * Called when triforce piece 2 is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkTriforce2_clicked(bool checked);
        
        /**
         * Called when triforce piece 3 is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkTriforce3_clicked(bool checked);
        
        /**
         * Called when triforce piece 4 is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkTriforce4_clicked(bool checked);
        
        /**
         * Called when triforce piece 5 is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkTriforce5_clicked(bool checked);
        
        /**
         * Called when triforce piece 6 is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkTriforce6_clicked(bool checked);
        
        /**
         * Called when triforce piece 7 is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkTriforce7_clicked(bool checked);
        
        /**
         * Called when triforce piece 8 is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkTriforce8_clicked(bool checked);
        
        /**
         * Called when the wand is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkWand_clicked(bool checked);
        
        /**
         * Called when the whistle is (de)selected.
         *
         * @param checked true if selected; false if deselected.
         */
        void on_checkWhistle_clicked(bool checked);
        
        /**
         * Called when close from the file menu is selected.
         */
        void on_fileClose_triggered(bool);
        
        /**
         * Called when exit from the file menu is selected.
         */
        void on_fileExit_triggered(bool);
        
        /**
         * Called when open from the file menu is selected.
         */
        void on_fileOpen_triggered(bool);      
        
        /**
         * Called when save from the file menu is selected.
         */
        void on_fileSave_triggered(bool);
        
        /**
         * Called when save as from the file menu is selected.
         */
        void on_fileSaveAs_triggered(bool);
        
        /**
         * Called when about from the help menu is selected.
         */
        void on_helpAbout_triggered(bool);
        
        /**
         * Called when the hero's name is edited.
         *
         * @param text The new name.
         */
        void on_lineHerosName_textEdited(const QString &text);
        
        /**
         * Called when the arrows are removed.
         */
        void on_radioArrowsNone_clicked(bool);
        
        /**
         * Called when the silver arrows are selected.
         */
        void on_radioArrowsSilver_clicked(bool);
        
        /**
         * Called when the wooden arrows are selected.
         */
        void on_radioArrowsWooden_clicked(bool);
        
        /**
         * Called when the blue candle is selected.
         */
        void on_radioCandleBlue_clicked(bool);
        
        /**
         * Called when the candle is removed.
         */
        void on_radioCandleNone_clicked(bool);
        
        /**
         * Called when the red candle is selected.
         */
        void on_radioCandleRed_clicked(bool);
        
        /**
         * Called when the note is set to Link.
         */
        void on_radioNoteLink_clicked(bool);
        
        /**
         * Called when the note is set to the old man.
         */
        void on_radioNoteOldMan_clicked(bool);
        
        /**
         * Called when the note is set to the old woman.
         */
        void on_radioNoteOldWoman_clicked(bool);
        
        /**
         * Called when the first quest is selected.
         */
        void on_radioQuestFirst_clicked(bool);
        
        /**
         * Called when the second quest is selected.
         */
        void on_radioQuestSecond_clicked(bool);
        
        /**
         * Called when the blue potion is selected.
         */
        void on_radioPotionBlue_clicked(bool);
        
        /**
         * Called when the potion is removed.
         */
        void on_radioPotionNone_clicked(bool);
        
        /**
         * Called when the red potion is selected.
         */
        void on_radioPotionRed_clicked(bool);
        
        /**
         * Called when the blue ring is selected.
         */
        void on_radioRingBlue_clicked(bool);
        
        /**
         * Called when the ring is removed.
         */
        void on_radioRingNone_clicked(bool);
        
        /**
         * Called when the red ring is selected.
         */
        void on_radioRingRed_clicked(bool);
        
        /**
         * Called when the master sword is selected.
         */
        void on_radioSwordMaster_clicked(bool);
        
        /**
         * Called when the sword is removed.
         */
        void on_radioSwordNone_clicked(bool);
        
        /**
         * Called when the white sword is selected.
         */
        void on_radioSwordWhite_clicked(bool);
        
        /**
         * Called when the wooden sword is selected.
         */
        void on_radioSwordWooden_clicked(bool);
        
        /**
         * Called when the bomb count is changed.
         *
         * @param value The new bomb count.
         */
        void on_spinBombsCarrying_valueChanged(int value);
        
        /**
         * Called when the bomb capacity is changed.
         *
         * @param value The new bomb capacity.
         */
        void on_spinBombsCapacity_valueChanged(int value);
        
        /**
         * Called when the heart containers are changed.
         *
         * @param value The new heart containers.
         */
        void on_spinHeartContainers_valueChanged(int value);
        
        /**
         * Called when the keys are changed.
         *
         * @param value The new keys.
         */
        void on_spinKeys_valueChanged(int value);
        
        /**
         * Called when the play count is changed.
         *
         * @param value The new play count.
         */
        void on_spinPlayCount_valueChanged(int value);
        
        /**
         * Called when the rupee count is changed.
         *
         * @param value The new rupees.
         */
        void on_spinRupees_valueChanged(int value);
        
        /**
         * Selects which save slot to edit.
         *
         * @param game The new slot.
         */
        void selectGame(int game);
        
    protected:
        /**
         * Called when the window is being closed.
         *
         * @param event The triggering event.
         */
        void closeEvent(QCloseEvent *event);
        
        /**
         * Called when something is dragged over the window.
         *
         * @param event The triggering event.
         */
        void dragEnterEvent(QDragEnterEvent *event);
        
        /**
         * Called when something is dropped on the window.
         *
         * @param event The triggering event.
         */
        void dropEvent(QDropEvent *event);
        
        /**
         * Called when the window is shown.
         */
        void showEvent(QShowEvent *);
        
    public:
        /**
         * Creates a new MainWindow.
         */
        MainWindow();
    };
    
    inline void MainWindow::on_checkBait_clicked(bool checked)
        { selectItem(ITEM_BAIT, checked); }
    
    inline void MainWindow::on_checkBook_clicked(bool checked)
        { selectItem(ITEM_BOOK, checked); }
    
    inline void MainWindow::on_checkBoomerang_clicked(bool checked)
        { selectItem(ITEM_BOOMERANG, checked); }
    
    inline void MainWindow::on_checkBow_clicked(bool checked)
        { selectItem(ITEM_BOW, checked); }
        
    inline void MainWindow::on_checkCompass1_clicked(bool checked)
        { selectCompass(1, checked); }
        
    inline void MainWindow::on_checkCompass2_clicked(bool checked)
        { selectCompass(2, checked); }
        
    inline void MainWindow::on_checkCompass3_clicked(bool checked)
        { selectCompass(3, checked); }
        
    inline void MainWindow::on_checkCompass4_clicked(bool checked)
        { selectCompass(4, checked); }
        
    inline void MainWindow::on_checkCompass5_clicked(bool checked)
        { selectCompass(5, checked); }
        
    inline void MainWindow::on_checkCompass6_clicked(bool checked)
        { selectCompass(6, checked); }
        
    inline void MainWindow::on_checkCompass7_clicked(bool checked)
        { selectCompass(7, checked); }
        
    inline void MainWindow::on_checkCompass8_clicked(bool checked)
        { selectCompass(8, checked); }
        
    inline void MainWindow::on_checkCompass9_clicked(bool checked)
        { selectCompass(9, checked); }
        
    inline void MainWindow::on_checkLadder_clicked(bool checked)
        { selectItem(ITEM_LADDER, checked); }
        
    inline void MainWindow::on_checkMap1_clicked(bool checked)
        { selectMap(1, checked); }
        
    inline void MainWindow::on_checkMap2_clicked(bool checked)
        { selectMap(2, checked); }
        
    inline void MainWindow::on_checkMap3_clicked(bool checked)
        { selectMap(3, checked); }
        
    inline void MainWindow::on_checkMap4_clicked(bool checked)
        { selectMap(4, checked); }
        
    inline void MainWindow::on_checkMap5_clicked(bool checked)
        { selectMap(5, checked); }
        
    inline void MainWindow::on_checkMap6_clicked(bool checked)
        { selectMap(6, checked); }
        
    inline void MainWindow::on_checkMap7_clicked(bool checked)
        { selectMap(7, checked); }
        
    inline void MainWindow::on_checkMap8_clicked(bool checked)
        { selectMap(8, checked); }
        
    inline void MainWindow::on_checkMap9_clicked(bool checked)
        { selectMap(9, checked); }
        
    inline void MainWindow::on_checkMagicBoomerang_clicked(bool checked)
        { selectItem(ITEM_MAGICBOOMERANG, checked); }
        
    inline void MainWindow::on_checkMagicKey_clicked(bool checked)
        { selectItem(ITEM_MAGICKEY, checked); }
    
    inline void MainWindow::on_checkMagicShield_clicked(bool checked)
        { selectItem(ITEM_MAGICSHIELD, checked); }
    
    inline void MainWindow::on_checkPowerBracelet_clicked(bool checked)
        { selectItem(ITEM_POWERBRACELET, checked); }
        
    inline void MainWindow::on_checkRaft_clicked(bool checked)
        { selectItem(ITEM_RAFT, checked); }
        
    inline void MainWindow::on_checkTriforce1_clicked(bool checked)
        { selectTriforce(1, checked); }
    
    inline void MainWindow::on_checkTriforce2_clicked(bool checked)
        { selectTriforce(2, checked); }
    
    inline void MainWindow::on_checkTriforce3_clicked(bool checked)
        { selectTriforce(3, checked); }
    
    inline void MainWindow::on_checkTriforce4_clicked(bool checked)
        { selectTriforce(4, checked); }
    
    inline void MainWindow::on_checkTriforce5_clicked(bool checked)
        { selectTriforce(5, checked); }
    
    inline void MainWindow::on_checkTriforce6_clicked(bool checked)
        { selectTriforce(6, checked); }
    
    inline void MainWindow::on_checkTriforce7_clicked(bool checked)
        { selectTriforce(7, checked); }
    
    inline void MainWindow::on_checkTriforce8_clicked(bool checked)
        { selectTriforce(8, checked); }
        
    inline void MainWindow::on_checkWand_clicked(bool checked)
        { selectItem(ITEM_WAND, checked); }
        
    inline void MainWindow::on_checkWhistle_clicked(bool checked)
        { selectItem(ITEM_WHISTLE, checked); }
        
    inline void MainWindow::on_fileExit_triggered(bool)
        { close(); }
        
    inline void MainWindow::on_radioArrowsNone_clicked(bool)
        { selectArrows(ARROW_NONE); }
        
    inline void MainWindow::on_radioArrowsSilver_clicked(bool)
        { selectArrows(ARROW_SILVER); }
        
    inline void MainWindow::on_radioArrowsWooden_clicked(bool)
        { selectArrows(ARROW_WOODEN); }
        
    inline void MainWindow::on_radioCandleBlue_clicked(bool)
        { selectCandle(CANDLE_BLUE); }
        
    inline void MainWindow::on_radioCandleNone_clicked(bool)
        { selectCandle(CANDLE_NONE); }
        
    inline void MainWindow::on_radioCandleRed_clicked(bool)
        { selectCandle(CANDLE_RED); }
        
    inline void MainWindow::on_radioNoteLink_clicked(bool)
        { selectNote(NOTE_LINK); }
        
    inline void MainWindow::on_radioNoteOldMan_clicked(bool)
        { selectNote(NOTE_OLDMAN); }
        
    inline void MainWindow::on_radioNoteOldWoman_clicked(bool)
        { selectNote(NOTE_OLDWOMAN); }
        
    inline void MainWindow::on_radioQuestFirst_clicked(bool)
        { selectQuest(QUEST_FIRST); }
        
    inline void MainWindow::on_radioQuestSecond_clicked(bool)
        { selectQuest(QUEST_SECOND); }
        
    inline void MainWindow::on_radioPotionBlue_clicked(bool)
        { selectPotion(POTION_BLUE); }
        
    inline void MainWindow::on_radioPotionNone_clicked(bool)
        { selectPotion(POTION_NONE); }
        
    inline void MainWindow::on_radioPotionRed_clicked(bool)
        { selectPotion(POTION_RED); }
        
    inline void MainWindow::on_radioRingBlue_clicked(bool)
        { selectRing(RING_BLUE); }
        
    inline void MainWindow::on_radioRingNone_clicked(bool)
        { selectRing(RING_NONE); }
        
    inline void MainWindow::on_radioRingRed_clicked(bool)
        { selectRing(RING_RED); }
        
    inline void MainWindow::on_radioSwordMaster_clicked(bool)
        { selectSword(SWORD_MASTER); }
        
    inline void MainWindow::on_radioSwordNone_clicked(bool)
        { selectSword(SWORD_NONE); }
        
    inline void MainWindow::on_radioSwordWhite_clicked(bool)
        { selectSword(SWORD_WHITE); }
        
    inline void MainWindow::on_radioSwordWooden_clicked(bool)
        { selectSword(SWORD_WOODEN); }
}

#endif

