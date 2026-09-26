# Suivi des features — Pokémon Tactica V1

Ce registre décrit l’état fonctionnel courant. Il ne sert pas d’historique de décisions : les anciens états sont dans Git.

## Tags

| Tag | Sens |
|---|---|
| `[DONE]` | Développement et validation automatisée pertinents terminés. Un contrôle ROM de confort peut rester listé sans rouvrir la feature. |
| `[PARTIEL]` | Une partie importante est intégrée mais un contrôle runtime/ROM ou un sous-contrat reste ouvert. |
| `[À CORRIGER]` | Écart concret connu entre l’état actuel et le contrat. |
| `[À FAIRE]` | Fonction V1 requise non commencée. |
| `[HORS V1]` | Reporté après V1. |

## État courant

| Feature | Statut | Preuve / contrôle restant |
|---|---|---|
| Vitesse native x1–x4 | `[DONE]` | Tests natifs existants ; ne pas réécrire sans défaut démontré. |
| Audio indépendant / preset | `[DONE]` | Tests audio existants ; contrôle auditif final uniquement. |
| Shiny Rate | `[DONE]` | Menu et persistance couverts. |
| Shiny Only | `[HORS V1]` | À reprendre après stabilisation V1. |
| 30 starters + Évoli | `[DONE]` | Logique cœur intégrée. |
| Sélecteur — curseur initial / annulation | `[PARTIEL]` | Correction intégrée automatiquement ; comportement visuel à revalider en ROM fraîche. |
| Œuf d’Orme / second starter distinct | `[PARTIEL]` | Garde-fou intégré ; flow à revalider en ROM. |
| Évolutions sans échange | `[DONE]` | Tests dédiés existants. |
| Rival — starter fixe / niveau 17 / 1→3→4→6 | `[PARTIEL]` | JSON/runtime/validateurs intégrés ; combats ROM à observer. |
| Rocket — 3→4→6 / Méga FINAL | `[PARTIEL]` | JSON/runtime/validateurs intégrés ; combats ROM à observer. |
| Archer = Méga-Sharpedo immédiat | `[PARTIEL]` | Set et garde-fou automatisés ; transformation à observer en ROM. |
| Mega Ring après Mortimer | `[PARTIEL]` | Ordre scripté et validé ; réception/utilisation à confirmer en ROM. |
| Méga uniques globalement | `[PARTIEL]` | Unicité automatisée ; transformations boss à observer en ROM. |
| Une Méga par Champion dès Mortimer | `[PARTIEL]` | Validateur exige exactement une Méga pour chaque Champion concerné. |
| Jeannine : Aéromite ace + Méga-Kravarech | `[PARTIEL]` | Aéromite reste l’ace ; Méga-Kravarech remplace Gaulet au slot 3. Canonique/runtime/Dragalgite/Adaptabilité/validateur intégrés ; combat non observé en ROM. |
| Soins des Champions | `[DONE]` | Au plus deux soins selon la progression ; ne pas recoder sans défaut concret. |
| Encounters 4 slots 30/30/30/10 | `[DONE]` | 405 tables standard + pools spéciaux structurés. |
| Premier accès réel / scaling | `[DONE]` | Déblocage des méthodes séparé du niveau de la zone. |
| Route 36 14–17 | `[DONE]` | Source/runtime synchronisés ; témoin ROM conservé pour candidate. |
| Scorplane après badge 2 | `[DONE]` | Route 34 jour, niveaux 25–28, slot 30 % à la place de Migalos ; aucune famille Scorplane avant le cap badge 2. |
| Wattouat disponible | `[DONE]` | Route 31 jour, slot 30 %. |
| Couverture globale pré-Ligue | `[DONE]` | Dataset standard : 479 espèces utilisées et 479 disponibles avant/à la Ligue ; 0 espèce utilisée reste uniquement post-Ligue. 72 slots ont été remplacés sur 43 tables ; 125 espèces restent dupliquées avant Ligue, ce qui est acceptable puisque la couverture est assurée. |
| Headbutt | `[DONE]` | 4 tables dédiées. |
| Safari | `[DONE]` | 53 pools. |
| Wiki baseline owner | `[DONE]` | PR #29 intégrée, y compris Guide/Accueil/Changements/Roadmap/Routes-Villes. Ne pas restaurer une ancienne baseline. |
| Sync Localisations — niveaux | `[DONE]` | Générateur canonique. |
| Sync Localisations — espèces / recherche | `[DONE]` | Générateur étendu aux espèces et `data-search`; synchronisations `--check` et CI Tactica vertes sur le HEAD de la PR de nettoyage. |
| Compteurs Pokédex | `[DONE]` | Recalculés depuis les tables standard + pools spéciaux visibles ; synchronisation idempotente confirmée par la CI Tactica. |
| Summary ROM | `[PARTIEL]` | Recomposition structurelle intégrée et protégée par le validateur UI ; rendu réel à confirmer dans une ROM fraîche. |
| HUD combat | `[PARTIEL]` | Plaque blanche résiduelle de la barre PV supprimée dans l’asset et protégée par le validateur UI ; panneaux/rendu final à confirmer en ROM. |
| UI boutiques | `[PARTIEL]` | Charte rouge/noir intégrée dans le runtime shop et protégée par le validateur UI ; lisibilité/sélection à confirmer en ROM. |
| Assistant d’entraînement | `[DONE]` | Tests dédiés existants. |
| Boutiques objets/moves/Méga-Gemmes — contenu | `[PARTIEL]` | Catalogues/runtime présents ; parcours d’achat ROM à revalider. |
| Balance 26 espèces / 15 learnsets | `[DONE]` | Source canonique + validateur dédié. |

## Règle de merge

Une feature automatisée propre n’attend pas le playtest owner final pour rejoindre `integration/v1`. Elle doit cependant conserver dans ce fichier ou `PLAYTEST_STATUS.md` les observations ROM encore utiles.

Ne jamais transformer « non testé manuellement » en « non implémenté », et ne jamais déclarer un contrôle ROM effectué s’il ne l’a pas été.

## Prochain ordre de travail

1. remettre toutes les synchronisations et la CI au vert ;
2. confirmer Summary, HUD et boutiques dans une ROM fraîche ;
3. réconcilier tout nouveau défaut observé ;
4. clean build puis déclarer une candidate owner dans `PLAYTEST_STATUS.md`.
