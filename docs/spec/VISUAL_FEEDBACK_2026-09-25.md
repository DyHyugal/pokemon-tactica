# Retours visuels et UX de playtest — 25–26 septembre 2026

Ce document consolide les retours owner observés en ROM jusqu'au badge 4 et les recontrôles du 26 septembre. Il complète `SOURCE_OF_TRUTH.md` et prime sur l'ancien état visuel importé.

## Direction visuelle globale

La palette custom Tactica est **noir / rouge / gris**. Les interfaces custom ne doivent pas conserver de grandes surfaces blanches/blanc cassé héritées de HnS.

## Menu starter

État observé le 26/09 :

- à l'ouverture de certaines listes, le curseur peut se retrouver sur `Retour` au lieu du haut ;
- après sélection puis annulation de la confirmation, la plupart des espèces reviennent correctement sur l'espèce précédente, mais **Élekid peut renvoyer sur `Retour`** ;
- ce comportement est incohérent et doit être corrigé.

Attendu :

- première ouverture d'une liste d'espèces = curseur sur la **première entrée** ;
- après annulation d'un choix = curseur restauré sur **exactement l'espèce précédemment sélectionnée** ;
- `Retour` reste la dernière entrée et n'est jamais une position initiale implicite ;
- tester explicitement Élekid et au moins une autre espèce de chaque catégorie.

## Menus hors combat et boutiques

- Le menu principal ne doit jamais corrompre les tuiles de carte.
- Le menu principal reste fond rouge / texte noir / séparateurs sombres.
- Les menus custom des boutiques observés le 26/09 utilisent encore des panneaux sombres ou gris et du texte/surlignage blanc hérités de l'UI générique.
- Attendu pour les menus custom de boutique : **fond rouge, texte noir**, sélection distincte sans grand surlignage blanc.
- Les fenêtres de dialogue et listes doivent rester lisibles avec les textes longs.

## Interface de combat

État observé le 26/09 :

- les healthboxes allié/adversaire ont bien été assombries ;
- le panneau d'actions `ATTAQUE / SAC / ÉQUIPE / FUITE` conserve encore de grandes surfaces gris/blanc ;
- le panneau des capacités conserve encore une forte présence de blanc/gris et n'est pas au niveau du rendu final attendu.

Attendu :

- healthboxes noires/sombres et lisibles ;
- barre EXP sombre avec progression rouge ;
- panneau d'actions harmonisé noir / rouge / gris, sans grande surface blanche ;
- panneau des capacités harmonisé de la même manière ;
- sélection clairement visible sans confondre couleur de fond, texte et curseur ;
- vérifier simples/doubles, états, noms longs, PP et types.

## Summary

Le Summary reste **À CORRIGER**. Les captures jusqu'au badge 4 montrent que le problème principal n'est pas seulement la palette : les fonds/tilemaps statiques de l'ancien Summary restent visibles sous les fenêtres dynamiques.

Symptômes :

- libellés statiques et données dynamiques se superposent ;
- la bascule `STATS / IV / EV` surcharge la page ;
- les anciennes zones `BATTLE MOVES / CONTEST MOVES`, catégories concours, PP et cadres donnent un rendu chargé ;
- de grandes surfaces blanc cassé restent visibles.

Priorité : corriger la composition BG/tilemaps/windows avant de retoucher les couleurs.

Si nécessaire, décision owner validée :

- conserver `BATTLE MOVES` pour les attaques ;
- remplacer la page `CONTEST MOVES` par une **page IV/EV dédiée** ;
- afficher clairement les 6 IV et 6 EV, avec total EV / nature si l'espace le permet ;
- supprimer les éléments concours non utiles à Tactica.

## Rival — premier combat

Observation 26/09 : le premier rival reste niveau 5.

Attendu :

- l'équipe reste composée uniquement du starter rival ;
- niveau du starter rival = **17**, correspondant au cap de préparation d'Albert ;
- son stade d'évolution doit être légal au niveau 17 ;
- les attaques doivent être légales à ce stade/niveau.

Ce point est un écart gameplay, pas uniquement un polish UX.

## Encounters — Route 36

Observation 26/09 : le propriétaire a eu l'impression de retrouver les anciennes rencontres de Route 36.

Comme la ROM utilisée peut avoir été un ancien `pokehns.gba` non recompilé après `git pull`, cette observation doit être **retestée sur une ROM fraîche** avant de conclure que les JSON/runtime divergent.

Contrôle obligatoire :

- compiler depuis le SHA annoncé ;
- relever les quatre slots Route 36 jour et nuit ;
- les comparer à `data/spec/encounters_standard.json` / données moteur générées ;
- si la ROM ne correspond pas aux données canoniques, bloquer la candidate et diagnostiquer génération/runtime.

## Mortimer / Méga

Régression déjà observée au badge 4 :

`SRC/BATTLE_MAIN.C:2121: ILLEGAL ABILITY SHADOW TAG FOR GENGAR`

Attendu :

- Ectoplasma de base reçoit un talent légal avant Méga ;
- Ectoplasmite conservée ;
- la Méga est déclenchée réellement par l'IA ;
- Shadow Tag vient de la transformation ;
- audit d'au moins un second boss Méga avec changement de talent.

Mortimer doit également donner le **Mega Ring après le badge 4**, avant la CT / sortie de l'arène selon le flow validé, afin que le joueur puisse Méga-évoluer avant le badge 5.

## Contrôles de recette de la prochaine candidate

- starter : ouverture en haut, annulation sur la même espèce, Élekid inclus ;
- menu principal : aucune corruption VRAM ;
- boutiques : fond rouge / texte noir ;
- combat : healthboxes, actions et capacités cohérents avec la charte ;
- Summary : toutes les pages lisibles, sans superpositions ;
- premier rival : starter seul niveau 17, stade légal ;
- Route 36 : slots runtime = données canoniques ;
- Mortimer : aucune assert Méga, transformation effective ;
- après badge 4 : Mega Ring reçu et Méga joueur utilisable.
