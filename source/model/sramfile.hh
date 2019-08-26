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

// $Id: sramfile.hh,v 1.10 2008/02/01 13:21:44 technoplaza Exp $

#ifndef LOZSRAME_SRAMFILE_HH_
#define LOZSRAME_SRAMFILE_HH_

#include <QString>

#include "exceptions/invalidsramfileexception.hh"

namespace lozsrame {
    /// offset of the arrow data
    const int ARROWS_OFFSET = 0x2;
    
    /// offset of link's bomb carrying capacity
    const int BOMBCAPACITY_OFFSET = 0x25;
    
    /// offset of bomb count data
    const int BOMBS_OFFSET = 0x1;
    
    /// offset of the candle data
    const int CANDLE_OFFSET = 0x4;
    
    /// starting offset of the checksum data
    const int CHECKSUM_OFFSET = 0x524;
    
    /// offset of the compass data
    const int COMPASS_OFFSET = 0x10;
    
    /// offset of the compass data for level 9
    const int COMPASS9_OFFSET = 0x12;
    
    /// offset of the heart container data
    const int HEARTCONTAINERS_OFFSET = 0x18;
    
    /// starting offset of the inventory data
    const int INVENTORY_DATA = 0x1A;
    
    /// size of the inventory data
    const int INVENTORY_DATA_SIZE = 0x28;
    
    /// offset of the keys
    const int KEYS_OFFSET = 0x17;
    
    /// starting offset of the map data
    const int MAP_DATA = 0x92;
    
    /// size of the map data
    const int MAP_DATA_SIZE = 0x180;
    
    /// offset of the level map data
    const int MAP_OFFSET = 0x11;
    
    /// offset of the level map data for level 9
    const int MAP9_OFFSET = 0x13;
    
    /// starting offset of the misc data
    const int MISC_DATA = 0x512;
    
    /// size of the misc data
    const int MISC_DATA_SIZE = 0x4;
    
    /// starting offset of the name data
    const int NAME_DATA = 0x2;
    
    /// size of the name data
    const int NAME_DATA_SIZE = 0x8;
    
    /// offset of the note data
    const int NOTE_OFFSET = 0xF;
    
    /// offset of the quest data
    const int QUEST_OFFSET = 0x9;
    
    /// offset of the play count
    const int PLAYCOUNT_OFFSET = 0x6;
    
    /// offset of the potion data
    const int POTION_OFFSET = 0x7;
    
    /// offset of the ring data
    const int RING_OFFSET = 0xB;
    
    /// offset of the rupees
    const int RUPEES_OFFSET = 0x16;
    
    /// size of the SRAM data
    const int SRAM_SIZE = 0x2000;
    
    /// offset of sword data
    const int SWORD_OFFSET = 0x0;
    
    /// offset of the triforce data
    const int TRIFORCE_OFFSET = 0x1A;
    
    /// the types of arrows
    enum sf_arrow {
        ARROW_NONE, ARROW_WOODEN, ARROW_SILVER
    };
    
    /// the types of candles
    enum sf_candle {
        CANDLE_NONE, CANDLE_BLUE, CANDLE_RED
    };
    
    /// the game items
    enum sf_item {
        ITEM_BOW = 3, ITEM_WHISTLE = 5, ITEM_BAIT, ITEM_WAND = 8, ITEM_RAFT,
        ITEM_BOOK, ITEM_LADDER = 12, ITEM_MAGICKEY, ITEM_POWERBRACELET,
        ITEM_BOOMERANG = 29, ITEM_MAGICBOOMERANG, ITEM_MAGICSHIELD
    };
    
    /// locations of the potion note
    enum sf_note {
        NOTE_OLDMAN, NOTE_LINK, NOTE_OLDWOMAN
    };
    
    /// the types of potions
    enum sf_potion {
        POTION_NONE, POTION_BLUE, POTION_RED
    };
    
    /// the types of quests
    enum sf_quest {
        QUEST_FIRST, QUEST_SECOND
    };
    
    /// the types of rings
    enum sf_ring {
        RING_NONE, RING_BLUE, RING_RED
    };
    
    /// the types of swords
    enum sf_sword {
        SWORD_NONE, SWORD_WOODEN, SWORD_WHITE, SWORD_MASTER
    };
    
    /**
     * A model of the SRAM data used by The Legend of Zelda.
     */
    class SRAMFile {
    private:
        int game;
        char sram[SRAM_SIZE];
        bool modified, valid[3];
        
        /**
         * Calculates the checksum for one of the games.
         *
         * @param game The game to checksum.
         *
         * @return The checksum.
         */
        quint16 checksum(int game) const;
        
        /**
         * Gets the checksum value for one of the games.
         *
         * @param game The game.
         *
         * @return The checksum.
         */
        quint16 getChecksum(int game) const;
        
        /**
         * Sets the checksum value for one of the games.
         *
         * @param game The game.
         * @param checksum The new checksum.
         */
        void setChecksum(int game, quint16 checksum);

    public:
        /**
         * Creates an SRAMFile object from an SRAM file.
         *
         * @param filename The SRAM filename.
         *
         * @return The new SRAMFile object.
         *
         * @throw InvalidSRAMFileException if the file is not a valid SRAM file.
         */
        SRAMFile(const QString &filename) throw(InvalidSRAMFileException);
        
        /**
         * Saves the SRAM data to a file.
         *
         * @param filename The file to save to.
         *
         * @return true if the save succeeded; false otherwise.
         */
        bool save(const QString &filename);
        
        /**
         * Gets the kind of arrows Link is carrying.
         *
         * @return The kind of arrows.
         */
        enum sf_arrow getArrows() const;
        
        /**
         * Sets the kind of arrows Link is carrying.
         *
         * @param arrows The new kind of arrows.
         */
        void setArrows(sf_arrow arrows);
        
        /**
         * Gets Link's bomb capacity.
         *
         * @return The bomb capacity.
         */
        int getBombCapacity() const;
        
        /**
         * Sets Link's bomb capacity.
         *
         * @param capacity The new bomb capacity.
         */
        void setBombCapacity(int capacity);
        
        /**
         * Gets the number of bombs Link is carrying.
         *
         * @return The bomb count.
         */
        int getBombs() const;
        
        /**
         * Sets the number of bombs Link is carrying.
         *
         * @param bombs The new bomb count.
         */
        void setBombs(int bombs);
        
        /**
         * Gets the candle Link is carrying.
         *
         * @return The candle.
         */
        enum sf_candle getCandle() const;
        
        /**
         * Sets the candle Link is carrying.
         *
         * @param candle The new candle.
         */
        void setCandle(enum sf_candle candle);
        
        /**
         * Checks if Link has the compass for a level.
         *
         * @param level The level to check.
         *
         * @return true if Link has the compass; false otherwise.
         */
        bool hasCompass(int level) const;
        
        /**
         * Sets whether Link has the compass for a level.
         *
         * @param level The level to check.
         * @param give true to give the compass; false to take away.
         */
        void setCompass(int level, bool give);
        
        /**
         * Gets the game slot being edited.
         *
         * @return The game slot (0 - 2).
         */
        int getGame() const;
        
        /**
         * Sets the game slot (0 - 2) being edited.
         *
         * @param game The new game.
         */
        void setGame(int game);
        
        /**
         * Gets the number of heart containers Link has.
         *
         * @return The number of heart containers.
         */
        int getHeartContainers() const;
        
        /**
         * Sets the number of heart containers Link has.
         *
         * @param containers The new number of heart containers.
         */
        void setHeartContainers(int containers);
        
        /**
         * Checks if Link has a particular item or not.
         *
         * @param item The item to check.
         *
         * @return true if Link has the item; false otherwise.
         */
        bool hasItem(sf_item item) const;
        
        /**
         * Sets whether Link has a particular item or not.
         *
         * @param item The item to set.
         * @param give true to give; false to take away.
         */
        void setItem(sf_item item, bool give);
        
        /**
         * Gets the number of keys Link has.
         *
         * @return The number of keys.
         */
        int getKeys() const;
        
        /**
         * Sets the number of keys Link has.
         *
         * @param keys The new number of keys.
         */
        void setKeys(int keys);
        
        /**
         * Checks if Link has the map for a level.
         *
         * @param level The level to check.
         *
         * @return true if Link has the map; false otherwise.
         */
        bool hasMap(int level) const;
        
        /**
         * Sets whether Link has the map for a level.
         *
         * @param level The level to set.
         * @param give true to give the map; false to take away.
         */
        void setMap(int level, bool give);
        
        /**
         * Checks if this SRAMFile has been modified or not.
         *
         * @return true if modified; false otherwise.
         */
        bool isModified() const;
        
        /**
         * Gets the name of the hero.
         *
         * @return The name.
         */
        QString getName() const;
        
        /**
         * Sets the name of the hero.
         *
         * @param name The new name.
         */
        void setName(const QString &name);
        
        /**
         * Gets the location of the potion note.
         *
         * @return The location.
         */
        enum sf_note getNote() const;
        
        /**
         * Sets the location of the potion note.
         *
         * @param note The new location.
         */
        void setNote(enum sf_note note);
        
        /**
         * Gets the game's play count.
         *
         * @return The play count.
         */
        int getPlayCount() const;
        
        /**
         * Sets the game's play count.
         *
         * @param count The new play count.
         */
        void setPlayCount(int count);
        
        /**
         * Gets the potion Link is carrying.
         *
         * @return The potion.
         */
        enum sf_potion getPotion() const;
        
        /**
         * Sets the potion Link is carrying.
         *
         * @param potion The new potion.
         */
        void setPotion(enum sf_potion potion);
        
        /**
         * Gets the quest Link is on.
         *
         * @return The current quest.
         */
        enum sf_quest getQuest() const;
        
        /**
         * Sets the quest Link is on.
         *
         * @param quest The new quest.
         */
        void setQuest(enum sf_quest quest);
        
        /**
         * Gets the ring Link is wearing.
         *
         * @return The ring.
         */
        enum sf_ring getRing() const;
        
        /**
         * Sets the ring Link is wearing.
         *
         * @param ring The new ring.
         */
        void setRing(enum sf_ring ring);
        
        /**
         * Gets how many rupees Link is carrying.
         *
         * @return The rupees.
         */
        int getRupees() const;
        
        /**
         * Sets how many rupees Link is carrying.
         *
         * @param rupees The new rupees.
         */
        void setRupees(int rupees);
        
        /**
         * Gets the sword Link has.
         *
         * @return The sword.
         */
        enum sf_sword getSword() const;
        
        /**
         * Sets the sword Link has.
         *
         * @param sword The new sword.
         */
        void setSword(enum sf_sword sword);
        
        /**
         * Checks if Link has a piece of the triforce.
         *
         * @param piece The piece to check.
         *
         * @return true if Link has the piece; false otherwise.
         */
        bool hasTriforce(int piece) const;
        
        /**
         * Sets whether Link has a piece of the triforce.
         *
         * @param piece The piece to set.
         * @param give true to give; false to take away.
         */
        void setTriforce(int piece, bool give);
        
        /**
         * Returns true if a game slot is valid; false otherwise.
         *
         * @param game The game slot to check.
         *
         * @return true if valid; false otherwise.
         */
        bool isValid(int game) const;
    };
    
    inline int SRAMFile::getGame() const
        {
            return game;
        }
    
    inline void SRAMFile::setGame(int game)
        {
            Q_ASSERT(isValid(game));
            
            this->game = game;
        }
        
    inline bool SRAMFile::isModified() const
        {
            return modified;
        }
        
    inline bool SRAMFile::isValid(int game) const
        {
            Q_ASSERT((game >= 0) && (game <= 2));
            
            return valid[game];
        }
}

#endif

