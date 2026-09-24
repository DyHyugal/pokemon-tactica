ItemShop_EventScript::
	lock
	faceplayer
	msgbox ItemShop_Text_Welcome, MSGBOX_DEFAULT
ItemShop_MainMenu:
	closemessage
	dynmultichoice 1, 1, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, ItemShop_Text_Buy, ItemShop_Text_Quit
	goto_if_eq VAR_RESULT, 0, ItemShop_CategoryMenu
	goto ItemShop_Exit

ItemShop_CategoryMenu:
	msgbox ItemShop_Text_ChooseCategory, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, ItemShop_Text_Evolution, ItemShop_Text_Strategic, ItemShop_Text_Other, ItemShop_Text_Back
	switch VAR_RESULT
	case 0, ItemShop_EvolutionMenu
	case 1, ItemShop_StrategicMenu
	case 2, ItemShop_OtherMenu
	goto ItemShop_MainMenu

ItemShop_EvolutionMenu:
	msgbox ItemShop_Text_ChooseSubcategory, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, ItemShop_Text_Stones, ItemShop_Text_TradeHeld, ItemShop_Text_SpecialRegional, ItemShop_Text_Back
	switch VAR_RESULT
	case 0, ItemShop_OpenStones
	case 1, ItemShop_OpenTradeHeld
	case 2, ItemShop_OpenSpecialRegional
	goto ItemShop_CategoryMenu

ItemShop_OpenStones:
	pokemart ItemShop_Items_Stones
	goto ItemShop_EvolutionMenu

ItemShop_OpenTradeHeld:
	pokemart ItemShop_Items_TradeHeld
	goto ItemShop_EvolutionMenu

ItemShop_OpenSpecialRegional:
	pokemart ItemShop_Items_SpecialRegional
	goto ItemShop_EvolutionMenu

ItemShop_StrategicMenu:
	msgbox ItemShop_Text_ChooseSubcategory, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, ItemShop_Text_TypeBoosters, ItemShop_Text_ChoiceOffensive, ItemShop_Text_DefenseSustain, ItemShop_Text_WeatherTerrain, ItemShop_Text_ActivationUtility, ItemShop_Text_CompetitiveBerries, ItemShop_Text_Back
	switch VAR_RESULT
	case 0, ItemShop_OpenTypeBoosters
	case 1, ItemShop_OpenChoiceOffensive
	case 2, ItemShop_OpenDefenseSustain
	case 3, ItemShop_OpenWeatherTerrain
	case 4, ItemShop_OpenActivationUtility
	case 5, ItemShop_OpenCompetitiveBerries
	goto ItemShop_CategoryMenu

ItemShop_OpenTypeBoosters:
	pokemart ItemShop_Items_TypeBoosters
	goto ItemShop_StrategicMenu

ItemShop_OpenChoiceOffensive:
	pokemart ItemShop_Items_ChoiceOffensive
	goto ItemShop_StrategicMenu

ItemShop_OpenDefenseSustain:
	pokemart ItemShop_Items_DefenseSustain
	goto ItemShop_StrategicMenu

ItemShop_OpenWeatherTerrain:
	pokemart ItemShop_Items_WeatherTerrain
	goto ItemShop_StrategicMenu

ItemShop_OpenActivationUtility:
	pokemart ItemShop_Items_ActivationUtility
	goto ItemShop_StrategicMenu

ItemShop_OpenCompetitiveBerries:
	pokemart ItemShop_Items_CompetitiveBerries
	goto ItemShop_StrategicMenu

ItemShop_OtherMenu:
	msgbox ItemShop_Text_ChooseSubcategory, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, ItemShop_Text_Training, ItemShop_Text_BreedingFriendship, ItemShop_Text_Useful, ItemShop_Text_Back
	switch VAR_RESULT
	case 0, ItemShop_OpenTraining
	case 1, ItemShop_OpenBreedingFriendship
	case 2, ItemShop_OpenUseful
	goto ItemShop_CategoryMenu

ItemShop_OpenTraining:
	pokemart ItemShop_Items_Training
	goto ItemShop_OtherMenu

ItemShop_OpenBreedingFriendship:
	pokemart ItemShop_Items_BreedingFriendship
	goto ItemShop_OtherMenu

ItemShop_OpenUseful:
	pokemart ItemShop_Items_Useful
	goto ItemShop_OtherMenu

ItemShop_Exit:
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

ItemShop_Text_Welcome:
	.string "What would you like to do today?$"
ItemShop_Text_ChooseCategory:
	.string "Which kind of item are you looking for?$"
ItemShop_Text_ChooseSubcategory:
	.string "Which category are you looking for?$"
ItemShop_Text_Buy:
	.string "Buy$"
ItemShop_Text_Quit:
	.string "Quit$"
ItemShop_Text_Evolution:
	.string "Evolution$"
ItemShop_Text_Strategic:
	.string "Strategic$"
ItemShop_Text_Other:
	.string "Other$"
ItemShop_Text_Back:
	.string "Back$"

ItemShop_Text_Stones:
	.string "Stones$"
ItemShop_Text_TradeHeld:
	.string "Trade / Held$"
ItemShop_Text_SpecialRegional:
	.string "Special / Regional$"
ItemShop_Text_TypeBoosters:
	.string "Type Boosters$"
ItemShop_Text_ChoiceOffensive:
	.string "Choice / Offense$"
ItemShop_Text_DefenseSustain:
	.string "Defense / Sustain$"
ItemShop_Text_WeatherTerrain:
	.string "Weather / Terrain$"
ItemShop_Text_ActivationUtility:
	.string "Activation / Utility$"
ItemShop_Text_CompetitiveBerries:
	.string "Competitive Berries$"
ItemShop_Text_Training:
	.string "Training / Optimize$"
ItemShop_Text_BreedingFriendship:
	.string "Breeding / Friendship$"
ItemShop_Text_Useful:
	.string "Other Useful$"

	.align 2
ItemShop_Items_Stones:
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_WATER_STONE
	.2byte ITEM_THUNDER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_ICE_STONE
	.2byte ITEM_SUN_STONE
	.2byte ITEM_MOON_STONE
	.2byte ITEM_SHINY_STONE
	.2byte ITEM_DUSK_STONE
	.2byte ITEM_DAWN_STONE
	pokemartlistend

	.align 2
ItemShop_Items_TradeHeld:
	.2byte ITEM_DRAGON_SCALE
	.2byte ITEM_UPGRADE
	.2byte ITEM_PROTECTOR
	.2byte ITEM_ELECTIRIZER
	.2byte ITEM_MAGMARIZER
	.2byte ITEM_DUBIOUS_DISC
	.2byte ITEM_REAPER_CLOTH
	.2byte ITEM_PRISM_SCALE
	.2byte ITEM_WHIPPED_DREAM
	.2byte ITEM_SACHET
	.2byte ITEM_OVAL_STONE
	.2byte ITEM_DEEP_SEA_SCALE
	.2byte ITEM_DEEP_SEA_TOOTH
	.2byte ITEM_METAL_COAT
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_RAZOR_CLAW
	.2byte ITEM_RAZOR_FANG
	pokemartlistend

	.align 2
ItemShop_Items_SpecialRegional:
	.2byte ITEM_SWEET_APPLE
	.2byte ITEM_TART_APPLE
	.2byte ITEM_SYRUPY_APPLE
	.2byte ITEM_CRACKED_POT
	.2byte ITEM_CHIPPED_POT
	.2byte ITEM_GALARICA_CUFF
	.2byte ITEM_GALARICA_WREATH
	.2byte ITEM_STRAWBERRY_SWEET
	.2byte ITEM_LOVE_SWEET
	.2byte ITEM_BERRY_SWEET
	.2byte ITEM_CLOVER_SWEET
	.2byte ITEM_FLOWER_SWEET
	.2byte ITEM_STAR_SWEET
	.2byte ITEM_RIBBON_SWEET
	.2byte ITEM_BLACK_AUGURITE
	.2byte ITEM_LINKING_CORD
	.2byte ITEM_PEAT_BLOCK
	.2byte ITEM_AUSPICIOUS_ARMOR
	.2byte ITEM_MALICIOUS_ARMOR
	.2byte ITEM_LEADERS_CREST
	.2byte ITEM_SCROLL_OF_DARKNESS
	.2byte ITEM_SCROLL_OF_WATERS
	.2byte ITEM_UNREMARKABLE_TEACUP
	.2byte ITEM_MASTERPIECE_TEACUP
	.2byte ITEM_METAL_ALLOY
	pokemartlistend

	.align 2
ItemShop_Items_TypeBoosters:
	.2byte ITEM_SILK_SCARF
	.2byte ITEM_BLACK_BELT
	.2byte ITEM_SHARP_BEAK
	.2byte ITEM_POISON_BARB
	.2byte ITEM_SOFT_SAND
	.2byte ITEM_HARD_STONE
	.2byte ITEM_SILVER_POWDER
	.2byte ITEM_SPELL_TAG
	.2byte ITEM_IRON_PLATE
	.2byte ITEM_CHARCOAL
	.2byte ITEM_MYSTIC_WATER
	.2byte ITEM_MIRACLE_SEED
	.2byte ITEM_MAGNET
	.2byte ITEM_TWISTED_SPOON
	.2byte ITEM_NEVER_MELT_ICE
	.2byte ITEM_DRAGON_FANG
	.2byte ITEM_BLACK_GLASSES
	.2byte ITEM_FAIRY_FEATHER
	pokemartlistend

	.align 2
ItemShop_Items_ChoiceOffensive:
	.2byte ITEM_CHOICE_BAND
	.2byte ITEM_CHOICE_SPECS
	.2byte ITEM_CHOICE_SCARF
	.2byte ITEM_LIFE_ORB
	.2byte ITEM_EXPERT_BELT
	.2byte ITEM_MUSCLE_BAND
	.2byte ITEM_WISE_GLASSES
	.2byte ITEM_SCOPE_LENS
	.2byte ITEM_QUICK_CLAW
	.2byte ITEM_FOCUS_BAND
	.2byte ITEM_FOCUS_SASH
	.2byte ITEM_WIDE_LENS
	.2byte ITEM_ZOOM_LENS
	.2byte ITEM_LOADED_DICE
	.2byte ITEM_PUNCHING_GLOVE
	.2byte ITEM_METRONOME
	pokemartlistend

	.align 2
ItemShop_Items_DefenseSustain:
	.2byte ITEM_LEFTOVERS
	.2byte ITEM_BLACK_SLUDGE
	.2byte ITEM_SHELL_BELL
	.2byte ITEM_EVIOLITE
	.2byte ITEM_ASSAULT_VEST
	.2byte ITEM_ROCKY_HELMET
	.2byte ITEM_HEAVY_DUTY_BOOTS
	.2byte ITEM_AIR_BALLOON
	.2byte ITEM_BRIGHT_POWDER
	.2byte ITEM_LIGHT_CLAY
	.2byte ITEM_COVERT_CLOAK
	.2byte ITEM_CLEAR_AMULET
	.2byte ITEM_ABILITY_SHIELD
	pokemartlistend

	.align 2
ItemShop_Items_WeatherTerrain:
	.2byte ITEM_HEAT_ROCK
	.2byte ITEM_DAMP_ROCK
	.2byte ITEM_SMOOTH_ROCK
	.2byte ITEM_ICY_ROCK
	.2byte ITEM_TERRAIN_EXTENDER
	.2byte ITEM_UTILITY_UMBRELLA
	pokemartlistend

	.align 2
ItemShop_Items_ActivationUtility:
	.2byte ITEM_WHITE_HERB
	.2byte ITEM_MENTAL_HERB
	.2byte ITEM_POWER_HERB
	.2byte ITEM_WEAKNESS_POLICY
	.2byte ITEM_RED_CARD
	.2byte ITEM_EJECT_BUTTON
	.2byte ITEM_EJECT_PACK
	.2byte ITEM_ROOM_SERVICE
	.2byte ITEM_BLUNDER_POLICY
	.2byte ITEM_THROAT_SPRAY
	.2byte ITEM_FLAME_ORB
	.2byte ITEM_TOXIC_ORB
	.2byte ITEM_LAGGING_TAIL
	.2byte ITEM_IRON_BALL
	.2byte ITEM_RING_TARGET
	.2byte ITEM_SHED_SHELL
	.2byte ITEM_SAFETY_GOGGLES
	.2byte ITEM_PROTECTIVE_PADS
	.2byte ITEM_ADRENALINE_ORB
	pokemartlistend

	.align 2
ItemShop_Items_CompetitiveBerries:
	.2byte ITEM_LUM_BERRY
	.2byte ITEM_SITRUS_BERRY
	.2byte ITEM_CHILAN_BERRY
	.2byte ITEM_OCCA_BERRY
	.2byte ITEM_PASSHO_BERRY
	.2byte ITEM_WACAN_BERRY
	.2byte ITEM_RINDO_BERRY
	.2byte ITEM_YACHE_BERRY
	.2byte ITEM_CHOPLE_BERRY
	.2byte ITEM_KEBIA_BERRY
	.2byte ITEM_SHUCA_BERRY
	.2byte ITEM_COBA_BERRY
	.2byte ITEM_PAYAPA_BERRY
	.2byte ITEM_TANGA_BERRY
	.2byte ITEM_CHARTI_BERRY
	.2byte ITEM_KASIB_BERRY
	.2byte ITEM_HABAN_BERRY
	.2byte ITEM_COLBUR_BERRY
	.2byte ITEM_BABIRI_BERRY
	.2byte ITEM_ROSELI_BERRY
	.2byte ITEM_LIECHI_BERRY
	.2byte ITEM_GANLON_BERRY
	.2byte ITEM_SALAC_BERRY
	.2byte ITEM_PETAYA_BERRY
	.2byte ITEM_APICOT_BERRY
	.2byte ITEM_LANSAT_BERRY
	.2byte ITEM_STARF_BERRY
	.2byte ITEM_CUSTAP_BERRY
	pokemartlistend

	.align 2
ItemShop_Items_Training:
	.2byte ITEM_MACHO_BRACE
	.2byte ITEM_POWER_WEIGHT
	.2byte ITEM_POWER_BRACER
	.2byte ITEM_POWER_BELT
	.2byte ITEM_POWER_LENS
	.2byte ITEM_POWER_BAND
	.2byte ITEM_POWER_ANKLET
	.2byte ITEM_PP_UP
	.2byte ITEM_PP_MAX
	.2byte ITEM_BOTTLE_CAP
	.2byte ITEM_GOLD_BOTTLE_CAP
	.2byte ITEM_ABILITY_CAPSULE
	.2byte ITEM_ABILITY_PATCH
	pokemartlistend

	.align 2
ItemShop_Items_BreedingFriendship:
	.2byte ITEM_EVERSTONE
	.2byte ITEM_DESTINY_KNOT
	.2byte ITEM_SOOTHE_BELL
	pokemartlistend

	.align 2
ItemShop_Items_Useful:
	.2byte ITEM_LUCKY_EGG
	.2byte ITEM_AMULET_COIN
	.2byte ITEM_SMOKE_BALL
	.2byte ITEM_CLEANSE_TAG
	pokemartlistend
