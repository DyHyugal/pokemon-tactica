# Starters, rival et évolutions

`data/spec/starters.json` énumère les 30 starters et Évoli, les boosters de type et les objets supplémentaires extraits du code validé. Le même sélecteur sert au starter initial et à l'œuf d'Orme. Évoli reçoit une préférence d'évolution et les objets prévus ; vérifier la livraison de tous les objets même si le sac est plein. **Le second choix via l'œuf ne peut pas être exactement le même starter que le choix initial** : l'UI doit l'exclure et la logique doit refuser le doublon si elle est appelée directement. Les évolutions sans échange et les seuils d'évolution par objet restent ceux de l'implémentation testée ; les auditer avant de recopier des chiffres d'anciens documents.

Le sélecteur de starter est confirmé fonctionnel en ROM. L'amélioration restante est du **polish UX** : lorsqu'un joueur revient à l'écran précédent puis rouvre une liste, le curseur doit revenir sur la dernière ligne sélectionnée plutôt que repartir au début.

## Tirage du rival

Évoli est exclu. Au choix initial, déterminer la **catégorie de départ** du joueur, tirer uniformément une catégorie parmi celles ci-dessous, puis tirer uniformément un des cinq starters de cette catégorie. Enregistrer le résultat pour que le starter du rival reste le même pendant toute la partie. Se baser sur cette matrice de catégories approuvée, et pas uniquement sur un contre-type unique en dur :

| Joueur | Catégories candidates du rival |
|---|---|
| Feu | Eau, Sol |
| Eau | Plante, Électrik |
| Plante | Feu, Glace |
| Électrik | Sol |
| Sol | Eau, Plante, Glace |
| Glace | Feu |

Le type secondaire actuel ou une éventuelle évolution custom d'un starter ne redéfinit pas rétroactivement sa catégorie de sélection. Vérifier les tirages, la persistance de la sauvegarde et les rares cas de changement de choix initial.

## Équipes du rival

L'archétype permanent dépend de **la catégorie tirée pour le rival** : Eau → Pluie ; Feu → Soleil ; Plante → Champ Herbu ; Électrik → Champ Électrifié ; Sol → Tempête de sable ; Glace → Neige. Les premières rencontres ont une version simple ; les suivantes développent la même stratégie et respectent les caps et la légalité des évolutions. **Le profil de niveaux du rival suit le prochain boss/Champion obligatoire**, et non les niveaux historiques du combat importé. Exemple : après le badge 2, le rival qui précède Blanche doit utiliser le profil de Blanche, avec membres autour de 29 et ace à 32. Les six propositions de rosters et leurs sets EARLY/MID/FINAL figurent dans `data/spec/rival.json` (`fight_rosters`) et dans [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md). Leur intégration moteur doit préserver cette règle de niveau.
