# Pokémon Tactica — source de vérité V1

État : décisions produit consolidées au 26 septembre 2026. La hiérarchie normative est : **1)** ce fichier pour les règles globales et arbitrages ; **2)** les documents métier `docs/spec/*.md` pour les décisions détaillées ; **3)** `data/spec/*.json` comme représentation machine ; **4)** code/runtime généré. Une donnée absente ne doit pas être inventée. Une contradiction document -> JSON peut exister uniquement comme dette temporaire explicitement signalée et doit être supprimée par la PR d'implémentation qui régénère les données.

## Périmètre

Pokémon Tactica est un fork jouable de HnS tourné vers les combats stratégiques, la composition d'équipe et une progression sans grind. Le code HnS peut être modifié lorsqu'il empêche un comportement Tactica approuvé. Conserver ses crédits, licences et identifiants techniques nécessaires au build. Renommer seulement l'identité visible du projet et les fichiers qui lui appartiennent, sans remplacement global aveugle.

## Contrats V1

1. Les rencontres utilisent **quatre vrais slots par table et par méthode applicable**, pondérés `30/30/30/10`. **Le niveau est fixé par le premier accès de la zone dans la progression de l'histoire ; le déblocage ultérieur d'une méthode n'augmente pas ce niveau.** Une zone accessible avant Albert est donc calibrée au cap 17 (ex. Route 36 : 14–17 max avec stades correspondants). Les cannes/CS contrôlent l'accès aux méthodes, pas le scaling de la zone. Détails : [ENCOUNTERS.md](ENCOUNTERS.md).
2. NORMAL et HARD ont le même contenu : équipes, espèces, niveaux, capacités, objets, talents, natures, progression et rencontres. HARD ajoute IV 31 pour les boss fixes, EV légaux selon les rôles et IA stratégique loyale ; NORMAL garde son optimisation native. HARD est le choix par défaut et NORMAL reste disponible.
3. Albert a 3 Pokémon, Hector 4, Blanche et les champions suivants 6. Jusqu'à deux soins lorsqu'ils sont prévus, avec des objets appropriés au stade de la progression native. Les formes et capacités doivent être légales au niveau du combat.
4. Rocket : chaque boss prend **le dernier level cap canonique d'un champion ou d'un rival jalon, plus 2**. Un combat Rocket ne devient jamais le référentiel du Rocket suivant. Réduire automatiquement une forme finale illégale à sa pré-évolution valide.
5. Starter : trente choix répartis en six catégories et Évoli pour le joueur ; l'œuf d'Orme donne un second choix. **Le rival ne tire plus une espèce au hasard parmi les cinq starters d'une catégorie** : il tire une seule fois un archétype autorisé selon la catégorie du joueur, puis cet archétype impose son starter fixe. Correspondances owner : Feu -> Eau ou Sol ; Eau -> Plante ou Électrik ; Plante -> Feu ou Glace ; Électrik -> Sol ou Plante ; Sol -> Eau ou Glace ; Glace -> Feu ou Eau. Le premier combat reste un 1v1 avec ce starter fixe au niveau 17 et son stade légal. Voir [STARTERS_RIVAL_EVOLUTIONS.md](STARTERS_RIVAL_EVOLUTIONS.md) et [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md).
6. Les vitesses natives x1 à x4, l'audio indépendant, les taux shiny présents, le cœur du choix starter/œuf et les évolutions solo déjà testés sont protégés contre les réécritures. Les défauts UX du sélecteur (position initiale / retour après annulation) restent néanmoins des corrections ciblées à effectuer. `Shiny Only` est après la V1 stable.
7. Les boutiques spécialisées rendent accessibles toutes les capacités du build compatibles avec le Pokémon et tous les objets utiles non vendus par les marchands normaux, hors objets de scénario et incohérences de progression. Une boutique de Méga-Gemmes dédiée est requise. Les **menus custom de boutique** respectent la charte rouge/noir : fond rouge, texte noir, sélection lisible sans grand surlignage blanc.
8. Les Pokémon modifiés et les 15 entrées de learnset de `data/spec/pokemon_balance.json` sont le contrat de balance. Dracaufeu a 110 Atk ; Méga-Dracaufeu X a 156 Atk ; Méga Y conserve les chiffres du dataset v4.
9. Chaque Méga de boss important est unique, sauf dérogation explicitement écrite. Une forme de base ne peut jamais recevoir illégalement le talent de sa Méga avant transformation. La Méga-Gemme reste tenue et le talent Méga provient de la transformation. Mortimer est un cas de contrôle obligatoire.
10. Les contenus documentaires FR et EN sont des miroirs, avec les noms officiels Pokémon et une interface lisible. La ROM V1 testable reste anglaise ; une ROM française distincte est reportée à la V1.1/V2.
11. Les retours de playtest des 25–26 septembre 2026 fixent la direction UI : **noir / rouge / gris, sans grande surface blanche**. Le Summary doit être recomposé proprement (BG/tilemaps/windows) ; la page `CONTEST MOVES` peut être remplacée par une page IV/EV dédiée. Les healthboxes sombres seules ne suffisent pas : panneaux action/attaques et fenêtres custom doivent suivre la même charte.
12. Le sélecteur starter ouvre une liste d'espèces avec le curseur **en haut**. Après annulation de la confirmation d'un Pokémon, il revient sur **ce même Pokémon** pour toutes les espèces ; `Retour` reste la dernière ligne et n'est jamais la position initiale par défaut.
13. Mortimer remet le **Mega Ring après le badge 4 puis la CT** ; le joueur doit pouvoir utiliser une Méga immédiatement avant le badge 5.
14. La conformité d'une feature qui dépend du rendu/runtime ne peut pas être déduite d'un JSON ou d'un test statique. Les encounters, l'UI, les boss Méga et les flows de menu doivent être vérifiés dans une ROM fraîche correspondant au SHA annoncé.

## Livraison

Le premier travail de Codex est un audit **code contre spec** `DONE / PARTIEL / À CORRIGER / À FAIRE` par fichier, avec état de tests et différences de données. Il maintient [FEATURE_STATUS.md](FEATURE_STATUS.md) et [PLAYTEST_STATUS.md](PLAYTEST_STATUS.md).

Une candidate owner n'existe que si toutes les corrections annoncées sont fusionnées dans `integration/v1`, la CI est verte sur le SHA exact, puis une ROM fraîche est reconstruite. **Un simple `git pull` ne met pas à jour `pokehns.gba`**, car les `*.gba` et `build/` sont ignorés par Git.

Validation avant candidate owner : validateurs + synchronisations `--check`, suite complète d'intégration (Settings/Audio/Native/Family/Evolution/Level Caps, boss NORMAL/HARD et Rocket, encounters standard/Safari/Headbutt, shops/rival), `make clean && make hns -j4`, puis checklist ROM de [PLAYTEST_STATUS.md](PLAYTEST_STATUS.md). Une vérification manuelle n'est déclarée faite que si elle a réellement eu lieu.


## Décision owner 26-09-2026 — Rival et Team Rocket

Les compositions finales et sets du rival et des Exécutifs Rocket sont désormais définis par [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md). En cas de contradiction avec `data/spec/rival.json`, `data/spec/rocket_progression.json` ou les entrées Rocket de `data/spec/bosses.json`, **ROSTERS_ROCKET_RIVAL.md prévaut jusqu'à régénération de ces JSON**.

Le rival utilise un starter fixe par archétype, développe progressivement son équipe 1 -> 3 -> 4 -> 6 et n'utilise sa Méga qu'après le badge 4. Les Exécutifs Rocket suivent 3 -> 4 -> 6 et n'utilisent leur Méga qu'en FINAL. Toute équipe météo/terrain du rival possède deux setters, avec exception documentée du Terrain Électrique où seul Wattapik dispose de Créa-Élec parmi les non-légendaires du build et Salarsen assure le second setup manuellement. Archer utilise **Méga-Sharpedo** en FINAL ; Méga-Démolosse reste réservé à Marion. Méga-Sharpedo Méga-évolue immédiatement lorsqu'il entre en jeu : aucun plan Abri -> Méga retardée n'est requis ni autorisé.


## Politique de validation

La validation est proportionnée au risque. Une PR docs/wiki exécute uniquement les contrôles documentaires/localisation. Une PR de code ou de données exécute validateurs, contrôles de synchronisation, build HnS et smoke tests ; les tests métier ciblés sont exécutés pendant le développement et consignés dans la PR/statut. Chaque push fusionné sur `integration/v1` ou `main` rejoue la suite complète de régression. La recette owner et le clean build complet ne sont pas une étape de chaque PR : ils sont réservés aux candidates explicitement déclarées dans `PLAYTEST_STATUS.md`.
