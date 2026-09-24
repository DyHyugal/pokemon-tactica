TmShop_EventScript::
	lock
	faceplayer
	msgbox TmShop_Text_Welcome, MSGBOX_DEFAULT
TmShop_MainMenu:
	closemessage
	dynmultichoice 1, 1, FALSE, 2, 0, DYN_MULTICHOICE_CB_NONE, TmShop_Text_Buy, TmShop_Text_Quit
	goto_if_eq VAR_RESULT, 0, TmShop_CategoryMenu
	goto TmShop_Exit

TmShop_CategoryMenu:
	msgbox TmShop_Text_ChooseCategory, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, TmShop_Text_Support, TmShop_Text_Weak, TmShop_Text_Powerful, TmShop_Text_Ultimate, TmShop_Text_Back
	switch VAR_RESULT
	case 0, TmShop_SupportTypeMenu
	case 1, TmShop_WeakTypeMenu
	case 2, TmShop_PowerfulTypeMenu
	case 3, TmShop_UltimateTypeMenu
	goto TmShop_MainMenu

TmShop_SupportTypeMenu:
	msgbox TmShop_Text_ChooseType, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, TmShop_Text_TypeNormal, TmShop_Text_TypeFighting, TmShop_Text_TypeFlying, TmShop_Text_TypePoison, TmShop_Text_TypeRock, TmShop_Text_TypeFire, TmShop_Text_TypeWater, TmShop_Text_TypeElectric, TmShop_Text_TypePsychic, TmShop_Text_TypeIce, TmShop_Text_TypeDark, TmShop_Text_Back
	switch VAR_RESULT
	case 0, TmShop_OpenSupportNormal
	case 1, TmShop_OpenSupportFighting
	case 2, TmShop_OpenSupportFlying
	case 3, TmShop_OpenSupportPoison
	case 4, TmShop_OpenSupportRock
	case 5, TmShop_OpenSupportFire
	case 6, TmShop_OpenSupportWater
	case 7, TmShop_OpenSupportElectric
	case 8, TmShop_OpenSupportPsychic
	case 9, TmShop_OpenSupportIce
	case 10, TmShop_OpenSupportDark
	goto TmShop_CategoryMenu

TmShop_OpenSupportNormal:
	pokemart TmShop_Items_Support_Normal
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportFighting:
	pokemart TmShop_Items_Support_Fighting
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportFlying:
	pokemart TmShop_Items_Support_Flying
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportPoison:
	pokemart TmShop_Items_Support_Poison
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportRock:
	pokemart TmShop_Items_Support_Rock
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportFire:
	pokemart TmShop_Items_Support_Fire
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportWater:
	pokemart TmShop_Items_Support_Water
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportElectric:
	pokemart TmShop_Items_Support_Electric
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportPsychic:
	pokemart TmShop_Items_Support_Psychic
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportIce:
	pokemart TmShop_Items_Support_Ice
	goto TmShop_SupportTypeMenu

TmShop_OpenSupportDark:
	pokemart TmShop_Items_Support_Dark
	goto TmShop_SupportTypeMenu

TmShop_WeakTypeMenu:
	msgbox TmShop_Text_ChooseType, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, TmShop_Text_TypeNormal, TmShop_Text_TypeFighting, TmShop_Text_TypeFlying, TmShop_Text_TypeRock, TmShop_Text_TypeBug, TmShop_Text_TypeGhost, TmShop_Text_TypeSteel, TmShop_Text_TypeWater, TmShop_Text_TypeGrass, TmShop_Text_TypeElectric, TmShop_Text_TypeIce, TmShop_Text_TypeDark, TmShop_Text_Back
	switch VAR_RESULT
	case 0, TmShop_OpenWeakNormal
	case 1, TmShop_OpenWeakFighting
	case 2, TmShop_OpenWeakFlying
	case 3, TmShop_OpenWeakRock
	case 4, TmShop_OpenWeakBug
	case 5, TmShop_OpenWeakGhost
	case 6, TmShop_OpenWeakSteel
	case 7, TmShop_OpenWeakWater
	case 8, TmShop_OpenWeakGrass
	case 9, TmShop_OpenWeakElectric
	case 10, TmShop_OpenWeakIce
	case 11, TmShop_OpenWeakDark
	goto TmShop_CategoryMenu

TmShop_OpenWeakNormal:
	pokemart TmShop_Items_Weak_Normal
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakFighting:
	pokemart TmShop_Items_Weak_Fighting
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakFlying:
	pokemart TmShop_Items_Weak_Flying
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakRock:
	pokemart TmShop_Items_Weak_Rock
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakBug:
	pokemart TmShop_Items_Weak_Bug
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakGhost:
	pokemart TmShop_Items_Weak_Ghost
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakSteel:
	pokemart TmShop_Items_Weak_Steel
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakWater:
	pokemart TmShop_Items_Weak_Water
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakGrass:
	pokemart TmShop_Items_Weak_Grass
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakElectric:
	pokemart TmShop_Items_Weak_Electric
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakIce:
	pokemart TmShop_Items_Weak_Ice
	goto TmShop_WeakTypeMenu

TmShop_OpenWeakDark:
	pokemart TmShop_Items_Weak_Dark
	goto TmShop_WeakTypeMenu

TmShop_PowerfulTypeMenu:
	msgbox TmShop_Text_ChooseType, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, TmShop_Text_TypeNormal, TmShop_Text_TypePoison, TmShop_Text_TypeGround, TmShop_Text_TypeRock, TmShop_Text_TypeBug, TmShop_Text_TypeGhost, TmShop_Text_TypeSteel, TmShop_Text_TypeFire, TmShop_Text_TypeGrass, TmShop_Text_TypeElectric, TmShop_Text_TypePsychic, TmShop_Text_TypeIce, TmShop_Text_TypeDragon, TmShop_Text_TypeDark, TmShop_Text_Back
	switch VAR_RESULT
	case 0, TmShop_OpenPowerfulNormal
	case 1, TmShop_OpenPowerfulPoison
	case 2, TmShop_OpenPowerfulGround
	case 3, TmShop_OpenPowerfulRock
	case 4, TmShop_OpenPowerfulBug
	case 5, TmShop_OpenPowerfulGhost
	case 6, TmShop_OpenPowerfulSteel
	case 7, TmShop_OpenPowerfulFire
	case 8, TmShop_OpenPowerfulGrass
	case 9, TmShop_OpenPowerfulElectric
	case 10, TmShop_OpenPowerfulPsychic
	case 11, TmShop_OpenPowerfulIce
	case 12, TmShop_OpenPowerfulDragon
	case 13, TmShop_OpenPowerfulDark
	goto TmShop_CategoryMenu

TmShop_OpenPowerfulNormal:
	pokemart TmShop_Items_Powerful_Normal
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulPoison:
	pokemart TmShop_Items_Powerful_Poison
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulGround:
	pokemart TmShop_Items_Powerful_Ground
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulRock:
	pokemart TmShop_Items_Powerful_Rock
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulBug:
	pokemart TmShop_Items_Powerful_Bug
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulGhost:
	pokemart TmShop_Items_Powerful_Ghost
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulSteel:
	pokemart TmShop_Items_Powerful_Steel
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulFire:
	pokemart TmShop_Items_Powerful_Fire
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulGrass:
	pokemart TmShop_Items_Powerful_Grass
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulElectric:
	pokemart TmShop_Items_Powerful_Electric
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulPsychic:
	pokemart TmShop_Items_Powerful_Psychic
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulIce:
	pokemart TmShop_Items_Powerful_Ice
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulDragon:
	pokemart TmShop_Items_Powerful_Dragon
	goto TmShop_PowerfulTypeMenu

TmShop_OpenPowerfulDark:
	pokemart TmShop_Items_Powerful_Dark
	goto TmShop_PowerfulTypeMenu

TmShop_UltimateTypeMenu:
	msgbox TmShop_Text_ChooseType, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, TmShop_Text_TypeNormal, TmShop_Text_TypeFighting, TmShop_Text_TypeSteel, TmShop_Text_TypeFire, TmShop_Text_TypeGrass, TmShop_Text_TypeElectric, TmShop_Text_TypeIce, TmShop_Text_Back
	switch VAR_RESULT
	case 0, TmShop_OpenUltimateNormal
	case 1, TmShop_OpenUltimateFighting
	case 2, TmShop_OpenUltimateSteel
	case 3, TmShop_OpenUltimateFire
	case 4, TmShop_OpenUltimateGrass
	case 5, TmShop_OpenUltimateElectric
	case 6, TmShop_OpenUltimateIce
	goto TmShop_CategoryMenu

TmShop_OpenUltimateNormal:
	pokemart TmShop_Items_Ultimate_Normal
	goto TmShop_UltimateTypeMenu

TmShop_OpenUltimateFighting:
	pokemart TmShop_Items_Ultimate_Fighting
	goto TmShop_UltimateTypeMenu

TmShop_OpenUltimateSteel:
	pokemart TmShop_Items_Ultimate_Steel
	goto TmShop_UltimateTypeMenu

TmShop_OpenUltimateFire:
	pokemart TmShop_Items_Ultimate_Fire
	goto TmShop_UltimateTypeMenu

TmShop_OpenUltimateGrass:
	pokemart TmShop_Items_Ultimate_Grass
	goto TmShop_UltimateTypeMenu

TmShop_OpenUltimateElectric:
	pokemart TmShop_Items_Ultimate_Electric
	goto TmShop_UltimateTypeMenu

TmShop_OpenUltimateIce:
	pokemart TmShop_Items_Ultimate_Ice
	goto TmShop_UltimateTypeMenu

TmShop_Exit:
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

TmShop_Text_Welcome:
	.string "What would you like to do today?$"
TmShop_Text_ChooseCategory:
	.string "Which kind of TM are you looking for?$"
TmShop_Text_ChooseType:
	.string "Which move type are you looking for?$"
TmShop_Text_Buy:
	.string "Buy$"
TmShop_Text_Quit:
	.string "Quit$"
TmShop_Text_Support:
	.string "Support$"
TmShop_Text_Weak:
	.string "Weak$"
TmShop_Text_Powerful:
	.string "Powerful$"
TmShop_Text_Ultimate:
	.string "Ultimate$"
TmShop_Text_Back:
	.string "Back$"

TmShop_Text_TypeNormal:
	.string "Normal$"
TmShop_Text_TypeFighting:
	.string "Fighting$"
TmShop_Text_TypeFlying:
	.string "Flying$"
TmShop_Text_TypePoison:
	.string "Poison$"
TmShop_Text_TypeGround:
	.string "Ground$"
TmShop_Text_TypeRock:
	.string "Rock$"
TmShop_Text_TypeBug:
	.string "Bug$"
TmShop_Text_TypeGhost:
	.string "Ghost$"
TmShop_Text_TypeSteel:
	.string "Steel$"
TmShop_Text_TypeFire:
	.string "Fire$"
TmShop_Text_TypeWater:
	.string "Water$"
TmShop_Text_TypeGrass:
	.string "Grass$"
TmShop_Text_TypeElectric:
	.string "Electric$"
TmShop_Text_TypePsychic:
	.string "Psychic$"
TmShop_Text_TypeIce:
	.string "Ice$"
TmShop_Text_TypeDragon:
	.string "Dragon$"
TmShop_Text_TypeDark:
	.string "Dark$"

	.align 2
TmShop_Items_Support_Normal:
	.2byte ITEM_TM_ROAR
	.2byte ITEM_TM_PROTECT
	.2byte ITEM_TM_SAFEGUARD
	.2byte ITEM_TM_DOUBLE_TEAM
	.2byte ITEM_TM_TORMENT
	.2byte ITEM_TM_ATTRACT
	.2byte ITEM_TM_ENDURE
	.2byte ITEM_TM_RECYCLE
	.2byte ITEM_TM_SWORDS_DANCE
	.2byte ITEM_TM_PSYCH_UP
	.2byte ITEM_TM_CAPTIVATE
	.2byte ITEM_TM_SLEEP_TALK
	.2byte ITEM_TM_SWAGGER
	.2byte ITEM_TM_SUBSTITUTE
	pokemartlistend

	.align 2
TmShop_Items_Support_Fighting:
	.2byte ITEM_TM_BULK_UP
	pokemartlistend

	.align 2
TmShop_Items_Support_Flying:
	.2byte ITEM_TM_ROOST
	pokemartlistend

	.align 2
TmShop_Items_Support_Poison:
	.2byte ITEM_TM_TOXIC
	pokemartlistend

	.align 2
TmShop_Items_Support_Rock:
	.2byte ITEM_TM_SANDSTORM
	.2byte ITEM_TM_ROCK_POLISH
	.2byte ITEM_TM_STEALTH_ROCK
	pokemartlistend

	.align 2
TmShop_Items_Support_Fire:
	.2byte ITEM_TM_SUNNY_DAY
	.2byte ITEM_TM_WILL_O_WISP
	pokemartlistend

	.align 2
TmShop_Items_Support_Water:
	.2byte ITEM_TM_RAIN_DANCE
	pokemartlistend

	.align 2
TmShop_Items_Support_Electric:
	.2byte ITEM_TM_THUNDER_WAVE
	pokemartlistend

	.align 2
TmShop_Items_Support_Psychic:
	.2byte ITEM_TM_CALM_MIND
	.2byte ITEM_TM_LIGHT_SCREEN
	.2byte ITEM_TM_REFLECT
	.2byte ITEM_TM_REST
	.2byte ITEM_TM_SKILL_SWAP
	.2byte ITEM_TM_TRICK_ROOM
	pokemartlistend

	.align 2
TmShop_Items_Support_Ice:
	.2byte ITEM_TM_HAIL
	pokemartlistend

	.align 2
TmShop_Items_Support_Dark:
	.2byte ITEM_TM_TAUNT
	.2byte ITEM_TM_SNATCH
	.2byte ITEM_TM_EMBARGO
	pokemartlistend

	.align 2
TmShop_Items_Weak_Normal:
	.2byte ITEM_TM_HIDDEN_POWER
	.2byte ITEM_TM_FACADE
	.2byte ITEM_TM_SECRET_POWER
	.2byte ITEM_TM_FALSE_SWIPE
	.2byte ITEM_TM_NATURAL_GIFT
	pokemartlistend

	.align 2
TmShop_Items_Weak_Fighting:
	.2byte ITEM_TM_BRICK_BREAK
	.2byte ITEM_TM_DRAIN_PUNCH
	pokemartlistend

	.align 2
TmShop_Items_Weak_Flying:
	.2byte ITEM_TM_AERIAL_ACE
	.2byte ITEM_TM_PLUCK
	pokemartlistend

	.align 2
TmShop_Items_Weak_Rock:
	.2byte ITEM_TM_ROCK_TOMB
	.2byte ITEM_TM_ROCK_SLIDE
	pokemartlistend

	.align 2
TmShop_Items_Weak_Bug:
	.2byte ITEM_TM_SILVER_WIND
	.2byte ITEM_TM_U_TURN
	pokemartlistend

	.align 2
TmShop_Items_Weak_Ghost:
	.2byte ITEM_TM_SHADOW_CLAW
	pokemartlistend

	.align 2
TmShop_Items_Weak_Steel:
	.2byte ITEM_TM_STEEL_WING
	pokemartlistend

	.align 2
TmShop_Items_Weak_Water:
	.2byte ITEM_TM_WATER_PULSE
	.2byte ITEM_TM_BRINE
	pokemartlistend

	.align 2
TmShop_Items_Weak_Grass:
	.2byte ITEM_TM_BULLET_SEED
	.2byte ITEM_TM_GIGA_DRAIN
	pokemartlistend

	.align 2
TmShop_Items_Weak_Electric:
	.2byte ITEM_TM_SHOCK_WAVE
	.2byte ITEM_TM_CHARGE_BEAM
	pokemartlistend

	.align 2
TmShop_Items_Weak_Ice:
	.2byte ITEM_TM_AVALANCHE
	pokemartlistend

	.align 2
TmShop_Items_Weak_Dark:
	.2byte ITEM_TM_THIEF
	.2byte ITEM_TM_FLING
	.2byte ITEM_TM_PAYBACK
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Normal:
	.2byte ITEM_TM_ROCK_CLIMB
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Poison:
	.2byte ITEM_TM_SLUDGE_BOMB
	.2byte ITEM_TM_POISON_JAB
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Ground:
	.2byte ITEM_TM_EARTHQUAKE
	.2byte ITEM_TM_DIG
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Rock:
	.2byte ITEM_TM_STONE_EDGE
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Bug:
	.2byte ITEM_TM_X_SCISSOR
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Ghost:
	.2byte ITEM_TM_SHADOW_BALL
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Steel:
	.2byte ITEM_TM_IRON_TAIL
	.2byte ITEM_TM_FLASH_CANNON
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Fire:
	.2byte ITEM_TM_FLAMETHROWER
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Grass:
	.2byte ITEM_TM_ENERGY_BALL
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Electric:
	.2byte ITEM_TM_THUNDERBOLT
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Psychic:
	.2byte ITEM_TM_PSYCHIC
	.2byte ITEM_TM_DREAM_EATER
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Ice:
	.2byte ITEM_TM_ICE_BEAM
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Dragon:
	.2byte ITEM_TM_DRAGON_CLAW
	.2byte ITEM_TM_DRAGON_PULSE
	pokemartlistend

	.align 2
TmShop_Items_Powerful_Dark:
	.2byte ITEM_TM_DARK_PULSE
	pokemartlistend

	.align 2
TmShop_Items_Ultimate_Normal:
	.2byte ITEM_TM_HYPER_BEAM
	.2byte ITEM_TM_FRUSTRATION
	.2byte ITEM_TM_RETURN
	.2byte ITEM_TM_EXPLOSION
	.2byte ITEM_TM_GIGA_IMPACT
	pokemartlistend

	.align 2
TmShop_Items_Ultimate_Fighting:
	.2byte ITEM_TM_FOCUS_PUNCH
	.2byte ITEM_TM_FOCUS_BLAST
	pokemartlistend

	.align 2
TmShop_Items_Ultimate_Steel:
	.2byte ITEM_TM_GYRO_BALL
	pokemartlistend

	.align 2
TmShop_Items_Ultimate_Fire:
	.2byte ITEM_TM_FIRE_BLAST
	.2byte ITEM_TM_OVERHEAT
	pokemartlistend

	.align 2
TmShop_Items_Ultimate_Grass:
	.2byte ITEM_TM_SOLAR_BEAM
	.2byte ITEM_TM_GRASS_KNOT
	pokemartlistend

	.align 2
TmShop_Items_Ultimate_Electric:
	.2byte ITEM_TM_THUNDER
	pokemartlistend

	.align 2
TmShop_Items_Ultimate_Ice:
	.2byte ITEM_TM_BLIZZARD
	pokemartlistend
