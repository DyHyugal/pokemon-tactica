---
layout: default
title: "Changes"
---

[Home](Home.md) · [Game Guide](Game-Guide.md) · [Changes](Changes.md) · [Bosses & Tips](Bosses-and-Tips.md) · [Pokédex](Pokedex.md) · [Locations](Locations.md) · [Credits & Versions](Credits-and-Versions.md) · **[FR](../FR/Changements.md)**

# Changes

This page groups the main differences between Pokémon Heart & Soul and Pokémon Tactica. The goal is to avoid a wiki full of tiny nested pages.

## Contents

- [Systems](#systems)
- [Starters and Elm's Egg](#starters-and-elms-egg)
- [Pokémon](#pokémon)
- [Evolutions and learnsets](#evolutions-and-learnsets)
- [Wild encounters](#wild-encounters)
- [Boss battles](#boss-battles)
- [Shops](#shops)
- [Interface and QoL](#interface-and-qol)

## Systems

Tactica adds or changes:

- HARD by default, NORMAL still available;\n- the same new Tactica boss rosters in both difficulties;
- Level Cap;
- Training NPC;
- native x1/x2/x3/x4 game speed;
- independent audio settings;
- redesigned game setup;
- simplified settings save/exit;
- clearer separation between difficulty, challenges and randomizer.

## Starters and Elm's Egg

Starter choice is no longer limited to the classic trio. Pokémon are grouped by type, with Eevee available as a separate path.

Elm's Egg becomes a second Tactica selection: the confirmed species is the actual Pokémon inside the Egg and keeps its identity through hatching.

Type boosters and relevant evolution items are granted automatically so starter choices remain practical without artificial detours.

## Pokémon

Several Pokémon receive targeted changes to typing, stats, abilities or role.

Examples already locked for V1 include:

| Pokémon | Main change |
|---|---|
| Charizard | Fire / Dragon |
| Typhlosion | Fire / Ground |
| Sceptile | Grass / Dragon, stronger physical identity |
| Luxray | Electric / Dark |
| Oshawott / Dewott / Samurott | Water / Fighting |
| Haxorus | revised secondary typing and offensive profile |
| Electivire | revised offensive profile and Speed |
| Kingdra | stronger physical profile |

The exhaustive table will be synchronized with compiled data before RC so the wiki does not preserve values that changed during integration.

## Evolutions and learnsets

Some item-based evolutions require a minimum level:

- base → stage 1: level 16;
- base → final with no middle stage: level 30;
- stage 1 → final: level 36.

Learnsets were expanded to support new roles. Two important move mappings used by the project are:

- **Wave Crash** = `MOVE_WAVE_CRASH`
- **Flip Turn** = `MOVE_FLIP_TURN`

## Wild encounters

Wild distribution is rebuilt around story progression.

Principles:

- 4 main species per table;
- target distribution 30 / 30 / 30 / 10;
- use grass, Surf, fishing, Rock Smash, time of day and other available methods;
- wild starters are rare and progressively placed;
- final evolutions generally appear late;
- no Legendary/Mythical Pokémon injected into ordinary random encounters;
- Safari-only species remain tied to Safari content.

Current integrated state before the final pass: **405 standard tables + 4 Headbutt tables**. Safari adds **53 separate pools**, each using four species at 30/30/30/10 with pool rotation during a Safari admission.

## Boss battles

Tactica boss rosters are shared by NORMAL and HARD. NORMAL uses NORMAL IV/EV scaling and AI; HARD adds the following optimization:

- held items;
- 31 IVs;
- role-based EVs;
- weather/terrain;
- hazards;
- screens;
- Trick Room;
- setup;
- pivots;
- win conditions.

From the third Gym onward, Gym Leaders use full teams of six. Mega Evolution starts from the fourth Gym.

Boss mechanics and practical advice are covered in [Bosses & Tips](Bosses-and-Tips.md).

## Shops

The TM Shop follows:

**Category → Type → TM**

Categories:

- Support
- Weak
- Powerful
- Ultimate

The item shop follows:

**Evolution → Strategic → Other**

Direct healing, Poké Balls, Repels, Key Items and story items must not bypass normal shop progression.

## Interface and QoL

Tactica keeps the HnS visual base and finishes a lighter V1 refresh across:

- windows;
- menus;
- battle UI;
- summary;
- Pokédex;
- shops;
- palettes.

A deeper visual overhaul is reserved for a future V2.
