# Pokémon Tactica — source de vérité V1

État : décisions produit courantes au 26 septembre 2026.

La hiérarchie normative est stricte :

1. `docs/spec/SOURCE_OF_TRUTH.md` pour les règles globales ;
2. les documents métier `docs/spec/*.md` pour le détail ;
3. `data/spec/*.json` comme représentation machine ;
4. le code/runtime généré.

Les documents actifs décrivent uniquement l’état attendu actuel. L’historique appartient à Git et, lorsqu’il est utile à la migration, à `MIGRATION.md`.

## Périmètre

Pokémon Tactica est un remake/fork jouable de la 2G basé sur HnS, centré sur les combats stratégiques, la composition d’équipe et une progression avec peu de grind. Le code HnS peut être modifié lorsqu’il empêche un comportement Tactica approuvé. Les crédits, licences et identifiants techniques nécessaires au build sont conservés.

## Contrats V1

### Rencontres

- Chaque table/méthode applicable contient quatre vrais slots pondérés `30/30/30/10`.
- Le niveau vient du premier accès réel à la zone. Surf, Pêche ou Éclate-Roc débloquent une méthode mais ne rehaussent pas artificiellement une ancienne zone.
- Route 36 est calibrée `14–17`.
- Habitat et stade d’évolution doivent rester cohérents avec la zone et le niveau.
- Aucune famille starter n’est disponible avant le badge 2.
- Aucun légendaire/fabuleux n’est ajouté comme encounter standard.
- Scorplane est disponible early : Route 36, herbe/sol, jour, slot 30 %.
- Wattouat reste disponible avant la Ligue : Route 31, herbe/sol, jour, slot 30 %.
- Objectif de couverture : une espèce pertinente doit être disponible avant la première Ligue. Lorsqu’un doublon occupe un slot alors qu’une espèce reste uniquement post-Ligue, le doublon est le premier candidat au remplacement, sous réserve d’habitat, progression, niveau et stade cohérents.
- La couverture globale pré-Ligue est un chantier de rééquilibrage dédié : ne pas randomiser les tables pour faire monter artificiellement un compteur.

Détail : [ENCOUNTERS.md](ENCOUNTERS.md).

### Difficulté et boss

- NORMAL et HARD utilisent le même contenu : équipes, espèces, niveaux, moves, objets, talents, natures, progression et rencontres.
- HARD ajoute IV 31, EV stratégiques légaux et une IA plus exigeante ; NORMAL conserve l’optimisation native.
- Albert a 3 Pokémon, Hector 4, Blanche et les Champions suivants 6.
- Les objets tenus sont uniques au sein d’une équipe.
- Les formes et moves doivent être légaux au niveau du combat.
- Les soins de boss existants sont conservés, avec au plus deux soins lorsqu’ils sont prévus.

### Méga-Évolution

- À partir de Mortimer inclus, chaque Champion possède exactement une Méga.
- Une Méga importante est unique globalement entre Champions, Conseil 4, Rival et Rocket sauf exception owner explicitement écrite.
- La forme de base doit entrer avec un talent légal ; le talent de Méga vient de la transformation.
- Mortimer remet le Mega Ring après le badge 4 puis la CT.
- Jeannine utilise Méga-Kravarech @ Dragalgite comme ace. Kravarech entre avec un talent de base légal ; Méga-Kravarech reçoit Adaptabilité à la transformation.

### Rival et Team Rocket

La source détaillée est [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md). Les JSON `rival.json`, `rocket_progression.json` et `bosses.json` doivent rester synchronisés avec ce document.

- Le rival possède un starter fixe par archétype, une progression d’équipe `1 → 3 → 4 → 6` et sa Méga seulement après le badge 4.
- Les Exécutifs Rocket suivent `3 → 4 → 6` et n’utilisent une Méga qu’en FINAL.
- Archer utilise Méga-Sharpedo et Méga-évolue immédiatement ; aucun plan Abri → Méga retardée.
- Méga-Démolosse reste réservé à Marion.

### Starters et systèmes protégés

- Le joueur choisit parmi 30 starters répartis en six catégories, plus Évoli.
- L’œuf d’Orme fournit un second choix distinct du starter principal.
- Le sélecteur ouvre en haut de liste et revient sur le Pokémon annulé ; `Retour` reste la dernière ligne.
- Vitesse native x1/x2/x3/x4, audio indépendant, Shiny Rate, évolutions solo et cœur du flow starter/œuf sont protégés contre les réécritures sans défaut démontré.
- `Shiny Only` reste post-V1.

### Boutiques et balance

- Les boutiques spécialisées couvrent moves/CT compatibles, objets stratégiques et Méga-Gemmes sans contourner les objets de scénario.
- Les 26 espèces modifiées et les 15 changements de learnset de `pokemon_balance.json` sont le contrat de balance courant.

### UI / UX ROM

Direction : noir / rouge / gris, sans grande surface blanche.

- Summary : recomposition structurelle BG/tilemaps/windows ; aucune information en double ; une page IV/EV peut remplacer `CONTEST MOVES`.
- HUD combat : healthbox visuellement unifiée ; le rectangle blanc à gauche de la barre de PV doit disparaître ; panneaux action/attaques alignés sur la charte.
- Boutiques : fond rouge, texte noir, sélection lisible, pas de grand rectangle blanc.
- Une validation statique ne suffit pas pour déclarer une UI terminée : le rendu doit être observé dans une ROM correspondant au SHA testé.

### Wiki joueur

La baseline actuelle inclut la passe owner fusionnée par la PR #29. Elle est protégée contre les restaurations ou refontes globales non demandées.

- Navigation FR/EN : Accueil/Home, Guide, Changements/Changes, Pokédex, Localisations/Locations, Routes et Villes/Routes & Cities, Boss & Conseils/Tips, Crédits, Roadmap.
- Routes/Villes est intégré et ne doit pas être retiré au nom d’une ancienne consigne.
- Les pages Localisations et les compteurs Pokédex sont dérivés des sources canoniques par `tools/sync_tactica_localization.py`.
- Les données de jeu ne doivent pas être recopiées manuellement dans le wiki lorsqu’elles peuvent être régénérées.

## Validation et merge V1

Le playtest owner final n’est pas un gate de merge vers `integration/v1`.

Une correction peut être fusionnée lorsque :

1. le développement est terminé ;
2. les validateurs/tests pertinents sont verts ;
3. les générateurs sont idempotents ;
4. la CI est verte ;
5. aucun défaut connu n’est masqué.

Les contrôles ROM non encore effectués restent explicitement notés dans `PLAYTEST_STATUS.md`.

Une **candidate owner** est plus stricte : toutes les corrections annoncées sont déjà fusionnées dans `integration/v1`, la CI est verte sur le SHA exact, puis une ROM fraîche est reconstruite avec `make clean && make hns -j4` avant la checklist ROM.

Un simple `git pull` ne met jamais à jour `pokehns.gba`, car les ROM et `build/` sont ignorés par Git.
