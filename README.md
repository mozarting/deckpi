# deckpi

A minimalist slide deck creator and presenter written in C.
Write slides in a simple text format, then present them directly in fullscreen.

> ⚠️ **Idea Phase / Recreational Project**
> This is not production-ready software. Right now deckpi is just an experiment, a toy project for exploring C, DSL design, and SDL2 rendering.
> It _will_ break, it _will_ change, and it might never leave “fun side-project” territory.
> Use at your own risk (or amusement).

## Example Deck

```txt
slide "Introduction"
    background: #202020
    text: "Welcome to my talk"
    text: "Built with C"

slide "Image Example"
    background: #ffffff
    text: "Here’s a picture"
    image: ./images/cat.png
```

## Features (planned)

- Text-based format for writing slides
- Live fullscreen presentation with **arrow key navigation**
- Backgrounds (colors, images)
- Text rendering with custom fonts
- Image embedding
- Lightweight — runs on SDL2

## Roadmap

- [ ] Basic parser (`deckpi.c` reads `.txt` → slide structs)
- [ ] Minimal presenter (render text, arrow keys to switch)
- [ ] Background colors + image support
- [ ] Export to PNG/PDF
- [ ] Code blocks with syntax highlighting
- [ ] Transitions and animations

## Build

Dependencies:

- SDL2
- SDL2_ttf
- SDL2_image

```bash
gcc -o deckpi deckpi.c -lSDL2 -lSDL2_ttf -lSDL2_image
```

## Usage

```bash
./deckpi slides.txt
```

Arrow keys ← → navigate slides.
