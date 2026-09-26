# Pokémon Tactica — source de vérité V1

État : décisions produit consolidées au 26 septembre 2026. Cette documentation et les fichiers `data/spec/` décrivent **l'objectif** ; le code importé décrit uniquement son état actuel. `docs/spec/` fixe les règles, `data/spec/` fixe les listes et valeurs. Une donnée absente ne doit pas être inventée. En cas de contradiction, signaler le conflit avant de modifier le comportement concerné.

## Périmètre

Pokémon Tactica est un fork jouable de HnS tourné vers les combats stratégiques, la composition d'équipe et une progression sans grind. Le code HnS peut être modifié lorsqu'il empêche un comportement Tactica approuvé. Conserver ses crédits, licences et identifiants techniques nécessaires au build. Renommer seulement l'identité visible du projet et les fichiers qui lui appartiennent, sans remplacement global aveugle.

## Contrats V1

1. Les rencontres utilisent **quatre vrais slots par table et par méthode applicable**, pondérés `30/30/30/10`. Les cannes contrôlent l'accès ; la source actuelle donne un pool logique de quatre espèces par table de pêche. Les 53 pools Safari tournent par secteur/session et les 4 tables Headbutt restent distincts. Détails : [ENCOUNTERS.md](ENCOUNTERS.md) et JSON canoniques.
2. NORMAL et HARD ont le même contenu : équipes, espèces, niveaux, capacités, objets, talents, natures, progression et rencontres. HARD ajoute IV 31 pour les boss fixes, EV légaux selon les rôles et IA stratégique loyale ; NORMAL garde son optimisation native. HARD est le choix par défaut et NORMAL reste disponible.
3. Albert a 3 Pokémon, Hector 4, Blanche et les champions suivants 6. Jusqu'à deux soins lorsqu'ils sont prévus, avec des objets appropriés au stade de la progression native. Les formes et capacités doivent être légales au niveau du combat.
4. Rocket : chaque boss prend **le dernier level cap canonique d'un champion ou d'un rival jalon, plus 2**. Un combat Rocket ne devient jamais le référentiel du Rocket suivant. Réduire automatiquement une forme finale illégale à sa pré-évolution valide.
5. Starter : trente choix répartis en six catégories et Évoli en dehors du tirage rival ; l'œuf d'Orme donne un second choix. Le rival tire aléatoirement une catégorie qui contre celle du joueur, puis un starter au hasard dans cette catégorie ; son équipe suit progressivement l'archétype météo/terrain lié à son propre starter. **Le premier combat rival reste un 1v1 starter, mais le starter rival est niveau 17 et prend son stade d'évolution légal à ce niveau.** Voir [STARTERS_RIVAL_EVOLUTIONS.md](STARTERS_RIVAL_EVOLUTIONS.md) et [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md).
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

Validation avant candidate owner : validateurs + synchronisations `--check`, groupes Settings/Audio/Native/Family/Evolution/Level Caps, boss NORMAL/HARD et Rocket, encounters standard/Safari/Headbutt, shops/rival, `make clean && make hns -j4`, puis checklist ROM de [PLAYTEST_STATUS.md](PLAYTEST_STATUS.md). Une vérification manuelle n'est déclarée faite que si elle a réellement eu lieu.
