# NieR Automata Switch Accessories (NASA)
Modifies NieR:Automata (Steam) to add support for the NX DLC

# REQUIREMENTS
This repo adds functionality support to NX items, but does not add the items themselves. You can get the outfits from here: https://www.nexusmods.com/nierautomata/mods/358?

# Installation
- Install as a nier-mod-loader plugin: https://github.com/xxk-i/nier-mod-loader/releases
    - (e.g.) data\mods\plugins\NASA.dll

# Known/Probable issues
- Certain 9S sequences probably don't look as they should with switch outfits equipped
- Outfits don't show as ```ACTIVE``` when equipped and cannot be de-equipped
    - Fix by equipping a normal DLC outfit and unequipping
 Further issues are listed on the Nexus mods page

# Next steps
~~- Fix ```ACTIVE``` flagging~~
- Add support for accessories
- Add support for pods

# Developer note
I have no current plans on working on this mod anymore. The mod's main functionality is a single game function hook (albeit a large one) that manages mesh visibility states, and all related bugs exist within that. If you would like to contribute to this mod, feel free to contact me on Discord and I can help you get started -- @grojdg

# Thank You
Thank you to all who have ever joined NieR:Modding discord server and especially the following who have stuck with me throughout this endeavor and made everything possible.

Members of the original switch porting groupchat:
- Baal Silverwing
- DevolasRevenge - my main duo (model porting and scripting)
- inculta
- Izalith
- oli
- Seek
- Woeful_Wolf - localization tools
- ♻️ Martino 🧹 - code cleanup

Glory to Mankind
