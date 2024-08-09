# vgctl [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2Fstanekondrej%2Fvgctl&count_bg=%232600FF&title_bg=%23000000&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=true)](https://hits.seeyoufarm.com)


vgctl is a utility to enable/disable the Riot Vanguard anticheat kernel space driver.

## Why would you want to do this?

Many have voiced their concerns about an anticheat running in the background at all times, and rightfully
so. Why should it need to? Riot doesn't need to know what I'm doing when I'm not playing VALORANT (not
that they'd need to while I'm playing it).

## Usage

<sup>Note: this is subject to change to be more simple and accessible in a future release.</sup>  
Open a command prompt/Powershell prompt **as administrator.** Then, navigate to the directory with
the executable, and run it with one of the flags:

- `-e`: enable Riot Vanguard (self-explanatory)
- `-d`: disable Riot Vanguard (also kills the process)
- ~~`-k`: kill Riot Vanguard (only kills the process, Vanguard will start on next boot)~~ this has
not been implemented yet.

## Getting vgctl

Download the binary from GitHub releases, or build from source (see [Building](#building)).

## Building

You can build just like you would regularly using CMake. Then, either execute vgctl directly,
or add it to your PATH. Don't bother trying to build this on something other than Windows, because
vgctl needs the Windows APIs to build, and obviously to function.

## Goals

These are some of the future goals I want to achieve in this project:

- [ ] Implement the Vanguard kill functionality
- [x] Write CI/CD things (e.g. GH actions, tests...)
- [x] Make vgctl open its own console window
- [x] Make vgctl ask for elevated privileges
