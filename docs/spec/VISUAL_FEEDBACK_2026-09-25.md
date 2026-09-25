# Retours visuels de playtest — 25 septembre 2026

Ce document retranscrit les retours utilisateur observés dans la ROM jusqu'au troisième badge. Il complète la règle de lisibilité de `SOURCE_OF_TRUTH.md` et prime sur l'ancien état visuel importé.

## Direction visuelle globale

La palette custom Tactica est **noir / rouge / gris**. Les interfaces custom ne doivent pas employer de grandes surfaces blanches ni un blanc dominant pour les textes/panneaux. Le contraste doit être obtenu avec les niveaux de gris, le noir et le rouge.

## Menus hors combat

- L'ouverture du menu principal et des sous-menus ne doit jamais altérer les tuiles de la carte, même pendant une seule frame. La corruption précédemment observée est confirmée corrigée en playtest.
- Le menu principal doit respecter la consigne **fond rouge / texte noir**, avec séparateurs sombres lisibles.
- Le sélecteur de starter est fonctionnel ; l'amélioration restante est uniquement ergonomique : après un retour, le curseur doit reprendre la dernière position au lieu de revenir au début.
- Les fenêtres des PNJ objets/CT sont fonctionnelles mais leur couleur de texte doit être harmonisée pour rester lisible dans la palette noir/rouge/gris.

## Interface de combat

- Le HUD doit rester dans la palette noir / rouge / gris, sans grand panneau blanc.
- Préférer un fond gris ou noir selon le panneau, avec texte noir ou rouge uniquement si le contraste reste suffisant.
- La couleur de sélection doit être distincte de la couleur du texte : si le texte est noir, le rouge peut servir d'indicateur ; éviter les combinaisons peu lisibles.
- La barre d'expérience utilise un fond sombre/gris et une progression rouge.
- Le panneau `ATTAQUE / SAC / ÉQUIPE / FUITE` et la grille des capacités doivent être harmonisés avec cette palette.

## Summary

Le Summary reste trop difficile à lire malgré les passes précédentes. Il doit être considéré comme **à corriger visuellement**, pas comme presque validé. Revoir contraste, fonds, texte, barres et sélection dans la palette noir/rouge/gris, sans blanc dominant.

## Contrôles de recette

- Ouvrir et fermer successivement menu principal, Pokédex, Sac et Équipe sur plusieurs cartes, sans corruption avant ou après déplacement.
- Vérifier le retour arrière dans toutes les étapes du sélecteur de starter et la conservation de la position du curseur.
- Vérifier en combat simple et double les HUD alliés/adverses, PV, statuts, EXP, menu d'actions et quatre capacités.
- Vérifier lisibilité des fenêtres PNJ objets/CT.
- Vérifier le Summary sur toutes ses pages avec noms longs, doubles types, talents, objets et valeurs IV/EV.
- Vérifier qu'aucune interface custom concernée ne réintroduit une grande surface blanche.

## Playtest jusqu'au badge 4 — Summary et Méga

### Summary : diagnostic visuel confirmé

Les captures du 25/09 jusqu'au badge 4 montrent que le problème principal n'est pas seulement la palette : les **fonds/tilemaps statiques de l'ancien Summary restent visibles sous les fenêtres dynamiques**, ce qui donne l'impression d'avoir une page Pokédex/Summary pré-imprimée en arrière-plan puis les vraies informations du Pokémon dessinées par-dessus.

Symptômes observés :
- page `POKÉMON SKILLS` : libellés statiques `START`, `STATS`, `BONHEUR`, colonnes de stats et encadrements restent visibles sous les valeurs dynamiques ;
- bascule `STATS / IV / EV` : les nombres et intitulés IV/EV se superposent aux libellés/tilemaps existants ;
- pages d'attaques : les anciennes zones `BATTLE MOVES / CONTEST MOVES`, catégories concours, PP et cadres donnent un rendu chargé et parfois illisible ;
- de grandes surfaces blanc cassé restent présentes et contredisent la palette noir / rouge / gris.

Priorité : **corriger la composition des BG/tilemaps/windows avant de retoucher les couleurs**. Il faut éviter d'empiler du texte dynamique sur un fond qui contient déjà les mêmes intitulés.

### Décision owner : page IV/EV dédiée autorisée

Si rendre `STATS / IV / EV` lisible sur une seule page impose trop de compromis, il est **explicitement autorisé et approuvé** de :
- conserver la page `BATTLE MOVES` pour les attaques ;
- **remplacer la dernière page `CONTEST MOVES` / concours (`COOL`, `BEAUTY`, etc.) par une page dédiée IV/EV** ;
- utiliser cette page pour afficher clairement les 6 IV et 6 EV, avec éventuellement nature, total EV et rappels utiles si l'espace le permet ;
- supprimer les éléments concours non nécessaires à Tactica sur cette page ;
- rester dans le thème noir / rouge / gris et privilégier la lisibilité.

Ce remplacement est préférable à une page Skills surchargée si les contraintes GBA rendent le mode alterné `STATS / IV / EV` instable ou illisible.

### Incident Mortimer / Méga-Ectoplasma

Au lancement du combat de Mortimer, écran d'assert observé :

`SRC/BATTLE_MAIN.C:2121: ILLEGAL ABILITY SHADOW TAG FOR GENGAR`

Le joueur a pu appuyer sur START, le combat a ensuite démarré sans crash, mais **Ectoplasma n'a pas Méga-Évolué**.

L'inspection du dépôt montre un écart concret dans la génération des boss :
- `data/spec/bosses.json` décrit `Mega Gengar @ Gengarite` avec `Shadow Tag` ;
- `tools/sync_tactica_bosses.py::engine_species()` retire le préfixe `Mega ` et génère l'espèce de base ;
- `render_mon()` recopie ensuite le talent canonique de la Méga sur l'espèce de base ;
- le moteur reçoit donc actuellement `Gengar @ Gengarite / Ability: Shadow Tag`, alors que Shadow Tag n'est pas un talent légal d'Ectoplasma avant sa Méga-Évolution.

Ce défaut est **bloquant pour la recette V1 des boss à Méga** et peut être systémique : auditer tous les boss dont le talent change à la Méga-Évolution (ex. Méga-Ectoplasma, Méga-Steelix, Méga-Dracaufeu X/Y, Méga-Roucarnage, etc.).

Attendu :
- l'espèce de base doit avoir un **talent légal avant Méga-Évolution** ;
- la Méga-Gemme doit rester tenue ;
- la Méga-Évolution du dresseur doit être effectivement déclenchée ;
- le talent de la forme Méga doit venir de la transformation, pas être injecté illégalement sur la forme de base ;
- ajouter au moins un test automatisé avec Ectoplasma et un autre cas où le talent de la Méga diffère du talent de base.
