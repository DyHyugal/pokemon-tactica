# Starters, rival et évolutions

`data/spec/starters.json` énumère les 30 starters et Évoli, les boosters de type et les objets supplémentaires. Le même sélecteur sert au starter initial et à l'œuf d'Orme. Évoli reçoit une préférence d'évolution et les objets prévus ; vérifier la livraison même si le sac est plein. Les évolutions sans échange restent celles de l'implémentation testée.

## UX du sélecteur

Le cœur du sélecteur est protégé, mais son comportement de curseur fait partie du contrat V1 :

- première ouverture d'une liste d'espèces : curseur sur la **première entrée** ;
- `Retour` est toujours la dernière entrée et ne devient jamais la sélection initiale ;
- après avoir choisi une espèce puis annulé sa confirmation, revenir sur **exactement cette espèce** ;
- tester explicitement Élekid, qui a été observé revenir sur `Retour` alors que d'autres espèces revenaient correctement ;
- l'œuf d'Orme ne peut pas redonner exactement le starter principal.

## Tirage du rival

Évoli est exclu. Au choix initial, déterminer la catégorie de départ du joueur, tirer uniformément une catégorie parmi celles ci-dessous, puis tirer uniformément un des cinq starters de cette catégorie. Enregistrer le résultat pour que le starter du rival reste le même pendant toute la partie.

| Joueur | Catégories candidates du rival |
|---|---|
| Feu | Eau, Sol |
| Eau | Plante, Électrik |
| Plante | Feu, Glace |
| Électrik | Sol |
| Sol | Eau, Plante, Glace |
| Glace | Feu |

Le type secondaire actuel ou une évolution custom d'un starter ne redéfinit pas rétroactivement sa catégorie de sélection.

## Équipes et niveaux du rival

L'archétype permanent dépend de la catégorie tirée : Eau → Pluie ; Feu → Soleil ; Plante → Champ Herbu ; Électrik → Champ Électrifié ; Sol → Tempête de sable ; Glace → Neige.

Les six rosters thématiques sont maintenant câblés depuis `data/spec/rival.json` / `src/data/tactica_rival.h`, mais les niveaux de chaque rencontre doivent suivre le jalon de préparation canonique.

### Premier combat

Contrat owner 26-09 :

- taille d'équipe : **1** ;
- contenu : **starter rival uniquement** ;
- niveau : **17**, soit le cap de préparation d'Albert ;
- stade d'évolution : résoudre légalement l'espèce au niveau 17 ;
- attaques : légales pour ce stade et ce niveau ;
- NORMAL/HARD gardent le même contenu.

Le template historique niveau 5 est donc un écart à corriger ; il ne doit pas être conservé au motif que le combat original HGSS était niveau 5.

### Combats suivants

Les tailles progressives restent celles définies dans `data/spec/rival.json` et les niveaux suivent le prochain jalon obligatoire de la progression. Les espèces, évolutions, capacités, objets, natures et EV HARD doivent être résolus au niveau réel du duel puis vérifiés en ROM.
