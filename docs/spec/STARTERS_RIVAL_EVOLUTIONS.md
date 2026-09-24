# Starters, rival et évolutions

`data/spec/starters.json` énumère les 30 starters et Évoli, les boosters de type et les objets supplémentaires extraits du code validé. Le même sélecteur sert au starter initial et à l'œuf d'Orme. Évoli reçoit une préférence d'évolution et les objets prévus ; vérifier la livraison de tous les objets même si le sac est plein. Les évolutions sans échange et les seuils d'évolution par objet restent ceux de l'implémentation testée ; les auditer avant de recopier des chiffres d'anciens documents.

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

L'archétype permanent dépend de **la catégorie tirée pour le rival** : Eau → Pluie ; Feu → Soleil ; Plante → Champ Herbu ; Électrik → Champ Électrifié ; Sol → Tempête de sable ; Glace → Neige. Les premières rencontres ont une version simple ; les suivantes développent la même stratégie et respectent les caps et la légalité des évolutions. Le roster précis de chaque combat **n'a pas encore été fourni** : `data/spec/rival.json` fixe l'algorithme et les thèmes, puis Codex doit inventorier les combats existants et proposer les équipes manquantes pour validation, sans inventer une liste canonique.
