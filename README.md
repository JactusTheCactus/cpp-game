# Important Functions
## `move(x, y)`
- Moves cursor
## `clrtobot()`
- Clears text from the cursor to the bottom of the terminal
## `clrtoeol()`
- Clears text from the cursor to the end of the line
## `mvprintw(x,y,str,…args)`
- Prints formatted text at an XY coordinate
- `move(x,y)` + `printw(str,…args)`
# Notes
- Do not use `printf(str,…args)`
	- using `printf(str,…args)` inside ncurses is bad
	- use `printw(str,…args)` instead