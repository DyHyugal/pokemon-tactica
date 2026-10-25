# Audit des tests Tactica — 24 octobre 2026

## Périmètre et objectif

Base : `39bbe7407d6811949026dfac4e00e95b9f57fff5`, branche `chore/v1-test-relevance`.
Audit des suites Tactica utilisées par la CI, des validateurs associés et des tests amont explicitement sélectionnés. Les autres tests de mécaniques amont restent disponibles ; cet audit ne prétend pas les avoir tous exécutés ou validés.

Une assertion doit vérifier un comportement réel ou un contrat de données identifié. Son nom et son compte rendu doivent décrire cette portée. Un résultat vert ne signifie ni « jeu entièrement testé », ni « interface agréable », ni « parcours owner effectué ».

## Défaut découvert dans le banc de test

Les fixtures `trainer_control.party` occupaient les IDs 1–14 dans `gTrainers` de la ROM de test. Le générateur de la table de test excluait donc les vrais dresseurs de ces IDs. Par exemple, Joey était remplacé par trois Pokémon de test au lieu de son Rattata. Des vérifications de bornes pouvaient réussir avec ces fausses équipes.

La ROM de test inclut désormais directement toute la table de production HnS. Ses 14 fixtures sont ajoutées après le dernier ID de production ; des assertions de compilation empêchent leur collision avec les dresseurs et les partenaires. Le runner de combat et les tests amont de construction utilisent les nouveaux IDs des fixtures. Le générateur intermédiaire qui excluait les premiers IDs est supprimé.

Le contrôle indépendant d’effectif/niveau a échoué sur Joey avant cette correction, puis réussi après. Les parcours globaux refusent aussi une équipe vide, pour éviter qu’une boucle de zéro Pokémon puisse réussir. La ROM distribuée n’utilisait pas ces fixtures et n’est pas modifiée par ce défaut du banc de test.

## Décisions

| Domaine | Décision et preuve visée |
|---|---|
| Achat de Méga-Gemme | Suppression du faux test qui appelait lui-même `AddBagItem` et `RemoveMoney`, sans passer par la boutique. Catalogue et prix conservés ; achat complet à tester dans l’interface. |
| Enseignement payant | Suppression de l’enseignement manuel dans le test ; nom limité au paiement réellement appelé. Compatibilité et refus de paiement conservés. |
| Preset recommandé | Suppression du `SetCurrentDifficultyLevel` exécuté par le test lui-même ; difficulté vérifiée après la vraie callback de sauvegarde du menu. |
| Dresseurs ordinaires | Construction réelle, 474 associations carte/dresseur couvertes. Ajout de 66 cas avec effectifs/niveaux littéraux indépendants. Le parcours des caps n’est plus recopié dans cette suite. |
| Matrice exhaustive des dresseurs | Conservée comme diagnostic explicite de 3 792 constructions, hors exécution courante. La couverture de toutes les associations reste obligatoire ; réduction de la répétition des contextes, pas de la couverture géographique. |
| Rocket / Major Bob | Utilisation du vrai point d’entrée de création d’équipe. Les niveaux Rocket attendus sont des valeurs littérales issues des minima/maxima canoniques, au lieu d’un second appel au getter testé. |
| Légalité des boss | Audit statique complet conservé. Suppression des passages complets redondants dans les unit tests Python et des lectures de données dupliquées en C. Assertions du rival intégrées à la boucle existante des six archétypes. |
| Méga-Évolutions | Combats réels des 28 couples conservés des deux côtés : talent, deux types, espèce et retour joueur. Les deux tests d’IA Méga sont sélectionnés, dont le choix réel d’utiliser la Méga. |
| Validateur Méga | Retrait des recherches de mots dans le C. Il vérifie seulement l’ordre statique badge 4 → remise du bracelet dans le script, en ignorant les commentaires. Il ne prouve pas que ce script est exécuté. |
| Validateur UI | Retrait des recherches de noms de fonctions, flags et même d’un commentaire. Conservation des contrôles des vrais fichiers graphiques, tailles des tilemaps, libellés neutralisés et centrage du texte dans sa bulle. Retrait du code de rendu jamais appelé. |
| Configuration UI / données Méga | Contrôle des valeurs réellement compilées en C, plutôt que de la présence d’un texte dans un fichier. Les tests de pixels/navigation existants restent distincts. |
| Messages et vitesse | Tests du vrai contrôleur, des nouveaux appuis A/B et de FASTER conservés. Le test de compatibilité du combat sous x4 ne constitue pas une mesure chronométrée de la vitesse. |
| Autres régressions | Évolutions et consommation d’objet, caps, starters/migration/PC plein, IA de terrain/setup, audio, CS, rencontres et entraînement conservés dans leurs domaines. |

## Exécution et CI

`tools/run_tactica_tests.py` est l’entrée des suites natives retenues. Il utilise des fichiers exacts quand possible, et un préfixe dédié pour séparer le diagnostic exhaustif du contrôle courant des dresseurs. Il refuse un échec de commande, l’absence de bilan, zéro test ou un total différent du nombre de tests réussis. Les tests ignorés par `ASSUME`, les échecs connus et les tâches non implémentées ne peuvent donc pas valider la suite. Chaque commande produit un journal et son nombre de groupes/durée ; la CI conserve ces fichiers en artifact.

`tools/tactica_test_plan.py` sélectionne les domaines d’après les fichiers modifiés. Un test isolé lance son domaine. Une modification du runtime, des données, des outils/générateurs ou d’un élément partagé non classé lance toutes les suites retenues par précaution. Un changement documentaire seul ne reconstruit pas la ROM, y compris après merge. Une modification du seul workflow de publication conserve les contrôles de données sans recompiler le jeu. La sélection n’est pas une analyse complète du graphe de dépendances.

```sh
python3 -m unittest discover -s tools/tests -p 'test_tactica_*.py'
python3 tools/run_tactica_tests.py --suites progression bosses --jobs 4
python3 tools/run_tactica_tests.py --suites all --jobs 4
# Diagnostic optionnel :
python3 tools/run_tactica_tests.py --suites progression --exhaustive --jobs 4
```

La commande manuelle GitHub Actions permet également de demander la matrice exhaustive. Les tests amont de construction sont conservés pour vérifier la séparation des fixtures. Leur présence dans ce profil ne signifie pas que tout le catalogue amont est exécuté. Le build natif compile encore le catalogue de tests amont ; ce tri ne promet pas de supprimer ce coût de compilation.

## Vérifications exécutées

Validation locale : **174 groupes natifs réussis**, répartis sur les 22 filtres uniques du profil courant, sans test ignoré. Les tests paramétrés restent comptés comme des groupes ; par exemple, les deux groupes Méga couvrent chacun 28 couples. Les 20 groupes amont de construction des fixtures passent également et sont inclus dans les 174. Les groupes Safari ne sont plus relancés une seconde fois via un préfixe plus large. L’attente amont du surnom par défaut a été adaptée au nom d’espèce configuré, qui est en majuscules dans Tactica.

**24 tests Python réussis**, dont les refus de bilan vide/incomplet, de test ignoré, d’échec connu et de commande en erreur, ainsi que les garde-fous de sélection CI. Les contrôles de données, générateurs `--check`, assets UI, localisation et index documentaire passent. Le build de production HnS réussit également ; aucune mutation n’est conservée. Le diagnostic exhaustif a aussi réussi : quatre groupes, 3 792 constructions, environ 277 secondes sur ce workspace.

**Preuve négative de scaling** : désactivation temporaire de l’appel à `ResolveTacticaOrdinaryTrainerMon` dans la vraie création d’équipe. Le témoin indépendant échoue immédiatement avec `EXPECT_EQ(4, 7)` (niveau obtenu 4, attendu 7). Après restauration, il réussit ; l’empreinte SHA-256 du fichier avant et après est identique (`4e999da854234c19a07a02b4717c9c42646e9cf3c91730cae70bd6616755b758`). La mutation ne fait pas partie du commit.

Les 22 filtres ont totalisé environ 141 secondes sur le workspace local **après compilation**, sans la matrice exhaustive. Ce n’est pas une promesse de durée sur une CI propre : la compilation et le matériel diffèrent. Journaux et résultats locaux : `triage-results-consolidated.json`, `triage-mutation-disabled.log`, `triage-mutation-restored.log` ; la CI conserve les journaux de sa propre exécution en artifact.

## Limites maintenues visibles

- Le runner de combat contourne volontairement la possession du Méga-Bracelet sous `TESTING`. Les tests de transformation ne valident donc pas le déblocage en aventure ni la touche START du joueur.
- Achats et enseignement complets dans les interfaces, rendu esthétique, transitions de carte, perception de la vitesse et contrôle auditif restent des observations ROM.
- Les fixtures de Méga décrivent les identités custom validées du projet ; elles ne constituent pas une nouvelle certification indépendante de toutes les données officielles.
- Les bornes de la matrice globale proviennent des données générées. Elles contrôlent l’intégration et la couverture, tandis que les 66 cas littéraux apportent des témoins indépendants. Aucune de ces approches ne remplace le playthrough.
- Aucun changement de gameplay, de format de sauvegarde ou de contenu de la candidate publique n’est demandé par ce tri. Le profil de validation change ; une nouvelle ROM ne serait pas utile pour cela.
