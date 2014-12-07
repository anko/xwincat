# xwincat

X11 window position logger with a simple output format: One window position
change per line on `stdout`.

![Window cats. The name is referringto the UNIX cat program though.][1]
<br /><sup>Image credit: [Pixabay][2] user 123090</sup>


Nice for drawing some sort of composited overlay, or for fun statistics.

## Format

On startup, `xwincat` prints `screen <width> <height>` where the numbers
represent the dimensions of the first screen it encountered.

Then, whenever a window changes position or size, it sends `window <window_id>
<x> <y> <width> <height>`.

If you passed `-time`, an epoch timestamp is additionally printed at the end of
`window` lines.

## Compilation

Just `make`.

Don't have `clang`? Edit the `makefile` to use `gcc`.

## Usage

Flags you can pass (all optional):

 - `-display <display>`: set target X display (default `:0`)
 - `-time`: also append epoch timestamps (default: don't)
 - `-help`: print usage hints and exit

Interrupt signal (`C-c`) to end.

## Related programs

If you want to similarly log what keys are pressed, you might like
[`xkbcat`][3].

## License

[ISC][4].


[1]: https://cloud.githubusercontent.com/assets/5231746/5329631/5b4f0714-7db8-11e4-8476-921b166ad1c3.jpg
[2]: http://pixabay.com/en/cat-window-sit-pet-masonry-437973/
[3]: https://github.com/anko/xkbcat
[4]: http://opensource.org/licenses/ISC
