# Common Mod

A **server-only** DayZ mod - a grab-bag of gameplay tweaks for a server admin.

## Features

| Feature          | What it does                                                                |
|------------------|-----------------------------------------------------------------------------|
| `/players`       | Private list of online players (excludes the requester).                    |
| `/time`          | Private in-game time as `HH:MM`.                                            |
| `/ping`          | Private current + average ping.                                             |
| `/rules`         | Private reply with the server rules.                                        |
| `/help`          | Private list of enabled commands. Aliases: `/commands`, `/h`, `/usage`.     |
| Rules on join    | Shows the server rules in chat on connect (repeatable, with delays).        |
| PvP-zone warning | Chat + toast alert on enter/exit of the NWAF PvP zone (chernarusplus only). |

Typing a command replies privately to the sender; the typed line itself stays visible (vanilla limitation).

## Install

1. Copy `@common-mod` into the server root.
2. Launch the server with `-serverMod=@common-mod`.
3. Use `-serverMod=`, not `-mod=`.

## Config

Auto-created on first run at `<profiles>/common-mod/config.json` (`<profiles>` = the server `-profiles` dir).
Holds all tunable text and toggles. New fields are added on upgrade without losing existing values.

## Build

Run `make` from the repo root - packs `dist/@common-mod/addons/common-mod.pbo`.
