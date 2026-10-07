/*
 * Maze_12MHZ
 * Copyright (C) 2026 otzpt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

void setup() 
{
 // runs once when the code starts | corre uma unica vez quando o codigo começa
 // initiate hardware here         | inicia o hardware aqui
 Serial.begin(115200)
}

void loop() 
{
  // runs repeatedly forever        | corre infinitamente
  // main robot behaviour goes here | codigo principal do robo aqui
  Serial.println("its alive!!!")
  delay(1000)
}
