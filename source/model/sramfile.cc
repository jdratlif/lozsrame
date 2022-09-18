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

#include <cstring>
#include <fstream>

#include <QtCore/qendian.h>

#include "model/sramfile.hh"

using namespace lozsrame;

SRAMFile::SRAMFile(const QString &filename) throw(InvalidSRAMFileException)
    : modified(false) {
    std::ifstream file(filename.toLatin1().data(),
                       std::ios_base::in | std::ios_base::binary);

    if (!file) {
        throw InvalidSRAMFileException(ISFE_FILENOTFOUND);
    }

    file.seekg(0, std::ios_base::end);

    if (file.tellg() != static_cast<std::streampos>(SRAM_SIZE)) {
        throw InvalidSRAMFileException(ISFE_INVALIDSIZE);
    }

    file.seekg(0, std::ios_base::beg);
    file.read(sram, SRAM_SIZE);
    file.close();

    // checksum to determine valid games
    std::memset(valid, 0, 3 * sizeof(bool));

    bool    foundValid = false;
    QString emptyName("        ");

    for (int game = 2; game >= 0; --game) {
        if (checksum(game) == getChecksum(game)) {
            valid[game] = true;
            setGame(game);

            if (getName() == emptyName) {
                // empty slot
                valid[game] = false;
            } else {
                foundValid = true;
            }
        }
    }

    if (!foundValid) {
        throw InvalidSRAMFileException(ISFE_NOVALIDGAMES);
    }
}

/*
    $A3E1:A9 00     LDA #$00        ; set checksum counters to 0
    $A3E3:85 CE     STA $00CE
    $A3E5:85 CF     STA $00CF

    $A3E7:A0 07     LDY #$07        ; y = 7 (name counter)

    $A3E9:B1 C4     LDA ($C4),Y     ; load name byte
    $A3EB:20 2B A4  JSR $A42B       ; add to checksum

    $A3EE:88        DEY             ; --y
    $A3EF:10 F8     BPL $A3E9       ; loop for all name bytes

    $A3F1:A0 27     LDY #$27        ; y = 27 (inventory counter)

    $A3F3:B1 C0     LDA ($C0),Y     ; load inventory byte
    $A3F5:20 2B A4  JSR $A42B       ; add to checksum

    $A3F8:88        DEY             ; --y
    $A3F9:10 F8     BPL $A3F3       ; loop for all inventory bytes

    $A3FB:A9 80     LDA #$80        ; c1 = 80h
    $A3FD:85 C1     STA $00C1
    $A3FF:A9 01     LDA #$01        ; c0 = 1h (map counter)
    $A401:85 C0     STA $00C0

    $A403:A0 00     LDY #$00        ; y = 0 (needed for addressing mode)

    $A405:B1 C2     LDA ($C2),Y     ; load map byte
    $A407:20 2B A4  JSR $A42B       ; add to checksum

    $A40A:E6 C2     INC $00C2       ; loop for all map bytes
    $A40C:D0 02     BNE $A410
    $A40E:E6 C3     INC $00C3
    $A410:C6 C1     DEC $00C1
    $A412:D0 F1     BNE $A405

    $A414:C6 C0     DEC $00C0
    $A416:A5 C0     LDA $00C0
    $A418:10 EB     BPL $A405

    $A41A:B1 C6     LDA ($C6),Y     ; load misc byte
    $A41C:20 2B A4  JSR $A42B       ; add to checksum

    $A41F:B1 C8     LDA ($C8),Y
    $A421:20 2B A4  JSR $A42B

    $A424:B1 CA     LDA ($CA),Y
    $A426:20 2B A4  JSR $A42B

    $A429:B1 CC     LDA ($CC),Y

    $A42B:18        CLC             ; 16-bit big endian add
    $A42C:65 CF     ADC $00CF
    $A42E:85 CF     STA $00CF
    $A430:A5 CE     LDA $00CE
    $A432:69 00     ADC #$00
    $A434:85 CE     STA $00CE
    $A436:60        RTS
*/
quint16 SRAMFile::checksum(int game) const {
    Q_ASSERT((game >= 0) && (game < 3));

    quint16 checksum = 0;

    // name data
    for (int i = 0; i < NAME_DATA_SIZE; ++i) {
        checksum += static_cast<unsigned char>(
            sram[NAME_DATA + i + (game * NAME_DATA_SIZE)]);
    }

    // inventory data
    for (int i = 0; i < INVENTORY_DATA_SIZE; ++i) {
        checksum += static_cast<unsigned char>(
            sram[INVENTORY_DATA + i + (game * INVENTORY_DATA_SIZE)]);
    }

    // map data
    for (int i = 0; i < MAP_DATA_SIZE; ++i) {
        checksum += static_cast<unsigned char>(
            sram[MAP_DATA + i + (game * MAP_DATA_SIZE)]);
    }

    // misc data (0x512, 0x515, 0x518, 0x51B)
    for (int i = 0; i < MISC_DATA_SIZE; ++i) {
        checksum +=
            static_cast<unsigned char>(sram[MISC_DATA + (i * 3) + game]);
    }

    return checksum;
}

bool SRAMFile::save(const QString &filename) {
    for (int i = 0; i < 3; ++i) {
        if (isValid(i)) {
            setChecksum(i, checksum(i));
        }
    }

    std::ofstream file(filename.toLatin1().data(),
                       std::ios_base::out | std::ios_base::binary);

    if (!file) {
        return false;
    }

    file.write(sram, SRAM_SIZE);

    if (file.tellp() != static_cast<std::streampos>(SRAM_SIZE)) {
        return false;
    }

    file.close();
    modified = false;

    return true;
}

enum sf_arrow SRAMFile::getArrows() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return static_cast<enum sf_arrow>(ptr[ARROWS_OFFSET]);
}

void SRAMFile::setArrows(sf_arrow arrows) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[ARROWS_OFFSET] = arrows;
    modified           = true;
}

int SRAMFile::getBombCapacity() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return ptr[BOMBCAPACITY_OFFSET];
}

void SRAMFile::setBombCapacity(int capacity) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((capacity >= 0) && (capacity <= 16));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[BOMBCAPACITY_OFFSET] = capacity;
    modified                 = true;
}

int SRAMFile::getBombs() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return ptr[BOMBS_OFFSET];
}

void SRAMFile::setBombs(int bombs) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((bombs >= 0) && (bombs <= 16));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[BOMBS_OFFSET] = bombs;
    modified          = true;
}

enum sf_candle SRAMFile::getCandle() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return static_cast<enum sf_candle>(ptr[CANDLE_OFFSET]);
}

void SRAMFile::setCandle(enum sf_candle candle) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[CANDLE_OFFSET] = candle;
    modified           = true;
}

quint16 SRAMFile::getChecksum(int game) const {
    Q_ASSERT((game >= 0) && (game < 3));

    const quint16 *ptr =
        reinterpret_cast<const quint16 *>(sram + CHECKSUM_OFFSET);

    return qFromBigEndian(ptr[game]);
}

void SRAMFile::setChecksum(int game, quint16 checksum) {
    Q_ASSERT((game >= 0) && (game < 3));

    quint16 *ptr = reinterpret_cast<quint16 *>(sram + CHECKSUM_OFFSET);

    ptr[game] = qToBigEndian(checksum);
}

bool SRAMFile::hasCompass(int level) const {
    Q_ASSERT(isValid(game));
    Q_ASSERT((level >= 1) && (level <= 9));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    if (level == 9) {
        return (ptr[COMPASS9_OFFSET] == 1);
    }

    return (ptr[COMPASS_OFFSET] & (1 << (level - 1)));
}

void SRAMFile::setCompass(int level, bool give) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((level >= 1) && (level <= 9));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    if (level == 9) {
        ptr[COMPASS9_OFFSET] = (give ? 1 : 0);
    }

    if (give) {
        ptr[COMPASS_OFFSET] |= (1 << (level - 1));
    } else {
        ptr[COMPASS_OFFSET] &= ~(1 << (level - 1));
    }

    modified = true;
}

int SRAMFile::getHeartContainers() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return ((static_cast<unsigned char>(ptr[HEARTCONTAINERS_OFFSET]) >> 4) + 1);
}

void SRAMFile::setHeartContainers(int containers) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((containers > 0) && (containers <= 16));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[HEARTCONTAINERS_OFFSET] &= 0x0F;
    ptr[HEARTCONTAINERS_OFFSET] |= ((containers - 1) << 4);
    modified = true;
}

bool SRAMFile::hasItem(enum sf_item item) const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return (ptr[item] == 1);
}

void SRAMFile::setItem(enum sf_item item, bool give) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[item] = (give ? 1 : 0);
    modified  = true;
}

int SRAMFile::getKeys() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return ptr[KEYS_OFFSET];
}

void SRAMFile::setKeys(int keys) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((keys >= 0) && (keys <= 99));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[KEYS_OFFSET] = keys;
    modified         = true;
}

bool SRAMFile::hasMap(int level) const {
    Q_ASSERT(isValid(game));
    Q_ASSERT((level >= 1) && (level <= 9));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    if (level == 9) {
        return (ptr[MAP9_OFFSET] == 1);
    }

    return (ptr[MAP_OFFSET] & (1 << (level - 1)));
}

void SRAMFile::setMap(int level, bool give) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((level >= 1) && (level <= 9));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    if (level == 9) {
        ptr[MAP9_OFFSET] = (give ? 1 : 0);
    }

    if (give) {
        ptr[MAP_OFFSET] |= (1 << (level - 1));
    } else {
        ptr[MAP_OFFSET] &= ~(1 << (level - 1));
    }

    modified = true;
}

QString SRAMFile::getName() const {
    Q_ASSERT(isValid(game));

    QString     name;
    const char *ptr = (sram + NAME_DATA + (game * NAME_DATA_SIZE));

    for (int i = 0; i < NAME_DATA_SIZE; ++i) {
        char ch = ptr[i];

        if ((ch >= 0) && (ch <= 9)) {
            name += ('0' + ch);
        } else if ((ch >= 0xA) && (ch <= 0x23)) {
            name += ('A' + ch - 0xA);
        } else if (ch == 0x24) {
            name += ' ';
        } else if (ch == 0x28) {
            name += ',';
        } else if (ch == 0x29) {
            name += '!';
        } else if (ch == 0x2A) {
            name += '\'';
        } else if (ch == 0x2B) {
            name += '&';
        } else if (ch == 0x2C) {
            name += '.';
        } else if (ch == 0x2D) {
            name += '\"';
        } else if (ch == 0x2E) {
            name += '?';
        } else if (ch == 0x2F) {
            name += '_';
        } else {
            // invalid character
            Q_ASSERT(false);
        }
    }

    return name;
}

void SRAMFile::setName(const QString &name) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + NAME_DATA + (game * NAME_DATA_SIZE));

    for (int count = 0; count < 8; ++count) {
        if (name.length() > count) {
            char ch = name[count].toLatin1();

            if ((ch >= '0') && (ch <= '9')) {
                ptr[count] = (ch - '0');
            } else if ((ch >= 'A') && (ch <= 'Z')) {
                ptr[count] = (ch - 'A' + 0xA);
            } else if (ch == ' ') {
                ptr[count] = 0x24;
            } else if (ch == ',') {
                ptr[count] = 0x28;
            } else if (ch == '!') {
                ptr[count] = 0x29;
            } else if (ch == '\'') {
                ptr[count] = 0x2A;
            } else if (ch == '&') {
                ptr[count] = 0x2B;
            } else if (ch == '.') {
                ptr[count] = 0x2C;
            } else if (ch == '\"') {
                ptr[count] = 0x2D;
            } else if (ch == '?') {
                ptr[count] = 0x2E;
            } else if (ch == '_') {
                ptr[count] = 0x2F;
            } else {
                Q_ASSERT(false);
            }
        } else {
            // pad with spaces
            ptr[count] = 0x24;
        }
    }

    modified = true;
}

enum sf_note SRAMFile::getNote() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return static_cast<enum sf_note>(ptr[NOTE_OFFSET]);
}

void SRAMFile::setNote(enum sf_note note) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[NOTE_OFFSET] = note;
    modified         = true;
}

int SRAMFile::getPlayCount() const {
    Q_ASSERT(isValid(game));

    const unsigned char *ptr = (reinterpret_cast<const unsigned char *>(sram)
                                + MISC_DATA + PLAYCOUNT_OFFSET);

    return ptr[game];
}

void SRAMFile::setPlayCount(int count) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((count >= 0) && (count <= 255));

    unsigned char *ptr = (reinterpret_cast<unsigned char *>(sram) + MISC_DATA
                          + PLAYCOUNT_OFFSET);

    ptr[game] = count;
    modified  = true;
}

enum sf_potion SRAMFile::getPotion() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return static_cast<sf_potion>(ptr[POTION_OFFSET]);
}

void SRAMFile::setPotion(enum sf_potion potion) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[POTION_OFFSET] = potion;
    modified           = true;
}

enum sf_quest SRAMFile::getQuest() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + MISC_DATA + QUEST_OFFSET);

    return static_cast<enum sf_quest>(ptr[game]);
}

void SRAMFile::setQuest(enum sf_quest quest) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + MISC_DATA + QUEST_OFFSET);

    ptr[game] = quest;
    modified  = true;
}

enum sf_ring SRAMFile::getRing() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return static_cast<enum sf_ring>(ptr[RING_OFFSET]);
}

void SRAMFile::setRing(enum sf_ring ring) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[RING_OFFSET] = ring;
    modified         = true;
}

int SRAMFile::getRupees() const {
    Q_ASSERT(isValid(game));

    const unsigned char *ptr =
        (reinterpret_cast<const unsigned char *>(sram) + INVENTORY_DATA
         + (game * INVENTORY_DATA_SIZE));

    return ptr[RUPEES_OFFSET];
}

void SRAMFile::setRupees(int rupees) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((rupees >= 0) && (rupees <= 255));

    unsigned char *ptr = (reinterpret_cast<unsigned char *>(sram)
                          + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[RUPEES_OFFSET] = rupees;
    modified           = true;
}

enum sf_sword SRAMFile::getSword() const {
    Q_ASSERT(isValid(game));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return static_cast<enum sf_sword>(ptr[SWORD_OFFSET]);
}

void SRAMFile::setSword(enum sf_sword sword) {
    Q_ASSERT(isValid(game));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    ptr[SWORD_OFFSET] = sword;
    modified          = true;
}

bool SRAMFile::hasTriforce(int piece) const {
    Q_ASSERT(isValid(game));
    Q_ASSERT((piece >= 1) && (piece <= 8));

    const char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    return (ptr[TRIFORCE_OFFSET] & (1 << (piece - 1)));
}

void SRAMFile::setTriforce(int piece, bool give) {
    Q_ASSERT(isValid(game));
    Q_ASSERT((piece >= 1) && (piece <= 8));

    char *ptr = (sram + INVENTORY_DATA + (game * INVENTORY_DATA_SIZE));

    if (give) {
        ptr[TRIFORCE_OFFSET] |= (1 << (piece - 1));
    } else {
        ptr[TRIFORCE_OFFSET] &= ~(1 << (piece - 1));
    }

    modified = true;
}
