# Rencontres V1

Les sources détaillées sont `data/spec/encounters_standard.json` (405 tables) et `data/spec/encounters_special.json` (4 Headbutt et 53 Safari). Chaque table active a quatre **entrées réelles**, avec poids `[30,30,30,10]`, sans duplication ou décomposition 9+1. Chaque méthode disponible dans une zone a sa propre table ; les niveaux et variantes temporelles viennent des données. Les starters sauvages autorisés sont rares dans le slot 10 %, les évolutions finales puissantes suivent la progression, et aucun légendaire/fabuleux ordinaire n'est ajouté.

`land_mons`, `water_mons`, `rock_smash_mons` et `fishing_mons` désignent les méthodes de la source standard. Les cannes ouvrent la pêche selon la progression. La source actuelle ne décrit **pas** de pools différents par canne : ne pas en inventer. Le code doit adapter le sélecteur Tactica aux quatre entrées et à leurs poids, y compris lorsque l'ancien moteur impose 12/5/10 entrées.

Les pools Safari restent au nombre de 53, avec rotation par secteur/session ; les quatre tables Headbutt restent distinctes.

## Premier accès et caps

`data/spec/encounter_access_caps.json` doit décrire **le premier accès réel à la zone dans la progression de l'histoire**. Le déblocage ultérieur d'une méthode (Vieille Canne, Éclate-Roc, Surf, etc.) contrôle seulement la disponibilité de cette méthode ; il **ne doit pas augmenter le niveau cible de la zone**.

Exemple : si une route est accessible avant Albert, ses tables restent calibrées sur le cap 17 même si Surf ou Éclate-Roc n'y deviennent utilisables que plus tard. Pour une table historiquement large de 3 niveaux, le recalage attendu est `14–17`.

Le dataset actuel fusionné contient encore des classifications erronées issues de l'ancien calcul par méthode. Il doit être audité/régénéré. `tools/sync_tactica_encounter_access.py` peut conserver la largeur des plages, mais le cap d'entrée doit provenir de la zone et du prochain champion/jalon pertinent, pas du déblocage de la méthode.

`tools/validate_tactica_encounter_evolutions.py` lit les évolutions par niveau réellement compilées et rejette toute forme sauvage dont l'évolution déterministe est déjà dépassée au niveau minimum de la table. Son audit initial du 26-09-2026 a fait évoluer 220 slots sans changer leur famille, leur habitat, leur taux ou leur plage de niveaux.

## Contrôle runtime obligatoire

Une validation JSON/compilée ne suffit pas à elle seule à déclarer les encounters « testés ». La ROM de candidate doit être reconstruite depuis le SHA annoncé et quelques zones témoins doivent être vérifiées en jeu.

### Route 36 — témoin de référence

État canonique actuel de `integration/v1` observé le 26-09-2026 :

| Méthode | Temps | Niveaux | Slots 30 / 30 / 30 / 10 |
|---|---|---:|---|
| Sol | Jour | **14–17 attendu** | Conserver les familles canoniques, avec stades légaux à 14–17 |
| Sol | Nuit | **14–17 attendu** | Conserver les familles canoniques, avec stades légaux à 14–17 |
| Éclate-Roc | Tous | **14–17 attendu** | Méthode débloquée plus tard, mais niveau toujours calé sur la progression de Route 36 |

État actuel connu à corriger : `encounter_access_caps.json` classe encore Route 36 `before_blanche` / cap 32 pour le sol et cap 38 pour Éclate-Roc ; les tables fusionnées sont donc restées 27–30. La passe d'évolution suivante a même remplacé Spinarak/Stufful par Ariados/Bewear parce qu'elle raisonnait à partir de ces niveaux erronés. La correction doit d'abord remettre la zone à 14–17, puis recalculer les stades d'évolution.

Si la ROM fraîche ne produit pas les espèces de cette table :

1. vérifier le SHA ;
2. vérifier que `pokehns.gba` vient d'être recompilé ;
3. vérifier la synchronisation `data/spec/encounters_standard.json` → `src/data/wild_encounters.json` ;
4. diagnostiquer le sélecteur runtime avant de déclarer la candidate testable.

Toute divergence entre ces tables canoniques et la ROM bloque la candidate owner.
