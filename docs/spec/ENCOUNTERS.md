# Rencontres V1

Les sources détaillées sont `data/spec/encounters_standard.json` (405 tables) et `data/spec/encounters_special.json` (4 Headbutt et 53 Safari). Chaque table active a quatre **entrées réelles**, avec poids `[30,30,30,10]`, sans duplication ou décomposition 9+1. Chaque méthode disponible dans une zone a sa propre table ; les niveaux et variantes temporelles viennent des données. Les starters sauvages autorisés sont rares dans le slot 10 %, les évolutions finales puissantes suivent la progression, et aucun légendaire/fabuleux ordinaire n'est ajouté.

`land_mons`, `water_mons`, `rock_smash_mons` et `fishing_mons` désignent les méthodes de la source standard. Les cannes ouvrent la pêche selon la progression. La source actuelle ne décrit **pas** de pools différents par canne : ne pas en inventer. Le code doit adapter le sélecteur Tactica aux quatre entrées et à leurs poids, y compris lorsque l'ancien moteur impose 12/5/10 entrées.

Les pools Safari restent au nombre de 53, avec rotation par secteur/session ; les quatre tables Headbutt restent distinctes.

## Premier accès et caps

`data/spec/encounter_access_caps.json` couvre explicitement les 354 couples carte/méthode des 128 cartes standard. Le cap effectif combine l'accès terrestre à la zone et le premier accès réel à la méthode : Vieille Canne au cap 19, Éclate-Roc après Simularbre au cap 38, Surf après Mortimer au cap 40. Les cartes bloquées par Siphon, Cascade, la Ligue, Kanto ou les badges de Kanto portent leur propre jalon plus tardif.

`tools/sync_tactica_encounter_access.py` abaisse uniquement une plage qui dépasse son cap, en conservant sa largeur. `tools/validate_tactica_spec.py` exige une couverture exacte de chaque couple, vérifie `max_level <= earliest_access_cap` et interdit toute famille starter avant le deuxième badge.

`tools/validate_tactica_encounter_evolutions.py` lit les évolutions par niveau réellement compilées et rejette toute forme sauvage dont l'évolution déterministe est déjà dépassée au niveau minimum de la table. Son audit initial du 26-09-2026 a fait évoluer 220 slots sans changer leur famille, leur habitat, leur taux ou leur plage de niveaux.

## Contrôle runtime obligatoire

Une validation JSON/compilée ne suffit pas à elle seule à déclarer les encounters « testés ». La ROM de candidate doit être reconstruite depuis le SHA annoncé et quelques zones témoins doivent être vérifiées en jeu.

### Route 36 — témoin de référence

État canonique actuel de `integration/v1` observé le 26-09-2026 :

| Méthode | Temps | Niveaux | Slots 30 / 30 / 30 / 10 |
|---|---|---:|---|
| Sol | Jour | 27–30 | Scraggy / Ariados / Bewear / Vulpix |
| Sol | Nuit | 27–30 | Crabrawler / Ariados / Sinistea / Helioptile |
| Éclate-Roc | Tous | 27–30 | Mienfoo / Cranidos / Shieldon / Drilbur |

Le propriétaire a signalé le 26/09 avoir l'impression de retrouver les anciennes rencontres de Route 36. Comme le `pokehns.gba` qu'il avait lancé pouvait ne pas avoir été recompilé après le pull, ce retour est **à recontrôler sur une ROM fraîche**.

Si la ROM fraîche ne produit pas les espèces de cette table :

1. vérifier le SHA ;
2. vérifier que `pokehns.gba` vient d'être recompilé ;
3. vérifier la synchronisation `data/spec/encounters_standard.json` → `src/data/wild_encounters.json` ;
4. diagnostiquer le sélecteur runtime avant de déclarer la candidate testable.

Toute divergence entre ces tables canoniques et la ROM bloque la candidate owner.
