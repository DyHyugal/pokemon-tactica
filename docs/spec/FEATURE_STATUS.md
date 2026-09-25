# Suivi des features — Tactica V1

**État audité au 25 septembre 2026 sur `integration/v1` (`80039831`).** Ce registre suit l'état de l'implémentation ; les règles produit restent dans `SOURCE_OF_TRUTH.md` et `data/spec/`. Un tag décrit les preuves disponibles, pas une nouvelle décision de game design. La branche `feature/wiki` contient du travail en cours qui n'est pas encore fusionné dans `integration/v1`.

Le build `make hns -j4` passe. La référence automatisée passe avec `Native` 3/3, `Audio` 6/6, `Settings` 5/5, `Family` 27/27, `Evolution requirements` 4/4, `Level cap` 9/9, `Training NPC` 7/7 et `Family Safari` 4/4. Les validateurs statiques `validate_tactica_spec.py` et `validate_family_remix.py` passent également. Ces résultats établissent l'absence de régression de compilation sur la base importée ; ils ne valident pas les contrats obsolètes explicitement relevés ci-dessous. En particulier, le test `Level cap` attend encore l'enchaînement Rocket `+2`, et le validateur historique accepte encore les groupes natifs 12/5/10 comme représentation des quatre slots.

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
| Vitesse native x1–x4 | `[DONE]` | `test/native_speed.c`, 3/3 le 25-09-2026 ; valeurs historiques, quatre multiplicateurs, audio et tours de combat couverts. Contrôle visuel final de confort conservé pour la recette ROM. |
| Audio indépendant et preset Recommended | `[DONE]` | `test/audio_volume.c`, 6/6 le 24-09-2026 ; volumes, muting, preset et sérialisation couverts. Contrôle auditif final conservé pour la recette ROM. |
| Menu Shiny Rate existant | `[DONE]` | `test/settings_menu.c`, 5/5 le 24-09-2026 ; seuils et sauvegarde/annulation couverts. |
| Sélecteur des 30 starters et Évoli | `[DONE]` | `src/family_starter.c` et `test/family_starter.c`, inclus dans `Family` 27/27 le 25-09-2026 ; sélection exacte, annulation, objets, équipe/PC pleins et cas Évoli couverts. |
| Œuf d'Orme et second choix | `[DONE]` | `test/family_starter.c`, inclus dans `Family` 27/27 le 25-09-2026 ; aperçu, personnalité, éclosion, livraison unique et PC couverts. |
| Évolutions sans échange | `[DONE]` | `test/evolution_requirements.c`, 4/4 le 25-09-2026 ; seuils, objets, branches et remplacements des échanges couverts. |
| Structure du menu Settings | `[PARTIEL]` | `test/settings_menu.c`, 5/5 le 24-09-2026 ; comportement et persistance passent. L'ordre et les libellés exacts de toutes les pages restent à contrôler dans la ROM. |
| Assistant d'entraînement | `[DONE]` | `src/training_npc.c`, `data/scripts/training_npc.inc` et `test/training_npc.c`, 7/7 le 25-09-2026 ; EXP/cap, IV, EV/reset, nature, talent et bonheur couverts. L'accès est câblé dans les scripts de carte. |
| Rencontres standard à quatre slots 30/30/30/10 | `[À CORRIGER]` | 405 tables canoniques ; ancien mapping 12/5/10 dans `src/data/wild_encounters.json`. Adapter moteur, données compilées, validateur et tirages limites. |
| Rotation Safari | `[DONE]` | 53 pools canoniques ; `test/family_safari.c`, 4/4 le 25-09-2026, et groupe `Family` vert ; compteurs par secteur, rotation de session, pools et poids 30/30/30/10 couverts. |
| Headbutt | `[À RECONTRÔLER]` | 4 tables spéciales ; contrôler sélection et niveaux en jeu. |
| Tirage/persistance du starter rival | `[À CORRIGER]` | `src/family_starter.c` utilise un contre-type unique ; appliquer la matrice de catégories de `data/spec/rival.json`. |
| Équipes et thème évolutif du rival | `[PARTIEL]` | Archétypes définis ; rosters précis à inventorier et proposer avant de les coder. |
| Caps champions/rival | `[PARTIEL]` | `test/level_caps.c`, 9/9 le 25-09-2026, confirme les jalons existants, la Ligue/Kanto/Red et le calcul HARD. Comparaison exhaustive à `PROGRESSION.md` à terminer avec la correction Rocket. |
| Caps Rocket | `[À CORRIGER]` | `src/caps.c` enchaîne la référence après Rocket et `test/level_caps.c` entérine encore ce comportement ; dernier cap champion/rival jalon +2 requis. Le vert 9/9 du 25-09-2026 n'est donc pas une validation du contrat actuel. |
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
