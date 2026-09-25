# Suivi des features — Tactica V1

**État audité au 25 septembre 2026 sur `feature/rival-progression-bosses` (`7bc70345`), issue d'`integration/v1` (`2aa3d086`).** Ce registre suit l'état de l'implémentation ; les règles produit restent dans `SOURCE_OF_TRUTH.md` et `data/spec/`. Un tag décrit les preuves disponibles, pas une nouvelle décision de game design. Les commits fonctionnels locaux sont distingués de leur intégration distante tant que l'authentification GitHub empêche leur publication.

Le build `make hns -j4` passe (ROM 94,53 %, EWRAM 94,70 %, IWRAM 78,45 %). La référence automatisée passe avec `Native` 3/3, `Audio` 6/6, `Settings` 5/5, `Family` 29/29, `Evolution requirements` 4/4, `Level cap` 9/9, `Training NPC` 7/7 et `Family Safari` 4/4. Le bloc rencontres ajoute `Tactica encounters` 2/2 et un validateur qui compare les 405 tables compilées à la source canonique. Les validateurs statiques `validate_tactica_spec.py` et `validate_family_remix.py`, ainsi que les contrôles d'idempotence `sync_tactica_encounters.py --check` et `sync_tactica_bosses.py --check`, passent également.

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
| Rencontres standard à quatre slots 30/30/30/10 | `[DONE]` | `src/data/wild_encounters.json` est synchronisé depuis les 405 tables canoniques par `tools/sync_tactica_encounters.py` ; chaque méthode HnS utilise quatre entrées réelles. `test/tactica_encounters.c` 2/2, `Family` 27/27, validateurs statiques et build HnS passent le 25-09-2026. Les bornes 0–29, 30–59, 60–89 et 90–99 sont couvertes ; toutes les cannes lisent le même pool après déverrouillage. |
| Rotation Safari | `[DONE]` | 53 pools canoniques ; `test/family_safari.c`, 4/4 le 25-09-2026, et groupe `Family` vert ; compteurs par secteur, rotation de session, pools et poids 30/30/30/10 couverts. |
| Headbutt | `[DONE]` | Les 4 tables dédiées de `src/data/family_remix_headbutt.h` correspondent aux pools canoniques et utilisent quatre entrées réelles. Le sélecteur commun 30/30/30/10 et le validateur statique passent le 25-09-2026. |
| Tirage/persistance du starter rival | `[PARTIEL]` | Implémentation complète dans le commit local `7bc70345` : matrice canonique de `data/spec/rival.json`, tirage puis persistance sans réécriture. `Family` 29/29 le 25-09-2026 ; reste à publier et intégrer. |
| Équipes et thème évolutif du rival | `[PARTIEL]` | Les archétypes sont définis, mais `data/spec/rival.json` marque encore les rosters précis `TO_AUTHOR_AFTER_AUDIT`. Décision produit requise avant codage ; aucun roster n'a été inventé. |
| Caps champions/rival | `[PARTIEL]` | `test/level_caps.c`, 9/9 le 25-09-2026, confirme les jalons existants, la Ligue/Kanto/Red et le calcul HARD. Les niveaux fixes sont synchronisés avec `data/spec/bosses.json` dans `7bc70345` ; la conformité du rival dépend encore de ses rosters canoniques manquants. |
| Caps Rocket | `[PARTIEL]` | Correction complète dans le commit local `7bc70345` : chaque Rocket utilise le dernier jalon champion/rival +2 sans chaîner les Rocket précédents. `Level cap` 9/9 et build HNS passent le 25-09-2026 ; reste à publier et intégrer. |
| Équipes boss et légalité des évolutions | `[PARTIEL]` | Les 22 groupes fixes, 127 Pokémon, niveaux, objets, formes et sets sont synchronisés depuis `data/spec/bosses.json` par `tools/sync_tactica_bosses.py` dans `7bc70345`. Les exceptions légales Teddiursa niv. 29 et Zweilous niv. 61 sont conservées ; validateur et build passent. Reste à publier et intégrer. |
| NORMAL/HARD, EV/IV et IA | `[PARTIEL]` | `7bc70345` garantit le même contenu NORMAL/HARD, l'absence d'EV optimisés en NORMAL, des EV/IV légaux en HARD et une IA stratégique sans information cachée. Le validateur contrôle 22 équipes fixes et 7 variantes Rocket ; reste à publier et intégrer. |
| Méga uniques et Jasmine | `[PARTIEL]` | `7bc70345` synchronise les objets Méga canoniques et remplace l'ancien ace par Méga-Galeking : Filter, Careful, Heavy Slam / Curse / Rest / Sleep Talk, EV 252 PV / 4 Déf / 252 Déf. Spé. Contrôles statiques et build passent ; disponibilité en jeu à auditer avec les boutiques. |
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
