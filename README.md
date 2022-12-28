# SFMLlearning

This is my last project for the Programming Technology course at RUDN University.

It's strongly inspired by [this tutorial](https://www.youtube.com/watch?v=lS_qeBy3aQI) and relies on the 
Verlet integration method for numerical calculations of motion:

```math
\vec{x_{n+1}} = \vec{x_{n}} + \Delta \vec{x_{n}} + \vec{a_{n}} *  \Delta t^{2}
```

## Usage

Press Left Mouse Button to add a ball. Shake the window to shake its contents.

## Environment

Written in Visual Studio 2022, compiled using MSVC on Windows 11. 
Uses [SFML](https://github.com/SFML/SFML) via static linking (all files included).
