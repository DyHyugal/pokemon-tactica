# Prompt de reprise Codex — Pokémon Tactica V1

Tu travailles dans le nouveau dépôt Pokémon Tactica, branche `integration/v1` ou branche fonctionnelle créée depuis celle-ci. Lis d'abord `AGENTS.md`, `docs/spec/SOURCE_OF_TRUTH.md`, puis tous les autres `docs/spec/*.md` et `data/spec/*.json`. Ces fichiers sont les **seules sources normatives du produit**. Le code importé montre l'état actuel, les docs HnS sont techniques/upstream. Ne transforme jamais une ancienne habitude HnS ou un vieux prompt en règle Tactica. Si une information nécessaire manque, consigne-la précisément et propose une solution pour validation ; n'invente pas un roster ou une règle.

## Premier bloc : prise de contexte, sans changement de gameplay

1. Note SHA, branche et propreté Git. Vérifie la présence et la cohérence de tous les documents et JSON canoniques. Repère les restes de documentation produit contradictoire et les mentions de l'ancien nom visibles au joueur ; garde les crédits/licences et identifiants HnS techniques nécessaires.
2. Compare le code, le validator, les tests, le wiki et les données aux contrats V1. Produis un tableau `DONE / PARTIEL / À CORRIGER / À FAIRE` **avec chemins, preuves et tests**, en distinguant code fusionné, branches en attente et travail local non poussé (non observable). Examine notamment les 405 tables standard, 57 spéciales, Rocket sans chaîne, tirage rival et rosters, Méga uniques, Jasmine, Dracaufeu, boutiques exhaustives, Traduction FR/EN et palette réelle.
3. Exécute seulement les vérifications appropriées pour établir la baseline. Ne déclare pas validé un test mGBA que tu n'as pas joué. Signale les écarts et propose l'ordre des PR. La priorité initiale est encounters + validateur, puis rival/progression/boss, balance et boutiques ; la localisation et le wiki suivent les mêmes JSON.

## Production par blocs

- Une branche de domaine issue d'`integration/v1` à la fois ; commit et PR ciblés avec fichiers modifiés, différences vis-à-vis de la spec, tests passés/échoués et contrôles manuels restants. Rebase/reconcilie avant chaque PR ; pas de force push sur `main`.
- Ne réécris pas vitesse native, audio, shiny rate, starter/œuf, évolutions solo validés. Des tests ciblés vérifient qu'ils restent corrects. Répare uniquement une régression constatée ou un écart explicite de la spec.
- Modifie le moteur HnS du fork si nécessaire pour les quatre vrais slots. Les pondérations sont `30/30/30/10` et les anciennes longueurs de tableaux ne constituent aucune règle produit. Préserve les autres systèmes non concernés.
- Rocket = dernier cap **champion/rival jalon** +2, sans enchaîner sur un Rocket antérieur. Une équipe NORMAL/HARD reste identique en contenu. Les objets de soins suivent la disponibilité native à ce stade.
- Le rival tire catégorie puis starter, conserve le résultat et développe un thème de combat correspondant. Les rosters précis restent à proposer après inventaire, jamais à déclarer validés d'avance.
- Shiny Only est **post-V1** ; il sera plus tard une ligne `Shiny Only — ON/OFF` dans Nuzlocke. Ne l'implémente pas dans cette V1.
- Les boutiques ne se limitent pas aux anciens catalogues : capacités actives compatibles, objets pertinents absents des magasins habituels, et PNJ Méga-Gemmes dédié à créer si absent.

## Portes de sortie

Validator actualisé + tests ciblés, `make hns -j4`, non-régression des systèmes protégés, contrôles boss/rival/Rocket/rencontres/shop, puis mGBA et revue FR/EN/wiki/mobile avant V1. N'annonce pas la release tant que les contrôles manuels et écarts de données ne sont pas clos.
