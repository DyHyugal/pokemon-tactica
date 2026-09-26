# Rencontres V1

Les sources détaillées sont `data/spec/encounters_standard.json` (405 tables) et `data/spec/encounters_special.json` (4 Headbutt et 53 Safari). Chaque table active a quatre **entrées réelles**, avec poids `[30,30,30,10]`, sans duplication ou décomposition 9+1. Chaque méthode disponible dans une zone a sa propre table ; les niveaux et variantes temporelles viennent des données. Les starters sauvages autorisés sont rares dans le slot 10 %, les évolutions finales puissantes suivent la progression, et aucun légendaire/fabuleux ordinaire n'est ajouté.

`land_mons`, `water_mons`, `rock_smash_mons` et `fishing_mons` désignent les méthodes de la source standard. Les cannes ouvrent la pêche selon la progression. La source actuelle ne décrit **pas** de pools différents par canne : ne pas en inventer. Le code doit adapter le sélecteur Tactica aux quatre entrées et à leurs poids, y compris lorsque l'ancien moteur impose 12/5/10 entrées. Une structure héritée de HnS n'est pas une règle de conception Tactica.

Les pools Safari restent au nombre de 53, avec rotation par secteur/session ; les quatre tables Headbutt restent distinctes. Vérifier que le moteur conserve leurs niveaux et méthodes et que l'interface de localisation montre la même source. Le validateur doit prouver unicité logique, comptes, quatre espèces, taux et tirages limites `0–29`, `30–59`, `60–89`, `90–99` sur chaque méthode représentative.

## Premier accès et caps

`data/spec/encounter_access_caps.json` couvre explicitement les 354 couples carte/méthode des 128 cartes standard. Le cap effectif combine l'accès terrestre à la zone et le premier accès réel à la méthode : Vieille Canne au cap 19, Éclate-Roc après Simularbre au cap 38, Surf après Mortimer au cap 40. Les cartes bloquées par Siphon, Cascade, la Ligue, Kanto ou les badges de Kanto portent leur propre jalon plus tardif.

`tools/sync_tactica_encounter_access.py` abaisse uniquement une plage qui dépasse son cap, en conservant sa largeur. L'audit initial a corrigé 231 des 405 tables canoniques. `tools/validate_tactica_spec.py` exige une couverture exacte de chaque couple, vérifie `max_level <= earliest_access_cap` et interdit toute famille starter avant le deuxième badge. Après toute modification de ces caps ou niveaux, synchroniser successivement les caps puis `src/data/wild_encounters.json`.
