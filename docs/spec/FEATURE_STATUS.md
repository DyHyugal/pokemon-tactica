# Suivi des features — Tactica V1

**Point de départ au 24 septembre 2026, avant l'audit post-migration.** Ce registre suit l'état de l'implémentation ; les règles produit restent dans `SOURCE_OF_TRUTH.md` et `data/spec/`. Un tag décrit les preuves disponibles, pas une nouvelle décision de game design. La branche `feature/wiki` contient du travail en cours qui n'est pas encore fusionné dans `integration/v1`.

## Règles des tags

| Tag unique par feature | Sens |
|---|---|
| `[À RECONTRÔLER]` | Fonction validée et testée avant migration, à vérifier dans le nouveau dépôt. **Protégée** : ne pas la réécrire sans défaut démontré. |
| `[À AUDITER]` | Présence ou conformité non établie avec assez de preuves. Inventaire nécessaire avant de conclure. |
| `[À FAIRE]` | Fonction requise en V1 et non implémentée d'après les preuves disponibles. |
| `[PARTIEL]` | Une partie existe ; le contrat complet n'est pas prouvé ou des éléments restent à réaliser. |
| `[À CORRIGER]` | Écart précis entre code connu et contrat validé. |
| `[DONE]` | Code intégré à `integration/v1`, contrat contrôlé et tests pertinents documentés **après migration** ; une vérification manuelle encore requise est indiquée explicitement. |
| `[HORS V1]` | Décision reportée après V1 ; aucun développement à lancer maintenant. |

À chaque audit ou PR, conserver **une ligne et un seul tag par feature** ; inscrire branche/SHA, fichiers, test exécuté et résultat, limite éventuelle, puis date. Ne passer à `[DONE]` qu'avec des preuves du dépôt Tactica, et ne pas déduire « terminé » d'un JSON conforme seul. Si un test anciennement validé échoue, passer à `[À CORRIGER]`, relier l'incident à la migration si les preuves le montrent, puis corriger et retester au plus près. La création d'une branche ou d'une PR ne change pas le tag à elle seule.

## Registre initial à réviser par Codex

La colonne « preuve / prochain contrôle » décrit la **situation connue au gel**, pas un test post-migration. `e5c7191` désigne l'ancienne production ; `f5a2a3d` est le premier arbre complet du nouveau `main`/`integration/v1`. Les travaux `feature/wiki` sont séparés.

| Feature | Tag | Preuve / prochain contrôle |
|---|---|---|
| Vitesse native x1–x4 | `[À RECONTRÔLER]` | Testée dans l'ancienne production `e5c7191` ; vérifier vitesse et musique sur Tactica sans réécrire le moteur. |
| Audio indépendant et preset Recommended | `[À RECONTRÔLER]` | Tests antérieurs validés ; vérifier volumes, sauvegarde et libellés dans le nouveau build. |
| Menu Shiny Rate existant | `[À RECONTRÔLER]` | Choix testés auparavant ; relever les valeurs réelles et vérifier leur persistance. |
| Sélecteur des 30 starters et Évoli | `[À RECONTRÔLER]` | `src/family_starter.c` ; retester choix, annulation, doublons, objet et équipe/PC pleins. |
| Œuf d'Orme et second choix | `[À RECONTRÔLER]` | Même sélecteur ; retester dialogue, livraison, sauvegarde et cas limites. |
| Évolutions sans échange | `[À RECONTRÔLER]` | Validées avant migration ; retester seuils et objets sans changer les règles. |
| Structure du menu Settings | `[À AUDITER]` | Contrat dans `GAME_SYSTEMS.md` ; comparer l'ordre et les options à la ROM. |
| Assistant d'entraînement | `[À AUDITER]` | Vérifier EXP/cap, IV, EV/reset, nature, talent, bonheur et accès réel. |
| Rencontres standard à quatre slots 30/30/30/10 | `[À CORRIGER]` | 405 tables canoniques ; ancien mapping 12/5/10 dans `src/data/wild_encounters.json`. Adapter moteur, données compilées, validateur et tirages limites. |
| Rotation Safari | `[À RECONTRÔLER]` | 53 pools dans `data/spec/encounters_special.json` ; retester secteur/session et niveaux. |
| Headbutt | `[À RECONTRÔLER]` | 4 tables spéciales ; contrôler sélection et niveaux en jeu. |
| Tirage/persistance du starter rival | `[À CORRIGER]` | `src/family_starter.c` utilise un contre-type unique ; appliquer la matrice de catégories de `data/spec/rival.json`. |
| Équipes et thème évolutif du rival | `[PARTIEL]` | Archétypes définis ; rosters précis à inventorier et proposer avant de les coder. |
| Caps champions/rival | `[À AUDITER]` | Courbe canonique dans `PROGRESSION.md` ; comparer chaque jalon au code. |
| Caps Rocket | `[À CORRIGER]` | `src/caps.c` enchaîne la référence après Rocket ; dernier cap champion/rival jalon +2 requis. |
| Équipes boss et légalité des évolutions | `[PARTIEL]` | Dataset `data/spec/bosses.json` ; comparer aux rosters compilés, niveaux, objets et formes. |
| NORMAL/HARD, EV/IV et IA | `[PARTIEL]` | Vérifier même contenu des équipes, EV légaux, IV, stratégie et absence d'information cachée. |
| Méga uniques et Jasmine | `[PARTIEL]` | Vérifier disponibilité des Méga, unicité et set de Méga-Galeking en combat. |
| Objets de soins des champions | `[À AUDITER]` | Jusqu'à deux si prévus ; vérifier disponibilité des objets au stade du jeu. |
| Balance de 26 espèces | `[PARTIEL]` | `data/spec/pokemon_balance.json` ; comparer statistiques/types/talents codés, notamment Dracaufeu X/Y. |
| Quinze learnsets custom | `[PARTIEL]` | JSON canonique ; comparer les 15 entrées au code et au niveau réel d'apprentissage. |
| PNJ capacités/CT | `[PARTIEL]` | PNJ existant ; inventaire de toutes capacités actives, compatibilité et achat à finir. |
| PNJ objets stratégiques | `[PARTIEL]` | Catalogues partiels ; inventaire exhaustif des objets utiles hors boutiques normales et scénario. |
| PNJ Méga-Gemmes | `[À AUDITER]` | Vérifier existence et catalogue ; si absent, créer le PNJ dédié près des capacités. |
| Textes du jeu FR/EN | `[À AUDITER]` | Vérifier noms officiels, identité Tactica et miroir des langues dans la ROM. |
| Wiki FR/EN | `[PARTIEL]` | Intégré dans `integration/v1` via PR #2, squash `e4b2c27e` : pages cœur FR/EN reconstruites ; Localisations = 405 tables standard + 57 spéciales ; Pokédex séparé et noms FR relocalisés ; 26 modifications + 15 learnsets alignés ; 151 slots de boss publiés ; palette contrôlée contre les valeurs réelles des menus ; QA des 15 pages cœur = 0 lien local cassé, 0 doublon de nom Pokédex FR/EN. Reste avant `[DONE]` : revue visuelle réelle navigateur/mobile. |
| Shiny Only dans Nuzlocke | `[HORS V1]` | Prévu après V1 stable ; ne pas l'implémenter dans ce cycle. |

Le premier audit Codex actualise ce tableau avec des chemins, SHA, tests et preuves. Il peut découper une ligne en sous-features si leurs états divergent, sans perdre le lien avec ce registre. Toute régression d'une feature protégée se traite comme un incident à diagnostiquer selon `MIGRATION.md`.
