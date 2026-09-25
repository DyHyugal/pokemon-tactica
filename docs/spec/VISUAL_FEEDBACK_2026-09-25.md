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
