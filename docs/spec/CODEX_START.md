# Prompt de reprise Codex — Pokémon Tactica V1

Tu travailles dans le nouveau dépôt Pokémon Tactica, branche `integration/v1` ou branche fonctionnelle réconciliée avec celle-ci. Lis d'abord `AGENTS.md`, `docs/spec/SOURCE_OF_TRUTH.md`, puis tous les autres `docs/spec/*.md` et `data/spec/*.json`. Ces fichiers sont les **seules sources normatives du produit**. Le code importé montre l'état actuel, les docs HnS sont techniques/upstream. Ne transforme jamais une ancienne habitude HnS ou un vieux prompt en règle Tactica. Les rosters Rocket et les propositions de six rosters rival sont désormais décrits dans `ROSTERS_ROCKET_RIVAL.md` et les JSON correspondants ; distingue toujours rédaction, intégration moteur et recette ROM.

Ce dépôt a un **nouvel historique** : lire `docs/spec/MIGRATION.md` pour les branches et SHA de provenance. Une régression d'une feature autrefois validée peut être révélée par le changement de dépôt, de branche, de fichiers ou d'environnement. Comparer les fichiers concernés avec l'ancienne production et reproduire le défaut avant de conclure ou de réécrire la feature. Tenir à jour `docs/spec/FEATURE_STATUS.md` : **un tag d'état par feature**, avec preuve, branche/SHA, test et prochaine action.

## Premier bloc : prise de contexte, sans changement de gameplay

1. Note SHA, branche et propreté Git. Vérifie la présence et la cohérence de tous les documents et JSON canoniques. Repère les restes de documentation produit contradictoire et les mentions de l'ancien nom visibles au joueur ; garde les crédits/licences et identifiants HnS techniques nécessaires.
2. Compare le code, le validator, les tests, le wiki et les données aux contrats V1. Mets à jour le registre `FEATURE_STATUS.md` **feature par feature** avec son tag, les chemins, preuves, tests et prochaine action ; distingue code fusionné, branches en attente et travail local non poussé (non observable). Produis aussi la synthèse `DONE / PARTIEL / À CORRIGER / À FAIRE`, en explicitant les états `À RECONTRÔLER` et `À AUDITER` qui ne sont pas des validations post-migration. Examine notamment les 405 tables standard, 57 spéciales, Rocket sans chaîne, tirage rival et rosters, Méga uniques, Jasmine, Dracaufeu, boutiques exhaustives, Traduction FR/EN et palette réelle.
3. Exécute seulement les vérifications appropriées pour établir la baseline. Ne déclare pas validé un test mGBA que tu n'as pas joué. Signale les écarts et propose l'ordre des PR. Les blocs rencontres, balance, boutiques et Rocket sont intégrés à `integration/v1` via la PR #4 ; les PR #7 et #8 traitent la progression des six équipes du rival. Vérifie leur CI et leur fusion, puis poursuis le dataset `earliest_access_cap`, les capacités au niveau réel et la recette ROM FR/EN.

## Production par blocs

- Une branche de domaine issue d'`integration/v1` à la fois ; commit et PR ciblés avec fichiers modifiés, différences vis-à-vis de la spec, **tags des features touchées**, tests passés/échoués et contrôles manuels restants. Mettre à jour le registre avec les preuves du résultat. Rebase/reconcilie avant chaque PR ; pas de force push sur `main`.
- Ne réécris pas vitesse native, audio, shiny rate, starter/œuf, évolutions solo validés. Des tests ciblés vérifient qu'ils restent corrects. Répare uniquement une régression constatée ou un écart explicite de la spec.
- Modifie le moteur HnS du fork si nécessaire pour les quatre vrais slots. Les pondérations sont `30/30/30/10` et les anciennes longueurs de tableaux ne constituent aucune règle produit. Préserve les autres systèmes non concernés.
- Rocket = dernier cap **champion/rival jalon** +2, sans enchaîner sur un Rocket antérieur. Une équipe NORMAL/HARD reste identique en contenu. Les objets de soins suivent la disponibilité native à ce stade.
- Le rival tire catégorie puis starter et conserve le résultat. La PR #8 branche sur les sept combats existants les six rosters et leurs phases générés depuis `data/spec/rival.json` ; vérifier sa CI, l'intégrer et contrôler les évolutions, capacités et difficultés par playtest ROM avant validation.
- Shiny Only est **post-V1** ; il sera plus tard une ligne `Shiny Only — ON/OFF` dans Nuzlocke. Ne l'implémente pas dans cette V1.
- Les boutiques ne se limitent pas aux anciens catalogues : capacités actives compatibles, objets pertinents absents des magasins habituels, et PNJ Méga-Gemmes dédié à créer si absent.

## Portes de sortie

Validator actualisé + tests ciblés, `make hns -j4`, non-régression des systèmes protégés, contrôles boss/rival/Rocket/rencontres/shop, puis mGBA et revue FR/EN/wiki/mobile avant V1. N'annonce pas la release tant que les contrôles manuels et écarts de données ne sont pas clos.
