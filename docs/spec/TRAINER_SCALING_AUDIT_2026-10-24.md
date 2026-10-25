# Audit global des niveaux des dresseurs — 24 octobre 2026

## Périmètre et livrables

Correctif fusionné : [PR #68](https://github.com/DyHyugal/pokemon-tactica/pull/68), commit `63dbd3d9edfbcb7646c5bf6baebbd539627d57e2`. [CI de PR réussie](https://github.com/DyHyugal/pokemon-tactica/actions/runs/40136575716). Le publieur vérifie le succès de [la CI complète de ce commit](https://github.com/DyHyugal/pokemon-tactica/actions/runs/40138221574) avant de construire la candidate.

Départ : `integration/v1` au commit `bf84dc9a2e45288ab7f86494a0e3e497336d33d4`, après fusion du correctif de légalité des boss (PR #67).

**560 cartes examinées**, Johto, Kanto et cartes annexes HnS compris. Les scripts et la table des revanches référencent **474 équipes ordinaires distinctes sur 99 cartes**. Les 60 autres identifiants ordinaires présents dans les données ne sont référencés par aucun de ces scripts ni par la table de revanches ; ils sont inventoriés séparément, sans leur inventer une localisation.

- [Tableau complet CSV, ouvrable dans Excel](../assets/tactica-trainer-scaling.csv) : chaque dresseur placé et chaque revanche, niveaux écrits, bande de zone et niveaux attendus à quatre étapes. Les cartes sans équipe ordinaire référencée figurent aussi dans le tableau.
- [Inventaire machine](../assets/tactica-trainer-scaling-inventory.json) : cartes, couverture et identifiants non référencés.
- `test/tactica_trainer_scaling_cases.h` : matrice de toutes les associations carte/dresseur pour les tests moteur.

Le CSV applique quatre contextes de contrôle à chaque carte, même si cette carte n'est normalement pas encore accessible dans l'un de ces contextes. Ses colonnes indiquent des **niveaux attendus selon le contrat**, pas un accès scénaristique accordé à ces zones. Les tests vérifient ensuite les équipes réellement créées.

Les Champions, le Conseil 4, le rival, les admins Rocket et les rencontres spéciales restent protégés. Leurs règles propres et Méga sont couvertes par [l'audit des boss](BOSS_SET_AUDIT_2026-10-24.md) ; elles ne sont pas remplacées par le scaling des dresseurs ordinaires.

## Diagnostic : deux défauts de progression, pas un interrupteur de menu

Le scaling des **niveaux** est automatique et indépendant du niveau du joueur, de NORMAL/HARD et du réglage de cap. Les options « Trainer Scaling IVs/EVs » ne changent que les IV/EV ; les activer n'aurait pas corrigé ces défauts.

**1. Premier rival oublié.** La liste partagée des jalons de cap commence à Albert, afin que le premier duel du rival ne fasse pas sauter la préparation du premier badge. Cette même liste était utilisée pour la progression des dresseurs ordinaires et omettait donc la victoire contre le rival niveau 18. Test avant correction : plancher obtenu **0**, attendu **18**. Les routes initiales restaient alors au niveau de leurs tables (par exemple Route 30 : 4–6, Route 31 : 7–11), bien après cette victoire.

Correction : compter les trois variantes historiques du premier rival dans le plancher des dresseurs ordinaires, sans les ajouter à la progression du cap joueur. Sa victoire établit immédiatement **18** ; la victoire ultérieure sur Albert ne fait pas redescendre ce niveau. Une ancienne sauvegarde ayant déjà cette victoire en bénéficie sans migration de format.

**2. Groupe central de Kanto.** Le cap joueur attend les victoires contre Morgane, Erika et Jeannine avant de passer au palier suivant. Le plancher des dresseurs ordinaires attendait lui aussi les trois, au lieu de tenir compte de chaque boss déjà vaincu. Test avant correction : après Major Bob et une seule victoire centrale, plancher obtenu **75**, attendu **80**.

Correction : pour les niveaux des dresseurs ordinaires, lire chaque victoire individuelle. Le plancher devient **80** dès la première des trois victoires. Le verrou du cap joueur reste à 80 tant que le groupe n'est pas terminé, puis passe à 85 comme prévu. Les Rocket dynamiques restent exclus du cumul des planchers.

## Application des données de zone

Le calcul existant reste celui du contrat : le niveau minimal d'une équipe est relevé au maximum de son minimum écrit, du plancher de sa zone et du plus haut minimum de roster déjà vaincu. Les différences internes restent bornées par la bande zone/scénario, les niveaux écrits plus élevés ne sont pas abaissés et la limite absolue reste 100.

Les tables de rencontres standard et les niveaux des boss ne sont pas rééquilibrés arbitrairement pour corriger ce défaut de suivi des victoires. Les tables de zone générées sont synchronisées et contrôlées. Les évolutions déterministes et les capacités adaptées suivent le niveau final via le chemin de création utilisé en combat.

**31 cartes occupées n'ont pas de bande sauvage de zone**, notamment certains intérieurs d'Arènes, la Tour Radio, le phare, l'Aquaria et des cartes annexes. « 0 » dans leur colonne de zone signifie **pas de plancher de zone renseigné**, pas un Pokémon de niveau 0. Elles utilisent le niveau écrit et le plancher du scénario ; elles sont incluses dans les tests globaux. Aucun niveau de zone n'est inventé pour ces cartes, conformément à `PROGRESSION.md`.

## Exemples attendus après correction

| Carte / dresseur | Niveaux écrits | Avant victoire sur le premier rival | Après victoire sur le premier rival | Après Blanche | Après Ligue 2 |
|---|---|---|---|---|---|
| Route 30 — Joey | 4 | 4 | 18 | 29 | 100 |
| Route 30 — Mikey | 2 / 4 | 4 / 6 | 18 / 18 | 29 / 29 | 100 / 100 |
| Route 31 — Wade | 4 / 5 | 7 / 8 | 18 / 18 | 29 / 29 | 100 / 100 |
| Caves Jumelles — Russell | 9 / 8 | 17 / 16 | 19 / 18 | 29 / 29 | 100 / 100 |
| Route 34 — Samuel | 12 / 10 / 12 | 27 / 25 / 27 | 27 / 25 / 27 | 29 / 29 / 29 | 100 / 100 / 100 |

Le deuxième rival, après Hector, établit déjà le plancher 29 avant Blanche. Une équipe réellement restée aux niveaux écrits 10–12 sur Route 34 après ce duel ne correspond pas au calcul de cette version ; vérifier le SHA/version du README distribué et reprendre avec **Continuer depuis une sauvegarde en jeu**, pas avec un ancien état instantané d'émulateur.

## Vérification moteur et CI

La ROM de test inclut maintenant toutes les équipes de production avec leurs vrais IDs. Les 14 équipes réservées au banc de test sont ajoutées après la table de production, sans remplacer les premiers dresseurs. L’audit des tests du même jour a révélé que l’ancien montage remplaçait ces 14 IDs : les anciennes validations de la matrice ne prouvaient donc pas l’utilisation des équipes réelles pour ces IDs. Ce défaut concernait les tests, pas le runtime distribué.

Le contrôle courant utilise `CreateNPCTrainerParty`, le point d’entrée réel du combat, pour les **474 associations carte/dresseur** (routes, grottes, intérieurs, sbires et revanches), avec des difficultés et contextes scénario répartis. Il vérifie des bornes ; il ne démontre pas à lui seul que la table de niveaux choisie est correcte. Un témoin indépendant ajoute **11 cas × 2 difficultés × 3 réglages de cap = 66 constructions**, avec effectifs et niveaux attendus littéraux pour Joey, Wade, Russell, Samuel, Carrie, Stephen et une revanche.

La matrice exhaustive **474 × 2 difficultés × 4 contextes = 3 792 constructions** reste disponible sur demande, et non à chaque PR. Les contrôles statiques de couverture de toutes les cartes restent exécutés. Une association nouvelle doit être intégrée via `--write-fixture`. Les caps Johto/Kanto/deux Ligues disposent de leur propre suite ; les doublons de parcours sont retirés. La protection des boss et le non-cumul Rocket sont conservés.

```sh
python3 tools/audit_tactica_trainer_scaling.py
python3 tools/sync_tactica_trainer_levels.py --check
python3 tools/run_tactica_tests.py --suites progression
# Diagnostic complet uniquement si nécessaire :
python3 tools/run_tactica_tests.py --suites progression --exhaustive
```

La preuve de détection d’une panne volontaire et les résultats exécutés figurent dans [l’audit des tests](TEST_SUITE_AUDIT_2026-10-24.md).

Le playtest manuel complet de chaque carte reste distinct de ces contrôles automatisés. Témoins owner utiles : reprendre une ancienne sauvegarde après le premier rival et voir Joey/Wade au moins niveau 18 ; comparer un dresseur Route 34 avant Blanche après le deuxième rival ; vérifier le plancher 80 après une seule victoire centrale de Kanto. Les noms de fichier ROM/sauvegarde restent fixes et le format de sauvegarde est conservé.
