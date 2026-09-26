# Prompt de reprise Codex — Pokémon Tactica V1

Tu travailles dans le dépôt Pokémon Tactica, **à partir du dernier `integration/v1`** ou d'une branche fonctionnelle créée depuis ce SHA. Lis d'abord `AGENTS.md`, `docs/spec/SOURCE_OF_TRUTH.md`, `docs/spec/PLAYTEST_STATUS.md`, puis tous les autres `docs/spec/*.md` et `data/spec/*.json`. La hiérarchie normative est stricte : `SOURCE_OF_TRUTH.md` arbitre les règles globales ; les documents métier `docs/spec/*.md` portent les décisions détaillées ; `data/spec/*.json` en sont la représentation machine ; le code/runtime généré vient ensuite. Une exception temporaire document -> JSON doit disparaître dans la PR d'implémentation qui régénère les données. Le code importé montre l'état actuel ; les docs HnS sont techniques/upstream. Ne transforme jamais une ancienne habitude HnS, un vieux prompt ou une branche non fusionnée en règle Tactica.

Ce dépôt a un nouvel historique : lire `docs/spec/MIGRATION.md` pour les branches et SHA de provenance. Une régression peut venir d'un changement de dépôt, branche, build ou ROM réellement lancée. Tenir `docs/spec/FEATURE_STATUS.md` à jour avec **un statut par feature, la preuve, le SHA, le test et la prochaine action**.

## Aparté important — confusion des « blocs 1 à 3 » du 26 septembre

Une confusion de pilotage s'est produite après le playtest du 25 septembre :

- les retours owner les plus récents (Summary, UI boutiques/combat, Méga de Mortimer, Mega Ring, ordre CORE → encounters → UI → RC) avaient été rédigés sur la branche / PR #9 `docs/normalize-decisions-2026-09-25`, mais cette PR n'a jamais été fusionnée dans `integration/v1` ;
- les runs Codex suivants sont repartis de `integration/v1` et ont donc travaillé avec une version plus ancienne des consignes ;
- la formule « blocs 1 à 3 terminés » a ensuite été utilisée pour désigner **encounters + rival/progression/boss + balance**, alors que le plan owner le plus récent appelait bloc 1 = CORE, bloc 2 = encounters, bloc 3 = UI/UX ;
- résultat : plusieurs contrôles automatisés étaient réellement verts, mais des retours owner déjà connus n'avaient jamais été implémentés.

**Règle pour éviter toute récidive :** ne jamais annoncer « bloc N terminé » sans citer le nom fonctionnel du bloc, le SHA intégré, les tests automatiques ET les contrôles ROM encore ouverts. Une branche/PR non fusionnée ne compte jamais comme version testable owner.

## Protocole obligatoire de version testable

Le propriétaire ne doit recevoir qu'une build issue du **dernier `integration/v1` fusionné**.

Avant de demander un playtest :

1. fusionner toutes les PR nécessaires au périmètre annoncé ;
2. attendre la CI Tactica verte sur le SHA d'intégration ;
3. mettre à jour `docs/spec/PLAYTEST_STATUS.md` avec le SHA exact, ce qui est réellement testable, les points encore non implémentés et la checklist ;
4. compiler une ROM fraîche : un simple `git pull` ne met **jamais** à jour `pokehns.gba`, car les `*.gba` et `build/` sont ignorés par Git ;
5. utiliser `make clean && make hns -j4` pour un playtest de référence après une grosse passe ou en cas de doute sur un ancien artefact ;
6. ne jamais demander au propriétaire de revalider un défaut connu dont la correction n'est pas présente sur ce SHA.

La ROM de référence du propriétaire doit donc être reconstruite après le pull. Le testeur doit vérifier `git rev-parse --short HEAD` puis l'heure de génération de `pokehns.gba` avant de lancer mGBA.

## Décisions owner à appliquer avant la prochaine candidate de playtest

### CORE / rival

- Le **premier combat rival** conserve un seul Pokémon, son starter, mais ce Pokémon doit utiliser le niveau de préparation du premier Champion : **niveau 17**. Son stade d'évolution doit être résolu légalement à ce niveau (ex. une espèce évoluant au niveau 16 ne doit pas rester artificiellement au stade de base).
- Les combats suivants conservent leurs tailles progressives et utilisent le profil de niveau du prochain jalon obligatoire défini par la spec.
- Corriger le générateur des boss Méga : la forme de base doit avoir un talent légal avant transformation ; la Méga-Gemme est conservée ; le talent Méga vient de la transformation. Méga-Ectoplasma de Mortimer est le cas minimal de régression à couvrir.
- Mortimer doit remettre le Mega Ring après le badge 4 puis la CT, afin que la Méga joueur soit utilisable immédiatement avant le badge 5.

### Sélecteur starter

- À la première ouverture d'une liste d'espèces, le curseur commence **en haut de la liste**, jamais sur `Retour`.
- Si le joueur ouvre un starter puis annule la confirmation, il revient sur **exactement le Pokémon qu'il venait de sélectionner**, pour toutes les espèces. Le comportement observé avec Élekid qui renvoie sur `Retour` est un bug.
- `Retour` reste la dernière ligne et ne devient jamais la position initiale par défaut.
- L'œuf d'Orme ne peut pas redonner exactement le starter principal.

### UI / UX ROM

- Le Summary reste **À CORRIGER** : auditer d'abord BG/tilemaps/windows et supprimer les superpositions de libellés statiques avec les données dynamiques. Les retouches de couleur seules ne suffisent pas.
- Si nécessaire, remplacer la page `CONTEST MOVES` par une page IV/EV dédiée ; cette solution est approuvée.
- Palette custom : noir / rouge / gris, sans grande surface blanche.
- Menus custom de boutique : **fond rouge, texte noir**, sélection clairement visible sans surlignage blanc dominant.
- HUD combat : conserver les healthboxes sombres mais harmoniser aussi les panneaux d'action et d'attaques ; aucune grande zone blanc cassé/grise ne doit rester comme ancienne UI.
- Finaliser les fenêtres PNJ/shop, textes longs, curseurs et autres polish rencontrés pendant une run.

### Encounters

- Quatre vrais slots par table/méthode, pondérés `30/30/30/10`.
- **Le niveau d'une zone est déterminé par son premier accès dans la progression de l'histoire, pas par la date de déblocage de Surf/Pêche/Éclate-Roc.** Une méthode débloquée plus tard ouvre ses slots mais ne rehausse pas artificiellement le niveau d'une ancienne zone.
- Une zone accessible avant Albert doit rester dans la fenêtre du premier cap, soit typiquement `14–17` pour les tables conçues avec une largeur de 3 niveaux, avec les stades d'évolution cohérents à cette plage.
- Route 36 est le cas témoin obligatoire : elle est accessible avant le badge 1, donc ses tables doivent être `14–17` max. L'actuel `before_blanche / cap 32` de `encounter_access_caps.json` est un **défaut connu à corriger**, pas une règle.
- Les données canoniques et le moteur doivent être testés **en ROM**, pas seulement par JSON/validator.
- La couverture, les caps de premier accès, habitats et stades d'évolution restent ceux de `ENCOUNTERS.md` une fois le dataset d'accès régénéré selon cette règle.

## Ordre de production actuel

### Décision owner prioritaire — rosters Rival/Rocket du 26-09

Avant toute poursuite du bloc rival/Rocket, relire [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md). Les anciennes compositions de `rival.json`, `rocket_progression.json` et des entrées Rocket de `bosses.json` sont **à régénérer** lorsqu'elles contredisent ce document. Ne pas conserver l'ancien tirage d'un starter parmi cinq : le rival possède désormais un starter fixe par archétype, des équipes Singles finalisées et une Méga déverrouillée après le badge 4. Pour Archer, la FINAL utilise **Méga-Sharpedo**, qui Méga-évolue immédiatement ; ne pas créer de logique Abri tour 1 -> Méga tour 2. Méga-Démolosse reste réservé à Marion.

### Bloc 1 — CORE / gameplay restant
Corriger rival niveau/stade, Méga boss, Mega Ring, starter/œuf et toute régression bloquante. Build/tests/validateurs verts obligatoires.

### Bloc 2 — Encounters / progression
Conserver les données déjà intégrées, résoudre tout écart runtime, vérifier Route 36 et autres points représentatifs, puis tests/validateurs/build.

### Bloc 3 — UI / UX ROM
Summary en priorité, puis boutiques, HUD/action/moves, fenêtres custom et polish starter. Les contrôles visuels ROM sont obligatoires ; un validateur statique ne suffit pas à déclarer l'UI terminée.

### Bloc 4 — Stabilisation / candidate owner
Réconcilier et fusionner tout ce qui précède, CI verte, build propre, mettre à jour `PLAYTEST_STATUS.md`, puis seulement demander le playtest owner.

### Bloc 5 — Wiki / secondaire
Le wiki reste géré séparément et ne doit pas consommer le chemin critique tant que CORE/encounters/UI ne sont pas une candidate jouable.

## Validation proportionnée au risque

Ne pas rejouer localement toute la non-régression à chaque petite correction. Pendant le développement : exécuter le validateur et les tests du domaine touché, puis un build incrémental si du code/runtime change. La CI de PR assure le contrôle documentaire, les validateurs, le build et un smoke test pour le code ; une PR docs/wiki ne compile pas la ROM. Après merge sur `integration/v1`, la CI exécute automatiquement la suite complète de régression. `make clean && make hns -j4` et la recette ROM complète sont réservés à une candidate owner, à une grosse passe transversale ou à un doute sur des artefacts anciens.

Un test ciblé doit être préféré à une run owner complète : starter -> flow starter ; Summary -> Summary ; boss -> combat concerné. Ne demander une run de progression complète qu'une fois plusieurs blocs intégrés dans une candidate explicite.

## Production par PR

- Une branche de domaine issue du dernier `integration/v1` à la fois.
- Chaque PR indique fichiers modifiés, différence vis-à-vis de la spec, statuts touchés, tests passés/échoués et contrôles manuels restants.
- Rebase/réconcilie avant PR ; pas de force push sur `main`.
- Ne réécris pas vitesse native, audio, shiny rate ou évolutions solo sans défaut démontré.
- Le rival, Rocket, shops et encounters sont générés depuis leurs sources canoniques ; toute modification doit conserver les contrôles d'idempotence.
- Ajouter/adapter les validateurs pour rejeter toute réutilisation non autorisée d'une Méga déjà réservée entre Champions, Conseil 4, rival et Rocket.
- Shiny Only reste post-V1.

## Portes de sortie

Avant une candidate owner : validateurs actualisés, synchronisations `--check`, suite complète d'intégration verte, `make clean && make hns -j4`, puis checklist ROM de `PLAYTEST_STATUS.md`. Pour une PR ordinaire, ne lancer que les tests du domaine modifié et laisser la CI d'intégration rejouer la non-régression globale après merge.

Un build réussi ne prouve pas le rendu UI, les encounters runtime ou le comportement des combats. Une feature nécessitant une observation en jeu reste `PARTIEL` / `À CORRIGER` jusqu'à cette observation.
