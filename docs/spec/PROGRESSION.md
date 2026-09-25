# Progression et caps

La courbe des niveaux d'ace validée est : Albert 17, Hector 25, Blanche 32, Mortimer 38, Chuck 45, Jasmine 52, Frédo 57, Sandra 64, Clément/Conseil 4 67, Maître 70. Les niveaux précis des autres membres viennent du roster canonique. Le cap courant ne décroît jamais.

Le rival est un combat de progression, mais **ses niveaux suivent le prochain boss/Champion obligatoire** afin qu'il reste cohérent avec la préparation du joueur. Il ne conserve pas les anciens niveaux historiques importés. Exemple normatif : après le badge 2, le rival précédant Blanche utilise le profil de Blanche, soit membres autour de 29 et ace à 32. Cette règle de niveau est distincte de la composition progressive 1/3/4/6 et doit être appliquée à chaque combat rival concerné.

Pour **chaque** boss Rocket, lire le dernier cap canonique champion/rival au moment de son combat, puis appliquer `+2`. Plusieurs Rocket sans nouveau jalon partagent donc le même niveau calculé. Leur propre niveau n'est jamais mémorisé comme cap de référence pour le Rocket suivant. Conserver leurs rosters NORMAL/HARD identiques, puis sélectionner la pré-évolution légale à ce niveau si nécessaire.

Les Pokémon des champions respectent également les niveaux d'évolution. Cochignon de Frédo est un choix intentionnel. Les Méga commencent au stade prévu par la progression du jeu ; inspecter le code et les combats avant de déclarer cette règle satisfaite.


## Rappel équipes des trois premières arènes

- Albert : 3 Pokémon.
- Hector : 4 Pokémon.
- Blanche : 6 Pokémon.

Décision owner du 25 septembre 2026 pour Blanche : le slot Ours doit être **Ursaring niv. 30**, talent **Cran**, objet **Orbe Flamme**, avec **Façade / Séisme / Close Combat / Mâchouille**. Les anciennes données Teddiursa ou Ursaring niv. 29 sont à réconcilier dans la source canonique, les données moteur, les tests et le wiki.

## Déblocage de la Méga-Évolution joueur

Le **Méga-Anneau / Mega Ring** devient accessible **immédiatement après l'obtention du badge 4 auprès de Mortimer**. L'ordre de récompense attendu dans le script de l'arène est :

1. victoire contre Mortimer ;
2. remise du badge 4 et fanfare de badge ;
3. remise de la CT de Mortimer ;
4. remise du **Méga-Anneau** avec le jingle et le texte standard d'obtention d'un objet clé.

À partir de cet instant, le joueur doit pouvoir utiliser la Méga-Évolution dès ses combats suivants, **y compris entre le badge 4 et le badge 5**. Il ne faut donc aucune condition supplémentaire liée au badge 5, à Chuck ou à un événement postérieur.

Le moteur actuel de `CanMegaEvolve()` autorise la Méga côté joueur si `ITEM_MEGA_RING` est présent dans le Sac ; le script de Mortimer constitue donc le jalon de progression attendu. Les dresseurs adverses peuvent utiliser leurs Méga avant que le joueur possède le Méga-Anneau lorsque leur combat le prévoit.
