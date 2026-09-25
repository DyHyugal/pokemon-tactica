# Pokémon Tactica — source de vérité V1

État : décisions produit validées le 24 septembre 2026. Cette documentation et les fichiers `data/spec/` décrivent **l'objectif** ; le code importé décrit uniquement son état actuel. `docs/spec/` fixe les règles, `data/spec/` fixe les listes et valeurs. Une donnée absente ne doit pas être inventée. En cas de contradiction, signaler le conflit avant de modifier le comportement concerné.

## Périmètre

Pokémon Tactica est un fork jouable de HnS tourné vers les combats stratégiques, la composition d'équipe et une progression sans grind. Le code HnS peut être modifié lorsqu'il empêche un comportement Tactica approuvé. Conserver ses crédits, licences et identifiants techniques nécessaires au build. Renommer seulement l'identité visible du projet et les fichiers qui lui appartiennent, sans remplacement global aveugle.

## Contrats V1

1. Les rencontres utilisent **quatre vrais slots par table et par méthode applicable**, pondérés `30/30/30/10`. Les cannes contrôlent l'accès ; la source actuelle donne un pool logique de quatre espèces par table de pêche. Les 53 pools Safari tournent par secteur/session et les 4 tables Headbutt restent distincts. Détails : [ENCOUNTERS.md](ENCOUNTERS.md) et JSON canoniques.
2. NORMAL et HARD ont le même contenu : équipes, espèces, niveaux, capacités, objets, talents, natures, progression et rencontres. HARD ajoute IV 31 pour les boss fixes, EV légaux selon les rôles et IA stratégique loyale ; NORMAL garde son optimisation native. HARD est le choix par défaut et NORMAL reste disponible.
3. Albert a 3 Pokémon, Hector 4, Blanche et les champions suivants 6. Jusqu'à deux soins lorsqu'ils sont prévus, avec des objets appropriés au stade de la progression native. Les formes et capacités doivent être légales au niveau du combat.
4. Rocket : chaque boss prend **le dernier level cap canonique d'un champion ou d'un rival jalon, plus 2**. Un combat Rocket ne devient jamais le référentiel du Rocket suivant. Réduire automatiquement une forme finale illégale à sa pré-évolution valide.
5. Starter : trente choix répartis en six catégories et Évoli en dehors du tirage rival ; l'œuf d'Orme donne un second choix. Le rival tire aléatoirement une catégorie qui contre celle du joueur, puis un starter au hasard dans cette catégorie ; son équipe suit progressivement l'archétype météo/terrain lié à son propre starter. Voir [STARTERS_RIVAL_EVOLUTIONS.md](STARTERS_RIVAL_EVOLUTIONS.md) et [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md) pour les équipes progressives Rocket/rival.
6. Les vitesses natives x1 à x4, l'audio indépendant, les taux shiny présents, le choix starter/œuf et les évolutions solo déjà testés sont **validés et protégés contre les réécritures**. Vérifier leur non-régression. `Shiny Only` est **après la V1 stable** ; future ligne `Shiny Only — ON/OFF` dans le menu Nuzlocke. Ne pas l'ajouter au sprint V1.
7. Les boutiques spécialisées rendent accessibles toutes les capacités du build compatibles avec le Pokémon et tous les objets utiles non vendus par les marchands normaux, hors objets de scénario et incohérences de progression. Une boutique de Méga-Gemmes dédiée, proche du PNJ capacités, est requise si elle n'existe pas déjà. Voir [SHOPS.md](SHOPS.md).
8. Les Pokémon modifiés et les 15 entrées de learnset de `data/spec/pokemon_balance.json` sont le contrat de balance. Dracaufeu a 110 Atk ; Méga-Dracaufeu X a 156 Atk ; Méga Y conserve les chiffres du dataset v4, sans ces dix points supplémentaires.
9. Chaque Méga de boss important est unique, sauf dérogation explicitement écrite. Pierre garde Méga-Steelix ; Jasmine reçoit Méga-Galeking avec un set de **Méga-Galeking** éprouvé. Vérifier la disponibilité réelle des Méga propres au build.
10. FR et EN sont des miroirs, avec les noms officiels Pokémon et une interface lisible. Le wiki tire sa palette des vraies valeurs de la ROM ; Guide de jeu, Changements, Pokédex, Localisations et Boss & Conseils ont chacun un rôle propre.
11. Les retours de playtest datés du 25 septembre 2026 fixent la lisibilité rouge/noir de l'interface et les règles détaillées d'équipes, de caps et d'accessibilité. Voir [VISUAL_FEEDBACK_2026-09-25.md](VISUAL_FEEDBACK_2026-09-25.md) et [STRATEGIC_TEAMS_AND_ACCESS.md](STRATEGIC_TEAMS_AND_ACCESS.md).

## Livraison

Le premier travail de Codex est un audit **code contre spec** `DONE / PARTIEL / À CORRIGER / À FAIRE` par fichier, avec état de tests et différences de données. Il maintient [FEATURE_STATUS.md](FEATURE_STATUS.md), un tag et des preuves par feature, selon la provenance et le diagnostic de [MIGRATION.md](MIGRATION.md). Il poursuit ensuite par blocs via branches fonctionnelles et PR vers `integration/v1`. Aucun système protégé n'est repris sans écart démontré. Une CI verte sur l'ancien mapping des encounters ne prouve pas le contrat à quatre slots.

Validation avant release : validator corrigé, `make hns -j4`, groupes Settings/Audio/Native/Family/Evolution/Level Caps, boss NORMAL/HARD et Rocket, encounters standard/Safari/Headbutt, tests shops/rival, playthrough mGBA représentatif, revue UI et wiki FR/EN sur mobile. Une vérification manuelle n'est déclarée faite que si elle a réellement eu lieu.
