# Systèmes de jeu V1

## Déjà validés : tests de non-régression

- Vitesse native x1/x2/x3/x4 sans accélérer la musique.
- Volumes musique et effets indépendants ; comportement du preset Recommended et leur sauvegarde.
- Menu Shiny Rate **exactement tel qu'il existe dans le build testé** : choix originels et ajouts validés. Relever ses valeurs dans le code, sans les redéfinir ici ni lui substituer la liste d'un ancien cahier des charges.
- Sélecteur starter commun et œuf d'Orme : choix, annulation, doublons interdits, équipe/PC pleins et file de récompenses.
- Évolutions sans échange et leurs minimums de niveau déjà testés.

Un défaut observé se corrige au plus près du code fautif, avec test ciblé. Aucun remaniement du moteur audio ou de vitesse n'est demandé.

## Réglages et entraînement

Conserver la structure réellement validée : `Mode / Features / Randomizer / Nuzlocke / Difficulty / Challenges`. Vérifier les textes et l'emplacement effectif des options avant toute correction. HARD par défaut ; NORMAL accessible. L'assistant centralisé prépare EXP jusqu'au cap, IV, EV, remise à zéro EV, nature, talent et bonheur. Les rencontres du milieu de jeu doivent pouvoir être entraînées rapidement pour les boss suivants.

## Après V1

Ajouter `Shiny Only — ON/OFF` au menu Nuzlocke, en s'appuyant sur le moteur shiny existant. Tester sauvegarde/chargement et interactions avec le taux choisi. Aucun code ou menu Shiny Only supplémentaire n'est nécessaire à la V1.
