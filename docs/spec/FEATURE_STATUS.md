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
| Jeannine = Méga-Kravarech | `[PARTIEL]` | Canonique + runtime + Dragalgite + Adaptabilité de forme Méga + validateur ; combat non observé en ROM. |
| Soins des Champions | `[DONE]` | Au plus deux soins selon la progression ; ne pas recoder sans défaut concret. |
| Encounters 4 slots 30/30/30/10 | `[DONE]` | 405 tables standard + pools spéciaux structurés. |
| Premier accès réel / scaling | `[DONE]` | Déblocage des méthodes séparé du niveau de la zone. |
| Route 36 14–17 | `[DONE]` | Source/runtime synchronisés ; témoin ROM conservé pour candidate. |
| Scorplane early | `[DONE]` | Route 36 jour, slot 30 %. |
| Wattouat disponible | `[DONE]` | Route 31 jour, slot 30 %. |
| Couverture globale pré-Ligue | `[PARTIEL]` | Dataset standard : 479 espèces utilisées, 409 présentes avant/à la Ligue, 70 uniquement post-Ligue, 152 espèces dupliquées avant Ligue. Rebalance dédiée à poursuivre par lots écologiques. |
| Headbutt | `[DONE]` | 4 tables dédiées. |
| Safari | `[DONE]` | 53 pools. |
| Wiki baseline owner | `[DONE]` | PR #29 intégrée, y compris Guide/Accueil/Changements/Roadmap/Routes-Villes. Ne pas restaurer une ancienne baseline. |
| Sync Localisations — niveaux | `[DONE]` | Générateur canonique. |
| Sync Localisations — espèces / recherche | `[PARTIEL]` | Générateur étendu aux espèces et `data-search`; CI/idempotence à confirmer sur la PR de nettoyage. |
| Compteurs Pokédex | `[PARTIEL]` | Recalculés depuis les tables standard + pools spéciaux visibles ; CI/idempotence à confirmer. |
| Summary ROM | `[À CORRIGER]` | Recomposition BG/tilemaps/windows, suppression doublons ; page IV/EV autorisée. |
| HUD combat | `[À CORRIGER]` | Supprimer le rectangle blanc à gauche des PV et harmoniser action/attaques. |
| UI boutiques | `[À CORRIGER]` | Fond rouge, texte noir, sélection lisible, aucune grande plaque blanche. |
| Assistant d’entraînement | `[DONE]` | Tests dédiés existants. |
| Boutiques objets/moves/Méga-Gemmes — contenu | `[PARTIEL]` | Catalogues/runtime présents ; parcours d’achat ROM à revalider. |
| Balance 26 espèces / 15 learnsets | `[DONE]` | Source canonique + validateur dédié. |

## Règle de merge

Une feature automatisée propre n’attend pas le playtest owner final pour rejoindre `integration/v1`. Elle doit cependant conserver dans ce fichier ou `PLAYTEST_STATUS.md` les observations ROM encore utiles.

Ne jamais transformer « non testé manuellement » en « non implémenté », et ne jamais déclarer un contrôle ROM effectué s’il ne l’a pas été.

## Prochain ordre de travail

1. remettre toutes les synchronisations et la CI au vert ;
2. traiter Summary ;
3. traiter HUD combat + boutiques ;
4. poursuivre la couverture pré-Ligue par lots cohérents séparés ;
5. réconcilier, clean build, puis déclarer une candidate owner dans `PLAYTEST_STATUS.md`.
