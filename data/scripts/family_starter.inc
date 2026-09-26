@ Shared, reversible menu flow. Results are temporary until the caller commits.
@ 8004: category, 8005: species, 8006: optional evolution preference.
@ 8007: species cursor, 8008: evolution cursor.
FamilyStarter_Select::
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, SPECIES_NONE
	setvar VAR_0x8006, SPECIES_NONE
	setvar VAR_0x8007, 0xFFFF
	setvar VAR_0x8008, 0
FamilyStarter_Category::
	closemessage
	callnative FamilyStarter_BuildCategoryMenu
	dynmultistack 1, 1, FALSE, 6, FALSE, VAR_0x8004, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, FamilyStarter_Cancel
	copyvar VAR_0x8004, VAR_RESULT
	setvar VAR_0x8005, SPECIES_NONE
	setvar VAR_0x8006, SPECIES_NONE
	setvar VAR_0x8007, 0xFFFF
	setvar VAR_0x8008, 0
	goto_if_eq VAR_0x8004, 6, FamilyStarter_EeveeEvolution
FamilyStarter_Species::
	callnative FamilyStarter_BuildSpeciesMenu
	dynmultistack 1, 1, FALSE, 6, FALSE, VAR_0x8007, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, FamilyStarter_Category
	goto_if_eq VAR_RESULT, SPECIES_NONE, FamilyStarter_Category
	callnative FamilyStarter_SaveSpeciesCursor
	copyvar VAR_0x8005, VAR_RESULT
	setvar VAR_0x8006, SPECIES_NONE
	callnative FamilyStarter_HasEvolutionChoices
	goto_if_eq VAR_RESULT, TRUE, FamilyStarter_Evolution
FamilyStarter_ConfirmChoice::
	callnative FamilyStarter_PreparePreview
	showmonpic VAR_0x8005, 10, 3
	bufferspeciesname STR_VAR_1, VAR_0x8005
	goto_if_ne VAR_0x8006, SPECIES_NONE, FamilyStarter_ConfirmEvolution
	msgbox FamilyStarter_ConfirmSpecies, MSGBOX_YESNO
	hidemonpic
	goto_if_ne VAR_RESULT, YES, FamilyStarter_Species
FamilyStarter_Accept::
	setvar VAR_RESULT, TRUE
	return
FamilyStarter_ConfirmEvolution::
	bufferspeciesname STR_VAR_2, VAR_0x8006
	msgbox FamilyStarter_ConfirmEvolutionText, MSGBOX_YESNO
	hidemonpic
	goto_if_ne VAR_RESULT, YES, FamilyStarter_Evolution
	goto FamilyStarter_Accept
FamilyStarter_EeveeEvolution::
	setvar VAR_0x8005, SPECIES_EEVEE
	setvar VAR_0x8006, SPECIES_NONE
FamilyStarter_Evolution::
	msgbox FamilyStarter_EvolutionPrompt, MSGBOX_DEFAULT
	closemessage
	callnative FamilyStarter_BuildEvolutionMenu
	dynmultistack 1, 1, FALSE, 6, FALSE, VAR_0x8008, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, FamilyStarter_EvolutionBack
	callnative FamilyStarter_SaveEvolutionCursor
	copyvar VAR_0x8006, VAR_RESULT
	goto FamilyStarter_ConfirmChoice
FamilyStarter_EvolutionBack::
	goto_if_eq VAR_0x8005, SPECIES_EEVEE, FamilyStarter_Category
	goto FamilyStarter_Species
FamilyStarter_Cancel::
	setvar VAR_0x8005, SPECIES_NONE
	setvar VAR_0x8006, SPECIES_NONE
	setvar VAR_RESULT, FALSE
	return

FamilyStarter_Fire:
	.string "Feu$"
FamilyStarter_Water:
	.string "Eau$"
FamilyStarter_Grass:
	.string "Plante$"
FamilyStarter_Electric:
	.string "Électrik$"
FamilyStarter_Ground:
	.string "Sol$"
FamilyStarter_Ice:
	.string "Glace$"
FamilyStarter_Eevee:
	.string "Évoli$"
FamilyStarter_ConfirmSpecies:
	.string "Choisis-tu {STR_VAR_1} ?$"
FamilyStarter_ConfirmEvolutionText:
	.string "Choisis-tu {STR_VAR_1}\n"
	.string "avec {STR_VAR_2} prévu ?$"
FamilyStarter_EvolutionPrompt:
	.string "Quelle évolution prévois-tu ?$"

FamilyStarter_ShowPendingItem::
	callnative FamilyStarter_PreparePendingItemMessage
	goto_if_eq VAR_RESULT, FALSE, FamilyStarter_ShowPendingItemReturn
	goto_if_eq VAR_RESULT, 2, FamilyStarter_ShowPendingPCItem
	call EventScript_ObtainItemMessage
	goto FamilyStarter_ShowPendingItem
FamilyStarter_ShowPendingPCItem:
	bufferitemnameplural STR_VAR_2, VAR_0x8000, VAR_0x8001
	checkitemtype VAR_0x8000
	call EventScript_BufferPocketNameAndTryFanfare
	message gText_ObtainedTheItem
	delay 10
	showitemdescription
	waitfanfare
	msgbox FamilyStarter_ItemSentPC, MSGBOX_DEFAULT
	hideitemdescription
	goto FamilyStarter_ShowPendingItem
FamilyStarter_ShowPendingItemReturn:
	return
FamilyStarter_ItemSentPC:
	.string "Le SAC est plein.\p"
	.string "L'objet a été envoyé au PC.$"
