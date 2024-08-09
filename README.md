# vgctl

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

## Building

Install Visual Studio, specifically the package for desktop C/C++ development. Then, compile the project
with MSVC++. Pretty simple and straight forward.

## Goals

These are some of the future goals I want to achieve in this project:

- [ ] Write CI/CD things (e.g. GH actions, tests...)
- [ ] Implement the Vanguard kill functionality
- [x] Make vgctl open its own console window
- [x] Make vgctl ask for elevated privileges
