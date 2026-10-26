[Home](Home.md) · **[Play](Play.md)** · [Guide](Game-Guide.md) · [Changes](Changes.md) · [Pokédex](Pokedex.md) · [Locations](Locations.md) · [Routes & Cities](Routes-and-Cities.md) · [Bosses & Tips](Bosses-and-Tips.md) · [Credits](Credits-and-Versions.md) · [Roadmap](Roadmap.md) · **[FR](../FR/Jouer.md)**

# Play Pokémon Tactica

## Download the game

Pokémon Tactica is distributed as a complete Game Boy Advance ROM. Open the [Tactica .gba ROM](https://github.com/DyHyugal/pokemon-tactica/releases/download/v1-candidate-46f30976/pokemon-tactica.gba) directly in an emulator. You do not need a Pokémon Emerald ROM, an EmeraldPlus version or a patching tool.

The [Tactica download package](https://github.com/DyHyugal/pokemon-tactica/releases/download/v1-candidate-46f30976/pokemon-tactica.zip) includes the same ROM, version information and the playtest checklist. Extract the ZIP to find the .gba file. The files are also available on the [candidate release page](https://github.com/DyHyugal/pokemon-tactica/releases/tag/v1-candidate-46f30976).

## Start your adventure

On a computer, [mGBA](https://mgba.io/downloads.html) can run the ROM. Install the version for your operating system, then open the Tactica .gba file from the emulator menu. On other devices, use a Game Boy Advance emulator and load the same file. The [Game Guide](Game-Guide.md) will help you through Johto and Kanto.

The V1 ROM is in English; the wiki is available in French and English. Difficulty settings and training tools are included in the game, so no additional download is needed to use them.

## The current candidate

The POKéGEAR button now opens the actual device from Start: the Tactica Map with Fly, Phone and Radio according to their story unlocks. For Snorlax, tune to POKé FLUTE while standing in front of it (all the way right, then two notches left), leave with B and talk with A without changing maps. The clean Johto and Kanto maps are preserved; no new game is needed.

Red now uses Partner Pikachu, Mega Lucario, Serperior, Dracovish, Gliscor and Rotom-Heat, all at level 100 at Mt. Silver and in the post-OBC rematch. All six held items are distinct; Lucario enters with Justified and gains Adaptability through Mega Evolution. Gliscor sets Stealth Rock and Rotom-Heat provides Defog.

The download is candidate **46f30976**, built from **integration/v1**. It fixes the Lt. Surge crash (Manectric with Lightning Rod and Signal Beam) and incompatible boss/rematch sets. Battle tests verify abilities and types for 29 Mega combinations. It retains rival selection using the official weaknesses of the starter menu category and migrates invalid saved pairings to a legal team that is then retained. Level scaling now counts the opening rival victory (floor 18) and each central Kanto Gym victory (80), with no level-scaling option to enable. It also includes ordinary trainer levels based on area and story progress, Haxorus’s base Attack reduced to 130, and the existing battle, HM and interface fixes. The evolution wiki has been synchronized with the game. This version is intended for playtesting and has not yet been declared stable.

The first validation playthrough goes through Morty, followed by the full adventure across Johto, Kanto, the second League and Red. When reporting an issue, include version **46f30976**, your difficulty, level cap setting and steps to reproduce it. Fixes and balance adjustments will follow actual gameplay feedback.

[Full trainer and area level table (CSV for Excel)](../../docs/assets/tactica-trainer-scaling.csv).

## Keep your save

When replacing the ROM, keep a copy of your save and the previous version. Resume from an in-game save by choosing Continue. If your emulator matches files by name, give the new ROM and a copy of your .sav file the same base name, using the emulator’s save folder. The ROM keeps the name pokemon-tactica.gba, so its matching save can remain pokemon-tactica.sav across future updates. This update keeps the save format unchanged: it checks the recorded primary starter category and preserves an already legal rival. An old Ice/Water pairing becomes Fire; Electric/Grass becomes Ground. Save normally after loading to write this choice to the file; the legal archetype will remain the same for the rest of the adventure.
