# deckpi

A minimalist slide deck creator and presenter written in C.
Write slides in a simple text format, then present them directly in fullscreen.

> ⚠️ **Idea Phase / Recreational Project**
> This is not production-ready software. Right now deckpi is just an experiment, a toy project for exploring C, DSL design, and SDL2 rendering.
> It _will_ break, it _will_ change, and it might never leave “fun side-project” territory.
> Use at your own risk (or amusement).

## Example Deck

```txt
slide 1 :
title : "Deckpi"
subtitle: "A minimalist slide deck creator and presenter written in C."

slide 2 :
title: "Features"
- "Text-based format for writing slides"
- "Live fullscreen presentation with arrow key navigation"
- "Backgrounds (colors, images)"
- "Text rendering with custom fonts"
- "Image embedding"
```

## Features (planned)

- Text-based format for writing slides
- Live fullscreen presentation with **arrow key navigation**
- Backgrounds (colors, images)
- Text rendering with custom fonts
- Image embedding
- Lightweight — runs on SDL2

## Roadmap

- [x] Basic parser (`deckpi.c` reads `.txt` → slide structs)
- [x] Minimal presenter (render text, arrow keys to switch)
- [ ] Background colors + image support
- [ ] Export to PNG/PDF
- [ ] Code blocks with syntax highlighting
- [ ] Transitions and animations

## Build

Dependencies:

- SDL2
- SDL2_ttf

```bash
cc -o nob nob.c
./nob
```

## Usage

```bash
./deckpi slides.txt
```

Arrow keys ← → navigate slides.
