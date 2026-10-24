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

Validation locale terminée : **14 groupes mGBA réussis**, dont les quatre groupes couvrant les 3 792 créations, le parcours complet des jalons et les deux régressions reproduites avant correction. Build HnS réussi ; **20 tests Python réussis**. Les contrôles de couverture, de données et de synchronisation passent. La CI distante doit confirmer le commit avant fusion.

Les tests passent par `CreateNPCTrainerParty`, **le même point d'entrée que le combat réel**, avec l'identifiant du dresseur et la localisation sauvegardée. Cette fonction existante est rendue accessible aux tests ; sa logique de construction n'est pas réécrite. Les anciens tests ne vérifiaient que le helper de résolution et n'avaient pas cette couverture de bout en bout.

Matrice : **474 associations × 2 difficultés × 4 contextes = 3 792 créations réelles d'équipes**, réparties en quatre groupes de test, couvrant les routes, grottes, intérieurs, sbires et revanches de toutes les cartes concernées. Les réglages de cap 0/1/2 sont exercés dans cette matrice. Chaque Pokémon créé doit respecter le minimum zone/scénario, ne pas descendre sous son niveau écrit et rester dans la borne haute autorisée, au plus 100.

En complément : test des trois variantes du premier duel, des trois victoires centrales prises séparément, de la conservation des caps joueur, et d'un parcours des jalons Johto/Kanto/deux Ligues en NORMAL/HARD. Les tests existants de protection des boss et de non-cumul Rocket sont maintenus.

```sh
python3 tools/audit_tactica_trainer_scaling.py
python3 tools/audit_tactica_trainer_scaling.py --report-dir /tmp/tactica-scaling
python3 tools/sync_tactica_trainer_levels.py --check
python3 -m unittest discover -s tools/tests -p 'test_tactica_*.py'
make hns -j4
make check BUILD=hns TESTS='Tactica ordinary trainers:' -j4
make check BUILD=hns TESTS='*Level cap:' -j4
make check BUILD=hns TESTS=Family -j4
make check BUILD=hns TESTS='Tactica boss legality:' -j4
```

La matrice est protégée par `--check` implicite dans l'auditeur. Une nouvelle association carte/dresseur doit être intégrée via `--write-fixture`. La CI des PR et la CI d'intégration exécutent le contrôle de couverture et les tests de scaling.

Le playtest manuel complet de chaque carte reste distinct de ces contrôles automatisés. Témoins owner utiles : reprendre une ancienne sauvegarde après le premier rival et voir Joey/Wade au moins niveau 18 ; comparer un dresseur Route 34 avant Blanche après le deuxième rival ; vérifier le plancher 80 après une seule victoire centrale de Kanto. Les noms de fichier ROM/sauvegarde restent fixes et le format de sauvegarde est conservé.
