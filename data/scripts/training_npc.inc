TrainingNpc_EventScript::
	lock
	faceplayer
	msgbox TrainingNpc_Text_Welcome, MSGBOX_DEFAULT
TrainingNpc_MainMenu:
	closemessage
	dynmultichoice 1, 1, FALSE, 7, 0, DYN_MULTICHOICE_CB_NONE, TrainingNpc_Text_IV, TrainingNpc_Text_EV, TrainingNpc_Text_EXP, TrainingNpc_Text_Friendship, TrainingNpc_Text_Nature, TrainingNpc_Text_Ability, TrainingNpc_Text_Quit
	switch VAR_RESULT
	case 0, TrainingNpc_IV
	case 1, TrainingNpc_EV
	case 2, TrainingNpc_EXP
	case 3, TrainingNpc_Friendship
	case 4, TrainingNpc_Nature
	case 5, TrainingNpc_Ability
	goto TrainingNpc_Exit

TrainingNpc_ChooseMon:
	special ChoosePartyMon
	waitstate
	goto_if_ge VAR_0x8004, PARTY_SIZE, TrainingNpc_ChooseMonCancel
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, TrainingNpc_ChooseMonEgg
	setvar VAR_RESULT, TRUE
	return
TrainingNpc_ChooseMonEgg:
	msgbox TrainingNpc_Text_NoEggs, MSGBOX_DEFAULT
TrainingNpc_ChooseMonCancel:
	setvar VAR_RESULT, FALSE
	return

TrainingNpc_IV:
	call TrainingNpc_ChooseMon
	goto_if_eq VAR_RESULT, FALSE, TrainingNpc_MainMenu
	msgbox TrainingNpc_Text_ChooseStat, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 7, 0, DYN_MULTICHOICE_CB_NONE, TrainingNpc_Text_HP, TrainingNpc_Text_Attack, TrainingNpc_Text_Defense, TrainingNpc_Text_SpAttack, TrainingNpc_Text_SpDefense, TrainingNpc_Text_Speed, TrainingNpc_Text_All, TrainingNpc_Text_Back
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, TrainingNpc_MainMenu
	goto_if_eq VAR_RESULT, 7, TrainingNpc_MainMenu
	copyvar VAR_0x8005, VAR_RESULT
	callnative TrainingNpc_ApplyIV
	goto_if_eq VAR_RESULT, 2, TrainingNpc_AlreadyOptimized
	msgbox TrainingNpc_Text_IVDone, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu

TrainingNpc_EV:
	call TrainingNpc_ChooseMon
	goto_if_eq VAR_RESULT, FALSE, TrainingNpc_MainMenu
	msgbox TrainingNpc_Text_ChooseStat, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, TrainingNpc_Text_HP, TrainingNpc_Text_Attack, TrainingNpc_Text_Defense, TrainingNpc_Text_SpAttack, TrainingNpc_Text_SpDefense, TrainingNpc_Text_Speed, TrainingNpc_Text_Back
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, TrainingNpc_MainMenu
	goto_if_eq VAR_RESULT, 6, TrainingNpc_MainMenu
	copyvar VAR_0x8005, VAR_RESULT
	msgbox TrainingNpc_Text_ChooseAction, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 7, 0, DYN_MULTICHOICE_CB_NONE, TrainingNpc_Text_Add4, TrainingNpc_Text_Add16, TrainingNpc_Text_Add64, TrainingNpc_Text_Max, TrainingNpc_Text_ResetStat, TrainingNpc_Text_ResetAll, TrainingNpc_Text_Back
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, TrainingNpc_MainMenu
	goto_if_eq VAR_RESULT, 6, TrainingNpc_MainMenu
	copyvar VAR_0x8006, VAR_RESULT
	callnative TrainingNpc_ApplyEV
	goto_if_eq VAR_RESULT, 2, TrainingNpc_EVAtCap
	msgbox TrainingNpc_Text_EVDone, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu

TrainingNpc_EXP:
	call TrainingNpc_ChooseMon
	goto_if_eq VAR_RESULT, FALSE, TrainingNpc_MainMenu
	msgbox TrainingNpc_Text_ChooseExp, MSGBOX_DEFAULT
	closemessage
	dynmultichoice 1, 1, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, TrainingNpc_Text_AddLevel1, TrainingNpc_Text_AddLevel10, TrainingNpc_Text_ToLevelCap, TrainingNpc_Text_Back
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, TrainingNpc_MainMenu
	goto_if_eq VAR_RESULT, 3, TrainingNpc_MainMenu
	copyvar VAR_0x8005, VAR_RESULT
	callnative TrainingNpc_StartExpTraining
	goto_if_eq VAR_RESULT, 2, TrainingNpc_EXPAtCap
	waitstate
	goto TrainingNpc_MainMenu

TrainingNpc_Friendship:
	call TrainingNpc_ChooseMon
	goto_if_eq VAR_RESULT, FALSE, TrainingNpc_MainMenu
	callnative TrainingNpc_ApplyFriendship
	goto_if_eq VAR_RESULT, 2, TrainingNpc_AlreadyOptimized
	msgbox TrainingNpc_Text_FriendshipDone, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu

TrainingNpc_Nature:
	call TrainingNpc_ChooseMon
	goto_if_eq VAR_RESULT, FALSE, TrainingNpc_MainMenu
	msgbox TrainingNpc_Text_ChooseNature, MSGBOX_DEFAULT
	closemessage
	callnative TrainingNpc_BuildNatureMenu
	setvar VAR_0x8005, 0
	dynmultistack 1, 1, FALSE, 7, FALSE, VAR_0x8005, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, TrainingNpc_MainMenu
	copyvar VAR_0x8005, VAR_RESULT
	callnative TrainingNpc_BufferNatureName
	msgbox TrainingNpc_Text_ConfirmNature, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, TrainingNpc_MainMenu
	callnative TrainingNpc_ApplyNature
	goto_if_eq VAR_RESULT, 2, TrainingNpc_AlreadyOptimized
	goto_if_eq VAR_RESULT, 3, TrainingNpc_NotEnoughMoney
	msgbox TrainingNpc_Text_NatureDone, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu

TrainingNpc_Ability:
	call TrainingNpc_ChooseMon
	goto_if_eq VAR_RESULT, FALSE, TrainingNpc_MainMenu
	msgbox TrainingNpc_Text_ChooseAbility, MSGBOX_DEFAULT
	closemessage
	callnative TrainingNpc_BuildAbilityMenu
	dynmultistack 1, 1, FALSE, 3, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, TrainingNpc_MainMenu
	copyvar VAR_0x8005, VAR_RESULT
	callnative TrainingNpc_BufferAbilityName
	callnative TrainingNpc_ApplyAbility
	goto_if_eq VAR_RESULT, 2, TrainingNpc_AlreadyOptimized
	msgbox TrainingNpc_Text_AbilityDone, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu

TrainingNpc_AlreadyOptimized:
	msgbox TrainingNpc_Text_AlreadyOptimized, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu
TrainingNpc_EVAtCap:
	msgbox TrainingNpc_Text_EVAtCap, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu
TrainingNpc_EXPAtCap:
	msgbox TrainingNpc_Text_EXPAtCap, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu
TrainingNpc_NotEnoughMoney:
	msgbox TrainingNpc_Text_NotEnoughMoney, MSGBOX_DEFAULT
	goto TrainingNpc_MainMenu
TrainingNpc_Exit:
	msgbox TrainingNpc_Text_Goodbye, MSGBOX_DEFAULT
	release
	end

TrainingNpc_Text_Welcome:
	.string "Training Pokémon !\nQuel service veux-tu ?$"
TrainingNpc_Text_IV:
	.string "IV$"
TrainingNpc_Text_EV:
	.string "EV$"
TrainingNpc_Text_EXP:
	.string "EXP$"
TrainingNpc_Text_Friendship:
	.string "Bonheur$"
TrainingNpc_Text_Nature:
	.string "Nature$"
TrainingNpc_Text_Ability:
	.string "Talent$"
TrainingNpc_Text_Quit:
	.string "Quitter$"
TrainingNpc_Text_HP:
	.string "PV$"
TrainingNpc_Text_Attack:
	.string "Attaque$"
TrainingNpc_Text_Defense:
	.string "Défense$"
TrainingNpc_Text_SpAttack:
	.string "Atq. Spé.$"
TrainingNpc_Text_SpDefense:
	.string "Déf. Spé.$"
TrainingNpc_Text_Speed:
	.string "Vitesse$"
TrainingNpc_Text_All:
	.string "Tout maximiser$"
TrainingNpc_Text_Back:
	.string "Retour$"
TrainingNpc_Text_Add4:
	.string "+4$"
TrainingNpc_Text_Add16:
	.string "+16$"
TrainingNpc_Text_Add64:
	.string "+64$"
TrainingNpc_Text_Max:
	.string "MAX POSSIBLE$"
TrainingNpc_Text_ResetStat:
	.string "RESET STAT$"
TrainingNpc_Text_ResetAll:
	.string "RESET ALL EVs$"
TrainingNpc_Text_AddLevel1:
	.string "+1 niveau$"
TrainingNpc_Text_AddLevel10:
	.string "+10 niveaux$"
TrainingNpc_Text_ToLevelCap:
	.string "Niveau plafond$"
TrainingNpc_Text_ChooseStat:
	.string "Quelle statistique ?$"
TrainingNpc_Text_ChooseAction:
	.string "Quelle action ?$"
TrainingNpc_Text_ChooseExp:
	.string "Combien de niveaux ?$"
TrainingNpc_Text_ChooseNature:
	.string "Choisis la nouvelle Nature.$"
TrainingNpc_Text_ChooseAbility:
	.string "Choisis un Talent disponible.$"
TrainingNpc_Text_ConfirmNature:
	.string "La Nature {STR_VAR_1} coûte 5 000 ¥.\nConfirmer ?$"
TrainingNpc_Text_IVDone:
	.string "Les IV sélectionnés sont maintenant\nau maximum : 31 !$"
TrainingNpc_Text_EVDone:
	.string "Les EV ont été modifiés dans les\nlimites légales.$"
TrainingNpc_Text_EXPDone:
	.string "Ce Pokémon atteint maintenant le\nniveau {STR_VAR_1} !$"
TrainingNpc_Text_FriendshipDone:
	.string "Son Bonheur est maintenant au\nmaximum !$"
TrainingNpc_Text_NatureDone:
	.string "La Nature est maintenant\n{STR_VAR_1} !$"
TrainingNpc_Text_AbilityDone:
	.string "Le Talent est maintenant\n{STR_VAR_1} !$"
TrainingNpc_Text_AlreadyOptimized:
	.string "Ce Pokémon possède déjà ce réglage.$"
TrainingNpc_Text_EVAtCap:
	.string "Impossible : cette statistique ou le\ntotal des EV est déjà au maximum.$"
TrainingNpc_Text_EXPAtCap:
	.string "Ce Pokémon a déjà atteint le\nniveau plafond actuel.$"
TrainingNpc_Text_NotEnoughMoney:
	.string "Il faut 5 000 ¥ pour changer de\nNature.$"
TrainingNpc_Text_NoEggs:
	.string "Je ne peux pas entraîner un Œuf.$"
TrainingNpc_Text_Goodbye:
	.string "Reviens quand tu veux optimiser\nun Pokémon !$"
