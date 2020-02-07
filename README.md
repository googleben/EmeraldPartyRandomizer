# Pokemon Emerald Party Randomizer

This is a romhack for Pokemon Emerald that randomizes your party's pokemon at the beginning of every battle; each pokemon in your party becomes a new pokmeon chosen randomly from every pokemon in the game. It preserves EVs, IVs, contest conditions, HP percentage, XP percentage, shiny/pokerus, and nickname across randomizations, and the moves of the new pokemon are the same as if they were found in the wild at that level. Pokemon will also be added to the pokedex if you get them through randomization.

# Download

Downloads can be found on the [releases page](https://github.com/googleben/EmeraldPartyRandomizer/releases) of this repository.

# Installation & Use

The hack is distributed as a .bps file used to patch your Pokemon Emerald ROM. You must acquire a Pokemon Emerald ROM to use this romhack; the patch file only contains my changes. .bps patches can be used with a patching tool called [Flips](https://www.romhacking.net/utilities/1040/) or directly inside some emulators. The ROM to patch is release #1986, which should be Pokemon Emerald U/E. The correct rom should have the SHA1 checksum `f3ae088181bf583e55daf962a92bb46f4f1d07b7` if you want to check that. After patching your rom (either by creating a new file or in-emulator), the randomizer will just work!

# About The Code

This randomizer works by modifying the code that's called at the beginning of every battle. In the [pokeemerald project](https://github.com/pret/pokeemerald) the function that's modified is SetUpBattleVarsAndBirchZigzagoon in battle_controllers.c. All it does is call my custon function rndParty if the check for being the first battle fails. Note that all the code is marked as static methods -- this is on purpose to make compilation easier by avoiding introducing another file to link, since the setup can be a little precarious.