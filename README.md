# deckpi

A minimalist slide deck creator and presenter written in C.
Write slides in a simple text format, then present them directly in fullscreen.

> ⚠️ **Recreational Project**
> This is not production-ready software. This is just an experiment, a toy project for exploring C, DSL design, and SDL2 rendering.

## Personal Note:

I built Deckpi purely for fun, without researching existing slide tools. The DSL syntax was improvised as I went along, and looking back, it’s a bit clunky. Using Markdown from the start would likely have been simpler and more flexible.

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

slide 3 :
title: "Image support"
subtitle: "Deckpi supports image embedding"
image: "image.png"
```

## Features

- Text-based format for writing slides
- Live fullscreen presentation with **arrow key navigation**
- Image embedding
- Lightweight — runs on SDL2

## Roadmap

- [x] Basic parser (`deckpi.c` reads `.txt` → slide structs)
- [x] Minimal presenter (render text, arrow keys to switch)
- ~~[ ] Background colors + image support~~
- ~~[ ] Code blocks with syntax highlighting~~
- ~~[ ] Export to PNG/PDF~~
- ~~[ ] Transitions and animations~~

## Build

Dependencies:

- SDL2
- SDL2_ttf
- SDL2_image

```bash
cc -o nob nob.c
./nob
```

## Usage

```bash
./deckpi slides.txt
```

Arrow keys ← → navigate slides.
