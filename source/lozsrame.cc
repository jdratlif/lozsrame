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

#include <QApplication>

#include "view/mainwindow.hh"

using namespace lozsrame;

auto main(int argc, char **argv) -> int {
    QApplication app(argc, argv);
    MainWindow   window;

    window.show();

    return app.exec();
}
