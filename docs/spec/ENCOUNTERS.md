# Rencontres V1

Les sources détaillées sont `data/spec/encounters_standard.json` (405 tables) et `data/spec/encounters_special.json` (4 Headbutt et 53 Safari). Chaque table active a quatre **entrées réelles**, avec poids `[30,30,30,10]`, sans duplication ou décomposition 9+1. Chaque méthode disponible dans une zone a sa propre table ; les niveaux et variantes temporelles viennent des données. Les starters sauvages autorisés sont rares dans le slot 10 %, les évolutions finales puissantes suivent la progression, et aucun légendaire/fabuleux ordinaire n'est ajouté.

`land_mons`, `water_mons`, `rock_smash_mons` et `fishing_mons` désignent les méthodes de la source standard. Les cannes ouvrent la pêche selon la progression. La source actuelle ne décrit **pas** de pools différents par canne : ne pas en inventer. Le code doit adapter le sélecteur Tactica aux quatre entrées et à leurs poids, y compris lorsque l'ancien moteur impose 12/5/10 entrées. Une structure héritée de HnS n'est pas une règle de conception Tactica.

Les pools Safari restent au nombre de 53, avec rotation par secteur/session ; les quatre tables Headbutt restent distinctes. Vérifier que le moteur conserve leurs niveaux et méthodes et que l'interface de localisation montre la même source. Le validateur doit prouver unicité logique, comptes, quatre espèces, taux et tirages limites `0–29`, `30–59`, `60–89`, `90–99` sur chaque méthode représentative.

## Couverture Pokédex pré-Ligue et déduplication inter-zones

L'objectif de la V1 est de rendre **100 %** des familles/espèces non légendaires et non fabuleuses prévues par le build accessibles avant la première Ligue via la capture d'une forme de base ou d'une pré-évolution permettant ensuite d'obtenir la famille. Une couverture **90–95 %** reste acceptable uniquement lorsqu'une contrainte réelle de progression, d'habitat ou de mécanique est documentée.

La diversité prime sur la répétition. Une forme de base déjà disponible ne doit pas occuper plusieurs routes/zones standard au détriment d'une autre famille encore absente avant la Ligue. Une forme de base a donc **une zone canonique principale** avant la Ligue. Wattouat, Mimigal/Spinarak et les cas similaires doivent être dédupliqués lorsqu'ils apparaissent sur plusieurs routes. Les répétitions exceptionnelles doivent être rares, justifiées et ne jamais réduire la couverture pré-Ligue.

L'audit doit identifier en priorité les familles uniquement accessibles trop tard. **Scorplane doit être capturable avant la première Ligue** ; obtenir plus tard Scorvol à Kanto puis reproduire ne constitue pas une disponibilité acceptable de la famille.

Le rapport d'audit doit fournir au minimum :
- le taux de couverture pré-Ligue ;
- les familles encore absentes ;
- les formes de base répétées sur plusieurs zones ;
- les slots libérables et les remplacements proposés ;
- la justification habitat/progression de chaque remplacement.

## Stade d'évolution au niveau réel

Chaque encounter doit employer le stade évolutif cohérent avec son niveau réel. Une forme de base ne doit pas rester artificiellement sauvage au-delà de son niveau d'évolution normal lorsqu'une évolution par niveau est disponible.

Exemple normatif : **Héricendre niv. 33 doit être remplacé par Feurisson niv. 33**.

La règle s'applique à toutes les familles. Les évolutions par pierre, bonheur ou condition spéciale sont traitées selon la progression et l'habitat, mais l'objectif reste d'éviter des formes de base absurdes à haut niveau.

## Validation attendue

En plus des contrôles existants, le validateur/audit doit vérifier :
1. quatre slots réels par méthode et les poids `30/30/30/10` ;
2. niveau maximal compatible avec `earliest_access_cap` ;
3. taux de couverture Pokédex/familles avant la première Ligue ;
4. absence de doublons inter-zones de formes de base, sauf exception documentée ;
5. disponibilité pré-Ligue des familles explicitement requises, notamment Scorplane ;
6. cohérence du stade d'évolution avec le niveau de rencontre.
