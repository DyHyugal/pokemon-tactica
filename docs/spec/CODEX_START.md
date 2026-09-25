# Prompt de reprise Codex — Pokémon Tactica V1

Tu travailles dans le nouveau dépôt Pokémon Tactica, branche `integration/v1` ou branche fonctionnelle réconciliée avec celle-ci. Lis d'abord `AGENTS.md`, `docs/spec/SOURCE_OF_TRUTH.md`, puis tous les autres `docs/spec/*.md` et `data/spec/*.json`. Ces fichiers sont les **seules sources normatives du produit**. Le code importé montre l'état actuel, les docs HnS sont techniques/upstream. Ne transforme jamais une ancienne habitude HnS ou un vieux prompt en règle Tactica. Les rosters Rocket et les six rosters rival sont décrits dans `ROSTERS_ROCKET_RIVAL.md` et les JSON correspondants ; distingue toujours décision produit, donnée canonique, intégration moteur et recette ROM.

Ce dépôt a un **nouvel historique** : lire `docs/spec/MIGRATION.md` pour les branches et SHA de provenance. Une régression d'une feature autrefois validée peut être révélée par le changement de dépôt, de branche, de fichiers ou d'environnement. Comparer les fichiers concernés avec l'ancienne production et reproduire le défaut avant de conclure ou de réécrire la feature. Tenir à jour `docs/spec/FEATURE_STATUS.md` : **un tag d'état par feature**, avec preuve, branche/SHA, test et prochaine action.

## Décisions normalisées owner — 25 septembre 2026

Ces décisions sont postérieures aux anciens handoffs et doivent être propagées dans les documents et données concernés avant de conclure un audit.

### Gameplay, progression et interface

- Le thème visuel custom Tactica est **noir / rouge / gris**. Le blanc n'est pas une couleur de surface/texte dominante des interfaces custom ; HUD, menus, Summary et fenêtres des PNJ custom doivent rester lisibles dans cette palette.
- Vitesse native x1/x2/x3/x4, audio et fonctionnement du sélecteur de starter sont confirmés en jeu. Le menu starter est fonctionnel ; le seul point restant est du **polish UX** : après un retour, replacer le curseur sur le choix précédent au lieu de repartir au début.
- L'œuf d'Orme donne un second starter mais **ne doit pas permettre de reprendre exactement le starter principal**. Exclure ce choix dans l'UI et garder une garde côté logique.
- Le rival conserve son starter/catégorie et ses rosters thématiques, mais son **profil de niveaux doit suivre le prochain boss/Champion obligatoire**. Exemple : après le badge 2, le rival avant Blanche utilise le profil de Blanche, soit membres autour de 29 et ace à 32, au lieu des anciens niveaux historiques 16–19/24.
- Albert = 3 Pokémon ; Hector = 4 ; Blanche = 6. Pour Blanche, remplacer le slot Ursaring/Teddiursa obsolète par **Ursaring niv. 30 @ Orbe Flamme, Cran, Façade / Séisme / Close Combat / Mâchouille**. Réconcilier `data/spec/bosses.json`, données moteur, wiki et tests.
- Salamèche et Reptincel **Dragon / Feu** ont été confirmés en ROM ; ne pas régresser ces types.

### Wiki : contenu et comportement

Le wiki ne doit pas seulement être joli : son **contenu doit être remis en cohérence avec les décisions finales**. L'ancien wiki du dépôt `johto-family-remix` / branche `wiki/family-remix-v1` peut servir de référence éditoriale pour les explications déjà bonnes, mais **jamais de source normative** : toute donnée doit être recoupée avec `docs/spec/`, `data/spec/` et le code réellement intégré.

- **Guide de jeu** : faire un vrai guide pratique, pas une simple introduction. Expliquer concrètement NORMAL/HARD, paramètres, level caps, starters/œuf, Training NPC, shiny rate, vitesse, audio, boutiques/PNJ, préparation des combats et progression utile.
- **Changements** : page exhaustive des changements réellement implémentés, pas un échantillon. Couvrir systèmes, 26 Pokémon modifiés, 15 learnsets, évolutions, encounters, boutiques/PNJ et règles de boss. Pour les Pokémon : une ligne/carte `sprite + nom + type(s) + modifications précises`.
- **Pokédex** et **Localisations** restent deux pages séparées. Ne pas remettre de colonne/section « Progression » dans le Pokédex. Les Localisations viennent des tables canoniques et affichent zone, méthode, niveau, taux et jour/nuit si applicable.
- **Boss & Conseils** : montrer les équipes réellement canoniques avec sprites transparents/détourés, niveaux, objets, talents et attaques, puis des conseils stratégiques compréhensibles sans imposer une solution tour par tour.
- FR = noms officiels français des Pokémon, attaques, talents, objets et types ; EN = équivalents officiels anglais. Aucun anglais résiduel sur la page FR.
- La recherche FR doit indexer **les noms français en premier**, tout en acceptant l'anglais comme alias et idéalement les saisies sans accents. L'EN fait l'inverse.
- Le bouton de langue doit ouvrir **la page équivalente dans l'autre langue**, jamais renvoyer systématiquement à l'accueil.
- Corriger les entêtes de tableaux qui recouvrent la première ligne ; si le sticky header ne peut pas être fiable, privilégier un header non sticky mais lisible.
- Déplacer le suivi du développement/V1/V2 hors des Crédits vers une page **Roadmap** FR/EN. Les Crédits restent consacrés aux projets amont, contributeurs et licences.
- Palette wiki et UI custom : **noir / rouge / gris, sans grandes surfaces blanches** ; vérifier le résultat réel navigateur/mobile.

### Audit encounters — couverture Pokédex avant la première Ligue

L'audit `earliest_access_cap` ne doit pas se limiter aux niveaux. Il doit aussi optimiser la **diversité et la disponibilité réelle du Pokédex avant la première Ligue**.

1. Conserver strictement **4 vrais slots par table et par méthode applicable**, pondérés `30/30/30/10`.
2. Cible : rendre **100 %** des familles/espèces non légendaires et non fabuleuses prévues par le build accessibles avant la première Ligue via capture d'une forme de base/pré-évolution permettant d'obtenir la famille. **90–95 %** est acceptable uniquement si une contrainte réelle de progression, d'habitat ou de mécanique est documentée.
3. Supprimer les doublons/triplons de formes de base entre zones standard lorsqu'ils consomment des slots qui pourraient rendre une autre famille accessible. Une forme de base doit avoir **une zone canonique principale** avant la Ligue ; par exemple Wattouat ne doit pas être réparti sur plusieurs routes. Même logique pour Mimigal/Spinarak et les répétitions similaires.
4. Les exceptions de répétition doivent être rares, explicitement justifiées et ne jamais se faire au détriment d'une famille encore indisponible avant la Ligue.
5. Auditer en priorité les familles actuellement absentes ou trop tardives. **Scorplane doit être disponible avant la première Ligue** ; le fait de pouvoir obtenir Scorvol à Kanto puis reproduire ne compte pas comme une disponibilité acceptable.
6. Le stade sauvage doit respecter le niveau réel de la rencontre. Exemple normatif : **un Héricendre niv. 33 doit devenir un Feurisson niv. 33**. Appliquer la même logique à toutes les familles : ne pas conserver artificiellement une forme de base au-delà de son niveau d'évolution normal.
7. Préserver habitat, méthode, progression et rareté : les remplacements servent à augmenter la couverture sans transformer les biomes en listes arbitraires.
8. Le rapport d'audit doit fournir : taux de couverture pré-Ligue, familles absentes, doublons inter-zones détectés, remplacements proposés, légalité des stades/niveaux et impact sur les quatre slots.

## Premier bloc : prise de contexte, sans changement de gameplay

1. Note SHA, branche et propreté Git. Vérifie la présence et la cohérence de tous les documents et JSON canoniques. Repère les restes de documentation produit contradictoire et les mentions de l'ancien nom visibles au joueur ; garde les crédits/licences et identifiants HnS techniques nécessaires.
2. Compare le code, le validator, les tests, le wiki et les données aux contrats V1. Mets à jour le registre `FEATURE_STATUS.md` **feature par feature** avec son tag, les chemins, preuves, tests et prochaine action ; distingue code fusionné, branches en attente et travail local non poussé (non observable). Produis aussi la synthèse `DONE / PARTIEL / À CORRIGER / À FAIRE`, en explicitant les états `À RECONTRÔLER` et `À AUDITER` qui ne sont pas des validations post-migration. Examine notamment les 405 tables standard, 57 spéciales, couverture Pokédex pré-Ligue, doublons inter-zones, Rocket sans chaîne, tirage/règle de niveaux du rival et rosters, Méga uniques, Blanche/Ursaring, Jasmine, Dracaufeu, boutiques exhaustives, traduction FR/EN, contenu réel du wiki et palette noire/rouge/grise.
3. Exécute seulement les vérifications appropriées pour établir la baseline. Ne déclare pas validé un test mGBA que tu n'as pas joué. Signale les écarts et propose l'ordre des PR. Pour les encounters, produire le dataset `earliest_access_cap` **et** l'audit de couverture/diversité avant Ligue ; pour le wiki, auditer le fond avant la forme. Les contrôles de rival, évolutions/capacités au niveau réel, revue FR/EN et recette ROM restent basés sur les mêmes sources canoniques.

## Production par blocs

- Une branche de domaine issue d'`integration/v1` à la fois ; commit et PR ciblés avec fichiers modifiés, différences vis-à-vis de la spec, **tags des features touchées**, tests passés/échoués et contrôles manuels restants. Mettre à jour le registre avec les preuves du résultat. Rebase/reconcilie avant chaque PR ; pas de force push sur `main`.
- Ne réécris pas vitesse native, audio, shiny rate, cœur du sélecteur starter/œuf ou évolutions solo validés. Des tests ciblés vérifient qu'ils restent corrects. Répare uniquement une régression constatée ou un écart explicite de la spec ; le retour de curseur starter et l'interdiction du doublon d'œuf sont des corrections ciblées.
- Modifie le moteur HnS du fork si nécessaire pour les quatre vrais slots. Les pondérations sont `30/30/30/10` et les anciennes longueurs de tableaux ne constituent aucune règle produit. Préserve les autres systèmes non concernés.
- Pour les encounters, privilégier la couverture pré-Ligue et éliminer les répétitions de formes de base entre routes avant d'ajouter une nouvelle occurrence d'une espèce déjà disponible. Toujours convertir vers le stade d'évolution légal/cohérent au niveau de rencontre.
- Rocket = dernier cap **champion/rival jalon** +2, sans enchaîner sur un Rocket antérieur. Une équipe NORMAL/HARD reste identique en contenu. Les objets de soins suivent la disponibilité native à ce stade.
- Le rival tire catégorie puis starter, conserve le résultat et développe un thème de combat correspondant. Ses équipes utilisent le profil de niveaux du **prochain boss obligatoire**, pas les niveaux historiques importés. Résoudre les évolutions/capacités légales puis valider en ROM.
- Shiny Only est **post-V1** ; il sera plus tard une ligne `Shiny Only — ON/OFF` dans Nuzlocke. Ne l'implémente pas dans cette V1.
- Les boutiques ne se limitent pas aux anciens catalogues : capacités actives compatibles, objets pertinents absents des magasins habituels et PNJ Méga-Gemmes dédié.

## Portes de sortie

Validator actualisé + tests ciblés, `make hns -j4`, non-régression des systèmes protégés, contrôles boss/rival/Rocket/rencontres/shop, couverture pré-Ligue documentée, puis mGBA et revue **contenu + UI** du wiki FR/EN/mobile avant V1. N'annonce pas la release tant que les contrôles manuels et écarts de données ne sont pas clos.
