# deckpi

A minimalist slide deck creator and presenter written in C.
Write slides in a simple text format, then present them directly in fullscreen.

> ⚠️ **Recreational Project**
> This is not production-ready software. This is just an experiment, a toy project for exploring C, DSL design, and SDL2 rendering.

## Personal Note:

I started building this purely for recreational purposes, without researching the existing tools that I was sure existed. I didn’t think much about the syntax of the DSL and invented it on the fly. Now, looking back, I think the syntax is pathetic. It should have just been Markdown.

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
- [ ] Background colors + image support
      ~~- [ ] Export to PNG/PDF~~
      ~~- [ ] Code blocks with syntax highlighting~~  
       ~~- [ ] Transitions and animations~~

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
